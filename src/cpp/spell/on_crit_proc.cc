#include "on_crit_proc.h"

#include "../player/player.h"

OnCritProc::OnCritProc(Player& player, std::shared_ptr<Aura> aura) : SpellProc(player, aura) { procs_on_crit = true; }

void OnCritProc::Setup() {
  SpellProc::Setup();

  if (procs_on_crit && on_crit_procs_enabled) {
    player.on_crit_procs.push_back(this);
  }
}

ImprovedShadowBolt::ImprovedShadowBolt(Player& player, std::shared_ptr<Aura> aura) : OnCritProc(player, aura) {
  name = SpellName::kImprovedShadowBolt;
  proc_chance = 100;
  on_crit_procs_enabled = !player.settings.using_custom_isb_uptime && player.talents.improved_shadow_bolt > 0;
  Setup();
}

bool ImprovedShadowBolt::ShouldProc(Spell* spell) { return spell->name == SpellName::kShadowBolt; }

TheLightningCapacitor::TheLightningCapacitor(Player& player) : OnCritProc(player) {
  name = SpellName::kTheLightningCapacitor;
  cooldown = 2.5;
  min_dmg = 694;
  max_dmg = 806;
  proc_chance = 100;
  does_damage = true;
  can_crit = true;
  can_miss = true;
  Setup();
}

void TheLightningCapacitor::StartCast(double predicted_damage) {
  if (cooldown_remaining <= 0) {
    player.auras.the_lightning_capacitor->Apply();
    if (player.auras.the_lightning_capacitor->stacks == player.auras.the_lightning_capacitor->max_stacks) {
      Spell::StartCast();
      cooldown_remaining = cooldown;
      player.auras.the_lightning_capacitor->Fade();
    }
  }
}

ShiffarsNexusHorn::ShiffarsNexusHorn(Player& player, std::shared_ptr<Aura> aura) : OnCritProc(player, aura) {
  name = SpellName::kShiffarsNexusHorn;
  cooldown = 45;
  proc_chance = 20;
  is_item = true;
  Setup();
}

SextantOfUnstableCurrents::SextantOfUnstableCurrents(Player& player, std::shared_ptr<Aura> aura)
    : OnCritProc(player, aura) {
  name = SpellName::kSextantOfUnstableCurrents;
  cooldown = 45;
  proc_chance = 20;
  is_item = true;
  Setup();
}