#pragma once

#include "../../spell.h"

struct ManaPotion : public Spell {
  ManaPotion(Player& player);
  void Cast();
};

struct SuperManaPotion : public ManaPotion {
  SuperManaPotion(Player& player);
};

struct DemonicRune : public ManaPotion {
  DemonicRune(Player& player);
  void Cast();
};