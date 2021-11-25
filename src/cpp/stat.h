#ifndef WARLOCK_SIMULATOR_TBC_STAT
#define WARLOCK_SIMULATOR_TBC_STAT

struct Player;

#include <map>
#include <string>

#include "enums.h"

struct Stat {
  Player& player;
  std::map<CharacterStat, double>& stat_map;
  CharacterStat characterStat;
  CalculationType calculationType;
  EntityType entity_type;
  std::string name;
  double value;
  bool affects_pet;  // true for stats like spell power where the pet scales from the player's spell power
  int combat_log_decimal_places;

  Stat(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value);
  void AddStat();
  void RemoveStat();

 private:
  void ModifyStat(std::string action);
};

struct SpellPower : public Stat {
  SpellPower(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value);
};

struct ShadowPower : public Stat {
  ShadowPower(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value);
};

struct FirePower : public Stat {
  FirePower(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value);
};

struct SpellHasteRating : public Stat {
  SpellHasteRating(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value);
};

struct SpellHastePercent : public Stat {
  SpellHastePercent(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value);
};

struct MeleeHastePercent : public Stat {
  MeleeHastePercent(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value);
};

struct ManaCostModifier : public Stat {
  ManaCostModifier(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value);
};

struct SpellCritChance : public Stat {
  SpellCritChance(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value);
};

struct SpellCritRating : public Stat {
  SpellCritRating(Player& player, std::map<CharacterStat, double>& stat_map, EntityType entity_type, double value);
};

struct AttackPower : public Stat {
  AttackPower(Player& player, std::map<CharacterStat, double>& stat_map, EntityType eentity_type, double value);
};

#endif