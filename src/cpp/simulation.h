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
  void selectedSpellHandler(Spell* spell, std::map<Spell*, double>& predictedDamageOfSpells);
};