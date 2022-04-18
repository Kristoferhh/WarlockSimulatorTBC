#include <utility>

#include "../include/on_resist_proc.h"

#include "../include/player.h"

OnResistProc::OnResistProc(Player& player, std::shared_ptr<Aura> aura)
  : SpellProc(player, std::move(aura)) {
  procs_on_resist = true;
}

void OnResistProc::Setup() {
  SpellProc::Setup();

  if (procs_on_resist && on_resist_procs_enabled) {
    entity.on_resist_procs.push_back(this);
  }
}

EyeOfMagtheridon::EyeOfMagtheridon(Player& player, const std::shared_ptr<Aura>& kAura)
  : OnResistProc(player, kAura) {
  name = SpellName::kEyeOfMagtheridon;
  proc_chance = 100;
  is_item = true;
  OnResistProc::Setup();
}
