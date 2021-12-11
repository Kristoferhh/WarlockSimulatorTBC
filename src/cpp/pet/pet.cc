#include "pet.h"

#include "../bindings.h"
#include "../common.h"
#include "../player/player.h"

Pet::Pet(Player& player)
    : Entity(player, player.settings, EntityType::kPet),
      spells(PetSpells()),
      auras(PetAuras()),
      glancing_blow_multiplier(1 - (0.1 + (player.settings.enemy_level * 5 - kLevel * 5) / 100.0)),
      glancing_blow_chance(std::max(0.0, 6 + (player.settings.enemy_level * 5 - kLevel * 5) * 1.2)),
      base_melee_speed(2) {}

void Pet::Initialize(Simulation* simulationPtr) {
  simulation = simulationPtr;
  Setup();

  if (pet_name == PetName::kImp) {
    spells.firebolt = std::make_unique<ImpFirebolt>(*this);
  } else {
    spells.melee = std::make_unique<Melee>(*this);

    if (pet_name == PetName::kSuccubus) {
      spells.lash_of_pain = std::make_unique<SuccubusLashOfPain>(*this);
    } else if (pet_name == PetName::kFelguard) {
      spells.cleave = std::make_unique<FelguardCleave>(*this);
      auras.demonic_frenzy = std::make_unique<DemonicFrenzy>(*this);
    }

    if (player.selected_auras.pet_battle_squawk) {
      auras.battle_squawk = std::make_unique<BattleSquawk>(*this);
    }
  }

  if (player.settings.prepop_black_book) {
    auras.black_book = std::make_unique<BlackBook>(*this);
  }
}

void Pet::CalculateStatsFromAuras() {
  // Calculate melee hit chance
  // Formula from https://wowwiki-archive.fandom.com/wiki/Hit?oldid=1584399
  const int kLevelDifference = player.settings.enemy_level - player.kLevel;
  stats.melee_hit_chance = 100 - (kLevelDifference <= 2 ? 5 + kLevelDifference * 0.5 : 7 + (kLevelDifference - 2) * 2);

  // Calculate spell hit chance
  // Formula from
  // https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/
  stats.spell_hit_chance = kLevelDifference <= 2   ? std::min(99, 100 - kLevelDifference - 4)
                           : kLevelDifference == 3 ? 83
                           : kLevelDifference == 4 ? 72
                                                   : 61;

  // Auras
  if (player.selected_auras.pet_blessing_of_kings) {
    stats.stamina_modifier *= 1.1;
    stats.intellect_modifier *= 1.1;
    stats.agility_modifier *= 1.1;
    stats.strength_modifier *= 1.1;
    stats.spirit_modifier *= 1.1;
  }
  if (player.selected_auras.pet_blessing_of_wisdom) {
    stats.mp5 += 41;
  }
  if (player.selected_auras.mana_spring_totem) {
    stats.mp5 += 50;
  }
  if (player.selected_auras.wrath_of_air_totem) {
    stats.spell_power += 101;
  }
  if (player.selected_auras.totem_of_wrath) {
    const int kHitAndCritAmount = player.settings.totem_of_wrath_amount * 3;

    stats.spell_crit_chance += kHitAndCritAmount;
    stats.spell_hit_chance = std::min(99.0, stats.spell_hit_chance + kHitAndCritAmount);
  }

  stats.spell_crit_chance += player.talents.demonic_tactics;
  stats.melee_crit_chance += player.talents.demonic_tactics;
  stats.attack_power += GetDebuffAttackPower();

  // todo improved motw
  if (player.selected_auras.pet_mark_of_the_wild) {
    stats.stamina += 14;
    stats.intellect += 14;
    stats.strength += 14;
    stats.agility += 14;
    stats.spirit += 14;
  }

  // todo improved imp
  if (player.selected_auras.blood_pact) {
    stats.stamina += 70;
  }
  if (player.selected_auras.pet_arcane_intellect) {
    stats.intellect += 40;
  }
  if (player.selected_auras.pet_prayer_of_fortitude) {
    stats.stamina += 79;
  }
  if (player.selected_auras.pet_prayer_of_spirit) {
    stats.spirit += 50 * (1 + (0.1 * player.settings.improved_divine_spirit));
  }
  if (player.selected_auras.inspiring_presence) {
    stats.spell_hit_chance = std::min(99.0, stats.spell_hit_chance + 1);
  }
  if (player.selected_auras.moonkin_aura) {
    stats.spell_crit_chance += 5;
  }
  if (player.selected_auras.eye_of_the_night) {
    stats.spell_power += 34;
  }
  if (player.selected_auras.chain_of_the_twilight_owl) {
    stats.spell_crit_chance += 2;
  }
  if (player.selected_auras.jade_pendant_of_blasting) {
    stats.spell_power += 15;
  }
  // Atiesh auras
  // Add 33sp if the player has Atiesh equipped since the aura's spell power is
  // just added to the item itself
  if (player.items.two_hand == 22630) {
    stats.spell_power += 33;
  }
  if (player.selected_auras.atiesh_warlock) {
    stats.spell_power += 33 * player.settings.warlock_atiesh_amount;
  }
  if (player.selected_auras.wrath_of_air_totem && player.settings.has_elemental_shaman_t4_bonus) {
    stats.spell_power += 20;
  }
  if (player.selected_auras.judgement_of_the_crusader) {
    stats.melee_crit_chance += 3;
    stats.spell_crit_chance += 3;
  }
  // Multiply by 25% instead of 5% since it's happening every 5 seconds instead
  // of every 1 second
  if (player.selected_auras.vampiric_touch) {
    stats.mp5 += player.settings.shadow_priest_dps * 0.25;
  }
  if (player.selected_auras.faerie_fire && player.settings.improved_faerie_fire) {
    stats.melee_hit_chance += 3;
  }
  if (player.selected_auras.pet_heroic_presence) {
    stats.melee_hit_chance++;
  }
  if (player.selected_auras.pet_blessing_of_might) {
    stats.attack_power += 220;
  }
  if (player.selected_auras.pet_strength_of_earth_totem) {
    stats.strength += 86;
  }
  if (player.selected_auras.pet_grace_of_air_totem) {
    stats.agility += 67;
  }
  if (player.selected_auras.pet_battle_shout) {
    stats.attack_power += 306;
  }
  if (player.selected_auras.pet_trueshot_aura) {
    stats.attack_power += 300;
  }
  if (player.selected_auras.pet_leader_of_the_pack) {
    stats.melee_crit_chance += 5;
  }
  if (player.selected_auras.pet_unleashed_rage) {
    stats.attack_power_modifier *= 1.1;
  }
  if (player.selected_auras.pet_stamina_scroll) {
    stats.stamina += 20;
  }
  if (player.selected_auras.pet_intellect_scroll) {
    stats.intellect += 20;
  }
  if (player.selected_auras.pet_strength_scroll) {
    stats.strength += 20;
  }
  if (player.selected_auras.pet_agility_scroll) {
    stats.agility += 20;
  }
  if (player.selected_auras.pet_spirit_scroll) {
    stats.spirit += 20;
  }
  if (player.selected_auras.pet_kiblers_bits) {
    stats.strength += 20;
    stats.spirit += 20;
  }
  if (player.selected_auras.ferocious_inspiration) {
    stats.damage_modifier *= std::pow(1.03, player.settings.ferocious_inspiration_amount);
  }
  if (player.settings.race == EmbindConstant::kOrc) {
    stats.damage_modifier *= 1.05;
  }

  // Hidden attack power modifiers (source: Max on warlock discord)
  stats.attack_power_modifier *= (pet_name == PetName::kFelguard ? 1.1 : pet_name == PetName::kSuccubus ? 1.05 : 1);

  // Calculate armor
  if (pet_type == PetType::kMelee) {
    // Formula from
    // https://wowwiki-archive.fandom.com/wiki/Damage_reduction?oldid=807810
    enemy_damage_reduction_from_armor =
        std::max(0.25, player.settings.enemy_level >= 60
                           ? 1 - player.settings.enemy_armor /
                                     (player.settings.enemy_armor - 22167.5 + 467.5 * player.settings.enemy_level)
                           : 1 - player.settings.enemy_armor /
                                     (player.settings.enemy_armor + 400 + 85 * player.settings.enemy_level));
  }

  stats.stamina_modifier *= 1 + (0.05 * player.talents.fel_stamina);
  stats.intellect_modifier *= 1 + (0.05 * player.talents.fel_intellect);
  stats.damage_modifier *= (1 + 0.04 * player.talents.unholy_power) * (1 + 0.05 * player.talents.soul_link);

  stats.spell_hit_chance = std::min(99.0, stats.spell_hit_chance);
  stats.max_mana = CalculateMaxMana();
}

double Pet::GetPlayerSpellPower() {
  return player.GetSpellPower(false) + std::max(player.stats.shadow_power, player.stats.fire_power);
}

double Pet::GetStamina() { return (stats.stamina + 0.3 * player.GetStamina()) * stats.stamina_modifier; }

double Pet::GetIntellect() { return (stats.intellect + 0.3 * player.GetIntellect()) * stats.intellect_modifier; }

double Pet::GetSpellPower() { return stats.spell_power + GetPlayerSpellPower() * 0.15; }

double Pet::CalculateMaxMana() {
  return GetIntellect() * (pet_type == PetType::kMelee ? 11.555
                           : pet_name == PetName::kImp ? 4.95
                                                       : 0) +
         (pet_name == PetName::kImp        ? 756
          : pet_name == PetName::kSuccubus ? 849
          : pet_name == PetName::kFelguard ? 893
                                           : 0);
}

void Pet::Setup() { CalculateStatsFromAuras(); }

void Pet::Reset() {
  stats.mana = CalculateMaxMana();
  five_second_rule_timer_remaining = 5;
  mp5_timer_remaining = 2;
  cast_time_remaining = 0;

  for (auto& spell : spell_list) {
    spell->Reset();
  }
}

void Pet::EndAuras() {
  for (auto& aura : aura_list) {
    if (aura->active) {
      aura->Fade();
    }
  }
}

double Pet::GetMeleeCritChance() {
  return GetAgility() * 0.04 + 0.65 + stats.melee_crit_chance - StatConstant::kMeleeCritChanceSuppression;
}

double Pet::GetSpellCritChance() { return (0.0125 * GetIntellect()) + 0.91 + stats.spell_crit_chance; }

bool Pet::IsCrit(AttackType type) {
  return player.RollRng(type == AttackType::kPhysical  ? GetMeleeCritChance()
                        : type == AttackType::kMagical ? GetSpellCritChance()
                                                       : 0);
}

bool Pet::IsHit(AttackType type) {
  return player.RollRng(type == AttackType::kPhysical  ? stats.melee_hit_chance
                        : type == AttackType::kMagical ? stats.spell_hit_chance
                                                       : 0);
}

double Pet::GetHastePercent() {
  return pet_type == PetType::kMelee ? stats.melee_haste_percent : stats.spell_haste_percent;
}

double Pet::GetAttackPower() {
  // Remove AP from debuffs on the boss before multiplying by the AP multiplier
  // since it doesn't affect those debuffs
  double attack_power_from_debuffs = GetDebuffAttackPower();
  double attack_power =
      ((GetStrength() * 2 - 20 + GetPlayerSpellPower() * 0.57) - attack_power_from_debuffs + stats.attack_power) *
      stats.attack_power_modifier;

  if (auras.demonic_frenzy != NULL) {
    attack_power *= (1 + 0.05 * auras.demonic_frenzy->stacks);
  }

  return attack_power + attack_power_from_debuffs;
}

double Pet::GetDebuffAttackPower() {
  double debuff_attack_power = 0;

  if (player.selected_auras.expose_weakness) {
    debuff_attack_power +=
        (player.settings.survival_hunter_agility * 0.25 * (player.settings.expose_weakness_uptime / 100.0));
  }

  if (player.selected_auras.improved_hunters_mark) {
    debuff_attack_power += 110;
  }

  return debuff_attack_power;
}

double Pet::GetAgility() { return stats.agility * stats.agility_modifier; }

double Pet::GetStrength() { return stats.strength * stats.strength_modifier; }

double Pet::FindTimeUntilNextAction() {
  double time = Entity::FindTimeUntilNextAction();

  if (time <= 0) {
    time = std::min(five_second_rule_timer_remaining, mp5_timer_remaining);
  }

  if ((player.talents.dark_pact > 0 || player.settings.pet_mode == EmbindConstant::kAggressive) &&
      mp5_timer_remaining < time)
    time = mp5_timer_remaining;

  if (player.settings.pet_mode == EmbindConstant::kAggressive) {
    for (auto& pet_spell : spell_list) {
      if (pet_spell->cooldown_remaining > 0 && pet_spell->cooldown_remaining < time) {
        time = pet_spell->cooldown_remaining;
      }
    }

    for (auto& pet_aura : aura_list) {
      if (pet_aura->active && pet_aura->duration_remaining < time) {
        time = pet_aura->duration_remaining;
      }
    }
  }

  return time;
}

void Pet::Tick(double t) {
  Entity::Tick(t);

  for (auto& spell : spell_list) {
    if (spell->cooldown_remaining > 0 || spell->casting) {
      spell->Tick(t);
    }
  }

  // MP5
  if (mp5_timer_remaining <= 0) {
    double mana_gain = stats.mp5;
    mp5_timer_remaining = 5;

    // Formulas from Max on the warlock discord
    // https://discord.com/channels/253210018697052162/823476479550816266/836007015762886707
    // &
    // https://discord.com/channels/253210018697052162/823476479550816266/839484387741138994
    // Mana regen from spirit
    if (five_second_rule_timer_remaining <= 0) {
      mana_gain += pet_name == PetName::kImp ? GetSpirit() + 0.7 * GetIntellect() - 258
                   : pet_name == PetName::kFelguard || pet_name == PetName::kSuccubus
                       ? 0.75 * GetSpirit() + 0.62 * GetIntellect() - 108
                       : 0;
    }
    // Mana regen while the 5 second spirit regen timer is active (no bonus from
    // spirit)
    else {
      mana_gain += pet_name == PetName::kImp                                          ? 0.375 * GetIntellect() - 123
                   : pet_name == PetName::kFelguard || pet_name == PetName::kSuccubus ? 0.365 * GetIntellect() - 48
                                                                                      : 0;
    }

    const int kCurrentMana = stats.mana;
    stats.mana = std::min(CalculateMaxMana(), stats.mana + static_cast<int>(mana_gain));
    if (stats.mana > kCurrentMana && ShouldWriteToCombatLog()) {
      CombatLog(name + " gains " + DoubleToString(round(mana_gain)) + " mana from Mp5/Spirit regeneration (" +
                DoubleToString(kCurrentMana) + " -> " + DoubleToString(stats.mana) + ")");
    }
  }
}

Imp::Imp(Player& player) : Pet(player) {
  name = "Imp";
  pet_name = PetName::kImp;
  pet_type = PetType::kRanged;
  stats.stamina += 101;
  stats.intellect += 327;
  stats.spirit += 263;
  stats.strength += 145;
  stats.agility += 38;
}

Succubus::Succubus(Player& player) : Pet(player) {
  name = "Succubus";
  pet_name = PetName::kSuccubus;
  pet_type = PetType::kMelee;
  stats.stamina += 280;
  stats.intellect += 133;
  stats.spirit += 122;
  stats.strength += 153;
  stats.agility += 109;
  stats.damage_modifier *= 1 + (0.02 * player.talents.master_demonologist);
}

Felguard::Felguard(Player& player) : Pet(player) {
  name = "Felguard";
  pet_type = PetType::kMelee;
  pet_name = PetName::kFelguard;
  stats.stamina += 280;
  stats.strength += 153;
  stats.agility += 108;
  stats.intellect += 133;
  stats.spirit += 122;
  stats.damage_modifier *= 1 + (0.01 * player.talents.master_demonologist);
}