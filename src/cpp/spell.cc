#include "spell.h"

#include "common.h"
#include "entity.h"
#include "player/player.h"

Spell::Spell(Entity& entity)
    : entity(entity),
      aura_effect(nullptr),
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
      amount_of_casts_per_fight(0) {}

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

  if (aura_effect == NULL && entity.recording_combat_log_breakdown) {
    entity.combat_log_breakdown.at(name)->casts++;
  }

  if (mana_cost > 0 && !entity.infinite_mana) {
    entity.stats.mana -= kManaCost;
    entity.five_second_rule_timer_remaining = 5;
  }

  if (cast_time > 0 && entity.ShouldWriteToCombatLog()) {
    entity.CombatLog(
        entity.name + " finished casting " + name + " - Mana: " + DoubleToString(kCurrentMana) + " -> " +
        DoubleToString(entity.stats.mana) + " - Mana Cost: " + DoubleToString(round(kManaCost)) +
        (entity.entity_type == EntityType::kPlayer
             ? " - Mana Cost Modifier: " + DoubleToString(round(entity.stats.mana_cost_modifier * 100)) + "%"
             : ""));
  }
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