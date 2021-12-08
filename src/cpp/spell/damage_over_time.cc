#include "damage_over_time.h"

#include "../common.h"
#include "../player/player.h"

DamageOverTime::DamageOverTime(Player& player)
    : player(player),
      duration(0),
      tick_timer_total(3),
      tick_timer_remaining(0),
      ticks_remaining(0),
      dmg(0),
      spell_power(0),
      modifier(1),
      active(false),
      coefficient(0),
      applied_with_amplify_curse(false) {}

void DamageOverTime::Setup() {
  original_duration = duration;

  // T4 4pc
  if ((name == SpellName::kCorruption || name == SpellName::kImmolate) && player.sets.t4 >= 4) {
    duration += 3;
  }

  ticks_total = duration / tick_timer_total;

  if (player.recording_combat_log_breakdown && player.combat_log_breakdown.count(name) == 0) {
    player.combat_log_breakdown.insert({name, std::make_unique<CombatLogBreakdown>(name)});
  }

  player.dot_list.push_back(this);
}

void DamageOverTime::Apply() {
  if (active && player.ShouldWriteToCombatLog()) {
    std::string msg = name + " refreshed before letting it expire";
  } else if (!active && player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->applied_at = player.simulation->fight_time;
  }
  const bool kIsAlreadyActive = active;
  spell_power = player.GetSpellPower(true, school);

  active = true;
  tick_timer_remaining = tick_timer_total;
  ticks_remaining = ticks_total;

  if (player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->count++;
  }
  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog(name + " " + (kIsAlreadyActive ? "refreshed" : "applied") + " (" + DoubleToString(spell_power) +
                     " Spell Power)");
  }
  // Siphon Life snapshots the presence of ISB. So if ISB isn't up when it's
  // Cast, it doesn't get the benefit even if it comes up later during the
  // duration.
  if (name == SpellName::kSiphonLife) {
    isb_is_active = !player.settings.using_custom_isb_uptime && player.auras.improved_shadow_bolt != NULL &&
                    player.auras.improved_shadow_bolt->active;
  }
  // Amplify Curse
  if ((name == SpellName::kCurseOfAgony || name == SpellName::kCurseOfDoom) && player.auras.amplify_curse != NULL &&
      player.auras.amplify_curse->active) {
    applied_with_amplify_curse = true;
    player.auras.amplify_curse->Fade();
  } else {
    applied_with_amplify_curse = false;
  }
}

void DamageOverTime::Fade() {
  active = false;
  tick_timer_remaining = 0;
  ticks_remaining = 0;

  if (player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->uptime +=
        player.simulation->fight_time - player.combat_log_breakdown.at(name)->applied_at;
  }

  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog(name + " faded");
  }
}

double DamageOverTime::GetModifier() {
  double damage_modifier = modifier * (school == SpellSchool::kShadow ? player.stats.shadow_modifier
                                       : school == SpellSchool::kFire ? player.stats.fire_modifier
                                                                      : 1);
  // Only add Improved Shadow Bolt if it's not Siphon Life or if ISB was active when the Siphon Life was cast.
  if ((school == SpellSchool::kShadow && player.auras.improved_shadow_bolt != NULL &&
       player.auras.improved_shadow_bolt->active && name != SpellName::kSiphonLife) ||
      (name == SpellName::kSiphonLife && isb_is_active)) {
    damage_modifier *= player.auras.improved_shadow_bolt->modifier;
  }
  return damage_modifier;
}

std::vector<double> DamageOverTime::GetConstantDamage() {
  double spell_power = this->spell_power;
  // If the DoT isn't currently active then this.spell_power will be 0, so use
  // the player's current Spell Power
  if (!active) {
    spell_power = player.GetSpellPower(true, school);
  }
  double modifier = GetModifier();
  double partial_resist_multiplier = player.GetPartialResistMultiplier(school);
  double base_damage = dmg;

  if (applied_with_amplify_curse) {
    base_damage *= 1.5;
  }
  // Add the t5 4pc bonus modifier to the base damage
  if ((name == SpellName::kCorruption || name == SpellName::kImmolate) && player.sets.t5 >= 4) {
    base_damage *= t5_bonus_modifier;
  }
  double total_damage = base_damage;
  total_damage += spell_power * coefficient;
  total_damage *= modifier * partial_resist_multiplier;

  return std::vector<double>{base_damage, total_damage, spell_power, modifier, partial_resist_multiplier};
}

double DamageOverTime::PredictDamage() {
  double damage = GetConstantDamage()[1];
  // If it's Corruption or Immolate then divide by the original duration (18s
  // and 15s) and multiply by the durationTotal property This is just for the t4
  // 4pc bonus since their durationTotal property is increased by 3 seconds to
  // include another tick but the damage they do stays the same which assumes
  // the normal duration without the bonus
  if (name == SpellName::kCorruption || name == SpellName::kImmolate) {
    damage /= original_duration;
    damage *= duration;
  }

  return damage;
}

void DamageOverTime::Tick(double t) {
  tick_timer_remaining -= t;

  if (tick_timer_remaining <= 0) {
    std::vector<double> constant_damage = GetConstantDamage();
    const double kBaseDamage = constant_damage[0];
    const double kDamage = constant_damage[1] / (original_duration / tick_timer_total);
    const double kSpellPower = constant_damage[2];
    const double kModifier = constant_damage[3];
    const double kPartialResistMultiplier = constant_damage[4];

    // Check for Nightfall proc
    if (name == SpellName::kCorruption && player.talents.nightfall > 0) {
      if (player.RollRng(player.talents.nightfall * 2)) {
        player.auras.shadow_trance->Apply();
      }
    }

    player.iteration_damage += kDamage;
    ticks_remaining--;
    tick_timer_remaining = tick_timer_total;

    if (player.recording_combat_log_breakdown) {
      player.combat_log_breakdown.at(name)->iteration_damage += kDamage;
    }

    if (player.ShouldWriteToCombatLog()) {
      std::string msg = name + " Tick " + DoubleToString(round(kDamage)) + " (" + DoubleToString(kBaseDamage) +
                        " Base Damage - " + DoubleToString(kSpellPower) + " Spell Power - " +
                        DoubleToString(coefficient, 3) + " Coefficient - " +
                        DoubleToString(round(kModifier * 10000) / 100, 3) + "% Damage Modifier - " +
                        DoubleToString(round(kPartialResistMultiplier * 1000) / 10) + "% Partial Resist Multiplier";
      if (t5_bonus_modifier > 1) {
        msg += " - " + DoubleToString(round(t5_bonus_modifier * 10000) / 100, 3) + "% Base Dmg Modifier (T5 4pc bonus)";
      }
      msg += ")";

      player.CombatLog(msg);
    }

    for (auto& proc : player.on_dot_tick_procs) {
      if (proc->Ready() && proc->ShouldProc(this) && player.RollRng(proc->proc_chance)) {
        proc->StartCast();
      }
    }

    if (ticks_remaining <= 0) {
      Fade();
    }
  }
}

CorruptionDot::CorruptionDot(Player& player) : DamageOverTime(player) {
  name = SpellName::kCorruption;
  duration = 18;
  tick_timer_total = 3;
  dmg = 900;
  school = SpellSchool::kShadow;
  coefficient = 0.936 + (0.12 * player.talents.empowered_corruption);
  t5_bonus_modifier = 1;
  Setup();

  // T3 4pc
  if (player.sets.plagueheart >= 4) {
    modifier *= 1.12;
  }
}

double CorruptionDot::GetModifier() {
  double modifier = DamageOverTime::GetModifier();
  if (player.talents.shadow_mastery > 0 && player.talents.contagion > 0) {
    modifier /= 1 + (player.talents.shadow_mastery * 0.02);
    modifier *= 1 * (1 + ((player.talents.shadow_mastery * 0.02) + (player.talents.contagion / 100.0)));
  }
  return modifier;
}

void CorruptionDot::Apply() {
  t5_bonus_modifier = 1;
  DamageOverTime::Apply();
}

UnstableAfflictionDot::UnstableAfflictionDot(Player& player) : DamageOverTime(player) {
  name = SpellName::kUnstableAffliction;
  duration = 18;
  tick_timer_total = 3;
  dmg = 1050;
  school = SpellSchool::kShadow;
  coefficient = 1.2;
  Setup();
}

SiphonLifeDot::SiphonLifeDot(Player& player) : DamageOverTime(player) {
  name = SpellName::kSiphonLife;
  duration = 30;
  tick_timer_total = 3;
  dmg = 630;
  school = SpellSchool::kShadow;
  coefficient = 1;
  Setup();
}

ImmolateDot::ImmolateDot(Player& player) : DamageOverTime(player) {
  name = SpellName::kImmolate;
  duration = 15;
  tick_timer_total = 3;
  dmg = 615;
  school = SpellSchool::kFire;
  coefficient = 0.65;
  t5_bonus_modifier = 1;
  Setup();
}

void ImmolateDot::Apply() {
  t5_bonus_modifier = 1;
  DamageOverTime::Apply();
}

CurseOfAgonyDot::CurseOfAgonyDot(Player& player) : DamageOverTime(player) {
  name = SpellName::kCurseOfAgony;
  duration = 24;
  tick_timer_total = 3;
  dmg = 1356;
  school = SpellSchool::kShadow;
  coefficient = 1.2;
  Setup();
}

double CurseOfAgonyDot::GetModifier() {
  double modifier = DamageOverTime::GetModifier();
  modifier /= 1 + (player.talents.shadow_mastery * 0.02);
  modifier *= 1 * (1 + ((player.talents.shadow_mastery * 0.02) + (player.talents.contagion / 100.0) +
                        (player.talents.improved_curse_of_agony * 0.05)));
  return modifier;
}

CurseOfDoomDot::CurseOfDoomDot(Player& player) : DamageOverTime(player) {
  name = SpellName::kCurseOfDoom;
  duration = 60;
  tick_timer_total = 60;
  dmg = 4200;
  school = SpellSchool::kShadow;
  coefficient = 2;
  Setup();
}

double CurseOfDoomDot::GetModifier() {
  double modifier = DamageOverTime::GetModifier();

  if (player.talents.shadow_mastery > 0) {
    modifier /= (1 + (0.02 * player.talents.shadow_mastery));
  }

  return modifier;
}