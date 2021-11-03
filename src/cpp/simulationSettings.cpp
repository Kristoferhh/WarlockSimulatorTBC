#include "simulationSettings.h"
#include "enums.h"

SimulationSettings::SimulationSettings(int iterations, int minTime, int maxTime, unsigned int* randomSeeds, SimulationType simulationType)
  : iterations(iterations), minTime(minTime), maxTime(maxTime), randomSeeds(randomSeeds), simulationType(simulationType) {}