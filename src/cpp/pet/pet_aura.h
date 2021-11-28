#ifndef WARLOCK_SIMULATOR_TBC_PET_AURA
#define WARLOCK_SIMULATOR_TBC_PET_AURA

struct Pet;
struct Player;
#include <iostream>
#include <vector>

#include "../stat.h"

struct PetAura {
  std::shared_ptr<Pet> pet;
  std::string name;
  std::vector<Stat> stats;
  int duration;
  double duration_remaining;
  bool active;
  // Demonic Frenzy
  int max_stacks;
  int stacks;

  PetAura(std::shared_ptr<Pet> pet);
  void Tick(double time);
  void Apply();
  void Fade();
  void Setup();
};

struct DemonicFrenzy : public PetAura {
  DemonicFrenzy(std::shared_ptr<Pet> pet);
};

struct BlackBook : public PetAura {
  BlackBook(std::shared_ptr<Pet> pet);
};

struct BattleSquawk : public PetAura {
  BattleSquawk(std::shared_ptr<Pet> pet);
};

#endif