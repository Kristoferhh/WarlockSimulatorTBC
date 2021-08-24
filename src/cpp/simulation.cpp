#include "simulation.h"
#include <vector>
#include <stdlib.h>
#include "common.h"
#include "bindings.h"

Simulation::Simulation(Player* _player, SimulationSettings* simulationSettings) : player(_player), settings(simulationSettings) {}

void Simulation::start()
{
    uint64_t totalDamage = 0;
    std::vector<int> dpsVector;
    this->player->totalDuration = 0;
    this->player->initialize();

    for (this->player->iteration = 0; this->player->iteration < this->settings->iterations; this->player->iteration++)
    {
        //std::cout << "Iteration " << this->player->iteration << std::endl;
        const int fightLength = random(this->settings->minTime, this->settings->maxTime);
        this->player->iterationDamage = 0;
        this->player->fightTime = 0;
        //this->player->combatLog("Fight length: " + std::to_string(fightLength) + " seconds");

        while (this->player->fightTime < fightLength)
        {
            // Player
            if (this->player->castTimeRemaining <= 0)
            {
                // Spells not on the GCD

                // Spells on the GCD
                if (this->player->gcdRemaining <= 0)
                {
                    
                }
            }

            this->player->fightTime += 0.5;
        }
    }
}