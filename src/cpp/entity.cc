#include "entity.h"

#include "bindings.h"
#include "common.h"
#include "simulation.h"

// We need the PlayerSettings struct that is passed to the Player struct because if we try to access the settings via
// Player->settings then that struct won't be initialized yet when the Entity constructor is called during the Player
// constructor
Entity::Entity(Player* player, PlayerSettings& player_settings, EntityType entity_type)
    : player(player),
      entity_type(entity_type),
      stats(entity_type == EntityType::kPlayer ? player_settings.stats : CharacterStats()),
      cast_time_remaining(0),
      gcd_remaining(0),
      five_second_rule_timer_remaining(5),
      mp5_timer_remaining(5),
      recording_combat_log_breakdown(player_settings.recording_combat_log_breakdown &&
                                     player_settings.equipped_item_simulation),
      equipped_item_simulation(player_settings.equipped_item_simulation) {}

void Entity::PostIterationDamageAndMana(const std::string& spell_name) {
  PostCombatLogBreakdownVector(spell_name.c_str(), combat_log_breakdown.at(spell_name)->iteration_mana_gain,
                               combat_log_breakdown.at(spell_name)->iteration_damage);
  combat_log_breakdown.at(spell_name)->iteration_damage = 0;
  combat_log_breakdown.at(spell_name)->iteration_mana_gain = 0;
}

void Entity::SendCombatLogBreakdown() {
  for (auto& combat_log_breakdown : combat_log_breakdown) {
    if (combat_log_breakdown.second->iteration_damage > 0 || combat_log_breakdown.second->iteration_mana_gain > 0) {
      PostIterationDamageAndMana(combat_log_breakdown.first);
    }

    PostCombatLogBreakdown(combat_log_breakdown.second->name.c_str(), combat_log_breakdown.second->casts,
                           combat_log_breakdown.second->crits, combat_log_breakdown.second->misses,
                           combat_log_breakdown.second->count, combat_log_breakdown.second->uptime,
                           combat_log_breakdown.second->dodge, combat_log_breakdown.second->glancing_blows);
  }
}

double Entity::GetStamina() { return stats.stamina * stats.stamina_modifier; }

double Entity::GetIntellect() { return stats.intellect * stats.intellect_modifier; }

double Entity::GetSpirit() { return stats.spirit * stats.spirit_modifier; }

bool Entity::ShouldWriteToCombatLog() { return simulation->iteration == 10 && equipped_item_simulation; }

void Entity::CombatLog(const std::string& entry) {
  player->combat_log_entries.push_back("|" + DoubleToString(simulation->fight_time, 4) + "| " + entry);
}

double Entity::FindTimeUntilNextAction() {
  double time = cast_time_remaining;

  if (time <= 0) {
    time = gcd_remaining;
  }

  if (time <= 0 || (mp5_timer_remaining < time && mp5_timer_remaining > 0)) {
    time = mp5_timer_remaining;
  }

  return time;
}

void Entity::Tick(double time) {
  cast_time_remaining -= time;
  gcd_remaining -= time;
  five_second_rule_timer_remaining -= time;
  mp5_timer_remaining -= time;

  // Auras need to tick before Spells because otherwise you'll, for example,
  // finish casting Corruption and then immediately afterwards, in the same
  // millisecond, immediately tick down the aura This was also causing buffs like
  // e.g. the t4 4pc buffs to expire sooner than they should.
  for (auto& aura : aura_list) {
    if (aura->active && aura->duration_remaining > 0) {
      aura->Tick(time);
    }
  }
}