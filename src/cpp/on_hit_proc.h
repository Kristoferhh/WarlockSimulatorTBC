#ifndef WARLOCK_SIMULATOR_TBC_ON_HIT_PROC
#define WARLOCK_SIMULATOR_TBC_ON_HIT_PROC

#include "spell_proc.h"

struct OnHitProc : public SpellProc {
  OnHitProc(Player& player, std::shared_ptr<Aura> aura = nullptr);
  void Setup();
};

struct MarkOfDefiance : public OnHitProc {
  MarkOfDefiance(Player& player);
};

struct BladeOfWizardry : public OnHitProc {
  BladeOfWizardry(Player& player, std::shared_ptr<Aura> aura);
};

struct InsightfulEarthstormDiamond : public OnHitProc {
  InsightfulEarthstormDiamond(Player& player);
};

struct RobeOfTheElderScribes : public OnHitProc {
  RobeOfTheElderScribes(Player& player, std::shared_ptr<Aura> aura);
};

struct QuagmirransEye : public OnHitProc {
  QuagmirransEye(Player& player, std::shared_ptr<Aura> aura);
};

struct BandOfTheEternalSage : public OnHitProc {
  BandOfTheEternalSage(Player& player, std::shared_ptr<Aura> aura);
};

struct MysticalSkyfireDiamond : public OnHitProc {
  MysticalSkyfireDiamond(Player& player, std::shared_ptr<Aura> aura);
};

struct JudgementOfWisdom : public OnHitProc {
  JudgementOfWisdom(Player& player);
};

struct Flameshadow : public OnHitProc {
  Flameshadow(Player& player, std::shared_ptr<Aura> aura);
  bool ShouldProc(Spell* spell);
};

struct Shadowflame : public OnHitProc {
  Shadowflame(Player& player, std::shared_ptr<Aura> aura);
  bool ShouldProc(Spell* spell);
};

struct Spellstrike : public OnHitProc {
  Spellstrike(Player& player, std::shared_ptr<Aura> aura);
};

struct ManaEtched4Set : public OnHitProc {
  ManaEtched4Set(Player& player, std::shared_ptr<Aura> aura);
};

struct WrathOfCenarius : public OnHitProc {
  WrathOfCenarius(Player& player, std::shared_ptr<Aura> aura);
};

struct DarkmoonCardCrusade : public OnHitProc {
  DarkmoonCardCrusade(Player& player, std::shared_ptr<Aura> aura);
};

#endif