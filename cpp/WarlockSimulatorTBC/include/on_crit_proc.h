#pragma once

#include "spell_proc.h"

struct OnCritProc : public SpellProc {
  OnCritProc(Player& player, std::shared_ptr<Aura> aura = nullptr);
  void Setup();
};

struct ImprovedShadowBolt : public OnCritProc {
  ImprovedShadowBolt(Player& player, std::shared_ptr<Aura> aura);
  bool ShouldProc(Spell* spell);
};

struct TheLightningCapacitor : public OnCritProc {
  TheLightningCapacitor(Player& player);
  void StartCast(double predicted_damage = 0);
};

struct ShiffarsNexusHorn : public OnCritProc {
  ShiffarsNexusHorn(Player& player, std::shared_ptr<Aura> aura);
};

struct SextantOfUnstableCurrents : public OnCritProc {
  SextantOfUnstableCurrents(Player& player, std::shared_ptr<Aura> aura);
};