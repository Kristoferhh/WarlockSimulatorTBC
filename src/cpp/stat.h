#ifndef WARLOCK_SIMULATOR_TBC_STAT
#define WARLOCK_SIMULATOR_TBC_STAT

struct Player;

#include <map>
#include <string>

#include "enums.h"

struct Stat {
  Player& player;
  double& character_stat;
  CalculationType calculation_type;
  EntityType entity_type;
  std::string name;
  double value;
  bool affects_pet;  // true for stats like spell power where the pet scales from the player's spell power
  int combat_log_decimal_places;

  Stat(Player& player, double& character_stat, EntityType entity_type, double value);
  void AddStat();
  void RemoveStat(int stacks = 1);

 private:
  void ModifyStat(std::string action, int stacks = 1);
};

struct SpellPower : public Stat {
  SpellPower(Player& player, double& character_stat, EntityType entity_type, double value);
};

struct ShadowPower : public Stat {
  ShadowPower(Player& player, double& character_stat, EntityType entity_type, double value);
};

struct FirePower : public Stat {
  FirePower(Player& player, double& character_stat, EntityType entity_type, double value);
};

struct SpellHasteRating : public Stat {
  SpellHasteRating(Player& player, double& character_stat, EntityType entity_type, double value);
};

struct SpellHastePercent : public Stat {
  SpellHastePercent(Player& player, double& character_stat, EntityType entity_type, double value);
};

struct MeleeHastePercent : public Stat {
  MeleeHastePercent(Player& player, double& character_stat, EntityType entity_type, double value);
};

struct ManaCostModifier : public Stat {
  ManaCostModifier(Player& player, double& character_stat, EntityType entity_type, double value);
};

struct SpellCritChance : public Stat {
  SpellCritChance(Player& player, double& character_stat, EntityType entity_type, double value);
};

struct SpellCritRating : public Stat {
  SpellCritRating(Player& player, double& character_stat, EntityType entity_type, double value);
};

struct AttackPower : public Stat {
  AttackPower(Player& player, double& character_stat, EntityType eentity_type, double value);
};

#endif