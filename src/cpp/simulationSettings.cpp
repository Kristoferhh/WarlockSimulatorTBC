#include "simulationSettings.h"
#include "enums.h"

SimulationSettings::SimulationSettings(int iterations, int minTime, int maxTime, std::shared_ptr<uint32_t[]> randomSeeds, SimulationType simulationType)
  : iterations(iterations), minTime(minTime), maxTime(maxTime), randomSeeds(randomSeeds), simulationType(simulationType) {}