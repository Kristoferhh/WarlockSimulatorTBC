#include "bindings.h"
#include "simulation.h"

#ifdef USE_EMSCRIPTEN
#include <emscripten.h>
#endif

void startSimulation(Simulation* sim)
{
    sim->start();
}

Simulation* allocSim(Player* player, SimulationSettings* simulationSettings)
{
    return new Simulation(player, simulationSettings);
}