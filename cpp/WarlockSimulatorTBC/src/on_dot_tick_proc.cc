#include "../include/on_dot_tick_proc.h"

#include "../include/player.h"

OnDotTickProc::OnDotTickProc(Player& player, std::shared_ptr<Aura> aura) : SpellProc(player, aura) {
  procs_on_dot_ticks = true;
}

bool OnDotTickProc::ShouldProc(DamageOverTime*) { return true; }

void OnDotTickProc::Setup() {
  SpellProc::Setup();

  if (procs_on_dot_ticks && on_dot_tick_procs_enabled) {
    entity.on_dot_tick_procs.push_back(this);
  }
}

AshtongueTalismanOfShadows::AshtongueTalismanOfShadows(Player& player, std::shared_ptr<Aura> aura)
    : OnDotTickProc(player, aura) {
  name = SpellName::kAshtongueTalismanOfShadows;
  proc_chance = 20;
  Setup();
}

bool AshtongueTalismanOfShadows::ShouldProc(DamageOverTime* spell) { return spell->name == SpellName::kCorruption; }

TimbalsFocusingCrystal::TimbalsFocusingCrystal(Player& player) : OnDotTickProc(player) {
  name = SpellName::kTimbalsFocusingCrystal;
  cooldown = 15;
  proc_chance = 10;
  min_dmg = 285;
  max_dmg = 475;
  does_damage = true;
  spell_school = SpellSchool::kShadow;
  can_crit = true;
  can_miss = true;
  Setup();
}