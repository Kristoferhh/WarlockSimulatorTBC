#pragma once

#include "spell_proc.h"

struct OnHitProc : public SpellProc {
  OnHitProc(Entity& entity, std::shared_ptr<Aura> aura = nullptr);
  void Setup();
};

struct MarkOfDefiance : public OnHitProc {
  MarkOfDefiance(Entity& entity);
};

struct BladeOfWizardry : public OnHitProc {
  BladeOfWizardry(Entity& entity, std::shared_ptr<Aura> aura);
};

struct InsightfulEarthstormDiamond : public OnHitProc {
  InsightfulEarthstormDiamond(Entity& entity);
};

struct RobeOfTheElderScribes : public OnHitProc {
  RobeOfTheElderScribes(Entity& entity, std::shared_ptr<Aura> aura);
};

struct QuagmirransEye : public OnHitProc {
  QuagmirransEye(Entity& entity, std::shared_ptr<Aura> aura);
};

struct BandOfTheEternalSage : public OnHitProc {
  BandOfTheEternalSage(Entity& entity, std::shared_ptr<Aura> aura);
};

struct MysticalSkyfireDiamond : public OnHitProc {
  MysticalSkyfireDiamond(Entity& entity, std::shared_ptr<Aura> aura);
};

struct JudgementOfWisdom : public OnHitProc {
  JudgementOfWisdom(Entity& entity);
};

struct Flameshadow : public OnHitProc {
  Flameshadow(Entity& entity, std::shared_ptr<Aura> aura);
  bool ShouldProc(Spell* spell);
};

struct Shadowflame : public OnHitProc {
  Shadowflame(Entity& entity, std::shared_ptr<Aura> aura);
  bool ShouldProc(Spell* spell);
};

struct Spellstrike : public OnHitProc {
  Spellstrike(Entity& entity, std::shared_ptr<Aura> aura);
};

struct ManaEtched4Set : public OnHitProc {
  ManaEtched4Set(Entity& entity, std::shared_ptr<Aura> aura);
};

struct WrathOfCenarius : public OnHitProc {
  WrathOfCenarius(Entity& entity, std::shared_ptr<Aura> aura);
};

struct DarkmoonCardCrusade : public OnHitProc {
  DarkmoonCardCrusade(Entity& entity, std::shared_ptr<Aura> aura);
};

struct DemonicFrenzy : public OnHitProc {
  DemonicFrenzy(Entity& entity, std::shared_ptr<Aura> aura);
};