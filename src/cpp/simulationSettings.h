#pragma once

#include <iostream>
#include "enums.h"

struct SimulationSettings
{
    int iterations;
    int minTime;
    int maxTime;
    std::shared_ptr<uint32_t[]> randomSeeds;
    SimulationType simulationType;

    SimulationSettings(int iterations, int minTime, int maxTime, std::shared_ptr<uint32_t[]> randomSeeds, SimulationType simulationType);
};