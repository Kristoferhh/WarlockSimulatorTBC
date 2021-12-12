#ifndef WARLOCK_SIMULATOR_TBC_PET_SPELL
#define WARLOCK_SIMULATOR_TBC_PET_SPELL

struct Pet;
#include <iostream>

#include "../enums.h"

struct PetSpell {
  Pet& pet;
  SpellSchool school;
  AttackType type;
  std::string name;
  double base_damage;
  bool casting;
  bool can_crit;
  double coefficient;
  double cooldown_remaining;
  double cast_time;
  int mana_cost;
  double modifier;
  int cooldown;

  PetSpell(Pet& pet);
  bool Ready();
  void Setup();
  void Reset();
  virtual double GetBaseDamage();
  double GetCastTime();
  virtual double GetCooldown();
  void Tick(double time);
  void StartCast();
  void Cast();
  void Damage(bool is_crit, bool is_glancing);
};

struct Melee : public PetSpell {
  Melee(Pet& pet);
  double GetBaseDamage();
  double GetCooldown();
};

struct ImpFirebolt : public PetSpell {
  ImpFirebolt(Pet& pet);
};

struct SuccubusLashOfPain : public PetSpell {
  SuccubusLashOfPain(Pet& pet);
};

struct FelguardCleave : public PetSpell {
  FelguardCleave(Pet& pet);
  double GetBaseDamage();
};

#endif