#pragma once
#include "spell_proc.h"

struct Player;

struct OnDotTickProc : SpellProc {
  explicit OnDotTickProc(Player& player, const std::shared_ptr<Aura>& kAura = nullptr);
  void Setup() override;
  virtual bool ShouldProc(DamageOverTime* spell);
};

struct AshtongueTalismanOfShadows final : OnDotTickProc {
  AshtongueTalismanOfShadows(Player& player, const std::shared_ptr<Aura>& kAura);
  bool ShouldProc(DamageOverTime* spell) override;
};

struct TimbalsFocusingCrystal final : OnDotTickProc {
  explicit TimbalsFocusingCrystal(Player& player);
};
