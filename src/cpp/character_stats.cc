#include "character_stats.h"

int CharacterStats::GetStamina() { return stamina * stamina_modifier; }

int CharacterStats::GetIntellect() { return intellect * intellect_modifier; }

int CharacterStats::GetSpirit() { return spirit * spirit_modifier; }