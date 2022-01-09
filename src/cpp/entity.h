#pragma once

struct Player;
struct Pet;
struct Simulation;

#include <map>

#include "auras.h"
#include "character_stats.h"
#include "combat_log_breakdown.h"
#include "enums.h"
#include "player/player_settings.h"
#include "player/player_spell/on_crit_proc.h"
#include "player/player_spell/on_damage_proc.h"
#include "player/player_spell/on_dot_tick_proc.h"
#include "player/player_spell/on_hit_proc.h"
#include "player/player_spell/on_resist_proc.h"
#include "spells.h"

struct Entity {
  const int kFloatNumberMultiplier = 1000;  // Multiply doubles such as hit and crit chance with this since we need an
                                            // integer when calling player.rng.range()
  const int kLevel = 70;
  const double kGcdValue = 1.5;
  const double kMinimumGcdValue = 1;
  const double kCritDamageMultiplier = 1.5;
  Simulation* simulation;
  Player* player;
  PlayerSettings& settings;
  Auras auras;
  Spells spells;
  std::shared_ptr<Pet> pet;
  CharacterStats stats;
  EntityType entity_type;
  std::string name;
  std::map<std::string, std::unique_ptr<CombatLogBreakdown>> combat_log_breakdown;
  std::vector<Aura*> aura_list;
  std::vector<Spell*> spell_list;
  std::vector<DamageOverTime*> dot_list;
  std::vector<OnHitProc*> on_hit_procs;
  std::vector<OnCritProc*> on_crit_procs;
  std::vector<OnDotTickProc*> on_dot_tick_procs;
  std::vector<OnDamageProc*> on_damage_procs;
  std::vector<OnResistProc*> on_resist_procs;
  double cast_time_remaining;
  double gcd_remaining;
  double five_second_rule_timer_remaining;
  double mp5_timer_remaining;
  bool recording_combat_log_breakdown;
  bool equipped_item_simulation;
  bool infinite_mana;
  int enemy_shadow_resist;  // TODO move these to an Enemy struct
  int enemy_fire_resist;
  int enemy_level_difference_resistance;

  Entity(Player* player, PlayerSettings& player_settings, EntityType entity_type);
  virtual double GetIntellect();
  virtual double GetStamina();
  virtual double GetHastePercent() = 0;
  virtual double FindTimeUntilNextAction();
  virtual void Tick(double time);
  virtual void EndAuras();
  virtual void Reset();
  virtual void Initialize(Simulation* simulation);
  virtual double GetSpellPower(bool dealing_damage, SpellSchool spell_school) = 0;
  virtual double GetSpellCritChance(SpellType spell_type = SpellType::kNoSpellType) = 0;
  virtual bool IsSpellCrit(SpellType spell_type, double extra_crit = 0);
  virtual bool IsMeleeCrit();
  virtual bool IsSpellHit(SpellType spell_type);
  virtual bool IsMeleeHit();
  double GetPartialResistMultiplier(SpellSchool school);
  double GetSpirit();
  double GetSpellHitChance(SpellType spell_type);
  double GetMeleeCritChance();
  double GetCustomImprovedShadowBoltDamageModifier();
  double GetGcdValue();
  double GetBaseSpellHitChance(int entity_level, int enemy_level);
  void PostIterationDamageAndMana(const std::string& spell_name);
  void SendCombatLogBreakdown();
  void CombatLog(const std::string& entry);
  bool ShouldWriteToCombatLog();
};