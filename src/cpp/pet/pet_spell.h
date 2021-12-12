#ifndef WARLOCK_SIMULATOR_TBC_PET_SPELL
#define WARLOCK_SIMULATOR_TBC_PET_SPELL

struct Pet;
#include <iostream>

#include "../enums.h"
#include "../spell.h"

struct PetSpell : Spell {
  Pet& pet;
  AttackType type;

  PetSpell(Pet& pet);
  virtual double GetBaseDamage();
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