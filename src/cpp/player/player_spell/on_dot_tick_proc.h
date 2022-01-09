#pragma once

#include "spell_proc.h"

struct OnDotTickProc : public SpellProc {
  OnDotTickProc(Player& player, std::shared_ptr<Aura> aura = nullptr);
  void Setup();
  virtual bool ShouldProc(DamageOverTime* spell);
};

struct AshtongueTalismanOfShadows : public OnDotTickProc {
  AshtongueTalismanOfShadows(Player& player, std::shared_ptr<Aura> aura);
  bool ShouldProc(DamageOverTime* spell);
};

struct TimbalsFocusingCrystal : public OnDotTickProc {
  TimbalsFocusingCrystal(Player& player);
};