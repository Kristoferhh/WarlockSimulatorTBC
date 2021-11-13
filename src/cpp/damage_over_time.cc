#include "damage_over_time.h"

#include "common.h"
#include "player.h"

DamageOverTime::DamageOverTime(std::shared_ptr<Player> player) : player(player) {
  duration = 0;
  tick_timer_total = 3;
  tick_timer_remaining = 0;
  ticks_remaining = 0;
  dmg = 0;
  spell_power = 0;
  modifier = 1;
  active = false;
  coefficient = 0;
  amplified = false;
}

void DamageOverTime::Setup() {
  original_duration = duration;

  // T4 4pc
  if ((name == "Corruption" || name == "Immolate") && player->sets->t4 >= 4) {
    duration += 3;
  }

  ticks_total = duration / tick_timer_total;

  if (player->combat_log_breakdown.count(name) == 0) {
    player->combat_log_breakdown.insert(std::make_pair(name, new CombatLogBreakdown(name)));
  }
}

void DamageOverTime::Apply() {
  if (active && player->ShouldWriteToCombatLog()) {
    std::string msg = name + " refreshed before letting it expire";
  } else if (!active) {
    player->combatLogBreakdown.at(name)->applied_at = player->fightTime;
  }
  bool is_active = active;
  int spell_power = player->getSpellPower(school);
  this->spell_power = spell_power;

  player->combatLogBreakdown.at(name)->count++;
  active = true;
  tick_timer_remaining = tick_timer_total;
  ticks_remaining = ticks_total;

  if (player->ShouldWriteToCombatLog()) {
    std::string refreshed_or_applied = is_active ? "refreshed" : "applied";
    player->CombatLog(name + " " + refreshed_or_applied + " (" + std::to_string(spell_power) + " Spell Power)");
  }
  // Siphon Life snapshots the presence of ISB. So if ISB isn't up when it's
  // Cast, it doesn't get the benefit even if it comes up later during the
  // duration.
  if (player->spells->SiphonLife != NULL && name == player->spells->SiphonLife->name) {
    isb_is_active = !player->settings->using_custom_isb_uptime && player->auras->ImprovedShadowBolt != NULL &&
                    player->auras->ImprovedShadowBolt->active;
  }
  // Amplify Curse
  if (((player->spells->CurseOfAgony != NULL && name == player->spells->CurseOfAgony->name) ||
       (player->spells->CurseOfDoom != NULL && name == player->spells->CurseOfDoom->name)) &&
      player->auras->AmplifyCurse != NULL && player->auras->AmplifyCurse->active) {
    amplified = true;
    player->auras->AmplifyCurse->Fade();
  } else {
    amplified = false;
  }
}

void DamageOverTime::Fade() {
  active = false;
  tick_timer_remaining = 0;
  ticks_remaining = 0;

  double aura_uptime = player->fight_time - player->combat_log_breakdown.at(name)->applied_at;
  player->combat_log_breakdown.at(name)->uptime += aura_uptime;

  if (player->ShouldWriteToCombatLog()) {
    player->CombatLog(name + " faded");
  }
}

double DamageOverTime::GetModifier() {
  double damage_modifier = modifier;
  if (school == SpellSchool::SHADOW) {
    damage_modifier *= player->stats->shadow_modifier;
  } else if (school == SpellSchool::FIRE) {
    damage_modifier *= player->stats->fire_modifier;
  }
  // Improved Shadow Bolt
  if ((school == SpellSchool::SHADOW && player->auras->improved_shadow_bolt != NULL &&
       player->auras->improved_shadow_bolt->active &&
       (player->spells->siphon_life == NULL || name != player->spells->siphon_life->name)) ||
      (player->spells->siphon_life != NULL && name == player->spells->siphon_life->name && isb_is_active)) {
    damage_modifier *= player->auras->improved_shadow_bolt->modifier;
  }
  return damage_modifier;
}

std::vector<double> DamageOverTime::GetConstantDamage() {
  double spell_power = this->spell_power;
  // If the DoT isn't currently active then this.spell_power will be 0, so use
  // the player's current Spell Power
  if (!active) {
    spell_power = player->GetSpellPower(school);
  }
  double modifier = GetModifier();
  double partial_resist_multiplier = player->GetPartialResistMultiplier(school);
  double base_damage = dmg;
  // Amplify Curse
  if (amplified) {
    base_damage *= 1.5;
  }
  // Add the t5 4pc bonus modifier to the base damage
  if (((player->spells->corruption != NULL && name == player->spells->corruption->name) ||
       (player->spells->immolate != NULL && name == player->spells->immolate->name)) &&
      player->sets->t5 >= 4) {
    base_damage *= t5_bonus_modifier;
  }
  double total_damage = base_damage;
  total_damage += spell_power * coefficient;
  total_damage *= modifier * partial_resist_multiplier;

  return std::vector<double>{base_damage, total_damage, spell_power, modifier, partial_resist_multiplier};
}

// Predicts how much damage the dot will do over its full duration
double DamageOverTime::PredictDamage() {
  std::vector<double> constant_damage = GetConstantDamage();
  double dmg = constant_damage[1];
  // If it's Corruption or Immolate then divide by the original duration (18s
  // and 15s) and multiply by the durationTotal property This is just for the t4
  // 4pc bonus since their durationTotal property is increased by 3 seconds to
  // include another tick but the damage they do stays the same which assumes
  // the normal duration without the bonus
  if ((player->spells->corruption != NULL && name == player->spells->corruption->name) ||
      (player->spells->immolate != NULL && name == player->spells->immolate->name)) {
    dmg /= original_duration;
    dmg *= duration;
  }

  return dmg;
}

void DamageOverTime::Tick(double t) {
  tick_timer_remaining -= t;

  if (tick_timer_remaining <= 0) {
    std::vector<double> constant_damage = GetConstantDamage();
    double base_damage = constant_damage[0];
    double dmg = constant_damage[1] / (original_duration / tick_timer_total);
    double spell_power = constant_damage[2];
    double modifier = constant_damage[3];
    double partial_resist_multiplier = constant_damage[4];

    // Check for Nightfall proc
    if (player->spells->corruption != NULL && name == player->spells->corruption->name &&
        player->talents->nightfall > 0) {
      if (player->GetRand() <= player->talents->nightfall * 2 * player->kFloatNumberMultiplier) {
        player->auras->shadow_trance->Apply();
      }
    }

    player->AddIterationDamageAndMana(name, 0, dmg);
    player->iteration_damage += dmg;
    ticks_remaining--;
    tick_timer_remaining = tick_timer_total;

    if (player->ShouldWriteToCombatLog()) {
      std::string msg = name + " Tick " + TruncateTrailingZeros(std::to_string(round(dmg))) + " (" +
                        TruncateTrailingZeros(std::to_string(base_damage)) + " Base Damage - " +
                        TruncateTrailingZeros(std::to_string(spell_power)) + " Spell Power - " +
                        TruncateTrailingZeros(std::to_string(coefficient), 3) + " Coefficient - " +
                        TruncateTrailingZeros(std::to_string(round(modifier * 10000) / 100), 3) +
                        "% Damage Modifier - " +
                        TruncateTrailingZeros(std::to_string(round(partial_resist_multiplier * 1000) / 10)) +
                        "% Partial Resist Multiplier";
      if (t5_bonus_modifier > 1) {
        msg += " - " + std::to_string(round(t5_bonus_modifier * 10000) / 100) + "% Base Dmg Modifier (T5 4pc bonus)";
      }
      msg += ")";
      player->CombatLog(msg);
    }

    // Ashtongue Talisman of Shadows
    if (player->spells->corruption != NULL && name == player->spells->corruption->name &&
        player->auras->ashtongue_talisman_of_shadows != NULL &&
        player->GetRand() <=
            player->auras->ashtongue_talisman_of_shadows->proc_chance * player->kFloatNumberMultiplier) {
      player->auras->ashtongue_talisman_of_shadows->Apply();
    }
    // Timbal's Focusing Crystal
    if (player->spells->timbals_focusing_crystal != NULL &&
        player->spells->timbals_focusing_crystal->cooldown_remaining <= 0 &&
        player->GetRand() <= player->spells->timbals_focusing_crystal->proc_chance * player->kFloatNumberMultiplier) {
      player->spells->timbals_focusing_crystal->StartCast();
    }

    if (ticks_remaining <= 0) {
      Fade();
    }
  }
}

CorruptionDot::CorruptionDot(std::shared_ptr<Player> player) : DamageOverTime(player) {
  name = "Corruption";
  duration = 18;
  tick_timer_total = 3;
  dmg = 900;
  school = SpellSchool::SHADOW;
  coefficient = 0.936 + (0.12 * player->talents->empowered_corruption);
  minimum_duration = 9;
  t5_bonus_modifier = 1;
  Setup();

  // T3 4pc
  if (player->sets->plagueheart >= 4) {
    modifier *= 1.12;
  }
}

double CorruptionDot::GetModifier() {
  double modifier = DamageOverTime::GetModifier();
  if (player->talents->shadow_mastery > 0 && player->talents->contagion > 0) {
    // Divide away the bonus from Shadow Mastery
    modifier /= (1 + (player->talents->shadow_mastery * 0.02));
    // Multiply the modifier with the bonus from Shadow Mastery + Contagion
    modifier *= (1 * (1 + ((player->talents->shadow_mastery * 0.02) + (player->talents->contagion / 100.0))));
  }
  return modifier;
}

void CorruptionDot::Apply() {
  // T5 4pc bonus modifier
  t5_bonus_modifier = 1;
  DamageOverTime::Apply();
}

UnstableAfflictionDot::UnstableAfflictionDot(std::shared_ptr<Player> player) : DamageOverTime(player) {
  name = "Unstable Affliction";
  duration = 18;
  tick_timer_total = 3;
  dmg = 1050;
  school = SpellSchool::SHADOW;
  coefficient = 1.2;
  minimum_duration = 9;
  Setup();
}

SiphonLifeDot::SiphonLifeDot(std::shared_ptr<Player> player) : DamageOverTime(player) {
  name = "Siphon Life";
  duration = 30;
  tick_timer_total = 3;
  dmg = 630;
  school = SpellSchool::SHADOW;
  coefficient = 1;
  minimum_duration = 30;
  Setup();
}

ImmolateDot::ImmolateDot(std::shared_ptr<Player> player) : DamageOverTime(player) {
  name = "Immolate";
  duration = 15;
  tick_timer_total = 3;
  dmg = 615;
  school = SpellSchool::FIRE;
  coefficient = 0.65;
  minimum_duration = 12;
  t5_bonus_modifier = 1;
  Setup();
}

void ImmolateDot::Apply() {
  // T5 4pc bonus modifier
  t5_bonus_modifier = 1;
  DamageOverTime::Apply();
}

CurseOfAgonyDot::CurseOfAgonyDot(std::shared_ptr<Player> player) : DamageOverTime(player) {
  name = "Curse of Agony";
  duration = 24;
  tick_timer_total = 3;
  dmg = 1356;
  school = SpellSchool::SHADOW;
  coefficient = 1.2;
  minimum_duration = 15;
  Setup();
}

double CurseOfAgonyDot::GetModifier() {
  double modifier = DamageOverTime::GetModifier();
  // Remove bonus from Shadow Mastery and add bonus from Shadow Mastery +
  // Contagion + Improved Curse of Agony
  modifier /= (1 + (player->talents->shadow_mastery * 0.02));
  modifier *= (1 * (1 + ((player->talents->shadow_mastery * 0.02) + (player->talents->contagion / 100.0) +
                         (player->talents->improved_curse_of_agony * 0.05))));
  return modifier;
}

CurseOfDoomDot::CurseOfDoomDot(std::shared_ptr<Player> player) : DamageOverTime(player) {
  name = "Curse of Doom";
  duration = 60;
  tick_timer_total = 60;
  dmg = 4200;
  school = SpellSchool::SHADOW;
  coefficient = 2;
  minimum_duration = 60;
  Setup();
}

double CurseOfDoomDot::GetModifier() {
  double modifier = DamageOverTime::GetModifier();

  // CoD doesn't benefit from SM
  if (player->talents->shadow_mastery > 0) {
    modifier /= (1 + (0.02 * player->talents->shadow_mastery));
  }

  return modifier;
}