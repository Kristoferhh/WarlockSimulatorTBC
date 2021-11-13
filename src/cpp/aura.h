#ifndef WARLOCK_SIMULATOR_TBC_AURA
#define WARLOCK_SIMULATOR_TBC_AURA

struct Player;
#include <iostream>

#include "aura_stats.h"

struct Aura {
  std::shared_ptr<Player> player;
  std::unique_ptr<AuraStats> stats;
  std::string name;
  int duration;
  double duration_remaining;
  int proc_chance;
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
  // The Lightning Capacitor & ISB
  int stacks;
  int max_stacks;
  // ISB
  double modifier;
  double uptime_so_far;
  // Bloodlust
  double haste_modifier;

  Aura(std::shared_ptr<Player> player);
  void Setup();
  virtual void Tick(double time);
  virtual void Apply();
  virtual void Fade();
  virtual void DecrementStacks();  // ISB
};

struct ImprovedShadowBoltAura : public Aura {
  double modifier;

  ImprovedShadowBoltAura(std::shared_ptr<Player> player);
  void Apply();
  void Fade();
  void DecrementStacks();
};

struct CurseOfTheElementsAura : public Aura {
  CurseOfTheElementsAura(std::shared_ptr<Player> player);
};

struct CurseOfRecklessnessAura : public Aura {
  CurseOfRecklessnessAura(std::shared_ptr<Player> player);
};

struct ShadowTranceAura : public Aura {
  ShadowTranceAura(std::shared_ptr<Player> player);
};

struct FlameshadowAura : public Aura {
  FlameshadowAura(std::shared_ptr<Player> player);
};

struct ShadowflameAura : public Aura {
  ShadowflameAura(std::shared_ptr<Player> player);
};

struct SpellstrikeAura : public Aura {
  SpellstrikeAura(std::shared_ptr<Player> player);
};

struct PowerInfusionAura : public Aura {
  PowerInfusionAura(std::shared_ptr<Player> player);
};

struct EyeOfMagtheridonAura : public Aura {
  EyeOfMagtheridonAura(std::shared_ptr<Player> player);
};

struct SextantOfUnstableCurrentsAura : public Aura {
  SextantOfUnstableCurrentsAura(std::shared_ptr<Player> player);
};

struct QuagmirransEyeAura : public Aura {
  QuagmirransEyeAura(std::shared_ptr<Player> player);
};

struct ShiffarsNexusHornAura : public Aura {
  ShiffarsNexusHornAura(std::shared_ptr<Player> player);
};

struct ManaEtched4SetAura : public Aura {
  ManaEtched4SetAura(std::shared_ptr<Player> player);
};

struct DestructionPotionAura : public Aura {
  DestructionPotionAura(std::shared_ptr<Player> player);
  void Apply();
  void Fade();
};

struct FlameCapAura : public Aura {
  FlameCapAura(std::shared_ptr<Player> player);
};

struct BloodFuryAura : public Aura {
  BloodFuryAura(std::shared_ptr<Player> player);
};

struct BloodlustAura : public Aura {
  BloodlustAura(std::shared_ptr<Player> player);
};

struct DrumsOfBattleAura : public Aura {
  DrumsOfBattleAura(std::shared_ptr<Player> player);
};

struct DrumsOfWarAura : public Aura {
  DrumsOfWarAura(std::shared_ptr<Player> player);
};

struct DrumsOfRestorationAura : public Aura {
  int manaGain;

  DrumsOfRestorationAura(std::shared_ptr<Player> player);
  void Apply();
  void Tick(double time);
};

struct AshtongueTalismanOfShadowsAura : public Aura {
  AshtongueTalismanOfShadowsAura(std::shared_ptr<Player> player);
};

struct DarkmoonCardCrusadeAura : public Aura {
  int stacks;
  int max_stacks;
  int spell_power_per_stack;

  DarkmoonCardCrusadeAura(std::shared_ptr<Player> player);
  void Apply();
  void Fade();
};

struct TheLightningCapacitorAura : public Aura {
  TheLightningCapacitorAura(std::shared_ptr<Player> player);
  void Apply();
  void Fade();
};

struct BandOfTheEternalSageAura : public Aura {
  BandOfTheEternalSageAura(std::shared_ptr<Player> player);
};

struct BladeOfWizardryAura : public Aura {
  BladeOfWizardryAura(std::shared_ptr<Player> player);
};

struct ShatteredSunPendantOfAcumenAura : public Aura {
  ShatteredSunPendantOfAcumenAura(std::shared_ptr<Player> player);
};

struct RobeOfTheElderScribesAura : public Aura {
  RobeOfTheElderScribesAura(std::shared_ptr<Player> player);
};

struct MysticalSkyfireDiamondAura : public Aura {
  MysticalSkyfireDiamondAura(std::shared_ptr<Player> player);
};

struct AmplifyCurseAura : public Aura {
  AmplifyCurseAura(std::shared_ptr<Player> player);
};

struct WrathOfCenariusAura : public Aura {
  WrathOfCenariusAura(std::shared_ptr<Player> player);
};

struct InnervateAura : public Aura {
  InnervateAura(std::shared_ptr<Player> player);
};

struct ChippedPowerCoreAura : public Aura {
  ChippedPowerCoreAura(std::shared_ptr<Player> player);
};

struct CrackedPowerCoreAura : public Aura {
  CrackedPowerCoreAura(std::shared_ptr<Player> player);
};

struct ManaTideTotemAura : public Aura {
  ManaTideTotemAura(std::shared_ptr<Player> player);
  void Tick(double time);
  void Apply();
};

#endif