#ifndef WARLOCK_SIMULATOR_TBC_MANA_OVER_TIME
#define WARLOCK_SIMULATOR_TBC_MANA_OVER_TIME

#include "../aura.h"

struct ManaOverTime : public Aura {
  ManaOverTime(Player& player);
  void Apply();
  void Tick(double time);
  void Setup();
  virtual int GetManaGain();
};

struct DrumsOfRestorationAura : public ManaOverTime {
  DrumsOfRestorationAura(Player& player);
  int GetManaGain();
};

struct ManaTideTotemAura : public ManaOverTime {
  ManaTideTotemAura(Player& player);
  int GetManaGain();
};

#endif