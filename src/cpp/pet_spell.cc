#include "pet_spell.h"

#include "common.h"
#include "pet_aura.h"
#include "player.h"

PetSpell::PetSpell(std::shared_ptr<Pet> pet)
    : pet(pet),
      casting(false),
      can_crit(true),
      cooldown_remaining(0),
      cast_time(0),
      mana_cost(0),
      modifier(1),
      school(SpellSchool::kNoSchool),
      cooldown(0) {}

void PetSpell::Setup() {
  if (pet->player.recording_combat_log_breakdown && pet->player.combat_log_breakdown.count(name) == 0) {
    pet->player.combat_log_breakdown.insert({name, std::make_unique<CombatLogBreakdown>(name)});
  }
}

bool PetSpell::Ready() {
  return cooldown_remaining <= 0 && pet->stats.at(CharacterStat::kMana) >= mana_cost && pet->cast_time_remaining <= 0;
}

double PetSpell::GetBaseDamage() { return dmg; }

double PetSpell::GetCastTime() { return round((cast_time / pet->GetHastePercent()) * 10000) / 10000; }

double PetSpell::GetCooldown() { return cooldown; }

void PetSpell::Tick(double t) {
  if (pet->player.ShouldWriteToCombatLog() && cooldown_remaining > 0 && cooldown_remaining - t <= 0) {
    pet->player.CombatLog(pet->name + "'s " + name + " off cooldown");
  }

  cooldown_remaining -= t;

  if (casting && pet->cast_time_remaining <= 0) {
    casting = false;
    Cast();
  }
}

void PetSpell::StartCast() {
  // Error: Starting to cast a spell while casting another spell
  if (pet->cast_time_remaining > 0) {
    pet->player.ThrowError("Pet attempting to cast " + name + " while pet's cast time remaining is at " +
                           std::to_string(pet->cast_time_remaining) + " sec");
  }

  // Error: Casting a spell while it's on cooldown
  if (cooldown_remaining > 0) {
    pet->player.ThrowError("Pet attempting to cast " + name + " while it's still on cooldown (" +
                           std::to_string(cooldown_remaining) + " seconds remaining)");
  }

  if (cast_time > 0) {
    casting = true;
    pet->cast_time_remaining = GetCastTime();

    if (pet->player.ShouldWriteToCombatLog()) {
      pet->player.CombatLog(pet->name + " started casting " + name +
                            ". Cast time: " + DoubleToString(pet->cast_time_remaining, 4) + " (" +
                            DoubleToString(pet->GetHastePercent() * 100 - 100, 2) + "% haste at a base cast speed of " +
                            DoubleToString(cast_time, 2) + ")");
    }
  } else {
    Cast();
  }
}

void PetSpell::Reset() {
  cooldown_remaining = 0;
  casting = false;
}

void PetSpell::Cast() {
  cooldown_remaining = GetCooldown();

  std::string combat_log_message = pet->name;
  if (pet->player.ShouldWriteToCombatLog()) {
    if (cast_time > 0) {
      combat_log_message.append(" finished casting " + name);
    } else {
      combat_log_message.append(" casts " + name);

      if (pet->spells.melee != NULL && name == pet->spells.melee->name) {
        combat_log_message.append(" - Attack Speed: " + DoubleToString(pet->spells.melee->GetCooldown(), 2) + " (" +
                                  DoubleToString(round(pet->GetHastePercent() * 10000) / 100.0 - 100, 4) +
                                  "% haste at a base attack speed of " +
                                  DoubleToString(pet->spells.melee->cooldown, 2) + ")");
      }
    }
  }

  if (mana_cost > 0 && !pet->player.settings.infinite_pet_mana) {
    pet->stats.at(CharacterStat::kMana) -= mana_cost;
    pet->five_second_rule_timer_remaining = 5;

    if (pet->player.ShouldWriteToCombatLog()) {
      combat_log_message.append(" - Pet mana: " + std::to_string(pet->stats.at(CharacterStat::kMana)) + "/" +
                                std::to_string(pet->stats.at(CharacterStat::kMaxMana)));
    }
  }

  if (pet->player.recording_combat_log_breakdown) {
    pet->player.combat_log_breakdown.at(name)->casts++;
  }

  if (pet->player.ShouldWriteToCombatLog()) {
    pet->player.CombatLog(combat_log_message);
  }

  // Physical dmg spell
  if (type == AttackType::kPhysical) {
    bool is_crit = false;
    bool is_glancing = false;
    double crit_chance = pet->GetMeleeCritChance() * pet->player.kFloatNumberMultiplier;
    double dodge_chance = crit_chance + pet->enemy_dodge_chance * pet->player.kFloatNumberMultiplier;
    double miss_chance =
        dodge_chance + (100 - pet->stats.at(CharacterStat::kMeleeHitChance)) * pet->player.kFloatNumberMultiplier;
    double glancing_chance = miss_chance;

    // Only check for a glancing if it's a normal melee attack
    if (pet->spells.melee != NULL && name == pet->spells.melee->name) {
      glancing_chance += pet->glancing_blow_chance * pet->player.kFloatNumberMultiplier;
    }

    // Check whether the roll is a crit, dodge, miss, glancing, or just a normal
    // hit.
    // todo should maybe give the pet its own Random generator object
    int attack_roll = pet->player.GetRand();

    // Crit
    if (attack_roll <= crit_chance) {
      is_crit = true;
      if (pet->player.recording_combat_log_breakdown) {
        pet->player.combat_log_breakdown.at(name)->crits++;
      }
    }
    // Dodge
    else if (attack_roll <= dodge_chance) {
      if (pet->player.recording_combat_log_breakdown) {
        pet->player.combat_log_breakdown.at(name)->dodge++;
      }
      if (pet->player.ShouldWriteToCombatLog()) {
        pet->player.CombatLog(pet->name + " " + name + " *dodge*");
      }
      return;
    }
    // Miss
    else if (attack_roll <= miss_chance) {
      if (pet->player.recording_combat_log_breakdown) {
        pet->player.combat_log_breakdown.at(name)->misses++;
      }
      if (pet->player.ShouldWriteToCombatLog()) {
        pet->player.CombatLog(pet->name + " " + name + " *miss*");
      }
      return;
    }
    // Glancing Blow
    else if (attack_roll <= glancing_chance && pet->spells.melee != NULL && name == pet->spells.melee->name) {
      is_glancing = true;
      if (pet->player.recording_combat_log_breakdown) {
        pet->player.combat_log_breakdown.at(name)->glancing_blows++;
      }
    }

    Damage(is_crit, is_glancing);
  }
  // Magic dmg spell
  else if (type == AttackType::kMagical) {
    // Check for resist
    if (!pet->IsHit(type)) {
      if (pet->player.recording_combat_log_breakdown) {
        pet->player.combat_log_breakdown.at(name)->misses++;
      }
      if (pet->player.ShouldWriteToCombatLog()) {
        pet->player.CombatLog(pet->name + " " + name + " *resist*");
      }
    } else {
      // Check for crit
      bool is_crit = false;

      if (can_crit && pet->IsCrit(type)) {
        is_crit = true;
        if (pet->player.recording_combat_log_breakdown) {
          pet->player.combat_log_breakdown.at(name)->crits++;
        }
      }

      Damage(is_crit, false);
    }
  }
}

void PetSpell::Damage(bool is_crit, bool is_glancing) {
  double base_damage = GetBaseDamage();
  double dmg = base_damage;
  double damage_modifier = modifier;

  // Add damage from Spell Power
  if (type == AttackType::kMagical) {
    dmg += pet->stats.at(CharacterStat::kSpellPower) * coefficient;
  }

  // Multiply if it's a crit
  if (is_crit) {
    dmg *= pet->crit_multiplier;
  }

  // Magic damage multipliers
  if (type == AttackType::kMagical) {
    // Curse of the Elements
    if (pet->player.selected_auras.curse_of_the_elements &&
        (school == SpellSchool::kShadow || school == SpellSchool::kFire)) {
      damage_modifier *= (1.1 + 0.01 * pet->player.settings.improved_curse_of_the_elements);
    }

    // Misery
    if (pet->player.selected_auras.misery) {
      damage_modifier *= 1.05;
    }

    // Shadow Damage Multipliers
    if (school == SpellSchool::kShadow) {
      // Shadow Weaving
      if (pet->player.selected_auras.shadow_weaving) {
        damage_modifier *= 1.1;
      }

      // ISB
      if ((pet->player.auras.improved_shadow_bolt != NULL && pet->player.auras.improved_shadow_bolt->active) ||
          pet->player.settings.using_custom_isb_uptime) {
        // Custom ISB Uptime
        if (pet->player.settings.using_custom_isb_uptime) {
          damage_modifier *= (1 + 0.2 * (pet->player.settings.custom_isb_uptime_value / 100.0));
        }
        // Normal ISB
        else {
          damage_modifier *= pet->player.auras.improved_shadow_bolt->modifier;
          pet->player.auras.improved_shadow_bolt->DecrementStacks();
        }
      }
    }
    // Fire Damage Multipliers
    else if (school == SpellSchool::kFire) {
      // Improved Scorch
      if (pet->player.selected_auras.improved_scorch) {
        damage_modifier *= 1.15;
      }
    }
  }
  // Physical Damage Multipliers
  else if (type == AttackType::kPhysical) {
    // Blood Frenzy
    if (pet->player.selected_auras.blood_frenzy) {
      damage_modifier *= 1.04;
    }

    // Armor Damage Reduction
    dmg *= pet->armor_multiplier;
  }

  // Pet Damage Modifier (from Unholy Power, Master Demonologist, etc.)
  damage_modifier *= pet->stats.at(CharacterStat::kDamageModifier);

  if (is_glancing) {
    dmg *= pet->glancing_blow_multiplier;
  }

  dmg *= damage_modifier;

  // Partial Resist Reduction
  double partial_resist_multiplier = pet->player.GetPartialResistMultiplier(school);
  if (type == AttackType::kMagical) {
    dmg *= partial_resist_multiplier;
  }

  pet->player.iteration_damage += dmg;

  if (pet->pet == PetName::kFelguard) {
    pet->auras.demonic_frenzy->Apply();
  }

  if (pet->player.recording_combat_log_breakdown) {
    pet->player.AddIterationDamageAndMana(name, 0, dmg);
  }
  std::string combat_log_message = pet->name + " " + name + " ";
  if (pet->player.ShouldWriteToCombatLog()) {
    if (is_crit) combat_log_message.append("*");
    combat_log_message.append(DoubleToString(round(dmg)));
    if (is_crit) combat_log_message.append("*");
    if (is_glancing) combat_log_message.append(" Glancing");
    combat_log_message.append(" (" + DoubleToString(round(base_damage)) + " Base Damage");
    if (type == AttackType::kMagical) {
      combat_log_message.append(" - " + DoubleToString(coefficient, 3) + " Coefficient");
      combat_log_message.append(" - " + std::to_string(pet->stats.at(CharacterStat::kSpellPower)) + " Spell Power");
      combat_log_message.append(" - " + DoubleToString(partial_resist_multiplier * 100) +
                                "% Partial Resist Multiplier");
    } else if (type == AttackType::kPhysical) {
      if (is_glancing)
        combat_log_message.append(" - " + DoubleToString(pet->glancing_blow_multiplier * 100, 1) +
                                  "% Glancing Blow Multiplier");
      combat_log_message.append(" - " + DoubleToString(round(pet->GetAttackPower())) + " Attack Power");
      combat_log_message.append(" - " + DoubleToString(round(pet->armor_multiplier * 10000) / 100.0, 2) +
                                "% Damage Modifier (Armor)");
    }
    if (is_crit) combat_log_message.append(" - " + DoubleToString(pet->crit_multiplier * 100, 1) + "% Crit Multiplier");
    combat_log_message.append(" - " + DoubleToString(round(damage_modifier * 10000) / 100.0, 2) + "% Damage Modifier");
    combat_log_message.append(")");
    pet->player.CombatLog(combat_log_message);
  }
}

ImpFirebolt::ImpFirebolt(std::shared_ptr<Pet> pet) : PetSpell(pet) {
  name = "Firebolt";
  cast_time = 2 - (0.25 * pet->player.talents.improved_firebolt);
  mana_cost = 145;
  dmg = 119.5 * (1 + 0.1 * pet->player.talents.improved_imp);
  coefficient = 2 / 3.5;
  school = SpellSchool::kFire;
  type = AttackType::kMagical;
  Setup();
}

Melee::Melee(std::shared_ptr<Pet> pet) : PetSpell(pet) {
  cooldown = 2;
  name = "Melee";
  type = AttackType::kPhysical;
  Setup();
}

double Melee::GetBaseDamage() { return pet->dmg; }

double Melee::GetCooldown() { return round((cooldown / pet->GetHastePercent()) * 10000) / 10000.0; }

FelguardCleave::FelguardCleave(std::shared_ptr<Pet> pet) : PetSpell(pet) {
  cooldown = 6;
  mana_cost = 417;
  name = "Cleave";
  type = AttackType::kPhysical;
  Setup();
}

double FelguardCleave::GetBaseDamage() { return pet->spells.melee->GetBaseDamage() + 78; }

SuccubusLashOfPain::SuccubusLashOfPain(std::shared_ptr<Pet> pet) : PetSpell(pet) {
  cooldown = 12 - 3 * pet->player.talents.improved_lash_of_pain;
  mana_cost = 190;
  name = "Lash of Pain";
  dmg = 123;
  school = SpellSchool::kShadow;
  coefficient = 0.429;
  type = AttackType::kMagical;
  can_crit = true;
  modifier *= 1 + pet->player.talents.improved_succubus / 10.0;
  Setup();
}