#include "stat.h"

#include "common.h"
#include "player.h"

Stat::Stat(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value)
    : player(player), stat_map(stat_map), entity_type(entity_type), value(value), combat_log_decimal_places(0) {}

void Stat::AddStat() { ModifyStat("add"); }

void Stat::RemoveStat() { ModifyStat("remove"); }

void Stat::ModifyStat(std::string action) {
  const double kCurrentStatValue = stat_map.at(characterStat);
  double new_stat_value = kCurrentStatValue;

  if (calculationType == CalculationType::kAdditive) {
    new_stat_value += value * (action == "remove" ? -1 : 1);
  } else if (calculationType == CalculationType::kMultiplicative) {
    if (action == "add") {
      new_stat_value *= value;
    } else if (action == "remove") {
      new_stat_value /= value;
    }
  }

  stat_map.at(characterStat) = new_stat_value;

  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog((entity_type == EntityType::kPlayer ? "Player "
                      : entity_type == EntityType::kPet  ? "Pet "
                                                         : "") +
                     name + " " +
                     (action == "add" ? (calculationType == CalculationType::kAdditive ? "+" : "*")
                                      : (calculationType == CalculationType::kAdditive ? "-" : "/")) +
                     " " + DoubleToString(value) + " (" + DoubleToString(kCurrentStatValue, combat_log_decimal_places) +
                     " -> " + DoubleToString(new_stat_value, combat_log_decimal_places) + ")");
  }

  if (affects_pet && player.pet != NULL) {
    player.pet->CalculateStatsFromPlayer();
  }
}

SpellPower::SpellPower(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value)
    : Stat(player, stat_map, entity_type, value) {
  name = "Spell Power";
  characterStat = CharacterStat::kSpellPower;
  calculationType = CalculationType::kAdditive;
  affects_pet = entity_type == EntityType::kPlayer;
}

ShadowPower::ShadowPower(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type,
                         double value)
    : Stat(player, stat_map, entity_type, value) {
  name = "Shadow Power";
  characterStat = CharacterStat::kShadowPower;
  calculationType = CalculationType::kAdditive;
  affects_pet = entity_type == EntityType::kPlayer;
}

FirePower::FirePower(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value)
    : Stat(player, stat_map, entity_type, value) {
  name = "Fire Power";
  characterStat = CharacterStat::kFirePower;
  calculationType = CalculationType::kAdditive;
  affects_pet = entity_type == EntityType::kPlayer;
}

SpellHasteRating::SpellHasteRating(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type,
                                   double value)
    : Stat(player, stat_map, entity_type, value) {
  name = "Spell Haste Rating";
  characterStat = CharacterStat::kSpellHasteRating;
  calculationType = CalculationType::kAdditive;
  affects_pet = false;
}

SpellHastePercent::SpellHastePercent(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type,
                                     double value)
    : Stat(player, stat_map, entity_type, value) {
  name = "Spell Haste Percent";
  characterStat = CharacterStat::kSpellHastePercent;
  calculationType = CalculationType::kMultiplicative;
  affects_pet = false;
  combat_log_decimal_places = 4;
}

MeleeHastePercent::MeleeHastePercent(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type,
                                     double value)
    : Stat(player, stat_map, entity_type, value) {
  name = "Melee Haste Percent";
  characterStat = CharacterStat::kMeleeHastePercent;
  calculationType = CalculationType::kMultiplicative;
  combat_log_decimal_places = 4;
}

ManaCostModifier::ManaCostModifier(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type,
                                   double value)
    : Stat(player, stat_map, entity_type, value) {
  name = "Mana Cost Modifier";
  characterStat = CharacterStat::kManaCostModifier;
  calculationType = CalculationType::kMultiplicative;
  affects_pet = false;
  combat_log_decimal_places = 2;
}

SpellCritChance::SpellCritChance(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type,
                                 double value)
    : Stat(player, stat_map, entity_type, value) {
  name = "Spell Crit Chance";
  characterStat = CharacterStat::kSpellCritChance;
  calculationType = CalculationType::kAdditive;
  affects_pet = false;
  combat_log_decimal_places = 2;
}

SpellCritRating::SpellCritRating(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type,
                                 double value)
    : Stat(player, stat_map, entity_type, value) {
  name = "Spell Crit Rating";
  characterStat = CharacterStat::kSpellCritRating;
  calculationType = CalculationType::kAdditive;
  affects_pet = false;
}

AttackPower::AttackPower(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type,
                         double value)
    : Stat(player, stat_map, entity_type, value) {
  name = "Attack Power";
  characterStat = CharacterStat::kAttackPower;
  calculationType = CalculationType::kAdditive;
}