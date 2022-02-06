#include "../include/mana_over_time.h"

#include "../include/common.h"
#include "../include/entity.h"
#include "../include/player.h"

ManaOverTime::ManaOverTime(Entity& entity) : Aura(entity) { tick_timer_total = 3; }

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
    const double kCurrentMana = entity.stats.mana;

    entity.stats.mana = std::min(entity.stats.max_mana, entity.stats.mana + GetManaGain());
    const double kManaGained = entity.stats.mana - kCurrentMana;

    if (entity.ShouldWriteToCombatLog()) {
      entity.CombatLog(entity.name + " gains " + DoubleToString(kManaGained) + " mana from " + name + " (" +
                       DoubleToString(kCurrentMana) + " -> " + DoubleToString(entity.stats.mana) + ")" + ")");
    }

    if (entity.recording_combat_log_breakdown) {
      entity.combat_log_breakdown.at(name)->casts++;
      entity.combat_log_breakdown.at(name)->iteration_mana_gain += kManaGained;
    }
    // todo pet

    ticks_remaining--;
    tick_timer_remaining = tick_timer_total;

    if (ticks_remaining <= 0) {
      Aura::Fade();
    }
  }
}

DrumsOfRestorationAura::DrumsOfRestorationAura(Entity& entity) : ManaOverTime(entity) {
  name = SpellName::kDrumsOfRestoration;
  duration = 15;
  group_wide = true;
  Setup();
}

double DrumsOfRestorationAura::GetManaGain() { return 600 / ticks_total; }

ManaTideTotemAura::ManaTideTotemAura(Entity& entity) : ManaOverTime(entity) {
  name = SpellName::kManaTideTotem;
  duration = 12;
  group_wide = true;
  Setup();
}

double ManaTideTotemAura::GetManaGain() { return entity.stats.max_mana * 0.06; }