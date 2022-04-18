#pragma once
#include <memory>
#include <string>
#include <vector>

#include "entity.h"
#include "enums.h"
#include "rng.h"


struct AuraSelection;
struct Simulation;
struct Aura;
struct Spell;
struct Trinket;
struct PlayerSettings;
struct Items;
struct Sets;
struct Talents;

struct Player final : Entity {
  AuraSelection& selected_auras;
  Talents& talents;
  Sets& sets;
  Items& items;
  PlayerSettings& settings;
  std::vector<Trinket> trinkets;
  std::shared_ptr<Spell> filler;
  std::shared_ptr<Spell> curse_spell;
  std::shared_ptr<Aura> curse_aura;
  std::vector<std::string> combat_log_entries;
  std::string custom_stat;
  Rng rng;
  double total_fight_duration;
  double iteration_damage;
  int power_infusions_ready;

  explicit Player(PlayerSettings& settings);
  void Initialize(Simulation* simulation_ptr) override;
  void Reset() override;
  void EndAuras() override;
  void ThrowError(const std::string& kError) const;
  void CastLifeTapOrDarkPact() const;
  void UseCooldowns(double kFightTimeRemaining);
  void SendCombatLogEntries() const;
  void SendPlayerInfoToCombatLog();
  void Tick(double kTime) override;
  double GetSpellPower(bool kDealingDamage, SpellSchool kSchool = SpellSchool::kNoSchool) override;
  double GetHastePercent() override;
  double GetSpellCritChance(SpellType kSpellType) override;
  double FindTimeUntilNextAction() override;
  double GetDamageModifier(Spell& spell, bool kIsDot) override;
  int GetRand();
  bool RollRng(double kChance);
};
