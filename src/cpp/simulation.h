#ifndef WARLOCK_SIMULATOR_TBC_SIMULATION
#define WARLOCK_SIMULATOR_TBC_SIMULATION

#include "player.h"
#include "simulation_settings.h"

struct Simulation
{
  std::shared_ptr<Player> player;
  std::shared_ptr<SimulationSettings> settings;

  Simulation(std::shared_ptr<Player> player, std::shared_ptr<SimulationSettings> simSettings);
  void start();
  double passTime();
  void selectedSpellHandler(std::shared_ptr<Spell>& spell, std::map<std::shared_ptr<Spell>, double>& predictedDamageOfSpells);
  void castSelectedSpell(std::shared_ptr<Spell>& spell, double predictedDamage = 0);
};

#endif