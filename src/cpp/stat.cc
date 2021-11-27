#include "stat.h"

#include "common.h"
#include "player.h"

Stat::Stat(Player& player, double& character_stat, EntityType entity_type, double value)
    : player(player),
      character_stat(character_stat),
      entity_type(entity_type),
      value(value),
      combat_log_decimal_places(0) {}

void Stat::AddStat() { ModifyStat("add"); }

void Stat::RemoveStat() { ModifyStat("remove"); }

void Stat::ModifyStat(std::string action) {
  const double kCurrentStatValue = character_stat;
  double new_stat_value = kCurrentStatValue;

  if (calculation_type == CalculationType::kAdditive) {
    new_stat_value += value * (action == "remove" ? -1 : action == "add" ? 1 : 0);
  } else if (calculation_type == CalculationType::kMultiplicative) {
    if (action == "add") {
      new_stat_value *= value;
    } else if (action == "remove") {
      new_stat_value /= value;
    }
  }

  character_stat = new_stat_value;

  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog((entity_type == EntityType::kPlayer ? "Player "
                      : entity_type == EntityType::kPet  ? "Pet "
                                                         : "") +
                     name + " " +
                     (action == "add" ? (calculation_type == CalculationType::kAdditive ? "+" : "*")
                                      : (calculation_type == CalculationType::kAdditive ? "-" : "/")) +
                     " " + DoubleToString(value) + " (" + DoubleToString(kCurrentStatValue, combat_log_decimal_places) +
                     " -> " + DoubleToString(new_stat_value, combat_log_decimal_places) + ")");
  }

  if (affects_pet && entity_type == EntityType::kPlayer && player.pet != NULL) {
    player.pet->CalculateStatsFromPlayer();
  }
}

SpellPower::SpellPower(Player& player, double& character_stat, EntityType entity_type, double value)
    : Stat(player, character_stat, entity_type, value) {
  name = "Spell Power";
  calculation_type = CalculationType::kAdditive;
  affects_pet = true;
}

ShadowPower::ShadowPower(Player& player, double& character_stat, EntityType entity_type, double value)
    : Stat(player, character_stat, entity_type, value) {
  name = "Shadow Power";
  calculation_type = CalculationType::kAdditive;
  affects_pet = true;
}

FirePower::FirePower(Player& player, double& character_stat, EntityType entity_type, double value)
    : Stat(player, character_stat, entity_type, value) {
  name = "Fire Power";
  calculation_type = CalculationType::kAdditive;
  affects_pet = true;
}

SpellHasteRating::SpellHasteRating(Player& player, double& character_stat, EntityType entity_type, double value)
    : Stat(player, character_stat, entity_type, value) {
  name = "Spell Haste Rating";
  calculation_type = CalculationType::kAdditive;
  affects_pet = false;
}

SpellHastePercent::SpellHastePercent(Player& player, double& character_stat, EntityType entity_type, double value)
    : Stat(player, character_stat, entity_type, value) {
  name = "Spell Haste Percent";
  calculation_type = CalculationType::kMultiplicative;
  affects_pet = false;
  combat_log_decimal_places = 4;
}

MeleeHastePercent::MeleeHastePercent(Player& player, double& character_stat, EntityType entity_type, double value)
    : Stat(player, character_stat, entity_type, value) {
  name = "Melee Haste Percent";
  calculation_type = CalculationType::kMultiplicative;
  combat_log_decimal_places = 4;
}

ManaCostModifier::ManaCostModifier(Player& player, double& character_stat, EntityType entity_type, double value)
    : Stat(player, character_stat, entity_type, value) {
  name = "Mana Cost Modifier";
  calculation_type = CalculationType::kMultiplicative;
  affects_pet = false;
  combat_log_decimal_places = 2;
}

SpellCritChance::SpellCritChance(Player& player, double& character_stat, EntityType entity_type, double value)
    : Stat(player, character_stat, entity_type, value) {
  name = "Spell Crit Chance";
  calculation_type = CalculationType::kAdditive;
  affects_pet = false;
  combat_log_decimal_places = 2;
}

SpellCritRating::SpellCritRating(Player& player, double& character_stat, EntityType entity_type, double value)
    : Stat(player, character_stat, entity_type, value) {
  name = "Spell Crit Rating";
  calculation_type = CalculationType::kAdditive;
  affects_pet = false;
}

AttackPower::AttackPower(Player& player, double& character_stat, EntityType entity_type, double value)
    : Stat(player, character_stat, entity_type, value) {
  name = "Attack Power";
  calculation_type = CalculationType::kAdditive;
}