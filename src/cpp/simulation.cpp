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

    // Dots
    for (std::map<std::string, DamageOverTime*>::iterator it = player->dots.begin(); it != player->dots.end(); it++)
    {
        if (it->second->active && it->second->tickTimerRemaining < time)
        {
            time = it->second->tickTimerRemaining;
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

    // Dots
    for (std::map<std::string, DamageOverTime*>::iterator it = player->dots.begin(); it != player->dots.end(); ++it)
    {
        if (it->second->tickTimerRemaining > 0)
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
                // Demonic Rune
                if ((player->fightTime > 5 || player->stats->mp5 == 0) && player->spells.count("demonicRune") > 0 && (player->stats->maxMana - player->stats->mana) > player->spells.at("demonicRune")->avgManaValue && player->spells.at("demonicRune")->ready())
                {
                    player->spells.at("demonicRune")->startCast();
                }
                // Super Mana Potion
                if ((player->fightTime > 5 || player->stats->mp5 == 0) && player->spells.count("superManaPotion") > 0 && (player->stats->maxMana - player->stats->mana) > player->spells.at("superManaPotion")->avgManaValue && player->spells.at("superManaPotion")->ready())
                {
                    player->spells.at("superManaPotion")->startCast();
                }

                // Spells on the GCD
                if (player->gcdRemaining <= 0)
                {
                    if (player->settings->isSingleTarget)
                    {
                        double timeRemaining = fightLength - player->fightTime;

                        // Map of spells with their predicted damage as the value. This is used by the sim to determine what the best spell to cast is.
                        std::map<std::string, double> predictedDamageOfSpells;

                        // If the sim is choosing the rotation for the user then predict the damage of the three filler spells if they're available (maybe just skip Searing Pain to save time, there's no way it will ever be the best spell to cast)
                        if (player->settings->simChoosingRotation)
                        {
                            predictedDamageOfSpells.insert(std::make_pair("shadowBolt", player->spells.at("shadowBolt")->predictDamage()));
                            predictedDamageOfSpells.insert(std::make_pair("incinerate", player->spells.at("incinerate")->predictDamage()));
                            predictedDamageOfSpells.insert(std::make_pair("searingPain", player->spells.at("searingPain")->predictDamage()));
                        }

                        //todo finishers

                        // Cast selected curse if it's either CoR or CoE and it's not up
                        if (player->curse != "" && (player->curse == "curseOfRecklessness" || player->curse == "curseOfTheElements") && !player->auras.at(player->curse)->active && player->spells.at(player->curse)->ready())
                        {
                            player->spells.at(player->curse)->startCast();
                        }
                        else
                        {
                            player->useCooldowns();
                            // Cast Curse of Doom if there's more than 60 seconds remaining
                            if (timeRemaining > 60 && player->curse == "curseOfDoom" && !player->dots.at("curseOfDoom")->active && player->spells.at("curseOfDoom")->canCast())
                            {
                                // If the sim is choosing the rotation for the player then predict the damage of the spell and put it in the map
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair(player->spells.at("curseOfDoom")->varName, player->spells.at("curseOfDoom")->predictDamage()));
                                }
                                // Else if the player is choosing the rotation themselves then just cast the highest priority spell that needs to be cast
                                else if (player->spells.at(player->curse)->hasEnoughMana())
                                {
                                    if (player->spells.count("amplifyCurse") > 0 && player->spells.at("amplifyCurse")->ready())
                                    {
                                        player->spells.at("amplifyCurse")->startCast();
                                    }
                                    player->spells.at("curseOfDoom")->startCast();
                                }
                            }
                            // Cast Curse of Agony if CoA is the selected curse or if Curse of Doom is the selected curse and there's less than 60 seconds remaining of the fight
                            if (player->curse != "" && player->dots.count("curseOfAgony") > 0 && !player->dots.at("curseOfAgony")->active && timeRemaining > player->dots.at("curseOfAgony")->minimumDuration && ((player->curse == "curseOfDoom" && !player->dots.at("curseOfDoom")->active && (player->spells.at("curseOfDoom")->cooldownRemaining > player->dots.at("curseOfAgony")->minimumDuration || timeRemaining < 60)) || (player->curse == "curseOfAgony" && player->spells.at("curseOfAgony")->canCast())))
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("curseOfAgony", player->spells.at("curseOfAgony")->predictDamage()));
                                }
                                else if (player->spells.at("curseOfAgony")->hasEnoughMana())
                                {
                                    if (player->spells.count("amplifyCurse") > 0 && player->spells.at("amplifyCurse")->ready())
                                    {
                                        player->spells.at("amplifyCurse")->startCast();
                                    }
                                    player->spells.at("curseOfAgony")->startCast();
                                }
                            }
                            // Cast Corruption if Corruption isn't up or if it will expire before the cast finishes (if no instant Corruption)
                            if (player->spells.count("corruption") > 0 && (!player->dots.at("corruption")->active || (player->dots.at("corruption")->ticksRemaining == 1 && player->dots.at("corruption")->tickTimerRemaining < player->spells.at("corruption")->getCastTime())) && player->spells.at("corruption")->canCast() && (timeRemaining - player->spells.at("corruption")->getCastTime()) >= player->dots.at("corruption")->minimumDuration)
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("corruption", player->spells.at("corruption")->predictDamage()));
                                }
                                else if (player->spells.at("corruption")->hasEnoughMana())
                                {
                                    player->spells.at("corruption")->startCast();
                                }
                            }
                            // Cast Shadow Bolt if Shadow Trance (Nightfall) is active and Corruption is active as well to avoid potentially wasting another Nightfall proc
                            if (player->spells.count("shadowBolt") > 0 && player->auras.count("shadowTrance") > 0 && player->auras.at("shadowTrance")->active && player->dots.at("corruption")->active && player->spells.at("shadowBolt")->canCast())
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    // todo: check if the spell is already in the array before adding it
                                    predictedDamageOfSpells.insert(std::make_pair("shadowBolt", player->spells.at("shadowBolt")->predictDamage()));
                                }
                                else if (player->spells.at("shadowBolt")->hasEnoughMana())
                                {
                                    player->spells.at("shadowBolt")->startCast();
                                }
                            }
                            // Cast Unstable Affliction if it's not up or if it's about to expire
                            if (player->spells.count("unstableAffliction") > 0 && player->spells.at("unstableAffliction")->canCast() && (!player->dots.at("unstableAffliction")->active || (player->dots.at("unstableAffliction")->ticksRemaining == 1 && player->dots.at("unstableAffliction")->tickTimerRemaining < player->spells.at("unstableAffliction")->getCastTime())) && (timeRemaining - player->spells.at("unstableAffliction")->getCastTime()) >= player->dots.at("unstableAffliction")->minimumDuration)
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("unstableAffliction", player->spells.at("unstableAffliction")->predictDamage()));
                                }
                                else if (player->spells.at("unstableAffliction")->hasEnoughMana())
                                {
                                    player->spells.at("unstableAffliction")->startCast();
                                }
                            }
                            // Cast Siphon Life if it's not up (todo: add option to only cast it while ISB is active if not using custom ISB uptime %)
                            if (player->spells.count("siphonLife") > 0 && !player->dots.at("siphonLife")->active && player->spells.at("siphonLife")->canCast() && timeRemaining >= player->dots.at("siphonLife")->minimumDuration)
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("siphonLife", player->spells.at("siphonLife")->predictDamage()));
                                }
                                else if (player->spells.at("siphonLife")->hasEnoughMana())
                                {
                                    player->spells.at("siphonLife")->startCast();
                                }
                            }
                            // Cast Immolate if it's not up or about to expire
                            if (player->spells.count("immolate") > 0 && player->spells.at("immolate")->canCast() && (!player->dots.at("immolate")->active || (player->dots.at("immolate")->ticksRemaining == 1 && player->dots.at("immolate")->tickTimerRemaining < player->spells.at("immolate")->getCastTime())) && (timeRemaining - player->spells.at("immolate")->getCastTime()) >= player->dots.at("immolate")->minimumDuration)
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("immolate", player->spells.at("immolate")->predictDamage()));
                                }
                                else if (player->spells.at("immolate")->hasEnoughMana())
                                {
                                    player->spells.at("immolate")->startCast();
                                }
                            }
                            // Cast Shadow Bolt if Shadow Trance (Nightfall) is active
                            if (player->spells.count("shadowBolt") > 0 && player->auras.count("shadowTrance") > 0 && player->auras.at("shadowTrance")->active && player->spells.at("shadowBolt")->canCast())
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("shadowBolt", player->spells.at("shadowBolt")->predictDamage()));
                                }
                                else if (player->spells.at("shadowBolt")->hasEnoughMana())
                                {
                                    player->spells.at("shadowBolt")->startCast();
                                }
                            }
                            // Cast Shadowfury
                            if (player->spells.count("shadowfury") > 0 && player->spells.at("shadowfury")->canCast())
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("shadowfury", player->spells.at("shadowfury")->predictDamage()));
                                }
                                else if (player->spells.at("shadowfury")->hasEnoughMana())
                                {
                                    player->spells.at("shadowfury")->startCast();
                                }
                            }
                            // Cast filler spell if sim is not choosing the rotation for the user
                            if (!player->settings->simChoosingRotation && player->spells.at(player->filler)->ready())
                            {
                                player->spells.at(player->filler)->startCast();
                            }
                            // Cast Dark Pact/Life Tap if nothing else is possible and the sim is not choosing the rotation for the user
                            if (!player->settings->simChoosingRotation && player->spells.count("darkPact") > 0 && player->spells.at("darkPact")->ready())
                            {
                                player->spells.at("darkPact")->startCast();
                            }
                            if (!player->settings->simChoosingRotation && player->spells.at("lifeTap")->ready())
                            {
                                player->spells.at("lifeTap")->startCast();
                            }
                        }

                        // If the sim is choosing the rotation for the player then check now which spell would be the best to cast
                        if (player->settings->simChoosingRotation)
                        {
                            std::string maxDamageSpellName = "";
                            double maxDamageSpellValue = 0;

                            for (std::map<std::string, double>::iterator it = predictedDamageOfSpells.begin(); it != predictedDamageOfSpells.end(); it++)
                            {
                                if (it->second > maxDamageSpellValue)
                                {
                                    maxDamageSpellName = it->first;
                                    maxDamageSpellValue = it->second;
                                }
                            }

                            // If a max damage spell was not found or if the max damage spell isn't ready (no mana), then cast Life Tap
                            if (maxDamageSpellName != "" && player->spells.at(maxDamageSpellName)->hasEnoughMana())
                            {
                                player->spells.at(maxDamageSpellName)->startCast();
                            }
                            else
                            {
                                player->castLifeTapOrDarkPact();
                            }
                        }
                    }
                    // AoE (currently just does Seed of Corruption by default)
                    else
                    {
                        if (player->spells.at("seedOfCorruption")->ready())
                        {
                            player->useCooldowns();
                            player->spells.at("seedOfCorruption")->startCast();
                        }
                        else
                        {
                            player->castLifeTapOrDarkPact();
                        }
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

        // End all active auras & dots
        for (std::map<std::string, Aura*>::iterator it = player->auras.begin(); it != player->auras.end(); it++)
        {
            if (it->second->active)
            {
                it->second->fade(true);
            }
        }
        for (std::map<std::string, DamageOverTime*>::iterator it = player->dots.begin(); it != player->dots.end(); it++)
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
    for(std::map<std::string, DamageOverTime*>::iterator it = player->dots.begin(); it != player->dots.end(); it++)
    {
        delete it->second;
    }
}