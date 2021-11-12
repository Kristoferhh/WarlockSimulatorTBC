#ifndef WARLOCK_SIMULATOR_TBC_SIMULATION_SETTINGS
#define WARLOCK_SIMULATOR_TBC_SIMULATION_SETTINGS

#include <iostream>
#include "enums.h"

struct SimulationSettings
{
    int iterations;
    int minTime;
    int maxTime;
    SimulationType simulationType;
};

#endif