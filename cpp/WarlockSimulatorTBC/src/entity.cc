#include "../include/entity.h"

#include "../include/player_settings.h"
#include "../include/combat_log_breakdown.h"
#include "../include/aura.h"
#include "../include/spell.h"
#include "../include/player.h"
#include "../include/simulation.h"
#include "../include/talents.h"
#include "../include/pet.h"
#include "../include/damage_over_time.h"
#include "../include/common.h"
#include "../include/bindings.h"
#include "../include/aura_selection.h"
#include "../include/sets.h"

Entity::Entity(Player* player, PlayerSettings& player_settings, EntityType entity_type)
  : player(player),
    settings(player_settings),
    stats(entity_type == EntityType::kPlayer ? player_settings.stats : CharacterStats()),
    entity_type(entity_type),
    recording_combat_log_breakdown(player_settings.recording_combat_log_breakdown &&
                                   player_settings.equipped_item_simulation),
    equipped_item_simulation(player_settings.equipped_item_simulation),
    enemy_shadow_resist(player_settings.enemy_shadow_resist),
    enemy_fire_resist(player_settings.enemy_fire_resist),
    // I don't know if this formula only works for bosses or not, so for the
    // moment I'm only using it for targets 3+ levels above.
    enemy_level_difference_resistance(player_settings.enemy_level >= kLevel + 3 ? 6 * kLevel * 5 / 75 : 0) {
  // Crit chance
  if (entity_type == EntityType::kPlayer) {
    if (player_settings.auras.atiesh_mage) {
      stats.spell_crit_rating += 28 * settings.mage_atiesh_amount;
    }

    stats.spell_crit_chance = StatConstant::kBaseCritChancePercent + player_settings.talents.devastation +
                              player_settings.talents.backlash + player_settings.talents.demonic_tactics;
  }
  if (player_settings.auras.moonkin_aura) {
    stats.spell_crit_chance += 5;
  }
  if (player_settings.auras.judgement_of_the_crusader) {
    constexpr int kCritChance = 3;

    stats.spell_crit_chance += kCritChance;
    stats.melee_crit_chance += kCritChance;
  }
  if (player_settings.auras.totem_of_wrath) {
    stats.spell_crit_chance += 3 * settings.totem_of_wrath_amount;
  }
  if (player_settings.auras.chain_of_the_twilight_owl) {
    stats.spell_crit_chance += 2;
  }

  // Hit chance
  if (entity_type == EntityType::kPlayer) {
    if (player_settings.sets.mana_etched >= 2) {
      stats.spell_hit_rating += 35;
    }

    stats.extra_spell_hit_chance = stats.spell_hit_rating / StatConstant::kHitRatingPerPercent;
  }
  if (player_settings.auras.totem_of_wrath) {
    stats.extra_spell_hit_chance += 3 * settings.totem_of_wrath_amount;
  }
  if (player_settings.auras.inspiring_presence) {
    stats.extra_spell_hit_chance++;
  }
  stats.spell_hit_chance = GetBaseSpellHitChance(kLevel, settings.enemy_level);

  if (player_settings.auras.curse_of_the_elements) {
    const double kDamageModifier = 1.1 + 0.01 * settings.improved_curse_of_the_elements;

    stats.shadow_modifier *= kDamageModifier;
    stats.fire_modifier *= kDamageModifier;
  }

  if (player_settings.auras.ferocious_inspiration) {
    stats.damage_modifier *= std::pow(1.03, settings.ferocious_inspiration_amount);
  }

  if ((player_settings.talents.demonic_sacrifice == 0 || !settings.sacrificing_pet) &&
      player_settings.talents.soul_link == 1) {
    stats.damage_modifier *= 1.05;
  }

  if (settings.using_custom_isb_uptime) {
    stats.shadow_modifier *= GetCustomImprovedShadowBoltDamageModifier();
  }

  if (player_settings.auras.wrath_of_air_totem && settings.has_elemental_shaman_t4_bonus) {
    stats.spell_power += 20;
  }

  if (player_settings.auras.blood_pact) {
    if (entity_type == EntityType::kPet) {
      // Only add 70 stam if it's a pet since it's added to the player's stats in the client.
      stats.stamina += 70;
    }

    auto improved_imp_points = settings.improved_imp;

    if (settings.selected_pet == EmbindConstant::kImp &&
        (!settings.sacrificing_pet || player_settings.talents.demonic_sacrifice == 0) &&
        player_settings.talents.improved_imp > improved_imp_points) {
      improved_imp_points = player_settings.talents.improved_imp;
    }

    stats.stamina += 70 * 0.1 * improved_imp_points;
  }

  // Add mp5 from Vampiric Touch (add 25% instead of 5% since we're adding it to
  // the mana per 5 seconds variable)
  if (player_settings.auras.vampiric_touch) {
    stats.mp5 += settings.shadow_priest_dps * 0.25;
  }

  if (player_settings.auras.atiesh_warlock) {
    stats.spell_power += 33 * settings.warlock_atiesh_amount;
  }
}

void Entity::PostIterationDamageAndMana(const std::string& kSpellName) const {
  PostCombatLogBreakdownVector(kSpellName.c_str(), combat_log_breakdown.at(kSpellName)->iteration_mana_gain,
                               combat_log_breakdown.at(kSpellName)->iteration_damage);
  combat_log_breakdown.at(kSpellName)->iteration_damage = 0;
  combat_log_breakdown.at(kSpellName)->iteration_mana_gain = 0;
}

void Entity::EndAuras() {
  for (const auto& kAura : aura_list) {
    if (kAura->active) {
      kAura->Fade();
    }
  }
}

void Entity::Reset() {
  cast_time_remaining = 0;
  gcd_remaining = 0;
  mp5_timer_remaining = 5;
  five_second_rule_timer_remaining = 5;

  for (const auto& kSpell : spell_list) {
    kSpell->Reset();
  }
}

void Entity::Initialize(Simulation* simulation_ptr) { simulation = simulation_ptr; }

void Entity::SendCombatLogBreakdown() const {
  for (const auto& [kSpellName, kSpell] : combat_log_breakdown) {
    if (kSpell->iteration_damage > 0 || kSpell->iteration_mana_gain > 0) {
      PostIterationDamageAndMana(kSpellName);
    }

    PostCombatLogBreakdown(kSpell->name.c_str(), kSpell->casts, kSpell->crits,
                           kSpell->misses, kSpell->count, kSpell->uptime,
                           kSpell->dodge, kSpell->glancing_blows);
  }
}

double Entity::GetStamina() { return stats.stamina * stats.stamina_modifier; }

double Entity::GetIntellect() { return stats.intellect * stats.intellect_modifier; }

double Entity::GetSpirit() const { return stats.spirit * stats.spirit_modifier; }

bool Entity::ShouldWriteToCombatLog() const { return simulation->iteration == 10 && equipped_item_simulation; }

void Entity::CombatLog(const std::string& kEntry) const {
  player->combat_log_entries.push_back("|" + DoubleToString(simulation->current_fight_time, 4) + "| " + kEntry);
}

double Entity::GetMultiplicativeDamageModifier(const Spell& kSpell, bool) const {
  auto damage_modifier = stats.damage_modifier;

  if (kSpell.spell_school == SpellSchool::kShadow) {
    damage_modifier *= stats.shadow_modifier;

    if (!settings.using_custom_isb_uptime && auras.improved_shadow_bolt != nullptr && auras.improved_shadow_bolt->active) {
      damage_modifier *= auras.improved_shadow_bolt->modifier;
    }
  } else if (kSpell.spell_school == SpellSchool::kFire) {
    damage_modifier *= stats.fire_modifier;
  }

  return damage_modifier;
}

double Entity::FindTimeUntilNextAction() {
  auto time = cast_time_remaining;

  if (time <= 0) {
    time = gcd_remaining;
  }

  if (time <= 0 || mp5_timer_remaining < time && mp5_timer_remaining > 0) {
    time = mp5_timer_remaining;
  }

  if (five_second_rule_timer_remaining > 0 && five_second_rule_timer_remaining < time) {
    time = five_second_rule_timer_remaining;
  }

  for (const auto& kSpell : spell_list) {
    if (kSpell->cooldown_remaining < time && kSpell->cooldown_remaining > 0) {
      time = kSpell->cooldown_remaining;
    }
  }

  for (const auto& kAura : aura_list) {
    if (kAura->active && kAura->has_duration) {
      if (kAura->duration_remaining < time) {
        time = kAura->duration_remaining;
      }

      if (kAura->tick_timer_remaining > 0 && kAura->tick_timer_remaining < time) {
        time = kAura->tick_timer_remaining;
      }
    }
  }

  // TODO make DamageOverTime inherit from Aura and combine dot_list and aura_list
  for (const auto& kDot : dot_list) {
    if (kDot->active && kDot->tick_timer_remaining < time) {
      time = kDot->tick_timer_remaining;
    }
  }

  return time;
}

double Entity::GetPartialResistMultiplier(const SpellSchool kSchool) const {
  auto enemy_resist = 0;

  if (kSchool == SpellSchool::kShadow) {
    enemy_resist = settings.enemy_shadow_resist;
  } else if (kSchool == SpellSchool::kFire) {
    enemy_resist = settings.enemy_fire_resist;
  }

  enemy_resist = std::max(enemy_resist - static_cast<int>(stats.spell_penetration), enemy_level_difference_resistance);

  if (enemy_resist <= 0) {
    return 1;
  }

  return 1.0 - 75.0 * enemy_resist / (kLevel * 5) / 100.0;
}

double Entity::GetGcdValue() { return std::max(kMinimumGcdValue, kGcdValue / GetHastePercent()); }

double Entity::GetSpellHitChance(const SpellType kSpellType) const {
  auto spell_hit_chance = stats.spell_hit_chance + stats.extra_spell_hit_chance;

  if (entity_type == EntityType::kPlayer && kSpellType == SpellType::kAffliction) {
    spell_hit_chance += player->talents.suppression * 2;
  }

  return std::min(99.0, spell_hit_chance);
}

// formula from
// https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/
// && https://royalgiraffe.github.io/resist-guide
double Entity::GetBaseSpellHitChance(const int kEntityLevel, const int kEnemyLevel) const {

  if (const int kLevelDifference = kEnemyLevel - kEntityLevel; kLevelDifference <= 2) {
    return std::min(99, 100 - kLevelDifference - 4);
  } else {
    if (kLevelDifference == 3) {
      return 83;
    }

    return 83 - 11 * kLevelDifference;
  }
}

bool Entity::IsSpellCrit(const SpellType kSpellType, const double kExtraCrit) {
  return player->RollRng(GetSpellCritChance(kSpellType) + kExtraCrit);
}

bool Entity::IsMeleeCrit() { return player->RollRng(GetMeleeCritChance()); }

bool Entity::IsMeleeHit() { return player->RollRng(stats.melee_hit_chance); }

bool Entity::IsSpellHit(const SpellType kSpellType) { return player->RollRng(GetSpellHitChance(kSpellType)); }

double Entity::GetMeleeCritChance() const {
  return pet->GetAgility() * 0.04 + 0.65 + stats.melee_crit_chance - StatConstant::kMeleeCritChanceSuppression;
}

double Entity::GetCustomImprovedShadowBoltDamageModifier() const {
  return 1 + 0.2 * (settings.custom_isb_uptime_value / 100.0);
}

void Entity::Tick(const double kTime) {
  cast_time_remaining -= kTime;
  gcd_remaining -= kTime;
  five_second_rule_timer_remaining -= kTime;
  mp5_timer_remaining -= kTime;

  // Auras need to tick before Spells because otherwise you'll, for example,
  // finish casting Corruption and then immediately afterwards, in the same
  // millisecond, immediately tick down the aura This was also causing buffs like
  // e.g. the t4 4pc buffs to expire sooner than they should.
  for (const auto& kAura : aura_list) {
    if (kAura->active && kAura->duration_remaining > 0) {
      kAura->Tick(kTime);
    }
  }

  for (const auto& kDot : dot_list) {
    if (kDot->active && kDot->tick_timer_remaining > 0) {
      kDot->Tick(kTime);
    }
  }

  // TLC needs to tick before other spells because otherwise a spell might proc TLC and then later in the same loop,
  // during the same millisecond of the fight, tick down TLC's cooldown
  // TODO find a better solution for this since this might occur for other spells as well.
  if (spells.the_lightning_capacitor != nullptr && spells.the_lightning_capacitor->cooldown_remaining > 0) {
    spells.the_lightning_capacitor->Tick(kTime);
  }

  for (const auto& kSpell : spell_list) {
    if (kSpell->name != SpellName::kTheLightningCapacitor && (kSpell->cooldown_remaining > 0 || kSpell->casting)) {
      kSpell->Tick(kTime);
    }
  }
}