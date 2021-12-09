#ifndef WARLOCK_SIMULATOR_TBC_MANA_OVER_TIME
#define WARLOCK_SIMULATOR_TBC_MANA_OVER_TIME

#include "../aura.h"

struct ManaOverTime : public Aura {
  ManaOverTime(Entity& Entity);
  void Apply();
  void Tick(double time);
  void Setup();
  virtual double GetManaGain();
};

struct DrumsOfRestorationAura : public ManaOverTime {
  DrumsOfRestorationAura(Entity& Entity);
  double GetManaGain();
};

struct ManaTideTotemAura : public ManaOverTime {
  ManaTideTotemAura(Entity& Entity);
  double GetManaGain();
};

#endif