#pragma once

struct Entity;

#include "aura.h"
#include "enums.h"
#include "player/player_spell/damage_over_time.h"
#include "spell_cast_result.h"

struct Spell {
  Entity& entity;
  std::shared_ptr<Aura> aura_effect;
  std::shared_ptr<DamageOverTime> dot_effect;
  std::vector<std::string> shared_cooldown_spells;
  SpellSchool spell_school;
  AttackType attack_type;
  SpellType spell_type;
  std::string name;
  double min_dmg;
  double max_dmg;
  double base_damage;
  bool casting;
  bool can_crit;
  bool on_gcd;
  bool is_proc;
  bool limited_amount_of_casts;
  bool is_non_warlock_ability;
  double coefficient;
  double cooldown_remaining;
  double cast_time;
  double modifier;
  double cooldown;
  double mana_cost;
  double min_mana_gain;
  double max_mana_gain;
  double mana_gain;
  int amount_of_casts_per_fight;
  int amount_of_casts_this_fight;
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

  Spell(Entity& entity, std::shared_ptr<Aura> aura = nullptr, std::shared_ptr<DamageOverTime> dot = nullptr);
  virtual void Setup();
  virtual double GetModifier();
  virtual void Cast();
  virtual bool CanCast();
  virtual double GetBaseDamage();
  virtual void Reset();
  virtual double GetCastTime();
  virtual bool Ready();
  void StartCast(double predicted_damage = 0);
  void OnCritProcs();
  void OnResistProcs();
  void OnDamageProcs();
  void OnHitProcs();
  void Tick(double time);
  double GetCritMultiplier(double entity_crit_multiplier);
  double PredictDamage();
  bool HasEnoughMana();

 private:
  virtual double GetCooldown();
  virtual void Damage(bool is_crit = false, bool is_glancing = false);
  double GetManaCost();
  std::vector<double> GetConstantDamage();
  SpellCastResult MagicSpellCast();
  SpellCastResult PhysicalSpellCast();
  void OnSpellHit(SpellCastResult& spell_cast_result);
  void CombatLogDamage(AttackType attack_type, bool is_crit, bool is_glancing, double total_damage, double base_damage,
                       double spell_power, double crit_multiplier, double damage_modifier,
                       double partial_resist_multiplier);
  void ManaGainOnCast();
};

struct ShadowBolt : public Spell {
  ShadowBolt(Entity& entity);
  void StartCast(double predicted_damage);
  double CalculateCastTime();
};

struct Incinerate : public Spell {
  Incinerate(Entity& entity);
};

struct SearingPain : public Spell {
  SearingPain(Entity& entity);
};

struct SoulFire : public Spell {
  SoulFire(Entity& entity);
};

struct Shadowburn : public Spell {
  Shadowburn(Entity& entity);
};

struct DeathCoil : public Spell {
  DeathCoil(Entity& entity);
};

struct Shadowfury : public Spell {
  Shadowfury(Entity& entity);
};

struct SeedOfCorruption : public Spell {
  int aoe_cap;
  SeedOfCorruption(Entity& entity);
  void Damage(bool is_crit = false, bool is_glancing = false);
  double GetModifier();
};

struct Corruption : public Spell {
  Corruption(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct UnstableAffliction : public Spell {
  UnstableAffliction(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct SiphonLife : public Spell {
  SiphonLife(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct Immolate : public Spell {
  Immolate(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
  double GetModifier();
};

struct CurseOfAgony : public Spell {
  CurseOfAgony(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct CurseOfTheElements : public Spell {
  CurseOfTheElements(Entity& entity, std::shared_ptr<Aura> aura);
};

struct CurseOfRecklessness : public Spell {
  CurseOfRecklessness(Entity& entity, std::shared_ptr<Aura> aura);
};

struct CurseOfDoom : public Spell {
  CurseOfDoom(Entity& entity, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct Conflagrate : public Spell {
  Conflagrate(Entity& entity);
  void Cast();
  bool CanCast();
};

struct DestructionPotion : public Spell {
  DestructionPotion(Entity& entity, std::shared_ptr<Aura> aura);
};

struct FlameCap : public Spell {
  FlameCap(Entity& entity, std::shared_ptr<Aura> aura);
};

struct BloodFury : public Spell {
  BloodFury(Entity& entity, std::shared_ptr<Aura> aura);
};

struct Bloodlust : public Spell {
  Bloodlust(Entity& entity, std::shared_ptr<Aura> aura);
};

struct DrumsOfBattle : public Spell {
  DrumsOfBattle(Entity& entity, std::shared_ptr<Aura> aura);
};

struct DrumsOfWar : public Spell {
  DrumsOfWar(Entity& entity, std::shared_ptr<Aura> aura);
};

struct DrumsOfRestoration : public Spell {
  DrumsOfRestoration(Entity& entity, std::shared_ptr<Aura> aura);
};

struct AmplifyCurse : public Spell {
  AmplifyCurse(Entity& entity, std::shared_ptr<Aura> aura);
};

struct PowerInfusion : public Spell {
  PowerInfusion(Entity& entity, std::shared_ptr<Aura> aura);
};

struct Innervate : public Spell {
  Innervate(Entity& entity, std::shared_ptr<Aura> aura);
};

struct ChippedPowerCore : public Spell {
  ChippedPowerCore(Entity& entity, std::shared_ptr<Aura> aura);
};

struct CrackedPowerCore : public Spell {
  CrackedPowerCore(Entity& entity, std::shared_ptr<Aura> aura);
};

struct ManaTideTotem : public Spell {
  ManaTideTotem(Entity& entity, std::shared_ptr<Aura> aura);
};

struct PetMelee : public Spell {
  PetMelee(Entity& entity);
  double GetBaseDamage();
  double GetCooldown();
};

struct ImpFirebolt : public Spell {
  ImpFirebolt(Entity& entity);
};

struct SuccubusLashOfPain : public Spell {
  SuccubusLashOfPain(Entity& entity);
};

struct FelguardCleave : public Spell {
  FelguardCleave(Entity& entity);
  double GetBaseDamage();
};