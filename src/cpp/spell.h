#ifndef WARLOCK_SIMULATOR_TBC_SPELL
#define WARLOCK_SIMULATOR_TBC_SPELL

struct Player;
#include <cmath>
#include <string>

#include "damage_over_time.h"
#include "enums.h"

struct Spell : std::enable_shared_from_this<Spell> {
  std::shared_ptr<Player> player;
  std::shared_ptr<Aura> aura_effect;
  std::shared_ptr<DamageOverTime> dot_effect;
  SpellSchool school;
  SpellType type;
  int min_dmg;
  int max_dmg;
  int min_mana;
  int max_mana;
  int proc_chance;
  double mana_gain;
  double cast_time;
  double mana_cost;
  double coefficient;
  double mana_return;
  double cooldown;
  double modifier;
  double bonus_crit;
  double cooldown_remaining;
  double dmg;
  double average_mana_value;
  int bonus_damage_from_immolate_min;
  int bonus_damage_from_immolate_max;
  double bonus_damage_from_immolate_average;
  bool is_dot;
  bool does_damage;
  bool can_crit;
  bool is_non_warlock_ability;
  bool casting;
  bool is_item;
  bool is_aura;
  bool on_gcd;
  bool is_proc;
  bool is_finisher;
  std::string name;
  // For Chipped Power Core
  bool usable_once_per_fight;
  bool has_not_been_cast_this_fight;

  Spell(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura = nullptr,
        std::shared_ptr<DamageOverTime> dot = nullptr);

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
  void Tick(double time);
  void OnCritProcs();
  void OnDamageProcs();
  void OnHitProcs();
};

struct LifeTap : public Spell {
  LifeTap(std::shared_ptr<Player> player);
  int ManaGain();
  void Cast();
};

struct ShadowBolt : public Spell {
  ShadowBolt(std::shared_ptr<Player> player);
  void StartCast(double predicted_damage);
  double CalculateCastTime();
};

struct Incinerate : public Spell {
  Incinerate(std::shared_ptr<Player> player);
};

struct SearingPain : public Spell {
  SearingPain(std::shared_ptr<Player> player);
};

struct SoulFire : public Spell {
  SoulFire(std::shared_ptr<Player> player);
};

struct Shadowburn : public Spell {
  Shadowburn(std::shared_ptr<Player> player);
};

struct DeathCoil : public Spell {
  DeathCoil(std::shared_ptr<Player> player);
};

struct Shadowfury : public Spell {
  Shadowfury(std::shared_ptr<Player> player);
};

struct SeedOfCorruption : public Spell {
  int aoe_cap;
  SeedOfCorruption(std::shared_ptr<Player> player);
  void Damage(bool is_crit = false);
  double GetModifier();
};

struct DarkPact : public Spell {
  DarkPact(std::shared_ptr<Player> player);
  void Cast();
  bool Ready();
  double ManaGain();
};

struct Corruption : public Spell {
  Corruption(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura,
             std::shared_ptr<DamageOverTime> dot);
};

struct UnstableAffliction : public Spell {
  UnstableAffliction(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura,
                     std::shared_ptr<DamageOverTime> dot);
};

struct SiphonLife : public Spell {
  SiphonLife(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura,
             std::shared_ptr<DamageOverTime> dot);
};

struct Immolate : public Spell {
  Immolate(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura,
           std::shared_ptr<DamageOverTime> dot);
  double GetModifier();
};

struct CurseOfAgony : public Spell {
  CurseOfAgony(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura,
               std::shared_ptr<DamageOverTime> dot);
};

struct CurseOfTheElements : public Spell {
  CurseOfTheElements(std::shared_ptr<Player> player,
                     std::shared_ptr<Aura> aura);
};

struct CurseOfRecklessness : public Spell {
  CurseOfRecklessness(std::shared_ptr<Player> player,
                      std::shared_ptr<Aura> aura);
};

struct CurseOfDoom : public Spell {
  CurseOfDoom(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura,
              std::shared_ptr<DamageOverTime> dot);
};

struct Conflagrate : public Spell {
  Conflagrate(std::shared_ptr<Player> player);
  void StartCast();
};

struct DestructionPotion : public Spell {
  DestructionPotion(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct SuperManaPotion : public Spell {
  SuperManaPotion(std::shared_ptr<Player> player);
  void Cast();
};

struct DemonicRune : public Spell {
  DemonicRune(std::shared_ptr<Player> player);
  void Cast();
};

struct FlameCap : public Spell {
  FlameCap(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
  void Cast();
};

struct BloodFury : public Spell {
  BloodFury(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct Bloodlust : public Spell {
  Bloodlust(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct DrumsOfBattle : public Spell {
  DrumsOfBattle(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
  bool Ready();
};

struct DrumsOfWar : public Spell {
  DrumsOfWar(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
  bool Ready();
};

struct DrumsOfRestoration : public Spell {
  DrumsOfRestoration(std::shared_ptr<Player> player,
                     std::shared_ptr<Aura> aura);
  bool Ready();
};

struct TimbalsFocusingCrystal : public Spell {
  TimbalsFocusingCrystal(std::shared_ptr<Player> player);
};

struct MarkOfDefiance : public Spell {
  MarkOfDefiance(std::shared_ptr<Player> player);
  void Cast();
};

struct TheLightningCapacitor : public Spell {
  TheLightningCapacitor(std::shared_ptr<Player> player,
                        std::shared_ptr<Aura> aura);
  void StartCast(double predicted_damage = 0);
};

struct BladeOfWizardry : public Spell {
  BladeOfWizardry(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct ShatteredSunPendantOfAcumen : public Spell {
  ShatteredSunPendantOfAcumen(std::shared_ptr<Player> player,
                              std::shared_ptr<Aura> aura);
};

struct RobeOfTheElderScribes : public Spell {
  RobeOfTheElderScribes(std::shared_ptr<Player> player,
                        std::shared_ptr<Aura> aura);
};

struct QuagmirransEye : public Spell {
  QuagmirransEye(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct ShiffarsNexusHorn : public Spell {
  ShiffarsNexusHorn(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct SextantOfUnstableCurrents : public Spell {
  SextantOfUnstableCurrents(std::shared_ptr<Player> player,
                            std::shared_ptr<Aura> aura);
};

struct BandOfTheEternalSage : public Spell {
  BandOfTheEternalSage(std::shared_ptr<Player> player,
                       std::shared_ptr<Aura> aura);
};

struct MysticalSkyfireDiamond : public Spell {
  MysticalSkyfireDiamond(std::shared_ptr<Player> player,
                         std::shared_ptr<Aura> aura);
};

struct InsightfulEarthstormDiamond : public Spell {
  InsightfulEarthstormDiamond(std::shared_ptr<Player> player);
  void Cast();
};

struct AmplifyCurse : public Spell {
  AmplifyCurse(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct PowerInfusion : public Spell {
  PowerInfusion(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct Innervate : public Spell {
  Innervate(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct ChippedPowerCore : public Spell {
  ChippedPowerCore(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
  void Cast();
};

struct CrackedPowerCore : public Spell {
  CrackedPowerCore(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
  void Cast();
};

struct ManaTideTotem : public Spell {
  ManaTideTotem(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

#endif