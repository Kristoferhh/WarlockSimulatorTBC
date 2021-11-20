#include "stat.h"

#include "common.h"
#include "player.h"

Stat::Stat(double value) {
  this->value = value;
  combat_log_decimal_places = 0;
}

void Stat::AddStat(Player& player) { ModifyStat(player, "add"); }

void Stat::RemoveStat(Player& player) { ModifyStat(player, "remove"); }

void Stat::ModifyStat(Player& player, std::string action) {
  const double kCurrentStatValue = player.stats.at(characterStat);
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

  player.stats.at(characterStat) = new_stat_value;

  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog(name + " " +
                     (action == "add" ? (calculationType == CalculationType::kAdditive ? "+" : "*")
                                      : (calculationType == CalculationType::kAdditive ? "-" : "/")) +
                     " " + DoubleToString(value) + " (" + DoubleToString(kCurrentStatValue, combat_log_decimal_places) +
                     " -> " + DoubleToString(new_stat_value, combat_log_decimal_places) + ")");
  }
}

SpellPower::SpellPower(double value) : Stat(value) {
  name = "Spell Power";
  characterStat = CharacterStat::kSpellPower;
  calculationType = CalculationType::kAdditive;
  affects_pet = true;
}

ShadowPower::ShadowPower(double value) : Stat(value) {
  name = "Shadow Power";
  characterStat = CharacterStat::kShadowPower;
  calculationType = CalculationType::kAdditive;
  affects_pet = true;
}

FirePower::FirePower(double value) : Stat(value) {
  name = "Fire Power";
  characterStat = CharacterStat::kFirePower;
  calculationType = CalculationType::kAdditive;
  affects_pet = true;
}

HasteRating::HasteRating(double value) : Stat(value) {
  name = "Haste Rating";
  characterStat = CharacterStat::kHasteRating;
  calculationType = CalculationType::kAdditive;
  affects_pet = false;
}

HastePercent::HastePercent(double value) : Stat(value) {
  name = "Haste Percent";
  characterStat = CharacterStat::kHastePercent;
  calculationType = CalculationType::kMultiplicative;
  affects_pet = false;
  combat_log_decimal_places = 4;
}

ManaCostModifier::ManaCostModifier(double value) : Stat(value) {
  name = "Mana Cost Modifier";
  characterStat = CharacterStat::kManaCostModifier;
  calculationType = CalculationType::kMultiplicative;
  affects_pet = false;
  combat_log_decimal_places = 2;
}

CritChance::CritChance(double value) : Stat(value) {
  name = "Crit Chance";
  characterStat = CharacterStat::kCritChance;
  calculationType = CalculationType::kAdditive;
  affects_pet = false;
  combat_log_decimal_places = 2;
}

CritRating::CritRating(double value) : Stat(value) {
  name = "Crit Rating";
  characterStat = CharacterStat::kCritRating;
  calculationType = CalculationType::kAdditive;
  affects_pet = false;
}