#include "pet.h"

#include "player.h"

Pet::Pet(std::shared_ptr<Player> player) : player(player) {
  spells = std::make_unique<PetSpells>();
  auras = std::make_unique<PetAuras>();
  base_stats = std::make_unique<PetStats>();
  buff_stats = std::make_unique<PetStats>();
  debuff_stats = std::make_unique<PetStats>();
  stats = std::make_unique<PetStats>(player);
  enemy_dodge_chance = 6.5;
  cast_time_remaining = 0;
  five_second_rule_timer_remaining = 5;
  spirit_tick_timer_remaining = 2;
  mode = player->settings->pet_is_aggressive ? PetMode::AGGRESSIVE : PetMode::PASSIVE;
  crit_multiplier = 1.5;
  glancing_blow_multiplier = 1 - (0.1 + (player->settings->enemy_level * 5 - player->kLevel * 5) / 100.0);
  glancing_blow_chance = std::max(0.0, 6 + (player->settings->enemy_level * 5 - player->kLevel * 5) * 1.2);
  crit_suppression = 4.73;
}

void Pet::Initialize() {
  Setup();

  if (pet == PetName::IMP) {
    spells->firebolt = std::make_unique<ImpFirebolt>(shared_from_this());
  } else {
    spells->melee = std::make_unique<Melee>(shared_from_this());

    if (pet == PetName::SUCCUBUS) {
      spells->lash_of_pain = std::make_unique<SuccubusLashOfPain>(shared_from_this());
    } else if (pet == PetName::FELGUARD) {
      spells->cleave = std::make_unique<FelguardCleave>(shared_from_this());
      auras->demonic_frenzy = std::make_unique<DemonicFrenzy>(shared_from_this());
    }
  }

  if (player->settings->prepop_black_book) {
    auras->black_book = std::make_unique<BlackBook>(shared_from_this());
  }
}

void Pet::CalculateStatsFromAuras() {
  // Calculate melee hit chance
  // Formula from https://wowwiki-archive.fandom.com/wiki/Hit?oldid=1584399
  int level_difference = player->settings->enemy_level - player->kLevel;
  if (level_difference <= 2) {
    stats->melee_hit_chance = 100 - (5 + level_difference * 0.5);
  } else {
    stats->melee_hit_chance = 100 - (7 + (level_difference - 2) * 2);
  }

  // Calculate spell hit chance
  // Formula from
  // https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/
  if (level_difference <= 2) {
    stats->spell_hit_chance = std::min(99, 100 - level_difference - 4);
  } else if (level_difference == 3) {
    stats->spell_hit_chance = 83;
  } else if (level_difference == 4) {
    stats->spell_hit_chance = 72;
  } else {
    stats->spell_hit_chance = 61;
  }

  // Auras
  if (player->selected_auras->petBlessingOfKings) {
    stats->stamina_modifier *= 1.1;
    stats->intellect_modifier *= 1.1;
    stats->agility_modifier *= 1.1;
    stats->strength_modifier *= 1.1;
    stats->spirit_modifier *= 1.1;
  }
  if (player->selected_auras->petBlessingOfWisdom) {
    stats->mp5 += 41;
  }
  if (player->selected_auras->manaSpringTotem) {
    stats->mp5 += 50;
  }
  if (player->selected_auras->wrathOfAirTotem) {
    buff_stats->spell_power += 101;
  }
  if (player->selected_auras->totemOfWrath) {
    int hit_and_crit_amount = player->settings->totem_of_wrath_amount * 3;

    buff_stats->spell_crit_chance += hit_and_crit_amount;
    stats->spell_hit_chance = std::min(99.0, stats->spell_hit_chance + hit_and_crit_amount);
  }
  // todo improved motw
  if (player->selected_auras->petMarkOfTheWild) {
    buff_stats->stamina += 14;
    buff_stats->intellect += 14;
    buff_stats->strength += 14;
    buff_stats->agility += 14;
    stats->spirit += 14;
  }
  // todo improved imp
  if (player->selected_auras->bloodPact) {
    buff_stats->stamina += 70;
  }
  if (player->selected_auras->petArcaneIntellect) {
    buff_stats->intellect += 40;
  }
  if (player->selected_auras->petPrayerOfFortitude) {
    buff_stats->stamina += 79;
  }
  if (player->selected_auras->petPrayerOfSpirit) {
    stats->spirit += 50 * (1 + (0.1 * player->settings->improved_divine_spirit));
  }
  if (player->selected_auras->inspiringPresence) {
    stats->spell_hit_chance = std::min(99.0, stats->spell_hit_chance + 1);
  }
  if (player->selected_auras->moonkinAura) {
    buff_stats->spell_crit_chance += 5;
  }
  if (player->selected_auras->eyeOfTheNight) {
    buff_stats->spell_power += 34;
  }
  if (player->selected_auras->chainOfTheTwilightOwl) {
    buff_stats->spell_crit_chance += 2;
  }
  if (player->selected_auras->jadePendantOfBlasting) {
    buff_stats->spell_power += 15;
  }
  if (player->selected_auras->idolOfTheRavenGoddess) {
    buff_stats->spell_crit_chance += 20;
  }
  // Atiesh auras
  // Add 33sp if the player has Atiesh equipped since the aura's spell power is
  // just added to the item itself
  if (player->items->two_hand == 22630) {
    buff_stats->spell_power += 33;
  }
  if (player->selected_auras->powerOfTheGuardianWarlock) {
    buff_stats->spell_power += 33 * player->settings->warlock_atiesh_amount;
  }
  if (player->selected_auras->wrathOfAirTotem && player->settings->has_elemental_shaman_t4_bonus) {
    buff_stats->spell_power += 20;
  }
  // todo: do pets even get crit from crit rating buffs?
  if (player->selected_auras->powerOfTheGuardianMage) {
    buff_stats->spell_crit_chance += 28 * player->settings->mage_atiesh_amount;
  }
  if (player->selected_auras->judgementOfTheCrusader) {
    buff_stats->melee_crit_chance += 3;
    buff_stats->spell_crit_chance += 3;
  }
  // Multiply by 25% instead of 5% since it's happening every 5 seconds instead
  // of every 1 second
  if (player->selected_auras->vampiricTouch) {
    stats->mp5 += player->settings->shadow_priest_dps * 0.25;
  }
  if (player->selected_auras->faerieFire && player->settings->improved_faerie_fire) {
    stats->melee_hit_chance += 3;
  }
  if (player->selected_auras->exposeWeakness) {
    debuff_stats->attack_power +=
        (player->settings->survival_hunter_agility * 0.25 * (player->settings->expose_weakness_uptime / 100.0));
  }
  if (player->selected_auras->improvedHuntersMark) {
    debuff_stats->attack_power += 110;
  }
  if (player->selected_auras->petHeroicPresence) {
    stats->melee_hit_chance++;
  }
  if (player->selected_auras->petBlessingOfMight) {
    buff_stats->attack_power += 220;
  }
  if (player->selected_auras->petStrengthOfEarthTotem) {
    buff_stats->strength += 86;
  }
  if (player->selected_auras->petGraceOfAirTotem) {
    buff_stats->agility += 67;
  }
  if (player->selected_auras->petBattleShout) {
    buff_stats->attack_power += 306;
  }
  if (player->selected_auras->petTrueshotAura) {
    buff_stats->attack_power += 300;
  }
  if (player->selected_auras->petLeaderOfThePack) {
    buff_stats->melee_crit_chance += 5;
  }
  if (player->selected_auras->petUnleashedRage) {
    stats->attack_power_modifier *= 1.1;
  }
  if (player->selected_auras->petStaminaScroll) {
    buff_stats->stamina += 20;
  }
  if (player->selected_auras->petIntellectScroll) {
    buff_stats->intellect += 20;
  }
  if (player->selected_auras->petStrengthScroll) {
    buff_stats->strength += 20;
  }
  if (player->selected_auras->petAgilityScroll) {
    buff_stats->agility += 20;
  }
  if (player->selected_auras->petSpiritScroll) {
    stats->spirit += 20;
  }
  if (player->selected_auras->petKiblersBits) {
    buff_stats->strength += 20;
    stats->spirit += 20;
  }
  if (player->selected_auras->ferociousInspiration) {
    stats->damage_modifier *= std::pow(1.03, player->settings->ferocious_inspiration_amount);
  }
  if (player->settings->is_orc) {
    stats->damage_modifier *= 1.05;
  }

  // Hidden attack power modifiers (source: Max on warlock discord)
  if (pet == PetName::FELGUARD) {
    stats->attack_power_modifier *= 1.1;
  } else if (pet == PetName::SUCCUBUS) {
    stats->attack_power_modifier *= 1.05;
  }

  // Calculate armor
  if (pet_type == PetType::MELEE) {
    // Formula from
    // https://wowwiki-archive.fandom.com/wiki/Damage_reduction?oldid=807810
    if (player->settings->enemy_level <= 59) {
      armor_multiplier = 1 - player->settings->enemy_armor /
                                 (player->settings->enemy_armor + 400 + 85 * player->settings->enemy_level);
    } else {
      armor_multiplier = 1 - player->settings->enemy_armor /
                                 (player->settings->enemy_armor - 22167.5 + 467.5 * player->settings->enemy_level);
    }
  }
  armor_multiplier = std::max(0.25, armor_multiplier);
}

void Pet::CalculateStatsFromPlayer(bool announce_in_combat_log) {
  stats->stamina =
      base_stats->stamina + buff_stats->stamina + (0.3 * player->stats->stamina * player->stats->stamina_modifier);
  stats->intellect = base_stats->intellect + buff_stats->intellect +
                     (0.3 * player->stats->intellect * player->stats->intellect_modifier);
  player->demonic_knowledge_spell_power =
      ((stats->stamina * stats->stamina_modifier) + (stats->intellect * stats->intellect_modifier)) *
      (0.04 * player->talents->demonic_knowledge);
  base_stats->attack_power = (base_stats->strength + buff_stats->strength) * stats->strength_modifier * 2 - 20;
  stats->attack_power =
      base_stats->attack_power + buff_stats->attack_power + debuff_stats->attack_power +
      (player->GetSpellPower() + std::max(player->stats->shadow_power, player->stats->fire_power)) * 0.57;
  stats->agility = base_stats->agility + buff_stats->agility;
  stats->melee_crit_chance = player->talents->demonic_tactics + (stats->agility * stats->agility_modifier) * 0.04 +
                             0.65 + buff_stats->melee_crit_chance;
  stats->spell_power =
      buff_stats->spell_power +
      (player->GetSpellPower() + std::max(player->stats->shadow_power, player->stats->fire_power)) * 0.15;
  if (pet_type == PetType::MELEE) {
    // Halp, need confirmation that this is actually the right way to get its
    // average melee damage.
    dmg = (GetAttackPower() / 14 + 51.7) * base_melee_speed;
    stats->max_mana = base_stats->mana + stats->intellect * 11.555 * stats->intellect_modifier;
  } else if (pet == PetName::IMP) {
    stats->max_mana = base_stats->mana + stats->intellect * stats->intellect_modifier * 4.95;
  }
  if (pet == PetName::IMP || pet == PetName::SUCCUBUS) {
    stats->spell_crit_chance = 0.0125 * (stats->intellect * stats->intellect_modifier) + 0.91 +
                               player->talents->demonic_tactics + buff_stats->spell_crit_chance +
                               (buff_stats->spell_crit_chance / player->kCritRatingPerPercent);
  }
  if (announce_in_combat_log && player->ShouldWriteToCombatLog()) {
    player->CombatLog("Recalculated " + name + "'s stats");
  }
}

void Pet::Setup() {
  CalculateStatsFromAuras();
  CalculateStatsFromPlayer(false);
}

void Pet::reset() {
  stats->mana = stats->max_mana;
  five_second_rule_timer_remaining = 5;
  spirit_tick_timer_remaining = 2;
  cast_time_remaining = 0;

  // Reset spells
  if (spells->Melee != NULL) spells->Melee->Reset();
  if (spells->Firebolt != NULL) spells->Firebolt->Reset();
  if (spells->LashOfPain != NULL) spells->LashOfPain->Reset();
  if (spells->Cleave != NULL) spells->Cleave->Reset();

  // End Auras
  if (auras->black_book != NULL && auras->black_book->active) auras->black_book->Fade(true);
  if (auras->demonic_frenzy != NULL && auras->demonic_frenzy->active) auras->demonic_frenzy->Fade(true);

  CalculateStatsFromPlayer(false);
}

double Pet::GetMeleeCritChance() { return stats->melee_crit_chance - crit_suppression; }

double Pet::GetSpellCritChance() { return stats->spell_crit_chance; }

bool Pet::IsCrit(AttackType type) {
  if (type == AttackType::PHYSICAL) {
    return player->GetRand() <= GetMeleeCritChance() * player->kFloatNumberMultiplier;
  } else if (type == AttackType::MAGICAL) {
    return player->GetRand() <= GetSpellCritChance() * player->kFloatNumberMultiplier;
  }

  return false;
}

double Pet::GetMeleeHitChance() { return stats->melee_hit_chance; }

double Pet::GetSpellHitChance() { return std::min(99.0, stats->spell_hit_chance); }

bool Pet::IsHit(AttackType type) {
  if (type == AttackType::PHYSICAL) {
    return player->GetRand() <= GetMeleeHitChance() * player->kFloatNumberMultiplier;
  } else {
    return player->GetRand() <= GetSpellHitChance() * player->kFloatNumberMultiplier;
  }
}

int Pet::GetAttackPower() {
  // Remove AP from debuffs on the boss before multiplying by the AP multiplier
  // since it doesn't affect those debuffs
  double attack_power = (stats->attack_power - debuff_stats->attack_power) * stats->attack_power_modifier;
  if (auras->demonic_frenzy != NULL) {
    attack_power *= (1 + 0.05 * auras->demonic_frenzy->stacks);
  }
  // Add the AP from boss debuffs back and return
  attack_power += debuff_stats->attack_power;

  return attack_power;
}

void Pet::Tick(double t) {
  cast_time_remaining -= t;
  five_second_rule_timer_remaining -= t;
  spirit_tick_timer_remaining -= t;

  // Auras
  if (auras->black_book != NULL && auras->black_book->duration_remaining > 0) auras->black_book->Tick(t);

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
      if (pet == PetName::IMP) {
        mana_gain += ((base_stats->spirit + stats->spirit) * stats->spirit_modifier) +
                     0.7 * (stats->intellect * stats->intellect_modifier) - 258;
      } else if (pet == PetName::FELGUARD || pet == PetName::SUCCUBUS) {
        mana_gain += 0.75 * ((base_stats->spirit + stats->spirit) * stats->spirit_modifier) +
                     0.62 * (stats->intellect * stats->intellect_modifier) - 108;
      }
    }
    // Mana regen while the 5 second spirit regen timer is active (no bonus from
    // spirit)
    else {
      if (pet == PetName::IMP) {
        mana_gain += 0.375 * (stats->intellect * stats->intellect_modifier) - 123;
      } else if (pet == PetName::FELGUARD || pet == PetName::SUCCUBUS) {
        mana_gain += 0.365 * (stats->intellect * stats->intellect_modifier) - 48;
      }
    }

    int current_mana = stats->mana;

    stats->mana = std::min(stats->max_mana, stats->mana + static_cast<int>(mana_gain));
    if (stats->mana > current_mana && player->ShouldWriteToCombatLog()) {
      player->CombatLog(name + " gains " + std::to_string(round(mana_gain)) + " mana from Mp5/Spirit regeneration (" +
                        std::to_string(current_mana) + " -> " + std::to_string(stats->mana) + ")");
    }
  }
}

Imp::Imp(std::shared_ptr<Player> player) : Pet(player) {
  name = "Imp";
  pet = PetName::IMP;
  pet_type = PetType::RANGED;
  base_stats->stamina = 101;
  base_stats->intellect = 327;
  base_stats->mana = 756;
  base_stats->spirit = 263;
  base_stats->strength = 145;
  base_stats->agility = 38;
  Setup();
}

Succubus::Succubus(std::shared_ptr<Player> player) : Pet(player) {
  name = "Succubus";
  pet = PetName::SUCCUBUS;
  pet_type = PetType::MELEE;
  base_stats->stamina = 280;
  base_stats->intellect = 133;
  base_stats->mana = 849;
  base_stats->spirit = 122;
  base_stats->strength = 153;
  base_stats->agility = 109;
  base_melee_speed = 2;
  stats->damage_modifier *= 1 + (0.02 * player->talents->master_demonologist);
  Setup();
}

Felguard::Felguard(std::shared_ptr<Player> player) : Pet(player) {
  name = "Felguard";
  pet_type = PetType::MELEE;
  pet = PetName::FELGUARD;
  base_stats->stamina = 280;
  base_stats->strength = 153;
  base_stats->agility = 108;
  base_stats->intellect = 133;
  base_stats->spirit = 122;
  base_stats->mana = 893;
  base_melee_speed = 2;
  stats->damage_modifier *= 1 + (0.01 * player->talents->master_demonologist);
  Setup();
}