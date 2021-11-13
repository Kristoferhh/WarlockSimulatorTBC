#include "spell.h"

#include <iomanip>

#include "bindings.h"
#include "common.h"
#include "player.h"

Spell::Spell(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : player(player), aura_effect(aura), dot_effect(dot) {
  modifier = 1;
  coefficient = 0;
  cooldown = 0;
  school = SpellSchool::kNoSchool;
  is_non_warlock_ability = false;
  is_dot = false;
  does_damage = false;
  can_crit = false;
  is_item = false;
  is_aura = false;
  on_gcd = true;
  is_proc = false;
  is_finisher = false;
  cast_time = 0;
  usable_once_per_fight = false;
  has_not_been_cast_this_fight = true;
}

void Spell::Reset() {
  cooldown_remaining = 0;
  casting = false;
  has_not_been_cast_this_fight = true;
}

void Spell::Setup() {
  if (min_dmg > 0 && max_dmg > 0) {
    dmg = (min_dmg + max_dmg) / 2;
  }
  if (min_mana > 0 && max_mana > 0) {
    average_mana_value = (min_mana + max_mana) / 2;
  }
  if (player->combat_log_breakdown.count(name) == 0) {
    player->combat_log_breakdown.insert(std::make_pair(name, new CombatLogBreakdown(name)));
  }
}

double Spell::GetCastTime() {
  return round((cast_time / player->GetHastePercent()) * 10000) / 10000 + player->kSpellDelay;
}

bool Spell::CanCast() {
  return cooldown_remaining <= 0 &&
         (is_non_warlock_ability ||
          ((!on_gcd || player->gcd_remaining <= 0) && (is_proc || player->cast_time_remaining <= 0))) &&
         (!usable_once_per_fight || has_not_been_cast_this_fight);
}

bool Spell::HasEnoughMana() { return mana_cost * player->stats->mana_cost_modifier <= player->stats->mana; }

bool Spell::Ready() { return CanCast() && HasEnoughMana(); }

void Spell::StartCast(double predicted_damage) {
  if (on_gcd && !is_non_warlock_ability) {
    // Error: Casting a spell while GCD is active
    if (player->gcd_remaining > 0) {
      player->ThrowError("Attempting to Cast " + name + " while player's GCD is at " +
                         std::to_string(player->gcd_remaining) + " seconds remaining");
    }
    player->gcd_remaining = player->GetGcdValue(shared_from_this());
  }

  // Error: Starting to Cast a spell while casting another spell
  if (player->cast_time_remaining > 0 && !is_non_warlock_ability && !is_proc) {
    player->ThrowError("Attempting to Cast " + name + " while player's Cast time remaining is at " +
                       std::to_string(player->cast_time_remaining) + " sec");
  }

  // Error: Casting a spell while it's on cooldown
  if (cooldown > 0 && cooldown_remaining > 0) {
    player->ThrowError("Attempting to Cast " + name + " while it's still on cooldown (" +
                       std::to_string(cooldown_remaining) + " seconds remaining)");
  }

  std::string combat_log_message = "";
  if (cast_time > 0) {
    casting = true;
    player->cast_time_remaining = GetCastTime();
    if (!is_proc && player->ShouldWriteToCombatLog()) {
      combat_log_message.append(
          "Started casting " + name + " - Cast time: " +
          TruncateTrailingZeros(std::to_string(player->cast_time_remaining - player->kSpellDelay), 4) + " (" +
          TruncateTrailingZeros(std::to_string((player->GetHastePercent() - 1) * 100), 4) +
          "% haste at a base Cast speed of " + TruncateTrailingZeros(std::to_string(cast_time), 2) + ")");
    }
  } else {
    if (!is_proc && player->ShouldWriteToCombatLog()) {
      combat_log_message.append("Cast " + name);
    }
    Cast();
  }
  if (on_gcd && !is_non_warlock_ability && player->ShouldWriteToCombatLog()) {
    combat_log_message.append(" - Global cooldown: " + TruncateTrailingZeros(std::to_string(player->gcd_remaining), 4));
  }
  if (predicted_damage > 0 && player->ShouldWriteToCombatLog()) {
    combat_log_message.append(" - Estimated Damage / Cast time: " +
                              TruncateTrailingZeros(std::to_string(round(predicted_damage))));
  }

  if (player->ShouldWriteToCombatLog()) {
    player->CombatLog(combat_log_message);
  }
}

void Spell::Tick(double t) {
  if (cooldown_remaining > 0 && cooldown_remaining - t <= 0 && player->ShouldWriteToCombatLog()) {
    player->CombatLog(name + " is off cooldown");
  }
  cooldown_remaining -= t;

  if (casting && player->cast_time_remaining <= 0) {
    Cast();
  }
}

void Spell::Cast() {
  const int kCurrentMana = player->stats->mana;
  bool is_crit = false;
  cooldown_remaining = cooldown;
  casting = false;
  has_not_been_cast_this_fight = false;

  if (!is_aura) {
    player->combat_log_breakdown.at(name)->casts++;
  }

  if (mana_cost > 0 && !player->settings->infinite_player_mana) {
    player->stats->mana -= mana_cost * player->stats->mana_cost_modifier;
    player->five_second_rule_timer = 5;
  }

  if (cast_time > 0 && player->ShouldWriteToCombatLog()) {
    player->CombatLog("Finished casting " + name + " - Mana: " + TruncateTrailingZeros(std::to_string(kCurrentMana)) +
                      " -> " + TruncateTrailingZeros(std::to_string(player->stats->mana)) + " - Mana Cost: " +
                      TruncateTrailingZeros(std::to_string(round(mana_cost))) + " - Mana Cost Modifier: " +
                      TruncateTrailingZeros(std::to_string(round(player->stats->mana_cost_modifier * 100))) + "%");
  }

  if (can_crit) {
    is_crit = player->IsCrit(type, bonus_crit);
    if (is_crit) {
      // Increment the crit counter whether the spell hits or not so that the
      // crit % on the Damage breakdown is correct. Otherwise the crit % will be
      // lower due to lost crits when the spell misses.
      player->combat_log_breakdown.at(name)->crits++;
    }
  }

  if (((!is_item && !is_non_warlock_ability &&
        (!player->auras->amplify_curse || name != player->auras->amplify_curse->name)) ||
       does_damage) &&
      !player->IsHit(type)) {
    if (player->ShouldWriteToCombatLog()) {
      player->CombatLog(name + " *resist*");
    }
    player->combat_log_breakdown.at(name)->misses++;
    return;
  }

  if (is_aura) {
    aura_effect->Apply();
  }
  if (is_dot) {
    dot_effect->Apply();
  }

  if (does_damage) {
    Damage(is_crit);
  }

  // If it's an item such as mana potion, demonic rune, destruction potion, or
  // if it's a proc with a hidden cooldown like Blade of Wizardry or Robe of the
  // Elder Scribes then don't check for on-hit procs
  if (!is_item && !is_proc && !is_non_warlock_ability &&
      (!player->auras->amplify_curse || name != player->auras->amplify_curse->name)) {
    OnHitProcs();
  }
}

double Spell::GetModifier() {
  double damage_modifier = modifier;
  if (school == SpellSchool::kShadow) {
    damage_modifier *= player->stats->shadow_modifier;

    // Improved Shadow Bolt
    if (!player->settings->using_custom_isb_uptime && player->auras->improved_shadow_bolt != NULL &&
        player->auras->improved_shadow_bolt->active) {
      damage_modifier *= player->auras->improved_shadow_bolt->modifier;
    }
  } else if (school == SpellSchool::kFire) {
    damage_modifier *= player->stats->fire_modifier;
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
  double crit_multiplier = player->kCritDamageMultiplier;

  if (isCrit) {
    crit_multiplier = GetCritMultiplier(crit_multiplier);
    total_damage *= crit_multiplier;
    OnCritProcs();
  } else if (school == SpellSchool::kShadow && !is_dot && player->auras->improved_shadow_bolt != NULL &&
             player->auras->improved_shadow_bolt->active && !player->settings->using_custom_isb_uptime) {
    player->auras->improved_shadow_bolt->DecrementStacks();
  }

  OnDamageProcs();
  player->iteration_damage += total_damage;

  // Combat Log
  player->AddIterationDamageAndMana(name, 0, total_damage);
  if (player->ShouldWriteToCombatLog()) {
    std::string msg = name + " ";
    if (isCrit) {
      msg += "*";
    }
    msg += TruncateTrailingZeros(std::to_string(round(total_damage)));
    if (isCrit) {
      msg += "*";
    }
    msg += " (" + TruncateTrailingZeros(std::to_string(dmg), 1) + " Base Damage - " +
           TruncateTrailingZeros(std::to_string(round(coefficient * 1000) / 1000), 3) + " Coefficient - " +
           TruncateTrailingZeros(std::to_string(round(kSpellPower))) + " Spell Power - ";
    if (isCrit) {
      msg += TruncateTrailingZeros(std::to_string(crit_multiplier * 100), 3) + "% Crit Multiplier - ";
    }
    msg += TruncateTrailingZeros(std::to_string(round(kDamageModifier * 10000) / 100), 2) + "% Damage Modifier - " +
           TruncateTrailingZeros(std::to_string(round(kPartialResistMultiplier * 1000) / 10)) +
           "% Partial Resist Multiplier)";
    player->CombatLog(msg);
  }

  // T5 4pc
  if (player->sets->t5 >= 4) {
    if (player->spells->shadow_bolt != NULL && name == player->spells->shadow_bolt->name &&
        player->auras->corruption != NULL && player->auras->corruption->active) {
      player->auras->corruption->t5_bonus_modifier *= 1.1;
    } else if (player->spells->incinerate != NULL && name == player->spells->incinerate->name &&
               player->auras->immolate != NULL && player->auras->immolate->active) {
      player->auras->immolate->t5_bonus_modifier *= 1.1;
    }
  }
}

// Returns the non-RNG Damage of the spell (basically just the base Damage +
// spell power + Damage modifiers, no crit/miss etc.)
// TODO investigate this no_rng variable
std::vector<double> Spell::GetConstantDamage(bool no_rng) {
  double total_damage =
      player->settings->randomize_values && min_dmg > 0 && max_dmg > 0 && !no_rng ? Random(min_dmg, max_dmg) : dmg;
  const double kBaseDamage = total_damage;
  const double kSpellPower = player->GetSpellPower(school);
  const double kDamageModifier = GetModifier();
  const double kPartialResistMultiplier = player->GetPartialResistMultiplier(school);

  // If casting Incinerate and Immolate is up, add the bonus Damage
  if (player->spells->incinerate != NULL && name == player->spells->incinerate->name &&
      player->auras->immolate != NULL && player->auras->immolate->active) {
    total_damage += player->settings->randomize_values && no_rng
                        ? Random(bonus_damage_from_immolate_min, bonus_damage_from_immolate_max)
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
  if (player->settings->meta_gem_id == 34220) {
    crit_multiplier *= 1.03;
  }
  // Ruin
  if (type == SpellType::kDestruction && player->talents->ruin == 1) {
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
    crit_damage = kNormalDamage * GetCritMultiplier(player->kCritDamageMultiplier);
    crit_chance = player->GetCritChance(type) / 100;
    chance_to_not_crit = 1 - crit_chance;
  }

  double hit_chance = player->GetHitChance(type) / 100;
  double estimated_damage =
      can_crit ? (kNormalDamage * chance_to_not_crit) + (crit_damage * crit_chance) : kNormalDamage;

  // Add the predicted Damage of the DoT over its full duration
  if (is_dot) {
    estimated_damage += dot_effect->PredictDamage();
  }

  // If the player is not using a custom ISB uptime, they have the ISB talent
  // selected, but the ISB aura is not active, then give some % modifier as an
  // "average" for the Damage. Without this, the sim will choose Incinerate over
  // Shadow Bolt because it basically just doesn't know that ISB exists
  if (school == SpellSchool::kShadow && !player->settings->using_custom_isb_uptime &&
      player->auras->improved_shadow_bolt != NULL && !player->auras->improved_shadow_bolt->active) {
    // If this isn't the player's first iteration then check what their ISB
    // uptime is and add that %
    if (player->iteration > 1) {
      estimated_damage *= (1 + 0.2 * player->auras->improved_shadow_bolt->uptime_so_far);
    }
    // If it's the first iteration where we don't have enough data to assume
    // what the player's ISB uptime is, then add a fixed amount
    else {
      estimated_damage *= 1.15;
    }
  }

  return (estimated_damage * hit_chance) / std::max(player->GetGcdValue(shared_from_this()), GetCastTime());
}

void Spell::OnCritProcs() {
  // ISB
  if (player->spells->shadow_bolt != NULL && name == player->spells->shadow_bolt->name &&
      !player->settings->using_custom_isb_uptime && player->talents->improved_shadow_bolt > 0) {
    player->auras->improved_shadow_bolt->Apply();
  }
  // The Lightning Capacitor
  if (player->spells->the_lightning_capacitor != NULL) {
    player->spells->the_lightning_capacitor->StartCast();
  }
  // Sextant of Unstable Currents
  if (player->spells->sextant_of_unstable_currents != NULL && player->spells->sextant_of_unstable_currents->Ready() &&
      player->GetRand() <= player->spells->sextant_of_unstable_currents->proc_chance * player->kFloatNumberMultiplier) {
    player->spells->sextant_of_unstable_currents->StartCast();
  }
  // Shiffar's Nexus-Horn
  if (player->spells->shiffars_nexus_horn != NULL && player->spells->shiffars_nexus_horn->Ready() &&
      player->GetRand() <= player->spells->shiffars_nexus_horn->proc_chance * player->kFloatNumberMultiplier) {
    player->spells->shiffars_nexus_horn->StartCast();
  }
}

void Spell::OnDamageProcs() {
  // Confirm that this procs on dealing Damage
  // Shattered Sun Pendant of Acumen
  if (player->settings->exalted_with_shattrath_faction && player->spells->shattered_sun_pendant_of_acumen != NULL &&
      player->spells->shattered_sun_pendant_of_acumen->cooldown_remaining <= 0 &&
      player->GetRand() <=
          player->spells->shattered_sun_pendant_of_acumen->proc_chance * player->kFloatNumberMultiplier) {
    player->spells->shattered_sun_pendant_of_acumen->StartCast();
  }
}

void Spell::OnHitProcs() {
  // Judgement of Wisdom (50% proc rate)
  if (player->selected_auras->judgement_of_wisdom && player->GetRand() <= 50 * player->kFloatNumberMultiplier) {
    const int kManaValue = 74;
    const int kCurrentMana = player->stats->mana;
    const int kManaGained = std::min(player->stats->max_mana - kCurrentMana, kManaValue);
    player->stats->mana += kManaGained;
    player->combat_log_breakdown.at("Judgement of Wisdom")->casts++;
    player->AddIterationDamageAndMana("Judgement of Wisdom", kManaGained, 0);
    if (player->ShouldWriteToCombatLog()) {
      player->CombatLog("Player gains " + std::to_string(kManaGained) + " mana from Judgement of Wisdom (" +
                        std::to_string(kCurrentMana) + " -> " + std::to_string(player->stats->mana) + ")");
    }
  }
  // T4 2pc
  if (player->sets->t4 >= 2 && (school == SpellSchool::kShadow || school == SpellSchool::kFire) &&
      player->GetRand() <= player->auras->flameshadow->proc_chance * player->kFloatNumberMultiplier) {
    if (school == SpellSchool::kShadow) {
      player->auras->flameshadow->Apply();
    } else if (school == SpellSchool::kFire) {
      player->auras->shadowflame->Apply();
    }
  }
  // Spellstrike
  if (player->sets->spellstrike == 2 &&
      player->GetRand() <= player->auras->spellstrike->proc_chance * player->kFloatNumberMultiplier) {
    player->auras->spellstrike->Apply();
  }
  // Quagmirran's Eye
  if (player->spells->quagmirrans_eye != NULL && player->spells->quagmirrans_eye->Ready() &&
      player->GetRand() <= player->spells->quagmirrans_eye->proc_chance * player->kFloatNumberMultiplier) {
    player->spells->quagmirrans_eye->StartCast();
  }
  // Mana-Etched Regalia 4pc
  if (player->sets->mana_etched >= 4 &&
      player->GetRand() <= player->auras->mana_etched_4_set->proc_chance * player->kFloatNumberMultiplier) {
    player->auras->mana_etched_4_set->Apply();
  }
  // Mark of Defiance
  if (player->spells->mark_of_defiance != NULL && player->spells->mark_of_defiance->Ready() &&
      player->GetRand() <= player->spells->mark_of_defiance->proc_chance * player->kFloatNumberMultiplier) {
    player->spells->mark_of_defiance->StartCast();
  }
  // Darkmoon Card: Crusade
  if (player->auras->darkmoon_card_crusade != NULL) {
    player->auras->darkmoon_card_crusade->Apply();
  }
  // Band of the Eternal Sage
  if (player->spells->band_of_the_eternal_sage != NULL && player->spells->band_of_the_eternal_sage->Ready() &&
      player->GetRand() <= player->spells->band_of_the_eternal_sage->proc_chance * player->kFloatNumberMultiplier) {
    player->spells->band_of_the_eternal_sage->StartCast();
  }
  // Blade of Wizardry
  if (player->spells->blade_of_wizardry != NULL && player->spells->blade_of_wizardry->Ready() &&
      player->GetRand() <= player->auras->blade_of_wizardry->proc_chance * player->kFloatNumberMultiplier) {
    player->spells->blade_of_wizardry->StartCast();
  }
  // Mystical Skyfire Diamond
  if (player->spells->mystical_skyfire_diamond != NULL && player->spells->mystical_skyfire_diamond->Ready() &&
      player->GetRand() <= player->spells->mystical_skyfire_diamond->proc_chance * player->kFloatNumberMultiplier) {
    player->spells->mystical_skyfire_diamond->StartCast();
  }
  // Robe of the Elder Scribes
  if (player->spells->robe_of_the_elder_scribes != NULL && player->spells->robe_of_the_elder_scribes->Ready() &&
      player->GetRand() <= player->spells->robe_of_the_elder_scribes->proc_chance * player->kFloatNumberMultiplier) {
    player->spells->robe_of_the_elder_scribes->StartCast();
  }
  // Insightful Earthstorm Diamond
  if (player->spells->insightful_earthstorm_diamond != NULL && player->spells->insightful_earthstorm_diamond->Ready() &&
      player->GetRand() <=
          player->spells->insightful_earthstorm_diamond->proc_chance * player->kFloatNumberMultiplier) {
    player->spells->insightful_earthstorm_diamond->StartCast();
  }
  // Wrath of Cenarius
  if (player->auras->wrath_of_cenarius != NULL &&
      player->GetRand() <= player->auras->wrath_of_cenarius->proc_chance * player->kFloatNumberMultiplier) {
    player->auras->wrath_of_cenarius->Apply();
  }
}

ShadowBolt::ShadowBolt(std::shared_ptr<Player> player) : Spell(player) {
  cast_time = CalculateCastTime();
  mana_cost = 420 * (1 - 0.01 * player->talents->cataclysm);
  coefficient = (3 / 3.5) + (0.04 * player->talents->shadow_and_flame);
  min_dmg = 544;
  max_dmg = 607;
  name = "Shadow Bolt";
  does_damage = true;
  can_crit = true;
  school = SpellSchool::kShadow;
  type = SpellType::kDestruction;
  Setup();

  // T6 4pc bonus
  if (player->sets->t6 >= 4) {
    modifier *= 1.06;
  }
}

void ShadowBolt::StartCast(double predicted_damage = 0) {
  const bool kHasShadowTrance = player->auras->shadow_trance != NULL;

  if (kHasShadowTrance && player->auras->shadow_trance->active) {
    cast_time = 0;
  }

  Spell::StartCast();

  if (kHasShadowTrance && player->auras->shadow_trance->active) {
    cast_time = CalculateCastTime();
    player->auras->shadow_trance->Fade();
  }
}

double ShadowBolt::CalculateCastTime() { return 3 - (0.1 * player->talents->bane); }

LifeTap::LifeTap(std::shared_ptr<Player> player) : Spell(player) {
  name = "Life Tap";
  mana_return = 582;
  coefficient = 0.8;
  modifier = 1 * (1 + 0.1 * player->talents->improved_life_tap);
  Setup();
}

int LifeTap::ManaGain() {
  return (mana_return + ((player->GetSpellPower() + player->stats->shadow_power) * coefficient)) * modifier;
}

void LifeTap::Cast() {
  const int kManaGain = this->ManaGain();
  player->combat_log_breakdown.at(name)->casts++;
  player->AddIterationDamageAndMana(name, kManaGain, 0);

  if (player->ShouldWriteToCombatLog() && player->stats->mana + kManaGain > player->stats->max_mana) {
    player->CombatLog("Life Tap used at too high mana (mana wasted)");
  }
  player->stats->mana = std::min(player->stats->max_mana, player->stats->mana + kManaGain);
}

Incinerate::Incinerate(std::shared_ptr<Player> player) : Spell(player) {
  name = "Incinerate";
  cast_time = round((2.5 * (1 - 0.02 * player->talents->emberstorm)) * 100) / 100;
  mana_cost = 355 * (1 - 0.01 * player->talents->cataclysm);
  coefficient = (2.5 / 3.5) + (0.04 * player->talents->shadow_and_flame);
  min_dmg = 444;
  max_dmg = 514;
  bonus_damage_from_immolate_min = 111;
  bonus_damage_from_immolate_max = 128;
  bonus_damage_from_immolate_average = (bonus_damage_from_immolate_min + bonus_damage_from_immolate_max) / 2;
  does_damage = true;
  can_crit = true;
  school = SpellSchool::kFire;
  type = SpellType::kDestruction;
  Setup();

  if (player->sets->t6 >= 4) {
    modifier *= 1.06;
  }
}

SearingPain::SearingPain(std::shared_ptr<Player> player) : Spell(player) {
  name = "Searing Pain";
  cast_time = 1.5;
  mana_cost = 205 * (1 - 0.01 * player->talents->cataclysm);
  coefficient = 1.5 / 3.5;
  min_dmg = 270;
  max_dmg = 320;
  does_damage = true;
  can_crit = true;
  school = SpellSchool::kFire;
  type = SpellType::kDestruction;
  bonus_crit = 4 * player->talents->improved_searing_pain;
  Setup();
};

SoulFire::SoulFire(std::shared_ptr<Player> player) : Spell(player) {
  name = "Soul Fire";
  cast_time = 6 - (0.4 * player->talents->bane);
  mana_cost = 250 * (1 - 0.01 * player->talents->cataclysm);
  coefficient = 1.15;
  min_dmg = 1003;
  max_dmg = 1257;
  does_damage = true;
  can_crit = true;
  school = SpellSchool::kFire;
  type = SpellType::kDestruction;
  Setup();
};

Shadowburn::Shadowburn(std::shared_ptr<Player> player) : Spell(player) {
  name = "Shadowburn";
  cooldown = 15;
  mana_cost = 515 * (1 - 0.01 * player->talents->cataclysm);
  coefficient = 0.22;
  min_dmg = 597;
  max_dmg = 665;
  does_damage = true;
  can_crit = true;
  is_finisher = true;
  school = SpellSchool::kShadow;
  type = SpellType::kDestruction;
  Setup();
};

DeathCoil::DeathCoil(std::shared_ptr<Player> player) : Spell(player) {
  name = "Death Coil";
  cooldown = 120;
  mana_cost = 600;
  coefficient = 0.4286;
  dmg = 526;
  does_damage = true;
  is_finisher = true;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  Setup();
};

Shadowfury::Shadowfury(std::shared_ptr<Player> player) : Spell(player) {
  name = "Shadowfury";
  cast_time = 0.5;
  mana_cost = 710 * (1 - 0.01 * player->talents->cataclysm);
  min_dmg = 612;
  max_dmg = 728;
  does_damage = true;
  can_crit = true;
  school = SpellSchool::kShadow;
  type = SpellType::kDestruction;
  cooldown = 20;
  coefficient = 0.195;
  Setup();
}

SeedOfCorruption::SeedOfCorruption(std::shared_ptr<Player> player) : Spell(player) {
  name = "Seed of Corruption";
  min_dmg = 1110;
  max_dmg = 1290;
  mana_cost = 882;
  cast_time = 2;
  aoe_cap = 13580;
  does_damage = true;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  coefficient = 0.214;
  Setup();
};

void SeedOfCorruption::Damage(bool isCrit) {
  const int kBaseDamage = player->settings->randomize_values && min_dmg && max_dmg ? Random(min_dmg, max_dmg) : dmg;
  const int kEnemyAmount = player->settings->enemy_amount - 1;  // Minus one because the enemy that Seed is being Cast
                                                                // on doesn't get hit
  int resist_amount = 0;
  int crit_amount = 0;
  const int kSpellPower = player->GetSpellPower(school);
  double crit_damage_multiplier = 0;
  double internal_modifier = GetModifier();
  double external_modifier = 1;

  // Remove debuffs from the modifier since they ignore the AOE cap, so we'll
  // add the debuff % modifiers after the Damage has been calculated.
  if (player->selected_auras->curse_of_the_elements) {
    const double kModifier = 1.1 + (0.01 * player->settings->improved_curse_of_the_elements);
    internal_modifier /= kModifier;
    external_modifier *= kModifier;
  }
  if (player->selected_auras->shadow_weaving) {
    const double kModifier = 1.1;
    internal_modifier /= kModifier;
    external_modifier *= kModifier;
  }
  if (player->selected_auras->misery) {
    const double kModifier = 1.05;
    internal_modifier /= kModifier;
    external_modifier *= kModifier;
  }

  for (int i = 0; i < kEnemyAmount; i++) {
    // Check for a resist
    if (!player->IsHit(type)) {
      resist_amount++;
    } else {
      OnDamageProcs();
      // Check for a crit
      if (player->IsCrit(type)) {
        crit_amount++;
        OnCritProcs();
      }
    }
  }

  double individual_seed_damage = kBaseDamage + (kSpellPower * coefficient);
  // Oblivion Raiment (dungeon set) 4pc bonus
  if (player->sets->oblivion >= 4) {
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
    crit_damage_multiplier = GetCritMultiplier(player->kCritDamageMultiplier);
    const double kIndividualSeedCrit = individual_seed_damage * crit_damage_multiplier;
    const double kBonusDamageFromCrit = kIndividualSeedCrit - individual_seed_damage;
    total_seed_damage += kBonusDamageFromCrit * crit_amount;
  }
  // Partial resists (probably need to calculate a partial resist for each seed
  // hit, not sure how it interacts for the aoe cap)
  const double kPartialResistMultiplier = player->GetPartialResistMultiplier(school);
  total_seed_damage *= kPartialResistMultiplier;

  // Add Damage from debuffs
  total_seed_damage *= external_modifier;

  player->iteration_damage += total_seed_damage;

  if (player->ShouldWriteToCombatLog()) {
    std::string msg =
        name + " " + TruncateTrailingZeros(std::to_string(round(total_seed_damage))) + " (" +
        std::to_string(kEnemyAmount) + " Enemies (" + std::to_string(resist_amount) + " Resists & " +
        std::to_string(crit_amount) + " Crits) - " + std::to_string(kBaseDamage) + " Base Damage - " +
        TruncateTrailingZeros(std::to_string(coefficient), 3) + " Coefficient - " + std::to_string(kSpellPower) +
        " Spell Power - " +
        TruncateTrailingZeros(std::to_string(round(internal_modifier * external_modifier * 1000) / 10), 1) +
        "% Modifier - ";
    if (crit_amount > 0) {
      msg += TruncateTrailingZeros(std::to_string(crit_damage_multiplier), 3) + "% Crit Multiplier";
    }
    msg += " - " + TruncateTrailingZeros(std::to_string(round(kPartialResistMultiplier * 1000) / 10)) +
           "% Partial Resist Multiplier)";
    player->CombatLog(msg);
  }
  player->AddIterationDamageAndMana(name, 0, total_seed_damage);
  player->combat_log_breakdown.at(name)->crits += crit_amount;
  player->combat_log_breakdown.at(name)->misses += resist_amount;
  // the Cast() function already adds 1 to the amount of casts so we only need
  // to add enemiesHit - 1 to the Cast amount
  player->combat_log_breakdown.at(name)->casts += (kEnemiesHit - 1);
}

double SeedOfCorruption::GetModifier() {
  double modifier = Spell::GetModifier();
  if (player->talents->shadow_mastery > 0 && player->talents->contagion > 0) {
    // Divide away the bonus from Shadow Mastery
    modifier /= (1 + (player->talents->shadow_mastery * 0.02));
    // Multiply the modifier with the bonus from Shadow Mastery + Contagion
    modifier *= (1 * (1 + ((player->talents->shadow_mastery * 0.02) + (player->talents->contagion / 100.0))));
  }
  return modifier;
}

DarkPact::DarkPact(std::shared_ptr<Player> player) : Spell(player) {
  name = "Dark Pact";
  mana_return = 700;
  coefficient = 0.96;
  Setup();
}

double DarkPact::ManaGain() { return 0; }

bool DarkPact::Ready() { return false; }

void DarkPact::Cast() {}

Corruption::Corruption(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(player, aura, dot) {
  name = "Corruption";
  mana_cost = 370;
  cast_time = round((2 - (0.4 * player->talents->improved_corruption)) * 100) / 100.0;
  is_dot = true;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  Setup();
}

UnstableAffliction::UnstableAffliction(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura,
                                       std::shared_ptr<DamageOverTime> dot)
    : Spell(player, aura, dot) {
  name = "Unstable Affliction";
  mana_cost = 400;
  cast_time = 1.5;
  is_dot = true;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  Setup();
}

SiphonLife::SiphonLife(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(player, aura, dot) {
  name = "Siphon Life";
  mana_cost = 410;
  is_dot = true;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  Setup();
}

Immolate::Immolate(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot)
    : Spell(player, aura, dot) {
  name = "Immolate";
  mana_cost = 445 * (1 - 0.01 * player->talents->cataclysm);
  cast_time = 2 - (0.1 * player->talents->bane);
  is_dot = true;
  does_damage = true;
  can_crit = true;
  dmg = 331;
  coefficient = 0.2;
  school = SpellSchool::kFire;
  type = SpellType::kDestruction;
  Setup();
}

double Immolate::GetModifier() {
  double modifier = Spell::GetModifier();
  if (player->talents->emberstorm > 0) {
    modifier /= (1 + 0.02 * player->talents->emberstorm);
  }
  modifier *= (1 + (0.02 * player->talents->emberstorm + 0.05 * player->talents->improved_immolate));
  return modifier;
}

CurseOfAgony::CurseOfAgony(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura,
                           std::shared_ptr<DamageOverTime> dot)
    : Spell(player, aura, dot) {
  name = "Curse of Agony";
  mana_cost = 265;
  is_dot = true;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  Setup();
}

CurseOfTheElements::CurseOfTheElements(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura)
    : Spell(player, aura) {
  name = "Curse of the Elements";
  mana_cost = 260;
  type = SpellType::kAffliction;
  is_aura = true;
  Setup();
}

CurseOfRecklessness::CurseOfRecklessness(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura)
    : Spell(player, aura) {
  name = "Curse of Recklessness";
  mana_cost = 160;
  type = SpellType::kAffliction;
  is_aura = true;
  Setup();
}

CurseOfDoom::CurseOfDoom(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura,
                         std::shared_ptr<DamageOverTime> dot)
    : Spell(player, aura, dot) {
  name = "Curse of Doom";
  mana_cost = 380;
  cooldown = 60;
  school = SpellSchool::kShadow;
  type = SpellType::kAffliction;
  is_dot = true;
  Setup();
}

Conflagrate::Conflagrate(std::shared_ptr<Player> player) : Spell(player) {
  name = "Conflagrate";
  mana_cost = 305 * (1 - 0.01 * player->talents->cataclysm);
  cooldown = 10;
  min_dmg = 579;
  max_dmg = 721;
  coefficient = 1.5 / 3.5;
  does_damage = true;
  is_finisher = true;
  can_crit = true;
  school = SpellSchool::kFire;
  type = SpellType::kDestruction;
  Setup();
}

void Conflagrate::StartCast() {
  /*if (player->auras->Immolate != NULL && player->auras->immolate->active)
  {
      Spell::StartCast();
      player->auras->immolate->active = false;
  }*/
}

DestructionPotion::DestructionPotion(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Destruction Potion";
  cooldown = 120;
  is_item = true;
  is_aura = true;
  on_gcd = false;
  Setup();
}

SuperManaPotion::SuperManaPotion(std::shared_ptr<Player> player) : Spell(player) {
  name = "Super Mana Potion";
  cooldown = 120;
  is_item = true;
  min_mana = 1800;
  max_mana = 3000;
  on_gcd = false;
  Setup();
}

void SuperManaPotion::Cast() {
  Spell::Cast();
  const int kCurrentPlayerMana = player->stats->mana;
  // todo check for the randomize values option
  const int kManaGain = Random(min_mana, max_mana);
  player->AddIterationDamageAndMana(name, kManaGain, 0);
  player->stats->mana = std::min(player->stats->max_mana, kCurrentPlayerMana + kManaGain);
  if (player->ShouldWriteToCombatLog()) {
    player->CombatLog("Player gains " +
                      TruncateTrailingZeros(std::to_string(round(player->stats->mana - kCurrentPlayerMana))) +
                      " mana from " + name + " (" + TruncateTrailingZeros(std::to_string(round(kCurrentPlayerMana))) +
                      " -> " + TruncateTrailingZeros(std::to_string(round(player->stats->mana))) + ")");
  }
}

DemonicRune::DemonicRune(std::shared_ptr<Player> player) : Spell(player) {
  name = "Demonic Rune";
  cooldown = 120;
  is_item = true;
  min_mana = 900;
  max_mana = 1500;
  on_gcd = false;
  Setup();
}

void DemonicRune::Cast() {
  Spell::Cast();
  const int kCurrentPlayerMana = player->stats->mana;
  // todo check for the randomize values option
  const int kManaGain = Random(min_mana, max_mana);
  player->AddIterationDamageAndMana(name, kManaGain, 0);
  player->stats->mana = std::min(player->stats->max_mana, kCurrentPlayerMana + kManaGain);
  if (player->ShouldWriteToCombatLog()) {
    player->CombatLog("Player gains " +
                      TruncateTrailingZeros(std::to_string(round(player->stats->mana - kCurrentPlayerMana))) +
                      " mana from " + name + " (" + TruncateTrailingZeros(std::to_string(round(kCurrentPlayerMana))) +
                      " -> " + TruncateTrailingZeros(std::to_string(round(player->stats->mana))) + ")");
  }

  // Put Chipped Power Core and Cracked Power Core on cooldown
  if (player->spells->chipped_power_core != NULL) {
    player->spells->chipped_power_core->cooldown_remaining = cooldown;
  }
  if (player->spells->cracked_power_core != NULL) {
    player->spells->cracked_power_core->cooldown_remaining = cooldown;
  }
}

FlameCap::FlameCap(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Flame Cap";
  cooldown = 180;
  is_item = true;
  is_aura = true;
  on_gcd = false;
  Setup();
}

void FlameCap::Cast() {
  Spell::Cast();

  if (player->spells->chipped_power_core != NULL) {
    player->spells->chipped_power_core->cooldown_remaining = cooldown;
  }
  if (player->spells->cracked_power_core != NULL) {
    player->spells->cracked_power_core->cooldown_remaining = cooldown;
  }
}

BloodFury::BloodFury(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Blood Fury";
  cooldown = 120;
  is_aura = true;
  on_gcd = false;
  Setup();
}

Bloodlust::Bloodlust(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Bloodlust";
  cooldown = 600;
  is_item = true;
  is_aura = true;
  on_gcd = false;
  is_non_warlock_ability = true;
  Setup();
}

DrumsOfBattle::DrumsOfBattle(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Drums of Battle";
  cooldown = 120;
  is_aura = true;
  on_gcd = false;
  is_non_warlock_ability = true;
  is_item = true;
  Setup();
}

bool DrumsOfBattle::Ready() { return cooldown_remaining <= 0; }

DrumsOfWar::DrumsOfWar(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Drums of War";
  cooldown = 120;
  is_aura = true;
  on_gcd = false;
  is_non_warlock_ability = true;
  is_item = true;
  Setup();
}

bool DrumsOfWar::Ready() { return cooldown_remaining <= 0; }

DrumsOfRestoration::DrumsOfRestoration(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura)
    : Spell(player, aura) {
  name = "Drums of Restoration";
  cooldown = 120;
  is_aura = true;
  on_gcd = false;
  is_non_warlock_ability = true;
  is_item = true;
  Setup();
}

bool DrumsOfRestoration::Ready() { return cooldown_remaining <= 0; }

TimbalsFocusingCrystal::TimbalsFocusingCrystal(std::shared_ptr<Player> player) : Spell(player) {
  name = "Timbal's Focusing Crystal";
  cooldown = 15;
  on_gcd = false;
  proc_chance = 10;
  min_dmg = 285;
  max_dmg = 475;
  does_damage = true;
  is_proc = true;
  school = SpellSchool::kShadow;
  can_crit = true;
  Setup();
}

MarkOfDefiance::MarkOfDefiance(std::shared_ptr<Player> player) : Spell(player) {
  name = "Mark of Defiance";
  cooldown = 17;
  proc_chance = 15;
  on_gcd = false;
  min_mana = 128;
  max_mana = 172;
  Setup();
}

void MarkOfDefiance::Cast() {
  if (cooldown_remaining <= 0) {
    const int kCurrentPlayerMana = player->stats->mana;
    player->stats->mana =
        std::min(static_cast<double>(player->stats->max_mana), kCurrentPlayerMana + average_mana_value);
    if (player->ShouldWriteToCombatLog()) {
      player->CombatLog("Player gains " +
                        TruncateTrailingZeros(std::to_string(round(player->stats->mana - kCurrentPlayerMana))) +
                        " mana from " + name + " (" + TruncateTrailingZeros(std::to_string(round(kCurrentPlayerMana))) +
                        " -> " + TruncateTrailingZeros(std::to_string(round(player->stats->mana))) + ")");
    }
    cooldown_remaining = cooldown;
  }
}

TheLightningCapacitor::TheLightningCapacitor(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura)
    : Spell(player, aura) {
  name = "The Lightning Capacitor";
  cooldown = 2.5;
  min_dmg = 694;
  max_dmg = 806;
  does_damage = true;
  can_crit = true;
  on_gcd = false;
  Setup();
}

void TheLightningCapacitor::StartCast(double predicted_damage) {
  if (cooldown_remaining <= 0) {
    player->auras->the_lightning_capacitor->Apply();
    if (player->auras->the_lightning_capacitor->stacks == 3) {
      Spell::StartCast();
      cooldown_remaining = cooldown;
      player->auras->the_lightning_capacitor->Fade();
    }
  }
}

BladeOfWizardry::BladeOfWizardry(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Blade of Wizardry";
  cooldown = 50;
  on_gcd = false;
  is_item = true;
  is_proc = true;
  is_aura = true;
  Setup();
}

ShatteredSunPendantOfAcumen::ShatteredSunPendantOfAcumen(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura)
    : Spell(player, aura) {
  name = "Shattered Sun Pendant of Acumen";
  cooldown = 45;
  proc_chance = 15;
  on_gcd = false;
  is_item = true;
  if (player->settings->is_aldor) {
    this->is_proc = true;
    this->is_aura = true;
  } else {
    this->does_damage = true;
    this->can_crit = true;
    this->dmg = 333;  // confirm
  }
  Setup();
}

RobeOfTheElderScribes::RobeOfTheElderScribes(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura)
    : Spell(player, aura) {
  name = "Robe of the Elder Scribes";
  cooldown = 50;
  proc_chance = 20;
  on_gcd = false;
  is_item = true;
  is_proc = true;
  is_aura = true;
  Setup();
}

QuagmirransEye::QuagmirransEye(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Quagmirran's Eye";
  cooldown = 45;
  proc_chance = 10;
  on_gcd = false;
  is_item = true;
  is_aura = true;
  Setup();
}

ShiffarsNexusHorn::ShiffarsNexusHorn(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Shiffar's Nexus-Horn";
  cooldown = 45;
  proc_chance = 20;
  on_gcd = false;
  is_item = true;
  is_aura = true;
  Setup();
}

SextantOfUnstableCurrents::SextantOfUnstableCurrents(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura)
    : Spell(player, aura) {
  name = "Sextant of Unstable Currents";
  cooldown = 45;
  proc_chance = 20;
  on_gcd = false;
  is_item = true;
  is_aura = true;
  Setup();
}

BandOfTheEternalSage::BandOfTheEternalSage(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura)
    : Spell(player, aura) {
  name = "Band of the Eternal Sage";
  cooldown = 60;
  proc_chance = 10;
  on_gcd = false;
  is_item = true;
  is_aura = true;
  Setup();
}

MysticalSkyfireDiamond::MysticalSkyfireDiamond(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura)
    : Spell(player, aura) {
  name = "Mystical Skyfire Diamond";
  cooldown = 35;
  proc_chance = 15;
  on_gcd = false;
  is_proc = true;
  is_item = true;
  is_aura = true;
  Setup();
}

InsightfulEarthstormDiamond::InsightfulEarthstormDiamond(std::shared_ptr<Player> player) : Spell(player) {
  name = "Insightful Earthstorm Diamond";
  cooldown = 15;
  proc_chance = 5;
  on_gcd = false;
  is_proc = true;
  is_item = true;
  mana_gain = 300;
  Setup();
}

void InsightfulEarthstormDiamond::Cast() {
  Spell::Cast();
  const int kCurrentPlayerMana = player->stats->mana;
  player->stats->mana = std::min(static_cast<double>(player->stats->max_mana), kCurrentPlayerMana + mana_gain);
  if (player->ShouldWriteToCombatLog()) {
    player->CombatLog("Player gains " + std::to_string(round(player->stats->mana - kCurrentPlayerMana)) +
                      " mana from " + name + " (" + std::to_string(round(kCurrentPlayerMana)) + " -> " +
                      std::to_string(round(player->stats->mana)) + ")");
  }
}

AmplifyCurse::AmplifyCurse(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Amplify Curse";
  cooldown = 180;
  is_aura = true;
  on_gcd = false;
  Setup();
}

PowerInfusion::PowerInfusion(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Power Infusion";
  cooldown = 180;
  is_aura = true;
  on_gcd = false;
  is_non_warlock_ability = true;
  Setup();
}

Innervate::Innervate(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Innervate";
  cooldown = 360;
  is_aura = true;
  on_gcd = false;
  is_non_warlock_ability = true;
  Setup();
}

ChippedPowerCore::ChippedPowerCore(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Chipped Power Core";
  cooldown = 120;
  usable_once_per_fight = true;  // The item is unique so you can only carry one at a time, so I'm
                                 // just gonna limit it to 1 use per fight.
  is_aura = true;
  on_gcd = false;
  Setup();
};

void ChippedPowerCore::Cast() {
  Spell::Cast();

  if (player->spells->demonic_rune != NULL) {
    player->spells->demonic_rune->cooldown_remaining = cooldown;
  }
  if (player->spells->flame_cap != NULL) {
    player->spells->flame_cap->cooldown_remaining = cooldown;
  }
  if (player->spells->cracked_power_core != NULL) {
    player->spells->cracked_power_core->cooldown_remaining = cooldown;
  }
}

CrackedPowerCore::CrackedPowerCore(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = "Cracked Power Core";
  cooldown = 120;
  usable_once_per_fight = true;  // The item is unique so you can only carry one at a time, so I'm
                                 // just gonna limit it to 1 use per fight.
  is_aura = true;
  on_gcd = false;
  Setup();
};

void CrackedPowerCore::Cast() {
  Spell::Cast();

  if (player->spells->demonic_rune != NULL) {
    player->spells->demonic_rune->cooldown_remaining = cooldown;
  }
  if (player->spells->flame_cap != NULL) {
    player->spells->flame_cap->cooldown_remaining = cooldown;
  }
  if (player->spells->chipped_power_core != NULL) {
    player->spells->chipped_power_core->cooldown_remaining = cooldown;
  }
}

ManaTideTotem::ManaTideTotem(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura) : Spell(player, aura) {
  name = aura->name;
  cooldown = 300;
  is_aura = true;
  is_non_warlock_ability = true;
}