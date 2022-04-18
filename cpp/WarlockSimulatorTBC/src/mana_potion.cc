#include "../include/mana_potion.h"

#include "../include/player.h"
#include "../include/player_settings.h"
#include "../include/common.h"
#include "../include/combat_log_breakdown.h"

ManaPotion::ManaPotion(Player& player)
  : Spell(player) {
  cooldown = 120;
  is_item = true;
  on_gcd = false;
}

void ManaPotion::Cast() {
  Spell::Cast();
  const double kCurrentPlayerMana = entity.stats.mana;
  const double kManaGain = entity.player->settings.randomize_values && min_mana_gain > 0 && max_mana_gain > 0
                             ? entity.player->rng.Range(min_mana_gain, max_mana_gain)
                             : mana_gain;

  entity.stats.mana = std::min(entity.stats.max_mana, kCurrentPlayerMana + kManaGain);
  const double kManaGained = entity.stats.mana - kCurrentPlayerMana;

  if (entity.recording_combat_log_breakdown) {
    entity.combat_log_breakdown.at(name)->iteration_mana_gain += kManaGained;
  }

  if (entity.ShouldWriteToCombatLog()) {
    entity.CombatLog("Player gains " + DoubleToString(kManaGained) + " mana from " + name + " (" +
                     DoubleToString(round(kCurrentPlayerMana)) + " -> " + DoubleToString(round(entity.stats.mana)) +
                     ")");
  }
}

SuperManaPotion::SuperManaPotion(Player& player)
  : ManaPotion(player) {
  name = SpellName::kSuperManaPotion;
  min_mana_gain = 1800;
  max_mana_gain = 3000;
  Spell::Setup();
}

DemonicRune::DemonicRune(Player& player)
  : ManaPotion(player) {
  name = SpellName::kDemonicRune;
  min_mana_gain = 900;
  max_mana_gain = 1500;
  Spell::Setup();
}

void DemonicRune::Cast() {
  ManaPotion::Cast();
  if (entity.player->spells.chipped_power_core != nullptr) {
    entity.player->spells.chipped_power_core->cooldown_remaining = cooldown;
  }
  if (entity.player->spells.cracked_power_core != nullptr) {
    entity.player->spells.cracked_power_core->cooldown_remaining = cooldown;
  }
}