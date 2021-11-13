#ifndef WARLOCK_SIMULATOR_TBC_COMBAT_LOG_BREAKDOWN
#define WARLOCK_SIMULATOR_TBC_COMBAT_LOG_BREAKDOWN

#include <iostream>
#include <vector>

struct CombatLogBreakdown {
  std::string name;
  uint32_t casts;
  uint32_t crits;
  uint32_t misses;
  uint32_t iterationManaGain;
  uint32_t iterationDamage;
  uint32_t count;  // same as casts but for aura applications
  uint32_t dodge;
  uint32_t glancingBlows;
  double appliedAt;
  double uptime;

  CombatLogBreakdown(std::string name);
};

#endif