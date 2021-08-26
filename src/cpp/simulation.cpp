#include "simulation.h"
#include <vector>
#include <stdlib.h>
#include "common.h"
#include "bindings.h"
#include "spell.h"
#include <algorithm>

Simulation::Simulation(Player* _player, SimulationSettings* simulationSettings) : player(_player), settings(simulationSettings) {}

double Simulation::passTime()
{
    double time = this->player->castTimeRemaining;
    if (time == 0 || (this->player->gcdRemaining > 0 && this->player->gcdRemaining < time)) time = this->player->gcdRemaining;

    // Find the lowest time until the next action needs to be taken
    // Spells
    for (std::map<std::string, Spell*>::iterator it = this->player->spells.begin(); it != this->player->spells.end(); ++it)
    {
        if (it->second->cooldownRemaining > 0 && it->second->cooldownRemaining < time)
        {
            time = it->second->cooldownRemaining;
        }
    }

    // Auras
    
    // Pass time
    // This needs to be the first modified value since the time in combat needs to be updated before spells start dealing damage/auras expiring etc. for the combat logging.
    this->player->fightTime += time;
    this->player->castTimeRemaining -= time;

    // Auras need to tick before Spells because otherwise you'll, for example, finish casting Corruption and then immediately afterwards, in the same millisecond, immediately tick down the aura
    // This was also causing buffs like e.g. the t4 4pc buffs to expire sooner than they should.
    // Auras


    // Spells
    for (std::map<std::string, Spell*>::iterator it = this->player->spells.begin(); it != this->player->spells.end(); ++it)
    {
        if (it->second->cooldownRemaining > 0 || it->second->casting)
        {
            it->second->tick(time);
        }
    }

    return time;
}

double Simulation::start()
{
    double totalDamage = 0;
    std::vector<double> dpsVector;
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
                    if (this->player->spells.at("shadowBolt")->ready())
                    {
                        this->player->spells.at("shadowBolt")->startCast();
                    }
                    else if (this->player->spells.at("lifeTap")->ready())
                    {
                        this->player->spells.at("lifeTap")->startCast();
                    }
                }
            }

            passTime();
        }

        this->player->totalDuration += fightLength;
        totalDamage += this->player->iterationDamage;
        dpsVector.push_back(this->player->iterationDamage / fightLength);

        //todo remove hard-coding
        if (this->player->iteration % 100 == 0)
        {
            simulationUpdate(this->player->iteration, this->settings->iterations, median(dpsVector));
        }
    }

    return median(dpsVector);
    // todo: maybe memory leak monkaW
    //auto min = *min_element(std::begin(dpsVector), std::end(dpsVector));
    //auto max = *max_element(std::begin(dpsVector), std::end(dpsVector));
    //return SimulationResults(median(dpsVector), min, max, this->player->combatLogEntries);
}