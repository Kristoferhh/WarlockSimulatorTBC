#include "mana_potion.h"

#include "../common.h"
#include "../player/player.h"

ManaPotion::ManaPotion(Player& player) : Spell(player) {
  cooldown = 120;
  is_item = true;
  on_gcd = false;
}

void ManaPotion::Cast() {
  Spell::Cast();
  const double kCurrentPlayerMana = player.stats.mana;
  // todo check for the randomize values option

  player.stats.mana =
      std::min(player.stats.max_mana, kCurrentPlayerMana + player.rng.range(min_mana_gain, max_mana_gain));
  const double kManaGained = player.stats.mana - kCurrentPlayerMana;

  if (player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->iteration_mana_gain += kManaGained;
  }

  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog("Player gains " + DoubleToString(kManaGained) + " mana from " + name + " (" +
                     DoubleToString(round(kCurrentPlayerMana)) + " -> " + DoubleToString(round(player.stats.mana)) +
                     ")");
  }
}

SuperManaPotion::SuperManaPotion(Player& player) : ManaPotion(player) {
  name = SpellName::kSuperManaPotion;
  min_mana_gain = 1800;
  max_mana_gain = 3000;
  Setup();
}

DemonicRune::DemonicRune(Player& player) : ManaPotion(player) {
  name = SpellName::kDemonicRune;
  min_mana_gain = 900;
  max_mana_gain = 1500;
  Setup();
}

void DemonicRune::Cast() {
  ManaPotion::Cast();
  if (player.spells.chipped_power_core != NULL) {
    player.spells.chipped_power_core->cooldown_remaining = cooldown;
  }
  if (player.spells.cracked_power_core != NULL) {
    player.spells.cracked_power_core->cooldown_remaining = cooldown;
  }
}