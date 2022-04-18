#include "../include/stat.h"

#include "../include/common.h"
#include "../include/entity.h"

Stat::Stat(Entity& entity_param, double& character_stat, const double kValue)
  : entity(entity_param),
    character_stat(character_stat),
    value(kValue) {
}

void Stat::AddStat() const { ModifyStat("add"); }

void Stat::RemoveStat(const int kStacks) const { ModifyStat("remove", kStacks); }

void Stat::ModifyStat(const std::string& kAction, const int kStacks) const {
  const double kCurrentStatValue = character_stat;
  auto new_stat_value = kCurrentStatValue;

  if (calculation_type == CalculationType::kAdditive) {
    new_stat_value += value * kStacks * (kAction == "remove" ? -1 : kAction == "add" ? 1 : 0);
  } else if (calculation_type == CalculationType::kMultiplicative) {
    if (kAction == "add") {
      new_stat_value *= value * kStacks;
    } else if (kAction == "remove") {
      new_stat_value /= value * kStacks;
    }
  }

  character_stat = new_stat_value;

  if (entity.ShouldWriteToCombatLog()) {
    auto msg = entity.name + " " + name + " ";

    if (kAction == "add") {
      if (calculation_type == CalculationType::kAdditive) {
        msg += "+";
      } else {
        msg += "*";
      }
    } else {
      if (calculation_type == CalculationType::kAdditive) {
        msg += "-";
      } else {
        msg += "/";
      }
    }

    msg += " " + DoubleToString(value * kStacks, combat_log_decimal_places) + " (" +
        DoubleToString(kCurrentStatValue, combat_log_decimal_places) + " -> " +
        DoubleToString(new_stat_value, combat_log_decimal_places) + ")";

    entity.CombatLog(msg);
  }
}

SpellPower::SpellPower(Entity& entity_param, const double kValue)
  : Stat(entity_param, entity_param.stats.spell_power, kValue) {
  name = StatName::kSpellPower;
  calculation_type = CalculationType::kAdditive;
}

ShadowPower::ShadowPower(Entity& entity_param, const double kValue)
  : Stat(entity_param, entity_param.stats.shadow_power, kValue) {
  name = StatName::kShadowPower;
  calculation_type = CalculationType::kAdditive;
}

FirePower::FirePower(Entity& entity_param, const double kValue)
  : Stat(entity_param, entity_param.stats.fire_power, kValue) {
  name = StatName::kFirePower;
  calculation_type = CalculationType::kAdditive;
}

SpellHasteRating::SpellHasteRating(Entity& entity_param, const double kValue)
  : Stat(entity_param, entity_param.stats.spell_haste_rating, kValue) {
  name = StatName::kSpellHasteRating;
  calculation_type = CalculationType::kAdditive;
}

SpellHastePercent::SpellHastePercent(Entity& entity_param, const double kValue)
  : Stat(entity_param, entity_param.stats.spell_haste_percent, kValue) {
  name = StatName::kSpellHastePercent;
  calculation_type = CalculationType::kMultiplicative;
  combat_log_decimal_places = 4;
}

MeleeHastePercent::MeleeHastePercent(Entity& entity_param, const double kValue)
  : Stat(entity_param, entity_param.stats.melee_haste_percent, kValue) {
  name = StatName::kMeleeHastePercent;
  calculation_type = CalculationType::kMultiplicative;
  combat_log_decimal_places = 4;
}

ManaCostModifier::ManaCostModifier(Entity& entity_param, const double kValue)
  : Stat(entity_param, entity_param.stats.mana_cost_modifier, kValue) {
  name = StatName::kManaCostModifier;
  calculation_type = CalculationType::kMultiplicative;
  combat_log_decimal_places = 2;
}

SpellCritChance::SpellCritChance(Entity& entity_param, const double kValue)
  : Stat(entity_param, entity_param.stats.spell_crit_chance, kValue) {
  name = StatName::kSpellCritChance;
  calculation_type = CalculationType::kAdditive;
  combat_log_decimal_places = 2;
}

SpellCritRating::SpellCritRating(Entity& entity_param, const double kValue)
  : Stat(entity_param, entity_param.stats.spell_crit_rating, kValue) {
  name = StatName::kSpellCritRating;
  calculation_type = CalculationType::kAdditive;
}

AttackPower::AttackPower(Entity& entity_param, const double kValue)
  : Stat(entity_param, entity_param.stats.attack_power, kValue) {
  name = StatName::kAttackPower;
  calculation_type = CalculationType::kAdditive;
}

AttackPowerModifier::AttackPowerModifier(Entity& entity_param, const double kValue)
  : Stat(entity_param, entity_param.stats.attack_power_modifier, kValue) {
  name = StatName::kAttackPowerModifier;
  calculation_type = CalculationType::kMultiplicative;
}