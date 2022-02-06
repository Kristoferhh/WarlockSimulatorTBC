#include "../include/life_tap.h"

#include "../include/common.h"
#include "../include/player.h"

LifeTap::LifeTap(Entity& entity) : Spell(entity) {
  name = SpellName::kLifeTap;
  mana_return = 582;
  coefficient = 0.8;
  modifier = 1 * (1 + 0.1 * entity.player->talents.improved_life_tap);
  spell_school = SpellSchool::kShadow;
  Setup();
}

double LifeTap::ManaGain() {
  return (mana_return + ((entity.GetSpellPower(false, spell_school)) * coefficient)) * modifier;
}

void LifeTap::Cast() {
  const double kCurrentPlayerMana = entity.stats.mana;
  const double kManaGain = this->ManaGain();

  entity.stats.mana = std::min(entity.stats.max_mana, entity.stats.mana + kManaGain);
  const double kManaGained = entity.stats.mana - kCurrentPlayerMana;

  if (entity.recording_combat_log_breakdown) {
    entity.combat_log_breakdown.at(name)->casts++;
    entity.combat_log_breakdown.at(name)->iteration_mana_gain += kManaGained;
  }
  if (entity.ShouldWriteToCombatLog()) {
    entity.CombatLog(name + " " + DoubleToString(kManaGained) + " (" +
                     DoubleToString(entity.GetSpellPower(false, spell_school)) + " Spell Power - " +
                     DoubleToString(coefficient, 3) + " Coefficient - " + DoubleToString(modifier * 100, 2) +
                     "% Modifier)");

    if (kCurrentPlayerMana + kManaGain > entity.stats.max_mana) {
      entity.CombatLog(name + " used at too high mana (mana wasted)");
    }
  }

  if (entity.player->talents.mana_feed > 0 && entity.pet != NULL) {
    const double kCurrentPetMana = entity.pet->stats.mana;

    entity.pet->stats.mana = std::min(kCurrentPetMana + (kManaGain * (entity.player->talents.mana_feed / 3.0)),
                                      entity.pet->CalculateMaxMana());

    if (entity.ShouldWriteToCombatLog()) {
      entity.CombatLog(entity.pet->name + " gains " + (DoubleToString(entity.pet->stats.mana - kCurrentPetMana)) +
                       " mana from Mana Feed");
    }
  }

  if (name == SpellName::kDarkPact) {
    entity.pet->stats.mana = std::max(0.0, entity.pet->stats.mana - kManaGain);
  }
}

DarkPact::DarkPact(Entity& entity) : LifeTap(entity) {
  name = SpellName::kDarkPact;
  mana_return = 700;
  coefficient = 0.96;
  modifier = 1;
  spell_school = SpellSchool::kShadow;
  Setup();
}

bool DarkPact::Ready() { return Spell::Ready() && entity.pet->stats.mana >= ManaGain(); }