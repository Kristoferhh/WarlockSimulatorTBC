#pragma once

#include <vector>

struct SimulationResults
{
    double medianDps;
    double minDps;
    double maxDps;
    double simDuration;
    std::vector<std::string> combatLog;

    SimulationResults(double medianDps, double minDps, double maxDps, std::vector<std::string> combatLog);
};