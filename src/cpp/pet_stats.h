#ifndef WARLOCK_SIMULATOR_TBC_PET_STATS
#define WARLOCK_SIMULATOR_TBC_PET_STATS

struct Player;
#include <memory>

struct PetStats {
  std::shared_ptr<Player> player;
  int stamina;
  int intellect;
  int strength;
  int agility;
  int max_mana;
  int attack_power;
  int spell_power;
  int mana;
  int mp5;
  int spirit;
  double melee_crit_chance;
  double melee_hit_chance;
  double spell_crit_chance;
  double spell_hit_chance;
  double spell_crit_chance;
  double haste_percent;
  double stamina_modifier;
  double intellect_modifier;
  double strength_modifier;
  double agility_modifier;
  double spirit_modifier;
  double attack_power_modifier;
  double damage_modifier;

  PetStats(std::shared_ptr<Player> player = nullptr);
};

#endif