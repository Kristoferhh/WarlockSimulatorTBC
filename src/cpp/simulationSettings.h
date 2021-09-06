#pragma once

#include <iostream>

struct SimulationSettings
{
    int iterations;
    int minTime;
    int maxTime;
    unsigned int* randomSeeds;

    SimulationSettings(int iterations, int minTime, int maxTime, unsigned int* randomSeeds);
};