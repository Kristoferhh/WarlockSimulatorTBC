#pragma once

#include "spell_proc.h"

struct OnDamageProc : public SpellProc {
  OnDamageProc(Player& player, std::shared_ptr<Aura> aura = nullptr);
  void Setup();
};

struct ShatteredSunPendantOfAcumenAldor : public OnDamageProc {
  ShatteredSunPendantOfAcumenAldor(Player& player, std::shared_ptr<Aura> aura);
};

struct ShatteredSunPendantOfAcumenScryers : public OnDamageProc {
  ShatteredSunPendantOfAcumenScryers(Player& player);
};