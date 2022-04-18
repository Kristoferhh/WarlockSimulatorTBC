#pragma once
#include "spell_proc.h"

struct Player;

struct OnResistProc : SpellProc {
  explicit OnResistProc(Player& player, std::shared_ptr<Aura> aura = nullptr);
  void Setup() override;
};

struct EyeOfMagtheridon : OnResistProc {
  EyeOfMagtheridon(Player& player, const std::shared_ptr<Aura>& kAura);
};
