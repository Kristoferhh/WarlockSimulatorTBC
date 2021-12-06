#ifndef WARLOCK_SIMULATOR_TBC_LIFE_TAP
#define WARLOCK_SIMULATOR_TBC_LIFE_TAP

#include "spell.h"

struct LifeTap : public Spell {
  Player& player;
  int mana_return;

  LifeTap(Player& player);
  double ManaGain();
  void Cast();
};

struct DarkPact : public LifeTap {
  DarkPact(Player& player);
  bool Ready();
};

#endif