#ifndef WARLOCK_SIMULATOR_TBC_PLAYER_SPELL
#define WARLOCK_SIMULATOR_TBC_PLAYER_SPELL

struct Player;
#include <cmath>
#include <string>

#include "../../enums.h"
#include "../../spell.h"
#include "damage_over_time.h"

struct PlayerSpell : Spell, std::enable_shared_from_this<PlayerSpell> {
  Player& player;
  std::shared_ptr<DamageOverTime> dot_effect;
  std::vector<std::string> shared_cooldown_spells;
  SpellType type;
  int proc_chance;
  int bonus_damage_from_immolate_min;
  int bonus_damage_from_immolate_max;
  double bonus_damage_from_immolate;
  double bonus_crit_chance;
  bool does_damage;
  bool is_item;
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
  bool procs_on_resist;
  bool on_resist_procs_enabled;

  PlayerSpell(Player& player, std::shared_ptr<Aura> aura = nullptr, std::shared_ptr<DamageOverTime> dot = nullptr);

  void Setup();
  virtual void Cast();
  virtual double GetModifier();
  virtual void Damage(bool is_crit = false);
  std::vector<double> GetConstantDamage();
  double GetCritMultiplier(double player_crit_multiplier);
  double PredictDamage();
  void OnCritProcs();
  void OnResistProcs();
  void OnDamageProcs();
  void OnHitProcs();
};

struct ShadowBolt : public PlayerSpell {
  ShadowBolt(Player& player);
  void StartCast(double predicted_damage);
  double CalculateCastTime();
};

struct Incinerate : public PlayerSpell {
  Incinerate(Player& player);
};

struct SearingPain : public PlayerSpell {
  SearingPain(Player& player);
};

struct SoulFire : public PlayerSpell {
  SoulFire(Player& player);
};

struct Shadowburn : public PlayerSpell {
  Shadowburn(Player& player);
};

struct DeathCoil : public PlayerSpell {
  DeathCoil(Player& player);
};

struct Shadowfury : public PlayerSpell {
  Shadowfury(Player& player);
};

struct SeedOfCorruption : public PlayerSpell {
  int aoe_cap;
  SeedOfCorruption(Player& player);
  void Damage(bool is_crit = false);
  double GetModifier();
};

struct Corruption : public PlayerSpell {
  Corruption(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct UnstableAffliction : public PlayerSpell {
  UnstableAffliction(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct SiphonLife : public PlayerSpell {
  SiphonLife(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct Immolate : public PlayerSpell {
  Immolate(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
  double GetModifier();
};

struct CurseOfAgony : public PlayerSpell {
  CurseOfAgony(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct CurseOfTheElements : public PlayerSpell {
  CurseOfTheElements(Player& player, std::shared_ptr<Aura> aura);
};

struct CurseOfRecklessness : public PlayerSpell {
  CurseOfRecklessness(Player& player, std::shared_ptr<Aura> aura);
};

struct CurseOfDoom : public PlayerSpell {
  CurseOfDoom(Player& player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct Conflagrate : public PlayerSpell {
  Conflagrate(Player& player);
  void Cast();
  bool CanCast();
};

struct DestructionPotion : public PlayerSpell {
  DestructionPotion(Player& player, std::shared_ptr<Aura> aura);
};

struct FlameCap : public PlayerSpell {
  FlameCap(Player& player, std::shared_ptr<Aura> aura);
};

struct BloodFury : public PlayerSpell {
  BloodFury(Player& player, std::shared_ptr<Aura> aura);
};

struct Bloodlust : public PlayerSpell {
  Bloodlust(Player& player, std::shared_ptr<Aura> aura);
};

struct DrumsOfBattle : public PlayerSpell {
  DrumsOfBattle(Player& player, std::shared_ptr<Aura> aura);
};

struct DrumsOfWar : public PlayerSpell {
  DrumsOfWar(Player& player, std::shared_ptr<Aura> aura);
};

struct DrumsOfRestoration : public PlayerSpell {
  DrumsOfRestoration(Player& player, std::shared_ptr<Aura> aura);
};

struct AmplifyCurse : public PlayerSpell {
  AmplifyCurse(Player& player, std::shared_ptr<Aura> aura);
};

struct PowerInfusion : public PlayerSpell {
  PowerInfusion(Player& player, std::shared_ptr<Aura> aura);
};

struct Innervate : public PlayerSpell {
  Innervate(Player& player, std::shared_ptr<Aura> aura);
};

struct ChippedPowerCore : public PlayerSpell {
  ChippedPowerCore(Player& player, std::shared_ptr<Aura> aura);
};

struct CrackedPowerCore : public PlayerSpell {
  CrackedPowerCore(Player& player, std::shared_ptr<Aura> aura);
};

struct ManaTideTotem : public PlayerSpell {
  ManaTideTotem(Player& player, std::shared_ptr<Aura> aura);
};

#endif