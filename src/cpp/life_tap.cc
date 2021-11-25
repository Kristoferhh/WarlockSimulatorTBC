#include "life_tap.h"

#include "common.h"
#include "player.h"

LifeTap::LifeTap(Player& player) : Spell(player), player(player) {
  name = "Life Tap";
  mana_return = 582;
  coefficient = 0.8;
  modifier = 1 * (1 + 0.1 * player.talents.improved_life_tap);
  school = SpellSchool::kShadow;
  Setup();
}

int LifeTap::ManaGain() { return (mana_return + ((player.GetSpellPower(school)) * coefficient)) * modifier; }

void LifeTap::Cast() {
  const int kManaGain = this->ManaGain();

  if (player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->casts++;
    player.AddIterationDamageAndMana(name, kManaGain, 0);
  }
  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog(name + " " + DoubleToString(kManaGain) + " (" + DoubleToString(player.GetSpellPower(school)) +
                     " Spell Power - " + DoubleToString(coefficient, 3) + " Coefficient - " +
                     DoubleToString(modifier * 100, 2) + "% Modifier)");

    if (player.stats.at(CharacterStat::kMana) + kManaGain > player.stats.at(CharacterStat::kMaxMana)) {
      player.CombatLog(name + " used at too high mana (mana wasted)");
    }
  }

  player.stats.at(CharacterStat::kMana) =
      std::min(player.stats.at(CharacterStat::kMaxMana), player.stats.at(CharacterStat::kMana) + kManaGain);
  if (player.spells.dark_pact != NULL && name == player.spells.dark_pact->name) {
    player.pet->stats.at(CharacterStat::kMana) = std::max(0.0, player.pet->stats.at(CharacterStat::kMana) - kManaGain);
  }
}

DarkPact::DarkPact(Player& player) : LifeTap(player) {
  name = "Dark Pact";
  mana_return = 700;
  coefficient = 0.96;
  modifier = 1;
  school = SpellSchool::kShadow;
  Setup();
}

bool DarkPact::Ready() { return Spell::Ready() && player.pet->stats.at(CharacterStat::kMana) >= ManaGain(); }