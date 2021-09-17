#include "combatLogBreakdown.h"
#include <iostream>

CombatLogBreakdown::CombatLogBreakdown(std::string name)
    : name(name), casts(0), crits(0), misses(0), iterationManaGain(0), iterationDamage(0), dodge(0), glancingBlows(0) {}