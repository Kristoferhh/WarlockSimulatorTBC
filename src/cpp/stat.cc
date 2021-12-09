#include "stat.h"

#include "common.h"
#include "player/player.h"

Stat::Stat(Entity& entity, double& character_stat, double value)
    : entity(entity), character_stat(character_stat), value(value), combat_log_decimal_places(0) {}

void Stat::AddStat() { ModifyStat("add"); }

void Stat::RemoveStat(int stacks) { ModifyStat("remove", stacks); }

void Stat::ModifyStat(std::string action, int stacks) {
  const double kCurrentStatValue = character_stat;
  double new_stat_value = kCurrentStatValue;

  if (calculation_type == CalculationType::kAdditive) {
    new_stat_value += (value * stacks) * (action == "remove" ? -1 : action == "add" ? 1 : 0);
  } else if (calculation_type == CalculationType::kMultiplicative) {
    if (action == "add") {
      new_stat_value *= (value * stacks);
    } else if (action == "remove") {
      new_stat_value /= (value * stacks);
    }
  }

  character_stat = new_stat_value;

  if (entity.ShouldWriteToCombatLog()) {
    entity.player->CombatLog((entity.entity_type == EntityType::kPlayer ? "Player "
                              : entity.entity_type == EntityType::kPet  ? "Pet "
                                                                        : "") +
                             name + " " +
                             (action == "add" ? (calculation_type == CalculationType::kAdditive ? "+" : "*")
                                              : (calculation_type == CalculationType::kAdditive ? "-" : "/")) +
                             " " + DoubleToString(value * stacks, combat_log_decimal_places) + " (" +
                             DoubleToString(kCurrentStatValue, combat_log_decimal_places) + " -> " +
                             DoubleToString(new_stat_value, combat_log_decimal_places) + ")");
  }
}

SpellPower::SpellPower(Entity& entity, double value) : Stat(entity, entity.stats.spell_power, value) {
  name = StatName::kSpellPower;
  calculation_type = CalculationType::kAdditive;
}

ShadowPower::ShadowPower(Entity& entity, double value) : Stat(entity, entity.stats.shadow_power, value) {
  name = StatName::kShadowPower;
  calculation_type = CalculationType::kAdditive;
}

FirePower::FirePower(Entity& entity, double value) : Stat(entity, entity.stats.fire_power, value) {
  name = StatName::kFirePower;
  calculation_type = CalculationType::kAdditive;
}

SpellHasteRating::SpellHasteRating(Entity& entity, double value)
    : Stat(entity, entity.stats.spell_haste_rating, value) {
  name = StatName::kSpellHasteRating;
  calculation_type = CalculationType::kAdditive;
}

SpellHastePercent::SpellHastePercent(Entity& entity, double value)
    : Stat(entity, entity.stats.spell_haste_percent, value) {
  name = StatName::kSpellHastePercent;
  calculation_type = CalculationType::kMultiplicative;
  combat_log_decimal_places = 4;
}

MeleeHastePercent::MeleeHastePercent(Entity& entity, double value)
    : Stat(entity, entity.stats.melee_haste_percent, value) {
  name = StatName::kMeleeHastePercent;
  calculation_type = CalculationType::kMultiplicative;
  combat_log_decimal_places = 4;
}

ManaCostModifier::ManaCostModifier(Entity& entity, double value)
    : Stat(entity, entity.stats.mana_cost_modifier, value) {
  name = StatName::kManaCostModifier;
  calculation_type = CalculationType::kMultiplicative;
  combat_log_decimal_places = 2;
}

SpellCritChance::SpellCritChance(Entity& entity, double value) : Stat(entity, entity.stats.spell_crit_chance, value) {
  name = StatName::kSpellCritChance;
  calculation_type = CalculationType::kAdditive;
  combat_log_decimal_places = 2;
}

SpellCritRating::SpellCritRating(Entity& entity, double value) : Stat(entity, entity.stats.spell_crit_rating, value) {
  name = StatName::kSpellCritRating;
  calculation_type = CalculationType::kAdditive;
}

AttackPower::AttackPower(Entity& entity, double value) : Stat(entity, entity.stats.attack_power, value) {
  name = StatName::kAttackPower;
  calculation_type = CalculationType::kAdditive;
}

AttackPowerModifier::AttackPowerModifier(Entity& entity, double value)
    : Stat(entity, entity.stats.attack_power_modifier, value) {
  name = StatName::kAttackPowerModifier;
  calculation_type = CalculationType::kMultiplicative;
}