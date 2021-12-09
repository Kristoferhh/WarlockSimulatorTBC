#include "life_tap.h"

#include "../common.h"
#include "../player/player.h"

LifeTap::LifeTap(Player& player) : Spell(player), player(player) {
  name = SpellName::kLifeTap;
  mana_return = 582;
  coefficient = 0.8;
  modifier = 1 * (1 + 0.1 * player.talents.improved_life_tap);
  school = SpellSchool::kShadow;
  Setup();
}

double LifeTap::ManaGain() { return (mana_return + ((player.GetSpellPower(false, school)) * coefficient)) * modifier; }

void LifeTap::Cast() {
  const double kCurrentPlayerMana = player.stats.mana;
  const double kManaGain = this->ManaGain();

  player.stats.mana = std::min(player.stats.max_mana, player.stats.mana + kManaGain);
  const double kManaGained = player.stats.mana - kCurrentPlayerMana;

  if (player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->casts++;
    player.combat_log_breakdown.at(name)->iteration_mana_gain += kManaGained;
  }
  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog(name + " " + DoubleToString(kManaGained) + " (" +
                     DoubleToString(player.GetSpellPower(false, school)) + " Spell Power - " +
                     DoubleToString(coefficient, 3) + " Coefficient - " + DoubleToString(modifier * 100, 2) +
                     "% Modifier)");

    if (kCurrentPlayerMana + kManaGain > player.stats.max_mana) {
      player.CombatLog(name + " used at too high mana (mana wasted)");
    }
  }

  if (player.talents.mana_feed > 0 && player.pet != NULL) {
    const double kCurrentPetMana = player.pet->stats.mana;

    player.pet->stats.mana =
        std::min(kCurrentPetMana + (kManaGain * (player.talents.mana_feed / 3.0)), player.pet->CalculateMaxMana());

    if (player.ShouldWriteToCombatLog()) {
      player.CombatLog(player.pet->name + " gains " + (DoubleToString(player.pet->stats.mana - kCurrentPetMana)) +
                       " mana from Mana Feed");
    }
  }

  if (name == SpellName::kDarkPact) {
    player.pet->stats.mana = std::max(0.0, player.pet->stats.mana - kManaGain);
  }
}

DarkPact::DarkPact(Player& player) : LifeTap(player) {
  name = SpellName::kDarkPact;
  mana_return = 700;
  coefficient = 0.96;
  modifier = 1;
  school = SpellSchool::kShadow;
  Setup();
}

bool DarkPact::Ready() { return Spell::Ready() && player.pet->stats.mana >= ManaGain(); }