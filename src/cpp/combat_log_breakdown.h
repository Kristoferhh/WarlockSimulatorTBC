#pragma once

#include <iostream>
#include <vector>

struct CombatLogBreakdown {
  std::string name;
  uint32_t casts;
  uint32_t crits;
  uint32_t misses;
  double iteration_mana_gain;
  double iteration_damage;
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