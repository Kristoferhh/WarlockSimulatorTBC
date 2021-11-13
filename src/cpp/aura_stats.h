#ifndef WARLOCK_SIMULATOR_TBC_AURA_STATS
#define WARLOCK_SIMULATOR_TBC_AURA_STATS

struct AuraStats {
  int spell_power;
  int haste_rating;
  int shadow_power;
  int fire_power;
  double haste_percent;
  double mana_cost_modifier;

  AuraStats(int spell_power, int shadow_power, int fire_power, int haste_rating, double haste_percent,
            double mana_cost_modifier);
};

#endif