#include "on_resist_proc.h"

#include "../player.h"

OnResistProc::OnResistProc(Player& player, std::shared_ptr<Aura> aura) : SpellProc(player, aura) {
  procs_on_resist = true;
}

void OnResistProc::Setup() {
  SpellProc::Setup();

  if (procs_on_resist && on_resist_procs_enabled) {
    player.on_resist_procs.push_back(this);
  }
}

EyeOfMagtheridon::EyeOfMagtheridon(Player& player, std::shared_ptr<Aura> aura) : OnResistProc(player, aura) {
  name = SpellName::kEyeOfMagtheridon;
  proc_chance = 100;
  is_item = true;
  Setup();
}