#ifndef WARLOCK_SIMULATOR_TBC_PET_AURA
#define WARLOCK_SIMULATOR_TBC_PET_AURA

struct Pet;
struct Player;
#include <iostream>

struct PetAura {
  std::shared_ptr<Pet> pet;
  std::string name;
  int duration;
  double duration_remaining;
  bool active;
  // Demonic Frenzy
  int max_stacks;
  int stacks;

  PetAura(std::shared_ptr<Pet> pet);
  void Tick(double time);
  virtual void Apply();
  virtual void Fade();
};

struct DemonicFrenzy : public PetAura {
  DemonicFrenzy(std::shared_ptr<Pet> pet);
  void Apply();
  void Fade();
};

struct BlackBook : public PetAura {
  BlackBook(std::shared_ptr<Pet> pet);
  void Apply(bool announce_in_combat_log = true);
  void Fade();
};

struct BattleSquawk : public PetAura {
  double haste_percent;

  BattleSquawk(std::shared_ptr<Pet> pet);
  void Apply();
  void Fade();
};

#endif