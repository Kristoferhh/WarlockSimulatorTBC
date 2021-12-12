#ifndef WARLOCK_SIMULATOR_TBC_PET
#define WARLOCK_SIMULATOR_TBC_PET

struct Player;
#include <iostream>
#include <map>

#include "../character_stats.h"
#include "../entity.h"
#include "../enums.h"
#include "../simulation.h"
#include "pet_auras.h"
#include "pet_spells.h"

struct Pet : public Entity, std::enable_shared_from_this<Pet> {
  const double kBaseMeleeSpeed = 2;
  PetSpells spells;
  PetAuras auras;
  PetName pet_name;
  PetType pet_type;
  double glancing_blow_multiplier;
  double glancing_blow_chance;
  double crit_suppression;
  double enemy_damage_reduction_from_armor;

  Pet(Player& player);
  void Initialize(Simulation* simulation);
  void CalculateStatsFromAuras();
  void Setup();
  void Reset();
  void Tick(double t);
  double GetAttackPower();
  double GetMeleeCritChance();
  double GetHastePercent();
  double GetSpellCritChance();
  double GetStamina();
  double GetIntellect();
  double GetAgility();
  double GetStrength();
  double GetPlayerSpellPower();
  double GetSpellPower();
  double GetDebuffAttackPower();
  double CalculateMaxMana();
  bool IsCrit(AttackType type);
  bool IsHit(AttackType type);
};

struct Imp : public Pet {
  Imp(Player& player);
};

struct Succubus : public Pet {
  Succubus(Player& player);
};

struct Felguard : public Pet {
  Felguard(Player& player);
};

#endif