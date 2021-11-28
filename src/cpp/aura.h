#ifndef WARLOCK_SIMULATOR_TBC_AURA
#define WARLOCK_SIMULATOR_TBC_AURA

struct Player;
#include <iostream>
#include <optional>
#include <vector>

#include "stat.h"

struct Aura {
  Player& player;
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
  int minimum_duration;
  int stacks;
  int max_stacks;
  // ISB
  double modifier;
  // Bloodlust
  double haste_modifier;

  Aura(Player& player);
  virtual void Setup();
  virtual void Tick(double time);
  virtual void Apply();
  void Fade();
  virtual void DecrementStacks();  // ISB
};

struct ImprovedShadowBoltAura : public Aura {
  double modifier;

  ImprovedShadowBoltAura(Player& player);
  void Apply();
  void DecrementStacks();
};

struct CurseOfTheElementsAura : public Aura {
  CurseOfTheElementsAura(Player& player);
};

struct CurseOfRecklessnessAura : public Aura {
  CurseOfRecklessnessAura(Player& player);
};

struct ShadowTranceAura : public Aura {
  ShadowTranceAura(Player& player);
};

struct FlameshadowAura : public Aura {
  FlameshadowAura(Player& player);
};

struct ShadowflameAura : public Aura {
  ShadowflameAura(Player& player);
};

struct SpellstrikeAura : public Aura {
  SpellstrikeAura(Player& player);
};

struct PowerInfusionAura : public Aura {
  PowerInfusionAura(Player& player);
};

struct EyeOfMagtheridonAura : public Aura {
  EyeOfMagtheridonAura(Player& player);
};

struct SextantOfUnstableCurrentsAura : public Aura {
  SextantOfUnstableCurrentsAura(Player& player);
};

struct QuagmirransEyeAura : public Aura {
  QuagmirransEyeAura(Player& player);
};

struct ShiffarsNexusHornAura : public Aura {
  ShiffarsNexusHornAura(Player& player);
};

struct ManaEtched4SetAura : public Aura {
  ManaEtched4SetAura(Player& player);
};

struct DestructionPotionAura : public Aura {
  DestructionPotionAura(Player& player);
};

struct FlameCapAura : public Aura {
  FlameCapAura(Player& player);
};

struct BloodFuryAura : public Aura {
  BloodFuryAura(Player& player);
};

struct BloodlustAura : public Aura {
  BloodlustAura(Player& player);
};

struct DrumsOfBattleAura : public Aura {
  DrumsOfBattleAura(Player& player);
};

struct DrumsOfWarAura : public Aura {
  DrumsOfWarAura(Player& player);
};

struct AshtongueTalismanOfShadowsAura : public Aura {
  AshtongueTalismanOfShadowsAura(Player& player);
};

struct DarkmoonCardCrusadeAura : public Aura {
  DarkmoonCardCrusadeAura(Player& player);
};

struct TheLightningCapacitorAura : public Aura {
  TheLightningCapacitorAura(Player& player);
};

struct BandOfTheEternalSageAura : public Aura {
  BandOfTheEternalSageAura(Player& player);
};

struct BladeOfWizardryAura : public Aura {
  BladeOfWizardryAura(Player& player);
};

struct ShatteredSunPendantOfAcumenAldorAura : public Aura {
  ShatteredSunPendantOfAcumenAldorAura(Player& player);
};

struct RobeOfTheElderScribesAura : public Aura {
  RobeOfTheElderScribesAura(Player& player);
};

struct MysticalSkyfireDiamondAura : public Aura {
  MysticalSkyfireDiamondAura(Player& player);
};

struct AmplifyCurseAura : public Aura {
  AmplifyCurseAura(Player& player);
};

struct WrathOfCenariusAura : public Aura {
  WrathOfCenariusAura(Player& player);
};

struct InnervateAura : public Aura {
  InnervateAura(Player& player);
};

struct ChippedPowerCoreAura : public Aura {
  ChippedPowerCoreAura(Player& player);
};

struct CrackedPowerCoreAura : public Aura {
  CrackedPowerCoreAura(Player& player);
};

#endif