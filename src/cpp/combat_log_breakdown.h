#ifndef WARLOCK_SIMULATOR_TBC_COMBAT_LOG_BREAKDOWN
#define WARLOCK_SIMULATOR_TBC_COMBAT_LOG_BREAKDOWN

#include <iostream>
#include <vector>

struct CombatLogBreakdown {
  std::string name;
  uint32_t casts;
  uint32_t crits;
  uint32_t misses;
  uint32_t iteration_mana_gain;
  uint32_t iteration_damage;
  uint32_t count;  // same as casts but for aura applications
  uint32_t dodge;
  uint32_t glancing_blows;
  double applied_at;
  double uptime;

  CombatLogBreakdown(std::string name)
      : name(name),
        casts(0),
        crits(0),
        misses(0),
        iteration_mana_gain(0),
        iteration_damage(0),
        dodge(0),
        glancing_blows(0) {}
};

#endif