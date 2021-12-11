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
#include "spell/spell.h"

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
  CharacterStats stats;
  EntityType entity_type;
  std::string name;
  std::map<std::string, std::unique_ptr<CombatLogBreakdown>> combat_log_breakdown;
  std::vector<Spell*> spell_list;
  std::vector<Aura*> aura_list;
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
  virtual double GetStamina();
  virtual double GetIntellect();
  double GetSpirit();
};

#endif