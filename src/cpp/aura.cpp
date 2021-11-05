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
    stacks = 0;
    tickTimerRemaining = 0;
    active = false;
}

void Aura::setup()
{
    if (player->combatLogBreakdown.count(name) == 0)
    {
        player->combatLogBreakdown.insert(std::make_pair(name, new CombatLogBreakdown(name)));
    }
}

void Aura::tick(double t)
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
        player->combatLog(name + " refreshed");
    }
    else if (!active)
    {
        bool recalculatePetStats = false;
        player->combatLogBreakdown.at(name)->appliedAt = player->fightTime;

        if (stats != NULL && stats->spellPower > 0)
        {
            if (player->shouldWriteToCombatLog())
            {
                int currentSpellPower = player->getSpellPower();
                player->combatLog("Spell Power + " + std::to_string(stats->spellPower) + " (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower + stats->spellPower) + ")");
            }
            player->stats->spellPower += stats->spellPower;
            recalculatePetStats = true;
        }
        if (stats != NULL && stats->shadowPower > 0)
        {
            if (player->shouldWriteToCombatLog())
            {
                int currentShadowPower = player->stats->shadowPower;
                player->combatLog("Shadow Power + " + std::to_string(stats->shadowPower) + " (" + std::to_string(currentShadowPower) + " -> " + std::to_string(currentShadowPower + stats->shadowPower) + ")");
            }
            player->stats->shadowPower += stats->shadowPower;
            recalculatePetStats = true;
        }
        if (stats != NULL && stats->firePower > 0)
        {
            if (player->shouldWriteToCombatLog())
            {
                int currentFirePower = player->stats->firePower;
                player->combatLog("Fire Power + " + std::to_string(stats->firePower) + " (" + std::to_string(currentFirePower) + " -> " + std::to_string(currentFirePower + stats->firePower) + ")");
            }
            player->stats->firePower += stats->firePower;
            recalculatePetStats = true;
        }
        if (stats != NULL && stats->hasteRating > 0)
        {
            if (player->shouldWriteToCombatLog())
            {
                int currentHasteRating = player->stats->hasteRating;
                player->combatLog("Haste Rating + " + std::to_string(stats->hasteRating) + " (" + std::to_string(currentHasteRating) + " -> " + std::to_string(currentHasteRating + stats->hasteRating) + ")");
            }
            player->stats->hasteRating += stats->hasteRating;
        }
        if (stats != NULL && stats->hastePercent > 0)
        {
            if (player->shouldWriteToCombatLog())
            {
                player->combatLog("Haste % * " + std::to_string(stats->hastePercent) + " (" + std::to_string((player->stats->hastePercent - 1) * 100) + " -> " + std::to_string((player->stats->hastePercent * (1 + (stats->hastePercent / 100)) - 1) * 100) + ")");
            }
            player->stats->hastePercent *= (1 + stats->hastePercent / 100.0);
            if (player->pet != NULL && groupWide)
            {
                player->pet->stats->hastePercent *= (1 + stats->hastePercent / 100.0);
            }
        }
        if (stats != NULL && stats->manaCostModifier > 0)
        {
            if (player->shouldWriteToCombatLog())
            {
                double currentmanaCostModifier = player->stats->manaCostModifier;
                player->combatLog("Mana Cost Modifier * " + truncateTrailingZeros(std::to_string(stats->manaCostModifier), 2) + " (" + truncateTrailingZeros(std::to_string(currentmanaCostModifier), 2) + " -> " + truncateTrailingZeros(std::to_string(currentmanaCostModifier * stats->manaCostModifier), 2) + ")");
            }
            player->stats->manaCostModifier *= stats->manaCostModifier;
        }
        
        if (recalculatePetStats && player->pet != NULL)
        {
            player->pet->calculateStatsFromPlayer();
        }
        if (player->shouldWriteToCombatLog())
        {
            player->combatLog(name + " applied");
        }

        active = true;
    }

    player->combatLogBreakdown.at(name)->count++;
    durationRemaining = duration;
}

void Aura::fade()
{
    if (!active)
    {
        player->throwError("Attempting to fade " + name + " when it isn't active");
    }

    active = false;
    bool recalculatePetStats = false;

    if (stats != NULL && stats->spellPower > 0)
    {
        if (player->shouldWriteToCombatLog())
        {
            int currentSpellPower = player->getSpellPower();
            player->combatLog("Spell Power - " + std::to_string(stats->spellPower) + " (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower - stats->spellPower) + ")");
        }

        player->stats->spellPower -= stats->spellPower;
        recalculatePetStats = true;
    }
    if (stats != NULL && stats->shadowPower > 0)
    {
        if (player->shouldWriteToCombatLog())
        {
            int currentShadowPower = player->stats->shadowPower;
            player->combatLog("Shadow Power - " + std::to_string(stats->shadowPower) + " (" + std::to_string(currentShadowPower) + " -> " + std::to_string(currentShadowPower - stats->shadowPower) + ")");
        }
        player->stats->shadowPower -= stats->shadowPower;
        recalculatePetStats = true;
    }
    if (stats != NULL && stats->firePower > 0)
    {
        if (player->shouldWriteToCombatLog())
        {
            int currentFirePower = player->stats->firePower;
            player->combatLog("Fire Power - " + std::to_string(stats->firePower) + " (" + std::to_string(currentFirePower) + " -> " + std::to_string(currentFirePower - stats->firePower) + ")");
        }
        player->stats->firePower -= stats->firePower;
        recalculatePetStats = true;
    }
    if (stats != NULL && stats->hasteRating > 0)
    {
        if (player->shouldWriteToCombatLog())
        {
            int currentHasteRating = player->stats->hasteRating;
            player->combatLog("Haste Rating - " + std::to_string(stats->hasteRating) + " (" + std::to_string(currentHasteRating) + " -> " + std::to_string(currentHasteRating - stats->hasteRating) + ")");
        }
        player->stats->hasteRating -= stats->hasteRating;
    }
    if (stats != NULL && stats->hastePercent > 0)
    {
        if (player->shouldWriteToCombatLog())
        {
            player->combatLog("Haste % / " + std::to_string(stats->hastePercent) + "% (" + std::to_string((player->stats->hastePercent - 1) * 100) + " -> " + std::to_string((player->stats->hastePercent / (1 + (stats->hastePercent / 100)) - 1) * 100) + ")");
        }
        player->stats->hastePercent /= (1 + stats->hastePercent / 100.0);
        if (player->pet != NULL && groupWide)
        {
            player->pet->stats->hastePercent /= (1 + stats->hastePercent / 100.0);
        }
    }
    if (stats != NULL && stats->manaCostModifier > 0)
    {
        if (player->shouldWriteToCombatLog())
        {
            double currentmanaCostModifier = player->stats->manaCostModifier;
            player->combatLog("Mana Cost Modifier / " + truncateTrailingZeros(std::to_string(stats->manaCostModifier), 2) + " (" + truncateTrailingZeros(std::to_string(currentmanaCostModifier), 2) + " -> " + truncateTrailingZeros(std::to_string(currentmanaCostModifier / stats->manaCostModifier), 2) + ")");
        }
        player->stats->manaCostModifier /= stats->manaCostModifier;
    }

    if (recalculatePetStats && player->pet != NULL)
    {
        player->pet->calculateStatsFromPlayer();
    }

    if (player->shouldWriteToCombatLog())
    {
        player->combatLog(name + " faded");
    }

    double auraUptime = player->fightTime - player->combatLogBreakdown.at(name)->appliedAt;
    player->combatLogBreakdown.at(name)->uptime += auraUptime;
}

void Aura::decrementStacks() {}

ImprovedShadowBoltAura::ImprovedShadowBoltAura(Player* player) : Aura(player)
{
    name = "Improved Shadow Bolt";
    duration = 12;
    stacks = 0;
    maxStacks = 4;
    Aura::modifier = 1 + player->talents->improvedShadowBolt * 0.04;
    uptimeSoFar = 0;
    setup();
}

void ImprovedShadowBoltAura::apply()
{
    Aura::apply();
    stacks = maxStacks;
}

void ImprovedShadowBoltAura::decrementStacks()
{
    stacks--;

    if (stacks <= 0)
    {
        fade();
    }
    else if (player->shouldWriteToCombatLog())
    {
        player->combatLog(name + " (" + std::to_string(stacks) + ")");
    }
}

void ImprovedShadowBoltAura::fade()
{
    Aura::fade();
    uptimeSoFar = player->combatLogBreakdown.at(name)->uptime / player->totalDuration;
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

ShadowTranceAura::ShadowTranceAura(Player* player) : Aura(player)
{
    name = "Shadow Trance (Nightfall)";
    duration = 10;
    setup();
}

FlameshadowAura::FlameshadowAura(Player* player) : Aura(player)
{
    name = "Flameshadow";
    duration = 15;
    procChance = 5;
    Aura::stats = std::make_unique<AuraStats>(0, 135, 0, 0, 0, 0);
    setup();
}

ShadowflameAura::ShadowflameAura(Player* player) : Aura(player)
{
    name = "Shadowflame";
    duration = 15;
    procChance = 5;
    Aura::stats = std::make_unique<AuraStats>(0, 0, 135, 0, 0, 0);
    setup();
}

SpellstrikeAura::SpellstrikeAura(Player* player) : Aura(player)
{
    name = "Spellstrike";
    duration = 10;
    procChance = 5;
    Aura::stats = std::make_unique<AuraStats>(92, 0, 0, 0, 0, 0);
    setup();
}

PowerInfusionAura::PowerInfusionAura(Player* player) : Aura(player)
{
    name = "Power Infusion";
    duration = 15;
    Aura::stats = std::make_unique<AuraStats>(0, 0, 0, 0, 20, 0.8);
    setup();
}

EyeOfMagtheridonAura::EyeOfMagtheridonAura(Player* player) : Aura(player)
{
    name = "Eye of Magtheridon";
    duration = 10;
    Aura::stats = std::make_unique<AuraStats>(170, 0, 0, 0, 0, 0);
    setup();
}

SextantOfUnstableCurrentsAura::SextantOfUnstableCurrentsAura(Player* player) : Aura(player)
{
    name = "Sextant of Unstable Currents";
    duration = 15;
    Aura::stats = std::make_unique<AuraStats>(190, 0, 0, 0, 0, 0);
    setup();
}

QuagmirransEyeAura::QuagmirransEyeAura(Player* player) : Aura(player)
{
    name = "Quagmirran's Eye";
    duration = 6;
    Aura::stats = std::make_unique<AuraStats>(0, 0, 0, 320, 0, 0);
    setup();  
}

ShiffarsNexusHornAura::ShiffarsNexusHornAura(Player* player) : Aura(player)
{
    name = "Shiffar's Nexus-Horn";
    duration = 10;
    Aura::stats = std::make_unique<AuraStats>(225, 0, 0, 0, 0, 0);
    setup();
}

ManaEtched4SetAura::ManaEtched4SetAura(Player* player) : Aura(player)
{
    name = "Mana-Etched 4-Set Bonus";
    duration = 15;
    procChance = 2;
    Aura::stats = std::make_unique<AuraStats>(110, 0, 0, 0, 0, 0);
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
            double currentCritChance = player->getCritChance(SpellType::DESTRUCTION);
            player->combatLog("Spell Power + 120 (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower + 120) +  + ")"")");
            player->combatLog("Crit Chance + 2% (" + std::to_string(round(currentCritChance * 100) / 100) + "% -> " + std::to_string(round((currentCritChance + 2) * 100) / 100) + "%)");
        }
        player->stats->spellPower += 120;
        player->stats->critChance += 2;
        if (player->pet != NULL)
        {
            player->pet->calculateStatsFromPlayer();
        }
    }
    Aura::apply();
}

void DestructionPotionAura::fade()
{
    if (active)
    {
        if (player->shouldWriteToCombatLog())
        {
            int currentSpellPower = player->getSpellPower();
            double currentCritChance = player->getCritChance(SpellType::DESTRUCTION);
            player->combatLog("Spell Power - 120 (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower - 120) +  + ")"")");
            player->combatLog("Crit Chance - 2% (" + std::to_string(round(currentCritChance * 100) / 100) + "% -> " + std::to_string(round((currentCritChance - 2) * 100) / 100) + "%)");
        }
        player->stats->spellPower -= 120;
        player->stats->critChance -= 2;
        if (player->pet != NULL)
        {
            player->pet->calculateStatsFromPlayer();
        }
    }
    Aura::fade();
}

FlameCapAura::FlameCapAura(Player* player) : Aura(player)
{
    name = "Flame Cap";
    duration = 60;
    Aura::stats = std::make_unique<AuraStats>(0, 0, 80, 0, 0, 0);
    setup();
}

BloodFuryAura::BloodFuryAura(Player* player) : Aura(player)
{
    name = "Blood Fury";
    duration = 15;
    Aura::stats = std::make_unique<AuraStats>(140, 0, 0, 0, 0, 0);
    setup();
}

BloodlustAura::BloodlustAura(Player* player) : Aura(player)
{
    name = "Bloodlust";
    duration = 40;
    groupWide = true;
    Aura::stats = std::make_unique<AuraStats>(0, 0, 0, 0, 30, 0);
    setup();
}

DrumsOfBattleAura::DrumsOfBattleAura(Player* player) : Aura(player)
{
    name = "Drums of Battle";
    duration = 30;
    groupWide = true;
    Aura::stats = std::make_unique<AuraStats>(0, 0, 0, 80, 0, 0);
    setup();
}

DrumsOfWarAura::DrumsOfWarAura(Player* player) : Aura(player)
{
    name = "Drums of War";
    duration = 30;
    groupWide = true;
    Aura::stats = std::make_unique<AuraStats>(30, 0, 0, 0, 0, 0);
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

void DrumsOfRestorationAura::tick(double t)
{
    tickTimerRemaining -= t;
    
    if (tickTimerRemaining <= 0)
    {
        int currentMana = player->stats->mana;
        player->stats->mana = std::min(player->stats->maxMana, player->stats->mana + manaGain);
        int manaGained = player->stats->mana - currentMana;
        player->combatLogBreakdown.at(name)->casts++;
        player->addIterationDamageAndMana(name, manaGained, 0);
        if (player->shouldWriteToCombatLog())
        {
            player->combatLog("Player gains " + std::to_string(manaGained) + " mana from Drums of Restoration (" + std::to_string(currentMana) + " -> " + std::to_string(player->stats->mana) + ")" + ")");
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
    Aura::stats = std::make_unique<AuraStats>(220, 0, 0, 0, 0, 0);
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
            player->combatLog("Spell Power + " + std::to_string(spellPowerPerStack) + " (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower + spellPowerPerStack) +  + ")"")");
        }
        player->stats->spellPower += spellPowerPerStack;
        stacks++;
        if (player->pet != NULL)
        {
            player->pet->calculateStatsFromPlayer();
        }
    }
    Aura::apply();
}

void DarkmoonCardCrusadeAura::fade()
{
    if (player->shouldWriteToCombatLog())
    {
        int currentSpellPower = player->getSpellPower();
        player->combatLog("Spell Power - " + std::to_string(spellPowerPerStack) + " (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower - spellPowerPerStack) +  + ")"")");
    }
    if (player->pet != NULL)
    {
        player->pet->calculateStatsFromPlayer();
    }
    player->stats->spellPower -= spellPowerPerStack * stacks;
    stacks = 0;
    Aura::fade();
}

TheLightningCapacitorAura::TheLightningCapacitorAura(Player* player) : Aura(player)
{
    name = "The Lightning Capacitor";
    hasDuration = false;
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
        player->combatLog(name + " + 1 stack (" + std::to_string(stacks) + ")");
    }
}

void TheLightningCapacitorAura::fade()
{
    stacks = 0;
    Aura::fade();
}

BandOfTheEternalSageAura::BandOfTheEternalSageAura(Player* player) : Aura(player)
{
    name = "Band of the Eternal Sage";
    duration = 10;
    Aura::stats = std::make_unique<AuraStats>(95, 0, 0, 0, 0, 0);
    setup();
}

BladeOfWizardryAura::BladeOfWizardryAura(Player* player) : Aura(player)
{
    name = "Blade of Wizardry";
    duration = 6;
    procChance = 15;
    Aura::stats = std::make_unique<AuraStats>(0, 0, 0, 280, 0, 0);
    setup();
}

ShatteredSunPendantOfAcumenAura::ShatteredSunPendantOfAcumenAura(Player* player) : Aura(player)
{
    name = "Shattered Sun Pendant of Acumen (Aldor)";
    duration = 10;
    Aura::stats = std::make_unique<AuraStats>(120, 0, 0, 0, 0, 0);
    setup();
}

RobeOfTheElderScribesAura::RobeOfTheElderScribesAura(Player* player) : Aura(player)
{
    name = "Robe of the Elder Scribes";
    duration = 10;
    Aura::stats = std::make_unique<AuraStats>(130, 0, 0, 0, 0, 0);
    setup();
}

MysticalSkyfireDiamondAura::MysticalSkyfireDiamondAura(Player* player) : Aura(player)
{
    name = "Mystical Skyfire Diamond";
    duration = 4;
    Aura::stats = std::make_unique<AuraStats>(0, 0, 0, 320, 0, 0);
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
    Aura::stats = std::make_unique<AuraStats>(132, 0, 0, 0, 0, 0);
    setup();
}

InnervateAura::InnervateAura(Player* player) : Aura(player)
{
    name = "Innervate";
    duration = 20;
    setup();
}