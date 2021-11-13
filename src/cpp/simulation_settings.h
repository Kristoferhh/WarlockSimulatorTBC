#ifndef WARLOCK_SIMULATOR_TBC_SIMULATION_SETTINGS
#define WARLOCK_SIMULATOR_TBC_SIMULATION_SETTINGS

#include <iostream>

#include "enums.h"

struct SimulationSettings {
  int iterations;
  int min_time;
  int max_time;
  SimulationType simulation_type;
};

#endif