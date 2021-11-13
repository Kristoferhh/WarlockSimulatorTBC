#ifndef WARLOCK_SIMULATOR_TBC_PET_AURAS
#define WARLOCK_SIMULATOR_TBC_PET_AURAS

#include "pet_aura.h"

struct PetAuras {
  std::unique_ptr<PetAura> DemonicFrenzy;
  std::unique_ptr<PetAura> BlackBook;
};

#endif