#ifndef WARLOCK_SIMULATOR_TBC_ENTITY
#define WARLOCK_SIMULATOR_TBC_ENTITY

struct Player;
struct Pet;
struct Simulation;

#include <map>

#include "character_stats.h"
#include "combat_log_breakdown.h"
#include "enums.h"
#include "player/player_settings.h"

struct Entity {
  const int kFloatNumberMultiplier = 1000;  // Multiply doubles such as hit and crit chance with this since we need an
                                            // integer when calling player.rng.range()
  const int kLevel = 70;
  const double kGcdValue = 1.5;
  const double kMinimumGcdValue = 1;
  const double kCritDamageMultiplier = 1.5;
  Simulation* simulation;
  Player* player;
  std::shared_ptr<Pet> pet;
  // base_stats, buff_stats, and debuff_stats are only used by Pet
  // but needs to be in this class for now because of the Stat class
  CharacterStats base_stats;
  CharacterStats buff_stats;  // Certain stats from buffs need to be separated from the main
                              // stat so we can re-calculate the pet's stats in
                              // calculateStatsFromPlayer().
  CharacterStats debuff_stats;
  CharacterStats stats;
  EntityType entity_type;
  std::string name;
  std::map<std::string, std::unique_ptr<CombatLogBreakdown>> combat_log_breakdown;
  double cast_time_remaining;
  double gcd_remaining;
  double five_second_rule_timer_remaining;
  double spirit_tick_timer_remaining;
  bool recording_combat_log_breakdown;
  bool equipped_item_simulation;

  Entity(Player* player, PlayerSettings& player_settings, EntityType entity_type);
  bool ShouldWriteToCombatLog();
  void PostIterationDamageAndMana(const std::string& spell_name);
  void SendCombatLogBreakdown();
  double GetStamina();
  double GetIntellect();
};

#endif