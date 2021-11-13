#ifndef WARLOCK_SIMULATOR_TBC_PET_SPELL
#define WARLOCK_SIMULATOR_TBC_PET_SPELL

struct Pet;
#include <iostream>

#include "enums.h"

struct PetSpell {
  std::shared_ptr<Pet> pet;
  SpellSchool school;
  AttackType type;
  std::string name;
  double dmg;
  bool casting;
  bool can_crit;
  double coefficient;
  double cooldown_remaining;
  double cast_time;
  int mana_cost;
  double modifier;
  int cooldown;

  PetSpell(std::shared_ptr<Pet> pet);
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
  Melee(std::shared_ptr<Pet> pet);
  double GetBaseDamage();
  double GetCooldown();
};

struct ImpFirebolt : public PetSpell {
  ImpFirebolt(std::shared_ptr<Pet> pet);
};

struct SuccubusLashOfPain : public PetSpell {
  SuccubusLashOfPain(std::shared_ptr<Pet> pet);
};

struct FelguardCleave : public PetSpell {
  FelguardCleave(std::shared_ptr<Pet> pet);
  double GetBaseDamage();
};

#endif