#include "../include/on_damage_proc.h"

#include "../include/entity.h"
#include "../include/player.h"
#include "../include/player_settings.h"

OnDamageProc::OnDamageProc(Player& player, const std::shared_ptr<Aura>& kAura) : SpellProc(player, kAura) {
  procs_on_damage = true;
}

void OnDamageProc::Setup() {
  SpellProc::Setup();

  if (procs_on_damage && on_damage_procs_enabled) {
    entity.on_damage_procs.push_back(this);
  }
}

ShatteredSunPendantOfAcumenAldor::ShatteredSunPendantOfAcumenAldor(Player& player, std::shared_ptr<Aura> aura)
  : OnDamageProc(player, aura) {
  name = SpellName::kShatteredSunPendantOfAcumenAldor;
  cooldown = 45;
  proc_chance = 15;
  is_item = true;
  on_damage_procs_enabled = player.settings.exalted_with_shattrath_faction;
  OnDamageProc::Setup();
}

ShatteredSunPendantOfAcumenScryers::ShatteredSunPendantOfAcumenScryers(Player& player)
  : OnDamageProc(player) {
  name = SpellName::kShatteredSunPendantOfAcumenScryers;
  cooldown = 45;
  proc_chance = 15;
  min_dmg = 333;
  max_dmg = 367;
  is_item = true;
  does_damage = true;
  can_crit = true;
  can_miss = true;
  on_damage_procs_enabled = player.settings.exalted_with_shattrath_faction;
  OnDamageProc::Setup();
}
