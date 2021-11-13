#ifndef WARLOCK_SIMULATOR_TBC_PET_SPELLS
#define WARLOCK_SIMULATOR_TBC_PET_SPELLS

#include "pet_spell.h"

struct PetSpells {
  std::unique_ptr<PetSpell> Melee;
  std::unique_ptr<PetSpell> Firebolt;
  std::unique_ptr<PetSpell> LashOfPain;
  std::unique_ptr<PetSpell> Cleave;
};

#endif