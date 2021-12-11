#ifndef WARLOCK_SIMULATOR_TBC_SIMULATION
#define WARLOCK_SIMULATOR_TBC_SIMULATION

#include "player/player.h"
#include "simulation_settings.h"
#include "spell/spell.h"

struct Simulation {
  Player& player;
  const SimulationSettings& settings;
  int iteration;
  double fight_time;

  Simulation(Player& player, const SimulationSettings& sim_settings);
  void Start();
  double PassTime();
  void Tick(double time);
  void SelectedSpellHandler(const std::shared_ptr<Spell>& spell,
                            std::map<std::shared_ptr<Spell>, double>& predicted_damage_of_spells,
                            double fight_time_remaining);
  void CastSelectedSpell(const std::shared_ptr<Spell>& spell, double fight_time_remaining, double predicted_damage = 0);
};

#endif