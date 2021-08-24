#pragma once
#include "player.h"
#include "simulationSettings.h"

struct Simulation
{
  Player* player;
  SimulationSettings* settings;

  Simulation(Player* _player, SimulationSettings* simSettings);
  double start();
};