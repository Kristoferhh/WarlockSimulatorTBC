#include "../include/damage_over_time.h"

#include "../include/common.h"
#include "../include/player.h"

DamageOverTime::DamageOverTime(Player& player) : player(player) {}

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
    auto msg = name + " refreshed before letting it expire";
  } else if (!active && player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->applied_at = player.simulation->current_fight_time;
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
    auto msg = name + " ";

    if (kIsAlreadyActive) {
      msg += "refreshed";
    } else {
      msg += "applied";
    }

    msg += " (" + DoubleToString(spell_power) + " Spell Power)";

    player.CombatLog(msg);
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
        player.simulation->current_fight_time - player.combat_log_breakdown.at(name)->applied_at;
  }

  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog(name + " faded");
  }
}

std::vector<double> DamageOverTime::GetConstantDamage() {
  auto current_spell_power = active ? spell_power : player.GetSpellPower(true, school);
  auto modifier = player.GetDamageModifier(*parent_spell, true);
  auto partial_resist_multiplier = player.GetPartialResistMultiplier(school);
  auto dmg = base_damage;

  if (applied_with_amplify_curse) {
    dmg *= 1.5;
  }
  // Add the t5 4pc bonus modifier to the base damage
  if ((name == SpellName::kCorruption || name == SpellName::kImmolate) && player.sets.t5 >= 4) {
    dmg *= t5_bonus_modifier;
  }

  auto total_damage = dmg;
  total_damage += current_spell_power * coefficient;
  total_damage *= modifier * partial_resist_multiplier;

  return std::vector<double>{dmg, total_damage, current_spell_power, modifier, partial_resist_multiplier};
}

double DamageOverTime::PredictDamage() {
  auto damage = GetConstantDamage()[1];
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
      auto msg = name + " Tick " + DoubleToString(round(kDamage)) + " (" + DoubleToString(kBaseDamage) +
                 " Base Damage - " + DoubleToString(kSpellPower) + " Spell Power - " + DoubleToString(coefficient, 3) +
                 " Coefficient - " + DoubleToString(round(kModifier * 10000) / 100, 3) + "% Damage Modifier - " +
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
  base_damage = 900;
  school = SpellSchool::kShadow;
  coefficient = 0.936 + (0.12 * player.talents.empowered_corruption);
  t5_bonus_modifier = 1;
  Setup();
}

void CorruptionDot::Apply() {
  t5_bonus_modifier = 1;
  DamageOverTime::Apply();
}

UnstableAfflictionDot::UnstableAfflictionDot(Player& player) : DamageOverTime(player) {
  name = SpellName::kUnstableAffliction;
  duration = 18;
  tick_timer_total = 3;
  base_damage = 1050;
  school = SpellSchool::kShadow;
  coefficient = 1.2;
  Setup();
}

SiphonLifeDot::SiphonLifeDot(Player& player) : DamageOverTime(player) {
  name = SpellName::kSiphonLife;
  duration = 30;
  tick_timer_total = 3;
  base_damage = 630;
  school = SpellSchool::kShadow;
  coefficient = 1;
  Setup();
}

ImmolateDot::ImmolateDot(Player& player) : DamageOverTime(player) {
  name = SpellName::kImmolate;
  duration = 15;
  tick_timer_total = 3;
  base_damage = 615;
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
  base_damage = 1356;
  school = SpellSchool::kShadow;
  coefficient = 1.2;
  Setup();
}

CurseOfDoomDot::CurseOfDoomDot(Player& player) : DamageOverTime(player) {
  name = SpellName::kCurseOfDoom;
  duration = 60;
  tick_timer_total = 60;
  base_damage = 4200;
  school = SpellSchool::kShadow;
  coefficient = 2;
  Setup();
}