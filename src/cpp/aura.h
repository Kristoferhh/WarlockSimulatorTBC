#ifndef WARLOCK_SIMULATOR_TBC_AURA
#define WARLOCK_SIMULATOR_TBC_AURA

struct Entity;
#include <iostream>
#include <optional>
#include <vector>

#include "stat.h"

struct Aura {
  Entity& entity;
  std::vector<Stat> stats;
  std::vector<Stat> stats_per_stack;
  std::string name;
  int duration;
  double duration_remaining;
  bool active;
  bool has_duration;
  bool group_wide;  // true if it's an aura that applies to everyone in the group
                    // (will apply to pets as well then)
  // dots
  int tick_timer_total;
  double tick_timer_remaining;
  int ticks_remaining;
  int ticks_total;
  int stacks;
  int max_stacks;
  // ISB
  double modifier;
  // Bloodlust
  double haste_modifier;

  Aura(Entity& entity);
  virtual void Setup();
  virtual void Tick(double time);
  virtual void Apply();
  void Fade();
  virtual void DecrementStacks();  // ISB
};

struct ImprovedShadowBoltAura : public Aura {
  double modifier;

  ImprovedShadowBoltAura(Entity& entity);
  void Apply();
  void DecrementStacks();
};

struct CurseOfTheElementsAura : public Aura {
  CurseOfTheElementsAura(Entity& entity);
};

struct CurseOfRecklessnessAura : public Aura {
  CurseOfRecklessnessAura(Entity& entity);
};

struct ShadowTranceAura : public Aura {
  ShadowTranceAura(Entity& entity);
};

struct FlameshadowAura : public Aura {
  FlameshadowAura(Entity& entity);
};

struct ShadowflameAura : public Aura {
  ShadowflameAura(Entity& entity);
};

struct SpellstrikeAura : public Aura {
  SpellstrikeAura(Entity& entity);
};

struct PowerInfusionAura : public Aura {
  PowerInfusionAura(Entity& entity);
};

struct EyeOfMagtheridonAura : public Aura {
  EyeOfMagtheridonAura(Entity& entity);
};

struct SextantOfUnstableCurrentsAura : public Aura {
  SextantOfUnstableCurrentsAura(Entity& entity);
};

struct QuagmirransEyeAura : public Aura {
  QuagmirransEyeAura(Entity& entity);
};

struct ShiffarsNexusHornAura : public Aura {
  ShiffarsNexusHornAura(Entity& entity);
};

struct ManaEtched4SetAura : public Aura {
  ManaEtched4SetAura(Entity& entity);
};

struct DestructionPotionAura : public Aura {
  DestructionPotionAura(Entity& entity);
};

struct FlameCapAura : public Aura {
  FlameCapAura(Entity& entity);
};

struct BloodFuryAura : public Aura {
  BloodFuryAura(Entity& entity);
};

struct BloodlustAura : public Aura {
  BloodlustAura(Entity& entity);
};

struct DrumsOfBattleAura : public Aura {
  DrumsOfBattleAura(Entity& entity);
};

struct DrumsOfWarAura : public Aura {
  DrumsOfWarAura(Entity& entity);
};

struct AshtongueTalismanOfShadowsAura : public Aura {
  AshtongueTalismanOfShadowsAura(Entity& entity);
};

struct DarkmoonCardCrusadeAura : public Aura {
  DarkmoonCardCrusadeAura(Entity& entity);
};

struct TheLightningCapacitorAura : public Aura {
  TheLightningCapacitorAura(Entity& entity);
};

struct BandOfTheEternalSageAura : public Aura {
  BandOfTheEternalSageAura(Entity& entity);
};

struct BladeOfWizardryAura : public Aura {
  BladeOfWizardryAura(Entity& entity);
};

struct ShatteredSunPendantOfAcumenAldorAura : public Aura {
  ShatteredSunPendantOfAcumenAldorAura(Entity& entity);
};

struct RobeOfTheElderScribesAura : public Aura {
  RobeOfTheElderScribesAura(Entity& entity);
};

struct MysticalSkyfireDiamondAura : public Aura {
  MysticalSkyfireDiamondAura(Entity& entity);
};

struct AmplifyCurseAura : public Aura {
  AmplifyCurseAura(Entity& entity);
};

struct WrathOfCenariusAura : public Aura {
  WrathOfCenariusAura(Entity& entity);
};

struct InnervateAura : public Aura {
  InnervateAura(Entity& entity);
};

struct ChippedPowerCoreAura : public Aura {
  ChippedPowerCoreAura(Entity& entity);
};

struct CrackedPowerCoreAura : public Aura {
  CrackedPowerCoreAura(Entity& entity);
};

struct AirmansRibbonOfGallantryAura : public Aura {
  AirmansRibbonOfGallantryAura(Entity& entity);
};

struct DemonicFrenzy : public Aura {
  DemonicFrenzy(Entity& entity);
};

struct BlackBook : public Aura {
  BlackBook(Entity& entity);
};

struct BattleSquawk : public Aura {
  BattleSquawk(Entity& entity);
};

#endif