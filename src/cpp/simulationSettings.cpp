#include "simulationSettings.h"

SimulationSettings::SimulationSettings(int iterations, int minTime, int maxTime, unsigned int* randomSeeds)
  : iterations(iterations), minTime(minTime), maxTime(maxTime), randomSeeds(randomSeeds) {}