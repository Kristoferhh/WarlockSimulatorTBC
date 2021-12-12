#ifndef WARLOCK_SIMULATOR_TBC_MANA_POTION
#define WARLOCK_SIMULATOR_TBC_MANA_POTION

#include "player_spell.h"

struct ManaPotion : public PlayerSpell {
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

#endif