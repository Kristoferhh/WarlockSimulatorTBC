#include "pet_spell.h"

#include "../common.h"
#include "../player/player.h"

PetSpell::PetSpell(Pet& pet) : Spell(pet), pet(pet) {}

double PetSpell::GetBaseDamage() { return base_damage; }

void PetSpell::Cast() {
  Spell::Cast();

  // Physical dmg spell
  if (type == AttackType::kPhysical) {
    bool is_crit = false;
    bool is_glancing = false;
    double crit_chance = can_crit ? (pet.GetMeleeCritChance() * pet.kFloatNumberMultiplier) : 0;
    double dodge_chance = crit_chance + StatConstant::kBaseEnemyDodgeChance * pet.kFloatNumberMultiplier;
    double miss_chance = dodge_chance + (100 - pet.stats.melee_hit_chance) * pet.kFloatNumberMultiplier;
    double glancing_chance = miss_chance;

    // Only check for a glancing if it's a normal melee attack
    if (name == SpellName::kMelee) {
      glancing_chance += pet.glancing_blow_chance * pet.kFloatNumberMultiplier;
    }

    // Check whether the roll is a crit, dodge, miss, glancing, or just a normal
    // hit.
    int attack_roll = pet.player->GetRand();

    // Crit
    if (can_crit && attack_roll <= crit_chance) {
      is_crit = true;

      if (pet.recording_combat_log_breakdown) {
        pet.combat_log_breakdown.at(name)->crits++;
      }
    }
    // Dodge
    else if (attack_roll <= dodge_chance) {
      if (pet.recording_combat_log_breakdown) {
        pet.combat_log_breakdown.at(name)->dodge++;
      }

      if (pet.ShouldWriteToCombatLog()) {
        pet.CombatLog(pet.name + " " + name + " *dodge*");
      }

      return;
    }
    // Miss
    else if (attack_roll <= miss_chance) {
      if (pet.recording_combat_log_breakdown) {
        pet.combat_log_breakdown.at(name)->misses++;
      }

      if (pet.ShouldWriteToCombatLog()) {
        pet.CombatLog(pet.name + " " + name + " *miss*");
      }

      return;
    }
    // Glancing Blow
    else if (attack_roll <= glancing_chance && name == SpellName::kMelee) {
      is_glancing = true;

      if (pet.recording_combat_log_breakdown) {
        pet.combat_log_breakdown.at(name)->glancing_blows++;
      }
    }

    Damage(is_crit, is_glancing);
  }
  // Magic dmg spell
  else if (type == AttackType::kMagical) {
    // Check for resist
    if (!pet.IsHit(type)) {
      if (pet.recording_combat_log_breakdown) {
        pet.combat_log_breakdown.at(name)->misses++;
      }

      if (pet.ShouldWriteToCombatLog()) {
        pet.CombatLog(pet.name + " " + name + " *resist*");
      }
    } else {
      // Check for crit
      bool is_crit = false;

      if (can_crit && pet.IsCrit(type)) {
        is_crit = true;

        if (pet.recording_combat_log_breakdown) {
          pet.combat_log_breakdown.at(name)->crits++;
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
  double spell_power = pet.GetSpellPower();

  // Add damage from Spell Power
  if (type == AttackType::kMagical) {
    dmg += spell_power * coefficient;
  }

  // Multiply if it's a crit
  if (is_crit) {
    dmg *= pet.kCritDamageMultiplier;
  }

  // Magic damage multipliers
  if (type == AttackType::kMagical) {
    // Curse of the Elements
    if (pet.player->selected_auras.curse_of_the_elements &&
        (school == SpellSchool::kShadow || school == SpellSchool::kFire)) {
      damage_modifier *= (1.1 + 0.01 * pet.player->settings.improved_curse_of_the_elements);
    }

    // Misery
    if (pet.player->selected_auras.misery) {
      damage_modifier *= 1.05;
    }

    // Shadow Damage Multipliers
    if (school == SpellSchool::kShadow) {
      // Shadow Weaving
      if (pet.player->selected_auras.shadow_weaving) {
        damage_modifier *= 1.1;
      }

      // ISB
      if ((pet.player->auras.improved_shadow_bolt != NULL && pet.player->auras.improved_shadow_bolt->active) ||
          pet.player->settings.using_custom_isb_uptime) {
        if (pet.player->settings.using_custom_isb_uptime) {
          damage_modifier *= pet.player->GetCustomImprovedShadowBoltDamageModifier();
        } else {
          damage_modifier *= pet.player->auras.improved_shadow_bolt->modifier;
          pet.player->auras.improved_shadow_bolt->DecrementStacks();
        }
      }
    }
    // Fire Damage Multipliers
    else if (school == SpellSchool::kFire) {
      if (pet.player->selected_auras.improved_scorch) {
        damage_modifier *= 1.15;
      }
    }
  }
  // Physical Damage Multipliers
  else if (type == AttackType::kPhysical) {
    if (pet.player->selected_auras.blood_frenzy) {
      damage_modifier *= 1.04;
    }

    dmg *= pet.enemy_damage_reduction_from_armor;
  }

  // Pet Damage Modifier (from Unholy Power, Master Demonologist, etc.)
  damage_modifier *= pet.stats.damage_modifier;

  if (is_glancing) {
    dmg *= pet.glancing_blow_multiplier;
  }

  dmg *= damage_modifier;

  // Partial Resist Reduction
  double partial_resist_multiplier = pet.player->GetPartialResistMultiplier(school);
  if (type == AttackType::kMagical) {
    dmg *= partial_resist_multiplier;
  }

  pet.player->iteration_damage += dmg;

  if (pet.pet_name == PetName::kFelguard) {
    pet.auras.demonic_frenzy->Apply();
  }

  if (pet.recording_combat_log_breakdown) {
    pet.combat_log_breakdown.at(name)->iteration_damage += dmg;
  }

  std::string combat_log_message = pet.name + " " + name + " ";
  if (pet.ShouldWriteToCombatLog()) {
    if (is_crit) combat_log_message.append("*");
    combat_log_message.append(DoubleToString(round(dmg)));
    if (is_crit) combat_log_message.append("*");
    if (is_glancing) combat_log_message.append(" Glancing");
    combat_log_message.append(" (" + DoubleToString(round(base_damage)) + " Base Damage");
    if (type == AttackType::kMagical) {
      combat_log_message.append(" - " + DoubleToString(coefficient, 3) + " Coefficient");
      combat_log_message.append(" - " + DoubleToString(spell_power, 0) + " Spell Power");
      combat_log_message.append(" - " + DoubleToString(partial_resist_multiplier * 100) +
                                "% Partial Resist Multiplier");
    } else if (type == AttackType::kPhysical) {
      if (is_glancing)
        combat_log_message.append(" - " + DoubleToString(pet.glancing_blow_multiplier * 100, 1) +
                                  "% Glancing Blow Multiplier");
      combat_log_message.append(" - " + DoubleToString(round(pet.GetAttackPower())) + " Attack Power");
      combat_log_message.append(" - " +
                                DoubleToString(round(pet.enemy_damage_reduction_from_armor * 10000) / 100.0, 2) +
                                "% Damage Modifier (Armor)");
    }
    if (is_crit)
      combat_log_message.append(" - " + DoubleToString(pet.kCritDamageMultiplier * 100, 1) + "% Crit Multiplier");
    combat_log_message.append(" - " + DoubleToString(round(damage_modifier * 10000) / 100.0, 2) + "% Damage Modifier");
    combat_log_message.append(")");

    pet.CombatLog(combat_log_message);
  }
}

ImpFirebolt::ImpFirebolt(Pet& pet) : PetSpell(pet) {
  name = SpellName::kFirebolt;
  cast_time = 2 - (0.25 * pet.player->talents.improved_firebolt);
  mana_cost = 145;
  base_damage = 119.5 * (1 + 0.1 * pet.player->talents.improved_imp);
  coefficient = 2 / 3.5;
  school = SpellSchool::kFire;
  type = AttackType::kMagical;
  can_crit = true;
  Setup();
}

Melee::Melee(Pet& pet) : PetSpell(pet) {
  name = SpellName::kMelee;
  type = AttackType::kPhysical;
  cooldown = 2;
  on_gcd = false;
  can_crit = true;
  Setup();
}

double Melee::GetBaseDamage() { return (pet.GetAttackPower() / 14 + 51.7) * pet.kBaseMeleeSpeed; }

double Melee::GetCooldown() { return cooldown / pet.GetHastePercent(); }

FelguardCleave::FelguardCleave(Pet& pet) : PetSpell(pet) {
  name = SpellName::kCleave;
  cooldown = 6;
  mana_cost = 417;
  type = AttackType::kPhysical;
  can_crit = true;
  Setup();
}

double FelguardCleave::GetBaseDamage() { return pet.spells.melee->GetBaseDamage() + 78; }

SuccubusLashOfPain::SuccubusLashOfPain(Pet& pet) : PetSpell(pet) {
  name = SpellName::kLashOfPain;
  cooldown = 12 - 3 * pet.player->talents.improved_lash_of_pain;
  mana_cost = 190;
  base_damage = 123;
  school = SpellSchool::kShadow;
  coefficient = 0.429;
  type = AttackType::kMagical;
  can_crit = true;
  modifier *= 1 + pet.player->talents.improved_succubus / 10.0;
  can_crit = true;
  Setup();
}