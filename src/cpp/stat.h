#ifndef WARLOCK_SIMULATOR_TBC_STAT
#define WARLOCK_SIMULATOR_TBC_STAT

struct Entity;

#include <map>
#include <string>

#include "enums.h"

struct Stat {
  Entity& entity;
  double& character_stat;
  CalculationType calculation_type;
  std::string name;
  double value;
  bool affects_pet;  // true for stats like spell power where the pet scales from the player's spell power
  int combat_log_decimal_places;

  Stat(Entity& entity, double& character_stat, double value);
  void AddStat();
  void RemoveStat(int stacks = 1);

 private:
  void ModifyStat(std::string action, int stacks = 1);
};

struct SpellPower : public Stat {
  SpellPower(Entity& entity, double& character_stat, double value);
};

struct ShadowPower : public Stat {
  ShadowPower(Entity& entity, double value);
};

struct FirePower : public Stat {
  FirePower(Entity& entity, double value);
};

struct SpellHasteRating : public Stat {
  SpellHasteRating(Entity& entity, double value);
};

struct SpellHastePercent : public Stat {
  SpellHastePercent(Entity& entity, double value);
};

struct MeleeHastePercent : public Stat {
  MeleeHastePercent(Entity& entity, double value);
};

struct ManaCostModifier : public Stat {
  ManaCostModifier(Entity& entity, double value);
};

struct SpellCritChance : public Stat {
  SpellCritChance(Entity& entity, double value);
};

struct SpellCritRating : public Stat {
  SpellCritRating(Entity& entity, double value);
};

struct AttackPower : public Stat {
  AttackPower(Entity& entity, double value);
};

#endif