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
#include "player/player_spell/player_spell.h"

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
  std::vector<Aura*> aura_list;
  std::vector<Spell*> spell_list;
  std::vector<DamageOverTime*> dot_list;
  double cast_time_remaining;
  double gcd_remaining;
  double five_second_rule_timer_remaining;
  double mp5_timer_remaining;
  bool recording_combat_log_breakdown;
  bool equipped_item_simulation;
  bool infinite_mana;

  Entity(Player* player, PlayerSettings& player_settings, EntityType entity_type);
  bool ShouldWriteToCombatLog();
  void PostIterationDamageAndMana(const std::string& spell_name);
  void SendCombatLogBreakdown();
  virtual double GetStamina();
  virtual double GetIntellect();
  double GetSpirit();
  virtual void Tick(double time);
  virtual double FindTimeUntilNextAction();
  void CombatLog(const std::string& entry);
  virtual void EndAuras();
  virtual void Reset();
  virtual void Initialize(Simulation* simulation);
  virtual double GetHastePercent() = 0;
  double GetGcdValue();
};

#endif