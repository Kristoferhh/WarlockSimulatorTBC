#include "../include/on_dot_tick_proc.h"

#include "../include/entity.h"
#include "../include/damage_over_time.h"
#include "../include/player.h"

OnDotTickProc::OnDotTickProc(Player& player, const std::shared_ptr<Aura>& kAura)
  : SpellProc(player, kAura) {
  procs_on_dot_ticks = true;
}

bool OnDotTickProc::ShouldProc(DamageOverTime*) { return true; }

void OnDotTickProc::Setup() {
  SpellProc::Setup();

  if (procs_on_dot_ticks && on_dot_tick_procs_enabled) {
    entity.on_dot_tick_procs.push_back(this);
  }
}

AshtongueTalismanOfShadows::AshtongueTalismanOfShadows(Player& player, const std::shared_ptr<Aura>& kAura)
  : OnDotTickProc(player, kAura) {
  name = SpellName::kAshtongueTalismanOfShadows;
  proc_chance = 20;
  OnDotTickProc::Setup();
}

bool AshtongueTalismanOfShadows::ShouldProc(DamageOverTime* spell) { return spell->name == SpellName::kCorruption; }

TimbalsFocusingCrystal::TimbalsFocusingCrystal(Player& player)
  : OnDotTickProc(player) {
  name = SpellName::kTimbalsFocusingCrystal;
  cooldown = 15;
  proc_chance = 10;
  min_dmg = 285;
  max_dmg = 475;
  does_damage = true;
  spell_school = SpellSchool::kShadow;
  can_crit = true;
  can_miss = true;
  OnDotTickProc::Setup();
}
