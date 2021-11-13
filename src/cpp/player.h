#ifndef WARLOCK_SIMULATOR_TBC_PLAYER
#define WARLOCK_SIMULATOR_TBC_PLAYER

#include <map>
#include <random>
#include <string>
#include <vector>

#include "aura.h"
#include "auras.h"
#include "combat_log_breakdown.h"
#include "damage_over_time.h"
#include "items.h"
#include "pet.h"
#include "player_auras.h"
#include "player_settings.h"
#include "player_spells.h"
#include "sets.h"
#include "spell.h"
#include "talents.h"
#include "trinket.h"

struct Player : std::enable_shared_from_this<Player> {
  const double kHitRatingPerPercent = 12.62;
  const double kCritRatingPerPercent = 22.08;
  const double kHasteRatingPerPercent = 15.77;
  const double kManaPerIntellect = 15;
  const double kHealthPerStamina = 10;
  const double kCritChancePerIntellect = 1 / 81.95;
  const double kBaseCritChancePercent = 1.701;
  const double kGcdValue = 1.5;
  const double kMinimumGcdValue = 1;
  const double kCritDamageMultiplier = 1.5;
  const int kLevel = 70;
  const int kFloatNumberMultiplier = 1000;  // Multiply hit and crit percent by this number to get rid of the
                                            // decimals when calling Random() since we need integers
  const double kSpellDelay = 0.0001;        // Increases the Cast time of spells by a very small
                                            // amount to e.g. make it so that if Immolate has 1.5
                                            // seconds remaining, the sim can start precasting
                                            // Immolate and it won't refresh before it expires.
  std::shared_ptr<Pet> pet;
  std::shared_ptr<Auras> selected_auras;
  std::shared_ptr<Talents> talents;
  std::shared_ptr<Sets> sets;
  std::shared_ptr<CharacterStats> stats;
  std::shared_ptr<Items> items;
  std::shared_ptr<PlayerSettings> settings;
  std::unique_ptr<PlayerSpells> spells;
  std::unique_ptr<PlayerAuras> auras;
  std::vector<std::unique_ptr<Trinket>> trinkets;
  std::shared_ptr<Spell> filler;
  std::shared_ptr<Spell> curse_spell;
  std::shared_ptr<Aura> curse_aura;
  std::vector<std::string> combat_log_entries;
  std::map<std::string, std::unique_ptr<CombatLogBreakdown>> combat_log_breakdown;
  uint32_t iteration_damage;
  std::mt19937 gen;
  std::uniform_int_distribution<> random_num{1, 100 * kFloatNumberMultiplier};
  std::string customStat;
  double cast_time_remaining;
  double gcd_remaining;
  double total_duration;
  double fight_time;
  int iteration;
  double mp5_timer;
  double five_second_rule_timer;
  double demonic_knowledge_spell_power;

  Player(std::shared_ptr<PlayerSettings> settings);
  void Initialize();
  void Reset();
  void EndAuras();
  void ThrowError(const std::string& error);
  double GetGcdValue(const std::shared_ptr<Spell>& spell);
  double GetSpellPower(SpellSchool school = SpellSchool::kNoSchool);
  double GetHastePercent();
  bool IsCrit(SpellType spell_type, double extra_crit = 0);
  bool IsHit(SpellType spell_type);
  double GetCritChance(SpellType spell_type);
  double GetHitChance(SpellType spell_type);
  double GetPartialResistMultiplier(SpellSchool school);
  double GetBaseHitChance(int player_level, int enemy_level);
  void UseCooldowns();
  int GetRand();
  void CastLifeTapOrDarkPact();
  bool ShouldWriteToCombatLog();
  void AddIterationDamageAndMana(const std::string& spell_name, int mana_gain, int damage);
  void PostIterationDamageAndMana(const std::string& spell_name);
  void SendCombatLogEntries();
  void CombatLog(const std::string& entry);
  void SendPlayerInfoToCombatLog();
};

#endif
