#include "aura.h"
#include "player.h"
#include <iostream>
#include "common.h"

AuraStats::AuraStats(int spellPower, int shadowPower, int firePower, int hasteRating, int hastePercent, double manaCostModifier)
    : spellPower(spellPower), shadowPower(shadowPower), firePower(firePower), hasteRating(hasteRating), hastePercent(hastePercent), manaCostModifier(manaCostModifier)
    {}

Aura::Aura(Player* player) : player(player)
{
    durationRemaining = 0;
    hasDuration = true;
    groupWide = false;
    modifier = 1;
    tickTimerRemaining = 0;
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
    else if (!active)
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
            recalculatePetStats = true;
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

CurseOfTheElementsAura::CurseOfTheElementsAura(Player* player) : Aura(player)
{
    name = "Curse of the Elements";
    duration = 300;
    setup();
}

CurseOfRecklessnessAura::CurseOfRecklessnessAura(Player* player) : Aura(player)
{
    name = "Curse of Recklessness";
    duration = 120;
    setup();
}

ShadowTrance::ShadowTrance(Player* player) : Aura(player)
{
    name = "Shadow Trance (Nightfall)";
    duration = 10;
    setup();
}

Flameshadow::Flameshadow(Player* player) : Aura(player)
{
    name = "Flameshadow";
    duration = 15;
    procChance = 5;
    Aura::stats = new AuraStats(0, 135, 0, 0, 0, 0);
    setup();
}

Shadowflame::Shadowflame(Player* player) : Aura(player)
{
    name = "Shadowflame";
    duration = 15;
    procChance = 5;
    Aura::stats = new AuraStats(0, 0, 135, 0, 0, 0);
    setup();
}

SpellstrikeAura::SpellstrikeAura(Player* player) : Aura(player)
{
    name = "Spellstrike";
    duration = 10;
    procChance = 5;
    Aura::stats = new AuraStats(92, 0, 0, 0, 0, 0);
    setup();
}

PowerInfusionAura::PowerInfusionAura(Player* player) : Aura(player)
{
    name = "Power Infusion";
    duration = 15;
    Aura::stats = new AuraStats(0, 0, 0, 0, 20, 0.8);
    setup();
}

EyeOfMagtheridonAura::EyeOfMagtheridonAura(Player* player) : Aura(player)
{
    name = "Eye of Magtheridon";
    duration = 10;
    Aura::stats = new AuraStats(170, 0, 0, 0, 0, 0);
    setup();
}

SextantOfUnstableCurrentsAura::SextantOfUnstableCurrentsAura(Player* player) : Aura(player)
{
    name = "Sextant of Unstable Currents";
    duration = 15;
    Aura::stats = new AuraStats(190, 0, 0, 0, 0, 0);
    setup();
}

QuagmirransEyeAura::QuagmirransEyeAura(Player* player) : Aura(player)
{
    name = "Quagmirran's Eye";
    duration = 6;
    Aura::stats = new AuraStats(0, 0, 0, 320, 0, 0);
    setup();  
}

ShiffarsNexusHornAura::ShiffarsNexusHornAura(Player* player) : Aura(player)
{
    name = "Shiffar's Nexus-Horn";
    duration = 10;
    Aura::stats = new AuraStats(225, 0, 0, 0, 0, 0);
    setup();
}

ManaEtched4SetAura::ManaEtched4SetAura(Player* player) : Aura(player)
{
    name = "Mana-Etched 4-Set Bonus";
    duration = 15;
    procChance = 2;
    Aura::stats = new AuraStats(110, 0, 0, 0, 0, 0);
    setup();
}

DestructionPotionAura::DestructionPotionAura(Player* player) : Aura(player)
{
    name = "Destruction Potion";
    duration = 15;
    setup();
}

void DestructionPotionAura::apply()
{
    if (!active)
    {
        if (player->shouldWriteToCombatLog())
        {
            int currentSpellPower = player->getSpellPower();
            std::string msg = "Spell Power + 120 (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower + 120) + ")";
            std::string msg2 = "Crit Chance + 2% (" + std::to_string(round(player->stats->critChance * 100) / 100) + "% -> " + std::to_string(round((player->stats->critChance + 2) * 100) / 100) + "%)";
            player->combatLog(msg);
            player->combatLog(msg2);
        }
        player->stats->spellPower += 120;
        player->stats->critChance += 2;
        /*if (player->pet != NULL)
        {
            player->pet->calculateStatsFromPlayer();
        }*/
    }
    Aura::apply();
}

void DestructionPotionAura::fade(bool endOfIteration)
{
    if (active)
    {
        if (!endOfIteration && player->shouldWriteToCombatLog())
        {
            int currentSpellPower = player->getSpellPower();
            std::string msg = "Spell Power - 120 (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower - 120) + ")";
            std::string msg2 = "Crit Chance - 2% (" + std::to_string(round(player->stats->critChance * 100) / 100) + "% -> " + std::to_string(round((player->stats->critChance - 2) * 100) / 100) + "%)";
            player->combatLog(msg);
            player->combatLog(msg2);
        }
        player->stats->spellPower -= 120;
        player->stats->critChance -= 2;
        /*if (!endOfIteration && player->pet != NULL)
        {
            player->pet->calculateStatsFromPlayer();
        }*/
    }
    Aura::fade(endOfIteration);
}

BloodFuryAura::BloodFuryAura(Player* player) : Aura(player)
{
    name = "Blood Fury";
    duration = 15;
    Aura::stats = new AuraStats(140, 0, 0, 0, 0, 0);
    setup();
}

BloodlustAura::BloodlustAura(Player* player) : Aura(player)
{
    name = "Bloodlust";
    duration = 40;
    groupWide = true;
    hasteModifier = 30;
    Aura::stats = new AuraStats(0, 0, 0, 0, 30, 0);
    setup();
}

DrumsOfBattleAura::DrumsOfBattleAura(Player* player) : Aura(player)
{
    name = "Drums of Battle";
    duration = 30;
    groupWide = true;
    Aura::stats = new AuraStats(0, 0, 0, 80, 0, 0);
    setup();
}

DrumsOfWarAura::DrumsOfWarAura(Player* player) : Aura(player)
{
    name = "Drums of War";
    duration = 30;
    groupWide = true;
    Aura::stats = new AuraStats(30, 0, 0, 0, 0, 0);
    setup();
}

DrumsOfRestorationAura::DrumsOfRestorationAura(Player* player) : Aura(player)
{
    name = "Drums of Restoration";
    duration = 15;
    groupWide = true;
    tickTimerTotal = 3;
    ticksRemaining = 0;
    ticksTotal = duration / tickTimerTotal;
    manaGain = 600.0 / (duration / tickTimerTotal);
    setup();
}

void DrumsOfRestorationAura::apply()
{
    tickTimerRemaining = tickTimerTotal;
    ticksRemaining = ticksTotal;
    Aura::apply();
}

void DrumsOfRestorationAura::tick(int t)
{
    if (tickTimerRemaining == 0)
    {
        int currentMana = player->stats->mana;
        player->stats->mana = std::min(player->stats->maxMana, player->stats->mana + manaGain);
        int manaGained = player->stats->mana - currentMana;
        if (player->shouldWriteToCombatLog())
        {
            std::string msg = "Player gains " + std::to_string(manaGained) + " mana from Drums of Restoration (" + std::to_string(currentMana) + " -> " + std::to_string(player->stats->mana) + ")";
            player->combatLog(msg);
        }
        // todo pet

        ticksRemaining--;
        tickTimerRemaining = tickTimerTotal;

        if (ticksRemaining <= 0)
        {
            Aura::fade();
        }
    }
}

AshtongueTalismanOfShadowsAura::AshtongueTalismanOfShadowsAura(Player* player) : Aura(player)
{
    name = "Ashtongue Talisman of Shadows";
    duration = 5;
    procChance = 20;
    Aura::stats = new AuraStats(220, 0, 0, 0, 0, 0);
    setup();
}

DarkmoonCardCrusadeAura::DarkmoonCardCrusadeAura(Player* player) : Aura(player)
{
    name = "Darkmoon Card: Crusade";
    duration = 10;
    maxStacks = 10;
    stacks = 0;
    spellPowerPerStack = 8;
    setup();
}

void DarkmoonCardCrusadeAura::apply()
{
    if (stacks < maxStacks)
    {
        if (player->shouldWriteToCombatLog())
        {
            int currentSpellPower = player->getSpellPower();
            std::string msg = "Spell Power + " + std::to_string(spellPowerPerStack) + " (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower + spellPowerPerStack) + ")";
            player->combatLog(msg);
        }
        player->stats->spellPower += spellPowerPerStack;
        stacks++;
        /*if (player->pet != NULL)
        {
            player->pet->calculateStatsFromPlayer();
        }*/
    }
    Aura::apply();
}

void DarkmoonCardCrusadeAura::fade(bool endOfIteration)
{
    if (!endOfIteration)
    {
        if (player->shouldWriteToCombatLog())
        {
            int currentSpellPower = player->getSpellPower();
            std::string msg = "Spell Power - " + std::to_string(spellPowerPerStack) + " (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower - spellPowerPerStack) + ")";
            player->combatLog(msg);
        }
        /*if (player->pet != NULL)
        {
            player->pet->calculateStatsFromPlayer();
        }*/
    }
    player->stats->spellPower -= spellPowerPerStack * stacks;
    stacks = 0;
    Aura::fade(endOfIteration);
}

TheLightningCapacitorAura::TheLightningCapacitorAura(Player* player) : Aura(player)
{
    name = "The Lightning Capacitor";
    hasDuration = false;
    stacks = 0;
    maxStacks = 3;
    setup();
}

void TheLightningCapacitorAura::apply()
{
    if (stacks < maxStacks)
    {
        stacks++;
    }
    active = true;

    if (player->shouldWriteToCombatLog())
    {
        std::string msg = name + " + 1 stack (" + std::to_string(stacks) + ")";
        player->combatLog(msg);
    }
}

void TheLightningCapacitorAura::fade(bool endOfIteration)
{
    stacks = 0;
    Aura::fade(endOfIteration);
}

BandOfTheEternalSageAura::BandOfTheEternalSageAura(Player* player) : Aura(player)
{
    name = "Band of the Eternal Sage";
    duration = 10;
    Aura::stats = new AuraStats(95, 0, 0, 0, 0, 0);
    setup();
}

BladeOfWizardryAura::BladeOfWizardryAura(Player* player) : Aura(player)
{
    name = "Blade of Wizardry";
    duration = 6;
    procChance = 15;
    Aura::stats = new AuraStats(0, 0, 0, 280, 0, 0);
    setup();
}

ShatteredSunpendantOfAcumenAura::ShatteredSunpendantOfAcumenAura(Player* player) : Aura(player)
{
    name = "Shattered Sun Pendant of Acumen (Aldor)";
    duration = 10;
    Aura::stats = new AuraStats(120, 0, 0, 0, 0, 0);
    setup();
}

RobeOfTheElderScribesAura::RobeOfTheElderScribesAura(Player* player) : Aura(player)
{
    name = "Robe of the Elder Scribes";
    duration = 10;
    Aura::stats = new AuraStats(130, 0, 0, 0, 0, 0);
    setup();
}

MysticalSkyfireDiamondAura::MysticalSkyfireDiamondAura(Player* player) : Aura(player)
{
    name = "Mystical Skyfire Diamond";
    duration = 4;
    Aura::stats = new AuraStats(0, 0, 0, 320, 0, 0);
    setup();
}

AmplifyCurseAura::AmplifyCurseAura(Player* player) : Aura(player)
{
    name = "Amplify Curse";
    duration = 30;
    setup();
};

WrathOfCenariusAura::WrathOfCenariusAura(Player* player) : Aura(player)
{
    name = "Wrath of Cenarius";
    duration = 10;
    procChance = 5;
    Aura::stats = new AuraStats(132, 0, 0, 0, 0, 0);
    setup();
}

InnervateAura::InnervateAura(Player* player) : Aura(player)
{
    name = "Innervate";
    duration = 20;
    setup();
}