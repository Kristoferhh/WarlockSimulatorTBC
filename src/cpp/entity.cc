#include "entity.h"

#include "bindings.h"
#include "simulation.h"

Entity::Entity(Player* player, PlayerSettings& player_settings, EntityType entity_type)
    : player(player),
      entity_type(entity_type),
      base_stats(CharacterStats()),
      buff_stats(CharacterStats()),
      debuff_stats(CharacterStats()),
      stats(entity_type == EntityType::kPlayer ? player_settings.stats : CharacterStats()),
      cast_time_remaining(0),
      gcd_remaining(0),
      five_second_rule_timer_remaining(5),
      spirit_tick_timer_remaining(2),
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

bool Entity::ShouldWriteToCombatLog() { return simulation->iteration == 10 && equipped_item_simulation; }