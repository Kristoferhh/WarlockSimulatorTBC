#pragma once
#include "spell_proc.h"

struct Player;

struct OnCritProc : SpellProc {
  explicit OnCritProc(Player& player, std::shared_ptr<Aura> aura = nullptr);
  void Setup() override;
};

struct ImprovedShadowBolt final : OnCritProc {
  ImprovedShadowBolt(Player& player, std::shared_ptr<Aura> aura);
  bool ShouldProc(Spell* spell) override;
};

struct TheLightningCapacitor final : OnCritProc {
  explicit TheLightningCapacitor(Player& player);
  void StartCast(double predicted_damage = 0) override;
};

struct ShiffarsNexusHorn final : OnCritProc {
  ShiffarsNexusHorn(Player& player, std::shared_ptr<Aura> aura);
};

struct SextantOfUnstableCurrents final : OnCritProc {
  SextantOfUnstableCurrents(Player& player, std::shared_ptr<Aura> aura);
};
