#include "simulationSettings.h"
#include "enums.h"

SimulationSettings::SimulationSettings(int iterations, int minTime, int maxTime, SimulationType simulationType)
  : iterations(iterations), minTime(minTime), maxTime(maxTime), simulationType(simulationType) {}