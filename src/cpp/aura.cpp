#include "aura.h"
#include "player.h"
#include <iostream>
#include "common.h"

Aura::Aura(Player* player) : player(player)
{
    durationRemaining = 0;
    hasDuration = true;
    groupWide = false;
    modifier = 1;
}

void Aura::setup()
{
    varName = camelCase(name);
    //breakdown map
}

void Aura::tick(int t)
{
    if (hasDuration)
    {
        durationRemaining -= t;
        if (durationRemaining <= 0)
        {
            fade();
        }
    }
}

void Aura::apply()
{
    if (active && player->shouldWriteToCombatLog())
    {
        std::string msg = name + " refreshed";
        player->combatLog(msg);
    }
    else
    {
        bool recalculatePetStats = false;
        if (stats->spellPower > 0)
        {
            if (player->shouldWriteToCombatLog())
            {
                int currentSpellPower = player->getSpellPower();
                std::string msg = "Spell Power + " + std::to_string(stats->spellPower) + " (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower + stats->spellPower);
                player->combatLog(msg);
            }
            player->stats->spellPower += stats->spellPower;
            recalculatePetStats = true;
        }
        /*this.player.auraBreakdown[this.varName].appliedAt = this.player.fightTime
        // Add stats from the aura if it has any
        for (const stat in this.stats) {
            if (this.player.stats.hasOwnProperty(stat)) {
            if (stat.toLowerCase().includes('modifier')) {
                this.player.combatLog(stat + ' * ' + this.stats[stat] + ' (' + Math.round(this.player.stats[stat] * 100) / 100 + ' -> ' + Math.round(this.player.stats[stat] * this.stats[stat] * 100) / 100 + ')')
                this.player.stats[stat] *= this.stats[stat]
            } else {
                this.player.combatLog(stat + ' + ' + this.stats[stat] + ' (' + Math.round(this.player.stats[stat]) + ' -> ' + Math.round(this.player.stats[stat] + this.stats[stat]) + ')')
                this.player.stats[stat] += this.stats[stat]
            }
            // Add stats to pet
            if (this.player.pet) {
                if (this.player.pet.stats.hasOwnProperty(stat) && this.groupWide) {
                this.player.pet.stats[stat] += this.stats[stat]
                }
                if (this.player.pet.stats.buffs.hasOwnProperty(stat)) {
                recalculatePetStats = true
                }
            }
        }*/
        
        if (recalculatePetStats)
        {
            //player->pet->calculateStatsFromPlayer();
        }
        if (player->shouldWriteToCombatLog())
        {
            std::string msg = name + " applied";
            player->combatLog(msg);
        }

        active = true;
    }

    //breakdown.count
    durationRemaining = duration;
}

void Aura::fade(bool endOfIteration)
{
    if (endOfIteration)
    {
        hiddenCooldown = 0;
    }

    if (active)
    {
        bool recalculatePetStats = false;
        if (stats->spellPower > 0)
        {
            if (player->shouldWriteToCombatLog())
            {
                int currentSpellPower = player->getSpellPower();
                std::string msg = "Spell Power - " + std::to_string(stats->spellPower) + " (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower - stats->spellPower);
                player->combatLog(msg);
            }
            player->stats->spellPower -= stats->spellPower;

            if (!endOfIteration)
            {
                recalculatePetStats = true;
            }
            //todo remove stats from pet
        }

        active = false;
        if (!endOfIteration)
        {
            if (recalculatePetStats)
            {
                //player->pet->calculateStatsFromPlayer();
            }
            if (player->shouldWriteToCombatLog())
            {
                std::string msg = name + " faded";
                player->combatLog(msg);
            }
        }

        //todo log aura uptime
    }
}

void Aura::decrementStacks() {}

ImprovedShadowBolt::ImprovedShadowBolt(Player* player) : Aura(player)
{
    name = "Improved Shadow Bolt";
    duration = 12;
    stacks = 0;
    maxStacks = 0;
    Aura::modifier = 1 + player->talents->improvedShadowBolt * 0.04;
    uptimeSoFar = 0;
    setup();
}

void ImprovedShadowBolt::apply()
{
    Aura::apply();
    stacks = maxStacks;
}

void ImprovedShadowBolt::decrementStacks()
{
    stacks--;

    if (stacks <= 0)
    {
        fade();
    }
    else if (player->shouldWriteToCombatLog())
    {
        std::string msg = name + " (" + std::to_string(stacks) + ")";
        player->combatLog(msg);
    }
}

void ImprovedShadowBolt::fade(bool endOfIteration)
{
    Aura::fade(endOfIteration);
    // todo update uptimeSoFar
}