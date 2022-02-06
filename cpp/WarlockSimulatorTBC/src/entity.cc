#include "../include/entity.h"

#include "../include/bindings.h"
#include "../include/common.h"
#include "../include/sets.h"
#include "../include/simulation.h"

Entity::Entity(Player* player, PlayerSettings& player_settings, EntityType entity_type)
    : player(player),
      entity_type(entity_type),
      stats(entity_type == EntityType::kPlayer ? player_settings.stats : CharacterStats()),
      settings(player_settings),
      recording_combat_log_breakdown(player_settings.recording_combat_log_breakdown &&
                                     player_settings.equipped_item_simulation),
      equipped_item_simulation(player_settings.equipped_item_simulation),
      enemy_shadow_resist(player_settings.enemy_shadow_resist),
      enemy_fire_resist(player_settings.enemy_fire_resist),
      // I don't know if this formula only works for bosses or not, so for the
      // moment I'm only using it for targets 3+ levels above.
      enemy_level_difference_resistance(player_settings.enemy_level >= (kLevel + 3) ? (6 * kLevel * 5) / 75 : 0) {
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
    const int kCritChance = 3;

    stats.spell_crit_chance += kCritChance;
    stats.melee_crit_chance += kCritChance;
  }
  if (player_settings.auras.totem_of_wrath) {
    stats.spell_crit_chance += (3 * settings.totem_of_wrath_amount);
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
    stats.extra_spell_hit_chance += (3 * settings.totem_of_wrath_amount);
  }
  if (player_settings.auras.inspiring_presence) {
    stats.extra_spell_hit_chance++;
  }
  stats.spell_hit_chance = GetBaseSpellHitChance(kLevel, settings.enemy_level);

  if (player_settings.auras.curse_of_the_elements) {
    const double kDamageModifier = 1.1 + (0.01 * settings.improved_curse_of_the_elements);

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

    int improved_imp_points = settings.improved_imp;

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

void Entity::PostIterationDamageAndMana(const std::string& spell_name) {
  PostCombatLogBreakdownVector(spell_name.c_str(), combat_log_breakdown.at(spell_name)->iteration_mana_gain,
                               combat_log_breakdown.at(spell_name)->iteration_damage);
  combat_log_breakdown.at(spell_name)->iteration_damage = 0;
  combat_log_breakdown.at(spell_name)->iteration_mana_gain = 0;
}

void Entity::EndAuras() {
  for (auto& aura : aura_list) {
    if (aura->active) {
      aura->Fade();
    }
  }
}

void Entity::Reset() {
  cast_time_remaining = 0;
  gcd_remaining = 0;
  mp5_timer_remaining = 5;
  five_second_rule_timer_remaining = 5;

  for (auto& spell : spell_list) {
    spell->Reset();
  }
}

void Entity::Initialize(Simulation* simulationPtr) { simulation = simulationPtr; }

void Entity::SendCombatLogBreakdown() {
  for (auto& breakdown : combat_log_breakdown) {
    if (breakdown.second->iteration_damage > 0 || breakdown.second->iteration_mana_gain > 0) {
      PostIterationDamageAndMana(breakdown.first);
    }

    PostCombatLogBreakdown(breakdown.second->name.c_str(), breakdown.second->casts, breakdown.second->crits,
                           breakdown.second->misses, breakdown.second->count, breakdown.second->uptime,
                           breakdown.second->dodge, breakdown.second->glancing_blows);
  }
}

double Entity::GetStamina() { return stats.stamina * stats.stamina_modifier; }

double Entity::GetIntellect() { return stats.intellect * stats.intellect_modifier; }

double Entity::GetSpirit() { return stats.spirit * stats.spirit_modifier; }

bool Entity::ShouldWriteToCombatLog() { return simulation->iteration == 10 && equipped_item_simulation; }

void Entity::CombatLog(const std::string& entry) {
  player->combat_log_entries.push_back("|" + DoubleToString(simulation->current_fight_time, 4) + "| " + entry);
}

double Entity::GetMultiplicativeDamageModifier(Spell& spell, bool) {
  auto damage_modifier = stats.damage_modifier;

  if (spell.spell_school == SpellSchool::kShadow) {
    damage_modifier *= stats.shadow_modifier;

    if (!settings.using_custom_isb_uptime && auras.improved_shadow_bolt != NULL && auras.improved_shadow_bolt->active) {
      damage_modifier *= auras.improved_shadow_bolt->modifier;
    }
  } else if (spell.spell_school == SpellSchool::kFire) {
    damage_modifier *= stats.fire_modifier;
  }

  return damage_modifier;
}

double Entity::FindTimeUntilNextAction() {
  double time = cast_time_remaining;

  if (time <= 0) {
    time = gcd_remaining;
  }

  if (time <= 0 || (mp5_timer_remaining < time && mp5_timer_remaining > 0)) {
    time = mp5_timer_remaining;
  }

  if (five_second_rule_timer_remaining > 0 && five_second_rule_timer_remaining < time) {
    time = five_second_rule_timer_remaining;
  }

  for (auto& spell : spell_list) {
    if (spell->cooldown_remaining < time && spell->cooldown_remaining > 0) {
      time = spell->cooldown_remaining;
    }
  }

  for (auto& aura : aura_list) {
    if (aura->active && aura->has_duration && aura->duration_remaining < time) {
      time = aura->duration_remaining;
    }
  }

  // TODO make DamageOverTime inherit from Aura and combine dot_list and aura_list
  for (auto& dot : dot_list) {
    if (dot->active && dot->tick_timer_remaining < time) {
      time = dot->tick_timer_remaining;
    }
  }

  return time;
}

double Entity::GetPartialResistMultiplier(SpellSchool school) {
  int enemy_resist = 0;

  if (school == SpellSchool::kShadow) {
    enemy_resist = settings.enemy_shadow_resist;
  } else if (school == SpellSchool::kFire) {
    enemy_resist = settings.enemy_fire_resist;
  }

  enemy_resist = std::max(enemy_resist - static_cast<int>(stats.spell_penetration), enemy_level_difference_resistance);

  if (enemy_resist <= 0) {
    return 1;
  }

  return 1.0 - ((75.0 * enemy_resist) / (kLevel * 5)) / 100.0;
}

double Entity::GetGcdValue() { return std::max(kMinimumGcdValue, kGcdValue / GetHastePercent()); }

double Entity::GetSpellHitChance(SpellType spell_type) {
  double spell_hit_chance = stats.spell_hit_chance + stats.extra_spell_hit_chance;

  if (entity_type == EntityType::kPlayer && spell_type == SpellType::kAffliction) {
    spell_hit_chance += player->talents.suppression * 2;
  }

  return std::min(99.0, spell_hit_chance);
}

// formula from
// https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/
// && https://royalgiraffe.github.io/resist-guide
double Entity::GetBaseSpellHitChance(int entity_level, int enemy_level) {
  const int kLevelDifference = enemy_level - entity_level;

  if (kLevelDifference <= 2) {
    return std::min(99, 100 - kLevelDifference - 4);
  } else if (kLevelDifference == 3) {
    return 83;
  } else {
    return 83 - 11 * kLevelDifference;
  }
}

bool Entity::IsSpellCrit(SpellType spell_type, double extra_crit) {
  return player->RollRng(GetSpellCritChance(spell_type) + extra_crit);
}

bool Entity::IsMeleeCrit() { return player->RollRng(GetMeleeCritChance()); }

bool Entity::IsMeleeHit() { return player->RollRng(stats.melee_hit_chance); }

bool Entity::IsSpellHit(SpellType spell_type) { return player->RollRng(GetSpellHitChance(spell_type)); }

double Entity::GetMeleeCritChance() {
  return pet->GetAgility() * 0.04 + 0.65 + stats.melee_crit_chance - StatConstant::kMeleeCritChanceSuppression;
}

double Entity::GetCustomImprovedShadowBoltDamageModifier() {
  return 1 + 0.2 * (settings.custom_isb_uptime_value / 100.0);
}

void Entity::Tick(double time) {
  cast_time_remaining -= time;
  gcd_remaining -= time;
  five_second_rule_timer_remaining -= time;
  mp5_timer_remaining -= time;

  // Auras need to tick before Spells because otherwise you'll, for example,
  // finish casting Corruption and then immediately afterwards, in the same
  // millisecond, immediately tick down the aura This was also causing buffs like
  // e.g. the t4 4pc buffs to expire sooner than they should.
  for (auto& aura : aura_list) {
    if (aura->active && aura->duration_remaining > 0) {
      aura->Tick(time);
    }
  }

  for (auto& dot : dot_list) {
    if (dot->active && dot->tick_timer_remaining > 0) {
      dot->Tick(time);
    }
  }

  // TLC needs to tick before other spells because otherwise a spell might proc TLC and then later in the same loop,
  // during the same millisecond of the fight, tick down TLC's cooldown
  // TODO find a better solution for this since this might occur for other spells as well.
  if (spells.the_lightning_capacitor != NULL && spells.the_lightning_capacitor->cooldown_remaining > 0) {
    spells.the_lightning_capacitor->Tick(time);
  }

  for (auto& spell : spell_list) {
    if (spell->name != SpellName::kTheLightningCapacitor && (spell->cooldown_remaining > 0 || spell->casting)) {
      spell->Tick(time);
    }
  }
}