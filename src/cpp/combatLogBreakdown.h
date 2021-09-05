#pragma once

#include <iostream>

struct CombatLogBreakdown
{
    std::string name;
    uint32_t casts;
    uint32_t crits;
    uint32_t misses;
    uint32_t manaGain;
    uint32_t damage;
    double appliedAt;
    double uptime;
    uint32_t count; // same as casts but for aura applications

    CombatLogBreakdown(std::string name);
};