#pragma once
#include "enums.h"

struct Entity;

struct Stat {
  Entity& entity;
  double& character_stat;
  CalculationType calculation_type = CalculationType::kNoType;
  std::string name;
  double value;
  int combat_log_decimal_places = 0;

  Stat(Entity& entity_param, double& character_stat, double kValue);
  void AddStat() const;
  void RemoveStat(int kStacks = 1) const;

private:
  void ModifyStat(const std::string& kAction, int kStacks = 1) const;
};

struct SpellPower : Stat {
  SpellPower(Entity& entity_param, double kValue);
};

struct ShadowPower : Stat {
  ShadowPower(Entity& entity_param, double kValue);
};

struct FirePower : Stat {
  FirePower(Entity& entity_param, double kValue);
};

struct SpellHasteRating : Stat {
  SpellHasteRating(Entity& entity_param, double kValue);
};

struct SpellHastePercent : Stat {
  SpellHastePercent(Entity& entity_param, double kValue);
};

struct MeleeHastePercent : Stat {
  MeleeHastePercent(Entity& entity_param, double kValue);
};

struct ManaCostModifier : Stat {
  ManaCostModifier(Entity& entity_param, double kValue);
};

struct SpellCritChance : Stat {
  SpellCritChance(Entity& entity_param, double kValue);
};

struct SpellCritRating : Stat {
  SpellCritRating(Entity& entity_param, double kValue);
};

struct AttackPower : Stat {
  AttackPower(Entity& entity_param, double kValue);
};

struct AttackPowerModifier : Stat {
  AttackPowerModifier(Entity& entity_param, double kValue);
};
