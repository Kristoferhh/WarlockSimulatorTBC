#pragma once
#include <memory>
#include <string>
#include <vector>

#include "auras.h"
#include "character_stats.h"
#include "enums.h"
#include "spells.h"

struct DamageOverTime;
struct Spell;
struct Aura;
enum class SpellSchool;
struct OnResistProc;
struct OnDamageProc;
struct OnDotTickProc;
struct OnCritProc;
struct OnHitProc;
struct CombatLogBreakdown;
enum class EntityType;
struct PlayerSettings;
struct Player;
struct Pet;
struct Simulation;

#include <map>

struct Entity {
  virtual ~Entity() = default;
  const int kFloatNumberMultiplier = 1000; // Multiply doubles such as hit and crit chance with this since we need an
  // integer when calling player.rng.range()
  const int kLevel = 70;
  const double kGcdValue = 1.5;
  const double kMinimumGcdValue = 1;
  const double kCritDamageMultiplier = 1.5;
  Simulation* simulation;
  Player* player;
  PlayerSettings& settings;
  Auras auras = Auras();
  Spells spells = Spells();
  std::shared_ptr<Pet> pet;
  CharacterStats stats;
  EntityType entity_type;
  std::string name;
  std::map<std::string, std::shared_ptr<CombatLogBreakdown>> combat_log_breakdown;
  std::vector<Aura*> aura_list;
  std::vector<Spell*> spell_list;
  std::vector<DamageOverTime*> dot_list;
  std::vector<OnHitProc*> on_hit_procs;
  std::vector<OnCritProc*> on_crit_procs;
  std::vector<OnDotTickProc*> on_dot_tick_procs;
  std::vector<OnDamageProc*> on_damage_procs;
  std::vector<OnResistProc*> on_resist_procs;
  double cast_time_remaining = 0;
  double gcd_remaining = 0;
  double five_second_rule_timer_remaining = 5;
  double mp5_timer_remaining = 5;
  bool recording_combat_log_breakdown;
  bool equipped_item_simulation;
  bool infinite_mana;
  int enemy_shadow_resist; // TODO move these to an Enemy struct
  int enemy_fire_resist;
  int enemy_level_difference_resistance;

  Entity(Player* player, PlayerSettings& player_settings, EntityType entity_type);
  virtual double GetIntellect();
  virtual double GetStamina();
  virtual double GetHastePercent() = 0;
  virtual double FindTimeUntilNextAction();
  virtual void Tick(double kTime);
  virtual void EndAuras();
  virtual void Reset();
  virtual void Initialize(Simulation* simulation_ptr);
  virtual double GetSpellPower(bool dealing_damage, SpellSchool spell_school) = 0;
  virtual double GetSpellCritChance(SpellType spell_type = SpellType::kNoSpellType) = 0;
  virtual double GetDamageModifier(Spell& spell, bool is_dot) = 0;
  virtual bool IsSpellCrit(SpellType kSpellType, double kExtraCrit = 0);
  virtual bool IsMeleeCrit();
  virtual bool IsSpellHit(SpellType kSpellType);
  virtual bool IsMeleeHit();
  [[nodiscard]] double GetMultiplicativeDamageModifier(const Spell& kSpell, bool is_dot) const;
  [[nodiscard]] double GetPartialResistMultiplier(SpellSchool kSchool) const;
  [[nodiscard]] double GetSpirit() const;
  [[nodiscard]] double GetSpellHitChance(SpellType kSpellType) const;
  [[nodiscard]] double GetMeleeCritChance() const;
  [[nodiscard]] double GetCustomImprovedShadowBoltDamageModifier() const;
  double GetGcdValue();
  [[nodiscard]] double GetBaseSpellHitChance(int kEntityLevel, int kEnemyLevel) const;
  void SendCombatLogBreakdown() const;
  void CombatLog(const std::string& kEntry) const;
  [[nodiscard]] bool ShouldWriteToCombatLog() const;
  void PostIterationDamageAndMana(const std::string& kSpellName) const;
};
