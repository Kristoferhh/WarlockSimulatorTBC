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
        if (it->second->active && it->second->hasDuration && it->second->durationRemaining < time)
        {
            time = it->second->durationRemaining;
        }
    }

    if (player->mp5Timer < time)
    {
        time = player->mp5Timer;
    }
    for (int i = 0; i < player->trinkets.size(); i++)
    {
        if (player->trinkets[i]->active && player->trinkets[i]->durationRemaining < time)
        {
            time = player->trinkets[i]->durationRemaining;
        }
        if (player->trinkets[i]->cooldownRemaining > 0 && player->trinkets[i]->cooldownRemaining < time)
        {
            time = player->trinkets[i]->cooldownRemaining;
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
        if (it->second->active && it->second->hasDuration)
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

    // Trinkets
    for (int i = 0; i < player->trinkets.size(); i++)
    {
        player->trinkets[i]->tick(time);
    }

    player->gcdRemaining -= time;
    player->mp5Timer -= time;
    player->fiveSecondRuleTimer -= time;
    if (player->mp5Timer <= 0)
    {
        player->mp5Timer = 5;
        if (player->stats->mp5 > 0 || player->fiveSecondRuleTimer <= 0 || (player->auras.count("innervate") > 0 && player->auras.at("innervate")->active))
        {
            bool innervateExists = player->auras.count("innervate") > 0;
            bool innervateActive = innervateExists && player->auras.at("innervate")->active;
            int currentPlayerMana = player->stats->mana;

            // MP5
            if (player->stats->mp5)
            {
                player->stats->mana = std::min(player->stats->maxMana, currentPlayerMana + player->stats->mp5);
            }
            // Spirit mana regen
            if (player->fiveSecondRuleTimer <= 0 || innervateActive)
            {
                // Formula from https://wowwiki-archive.fandom.com/wiki/Spirit?oldid=1572910
                int mp5FromSpirit = 5 * (0.001 + std::sqrt(player->stats->intellect * player->stats->intellectModifier) * (player->stats->spirit * player->stats->spiritModifier) * 0.009327);
                if (innervateActive)
                {
                    mp5FromSpirit *= 4;
                }
                player->stats->mana = std::min(player->stats->maxMana, currentPlayerMana + mp5FromSpirit);
            }

            int manaGained = player->stats->mana - currentPlayerMana;
            player->totalManaRegenerated += manaGained;
            
            if (player->shouldWriteToCombatLog())
            {
                std::string msg = "Player gains " + std::to_string(round(manaGained)) + " mana from MP5 (" + std::to_string(currentPlayerMana) + " -> " + std::to_string(player->stats->mana) + ")";
                player->combatLog(msg);
            }
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
            // Use Drums
            if (player->auras.count("drumsOfBattle") > 0 && !player->auras.at("drumsOfBattle")->active && player->spells.at("drumsOfBattle")->ready())
            {
                player->spells.at("drumsOfBattle")->startCast();
            }
            else if (player->auras.count("drumsOfWar") > 0 && !player->auras.at("drumsOfWar")->active && player->spells.at("drumsOfWar")->ready())
            {
                player->spells.at("drumsOfWar")->startCast();
            }
            else if (player->auras.count("drumsOfRestoration") > 0 && !player->auras.at("drumsOfRestoration")->active && player->spells.at("drumsOfRestoration")->ready())
            {
                player->spells.at("drumsOfRestoration")->startCast();
            }

            // Player
            if (player->castTimeRemaining <= 0)
            {
                // Spells not on the GCD
                if (player->spells.count("demonicRune") > 0 && (player->stats->maxMana - player->stats->mana) > player->spells.at("demonicRune")->avgManaValue && player->spells.at("demonicRune")->ready())
                {
                    player->spells.at("demonicRune")->startCast();
                }
                if (player->spells.count("superManaPotion") > 0 && (player->stats->maxMana - player->stats->mana) > player->spells.at("superManaPotion")->avgManaValue && player->spells.at("superManaPotion")->ready())
                {
                    player->spells.at("superManaPotion")->startCast();
                }

                // Spells on the GCD
                if (player->gcdRemaining <= 0)
                {
                    player->useCooldowns();
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