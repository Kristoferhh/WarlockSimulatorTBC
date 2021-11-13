#include "pet_stats.h"

#include "player.h"

PetStats::PetStats(std::shared_ptr<Player> player)
    : player(player),
      damage_modifier(1),
      stamina_modifier(1),
      intellect_modifier(1),
      strength_modifier(1),
      agility_modifier(1),
      spirit_modifier(1),
      attack_power_modifier(1),
      haste_percent(1) {
  stamina_modifier *= 1 + (0.05 * player->talents->fel_stamina);
  intellect_modifier *= 1 + (0.05 * player->talents->fel_intellect);
  damage_modifier *= (1 + 0.04 * player->talents->unholy_power) * (1 + 0.05 * player->talents->soul_link);
}

int PetStats::GetStamina() { return stamina * stamina_modifier; }

int PetStats::GetIntellect() { return intellect * intellect_modifier; }