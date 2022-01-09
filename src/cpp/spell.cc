#include "spell.h"

#include "common.h"
#include "entity.h"
#include "player/player.h"

Spell::Spell(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : entity(entity),
      aura_effect(aura),
      dot_effect(dot),
      min_dmg(0),
      max_dmg(0),
      base_damage(0),
      casting(false),
      can_crit(false),
      on_gcd(true),
      coefficient(0),
      cooldown_remaining(0),
      cast_time(0),
      modifier(1),
      cooldown(0),
      mana_cost(0),
      min_mana_gain(0),
      max_mana_gain(0),
      mana_gain(0),
      limited_amount_of_casts(false),
      amount_of_casts_this_fight(0),
      is_proc(false),
      is_non_warlock_ability(false),
      amount_of_casts_per_fight(0),
      does_damage(false),
      is_item(false),
      is_finisher(false),
      gain_mana_on_cast(false),
      procs_on_hit(false),
      on_hit_procs_enabled(true),
      procs_on_crit(false),
      on_crit_procs_enabled(true),
      can_miss(false),
      procs_on_dot_ticks(false),
      on_dot_tick_procs_enabled(true),
      procs_on_resist(false),
      on_resist_procs_enabled(true) {}

void Spell::Setup() {
  if (min_dmg > 0 && max_dmg > 0) {
    base_damage = (min_dmg + max_dmg) / 2.0;
  }

  if (min_mana_gain > 0 && max_mana_gain > 0) {
    mana_gain = (min_mana_gain + max_mana_gain) / 2.0;
  }

  if (entity.recording_combat_log_breakdown && entity.combat_log_breakdown.count(name) == 0) {
    entity.combat_log_breakdown.insert({name, std::make_unique<CombatLogBreakdown>(name)});
  }

  if (entity.entity_type == EntityType::kPlayer && spell_type == SpellType::kDestruction) {
    mana_cost *= 1 - 0.01 * entity.player->talents.cataclysm;
  }

  entity.spell_list.push_back(this);
}

void Spell::Reset() {
  casting = false;
  cooldown_remaining = 0;
  amount_of_casts_this_fight = 0;
}

bool Spell::Ready() { return CanCast() && HasEnoughMana(); }

bool Spell::HasEnoughMana() { return GetManaCost() <= entity.stats.mana; }

bool Spell::CanCast() {
  return cooldown_remaining <= 0 &&
         (is_non_warlock_ability ||
          ((!on_gcd || entity.gcd_remaining <= 0) && (is_proc || entity.cast_time_remaining <= 0))) &&
         (!limited_amount_of_casts || amount_of_casts_this_fight < amount_of_casts_per_fight);
}

double Spell::GetManaCost() { return mana_cost * entity.stats.mana_cost_modifier; }

double Spell::GetCastTime() { return cast_time / entity.GetHastePercent(); }

void Spell::Tick(double time) {
  if (cooldown_remaining > 0 && cooldown_remaining - time <= 0) {
    if (name == SpellName::kPowerInfusion) {
      entity.player->power_infusions_ready++;
    }

    if (entity.ShouldWriteToCombatLog()) {
      entity.CombatLog(entity.name + "'s " + name + " off cooldown");
    }
  }

  cooldown_remaining -= time;

  if (casting && entity.cast_time_remaining <= 0) {
    Cast();
  }
}

double Spell::GetCooldown() { return cooldown; }

void Spell::Cast() {
  const double kCurrentMana = entity.stats.mana;
  const double kManaCost = GetManaCost();
  cooldown_remaining = GetCooldown();
  casting = false;
  amount_of_casts_this_fight++;

  for (auto& spell_name : shared_cooldown_spells) {
    for (auto& player_spell : entity.spell_list) {
      if (player_spell->name == spell_name) {
        player_spell->cooldown_remaining = cooldown;
      }
    }
  }

  if (name == SpellName::kPowerInfusion) {
    entity.player->power_infusions_ready--;
  }

  if (aura_effect == NULL && entity.recording_combat_log_breakdown) {
    entity.combat_log_breakdown.at(name)->casts++;
  }

  if (mana_cost > 0 && !entity.infinite_mana) {
    entity.stats.mana -= kManaCost;
    entity.five_second_rule_timer_remaining = 5;
  }

  if (cast_time > 0 && entity.ShouldWriteToCombatLog()) {
    std::string msg = entity.name + " finished casting " + name;
    msg += " - Mana: " + DoubleToString(kCurrentMana) + " -> " + DoubleToString(entity.stats.mana);
    msg += " - Mana Cost: " + DoubleToString(round(kManaCost));

    if (entity.entity_type == EntityType::kPlayer) {
      msg += " - Mana Cost Modifier: " + DoubleToString(round(entity.stats.mana_cost_modifier * 100)) + "%";
    }

    entity.CombatLog(msg);
  }

  if (gain_mana_on_cast) {
    ManaGainOnCast();
  }

  SpellCastResult spell_cast_result = (attack_type == AttackType::kPhysical ? PhysicalSpellCast() : MagicSpellCast());

  if (spell_cast_result.is_miss || spell_cast_result.is_dodge) {
    return;
  }

  OnSpellHit(spell_cast_result);
}

void Spell::Damage(bool is_crit, bool is_glancing) {
  const std::vector<double> kConstantDamage = GetConstantDamage();
  const double kBaseDamage = kConstantDamage[0];
  double total_damage = kConstantDamage[1];
  const double kDamageModifier = kConstantDamage[2];
  const double kPartialResistMultiplier = kConstantDamage[3];
  const double kSpellPower = kConstantDamage[4];
  double crit_multiplier = entity.kCritDamageMultiplier;

  if (is_crit) {
    crit_multiplier = GetCritMultiplier(crit_multiplier);
    total_damage *= crit_multiplier;
    OnCritProcs();
  } else if (spell_school == SpellSchool::kShadow && dot_effect == NULL &&
             entity.player->auras.improved_shadow_bolt != NULL && entity.player->auras.improved_shadow_bolt->active &&
             !entity.player->settings.using_custom_isb_uptime) {
    entity.player->auras.improved_shadow_bolt->DecrementStacks();
  }

  if (is_glancing) {
    total_damage *= entity.pet->glancing_blow_multiplier;
  }

  OnDamageProcs();
  entity.player->iteration_damage += total_damage;

  if (entity.recording_combat_log_breakdown) {
    entity.combat_log_breakdown.at(name)->iteration_damage += total_damage;
  }

  if (entity.ShouldWriteToCombatLog()) {
    CombatLogDamage(attack_type, is_crit, is_glancing, total_damage, kBaseDamage, kSpellPower, crit_multiplier,
                    kDamageModifier, kPartialResistMultiplier);
  }

  // T5 4pc
  if (entity.entity_type == EntityType::kPlayer && entity.player->sets.t5 >= 4) {
    if (name == SpellName::kShadowBolt && entity.player->auras.corruption != NULL &&
        entity.player->auras.corruption->active) {
      entity.player->auras.corruption->t5_bonus_modifier *= 1.1;
    } else if (name == SpellName::kIncinerate && entity.player->auras.immolate != NULL &&
               entity.player->auras.immolate->active) {
      entity.player->auras.immolate->t5_bonus_modifier *= 1.1;
    }
  }
}

// Returns the non-RNG Damage of the spell (basically just the base Damage +
// spell power + Damage modifiers, no crit/miss etc.)
std::vector<double> Spell::GetConstantDamage() {
  double total_damage = GetBaseDamage();
  const double kBaseDamage = total_damage;
  const double kSpellPower = entity.GetSpellPower(true, spell_school);
  const double kDamageModifier = GetModifier();
  const double kPartialResistMultiplier = entity.GetPartialResistMultiplier(spell_school);

  // If casting Incinerate and Immolate is up, add the bonus Damage
  if (name == SpellName::kIncinerate && entity.player->auras.immolate != NULL &&
      entity.player->auras.immolate->active) {
    if (entity.player->settings.randomize_values && bonus_damage_from_immolate_min > 0 &&
        bonus_damage_from_immolate_max > 0) {
      total_damage += entity.player->rng.range(bonus_damage_from_immolate_min, bonus_damage_from_immolate_max);
    } else {
      total_damage += bonus_damage_from_immolate;
    }
  }

  if (attack_type == AttackType::kMagical) {
    total_damage += kSpellPower * coefficient;
  }

  total_damage *= kDamageModifier;

  if (attack_type == AttackType::kMagical) {
    total_damage *= kPartialResistMultiplier;
  }

  if (attack_type == AttackType::kPhysical) {
    total_damage *= entity.pet->enemy_damage_reduction_from_armor;
  }

  return std::vector<double>{kBaseDamage, total_damage, kDamageModifier, kPartialResistMultiplier, kSpellPower};
}

double Spell::GetCritMultiplier(double player_crit_multiplier) {
  double crit_multiplier = player_crit_multiplier;

  // Chaotic Skyfire Diamond
  if (entity.player->settings.meta_gem_id == 34220) {
    crit_multiplier *= 1.03;
  }

  // Ruin
  if (spell_type == SpellType::kDestruction && entity.player->talents.ruin == 1) {
    // Ruin doubles the *bonus* of your crits, not the Damage of the crit itself
    // So if your crit Damage % is e.g. 154.5% it would become 209% because
    // the 54.5% is being doubled
    crit_multiplier -= 1;
    crit_multiplier *= 2;
    crit_multiplier += 1;
  }

  return crit_multiplier;
}

double Spell::GetBaseDamage() {
  if (entity.player->settings.randomize_values && min_dmg > 0 && max_dmg > 0) {
    return entity.player->rng.range(min_dmg, max_dmg);
  } else {
    return base_damage;
  }
}

double Spell::PredictDamage() {
  const std::vector<double> kConstantDamage = GetConstantDamage();
  const double kNormalDamage = kConstantDamage[1];
  double crit_damage = 0;
  double crit_chance = 0;
  double chance_to_not_crit = 0;

  if (can_crit) {
    crit_damage = kNormalDamage * GetCritMultiplier(entity.kCritDamageMultiplier);
    crit_chance = entity.GetSpellCritChance(spell_type) / 100;
    chance_to_not_crit = 1 - crit_chance;
  }

  double hit_chance = entity.GetSpellHitChance(spell_type) / 100;
  double estimated_damage =
      can_crit ? (kNormalDamage * chance_to_not_crit) + (crit_damage * crit_chance) : kNormalDamage;

  // Add the predicted Damage of the DoT over its full duration
  if (dot_effect != NULL) {
    estimated_damage += dot_effect->PredictDamage();
  }

  // If the player is not using a custom ISB uptime, they have the ISB talent
  // selected, but the ISB aura is not active, then give some % modifier as an
  // "average" for the Damage. Without this, the sim will choose Incinerate over
  // Shadow Bolt because it basically just doesn't know that ISB exists
  if (spell_school == SpellSchool::kShadow && !entity.player->settings.using_custom_isb_uptime &&
      entity.player->auras.improved_shadow_bolt != NULL && !entity.player->auras.improved_shadow_bolt->active) {
    estimated_damage *= 1.15;  // using 75% uptime as the default for now
  }

  return (estimated_damage * hit_chance) / std::max(entity.GetGcdValue(), GetCastTime());
}

void Spell::OnCritProcs() {
  for (auto& proc : entity.on_crit_procs) {
    if (proc->Ready() && proc->ShouldProc(this) && entity.player->RollRng(proc->proc_chance)) {
      proc->StartCast();
    }
  }
}

void Spell::OnResistProcs() {
  for (auto& proc : entity.on_resist_procs) {
    if (proc->Ready() && proc->ShouldProc(this) && entity.player->RollRng(proc->proc_chance)) {
      proc->StartCast();
    }
  }
}

void Spell::OnDamageProcs() {
  for (auto& proc : entity.on_damage_procs) {
    if (proc->Ready() && proc->ShouldProc(this) && entity.player->RollRng(proc->proc_chance)) {
      proc->StartCast();
    }
  }
}

void Spell::OnHitProcs() {
  for (auto& proc : entity.on_hit_procs) {
    if (proc->Ready() && proc->ShouldProc(this) && entity.player->RollRng(proc->proc_chance)) {
      proc->StartCast();
    }
  }
}

double Spell::GetModifier() {
  double damage_modifier = modifier * entity.stats.damage_modifier;

  if (attack_type == AttackType::kMagical) {
    if (spell_school == SpellSchool::kShadow) {
      damage_modifier *= entity.stats.shadow_modifier;

      if (!entity.player->settings.using_custom_isb_uptime && entity.player->auras.improved_shadow_bolt != NULL &&
          entity.player->auras.improved_shadow_bolt->active) {
        damage_modifier *= entity.player->auras.improved_shadow_bolt->modifier;
      }
    } else if (spell_school == SpellSchool::kFire) {
      damage_modifier *= entity.stats.fire_modifier;
    }
  } else if (attack_type == AttackType::kPhysical) {
    damage_modifier *= entity.stats.physical_modifier;
  }

  return damage_modifier;
}

void Spell::StartCast(double predicted_damage) {
  if (on_gcd && !is_non_warlock_ability) {
    // Error: Casting a spell while GCD is active
    if (entity.gcd_remaining > 0) {
      entity.player->ThrowError(entity.name + " attempting to cast " + name + " while " + entity.name +
                                "'s GCD is at " + std::to_string(entity.gcd_remaining) + " seconds remaining");
    }

    entity.gcd_remaining = entity.GetGcdValue();
  }

  // Error: Starting to Cast a spell while casting another spell
  if (entity.cast_time_remaining > 0 && !is_non_warlock_ability && !is_proc) {
    entity.player->ThrowError(entity.name + " attempting to cast " + name + " while " + entity.name +
                              "'s cast time remaining is at " + std::to_string(entity.cast_time_remaining) + " sec");
  }

  // Error: Casting a spell while it's on cooldown
  if (cooldown > 0 && cooldown_remaining > 0) {
    entity.player->ThrowError(entity.name + " attempting to cast " + name + " while it's still on cooldown (" +
                              std::to_string(cooldown_remaining) + " seconds remaining)");
  }

  std::string combat_log_message = "";
  if (cast_time > 0) {
    casting = true;
    entity.cast_time_remaining = GetCastTime();

    if (!is_proc && entity.ShouldWriteToCombatLog()) {
      combat_log_message.append(entity.name + " started casting " + name +
                                " - Cast time: " + DoubleToString(entity.cast_time_remaining, 4) + " (" +
                                DoubleToString((entity.GetHastePercent() - 1) * 100, 4) +
                                "% haste at a base Cast speed of " + DoubleToString(cast_time, 2) + ")");
    }
  } else {
    if (!is_proc && entity.ShouldWriteToCombatLog()) {
      combat_log_message.append(entity.name + " casts " + name);

      if (name == SpellName::kMelee) {
        combat_log_message.append(" - Attack Speed: " + DoubleToString(GetCooldown(), 2) + " (" +
                                  DoubleToString(round(entity.GetHastePercent() * 10000) / 100.0 - 100, 4) +
                                  "% haste at a base attack speed of " + DoubleToString(cooldown, 2) + ")");
      }
    }

    Cast();
  }

  if (on_gcd && !is_non_warlock_ability && entity.ShouldWriteToCombatLog()) {
    combat_log_message.append(" - Global cooldown: " + DoubleToString(entity.gcd_remaining, 4));
  }

  if (predicted_damage > 0 && entity.ShouldWriteToCombatLog()) {
    combat_log_message.append(" - Estimated Damage / Cast time: " + DoubleToString(round(predicted_damage)));
  }

  if (combat_log_message.length() > 0) {
    entity.CombatLog(combat_log_message);
  }
}

SpellCastResult Spell::MagicSpellCast() {
  bool is_crit = false;
  bool is_resist = can_miss && (attack_type == AttackType::kMagical && !entity.IsSpellHit(spell_type));

  if (can_crit) {
    is_crit = entity.IsSpellCrit(spell_type, bonus_crit_chance);

    if (is_crit && entity.recording_combat_log_breakdown) {
      // Increment the crit counter whether the spell hits or not so that the
      // crit % on the Damage breakdown is correct. Otherwise the crit % will be
      // lower due to lost crits when the spell misses.
      entity.combat_log_breakdown.at(name)->crits++;
    }
  }

  if (is_resist) {
    if (entity.ShouldWriteToCombatLog()) {
      entity.CombatLog(name + " *resist*");
    }

    if (entity.recording_combat_log_breakdown) {
      entity.combat_log_breakdown.at(name)->misses++;
    }

    OnResistProcs();
  }

  return SpellCastResult(is_resist, is_crit);
}

SpellCastResult Spell::PhysicalSpellCast() {
  bool is_crit = false;
  bool is_glancing = false;
  bool is_miss = false;
  bool is_dodge = false;
  double crit_chance = can_crit ? (entity.GetMeleeCritChance() * entity.kFloatNumberMultiplier) : 0;
  double dodge_chance = crit_chance + StatConstant::kBaseEnemyDodgeChance * entity.kFloatNumberMultiplier;
  double miss_chance = dodge_chance + (100 - entity.stats.melee_hit_chance) * entity.kFloatNumberMultiplier;
  double glancing_chance = miss_chance;

  // Only check for a glancing if it's a normal melee attack
  if (name == SpellName::kMelee) {
    glancing_chance += entity.pet->glancing_blow_chance * entity.kFloatNumberMultiplier;
  }

  // Check whether the roll is a crit, dodge, miss, glancing, or just a normal hit.
  int attack_roll = entity.player->GetRand();

  // Crit
  if (can_crit && attack_roll <= crit_chance) {
    is_crit = true;

    if (entity.recording_combat_log_breakdown) {
      entity.combat_log_breakdown.at(name)->crits++;
    }
  }
  // Dodge
  else if (attack_roll <= dodge_chance) {
    is_dodge = true;

    if (entity.recording_combat_log_breakdown) {
      entity.combat_log_breakdown.at(name)->dodge++;
    }

    if (entity.ShouldWriteToCombatLog()) {
      entity.CombatLog(entity.name + " " + name + " *dodge*");
    }
  }
  // Miss
  else if (attack_roll <= miss_chance) {
    is_miss = true;

    if (entity.recording_combat_log_breakdown) {
      entity.combat_log_breakdown.at(name)->misses++;
    }

    if (entity.ShouldWriteToCombatLog()) {
      entity.CombatLog(entity.name + " " + name + " *miss*");
    }
  }
  // Glancing Blow
  else if (attack_roll <= glancing_chance && name == SpellName::kMelee) {
    is_glancing = true;

    if (entity.recording_combat_log_breakdown) {
      entity.combat_log_breakdown.at(name)->glancing_blows++;
    }
  }

  return SpellCastResult(is_miss, is_crit, is_glancing, is_dodge);
}

void Spell::OnSpellHit(SpellCastResult& spell_cast_result) {
  if (aura_effect != NULL) {
    aura_effect->Apply();
  }

  if (dot_effect != NULL) {
    dot_effect->Apply();
  }

  if (does_damage) {
    Damage(spell_cast_result.is_crit, spell_cast_result.is_glancing);
  }

  if (!is_item && !is_proc && !is_non_warlock_ability && name != SpellName::kAmplifyCurse) {
    OnHitProcs();
  }
}

void Spell::CombatLogDamage(AttackType attack_type, bool is_crit, bool is_glancing, double total_damage,
                            double base_damage, double spell_power, double crit_multiplier, double damage_modifier,
                            double partial_resist_multiplier) {
  std::string msg = name + " ";

  if (is_crit) {
    msg += "*";
  }

  msg += DoubleToString(round(total_damage));

  if (is_crit) {
    msg += "*";
  }

  if (is_glancing) {
    msg += " Glancing";
  }

  msg += " (" + DoubleToString(base_damage, 1) + " Base Damage";

  if (attack_type == AttackType::kMagical) {
    msg += " - " + DoubleToString(round(coefficient * 1000) / 1000, 3) + " Coefficient";
    msg += " - " + DoubleToString(round(spell_power)) + " Spell Power";
    msg += " - " + DoubleToString(round(partial_resist_multiplier * 1000) / 10) + "% Partial Resist Multiplier)";
  } else if (attack_type == AttackType::kPhysical) {
    if (is_glancing) {
      msg += " - " + DoubleToString(entity.pet->glancing_blow_multiplier * 100, 1) + "% Glancing Blow Multiplier";
    }
    msg += " - " + DoubleToString(round(entity.pet->GetAttackPower())) + " Attack Power";
    msg += " - " + DoubleToString(round(entity.pet->enemy_damage_reduction_from_armor * 10000) / 100.0, 2) +
           "% Damage Modifier (Armor)";
  }

  if (is_crit) {
    msg += " - " + DoubleToString(crit_multiplier * 100, 3) + "% Crit Multiplier";
  }

  msg += " - " + DoubleToString(round(damage_modifier * 10000) / 100, 2) + "% Damage Modifier";

  entity.CombatLog(msg);
}

void Spell::ManaGainOnCast() {
  const double kCurrentMana = entity.stats.mana;

  entity.stats.mana = std::min(static_cast<double>(entity.stats.max_mana), kCurrentMana + mana_gain);
  const double kManaGained = entity.stats.mana - kCurrentMana;

  if (entity.recording_combat_log_breakdown) {
    entity.combat_log_breakdown.at(name)->iteration_mana_gain += kManaGained;
  }

  if (entity.ShouldWriteToCombatLog()) {
    entity.CombatLog("Player gains " + DoubleToString(kManaGained) + " mana from " + name + " (" +
                     DoubleToString(kCurrentMana) + " -> " + DoubleToString(entity.stats.mana) + ")");
  }
}

ShadowBolt::ShadowBolt(Entity& entity) : Spell(entity) {
  name = SpellName::kShadowBolt;
  cast_time = CalculateCastTime();
  mana_cost = 420;
  coefficient = (3 / 3.5) + (0.04 * entity.player->talents.shadow_and_flame);
  min_dmg = 544;
  max_dmg = 607;
  does_damage = true;
  can_crit = true;
  spell_school = SpellSchool::kShadow;
  spell_type = SpellType::kDestruction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();

  // T6 4pc bonus
  if (entity.player->sets.t6 >= 4) {
    modifier *= 1.06;
  }
}

void ShadowBolt::StartCast(double predicted_damage = 0) {
  const bool kHasShadowTrance = entity.player->auras.shadow_trance != NULL;

  if (kHasShadowTrance && entity.player->auras.shadow_trance->active) {
    cast_time = 0;
  }

  Spell::StartCast();

  if (kHasShadowTrance && entity.player->auras.shadow_trance->active) {
    cast_time = CalculateCastTime();
    entity.player->auras.shadow_trance->Fade();
  }
}

double ShadowBolt::CalculateCastTime() { return 3 - (0.1 * entity.player->talents.bane); }

Incinerate::Incinerate(Entity& entity) : Spell(entity) {
  name = SpellName::kIncinerate;
  cast_time = 2.5 * (1 - 0.02 * entity.player->talents.emberstorm);
  mana_cost = 355;
  coefficient = (2.5 / 3.5) + (0.04 * entity.player->talents.shadow_and_flame);
  min_dmg = 444;
  max_dmg = 514;
  bonus_damage_from_immolate_min = 111;
  bonus_damage_from_immolate_max = 128;
  bonus_damage_from_immolate = (bonus_damage_from_immolate_min + bonus_damage_from_immolate_max) / 2.0;
  does_damage = true;
  can_crit = true;
  spell_school = SpellSchool::kFire;
  spell_type = SpellType::kDestruction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();

  if (entity.player->sets.t6 >= 4) {
    modifier *= 1.06;
  }
}

SearingPain::SearingPain(Entity& entity) : Spell(entity) {
  name = SpellName::kSearingPain;
  cast_time = 1.5;
  mana_cost = 205;
  coefficient = 1.5 / 3.5;
  min_dmg = 270;
  max_dmg = 320;
  does_damage = true;
  can_crit = true;
  spell_school = SpellSchool::kFire;
  spell_type = SpellType::kDestruction;
  bonus_crit_chance = 4 * entity.player->talents.improved_searing_pain;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
};

SoulFire::SoulFire(Entity& entity) : Spell(entity) {
  name = SpellName::kSoulFire;
  cast_time = 6 - (0.4 * entity.player->talents.bane);
  mana_cost = 250;
  coefficient = 1.15;
  min_dmg = 1003;
  max_dmg = 1257;
  does_damage = true;
  can_crit = true;
  spell_school = SpellSchool::kFire;
  spell_type = SpellType::kDestruction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
};

Shadowburn::Shadowburn(Entity& entity) : Spell(entity) {
  name = SpellName::kShadowburn;
  cooldown = 15;
  mana_cost = 515;
  coefficient = 0.22;
  min_dmg = 597;
  max_dmg = 665;
  does_damage = true;
  can_crit = true;
  is_finisher = true;
  spell_school = SpellSchool::kShadow;
  spell_type = SpellType::kDestruction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
};

DeathCoil::DeathCoil(Entity& entity) : Spell(entity) {
  name = SpellName::kDeathCoil;
  cooldown = 120;
  mana_cost = 600;
  coefficient = 0.4286;
  base_damage = 526;
  does_damage = true;
  is_finisher = true;
  spell_school = SpellSchool::kShadow;
  spell_type = SpellType::kAffliction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
};

Shadowfury::Shadowfury(Entity& entity) : Spell(entity) {
  name = SpellName::kShadowfury;
  cast_time = 0.5;
  mana_cost = 710;
  min_dmg = 612;
  max_dmg = 728;
  does_damage = true;
  can_crit = true;
  spell_school = SpellSchool::kShadow;
  spell_type = SpellType::kDestruction;
  cooldown = 20;
  coefficient = 0.195;
  can_miss = true;
  on_gcd = false;
  attack_type = AttackType::kMagical;
  Setup();
}

SeedOfCorruption::SeedOfCorruption(Entity& entity) : Spell(entity) {
  name = SpellName::kSeedOfCorruption;
  min_dmg = 1110;
  max_dmg = 1290;
  mana_cost = 882;
  cast_time = 2;
  aoe_cap = 13580;
  does_damage = true;
  spell_school = SpellSchool::kShadow;
  spell_type = SpellType::kAffliction;
  coefficient = 0.214;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
};

void SeedOfCorruption::Damage(bool is_crit) {
  const double kBaseDamage = entity.player->settings.randomize_values && min_dmg > 0 && max_dmg > 0
                                 ? entity.player->rng.range(min_dmg, max_dmg)
                                 : base_damage;
  const int kEnemyAmount = entity.player->settings.enemy_amount - 1;  // Minus one because the enemy that Seed is
                                                                      // being Cast on doesn't get hit
  int resist_amount = 0;
  int crit_amount = 0;
  const double kSpellPower = entity.GetSpellPower(true, spell_school);
  double crit_damage_multiplier = 0;
  double internal_modifier = GetModifier();
  double external_modifier = 1;

  // If using a custom ISB uptime % then remove the damage bonus since ISB won't be up on the mobs
  if (entity.player->settings.using_custom_isb_uptime) {
    internal_modifier /= entity.GetCustomImprovedShadowBoltDamageModifier();
  }

  // Remove debuffs from the modifier since they ignore the AOE cap, so we'll
  // add the debuff % modifiers after the Damage has been calculated.
  if (entity.player->selected_auras.curse_of_the_elements) {
    const double kModifier = 1.1 + (0.01 * entity.player->settings.improved_curse_of_the_elements);
    internal_modifier /= kModifier;
    external_modifier *= kModifier;
  }
  if (entity.player->selected_auras.shadow_weaving) {
    const double kModifier = 1.1;
    internal_modifier /= kModifier;
    external_modifier *= kModifier;
  }
  if (entity.player->selected_auras.misery) {
    const double kModifier = 1.05;
    internal_modifier /= kModifier;
    external_modifier *= kModifier;
  }

  for (int i = 0; i < kEnemyAmount; i++) {
    // Check for a resist
    if (!entity.IsSpellHit(spell_type)) {
      resist_amount++;
      OnResistProcs();
    } else {
      OnDamageProcs();
      // Check for a crit
      if (entity.IsSpellCrit(spell_type)) {
        crit_amount++;
        OnCritProcs();
      }
    }
  }

  double individual_seed_damage = kBaseDamage + (kSpellPower * coefficient);
  // Oblivion Raiment (dungeon set) 4pc bonus
  if (entity.player->sets.oblivion >= 4) {
    individual_seed_damage += 180;
  }
  individual_seed_damage *= internal_modifier;

  const int kEnemiesHit = kEnemyAmount - resist_amount;
  double total_seed_damage = individual_seed_damage * kEnemiesHit;
  // Because of the Seed bug explained below, we need to use this formula to
  // calculate the actual aoe cap for the amount of mobs that will be hit by the
  // spell. Explained by Tesram on the TBC Warlock discord
  // https://discord.com/channels/253210018697052162/825310481358651432/903413703595143218
  const int kTrueAoeCap = (aoe_cap * kEnemiesHit) / (kEnemiesHit + 1);
  // If the total Damage goes above the aoe cap then we need to reduce the
  // amount of each seed's Damage
  if (total_seed_damage > kTrueAoeCap) {
    // Set the Damage of each individual seed to the "true" aoe cap divided by
    // the amount of enemies hit There's a bug with Seed of Corruption where if
    // you hit the AoE cap, the number used to divide here is 1 higher because
    // it's including the enemy that Seed is being Cast on, even though that
    // enemy doesn't actually get damaged by the Seed. Nice game :)
    individual_seed_damage = kTrueAoeCap / kEnemiesHit;
    // Re-calculate the total Damage done by all seed hits
    total_seed_damage = individual_seed_damage * kEnemiesHit;
  }
  // Add Damage from Seed crits
  if (crit_amount > 0) {
    crit_damage_multiplier = GetCritMultiplier(entity.kCritDamageMultiplier);
    const double kIndividualSeedCrit = individual_seed_damage * crit_damage_multiplier;
    const double kBonusDamageFromCrit = kIndividualSeedCrit - individual_seed_damage;
    total_seed_damage += kBonusDamageFromCrit * crit_amount;
  }
  // Partial resists (probably need to calculate a partial resist for each seed
  // hit, not sure how it interacts for the aoe cap)
  const double kPartialResistMultiplier = entity.GetPartialResistMultiplier(spell_school);
  total_seed_damage *= kPartialResistMultiplier;

  // Add Damage from debuffs
  total_seed_damage *= external_modifier;

  entity.player->iteration_damage += total_seed_damage;

  if (entity.ShouldWriteToCombatLog()) {
    std::string msg = name + " " + DoubleToString(round(total_seed_damage)) + " (" + std::to_string(kEnemyAmount) +
                      " Enemies (" + std::to_string(resist_amount) + " Resists & " + std::to_string(crit_amount) +
                      " Crits) - " + DoubleToString(kBaseDamage, 1) + " Base Damage - " +
                      DoubleToString(coefficient, 3) + " Coefficient - " + DoubleToString(kSpellPower) +
                      " Spell Power - " + DoubleToString(round(internal_modifier * external_modifier * 1000) / 10, 1) +
                      "% Modifier - ";
    if (crit_amount > 0) {
      msg += DoubleToString(crit_damage_multiplier, 3) + "% Crit Multiplier";
    }
    msg += " - " + DoubleToString(round(kPartialResistMultiplier * 1000) / 10) + "% Partial Resist Multiplier)";
    entity.CombatLog(msg);
  }
  if (entity.recording_combat_log_breakdown) {
    entity.combat_log_breakdown.at(name)->iteration_damage += total_seed_damage;
    entity.combat_log_breakdown.at(name)->crits += crit_amount;
    entity.combat_log_breakdown.at(name)->misses += resist_amount;
    // the Cast() function already adds 1 to the amount of casts so we only need
    // to add enemiesHit - 1 to the Cast amount
    entity.combat_log_breakdown.at(name)->casts += (kEnemiesHit - 1);
  }
}

double SeedOfCorruption::GetModifier() {
  double modifier = Spell::GetModifier();
  if (entity.player->talents.shadow_mastery > 0 && entity.player->talents.contagion > 0) {
    // Divide away the bonus from Shadow Mastery
    modifier /= (1 + (entity.player->talents.shadow_mastery * 0.02));
    // Multiply the modifier with the bonus from Shadow Mastery + Contagion
    modifier *=
        (1 * (1 + ((entity.player->talents.shadow_mastery * 0.02) + (entity.player->talents.contagion / 100.0))));
  }
  return modifier;
}

Corruption::Corruption(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(entity, aura, dot) {
  name = SpellName::kCorruption;
  mana_cost = 370;
  cast_time = 2 - (0.4 * entity.player->talents.improved_corruption);
  spell_school = SpellSchool::kShadow;
  spell_type = SpellType::kAffliction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
}

UnstableAffliction::UnstableAffliction(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(entity, aura, dot) {
  name = SpellName::kUnstableAffliction;
  mana_cost = 400;
  cast_time = 1.5;
  spell_school = SpellSchool::kShadow;
  spell_type = SpellType::kAffliction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
}

SiphonLife::SiphonLife(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(entity, aura, dot) {
  name = SpellName::kSiphonLife;
  mana_cost = 410;
  spell_school = SpellSchool::kShadow;
  spell_type = SpellType::kAffliction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
}

Immolate::Immolate(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(entity, aura, dot) {
  name = SpellName::kImmolate;
  mana_cost = 445;
  cast_time = 2 - (0.1 * entity.player->talents.bane);
  does_damage = true;
  can_crit = true;
  base_damage = 331;
  coefficient = 0.2;
  spell_school = SpellSchool::kFire;
  spell_type = SpellType::kDestruction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
}

double Immolate::GetModifier() {
  double modifier = Spell::GetModifier();
  if (entity.player->talents.emberstorm > 0) {
    modifier /= (1 + 0.02 * entity.player->talents.emberstorm);
  }
  modifier *= (1 + (0.02 * entity.player->talents.emberstorm + 0.05 * entity.player->talents.improved_immolate));
  return modifier;
}

CurseOfAgony::CurseOfAgony(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(entity, aura, dot) {
  name = SpellName::kCurseOfAgony;
  mana_cost = 265;
  spell_school = SpellSchool::kShadow;
  spell_type = SpellType::kAffliction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
}

CurseOfTheElements::CurseOfTheElements(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kCurseOfTheElements;
  mana_cost = 260;
  spell_type = SpellType::kAffliction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
}

CurseOfRecklessness::CurseOfRecklessness(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kCurseOfRecklessness;
  mana_cost = 160;
  spell_type = SpellType::kAffliction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
}

CurseOfDoom::CurseOfDoom(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(entity, aura, dot) {
  name = SpellName::kCurseOfDoom;
  mana_cost = 380;
  cooldown = 60;
  spell_school = SpellSchool::kShadow;
  spell_type = SpellType::kAffliction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
}

Conflagrate::Conflagrate(Entity& entity) : Spell(entity) {
  name = SpellName::kConflagrate;
  mana_cost = 305;
  cooldown = 10;
  min_dmg = 579;
  max_dmg = 721;
  coefficient = 1.5 / 3.5;
  does_damage = true;
  is_finisher = true;
  can_crit = true;
  spell_school = SpellSchool::kFire;
  spell_type = SpellType::kDestruction;
  can_miss = true;
  attack_type = AttackType::kMagical;
  Setup();
}

bool Conflagrate::CanCast() {
  return entity.player->auras.immolate != NULL && entity.player->auras.immolate->active && Spell::CanCast();
}

void Conflagrate::Cast() {
  Spell::Cast();
  entity.player->auras.immolate->Fade();
}

DestructionPotion::DestructionPotion(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kDestructionPotion;
  cooldown = 120;
  is_item = true;
  on_gcd = false;
  Setup();
}

FlameCap::FlameCap(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kFlameCap;
  cooldown = 180;
  is_item = true;
  on_gcd = false;
  shared_cooldown_spells.push_back(SpellName::kChippedPowerCore);
  shared_cooldown_spells.push_back(SpellName::kCrackedPowerCore);
  Setup();
}

BloodFury::BloodFury(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kBloodFury;
  cooldown = 120;
  on_gcd = false;
  is_item = true;  // TODO create some other property for spells like this instead of making them items
  Setup();
}

Bloodlust::Bloodlust(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kBloodlust;
  cooldown = 600;
  is_item = true;
  on_gcd = false;
  is_non_warlock_ability = true;
  Setup();
}

DrumsOfBattle::DrumsOfBattle(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kDrumsOfBattle;
  cooldown = 120;
  on_gcd = false;
  is_non_warlock_ability = true;
  is_item = true;
  Setup();
}

DrumsOfWar::DrumsOfWar(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kDrumsOfWar;
  cooldown = 120;
  on_gcd = false;
  is_non_warlock_ability = true;
  is_item = true;
  Setup();
}

DrumsOfRestoration::DrumsOfRestoration(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kDrumsOfRestoration;
  cooldown = 120;
  on_gcd = false;
  is_non_warlock_ability = true;
  is_item = true;
  Setup();
}

AmplifyCurse::AmplifyCurse(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kAmplifyCurse;
  cooldown = 180;
  on_gcd = false;
  Setup();
}

PowerInfusion::PowerInfusion(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kPowerInfusion;
  cooldown = 180;
  on_gcd = false;
  is_non_warlock_ability = true;
  Setup();
}

Innervate::Innervate(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kInnervate;
  cooldown = 360;
  on_gcd = false;
  is_non_warlock_ability = true;
  Setup();
}

ChippedPowerCore::ChippedPowerCore(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kChippedPowerCore;
  cooldown = 120;
  on_gcd = false;
  is_item = true;
  limited_amount_of_casts = true;
  amount_of_casts_per_fight = entity.player->settings.chipped_power_core_amount;
  shared_cooldown_spells.insert(shared_cooldown_spells.end(),
                                {SpellName::kDemonicRune, SpellName::kFlameCap, SpellName::kCrackedPowerCore});
  Setup();
};

CrackedPowerCore::CrackedPowerCore(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kCrackedPowerCore;
  cooldown = 120;
  on_gcd = false;
  is_item = true;
  limited_amount_of_casts = true;
  amount_of_casts_per_fight = entity.player->settings.cracked_power_core_amount;
  shared_cooldown_spells.insert(shared_cooldown_spells.end(),
                                {SpellName::kDemonicRune, SpellName::kFlameCap, SpellName::kChippedPowerCore});
  Setup();
};

ManaTideTotem::ManaTideTotem(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, aura) {
  name = SpellName::kManaTideTotem;
  cooldown = 300;
  is_non_warlock_ability = true;
  Setup();
}

ImpFirebolt::ImpFirebolt(Entity& entity) : Spell(entity) {
  name = SpellName::kFirebolt;
  cast_time = 2 - (0.25 * entity.player->talents.improved_firebolt);
  mana_cost = 145;
  base_damage = 119.5 * (1 + 0.1 * entity.player->talents.improved_imp);
  coefficient = 2 / 3.5;
  spell_school = SpellSchool::kFire;
  attack_type = AttackType::kMagical;
  can_crit = true;
  does_damage = true;
  can_miss = true;
  Setup();
}

PetMelee::PetMelee(Entity& entity) : Spell(entity) {
  name = SpellName::kMelee;
  attack_type = AttackType::kPhysical;
  cooldown = 2;
  on_gcd = false;
  can_crit = true;
  does_damage = true;
  can_miss = true;
  Setup();
}

double PetMelee::GetBaseDamage() { return (entity.pet->GetAttackPower() / 14 + 51.7) * entity.pet->kBaseMeleeSpeed; }

double PetMelee::GetCooldown() { return cooldown / entity.GetHastePercent(); }

FelguardCleave::FelguardCleave(Entity& entity) : Spell(entity) {
  name = SpellName::kCleave;
  cooldown = 6;
  mana_cost = 417;
  attack_type = AttackType::kPhysical;
  can_crit = true;
  does_damage = true;
  can_miss = true;
  Setup();
}

double FelguardCleave::GetBaseDamage() { return entity.pet->spells.melee->GetBaseDamage() + 78; }

SuccubusLashOfPain::SuccubusLashOfPain(Entity& entity) : Spell(entity) {
  name = SpellName::kLashOfPain;
  cooldown = 12 - 3 * entity.player->talents.improved_lash_of_pain;
  mana_cost = 190;
  base_damage = 123;
  spell_school = SpellSchool::kShadow;
  coefficient = 0.429;
  attack_type = AttackType::kMagical;
  can_crit = true;
  modifier *= 1 + entity.player->talents.improved_succubus / 10.0;
  can_crit = true;
  does_damage = true;
  can_miss = true;
  Setup();
}