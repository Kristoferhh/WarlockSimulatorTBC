#pragma once

#include <map>
#include <string>
#include <vector>

#include "../aura.h"
#include "../aura_selection.h"
#include "../character_stats.h"
#include "../combat_log_breakdown.h"
#include "../entity.h"
#include "../items.h"
#include "../pet.h"
#include "../rng.h"
#include "../sets.h"
#include "../talents.h"
#include "../trinket.h"
#include "player_settings.h"
#include "player_spell/damage_over_time.h"
#include "player_spell/on_crit_proc.h"
#include "player_spell/on_damage_proc.h"
#include "player_spell/on_dot_tick_proc.h"
#include "player_spell/on_hit_proc.h"
#include "player_spell/on_resist_proc.h"

struct Player : public Entity {
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

  Player(PlayerSettings& settings);
  void Initialize(Simulation* simulation);
  void Reset();
  void EndAuras();
  void ThrowError(const std::string& error);
  void CastLifeTapOrDarkPact();
  void UseCooldowns(double fight_time_remaining);
  void SendCombatLogEntries();
  void SendPlayerInfoToCombatLog();
  void Tick(double time);
  double GetSpellPower(bool dealing_damage, SpellSchool school = SpellSchool::kNoSchool);
  double GetHastePercent();
  double GetSpellCritChance(SpellType spell_type);
  double FindTimeUntilNextAction();
  int GetRand();
  bool isMeleeCrit();
  bool IsHit(SpellType spell_type);
  bool RollRng(double chance);
};