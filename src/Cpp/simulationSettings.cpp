#include "simulationSettings.h"

SimulationSettings::SimulationSettings(int iterations, int minTime, int maxTime, unsigned int* randomSeeds, bool multiItemSimulation)
  : iterations(iterations), minTime(minTime), maxTime(maxTime), randomSeeds(randomSeeds), multiItemSimulation(multiItemSimulation) {}