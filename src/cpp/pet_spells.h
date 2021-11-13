#ifndef WARLOCK_SIMULATOR_TBC_PET_SPELLS
#define WARLOCK_SIMULATOR_TBC_PET_SPELLS

#include "pet_spell.h"

struct PetSpells {
  std::unique_ptr<PetSpell> melee;
  std::unique_ptr<PetSpell> firebolt;
  std::unique_ptr<PetSpell> lash_of_pain;
  std::unique_ptr<PetSpell> cleave;
};

#endif