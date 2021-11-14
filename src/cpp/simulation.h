#ifndef WARLOCK_SIMULATOR_TBC_SIMULATION
#define WARLOCK_SIMULATOR_TBC_SIMULATION

#include "player.h"
#include "simulation_settings.h"

struct Simulation {
  std::shared_ptr<Player> player;
  std::shared_ptr<SimulationSettings> settings;

  Simulation(std::shared_ptr<Player> player, std::shared_ptr<SimulationSettings> sim_settings);
  void Start();
  double PassTime();
  void SelectedSpellHandler(std::shared_ptr<Spell>& spell,
                            std::map<std::shared_ptr<Spell>, double>& predicted_damage_of_spells,
                            double fight_time_remaining);
  void CastSelectedSpell(std::shared_ptr<Spell>& spell, double fight_time_remaining, double predicted_damage = 0);
};

#endif