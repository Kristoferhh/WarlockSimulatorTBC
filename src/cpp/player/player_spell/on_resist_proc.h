#pragma once

#include "spell_proc.h"

struct OnResistProc : public SpellProc {
  OnResistProc(Player& player, std::shared_ptr<Aura> aura = nullptr);
  void Setup();
};

struct EyeOfMagtheridon : public OnResistProc {
  EyeOfMagtheridon(Player& player, std::shared_ptr<Aura> aura);
};