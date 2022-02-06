#pragma once

struct Entity;

#include "aura.h"
#include "damage_over_time.h"
#include "enums.h"
#include "spell_cast_result.h"

struct Spell {
  Entity& entity;
  std::shared_ptr<Aura> aura_effect;
  std::shared_ptr<DamageOverTime> dot_effect;
  std::vector<std::string> shared_cooldown_spells;
  SpellSchool spell_school = SpellSchool::kNoSchool;
  AttackType attack_type = AttackType::kNoAttackType;
  SpellType spell_type = SpellType::kNoSpellType;
  std::string name;
  int min_dmg = 0;
  int max_dmg = 0;
  double base_damage = 0;
  bool casting = false;
  bool can_crit = false;
  bool on_gcd = true;
  bool is_proc = false;
  bool limited_amount_of_casts = false;
  bool is_non_warlock_ability = false;
  double coefficient = 0;
  double cooldown_remaining = 0;
  double cast_time = 0;
  double cooldown = 0;
  double mana_cost = 0;
  int min_mana_gain = 0;
  int max_mana_gain = 0;
  double mana_gain = 0;
  int amount_of_casts_per_fight = 0;
  int amount_of_casts_this_fight = 0;
  int proc_chance = 0;
  int bonus_damage_from_immolate_min = 0;
  int bonus_damage_from_immolate_max = 0;
  double bonus_damage_from_immolate = 0;
  double bonus_crit_chance = 0;
  bool does_damage = false;
  bool is_item = false;
  bool can_miss = false;
  bool is_finisher = false;
  bool gain_mana_on_cast = false;
  bool procs_on_hit = false;
  bool on_hit_procs_enabled = true;
  bool procs_on_crit = false;
  bool on_crit_procs_enabled = true;
  bool procs_on_dot_ticks = false;
  bool on_dot_tick_procs_enabled = true;
  bool procs_on_damage = false;
  bool on_damage_procs_enabled = true;
  bool procs_on_resist = false;
  bool on_resist_procs_enabled = true;

  Spell(Entity& entity, std::shared_ptr<Aura> aura = nullptr, std::shared_ptr<DamageOverTime> dot = nullptr);
  virtual void Setup();
  virtual void Cast();
  virtual bool CanCast();
  virtual double GetBaseDamage();
  virtual void Reset();
  virtual double GetCastTime();
  virtual bool Ready();
  virtual void StartCast(double predicted_damage = 0);
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
  void CombatLogDamage(bool is_crit, bool is_glancing, double total_damage, double spell_base_damage,
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
  double aoe_cap;
  SeedOfCorruption(Entity& entity);
  void Damage(bool is_crit = false, bool is_glancing = false);
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