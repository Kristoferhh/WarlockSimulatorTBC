#pragma once
#include "spell_proc.h"

struct OnHitProc : SpellProc {
  explicit OnHitProc(Entity& entity, std::shared_ptr<Aura> aura = nullptr);
  void Setup() override;
};

struct MarkOfDefiance final : OnHitProc {
  explicit MarkOfDefiance(Entity& entity);
};

struct BladeOfWizardry final : OnHitProc {
  explicit BladeOfWizardry(Entity& entity, std::shared_ptr<Aura> aura);
};

struct InsightfulEarthstormDiamond final : OnHitProc {
  explicit InsightfulEarthstormDiamond(Entity& entity);
};

struct RobeOfTheElderScribes final : OnHitProc {
  explicit RobeOfTheElderScribes(Entity& entity, std::shared_ptr<Aura> aura);
};

struct QuagmirransEye final : OnHitProc {
  explicit QuagmirransEye(Entity& entity, std::shared_ptr<Aura> aura);
};

struct BandOfTheEternalSage final : OnHitProc {
  explicit BandOfTheEternalSage(Entity& entity, std::shared_ptr<Aura> aura);
};

struct MysticalSkyfireDiamond final : OnHitProc {
  explicit MysticalSkyfireDiamond(Entity& entity, std::shared_ptr<Aura> aura);
};

struct JudgementOfWisdom final : OnHitProc {
  explicit JudgementOfWisdom(Entity& entity);
};

struct Flameshadow final : OnHitProc {
  explicit Flameshadow(Entity& entity, std::shared_ptr<Aura> aura);
  bool ShouldProc(Spell* spell) override;
};

struct Shadowflame final : OnHitProc {
  explicit Shadowflame(Entity& entity, std::shared_ptr<Aura> aura);
  bool ShouldProc(Spell* spell) override;
};

struct Spellstrike final : OnHitProc {
  explicit Spellstrike(Entity& entity, std::shared_ptr<Aura> aura);
};

struct ManaEtched4Set final : OnHitProc {
  explicit ManaEtched4Set(Entity& entity, std::shared_ptr<Aura> aura);
};

struct WrathOfCenarius final : OnHitProc {
  explicit WrathOfCenarius(Entity& entity, std::shared_ptr<Aura> aura);
};

struct DarkmoonCardCrusade final : OnHitProc {
  explicit DarkmoonCardCrusade(Entity& entity, std::shared_ptr<Aura> aura);
};

struct DemonicFrenzy final : OnHitProc {
  explicit DemonicFrenzy(Entity& entity, std::shared_ptr<Aura> aura);
};
