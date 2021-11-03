#pragma once

#include <iostream>
#include "enums.h"

struct SimulationSettings
{
    int iterations;
    int minTime;
    int maxTime;
    unsigned int* randomSeeds;
    SimulationType simulationType;

    SimulationSettings(int iterations, int minTime, int maxTime, unsigned int* randomSeeds, SimulationType simulationType);
};