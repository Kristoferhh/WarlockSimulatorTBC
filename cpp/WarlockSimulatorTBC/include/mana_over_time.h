#pragma once
#include "aura.h"

struct ManaOverTime : Aura {
  explicit ManaOverTime(Entity& entity);
  void Apply() override;
  void Tick(double kTime) override;
  void Setup() override;
  virtual double GetManaGain() = 0;
};

struct DrumsOfRestorationAura final : ManaOverTime {
  explicit DrumsOfRestorationAura(Entity& entity);
  double GetManaGain() override;
};

struct ManaTideTotemAura final : ManaOverTime {
  explicit ManaTideTotemAura(Entity& entity);
  double GetManaGain() override;
};

struct FelEnergyAura final : ManaOverTime {
  explicit FelEnergyAura(Entity& entity);
  double GetManaGain() override;
};
