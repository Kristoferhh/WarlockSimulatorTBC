#pragma once
#include "simulation.h"

extern "C"
{
  Simulation* allocSim(Player* player, SimulationSettings* simulationSettings);
  void freeSim();
  void startSimulation(Simulation* sim);
}