#include "simulationResults.h"
#include <iostream>

SimulationResults::SimulationResults(double medianDps, double minDps, double maxDps, std::vector<std::string> combatLog)
    : medianDps(medianDps), minDps(minDps), maxDps(maxDps), combatLog(combatLog) {}