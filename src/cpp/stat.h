#ifndef WARLOCK_SIMULATOR_TBC_STAT
#define WARLOCK_SIMULATOR_TBC_STAT

struct Player;

#include <map>
#include <string>

#include "enums.h"

struct Stat {
  CharacterStat characterStat;
  CalculationType calculationType;
  std::string name;
  double value;
  bool affects_pet;  // true for stats like spell power where the pet scales from the player's spell power
  int combat_log_decimal_places;

  Stat(double value);
  void AddStat(Player& player);
  void RemoveStat(Player& player);

 private:
  void ModifyStat(Player& player, std::string action);
};

struct SpellPower : public Stat {
  SpellPower(double value);
};
struct HasteRating : public Stat {
  HasteRating(double value);
};
struct ShadowPower : public Stat {
  ShadowPower(double value);
};
struct FirePower : public Stat {
  FirePower(double value);
};
struct HastePercent : public Stat {
  HastePercent(double value);
};
struct ManaCostModifier : public Stat {
  ManaCostModifier(double value);
};
struct CritChance : public Stat {
  CritChance(double value);
};
struct CritRating : public Stat {
  CritRating(double value);
};

#endif