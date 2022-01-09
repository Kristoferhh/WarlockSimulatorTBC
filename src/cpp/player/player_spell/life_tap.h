#pragma once

#include "../../spell.h"

struct LifeTap : public Spell {
  int mana_return;

  LifeTap(Entity& entity);
  double ManaGain();
  void Cast();
};

struct DarkPact : public LifeTap {
  DarkPact(Entity& entity);
  bool Ready();
};