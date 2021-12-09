#ifndef WARLOCK_SIMULATOR_TBC_PET_AURAS
#define WARLOCK_SIMULATOR_TBC_PET_AURAS

#include "../aura.h"

struct PetAuras {
  std::unique_ptr<Aura> demonic_frenzy;
  std::unique_ptr<Aura> black_book;
  std::unique_ptr<Aura> battle_squawk;
};

#endif