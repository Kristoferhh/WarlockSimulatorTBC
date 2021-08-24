#pragma once

#include <iostream>

struct SimulationSettings
{
    int iterations;
    int minTime;
    int maxTime;

    SimulationSettings(int iterations, int minTime, int maxTime);
};