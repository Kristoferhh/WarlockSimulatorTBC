#include "spell.h"

#include <iomanip>

#include "../bindings.h"
#include "../common.h"
#include "../player/player.h"

Spell::Spell(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : player(player),
      aura_effect(aura),
      dot_effect(dot),
      modifier(1),
      coefficient(0),
      cooldown(0),
      is_non_warlock_ability(false),
      does_damage(false),
      can_crit(false),
      is_item(false),
      on_gcd(true),
      is_proc(false),
      is_finisher(false),
      cast_time(0),
      usable_once_per_fight(false),
      has_not_been_cast_this_fight(true),
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

void Spell::Reset() {
  cooldown_remaining = 0;
  casting = false;
  has_not_been_cast_this_fight = true;
}

void Spell::Setup() {
  if (min_dmg > 0 && max_dmg > 0) {
    dmg = (min_dmg + max_dmg) / 2.0;
  }
  if (min_mana_gain > 0 && max_mana_gain > 0) {
    mana_gain = (min_mana_gain + max_mana_gain) / 2.0;
  }
  if (player.recording_combat_log_breakdown && player.combat_log_breakdown.count(name) == 0) {
    player.combat_log_breakdown.insert({name, std::make_unique<CombatLogBreakdown>(name)});
  }

  // Cataclysm talent
  if (type == SpellType::kDestruction) {
    mana_cost *= 1 - 0.01 * player.talents.cataclysm;
  }

  player.spell_list.push_back(this);
}

double Spell::GetCastTime() {
  return round((cast_time / player.GetHastePercent()) * 10000) / 10000 + player.kSpellDelay;
}

bool Spell::CanCast() {
  return cooldown_remaining <= 0 &&
         (is_non_warlock_ability ||
          ((!on_gcd || player.gcd_remaining <= 0) && (is_proc || player.cast_time_remaining <= 0))) &&
         (!usable_once_per_fight || has_not_been_cast_this_fight);
}

bool Spell::HasEnoughMana() { return GetManaCost() <= player.stats.mana; }

bool Spell::Ready() { return CanCast() && HasEnoughMana(); }

void Spell::StartCast(double predicted_damage) {
  if (on_gcd && !is_non_warlock_ability) {
    // Error: Casting a spell while GCD is active
    if (player.gcd_remaining > 0) {
      player.ThrowError("Attempting to Cast " + name + " while player's GCD is at " +
                        std::to_string(player.gcd_remaining) + " seconds remaining");
    }
    player.gcd_remaining = player.GetGcdValue(name);
  }

  // Error: Starting to Cast a spell while casting another spell
  if (player.cast_time_remaining > 0 && !is_non_warlock_ability && !is_proc) {
    player.ThrowError("Attempting to Cast " + name + " while player's Cast time remaining is at " +
                      std::to_string(player.cast_time_remaining) + " sec");
  }

  // Error: Casting a spell while it's on cooldown
  if (cooldown > 0 && cooldown_remaining > 0) {
    player.ThrowError("Attempting to Cast " + name + " while it's still on cooldown (" +
                      std::to_string(cooldown_remaining) + " seconds remaining)");
  }

  std::string combat_log_message = "";
  if (cast_time > 0) {
    casting = true;
    player.cast_time_remaining = GetCastTime();
    if (!is_proc && player.ShouldWriteToCombatLog()) {
      combat_log_message.append("Started casting " + name +
                                " - Cast time: " + DoubleToString(player.cast_time_remaining - player.kSpellDelay, 4) +
                                " (" + DoubleToString((player.GetHastePercent() - 1) * 100, 4) +
                                "% haste at a base Cast speed of " + DoubleToString(cast_time, 2) + ")");
    }
  } else {
    if (!is_proc && player.ShouldWriteToCombatLog()) {
      combat_log_message.append("Cast " + name);
    }
    Cast();
  }
  if (on_gcd && !is_non_warlock_ability && player.ShouldWriteToCombatLog()) {
    combat_log_message.append(" - Global cooldown: " + DoubleToString(player.gcd_remaining, 4));
  }
  if (predicted_damage > 0 && player.ShouldWriteToCombatLog()) {
    combat_log_message.append(" - Estimated Damage / Cast time: " + DoubleToString(round(predicted_damage)));
  }

  if (combat_log_message.length() > 0 && player.ShouldWriteToCombatLog()) {
    player.CombatLog(combat_log_message);
  }
}

int Spell::GetManaCost() { return mana_cost * player.stats.mana_cost_modifier; }

void Spell::Tick(double t) {
  if (cooldown_remaining > 0 && cooldown_remaining - t <= 0) {
    if (name == SpellName::kPowerInfusion) {
      player.power_infusions_ready++;
    }
    if (player.ShouldWriteToCombatLog()) {
      player.CombatLog(name + " is off cooldown");
    }
  }

  cooldown_remaining -= t;
  if (casting && player.cast_time_remaining <= 0) {
    Cast();
  }
}

void Spell::Cast() {
  const int kCurrentMana = player.stats.mana;
  bool is_crit = false;
  cooldown_remaining = cooldown;
  casting = false;
  has_not_been_cast_this_fight = false;

  for (auto& spell_name : shared_cooldown_spells) {
    for (auto& player_spell : player.spell_list) {
      if (player_spell->name == spell_name) {
        player_spell->cooldown_remaining = cooldown;
      }
    }
  }

  if (name == SpellName::kPowerInfusion) {
    player.power_infusions_ready--;
  }

  if (aura_effect == NULL && player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->casts++;
  }

  if (mana_cost > 0 && !player.settings.infinite_player_mana) {
    player.stats.mana -= GetManaCost();
    player.five_second_rule_timer = 5;
  }

  if (cast_time > 0 && player.ShouldWriteToCombatLog()) {
    player.CombatLog("Finished casting " + name + " - Mana: " + DoubleToString(kCurrentMana) + " -> " +
                     DoubleToString(player.stats.mana) + " - Mana Cost: " + DoubleToString(round(GetManaCost())) +
                     " - Mana Cost Modifier: " + DoubleToString(round(player.stats.mana_cost_modifier * 100)) + "%");
  }

  if (gain_mana_on_cast) {
    player.stats.mana = std::min(static_cast<double>(player.stats.max_mana), kCurrentMana + mana_gain);
    const int kManaGained = player.stats.mana - kCurrentMana;
    if (player.recording_combat_log_breakdown) {
      player.AddIterationDamageAndMana(name, kManaGained, 0);
    }
    if (player.ShouldWriteToCombatLog()) {
      player.CombatLog("Player gains " + DoubleToString(round(kManaGained)) + " mana from " + name + " (" +
                       DoubleToString(round(kCurrentMana)) + " -> " + DoubleToString(round(player.stats.mana)) + ")");
    }
  }

  if (can_crit) {
    is_crit = player.IsCrit(type, bonus_crit_chance);
    if (is_crit && player.recording_combat_log_breakdown) {
      // Increment the crit counter whether the spell hits or not so that the
      // crit % on the Damage breakdown is correct. Otherwise the crit % will be
      // lower due to lost crits when the spell misses.
      player.combat_log_breakdown.at(name)->crits++;
    }
  }

  if (can_miss && !player.IsHit(type)) {
    if (player.ShouldWriteToCombatLog()) {
      player.CombatLog(name + " *resist*");
    }
    if (player.recording_combat_log_breakdown) {
      player.combat_log_breakdown.at(name)->misses++;
    }
    OnResistProcs();

    return;
  }

  if (aura_effect != NULL) {
    aura_effect->Apply();
  }
  if (dot_effect != NULL) {
    dot_effect->Apply();
  }
  if (does_damage) {
    Damage(is_crit);
  }

  if (!is_item && !is_proc && !is_non_warlock_ability && name != SpellName::kAmplifyCurse) {
    OnHitProcs();
  }
}

double Spell::GetModifier() {
  double damage_modifier = modifier;
  if (school == SpellSchool::kShadow) {
    damage_modifier *= player.stats.shadow_modifier;

    // Improved Shadow Bolt
    if (!player.settings.using_custom_isb_uptime && player.auras.improved_shadow_bolt != NULL &&
        player.auras.improved_shadow_bolt->active) {
      damage_modifier *= player.auras.improved_shadow_bolt->modifier;
    }
  } else if (school == SpellSchool::kFire) {
    damage_modifier *= player.stats.fire_modifier;
  }

  return damage_modifier;
}

void Spell::Damage(bool isCrit) {
  const std::vector<double> kConstantDamage = GetConstantDamage();
  const double kBaseDamage = kConstantDamage[0];
  double total_damage = kConstantDamage[1];
  const double kDamageModifier = kConstantDamage[2];
  const double kPartialResistMultiplier = kConstantDamage[3];
  const double kSpellPower = kConstantDamage[4];
  double crit_multiplier = player.kCritDamageMultiplier;

  if (isCrit) {
    crit_multiplier = GetCritMultiplier(crit_multiplier);
    total_damage *= crit_multiplier;
    OnCritProcs();
  } else if (school == SpellSchool::kShadow && dot_effect == NULL && player.auras.improved_shadow_bolt != NULL &&
             player.auras.improved_shadow_bolt->active && !player.settings.using_custom_isb_uptime) {
    player.auras.improved_shadow_bolt->DecrementStacks();
  }

  OnDamageProcs();
  player.iteration_damage += total_damage;

  if (player.recording_combat_log_breakdown) {
    player.AddIterationDamageAndMana(name, 0, total_damage);
  }
  if (player.ShouldWriteToCombatLog()) {
    std::string msg = name + " ";
    if (isCrit) {
      msg += "*";
    }
    msg += DoubleToString(round(total_damage));
    if (isCrit) {
      msg += "*";
    }
    msg += " (" + DoubleToString(dmg, 1) + " Base Damage - " + DoubleToString(round(coefficient * 1000) / 1000, 3) +
           " Coefficient - " + DoubleToString(round(kSpellPower)) + " Spell Power - ";
    if (isCrit) {
      msg += DoubleToString(crit_multiplier * 100, 3) + "% Crit Multiplier - ";
    }
    msg += DoubleToString(round(kDamageModifier * 10000) / 100, 2) + "% Damage Modifier - " +
           DoubleToString(round(kPartialResistMultiplier * 1000) / 10) + "% Partial Resist Multiplier)";
    player.CombatLog(msg);
  }

  // T5 4pc
  if (player.sets.t5 >= 4) {
    if (name == SpellName::kShadowBolt && player.auras.corruption != NULL && player.auras.corruption->active) {
      player.auras.corruption->t5_bonus_modifier *= 1.1;
    } else if (name == SpellName::kIncinerate && player.auras.immolate != NULL && player.auras.immolate->active) {
      player.auras.immolate->t5_bonus_modifier *= 1.1;
    }
  }
}

// Returns the non-RNG Damage of the spell (basically just the base Damage +
// spell power + Damage modifiers, no crit/miss etc.)
// TODO investigate this no_rng variable
std::vector<double> Spell::GetConstantDamage(bool no_rng) {
  double total_damage = player.settings.randomize_values && min_dmg > 0 && max_dmg > 0 && !no_rng
                            ? player.rng.range(min_dmg, max_dmg)
                            : dmg;
  const double kBaseDamage = total_damage;
  const double kSpellPower = player.GetSpellPower(school);
  const double kDamageModifier = GetModifier();
  const double kPartialResistMultiplier = player.GetPartialResistMultiplier(school);

  // If casting Incinerate and Immolate is up, add the bonus Damage
  if (name == SpellName::kIncinerate && player.auras.immolate != NULL && player.auras.immolate->active) {
    total_damage += player.settings.randomize_values && no_rng
                        ? player.rng.range(bonus_damage_from_immolate_min, bonus_damage_from_immolate_max)
                        : bonus_damage_from_immolate_average;
  }
  // Add Damage from Spell Power
  total_damage += kSpellPower * coefficient;
  // Modifier & Partial Resist
  total_damage *= kDamageModifier;
  total_damage *= kPartialResistMultiplier;

  return std::vector<double>{kBaseDamage, total_damage, kDamageModifier, kPartialResistMultiplier, kSpellPower};
}

double Spell::GetCritMultiplier(double player_crit_multiplier) {
  double crit_multiplier = player_crit_multiplier;

  // Chaotic Skyfire Diamond
  if (player.settings.meta_gem_id == 34220) {
    crit_multiplier *= 1.03;
  }
  // Ruin
  if (type == SpellType::kDestruction && player.talents.ruin == 1) {
    // Ruin doubles the *bonus* of your crits, not the Damage of the crit itself
    // So if your crit Damage % is e.g. 154.5% it would become 209% because
    // the 54.5% is being doubled
    crit_multiplier -= 1;
    crit_multiplier *= 2;
    crit_multiplier += 1;
  }
  return crit_multiplier;
}

double Spell::PredictDamage() {
  const std::vector<double> kConstantDamage = GetConstantDamage();
  const double kNormalDamage = kConstantDamage[1];
  double crit_damage = 0;
  double crit_chance = 0;
  double chance_to_not_crit = 0;

  if (can_crit) {
    crit_damage = kNormalDamage * GetCritMultiplier(player.kCritDamageMultiplier);
    crit_chance = player.GetCritChance(type) / 100;
    chance_to_not_crit = 1 - crit_chance;
  }

  double hit_chance = player.GetHitChance(type) / 100;
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
  if (school == SpellSchool::kShadow && !player.settings.using_custom_isb_uptime &&
      player.auras.improved_shadow_bolt != NULL && !player.auras.improved_shadow_bolt->active) {
    estimated_damage *= 1.15;  // using 75% uptime as the default for now
  }

  return (estimated_damage * hit_chance) / std::max(player.GetGcdValue(name), GetCastTime());
}

void Spell::OnCritProcs() {
  for (auto& proc : player.on_crit_procs) {
    if (proc->Ready() && proc->ShouldProc(this) && player.RollRng(proc->proc_chance)) {
      proc->StartCast();
    }
  }
}

void Spell::OnResistProcs() {
  for (auto& proc : player.on_resist_procs) {
    if (proc->Ready() && proc->ShouldProc(this) && player.RollRng(proc->proc_chance)) {
      proc->StartCast();
    }
  }
}

void Spell::OnDamageProcs() {
  for (auto& proc : player.on_damage_procs) {
    if (proc->Ready() && proc->ShouldProc(this) && player.RollRng(proc->proc_chance)) {
      proc->StartCast();
    }
  }
}

void Spell::OnHitProcs() {
  for (auto& proc : player.on_hit_procs) {
    if (proc->Ready() && proc->ShouldProc(this) && player.RollRng(proc->proc_chance)) {
      proc->StartCast();
    }
  }
}

ShadowBolt::ShadowBolt(Player& player) : Spell(player) {
  name = SpellName::kShadowBolt;
  cast_time = CalculateCastTime();
  mana_cost = 420;
  coefficient = (3 / 3.5) + (0.04 * player.talents.shadow_and_flame);
  min_dmg = 544;
  max_dmg = 607;
  does_damage = true;
  can_crit = true;
  school = SpellSchool::kShadow;
  type = SpellType::kDestruction;
  can_miss = true;
  Setup();

  // T6 4pc bonus
  if (player.sets.t6 >= 4) {
    modifier *= 1.06;
  }
}

void ShadowBolt::StartCast(double predicted_damage = 0) {
  const bool kHasShadowTrance = player.auras.shadow_trance != NULL;

  if (kHasShadowTrance && player.auras.shadow_trance->active) {
    cast_time = 0;
  }

  Spell::StartCast();

  if (kHasShadowTrance && player.auras.shadow_trance->active) {
    cast_time = CalculateCastTime();
    player.auras.shadow_trance->Fade();
  }
}

double ShadowBolt::CalculateCastTime() { return 3 - (0.1 * player.talents.bane); }

Incinerate::Incinerate(Player& player) : Spell(player) {
  name = SpellName::kIncinerate;
  cast_time = round((2.5 * (1 - 0.02 * player.talents.emberstorm)) * 100) / 100;
  mana_cost = 355;
  coefficient = (2.5 / 3.5) + (0.04 * player.talents.shadow_and_flame);
  min_dmg = 444;
  max_dmg = 514;
  bonus_damage_from_immolate_min = 111;
  bonus_damage_from_immolate_max = 128;
  bonus_damage_from_immolate_average = (bonus_damage_from_immolate_min + bonus_damage_from_immolate_max) / 2;
  does_damage = true;
  can_crit = true;
  school = SpellSchool::kFire;
  type = SpellType::kDestruction;
  can_miss = true;
  Setup();

  if (player.sets.t6 >= 4) {
    modifier *= 1.06;
  }
}

SearingPain::SearingPain(Player& player) : Spell(player) {
  name = SpellName::kSearingPain;
  cast_time = 1.5;
  mana_cost = 205;
  coefficient = 1.5 / 3.5;
  min_dmg = 270;
  max_dmg = 320;
  does_damage = true;
  can_crit = true;
  school = SpellSchool::kFire;
  type = SpellType::kDestruction;
  bonus_crit_chance = 4 * player.talents.improved_searing_pain;
  can_miss = true;
  Setup();
};

SoulFire::SoulFire(Player& player) : Spell(player) {
  name = SpellName::kSoulFire;
  cast_time = 6 - (0.4 * player.talents.bane);
  mana_cost = 250;
  coefficient = 1.15;
  min_dmg = 1003;
  max_dmg = 1257;
  does_damage = true;
  can_crit = true;
  school = SpellSchool::kFire;
  type = SpellType::kDestruction;
  can_miss = true;
  Setup();
};

Shadowburn::Shadowburn(Player& player) : Spell(player) {
  name = SpellName::kShadowburn;
  cooldown = 15;
  mana_cost = 515;
  coefficient = 0.22;
  min_dmg = 597;
  max_dmg = 665;
  does_damage = true;
  can_crit = true;
  is_finisher = true;
  school = SpellSchool::kShadow;
  type = SpellType::kDestruction;
  can_miss = true;
  Setup();
};

DeathCoil::DeathCoil(Player& player) : Spell(player) {
  name = SpellName::kDeathCoil;
  cooldown = 120;
  mana_cost = 600;
  coefficient = 0.4286;
  dmg = 526;
  does_damage = true;
  is_finisher = true;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  can_miss = true;
  Setup();
};

Shadowfury::Shadowfury(Player& player) : Spell(player) {
  name = SpellName::kShadowfury;
  cast_time = 0.5;
  mana_cost = 710;
  min_dmg = 612;
  max_dmg = 728;
  does_damage = true;
  can_crit = true;
  school = SpellSchool::kShadow;
  type = SpellType::kDestruction;
  cooldown = 20;
  coefficient = 0.195;
  can_miss = true;
  Setup();
}

SeedOfCorruption::SeedOfCorruption(Player& player) : Spell(player) {
  name = SpellName::kSeedOfCorruption;
  min_dmg = 1110;
  max_dmg = 1290;
  mana_cost = 882;
  cast_time = 2;
  aoe_cap = 13580;
  does_damage = true;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  coefficient = 0.214;
  can_miss = true;
  Setup();
};

void SeedOfCorruption::Damage(bool isCrit) {
  const int kBaseDamage =
      player.settings.randomize_values && min_dmg && max_dmg ? player.rng.range(min_dmg, max_dmg) : dmg;
  const int kEnemyAmount = player.settings.enemy_amount - 1;  // Minus one because the enemy that Seed is being Cast
                                                              // on doesn't get hit
  int resist_amount = 0;
  int crit_amount = 0;
  const int kSpellPower = player.GetSpellPower(school);
  double crit_damage_multiplier = 0;
  double internal_modifier = GetModifier();
  double external_modifier = 1;

  // If using a custom ISB uptime % then remove the damage bonus since ISB won't be up on the mobs
  if (player.settings.using_custom_isb_uptime) {
    internal_modifier /= 1 + player.GetCustomImprovedShadowBoltDamageModifier();
  }

  // Remove debuffs from the modifier since they ignore the AOE cap, so we'll
  // add the debuff % modifiers after the Damage has been calculated.
  if (player.selected_auras.curse_of_the_elements) {
    const double kModifier = 1.1 + (0.01 * player.settings.improved_curse_of_the_elements);
    internal_modifier /= kModifier;
    external_modifier *= kModifier;
  }
  if (player.selected_auras.shadow_weaving) {
    const double kModifier = 1.1;
    internal_modifier /= kModifier;
    external_modifier *= kModifier;
  }
  if (player.selected_auras.misery) {
    const double kModifier = 1.05;
    internal_modifier /= kModifier;
    external_modifier *= kModifier;
  }

  for (int i = 0; i < kEnemyAmount; i++) {
    // Check for a resist
    if (!player.IsHit(type)) {
      resist_amount++;
      OnResistProcs();
    } else {
      OnDamageProcs();
      // Check for a crit
      if (player.IsCrit(type)) {
        crit_amount++;
        OnCritProcs();
      }
    }
  }

  double individual_seed_damage = kBaseDamage + (kSpellPower * coefficient);
  // Oblivion Raiment (dungeon set) 4pc bonus
  if (player.sets.oblivion >= 4) {
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
    crit_damage_multiplier = GetCritMultiplier(player.kCritDamageMultiplier);
    const double kIndividualSeedCrit = individual_seed_damage * crit_damage_multiplier;
    const double kBonusDamageFromCrit = kIndividualSeedCrit - individual_seed_damage;
    total_seed_damage += kBonusDamageFromCrit * crit_amount;
  }
  // Partial resists (probably need to calculate a partial resist for each seed
  // hit, not sure how it interacts for the aoe cap)
  const double kPartialResistMultiplier = player.GetPartialResistMultiplier(school);
  total_seed_damage *= kPartialResistMultiplier;

  // Add Damage from debuffs
  total_seed_damage *= external_modifier;

  player.iteration_damage += total_seed_damage;

  if (player.ShouldWriteToCombatLog()) {
    std::string msg = name + " " + DoubleToString(round(total_seed_damage)) + " (" + std::to_string(kEnemyAmount) +
                      " Enemies (" + std::to_string(resist_amount) + " Resists & " + std::to_string(crit_amount) +
                      " Crits) - " + std::to_string(kBaseDamage) + " Base Damage - " + DoubleToString(coefficient, 3) +
                      " Coefficient - " + std::to_string(kSpellPower) + " Spell Power - " +
                      DoubleToString(round(internal_modifier * external_modifier * 1000) / 10, 1) + "% Modifier - ";
    if (crit_amount > 0) {
      msg += DoubleToString(crit_damage_multiplier, 3) + "% Crit Multiplier";
    }
    msg += " - " + DoubleToString(round(kPartialResistMultiplier * 1000) / 10) + "% Partial Resist Multiplier)";
    player.CombatLog(msg);
  }
  if (player.recording_combat_log_breakdown) {
    player.AddIterationDamageAndMana(name, 0, total_seed_damage);
    player.combat_log_breakdown.at(name)->crits += crit_amount;
    player.combat_log_breakdown.at(name)->misses += resist_amount;
    // the Cast() function already adds 1 to the amount of casts so we only need
    // to add enemiesHit - 1 to the Cast amount
    player.combat_log_breakdown.at(name)->casts += (kEnemiesHit - 1);
  }
}

double SeedOfCorruption::GetModifier() {
  double modifier = Spell::GetModifier();
  if (player.talents.shadow_mastery > 0 && player.talents.contagion > 0) {
    // Divide away the bonus from Shadow Mastery
    modifier /= (1 + (player.talents.shadow_mastery * 0.02));
    // Multiply the modifier with the bonus from Shadow Mastery + Contagion
    modifier *= (1 * (1 + ((player.talents.shadow_mastery * 0.02) + (player.talents.contagion / 100.0))));
  }
  return modifier;
}

Corruption::Corruption(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(player, aura, dot) {
  name = SpellName::kCorruption;
  mana_cost = 370;
  cast_time = round((2 - (0.4 * player.talents.improved_corruption)) * 100) / 100.0;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  can_miss = true;
  Setup();
}

UnstableAffliction::UnstableAffliction(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(player, aura, dot) {
  name = SpellName::kUnstableAffliction;
  mana_cost = 400;
  cast_time = 1.5;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  can_miss = true;
  Setup();
}

SiphonLife::SiphonLife(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(player, aura, dot) {
  name = SpellName::kSiphonLife;
  mana_cost = 410;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  can_miss = true;
  Setup();
}

Immolate::Immolate(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(player, aura, dot) {
  name = SpellName::kImmolate;
  mana_cost = 445;
  cast_time = 2 - (0.1 * player.talents.bane);
  does_damage = true;
  can_crit = true;
  dmg = 331;
  coefficient = 0.2;
  school = SpellSchool::kFire;
  type = SpellType::kDestruction;
  can_miss = true;
  Setup();
}

double Immolate::GetModifier() {
  double modifier = Spell::GetModifier();
  if (player.talents.emberstorm > 0) {
    modifier /= (1 + 0.02 * player.talents.emberstorm);
  }
  modifier *= (1 + (0.02 * player.talents.emberstorm + 0.05 * player.talents.improved_immolate));
  return modifier;
}

CurseOfAgony::CurseOfAgony(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(player, aura, dot) {
  name = SpellName::kCurseOfAgony;
  mana_cost = 265;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  can_miss = true;
  Setup();
}

CurseOfTheElements::CurseOfTheElements(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kCurseOfTheElements;
  mana_cost = 260;
  type = SpellType::kAffliction;
  can_miss = true;
  Setup();
}

CurseOfRecklessness::CurseOfRecklessness(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kCurseOfRecklessness;
  mana_cost = 160;
  type = SpellType::kAffliction;
  can_miss = true;
  Setup();
}

CurseOfDoom::CurseOfDoom(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(player, aura, dot) {
  name = SpellName::kCurseOfDoom;
  mana_cost = 380;
  cooldown = 60;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  can_miss = true;
  Setup();
}

Conflagrate::Conflagrate(Player& player) : Spell(player) {
  name = SpellName::kConflagrate;
  mana_cost = 305;
  cooldown = 10;
  min_dmg = 579;
  max_dmg = 721;
  coefficient = 1.5 / 3.5;
  does_damage = true;
  is_finisher = true;
  can_crit = true;
  school = SpellSchool::kFire;
  type = SpellType::kDestruction;
  can_miss = true;
  Setup();
}

bool Conflagrate::Ready() { return player.auras.immolate != NULL && player.auras.immolate->active && Spell::Ready(); }

void Conflagrate::Damage(bool is_crit) {
  Spell::Damage(is_crit);
  player.auras.immolate->Fade();
}

DestructionPotion::DestructionPotion(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kDestructionPotion;
  cooldown = 120;
  is_item = true;
  on_gcd = false;
  Setup();
}

FlameCap::FlameCap(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kFlameCap;
  cooldown = 180;
  is_item = true;
  on_gcd = false;
  shared_cooldown_spells.insert(shared_cooldown_spells.end(),
                                {SpellName::kChippedPowerCore, SpellName::kCrackedPowerCore});
  Setup();
}

BloodFury::BloodFury(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kBloodFury;
  cooldown = 120;
  on_gcd = false;
  Setup();
}

Bloodlust::Bloodlust(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kBloodlust;
  cooldown = 600;
  is_item = true;
  on_gcd = false;
  is_non_warlock_ability = true;
  Setup();
}

DrumsOfBattle::DrumsOfBattle(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kDrumsOfBattle;
  cooldown = 120;
  on_gcd = false;
  is_non_warlock_ability = true;
  is_item = true;
  Setup();
}

DrumsOfWar::DrumsOfWar(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kDrumsOfWar;
  cooldown = 120;
  on_gcd = false;
  is_non_warlock_ability = true;
  is_item = true;
  Setup();
}

DrumsOfRestoration::DrumsOfRestoration(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kDrumsOfRestoration;
  cooldown = 120;
  on_gcd = false;
  is_non_warlock_ability = true;
  is_item = true;
  Setup();
}

AmplifyCurse::AmplifyCurse(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kAmplifyCurse;
  cooldown = 180;
  on_gcd = false;
  Setup();
}

PowerInfusion::PowerInfusion(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kPowerInfusion;
  cooldown = 180;
  on_gcd = false;
  is_non_warlock_ability = true;
  Setup();
}

Innervate::Innervate(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kInnervate;
  cooldown = 360;
  on_gcd = false;
  is_non_warlock_ability = true;
  Setup();
}

ChippedPowerCore::ChippedPowerCore(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kChippedPowerCore;
  cooldown = 120;
  usable_once_per_fight = true;  // The item is unique so you can only carry one at a time, so I'm
                                 // just gonna limit it to 1 use per fight.
  on_gcd = false;
  shared_cooldown_spells.insert(shared_cooldown_spells.end(),
                                {SpellName::kDemonicRune, SpellName::kFlameCap, SpellName::kCrackedPowerCore});
  Setup();
};

CrackedPowerCore::CrackedPowerCore(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kCrackedPowerCore;
  cooldown = 120;
  usable_once_per_fight = true;
  on_gcd = false;
  shared_cooldown_spells.insert(shared_cooldown_spells.end(),
                                {SpellName::kDemonicRune, SpellName::kFlameCap, SpellName::kChippedPowerCore});
  Setup();
};

ManaTideTotem::ManaTideTotem(Player& player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = SpellName::kManaTideTotem;
  cooldown = 300;
  is_non_warlock_ability = true;
}