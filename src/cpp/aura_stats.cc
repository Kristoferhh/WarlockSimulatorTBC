#include "aura_stats.h"

AuraStats::AuraStats(int spell_power, int shadow_power, int fire_power, int haste_rating, double haste_percent,
                     double mana_cost_modifier)
    : spell_power(spell_power),
      shadow_power(shadow_power),
      fire_power(fire_power),
      haste_rating(haste_rating),
      haste_percent(haste_percent),
      mana_cost_modifier(mana_cost_modifier) {}