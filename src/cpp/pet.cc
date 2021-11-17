#include "pet.h"

#include "player.h"

Pet::Pet(Player& player) : player(player) {
  spells = std::make_unique<PetSpells>();
  auras = std::make_unique<PetAuras>();
  base_stats = std::make_unique<PetStats>(player);
  buff_stats = std::make_unique<PetStats>(player);
  debuff_stats = std::make_unique<PetStats>(player);
  stats = std::make_unique<PetStats>(player);
  enemy_dodge_chance = 6.5;
  cast_time_remaining = 0;
  five_second_rule_timer_remaining = 5;
  spirit_tick_timer_remaining = 2;
  crit_multiplier = 1.5;
  glancing_blow_multiplier = 1 - (0.1 + (player.settings.enemy_level * 5 - player.kLevel * 5) / 100.0);
  glancing_blow_chance = std::max(0.0, 6 + (player.settings.enemy_level * 5 - player.kLevel * 5) * 1.2);
  crit_suppression = 4.73;
}

void Pet::Initialize() {
  Setup();

  if (pet == PetName::kImp) {
    spells->firebolt = std::make_unique<ImpFirebolt>(shared_from_this());
  } else {
    spells->melee = std::make_unique<Melee>(shared_from_this());

    if (pet == PetName::kSuccubus) {
      spells->lash_of_pain = std::make_unique<SuccubusLashOfPain>(shared_from_this());
    } else if (pet == PetName::kFelguard) {
      spells->cleave = std::make_unique<FelguardCleave>(shared_from_this());
      auras->demonic_frenzy = std::make_unique<DemonicFrenzy>(shared_from_this());
    }
    if (player.selected_auras.pet_battle_squawk) {
      std::cout << "creating battle squawk" << std::endl;
      auras->battle_squawk = std::make_unique<BattleSquawk>(shared_from_this());
    }
  }

  if (player.settings.prepop_black_book) {
    auras->black_book = std::make_unique<BlackBook>(shared_from_this());
  }
}

void Pet::CalculateStatsFromAuras() {
  // Calculate melee hit chance
  // Formula from https://wowwiki-archive.fandom.com/wiki/Hit?oldid=1584399
  const int kLevelDifference = player.settings.enemy_level - player.kLevel;
  stats->melee_hit_chance = 100 - (kLevelDifference <= 2 ? 5 + kLevelDifference * 0.5 : 7 + (kLevelDifference - 2) * 2);

  // Calculate spell hit chance
  // Formula from
  // https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/
  if (kLevelDifference <= 2) {
    stats->spell_hit_chance = std::min(99, 100 - kLevelDifference - 4);
  } else if (kLevelDifference == 3) {
    stats->spell_hit_chance = 83;
  } else if (kLevelDifference == 4) {
    stats->spell_hit_chance = 72;
  } else {
    stats->spell_hit_chance = 61;
  }

  // Auras
  if (player.selected_auras.pet_blessing_of_kings) {
    stats->stamina_modifier *= 1.1;
    stats->intellect_modifier *= 1.1;
    stats->agility_modifier *= 1.1;
    stats->strength_modifier *= 1.1;
    stats->spirit_modifier *= 1.1;
  }
  if (player.selected_auras.pet_blessing_of_wisdom) {
    stats->mp5 += 41;
  }
  if (player.selected_auras.mana_spring_totem) {
    stats->mp5 += 50;
  }
  if (player.selected_auras.wrath_of_air_totem) {
    buff_stats->spell_power += 101;
  }
  if (player.selected_auras.totem_of_wrath) {
    const int kHitAndCritAmount = player.settings.totem_of_wrath_amount * 3;

    buff_stats->spell_crit_chance += kHitAndCritAmount;
    stats->spell_hit_chance = std::min(99.0, stats->spell_hit_chance + kHitAndCritAmount);
  }
  // todo improved motw
  if (player.selected_auras.pet_mark_of_the_wild) {
    buff_stats->stamina += 14;
    buff_stats->intellect += 14;
    buff_stats->strength += 14;
    buff_stats->agility += 14;
    stats->spirit += 14;
  }
  // todo improved imp
  if (player.selected_auras.blood_pact) {
    buff_stats->stamina += 70;
  }
  if (player.selected_auras.pet_arcane_intellect) {
    buff_stats->intellect += 40;
  }
  if (player.selected_auras.pet_prayer_of_fortitude) {
    buff_stats->stamina += 79;
  }
  if (player.selected_auras.pet_prayer_of_spirit) {
    stats->spirit += 50 * (1 + (0.1 * player.settings.improved_divine_spirit));
  }
  if (player.selected_auras.inspiring_presence) {
    stats->spell_hit_chance = std::min(99.0, stats->spell_hit_chance + 1);
  }
  if (player.selected_auras.moonkin_aura) {
    buff_stats->spell_crit_chance += 5;
  }
  if (player.selected_auras.eye_of_the_night) {
    buff_stats->spell_power += 34;
  }
  if (player.selected_auras.chain_of_the_twilight_owl) {
    buff_stats->spell_crit_chance += 2;
  }
  if (player.selected_auras.jade_pendant_of_blasting) {
    buff_stats->spell_power += 15;
  }
  // Atiesh auras
  // Add 33sp if the player has Atiesh equipped since the aura's spell power is
  // just added to the item itself
  if (player.items.two_hand == 22630) {
    buff_stats->spell_power += 33;
  }
  if (player.selected_auras.atiesh_warlock) {
    buff_stats->spell_power += 33 * player.settings.warlock_atiesh_amount;
  }
  if (player.selected_auras.wrath_of_air_totem && player.settings.has_elemental_shaman_t4_bonus) {
    buff_stats->spell_power += 20;
  }
  if (player.selected_auras.judgement_of_the_crusader) {
    buff_stats->melee_crit_chance += 3;
    buff_stats->spell_crit_chance += 3;
  }
  // Multiply by 25% instead of 5% since it's happening every 5 seconds instead
  // of every 1 second
  if (player.selected_auras.vampiric_touch) {
    stats->mp5 += player.settings.shadow_priest_dps * 0.25;
  }
  if (player.selected_auras.faerie_fire && player.settings.improved_faerie_fire) {
    stats->melee_hit_chance += 3;
  }
  if (player.selected_auras.expose_weakness) {
    debuff_stats->attack_power +=
        (player.settings.survival_hunter_agility * 0.25 * (player.settings.expose_weakness_uptime / 100.0));
  }
  if (player.selected_auras.improved_hunters_mark) {
    debuff_stats->attack_power += 110;
  }
  if (player.selected_auras.pet_heroic_presence) {
    stats->melee_hit_chance++;
  }
  if (player.selected_auras.pet_blessing_of_might) {
    buff_stats->attack_power += 220;
  }
  if (player.selected_auras.pet_strength_of_earth_totem) {
    buff_stats->strength += 86;
  }
  if (player.selected_auras.pet_grace_of_air_totem) {
    buff_stats->agility += 67;
  }
  if (player.selected_auras.pet_battle_shout) {
    buff_stats->attack_power += 306;
  }
  if (player.selected_auras.pet_trueshot_aura) {
    buff_stats->attack_power += 300;
  }
  if (player.selected_auras.pet_leader_of_the_pack) {
    buff_stats->melee_crit_chance += 5;
  }
  if (player.selected_auras.pet_unleashed_rage) {
    stats->attack_power_modifier *= 1.1;
  }
  if (player.selected_auras.pet_stamina_scroll) {
    buff_stats->stamina += 20;
  }
  if (player.selected_auras.pet_intellect_scroll) {
    buff_stats->intellect += 20;
  }
  if (player.selected_auras.pet_strength_scroll) {
    buff_stats->strength += 20;
  }
  if (player.selected_auras.pet_agility_scroll) {
    buff_stats->agility += 20;
  }
  if (player.selected_auras.pet_spirit_scroll) {
    stats->spirit += 20;
  }
  if (player.selected_auras.pet_kiblers_bits) {
    buff_stats->strength += 20;
    stats->spirit += 20;
  }
  if (player.selected_auras.ferocious_inspiration) {
    stats->damage_modifier *= std::pow(1.03, player.settings.ferocious_inspiration_amount);
  }
  if (player.settings.race == EmbindConstant::kOrc) {
    stats->damage_modifier *= 1.05;
  }

  // Hidden attack power modifiers (source: Max on warlock discord)
  if (pet == PetName::kFelguard) {
    stats->attack_power_modifier *= 1.1;
  } else if (pet == PetName::kSuccubus) {
    stats->attack_power_modifier *= 1.05;
  }

  // Calculate armor
  if (pet_type == PetType::kMelee) {
    // Formula from
    // https://wowwiki-archive.fandom.com/wiki/Damage_reduction?oldid=807810
    if (player.settings.enemy_level <= 59) {
      armor_multiplier =
          1 - player.settings.enemy_armor / (player.settings.enemy_armor + 400 + 85 * player.settings.enemy_level);
    } else {
      armor_multiplier = 1 - player.settings.enemy_armor /
                                 (player.settings.enemy_armor - 22167.5 + 467.5 * player.settings.enemy_level);
    }
  }
  armor_multiplier = std::max(0.25, armor_multiplier);

  stats->stamina_modifier *= 1 + (0.05 * player.talents.fel_stamina);
  stats->intellect_modifier *= 1 + (0.05 * player.talents.fel_intellect);
  stats->damage_modifier *= (1 + 0.04 * player.talents.unholy_power) * (1 + 0.05 * player.talents.soul_link);
}

int Pet::GetPlayerSpellPower() {
  return player.GetSpellPower() + std::max(player.stats.shadow_power, player.stats.fire_power);
}

void Pet::CalculateStatsFromPlayer(bool announce_in_combat_log) {
  stats->stamina = base_stats->stamina + buff_stats->stamina + 0.3 * player.stats.GetStamina();
  stats->intellect = base_stats->intellect + buff_stats->intellect + (0.3 * player.stats.GetIntellect());
  player.demonic_knowledge_spell_power = (GetStamina() + GetIntellect()) * (0.04 * player.talents.demonic_knowledge);
  base_stats->attack_power = (GetStrength() * 2) - 20;
  stats->attack_power =
      base_stats->attack_power + buff_stats->attack_power + debuff_stats->attack_power + (GetPlayerSpellPower() * 0.57);
  stats->agility = base_stats->agility + buff_stats->agility;
  stats->melee_crit_chance =
      player.talents.demonic_tactics + (GetAgility() * 0.04) + 0.65 + buff_stats->melee_crit_chance;
  stats->spell_power = buff_stats->spell_power + (GetPlayerSpellPower() * 0.15);
  if (pet_type == PetType::kMelee) {
    dmg = ((GetAttackPower() / 14) + 51.7) * base_melee_speed;
  }
  stats->max_mana = base_stats->mana + (GetIntellect() * (pet_type == PetType::kMelee ? 11.555
                                                          : pet == PetName::kImp      ? 4.95
                                                                                      : 0));
  if (pet == PetName::kImp || pet == PetName::kSuccubus) {
    stats->spell_crit_chance =
        (0.0125 * GetIntellect()) + 0.91 + player.talents.demonic_tactics + buff_stats->spell_crit_chance;
  }
  if (announce_in_combat_log && player.ShouldWriteToCombatLog()) {
    player.CombatLog("Recalculated " + name + "'s stats");
  }
}

void Pet::Setup() {
  CalculateStatsFromAuras();
  CalculateStatsFromPlayer(false);
}

void Pet::Reset() {
  stats->mana = stats->max_mana;
  five_second_rule_timer_remaining = 5;
  spirit_tick_timer_remaining = 2;
  cast_time_remaining = 0;

  if (spells->melee != NULL) spells->melee->Reset();
  if (spells->firebolt != NULL) spells->firebolt->Reset();
  if (spells->lash_of_pain != NULL) spells->lash_of_pain->Reset();
  if (spells->cleave != NULL) spells->cleave->Reset();

  CalculateStatsFromPlayer(false);
}

void Pet::EndAuras() {
  if (auras->black_book != NULL && auras->black_book->active) auras->black_book->Fade();
  if (auras->demonic_frenzy != NULL && auras->demonic_frenzy->active) auras->demonic_frenzy->Fade();
  if (auras->battle_squawk != NULL && auras->battle_squawk->active) auras->battle_squawk->Fade();
}

double Pet::GetMeleeCritChance() { return stats->melee_crit_chance - crit_suppression; }

double Pet::GetSpellCritChance() { return stats->spell_crit_chance; }

bool Pet::IsCrit(AttackType type) {
  return player.RollRng(type == AttackType::kPhysical  ? GetMeleeCritChance()
                        : type == AttackType::kMagical ? GetSpellCritChance()
                                                       : 0);
}

double Pet::GetMeleeHitChance() { return stats->melee_hit_chance; }

double Pet::GetSpellHitChance() { return std::min(99.0, stats->spell_hit_chance); }

bool Pet::IsHit(AttackType type) {
  return player.RollRng(type == AttackType::kPhysical  ? GetMeleeHitChance()
                        : type == AttackType::kMagical ? GetSpellHitChance()
                                                       : 0);
}

double Pet::GetHastePercent() { return stats->haste_percent * (pet != PetName::kImp ? stats->melee_haste_percent : 1); }

int Pet::GetAttackPower() {
  // Remove AP from debuffs on the boss before multiplying by the AP multiplier
  // since it doesn't affect those debuffs
  double attack_power = (stats->attack_power - debuff_stats->attack_power) * stats->attack_power_modifier;
  if (auras->demonic_frenzy != NULL) {
    attack_power *= (1 + 0.05 * auras->demonic_frenzy->stacks);
  }

  return attack_power + debuff_stats->attack_power;
}

int Pet::GetSpirit() { return (base_stats->spirit + buff_stats->spirit + stats->spirit) * stats->spirit_modifier; }

int Pet::GetStamina() { return stats->stamina * stats->stamina_modifier; }

int Pet::GetIntellect() { return stats->intellect * stats->intellect_modifier; }

int Pet::GetAgility() { return stats->agility * stats->agility_modifier; }

int Pet::GetStrength() { return (base_stats->strength + buff_stats->strength) * stats->strength_modifier; }

void Pet::Tick(double t) {
  cast_time_remaining -= t;
  five_second_rule_timer_remaining -= t;
  spirit_tick_timer_remaining -= t;

  // Auras
  if (auras->black_book != NULL && auras->black_book->active) auras->black_book->Tick(t);
  if (auras->battle_squawk != NULL && auras->battle_squawk->active) auras->battle_squawk->Tick(t);

  // Spells
  if (spells->melee != NULL && spells->melee->cooldown_remaining > 0) spells->melee->Tick(t);
  if (spells->cleave != NULL && spells->cleave->cooldown_remaining > 0) spells->cleave->Tick(t);
  if (spells->firebolt != NULL && spells->firebolt->casting) spells->firebolt->Tick(t);
  if (spells->lash_of_pain != NULL && spells->lash_of_pain->cooldown_remaining > 0) spells->lash_of_pain->Tick(t);

  // MP5
  if (spirit_tick_timer_remaining <= 0) {
    double mana_gain = stats->mp5;
    spirit_tick_timer_remaining = 5;

    // Formulas from Max on the warlock discord
    // https://discord.com/channels/253210018697052162/823476479550816266/836007015762886707
    // &
    // https://discord.com/channels/253210018697052162/823476479550816266/839484387741138994
    // Mana regen from spirit
    if (five_second_rule_timer_remaining <= 0) {
      mana_gain += pet == PetName::kImp ? GetSpirit() + 0.7 * GetIntellect() - 258
                   : pet == PetName::kFelguard || pet == PetName::kSuccubus
                       ? 0.75 * GetSpirit() + 0.62 * GetIntellect() - 108
                       : 0;
    }
    // Mana regen while the 5 second spirit regen timer is active (no bonus from
    // spirit)
    else {
      mana_gain += pet == PetName::kImp                                     ? 0.375 * GetIntellect() - 123
                   : pet == PetName::kFelguard || pet == PetName::kSuccubus ? 0.365 * GetIntellect() - 48
                                                                            : 0;
    }

    const int kCurrentMana = stats->mana;

    stats->mana = std::min(stats->max_mana, stats->mana + static_cast<int>(mana_gain));
    if (stats->mana > kCurrentMana && player.ShouldWriteToCombatLog()) {
      player.CombatLog(name + " gains " + std::to_string(round(mana_gain)) + " mana from Mp5/Spirit regeneration (" +
                       std::to_string(kCurrentMana) + " -> " + std::to_string(stats->mana) + ")");
    }
  }
}

Imp::Imp(Player& player) : Pet(player) {
  name = "Imp";
  pet = PetName::kImp;
  pet_type = PetType::kRanged;
  base_stats->stamina = 101;
  base_stats->intellect = 327;
  base_stats->mana = 756;
  base_stats->spirit = 263;
  base_stats->strength = 145;
  base_stats->agility = 38;
}

Succubus::Succubus(Player& player) : Pet(player) {
  name = "Succubus";
  pet = PetName::kSuccubus;
  pet_type = PetType::kMelee;
  base_stats->stamina = 280;
  base_stats->intellect = 133;
  base_stats->mana = 849;
  base_stats->spirit = 122;
  base_stats->strength = 153;
  base_stats->agility = 109;
  base_melee_speed = 2;
  stats->damage_modifier *= 1 + (0.02 * player.talents.master_demonologist);
}

Felguard::Felguard(Player& player) : Pet(player) {
  name = "Felguard";
  pet_type = PetType::kMelee;
  pet = PetName::kFelguard;
  base_stats->stamina = 280;
  base_stats->strength = 153;
  base_stats->agility = 108;
  base_stats->intellect = 133;
  base_stats->spirit = 122;
  base_stats->mana = 893;
  base_melee_speed = 2;
  stats->damage_modifier *= 1 + (0.01 * player.talents.master_demonologist);
}