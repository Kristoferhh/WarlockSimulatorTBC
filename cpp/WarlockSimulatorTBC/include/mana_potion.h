#pragma once
#include "spell.h"

struct Player;

struct ManaPotion : Spell {
  explicit ManaPotion(Player& player);
  void Cast() override;
};

struct SuperManaPotion final : ManaPotion {
  explicit SuperManaPotion(Player& player);
};

struct DemonicRune final : ManaPotion {
  explicit DemonicRune(Player& player);
  void Cast() override;
};
