#pragma once
#include "player.h"
#include "simulationSettings.h"

struct Simulation
{
  Player* player;
  SimulationSettings* settings;

  Simulation(Player* _player, SimulationSettings* simSettings);
  void start();
  double passTime();
  void selectedSpellHandler(std::shared_ptr<Spell> spell, std::map<std::shared_ptr<Spell>, double>& predictedDamageOfSpells);
  void selectedSpellStartCast(std::shared_ptr<Spell> spell);
};