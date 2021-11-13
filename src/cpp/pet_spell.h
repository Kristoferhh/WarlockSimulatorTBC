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
  bool canCrit;
  double coefficient;
  double cooldownRemaining;
  double castTime;
  int manaCost;
  double modifier;
  int cooldown;

  PetSpell(std::shared_ptr<Pet> pet);
  bool ready();
  void setup();
  void reset();
  virtual double getBaseDamage();
  double getCastTime();
  virtual double getCooldown();
  void tick(double time);
  void startCast();
  void cast();
  void damage(bool isCrit, bool isGlancing);
};

struct Melee : public PetSpell {
  Melee(std::shared_ptr<Pet> pet);
  double getBaseDamage();
  double getCooldown();
};

struct ImpFirebolt : public PetSpell {
  ImpFirebolt(std::shared_ptr<Pet> pet);
};

struct SuccubusLashOfPain : public PetSpell {
  SuccubusLashOfPain(std::shared_ptr<Pet> pet);
};

struct FelguardCleave : public PetSpell {
  FelguardCleave(std::shared_ptr<Pet> pet);
  double getBaseDamage();
};

#endif