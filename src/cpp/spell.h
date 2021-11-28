#ifndef WARLOCK_SIMULATOR_TBC_SPELL
#define WARLOCK_SIMULATOR_TBC_SPELL

struct Player;
#include <cmath>
#include <string>

#include "damage_over_time.h"
#include "enums.h"

struct Spell : std::enable_shared_from_this<Spell> {
  Player& player;
  std::shared_ptr<Aura> aura_effect;
  std::shared_ptr<DamageOverTime> dot_effect;
  SpellSchool school;
  SpellType type;
  std::string name;
  int min_dmg;
  int max_dmg;
  int min_mana_gain;
  int max_mana_gain;
  int proc_chance;
  double mana_gain;
  double cast_time;
  double mana_cost;
  double coefficient;
  double cooldown;
  double modifier;
  double bonus_crit_chance;
  double cooldown_remaining;
  double dmg;
  int bonus_damage_from_immolate_min;
  int bonus_damage_from_immolate_max;
  double bonus_damage_from_immolate_average;
  bool does_damage;
  bool can_crit;
  bool is_non_warlock_ability;
  bool casting;
  bool is_item;
  bool on_gcd;
  bool is_proc;
  bool can_miss;
  bool is_finisher;
  bool gain_mana_on_cast;
  bool procs_on_hit;
  bool on_hit_procs_enabled;
  bool procs_on_crit;
  bool on_crit_procs_enabled;
  bool procs_on_dot_ticks;
  bool on_dot_tick_procs_enabled;
  bool procs_on_damage;
  bool on_damage_procs_enabled;
  // For Chipped Power Core
  bool usable_once_per_fight;
  bool has_not_been_cast_this_fight;

  Spell(Player& player, std::shared_ptr<Aura> aura = nullptr, std::shared_ptr<DamageOverTime> dot = nullptr);

  void Reset();
  void Setup();
  bool CanCast();
  bool HasEnoughMana();
  virtual bool Ready();
  virtual double GetCastTime();
  virtual void StartCast(double predicted_damage = 0);
  virtual void Cast();
  virtual double GetModifier();
  virtual void Damage(bool is_crit = false);
  std::vector<double> GetConstantDamage(bool no_rng = true);
  double GetCritMultiplier(double player_crit_multiplier);
  double PredictDamage();
  int GetManaCost();
  void Tick(double time);
  void OnCritProcs();
  void OnDamageProcs();
  void OnHitProcs();
};

struct ShadowBolt : public Spell {
  ShadowBolt(Player& player);
  void StartCast(double predicted_damage);
  double CalculateCastTime();
};

struct Incinerate : public Spell {
  Incinerate(Player& player);
};

struct SearingPain : public Spell {
  SearingPain(Player& player);
};

struct SoulFire : public Spell {
  SoulFire(Player& player);
};

struct Shadowburn : public Spell {
  Shadowburn(Player& player);
};

struct DeathCoil : public Spell {
  DeathCoil(Player& player);
};

struct Shadowfury : public Spell {
  Shadowfury(Player& player);
};

struct SeedOfCorruption : public Spell {
  int aoe_cap;
  SeedOfCorruption(Player& player);
  void Damage(bool is_crit = false);
  double GetModifier();
};

struct Corruption : public Spell {
  Corruption(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct UnstableAffliction : public Spell {
  UnstableAffliction(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct SiphonLife : public Spell {
  SiphonLife(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct Immolate : public Spell {
  Immolate(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
  double GetModifier();
};

struct CurseOfAgony : public Spell {
  CurseOfAgony(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct CurseOfTheElements : public Spell {
  CurseOfTheElements(Player& player, std::shared_ptr<Aura> aura);
};

struct CurseOfRecklessness : public Spell {
  CurseOfRecklessness(Player& player, std::shared_ptr<Aura> aura);
};

struct CurseOfDoom : public Spell {
  CurseOfDoom(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct Conflagrate : public Spell {
  Conflagrate(Player& player);
  void StartCast();
};

struct DestructionPotion : public Spell {
  DestructionPotion(Player& player, std::shared_ptr<Aura> aura);
};

struct FlameCap : public Spell {
  FlameCap(Player& player, std::shared_ptr<Aura> aura);
  void Cast();
};

struct BloodFury : public Spell {
  BloodFury(Player& player, std::shared_ptr<Aura> aura);
};

struct Bloodlust : public Spell {
  Bloodlust(Player& player, std::shared_ptr<Aura> aura);
};

struct DrumsOfBattle : public Spell {
  DrumsOfBattle(Player& player, std::shared_ptr<Aura> aura);
};

struct DrumsOfWar : public Spell {
  DrumsOfWar(Player& player, std::shared_ptr<Aura> aura);
};

struct DrumsOfRestoration : public Spell {
  DrumsOfRestoration(Player& player, std::shared_ptr<Aura> aura);
};

struct AmplifyCurse : public Spell {
  AmplifyCurse(Player& player, std::shared_ptr<Aura> aura);
};

struct PowerInfusion : public Spell {
  PowerInfusion(Player& player, std::shared_ptr<Aura> aura);
};

struct Innervate : public Spell {
  Innervate(Player& player, std::shared_ptr<Aura> aura);
};

struct ChippedPowerCore : public Spell {
  ChippedPowerCore(Player& player, std::shared_ptr<Aura> aura);
  void Cast();
};

struct CrackedPowerCore : public Spell {
  CrackedPowerCore(Player& player, std::shared_ptr<Aura> aura);
  void Cast();
};

struct ManaTideTotem : public Spell {
  ManaTideTotem(Player& player, std::shared_ptr<Aura> aura);
};

#endif