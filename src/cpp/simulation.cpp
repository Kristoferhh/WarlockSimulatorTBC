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
    for (std::vector<Spell*>::iterator it = player->spells.begin(); it != player->spells.end(); it++)
    {
        if ((*it)->cooldownRemaining > 0 && (*it)->cooldownRemaining < time)
        {
            time = (*it)->cooldownRemaining;
        }
    }

    // Auras
    for (std::vector<Aura*>::iterator it = player->auras.begin(); it != player->auras.end(); it++)
    {
        if ((*it)->active && (*it)->hasDuration && (*it)->durationRemaining < time)
        {
            time = (*it)->durationRemaining;
        }
    }

    // Dots
    for (std::vector<DamageOverTime*>::iterator it = player->dots.begin(); it != player->dots.end(); it++)
    {
        if ((*it)->active && (*it)->tickTimerRemaining < time)
        {
            time = (*it)->tickTimerRemaining;
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
    for (std::vector<Aura*>::iterator it = player->auras.begin(); it != player->auras.end(); it++)
    {
        if ((*it)->active && (*it)->hasDuration)
        {
            (*it)->tick(time);
        }
    }

    // Dots
    for (std::vector<DamageOverTime*>::iterator it = player->dots.begin(); it != player->dots.end(); ++it)
    {
        if ((*it)->tickTimerRemaining > 0)
        {
            (*it)->tick(time);
        }
    }

    // Spells
    for (std::vector<Spell*>::iterator it = player->spells.begin(); it != player->spells.end(); ++it)
    {
        if ((*it)->cooldownRemaining > 0 || (*it)->casting)
        {
            (*it)->tick(time);
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
        if (player->stats->mp5 > 0 || player->fiveSecondRuleTimer <= 0 || (player->getSpell("innervate") != nullptr && player->getAura("innervate")->active))
        {
            bool innervateExists = player->getSpell("innervate") != nullptr;
            bool innervateActive = innervateExists && player->getAura("innervate")->active;
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
            if (player->getSpell("drumsOfBattle") != nullptr && !player->getAura("drumsOfBattle")->active && player->getSpell("drumsOfBattle")->ready())
            {
                player->getSpell("drumsOfBattle")->startCast();
            }
            else if (player->getSpell("drumsOfWar") != nullptr && !player->getAura("drumsOfWar")->active && player->getSpell("drumsOfWar")->ready())
            {
                player->getSpell("drumsOfWar")->startCast();
            }
            else if (player->getSpell("drumsOfRestoration") != nullptr && !player->getAura("drumsOfRestoration")->active && player->getSpell("drumsOfRestoration")->ready())
            {
                player->getSpell("drumsOfRestoration")->startCast();
            }

            // Player
            if (player->castTimeRemaining <= 0)
            {
                // Spells not on the GCD
                // Demonic Rune
                if ((player->fightTime > 5 || player->stats->mp5 == 0) && player->getSpell("demonicRune") != nullptr && (player->stats->maxMana - player->stats->mana) > player->getSpell("demonicRune")->avgManaValue && player->getSpell("demonicRune")->ready())
                {
                    player->getSpell("demonicRune")->startCast();
                }
                // Super Mana Potion
                if ((player->fightTime > 5 || player->stats->mp5 == 0) && player->getSpell("superManaPotion") != nullptr && (player->stats->maxMana - player->stats->mana) > player->getSpell("superManaPotion")->avgManaValue && player->getSpell("superManaPotion")->ready())
                {
                    player->getSpell("superManaPotion")->startCast();
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
                            predictedDamageOfSpells.insert(std::make_pair("shadowBolt", player->getSpell("shadowBolt")->predictDamage()));
                            predictedDamageOfSpells.insert(std::make_pair("incinerate", player->getSpell("incinerate")->predictDamage()));
                            predictedDamageOfSpells.insert(std::make_pair("searingPain", player->getSpell("searingPain")->predictDamage()));
                        }

                        //todo finishers

                        // Cast selected curse if it's either CoR or CoE and it's not up
                        if (player->curse != "" && (player->curse == "curseOfRecklessness" || player->curse == "curseOfTheElements") && !player->getAura(player->curse)->active && player->getSpell(player->curse)->ready())
                        {
                            player->getSpell(player->curse)->startCast();
                        }
                        else
                        {
                            player->useCooldowns();
                            // Cast Curse of Doom if there's more than 60 seconds remaining
                            if (timeRemaining > 60 && player->curse == "curseOfDoom" && !player->getDot("curseOfDoom")->active && player->getSpell("curseOfDoom")->canCast())
                            {
                                // If the sim is choosing the rotation for the player then predict the damage of the spell and put it in the map
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair(player->getSpell("curseOfDoom")->varName, player->getSpell("curseOfDoom")->predictDamage()));
                                }
                                // Else if the player is choosing the rotation themselves then just cast the highest priority spell that needs to be cast
                                else if (player->getSpell(player->curse)->hasEnoughMana())
                                {
                                    if (player->getSpell("amplifyCurse") != nullptr && player->getSpell("amplifyCurse")->ready())
                                    {
                                        player->getSpell("amplifyCurse")->startCast();
                                    }
                                    player->getSpell("curseOfDoom")->startCast();
                                }
                            }
                            // Cast Curse of Agony if CoA is the selected curse or if Curse of Doom is the selected curse and there's less than 60 seconds remaining of the fight
                            if (player->curse != "" && player->getDot("curseOfAgony") != nullptr && !player->getDot("curseOfAgony")->active && timeRemaining > player->getDot("curseOfAgony")->minimumDuration && ((player->curse == "curseOfDoom" && !player->getDot("curseOfDoom")->active && (player->getSpell("curseOfDoom")->cooldownRemaining > player->getDot("curseOfAgony")->minimumDuration || timeRemaining < 60)) || (player->curse == "curseOfAgony" && player->getSpell("curseOfAgony")->canCast())))
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("curseOfAgony", player->getSpell("curseOfAgony")->predictDamage()));
                                }
                                else if (player->getSpell("curseOfAgony")->hasEnoughMana())
                                {
                                    if (player->getSpell("amplifyCurse") != nullptr && player->getSpell("amplifyCurse")->ready())
                                    {
                                        player->getSpell("amplifyCurse")->startCast();
                                    }
                                    player->getSpell("curseOfAgony")->startCast();
                                }
                            }
                            // Cast Corruption if Corruption isn't up or if it will expire before the cast finishes (if no instant Corruption)
                            if (player->getSpell("corruption") != nullptr && (!player->getDot("corruption")->active || (player->getDot("corruption")->ticksRemaining == 1 && player->getDot("corruption")->tickTimerRemaining < player->getSpell("corruption")->getCastTime())) && player->getSpell("corruption")->canCast() && (timeRemaining - player->getSpell("corruption")->getCastTime()) >= player->getDot("corruption")->minimumDuration)
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("corruption", player->getSpell("corruption")->predictDamage()));
                                }
                                else if (player->getSpell("corruption")->hasEnoughMana())
                                {
                                    player->getSpell("corruption")->startCast();
                                }
                            }
                            // Cast Shadow Bolt if Shadow Trance (Nightfall) is active and Corruption is active as well to avoid potentially wasting another Nightfall proc
                            if (player->getSpell("shadowBolt") != nullptr && player->getSpell("shadowTrance") != nullptr && player->getAura("shadowTrance")->active && player->getDot("corruption")->active && player->getSpell("shadowBolt")->canCast())
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    // todo: check if the spell is already in the array before adding it
                                    predictedDamageOfSpells.insert(std::make_pair("shadowBolt", player->getSpell("shadowBolt")->predictDamage()));
                                }
                                else if (player->getSpell("shadowBolt")->hasEnoughMana())
                                {
                                    player->getSpell("shadowBolt")->startCast();
                                }
                            }
                            // Cast Unstable Affliction if it's not up or if it's about to expire
                            if (player->getSpell("unstableAffliction") != nullptr && player->getSpell("unstableAffliction")->canCast() && (!player->getDot("unstableAffliction")->active || (player->getDot("unstableAffliction")->ticksRemaining == 1 && player->getDot("unstableAffliction")->tickTimerRemaining < player->getSpell("unstableAffliction")->getCastTime())) && (timeRemaining - player->getSpell("unstableAffliction")->getCastTime()) >= player->getDot("unstableAffliction")->minimumDuration)
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("unstableAffliction", player->getSpell("unstableAffliction")->predictDamage()));
                                }
                                else if (player->getSpell("unstableAffliction")->hasEnoughMana())
                                {
                                    player->getSpell("unstableAffliction")->startCast();
                                }
                            }
                            // Cast Siphon Life if it's not up (todo: add option to only cast it while ISB is active if not using custom ISB uptime %)
                            if (player->getSpell("siphonLife") != nullptr && !player->getDot("siphonLife")->active && player->getSpell("siphonLife")->canCast() && timeRemaining >= player->getDot("siphonLife")->minimumDuration)
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("siphonLife", player->getSpell("siphonLife")->predictDamage()));
                                }
                                else if (player->getSpell("siphonLife")->hasEnoughMana())
                                {
                                    player->getSpell("siphonLife")->startCast();
                                }
                            }
                            // Cast Immolate if it's not up or about to expire
                            if (player->getSpell("immolate") != nullptr && player->getSpell("immolate")->canCast() && (!player->getDot("immolate")->active || (player->getDot("immolate")->ticksRemaining == 1 && player->getDot("immolate")->tickTimerRemaining < player->getSpell("immolate")->getCastTime())) && (timeRemaining - player->getSpell("immolate")->getCastTime()) >= player->getDot("immolate")->minimumDuration)
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("immolate", player->getSpell("immolate")->predictDamage()));
                                }
                                else if (player->getSpell("immolate")->hasEnoughMana())
                                {
                                    player->getSpell("immolate")->startCast();
                                }
                            }
                            // Cast Shadow Bolt if Shadow Trance (Nightfall) is active
                            if (player->getSpell("shadowBolt") != nullptr && player->getSpell("shadowTrance") != nullptr && player->getAura("shadowTrance")->active && player->getSpell("shadowBolt")->canCast())
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("shadowBolt", player->getSpell("shadowBolt")->predictDamage()));
                                }
                                else if (player->getSpell("shadowBolt")->hasEnoughMana())
                                {
                                    player->getSpell("shadowBolt")->startCast();
                                }
                            }
                            // Cast Shadowfury
                            if (player->getSpell("shadowfury") != nullptr && player->getSpell("shadowfury")->canCast())
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair("shadowfury", player->getSpell("shadowfury")->predictDamage()));
                                }
                                else if (player->getSpell("shadowfury")->hasEnoughMana())
                                {
                                    player->getSpell("shadowfury")->startCast();
                                }
                            }
                            // Cast filler spell if sim is not choosing the rotation for the user
                            if (!player->settings->simChoosingRotation && player->getSpell(player->filler)->ready())
                            {
                                player->getSpell(player->filler)->startCast();
                            }
                            // Cast Dark Pact/Life Tap if nothing else is possible and the sim is not choosing the rotation for the user
                            if (!player->settings->simChoosingRotation && player->getSpell("darkPact") != nullptr && player->getSpell("darkPact")->ready())
                            {
                                player->getSpell("darkPact")->startCast();
                            }
                            if (!player->settings->simChoosingRotation && player->getSpell("lifeTap")->ready())
                            {
                                player->getSpell("lifeTap")->startCast();
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
                            if (maxDamageSpellName != "" && player->getSpell(maxDamageSpellName)->hasEnoughMana())
                            {
                                player->getSpell(maxDamageSpellName)->startCast();
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
                        if (player->getSpell("seedOfCorruption")->ready())
                        {
                            player->useCooldowns();
                            player->getSpell("seedOfCorruption")->startCast();
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
        for (std::vector<Aura*>::iterator it = player->auras.begin(); it != player->auras.end(); it++)
        {
            if ((*it)->active)
            {
                (*it)->fade(true);
            }
        }
        for (std::vector<DamageOverTime*>::iterator it = player->dots.begin(); it != player->dots.end(); it++)
        {
            if ((*it)->active)
            {
                (*it)->fade(true);
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
    for(std::vector<Spell*>::iterator it = player->spells.begin(); it != player->spells.end(); it++)
    {
        delete (*it);
    }
    for(std::vector<Aura*>::iterator it = player->auras.begin(); it != player->auras.end(); it++)
    {
        delete (*it);
    }
    for(std::vector<DamageOverTime*>::iterator it = player->dots.begin(); it != player->dots.end(); it++)
    {
        delete (*it);
    }
}