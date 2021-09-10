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
    uint32_t dodge;
    uint32_t glancingBlows;

    CombatLogBreakdown(std::string name);
};