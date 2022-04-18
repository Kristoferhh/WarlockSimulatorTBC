#pragma once
#include <string>

struct CombatLogBreakdown {
  std::string name;
  uint32_t casts = 0;
  uint32_t crits = 0;
  uint32_t misses = 0;
  double iteration_mana_gain = 0;
  double iteration_damage = 0;
  uint32_t count = 0; // same as casts but for aura applications
  uint32_t dodge = 0;
  uint32_t glancing_blows = 0;
  double applied_at = 0;
  double uptime = 0;

  explicit CombatLogBreakdown(std::string name)
    : name(std::move(name)) {
  }
};
