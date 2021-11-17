#ifndef WARLOCK_SIMULATOR_TBC_PET_AURAS
#define WARLOCK_SIMULATOR_TBC_PET_AURAS

#include "pet_aura.h"

struct PetAuras {
  std::unique_ptr<PetAura> demonic_frenzy;
  std::unique_ptr<PetAura> black_book;
  std::unique_ptr<PetAura> battle_squawk;
};

#endif