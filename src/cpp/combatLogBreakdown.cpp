#include "combatLogBreakdown.h"
#include <iostream>

CombatLogBreakdown::CombatLogBreakdown(std::string name)
    : name(name), casts(0), crits(0), misses(0), manaGain(0), damage(0) {}