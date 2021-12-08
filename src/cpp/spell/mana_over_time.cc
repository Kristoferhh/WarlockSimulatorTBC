#include "mana_over_time.h"

#include "../common.h"
#include "../player/player.h"

ManaOverTime::ManaOverTime(Player& player) : Aura(player) { tick_timer_total = 3; }

void ManaOverTime::Setup() {
  ticks_total = duration / tick_timer_total;
  Aura::Setup();
}

void ManaOverTime::Apply() {
  Aura::Apply();
  tick_timer_remaining = tick_timer_total;
  ticks_remaining = ticks_total;
}

void ManaOverTime::Tick(double t) {
  tick_timer_remaining -= t;

  if (tick_timer_remaining <= 0) {
    const double kCurrentMana = player.stats.mana;

    player.stats.mana = std::min(player.stats.max_mana, player.stats.mana + GetManaGain());
    const double kManaGained = player.stats.mana - kCurrentMana;

    if (player.ShouldWriteToCombatLog()) {
      player.CombatLog("Player gains " + DoubleToString(kManaGained) + " mana from " + name + " (" +
                       DoubleToString(kCurrentMana) + " -> " + DoubleToString(player.stats.mana) + ")" + ")");
    }

    if (player.recording_combat_log_breakdown) {
      player.combat_log_breakdown.at(name)->casts++;
      player.combat_log_breakdown.at(name)->iteration_mana_gain += kManaGained;
    }
    // todo pet

    ticks_remaining--;
    tick_timer_remaining = tick_timer_total;

    if (ticks_remaining <= 0) {
      Aura::Fade();
    }
  }
}

DrumsOfRestorationAura::DrumsOfRestorationAura(Player& player) : ManaOverTime(player) {
  name = SpellName::kDrumsOfRestoration;
  duration = 15;
  group_wide = true;
  Setup();
}

double DrumsOfRestorationAura::GetManaGain() { return 600 / ticks_total; }

ManaTideTotemAura::ManaTideTotemAura(Player& player) : ManaOverTime(player) {
  name = SpellName::kManaTideTotem;
  duration = 12;
  group_wide = true;
  Setup();
}

double ManaTideTotemAura::GetManaGain() { return player.stats.max_mana * 0.06; }