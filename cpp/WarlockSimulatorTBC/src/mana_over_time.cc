#include "../include/mana_over_time.h"

#include "../include/entity.h"
#include "../include/common.h"
#include "../include/combat_log_breakdown.h"
#include "../include/stat.h"

ManaOverTime::ManaOverTime(Entity& entity)
  : Aura(entity) {}

void ManaOverTime::Setup() {
  ticks_total = duration / tick_timer_total;
  Aura::Setup();
}

void ManaOverTime::Apply() {
  Aura::Apply();
  tick_timer_remaining = tick_timer_total;
  ticks_remaining = ticks_total;
}

void ManaOverTime::Tick(const double kTime) {
  tick_timer_remaining -= kTime;

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
      Fade();
    }
  }
}

DrumsOfRestorationAura::DrumsOfRestorationAura(Entity& entity)
  : ManaOverTime(entity) {
  name = SpellName::kDrumsOfRestoration;
  duration = 15;
  tick_timer_total = 3;
  group_wide = true;
  ManaOverTime::Setup();
}

double DrumsOfRestorationAura::GetManaGain() { return 600.0 / ticks_total; }

ManaTideTotemAura::ManaTideTotemAura(Entity& entity)
  : ManaOverTime(entity) {
  name = SpellName::kManaTideTotem;
  duration = 12;
  tick_timer_total = 3;
  group_wide = true;
  ManaOverTime::Setup();
}

double ManaTideTotemAura::GetManaGain() { return entity.stats.max_mana * 0.06; }

FelEnergyAura::FelEnergyAura(Entity& entity)
  : ManaOverTime(entity) {
  name = SpellName::kFelEnergy;
  duration = 9999;
  tick_timer_total = 4;
  ManaOverTime::Setup();
}

double FelEnergyAura::GetManaGain() { return entity.stats.max_mana * 0.03; }