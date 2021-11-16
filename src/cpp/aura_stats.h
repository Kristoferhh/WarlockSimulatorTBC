#ifndef WARLOCK_SIMULATOR_TBC_AURA_STATS
#define WARLOCK_SIMULATOR_TBC_AURA_STATS

struct AuraStats {
  int spell_power;
  int haste_rating;
  int shadow_power;
  int fire_power;
  double haste_percent;
  double mana_cost_modifier;
  int crit_chance;

  AuraStats(int spell_power, int shadow_power, int fire_power, int haste_rating, double haste_percent,
            double mana_cost_modifier, int crit_chance)
      : spell_power(spell_power),
        shadow_power(shadow_power),
        fire_power(fire_power),
        haste_rating(haste_rating),
        haste_percent(haste_percent),
        mana_cost_modifier(mana_cost_modifier),
        crit_chance(crit_chance) {}
};

#endif