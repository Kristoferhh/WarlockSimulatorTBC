#include "pet.h"

#include "bindings.h"
#include "common.h"
#include "player.h"

Pet::Pet(Player& player)
    : player(player),
      spells(PetSpells()),
      auras(PetAuras()),
      base_stats(AllocStats()),
      buff_stats(AllocStats()),
      debuff_stats(AllocStats()),
      stats(AllocStats()),
      enemy_dodge_chance(6.5),
      cast_time_remaining(0),
      five_second_rule_timer_remaining(5),
      spirit_tick_timer_remaining(2),
      crit_multiplier(1.5),
      glancing_blow_multiplier(1 - (0.1 + (player.settings.enemy_level * 5 - player.kLevel * 5) / 100.0)),
      glancing_blow_chance(std::max(0.0, 6 + (player.settings.enemy_level * 5 - player.kLevel * 5) * 1.2)),
      crit_suppression(4.73),
      entity_type(EntityType::kPet) {}

void Pet::Initialize() {
  Setup();

  if (pet == PetName::kImp) {
    spells.firebolt = std::make_unique<ImpFirebolt>(shared_from_this());
  } else {
    spells.melee = std::make_unique<Melee>(shared_from_this());

    if (pet == PetName::kSuccubus) {
      spells.lash_of_pain = std::make_unique<SuccubusLashOfPain>(shared_from_this());
    } else if (pet == PetName::kFelguard) {
      spells.cleave = std::make_unique<FelguardCleave>(shared_from_this());
      auras.demonic_frenzy = std::make_unique<DemonicFrenzy>(shared_from_this());
    }
    if (player.selected_auras.pet_battle_squawk) {
      auras.battle_squawk = std::make_unique<BattleSquawk>(shared_from_this());
    }
  }

  if (player.settings.prepop_black_book) {
    auras.black_book = std::make_unique<BlackBook>(shared_from_this());
  }
}

void Pet::CalculateStatsFromAuras() {
  // Calculate melee hit chance
  // Formula from https://wowwiki-archive.fandom.com/wiki/Hit?oldid=1584399
  const int kLevelDifference = player.settings.enemy_level - player.kLevel;
  stats.at(CharacterStat::kMeleeHitChance) =
      100 - (kLevelDifference <= 2 ? 5 + kLevelDifference * 0.5 : 7 + (kLevelDifference - 2) * 2);

  // Calculate spell hit chance
  // Formula from
  // https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/
  stats.at(CharacterStat::kSpellHitChance) = kLevelDifference <= 2   ? std::min(99, 100 - kLevelDifference - 4)
                                             : kLevelDifference == 3 ? 83
                                             : kLevelDifference == 4 ? 72
                                                                     : 61;

  // Auras
  if (player.selected_auras.pet_blessing_of_kings) {
    stats.at(CharacterStat::kStaminaModifier) *= 1.1;
    stats.at(CharacterStat::kIntellectModifier) *= 1.1;
    stats.at(CharacterStat::kAgilityModifier) *= 1.1;
    stats.at(CharacterStat::kStrengthModifier) *= 1.1;
    stats.at(CharacterStat::kSpiritModifier) *= 1.1;
  }
  if (player.selected_auras.pet_blessing_of_wisdom) {
    stats.at(CharacterStat::kMp5) += 41;
  }
  if (player.selected_auras.mana_spring_totem) {
    stats.at(CharacterStat::kMp5) += 50;
  }
  if (player.selected_auras.wrath_of_air_totem) {
    buff_stats.at(CharacterStat::kSpellPower) += 101;
  }
  if (player.selected_auras.totem_of_wrath) {
    const int kHitAndCritAmount = player.settings.totem_of_wrath_amount * 3;

    buff_stats.at(CharacterStat::kSpellCritChance) += kHitAndCritAmount;
    stats.at(CharacterStat::kSpellHitChance) =
        std::min(99.0, stats.at(CharacterStat::kSpellHitChance) + kHitAndCritAmount);
  }
  // todo improved motw
  if (player.selected_auras.pet_mark_of_the_wild) {
    buff_stats.at(CharacterStat::kStamina) += 14;
    buff_stats.at(CharacterStat::kIntellect) += 14;
    buff_stats.at(CharacterStat::kStrength) += 14;
    buff_stats.at(CharacterStat::kAgility) += 14;
    stats.at(CharacterStat::kSpirit) += 14;
  }
  // todo improved imp
  if (player.selected_auras.blood_pact) {
    buff_stats.at(CharacterStat::kStamina) += 70;
  }
  if (player.selected_auras.pet_arcane_intellect) {
    buff_stats.at(CharacterStat::kIntellect) += 40;
  }
  if (player.selected_auras.pet_prayer_of_fortitude) {
    buff_stats.at(CharacterStat::kStamina) += 79;
  }
  if (player.selected_auras.pet_prayer_of_spirit) {
    stats.at(CharacterStat::kSpirit) += 50 * (1 + (0.1 * player.settings.improved_divine_spirit));
  }
  if (player.selected_auras.inspiring_presence) {
    stats.at(CharacterStat::kSpellHitChance) = std::min(99.0, stats.at(CharacterStat::kSpellHitChance) + 1);
  }
  if (player.selected_auras.moonkin_aura) {
    buff_stats.at(CharacterStat::kSpellCritChance) += 5;
  }
  if (player.selected_auras.eye_of_the_night) {
    buff_stats.at(CharacterStat::kSpellPower) += 34;
  }
  if (player.selected_auras.chain_of_the_twilight_owl) {
    buff_stats.at(CharacterStat::kSpellCritChance) += 2;
  }
  if (player.selected_auras.jade_pendant_of_blasting) {
    buff_stats.at(CharacterStat::kSpellPower) += 15;
  }
  // Atiesh auras
  // Add 33sp if the player has Atiesh equipped since the aura's spell power is
  // just added to the item itself
  if (player.items.two_hand == 22630) {
    buff_stats.at(CharacterStat::kSpellPower) += 33;
  }
  if (player.selected_auras.atiesh_warlock) {
    buff_stats.at(CharacterStat::kSpellPower) += 33 * player.settings.warlock_atiesh_amount;
  }
  if (player.selected_auras.wrath_of_air_totem && player.settings.has_elemental_shaman_t4_bonus) {
    buff_stats.at(CharacterStat::kSpellPower) += 20;
  }
  if (player.selected_auras.judgement_of_the_crusader) {
    buff_stats.at(CharacterStat::kMeleeCritChance) += 3;
    buff_stats.at(CharacterStat::kSpellCritChance) += 3;
  }
  // Multiply by 25% instead of 5% since it's happening every 5 seconds instead
  // of every 1 second
  if (player.selected_auras.vampiric_touch) {
    stats.at(CharacterStat::kMp5) += player.settings.shadow_priest_dps * 0.25;
  }
  if (player.selected_auras.faerie_fire && player.settings.improved_faerie_fire) {
    stats.at(CharacterStat::kMeleeHitChance) += 3;
  }
  if (player.selected_auras.expose_weakness) {
    debuff_stats.at(CharacterStat::kAttackPower) +=
        (player.settings.survival_hunter_agility * 0.25 * (player.settings.expose_weakness_uptime / 100.0));
  }
  if (player.selected_auras.improved_hunters_mark) {
    debuff_stats.at(CharacterStat::kAttackPower) += 110;
  }
  if (player.selected_auras.pet_heroic_presence) {
    stats.at(CharacterStat::kMeleeHitChance)++;
  }
  if (player.selected_auras.pet_blessing_of_might) {
    buff_stats.at(CharacterStat::kAttackPower) += 220;
  }
  if (player.selected_auras.pet_strength_of_earth_totem) {
    buff_stats.at(CharacterStat::kStrength) += 86;
  }
  if (player.selected_auras.pet_grace_of_air_totem) {
    buff_stats.at(CharacterStat::kAgility) += 67;
  }
  if (player.selected_auras.pet_battle_shout) {
    buff_stats.at(CharacterStat::kAttackPower) += 306;
  }
  if (player.selected_auras.pet_trueshot_aura) {
    buff_stats.at(CharacterStat::kAttackPower) += 300;
  }
  if (player.selected_auras.pet_leader_of_the_pack) {
    buff_stats.at(CharacterStat::kMeleeCritChance) += 5;
  }
  if (player.selected_auras.pet_unleashed_rage) {
    stats.at(CharacterStat::kAttackPowerModifier) *= 1.1;
  }
  if (player.selected_auras.pet_stamina_scroll) {
    buff_stats.at(CharacterStat::kStamina) += 20;
  }
  if (player.selected_auras.pet_intellect_scroll) {
    buff_stats.at(CharacterStat::kIntellect) += 20;
  }
  if (player.selected_auras.pet_strength_scroll) {
    buff_stats.at(CharacterStat::kStrength) += 20;
  }
  if (player.selected_auras.pet_agility_scroll) {
    buff_stats.at(CharacterStat::kAgility) += 20;
  }
  if (player.selected_auras.pet_spirit_scroll) {
    stats.at(CharacterStat::kSpirit) += 20;
  }
  if (player.selected_auras.pet_kiblers_bits) {
    buff_stats.at(CharacterStat::kStrength) += 20;
    stats.at(CharacterStat::kSpirit) += 20;
  }
  if (player.selected_auras.ferocious_inspiration) {
    stats.at(CharacterStat::kDamageModifier) *= std::pow(1.03, player.settings.ferocious_inspiration_amount);
  }
  if (player.settings.race == EmbindConstant::kOrc) {
    stats.at(CharacterStat::kDamageModifier) *= 1.05;
  }

  // Hidden attack power modifiers (source: Max on warlock discord)
  stats.at(CharacterStat::kAttackPowerModifier) *= (pet == PetName::kFelguard   ? 1.1
                                                    : pet == PetName::kSuccubus ? 1.05
                                                                                : 1);

  // Calculate armor
  if (pet_type == PetType::kMelee) {
    // Formula from
    // https://wowwiki-archive.fandom.com/wiki/Damage_reduction?oldid=807810
    armor_multiplier =
        std::max(0.25, player.settings.enemy_level >= 60
                           ? 1 - player.settings.enemy_armor /
                                     (player.settings.enemy_armor - 22167.5 + 467.5 * player.settings.enemy_level)
                           : 1 - player.settings.enemy_armor /
                                     (player.settings.enemy_armor + 400 + 85 * player.settings.enemy_level));
  }

  stats.at(CharacterStat::kStaminaModifier) *= 1 + (0.05 * player.talents.fel_stamina);
  stats.at(CharacterStat::kIntellectModifier) *= 1 + (0.05 * player.talents.fel_intellect);
  stats.at(CharacterStat::kDamageModifier) *=
      (1 + 0.04 * player.talents.unholy_power) * (1 + 0.05 * player.talents.soul_link);

  stats.at(CharacterStat::kSpellHitChance) = std::min(99.0, stats.at(CharacterStat::kSpellHitChance));
}

int Pet::GetPlayerSpellPower() {
  return player.GetSpellPower() +
         std::max(player.stats.at(CharacterStat::kShadowPower), player.stats.at(CharacterStat::kFirePower));
}

void Pet::CalculateStatsFromPlayer(bool announce_in_combat_log) {
  stats.at(CharacterStat::kStamina) =
      base_stats.at(CharacterStat::kStamina) + buff_stats.at(CharacterStat::kStamina) + 0.3 * player.GetStamina();
  stats.at(CharacterStat::kIntellect) = base_stats.at(CharacterStat::kIntellect) +
                                        buff_stats.at(CharacterStat::kIntellect) + (0.3 * player.GetIntellect());
  player.demonic_knowledge_spell_power = (GetStamina() + GetIntellect()) * (0.04 * player.talents.demonic_knowledge);
  base_stats.at(CharacterStat::kAttackPower) = (GetStrength() * 2) - 20;
  stats.at(CharacterStat::kAttackPower) = base_stats.at(CharacterStat::kAttackPower) +
                                          buff_stats.at(CharacterStat::kAttackPower) +
                                          debuff_stats.at(CharacterStat::kAttackPower) + (GetPlayerSpellPower() * 0.57);
  stats.at(CharacterStat::kAgility) = base_stats.at(CharacterStat::kAgility) + buff_stats.at(CharacterStat::kAgility);
  stats.at(CharacterStat::kMeleeCritChance) =
      player.talents.demonic_tactics + (GetAgility() * 0.04) + 0.65 + buff_stats.at(CharacterStat::kMeleeCritChance);
  stats.at(CharacterStat::kSpellPower) = buff_stats.at(CharacterStat::kSpellPower) + (GetPlayerSpellPower() * 0.15);
  if (pet_type == PetType::kMelee) {
    dmg = ((GetAttackPower() / 14) + 51.7) * base_melee_speed;
  }
  stats.at(CharacterStat::kMaxMana) =
      base_stats.at(CharacterStat::kMana) + (GetIntellect() * (pet_type == PetType::kMelee ? 11.555
                                                               : pet == PetName::kImp      ? 4.95
                                                                                           : 0));
  if (pet == PetName::kImp || pet == PetName::kSuccubus) {
    stats.at(CharacterStat::kSpellCritChance) = (0.0125 * GetIntellect()) + 0.91 + player.talents.demonic_tactics +
                                                buff_stats.at(CharacterStat::kSpellCritChance);
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
  stats.at(CharacterStat::kMana) = stats.at(CharacterStat::kMaxMana);
  five_second_rule_timer_remaining = 5;
  spirit_tick_timer_remaining = 2;
  cast_time_remaining = 0;

  if (spells.melee != NULL) spells.melee->Reset();
  if (spells.firebolt != NULL) spells.firebolt->Reset();
  if (spells.lash_of_pain != NULL) spells.lash_of_pain->Reset();
  if (spells.cleave != NULL) spells.cleave->Reset();

  CalculateStatsFromPlayer(false);
}

void Pet::EndAuras() {
  if (auras.black_book != NULL && auras.black_book->active) auras.black_book->Fade();
  if (auras.demonic_frenzy != NULL && auras.demonic_frenzy->active) auras.demonic_frenzy->Fade();
  if (auras.battle_squawk != NULL && auras.battle_squawk->active) auras.battle_squawk->Fade();
}

double Pet::GetMeleeCritChance() { return stats.at(CharacterStat::kMeleeCritChance) - crit_suppression; }

double Pet::GetSpellCritChance() { return stats.at(CharacterStat::kSpellCritChance); }

bool Pet::IsCrit(AttackType type) {
  return player.RollRng(type == AttackType::kPhysical  ? GetMeleeCritChance()
                        : type == AttackType::kMagical ? GetSpellCritChance()
                                                       : 0);
}

bool Pet::IsHit(AttackType type) {
  return player.RollRng(type == AttackType::kPhysical  ? stats.at(CharacterStat::kMeleeHitChance)
                        : type == AttackType::kMagical ? stats.at(CharacterStat::kSpellHitChance)
                                                       : 0);
}

double Pet::GetHastePercent() {
  return pet_type == PetType::kMelee ? stats.at(CharacterStat::kMeleeHastePercent)
                                     : stats.at(CharacterStat::kSpellHastePercent);
}

int Pet::GetAttackPower() {
  // Remove AP from debuffs on the boss before multiplying by the AP multiplier
  // since it doesn't affect those debuffs
  double attack_power = (stats.at(CharacterStat::kAttackPower) - debuff_stats.at(CharacterStat::kAttackPower)) *
                        stats.at(CharacterStat::kAttackPowerModifier);
  if (auras.demonic_frenzy != NULL) {
    attack_power *= (1 + 0.05 * auras.demonic_frenzy->stacks);
  }

  return attack_power + debuff_stats.at(CharacterStat::kAttackPower);
}

int Pet::GetSpirit() {
  return (base_stats.at(CharacterStat::kSpirit) + buff_stats.at(CharacterStat::kSpirit) +
          stats.at(CharacterStat::kSpirit)) *
         stats.at(CharacterStat::kSpiritModifier);
}

int Pet::GetStamina() { return stats.at(CharacterStat::kStamina) * stats.at(CharacterStat::kStaminaModifier); }

int Pet::GetIntellect() { return stats.at(CharacterStat::kIntellect) * stats.at(CharacterStat::kIntellectModifier); }

int Pet::GetAgility() { return stats.at(CharacterStat::kAgility) * stats.at(CharacterStat::kAgilityModifier); }

int Pet::GetStrength() {
  return (base_stats.at(CharacterStat::kStrength) + buff_stats.at(CharacterStat::kStrength)) *
         stats.at(CharacterStat::kStrengthModifier);
}

void Pet::Tick(double t) {
  cast_time_remaining -= t;
  five_second_rule_timer_remaining -= t;
  spirit_tick_timer_remaining -= t;

  // Auras
  if (auras.black_book != NULL && auras.black_book->active) auras.black_book->Tick(t);
  if (auras.battle_squawk != NULL && auras.battle_squawk->active) auras.battle_squawk->Tick(t);

  // Spells
  if (spells.melee != NULL && spells.melee->cooldown_remaining > 0) spells.melee->Tick(t);
  if (spells.cleave != NULL && spells.cleave->cooldown_remaining > 0) spells.cleave->Tick(t);
  if (spells.firebolt != NULL && spells.firebolt->casting) spells.firebolt->Tick(t);
  if (spells.lash_of_pain != NULL && spells.lash_of_pain->cooldown_remaining > 0) spells.lash_of_pain->Tick(t);

  // MP5
  if (spirit_tick_timer_remaining <= 0) {
    double mana_gain = stats.at(CharacterStat::kMp5);
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

    const int kCurrentMana = stats.at(CharacterStat::kMana);

    stats.at(CharacterStat::kMana) =
        std::min(stats.at(CharacterStat::kMaxMana), stats.at(CharacterStat::kMana) + static_cast<int>(mana_gain));
    if (stats.at(CharacterStat::kMana) > kCurrentMana && player.ShouldWriteToCombatLog()) {
      player.CombatLog(name + " gains " + DoubleToString(round(mana_gain)) + " mana from Mp5/Spirit regeneration (" +
                       DoubleToString(kCurrentMana) + " -> " + DoubleToString(stats.at(CharacterStat::kMana)) + ")");
    }
  }
}

Imp::Imp(Player& player) : Pet(player) {
  name = "Imp";
  pet = PetName::kImp;
  pet_type = PetType::kRanged;
  base_stats.at(CharacterStat::kStamina) = 101;
  base_stats.at(CharacterStat::kIntellect) = 327;
  base_stats.at(CharacterStat::kMana) = 756;
  base_stats.at(CharacterStat::kSpirit) = 263;
  base_stats.at(CharacterStat::kStrength) = 145;
  base_stats.at(CharacterStat::kAgility) = 38;
}

Succubus::Succubus(Player& player) : Pet(player) {
  name = "Succubus";
  pet = PetName::kSuccubus;
  pet_type = PetType::kMelee;
  base_stats.at(CharacterStat::kStamina) = 280;
  base_stats.at(CharacterStat::kIntellect) = 133;
  base_stats.at(CharacterStat::kMana) = 849;
  base_stats.at(CharacterStat::kSpirit) = 122;
  base_stats.at(CharacterStat::kStrength) = 153;
  base_stats.at(CharacterStat::kAgility) = 109;
  base_melee_speed = 2;
  stats.at(CharacterStat::kDamageModifier) *= 1 + (0.02 * player.talents.master_demonologist);
}

Felguard::Felguard(Player& player) : Pet(player) {
  name = "Felguard";
  pet_type = PetType::kMelee;
  pet = PetName::kFelguard;
  base_stats.at(CharacterStat::kStamina) = 280;
  base_stats.at(CharacterStat::kStrength) = 153;
  base_stats.at(CharacterStat::kAgility) = 108;
  base_stats.at(CharacterStat::kIntellect) = 133;
  base_stats.at(CharacterStat::kSpirit) = 122;
  base_stats.at(CharacterStat::kMana) = 893;
  base_melee_speed = 2;
  stats.at(CharacterStat::kDamageModifier) *= 1 + (0.01 * player.talents.master_demonologist);
}