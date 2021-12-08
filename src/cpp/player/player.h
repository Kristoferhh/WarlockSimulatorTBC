#ifndef WARLOCK_SIMULATOR_TBC_PLAYER
#define WARLOCK_SIMULATOR_TBC_PLAYER

#include <map>
#include <string>
#include <vector>

#include "../aura.h"
#include "../auras.h"
#include "../character_stats.h"
#include "../combat_log_breakdown.h"
#include "../entity.h"
#include "../items.h"
#include "../pet/pet.h"
#include "../rng.h"
#include "../sets.h"
#include "../spell/damage_over_time.h"
#include "../spell/on_crit_proc.h"
#include "../spell/on_damage_proc.h"
#include "../spell/on_dot_tick_proc.h"
#include "../spell/on_hit_proc.h"
#include "../spell/on_resist_proc.h"
#include "../spell/spell.h"
#include "../talents.h"
#include "../trinket.h"
#include "player_auras.h"
#include "player_settings.h"
#include "player_spells.h"

struct Player : public Entity {
  Auras& selected_auras;
  Talents& talents;
  Sets& sets;
  Items& items;
  PlayerSettings& settings;
  CharacterStats stats;
  PlayerSpells spells;
  PlayerAuras auras;
  std::vector<Trinket> trinkets;
  std::shared_ptr<Spell> filler;
  std::shared_ptr<Spell> curse_spell;
  std::shared_ptr<Aura> curse_aura;
  std::vector<std::string> combat_log_entries;
  std::map<std::string, std::unique_ptr<CombatLogBreakdown>> combat_log_breakdown;
  std::string custom_stat;
  std::vector<Spell*> spell_list;
  std::vector<Aura*> aura_list;
  std::vector<DamageOverTime*> dot_list;
  std::vector<PetSpell*> pet_spell_list;
  std::vector<PetAura*> pet_aura_list;
  std::vector<OnHitProc*> on_hit_procs;
  std::vector<OnCritProc*> on_crit_procs;
  std::vector<OnDotTickProc*> on_dot_tick_procs;
  std::vector<OnDamageProc*> on_damage_procs;
  std::vector<OnResistProc*> on_resist_procs;
  Rng rng;
  double total_duration;
  double fight_time;
  double mp5_timer;
  double demonic_knowledge_spell_power;
  double iteration_damage;
  int iteration;
  int power_infusions_ready;
  bool recording_combat_log_breakdown;

  Player(PlayerSettings& settings);
  void Initialize();
  void Reset();
  void EndAuras();
  void ThrowError(const std::string& error);
  void CastLifeTapOrDarkPact();
  void UseCooldowns(double fight_time_remaining);
  void PostIterationDamageAndMana(const std::string& spell_name);
  void SendCombatLogEntries();
  void CombatLog(const std::string& entry);
  void SendPlayerInfoToCombatLog();
  double GetGcdValue(const std::string& spell_name);
  double GetSpellPower(bool dealing_damage, SpellSchool school = SpellSchool::kNoSchool);
  double GetHastePercent();
  double GetCritChance(SpellType spell_type);
  double GetHitChance(SpellType spell_type);
  double GetPartialResistMultiplier(SpellSchool school);
  double GetBaseHitChance(int player_level, int enemy_level);
  double GetStamina();
  double GetIntellect();
  double GetSpirit();
  int GetRand();
  double GetCustomImprovedShadowBoltDamageModifier();
  void Tick(double time);
  double FindTimeUntilNextAction();
  bool IsCrit(SpellType spell_type, double extra_crit = 0);
  bool IsHit(SpellType spell_type);
  bool ShouldWriteToCombatLog();
  bool RollRng(double chance);
};

#endif
