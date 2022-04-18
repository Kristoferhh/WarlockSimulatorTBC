#pragma once
#include <memory>
#include <string>
#include <vector>

#include "spell_cast_result.h"

enum class SpellType;
enum class AttackType;
enum class SpellSchool;
struct DamageOverTime;
struct Aura;
struct Entity;

struct Spell {
  virtual ~Spell() = default;
  Entity& entity;
  std::shared_ptr<Aura> aura_effect;
  std::shared_ptr<DamageOverTime> dot_effect;
  std::vector<std::string> shared_cooldown_spells;
  SpellSchool spell_school;
  AttackType attack_type;
  SpellType spell_type;
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

  explicit Spell(Entity& entity_param, std::shared_ptr<Aura> aura = nullptr,
                 std::shared_ptr<DamageOverTime> dot = nullptr);
  virtual void Setup();
  virtual void Cast();
  virtual bool CanCast();
  virtual double GetBaseDamage();
  virtual void Reset();
  virtual double GetCastTime();
  virtual bool Ready();
  virtual void StartCast(double kPredictedDamage = 0);
  void OnCritProcs();
  void OnResistProcs();
  void OnDamageProcs();
  void OnHitProcs();
  void Tick(double kTime);
  [[nodiscard]] double GetCritMultiplier(double kEntityCritMultiplier) const;
  double PredictDamage();
  [[nodiscard]] bool HasEnoughMana() const;

private:
  virtual double GetCooldown();
  virtual void Damage(bool kIsCrit = false, bool kIsGlancing = false);
  [[nodiscard]] double GetManaCost() const;
  std::vector<double> GetConstantDamage();
  SpellCastResult MagicSpellCast();
  [[nodiscard]] SpellCastResult PhysicalSpellCast() const;
  void OnSpellHit(const SpellCastResult& kSpellCastResult);
  void CombatLogDamage(bool kIsCrit, bool kIsGlancing, double kTotalDamage, double kSpellBaseDamage,
                       double kSpellPower, double kCritMultiplier, double kDamageModifier,
                       double kPartialResistMultiplier) const;
  void ManaGainOnCast() const;
};

struct ShadowBolt final : Spell {
  explicit ShadowBolt(Entity& entity_param);
  void StartCast(double predicted_damage) override;
  [[nodiscard]] double CalculateCastTime() const;
};

struct Incinerate final : Spell {
  explicit Incinerate(Entity& entity_param);
};

struct SearingPain final : Spell {
  explicit SearingPain(Entity& entity_param);
};

struct SoulFire final : Spell {
  explicit SoulFire(Entity& entity_param);
};

struct Shadowburn final : Spell {
  explicit Shadowburn(Entity& entity_param);
};

struct DeathCoil final : Spell {
  explicit DeathCoil(Entity& entity_param);
};

struct Shadowfury final : Spell {
  explicit Shadowfury(Entity& entity_param);
};

struct SeedOfCorruption final : Spell {
  explicit SeedOfCorruption(Entity& entity_param);
  double aoe_cap;
  void Damage(bool is_crit = false, bool is_glancing = false) override;
};

struct Corruption final : Spell {
  explicit Corruption(Entity& entity_param, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct UnstableAffliction final : Spell {
  explicit UnstableAffliction(Entity& entity_param, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct SiphonLife final : Spell {
  explicit SiphonLife(Entity& entity_param, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct Immolate final : Spell {
  explicit Immolate(Entity& entity_param, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct CurseOfAgony final : Spell {
  explicit CurseOfAgony(Entity& entity_param, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct CurseOfTheElements final : Spell {
  explicit CurseOfTheElements(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct CurseOfRecklessness final : Spell {
  explicit CurseOfRecklessness(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct CurseOfDoom final : Spell {
  explicit CurseOfDoom(Entity& entity_param, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct Conflagrate final : Spell {
  explicit Conflagrate(Entity& entity_param);
  void Cast() override;
  bool CanCast() override;
};

struct DestructionPotion final : Spell {
  explicit DestructionPotion(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct FlameCap final : Spell {
  explicit FlameCap(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct BloodFury final : Spell {
  explicit BloodFury(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct Bloodlust final : Spell {
  explicit Bloodlust(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct DrumsOfBattle final : Spell {
  explicit DrumsOfBattle(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct DrumsOfWar final : Spell {
  explicit DrumsOfWar(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct DrumsOfRestoration final : Spell {
  explicit DrumsOfRestoration(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct AmplifyCurse final : Spell {
  explicit AmplifyCurse(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct PowerInfusion final : Spell {
  explicit PowerInfusion(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct Innervate final : Spell {
  explicit Innervate(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct ChippedPowerCore final : Spell {
  explicit ChippedPowerCore(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct CrackedPowerCore final : Spell {
  explicit CrackedPowerCore(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct ManaTideTotem final : Spell {
  explicit ManaTideTotem(Entity& entity_param, std::shared_ptr<Aura> aura);
};

struct PetMelee final : Spell {
  explicit PetMelee(Entity& entity_param);
  double GetBaseDamage() override;
  double GetCooldown() override;
};

struct ImpFirebolt final : Spell {
  explicit ImpFirebolt(Entity& entity_param);
};

struct SuccubusLashOfPain final : Spell {
  explicit SuccubusLashOfPain(Entity& entity_param);
};

struct FelguardCleave final : Spell {
  explicit FelguardCleave(Entity& entity_param);
  double GetBaseDamage() override;
};
