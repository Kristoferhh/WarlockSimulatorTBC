#include "simulation.h"
#include <vector>
#include <stdlib.h>
#include "common.h"
#include "bindings.h"
#include "spell.h"
#include <algorithm>
#include <chrono>

Simulation::Simulation(Player* _player, SimulationSettings* simulationSettings) : player(_player), settings(simulationSettings) {}

double Simulation::passTime()
{
    double time = player->castTimeRemaining;
    if (time == 0 || (player->gcdRemaining > 0 && player->gcdRemaining < time)) time = player->gcdRemaining;

    // Find the lowest time until the next action needs to be taken
    // Spells
    for (std::map<std::string, Spell*>::iterator it = player->spells.begin(); it != player->spells.end(); it++)
    {
        if (it->second->cooldownRemaining > 0 && it->second->cooldownRemaining < time)
        {
            time = it->second->cooldownRemaining;
        }
    }

    // Auras
    for (std::map<std::string, Aura*>::iterator it = player->auras.begin(); it != player->auras.end(); it++)
    {
        if (it->second->active && it->second->durationRemaining < time)
        {
            time = it->second->durationRemaining;
        }
    }

    // Pass time
    // This needs to be the first modified value since the time in combat needs to be updated before spells start dealing damage/auras expiring etc. for the combat logging.
    player->fightTime += time;
    player->castTimeRemaining -= time;

    // Auras need to tick before Spells because otherwise you'll, for example, finish casting Corruption and then immediately afterwards, in the same millisecond, immediately tick down the aura
    // This was also causing buffs like e.g. the t4 4pc buffs to expire sooner than they should.
    // Auras
    for (std::map<std::string, Aura*>::iterator it = player->auras.begin(); it != player->auras.end(); it++)
    {
        if (it->second->active)
        {
            it->second->tick(time);
        }
    }

    // Spells
    for (std::map<std::string, Spell*>::iterator it = player->spells.begin(); it != player->spells.end(); ++it)
    {
        if (it->second->cooldownRemaining > 0 || it->second->casting)
        {
            it->second->tick(time);
        }
    }

    return time;
}

void Simulation::start()
{
    double totalDamage = 0;
    std::vector<double> dpsVector;
    player->totalDuration = 0;
    player->initialize();
    double minDps = 99999;
    double maxDps = 0;
    auto startTime = std::chrono::high_resolution_clock::now();

    for (player->iteration = 0; player->iteration < this->settings->iterations; player->iteration++)
    {
        player->reset();
        const int fightLength = random(this->settings->minTime, this->settings->maxTime);
        player->iterationDamage = 0;
        player->fightTime = 0;
        //player->combatLog("Fight length: " + std::to_string(fightLength) + " seconds");

        while (player->fightTime < fightLength)
        {
            // Player
            if (player->castTimeRemaining <= 0)
            {
                // Spells not on the GCD

                // Spells on the GCD
                if (player->gcdRemaining <= 0)
                {
                    if (player->spells.at("shadowBolt")->ready())
                    {
                        player->spells.at("shadowBolt")->startCast();
                    }
                    else if (player->spells.at("lifeTap")->ready())
                    {
                        player->spells.at("lifeTap")->startCast();
                    }
                }
            }

            passTime();
        }

        player->totalDuration += fightLength;
        totalDamage += player->iterationDamage;
        double dps = player->iterationDamage / fightLength;
        if (dps > maxDps)
        {
            maxDps = dps;
        }
        if (dps < minDps)
        {
            minDps = dps;
        }
        dpsVector.push_back(dps);

        //todo remove hard-coding
        if (player->iteration % 100 == 0)
        {
            simulationUpdate(player->iteration, this->settings->iterations, median(dpsVector), player->settings->itemId);
        }

        // End all active auras
        for (std::map<std::string, Aura*>::iterator it = player->auras.begin(); it != player->auras.end(); it++)
        {
            if (it->second->active)
            {
                it->second->fade(true);
            }
        }
    }

    auto finishTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = finishTime - startTime;
    // Send the contents of the combat log to the web worker
    /*for (const auto& value: player->combatLogEntries)
    {
        combatLogUpdate(value.c_str());
    }*/
    simulationEnd(median(dpsVector), minDps, maxDps, elapsedTime, player->settings->itemId);

    // Free spells and auras
    for(std::map<std::string, Spell*>::iterator it = player->spells.begin(); it != player->spells.end(); it++)
    {
        delete it->second;
    }
    for(std::map<std::string, Aura*>::iterator it = player->auras.begin(); it != player->auras.end(); it++)
    {
        delete it->second;
    }
}