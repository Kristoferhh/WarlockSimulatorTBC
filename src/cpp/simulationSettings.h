#pragma once

#include <iostream>
#include "enums.h"

struct SimulationSettings
{
    int iterations;
    int minTime;
    int maxTime;
    SimulationType simulationType;

    SimulationSettings(int iterations, int minTime, int maxTime, SimulationType simulationType);
};