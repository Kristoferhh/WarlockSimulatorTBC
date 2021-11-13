#include "combat_log_breakdown.h"

#include <iostream>

CombatLogBreakdown::CombatLogBreakdown(std::string name)
    : name(name),
      casts(0),
      crits(0),
      misses(0),
      iteration_mana_gain(0),
      iteration_damage(0),
      dodge(0),
      glancing_blows(0) {}