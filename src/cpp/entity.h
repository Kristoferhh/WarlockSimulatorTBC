#ifndef WARLOCK_SIMULATOR_TBC_ENTITY
#define WARLOCK_SIMULATOR_TBC_ENTITY

struct Player;
struct Pet;

#include "character_stats.h"
#include "enums.h"

struct Entity {
  const int kFloatNumberMultiplier = 1000;  // Multiply doubles such as hit and crit chance with this since we need an
                                            // integer when calling player.rng.range()
  const int kLevel = 70;
  const double kGcdValue = 1.5;
  const double kMinimumGcdValue = 1;
  const double kCritDamageMultiplier = 1.5;
  Player* player;
  std::shared_ptr<Pet> pet;
  CharacterStats stats;
  EntityType entity_type;
  std::string name;
  double cast_time_remaining;
  double gcd_remaining;
  double five_second_rule_timer_remaining;
  double spirit_tick_timer_remaining;

  Entity(Player* player, EntityType entity_type);
};

#endif