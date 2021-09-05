#include "damageOverTime.h"
#include "common.h"
#include "player.h"

DamageOverTime::DamageOverTime(Player* player) : player(player)
{
    duration = 0;
    tickTimerTotal = 3;
    tickTimerRemaining = 0;
    ticksRemaining = 0;
    dmg = 0;
    spellPower = 0;
    modifier = 1;
    active = false;
    coefficient = 0;
    amplified = false;
}

void DamageOverTime::setup()
{
    varName = camelCase(name);
    originalDuration = duration;
    // T4 4pc
    if ((varName == "immolate" || varName == "corruption") && player->sets->t4 >= 4)
    {
        duration += 3;
    }
    ticksTotal = duration / tickTimerTotal;
    if (player->combatLogBreakdown.count(varName) == 0)
    {
        player->combatLogBreakdown.insert(std::make_pair(varName, new CombatLogBreakdown(name)));
    }
}

void DamageOverTime::apply()
{
    if (active && player->shouldWriteToCombatLog())
    {
        std::string msg = name + " refreshed before letting it expire";
    }
    else if (!active)
    {
        player->combatLogBreakdown.at(varName)->appliedAt = player->fightTime;
    }
    bool isActive = active;
    int spellPower = player->getSpellPower(school);
    this->spellPower = spellPower;

    player->combatLogBreakdown.at(varName)->count++;
    active = true;
    tickTimerRemaining = tickTimerTotal;
    ticksRemaining = ticksTotal;

    if (player->shouldWriteToCombatLog())
    {
        std::string refreshedOrApplied = isActive ? "refreshed" : "applied";
        std::string msg = name + " " + refreshedOrApplied + " (" + std::to_string(spellPower) + " Spell Power)";
        player->combatLog(msg);
    }
    // Siphon Life snapshots the presence of ISB. So if ISB isn't up when it's cast, it doesn't get the benefit even if it comes up later during the duration.
    if (varName == "siphonLife")
    {
        isbActive = !player->settings->usingCustomIsbUptime && player->auras->ImprovedShadowBolt != NULL && player->auras->ImprovedShadowBolt->active;
    }
    // Amplify Curse
    if ((varName == "curseOfAgony" || varName == "curseOfDoom") && player->auras->AmplifyCurse != NULL && player->auras->AmplifyCurse->active)
    {
        amplified = true;
        player->auras->AmplifyCurse->fade();
    }
    else
    {
        amplified = false;
    }
}

void DamageOverTime::fade(bool endOfIteration)
{
    active = false;
    tickTimerRemaining = 0;
    ticksRemaining = 0;
    
    double auraUptime = player->fightTime - player->combatLogBreakdown.at(varName)->appliedAt;
    player->combatLogBreakdown.at(varName)->uptime += auraUptime;

    if (!endOfIteration && player->shouldWriteToCombatLog())
    {
        std::string msg = name + " faded";
        player->combatLog(msg);
    }
}

double DamageOverTime::getModifier()
{
    double dmgModifier = modifier;
    if (school == SpellSchool::SHADOW)
    {
        dmgModifier *= player->stats->shadowModifier;
    }
    else if (school == SpellSchool::FIRE)
    {
        dmgModifier *= player->stats->fireModifier;
    }
    // Amplify Curse
    if (amplified)
    {
        dmgModifier *= 1.5;
    }
    // Improved Shadow Bolt
    if ((school == SpellSchool::SHADOW && player->auras->ImprovedShadowBolt != NULL && player->auras->ImprovedShadowBolt->active && varName != "siphonLife") || (varName == "siphonLife" && isbActive))
    {
        dmgModifier *= player->auras->ImprovedShadowBolt->modifier;
    }
    return dmgModifier;
}

double* DamageOverTime::getConstantDamage()
{
    double spellPower = this->spellPower;
    // If the DoT isn't currently active then this.spellPower will be 0, so use the player's current Spell Power
    if (!active)
    {
        spellPower = player->getSpellPower(school);
    }
    double modifier = getModifier();
    double partialResistMultiplier = player->getPartialResistMultiplier(school);
    double dmg = this->dmg;
    // Add the t5 4pc bonus modifier to the base damage
    if ((varName == "corruption" || varName == "immolate") && player->sets->t5 >= 4)
    {
        dmg *= t5BonusModifier;
    }
    dmg += spellPower * coefficient;
    dmg *= modifier * partialResistMultiplier;

    return new double[4] {dmg, spellPower, modifier, partialResistMultiplier};
}

// Predicts how much damage the dot will do over its full duration
double DamageOverTime::predictDamage()
{
    double* constantDamage = getConstantDamage();
    double dmg = constantDamage[0];
    // If it's Corruption or Immolate then divide by the original duration (18s and 15s) and multiply by the durationTotal property
    // This is just for the t4 4pc bonus since their durationTotal property is increased by 3 seconds to include another tick
    // but the damage they do stays the same which assumes the normal duration without the bonus
    if (varName == "corruption" || varName == "immolate")
    {
        dmg /= originalDuration;
        dmg *= duration;
    }
    
    delete constantDamage;
    return dmg;
}

void DamageOverTime::tick(double t)
{
    tickTimerRemaining -= t;

    if (tickTimerRemaining <= 0)
    {
        double* constantDamage = getConstantDamage();
        double dmg = constantDamage[0] / (originalDuration / tickTimerTotal);
        double spellPower = constantDamage[1];
        double modifier = constantDamage[2];
        double partialResistMultiplier = constantDamage[3];

        // Check for Nightfall proc
        if (varName == "corruption" && player->talents->nightfall > 0)
        {
            if (random(1, 100) <= player->talents->nightfall * 2)
            {
                player->auras->ShadowTrance->apply();
            }
        }

        player->combatLogBreakdown.at(varName)->damage += dmg;
        player->iterationDamage += dmg;
        ticksRemaining--;
        tickTimerRemaining = tickTimerTotal;

        if (player->shouldWriteToCombatLog())
        {
            std::string msg = name + " Tick " + truncateTrailingZeros(std::to_string(round(dmg))) + " (" + truncateTrailingZeros(std::to_string(this->dmg)) + " Base Damage - " + truncateTrailingZeros(std::to_string(player->getSpellPower(school))) + " Spell Power - " + truncateTrailingZeros(std::to_string(coefficient), 3) + " Coefficient - " + truncateTrailingZeros(std::to_string(round(modifier * 10000) / 100), 3) + "% Damage Modifier - " + truncateTrailingZeros(std::to_string(round(partialResistMultiplier * 1000) / 10)) + "% Partial Resist Multiplier";
            if (t5BonusModifier > 1)
            {
                msg += " - " + std::to_string(round(t5BonusModifier * 10000) / 100) + "% Base Dmg Modifier (T5 4pc bonus)";
            }
            msg += ")";
            player->combatLog(msg);
        }

        // Ashtongue Talisman of Shadows
        if (varName == "corruption" && player->auras->AshtongueTalismanOfShadows != NULL && random(1, 100) <= player->auras->AshtongueTalismanOfShadows->procChance)
        {
            player->auras->AshtongueTalismanOfShadows->apply();
        }
        // Timbal's Focusing Crystal
        if (player->spells->TimbalsFocusingCrystal != NULL && player->spells->TimbalsFocusingCrystal->cooldownRemaining <= 0 && random(1, 100) <= player->spells->TimbalsFocusingCrystal->procChance)
        {
            player->spells->TimbalsFocusingCrystal->startCast();
        }

        if (ticksRemaining <= 0)
        {
            fade();
        }
    }
}

CorruptionDot::CorruptionDot(Player* player) : DamageOverTime(player)
{
    name = "Corruption";
    duration = 18;
    tickTimerTotal = 3;
    dmg = 900;
    school = SpellSchool::SHADOW;
    coefficient = 0.936 + (0.12 * player->talents->empoweredCorruption);
    minimumDuration = 18;
    t5BonusModifier = 1;
    setup();

    // T3 4pc
    if (player->sets->plagueheart >= 4)
    {
        modifier *= 1.12;
    }
}

double CorruptionDot::getModifier()
{
    double modifier = getModifier();
    if (player->talents->shadowMastery > 0 && player->talents->contagion > 0)
    {
        // Divide away the bonus from Shadow Mastery
        modifier /= (1 + (player->talents->shadowMastery * 0.02));
        // Multiply the modifier with the bonus from Shadow Mastery + Contagion
        modifier *= (1 * (1 + ((player->talents->shadowMastery * 0.02) + (player->talents->contagion / 100))));
    }
    return modifier;
}

void CorruptionDot::apply()
{
    // T5 4pc bonus modifier
    t5BonusModifier = 1;
    DamageOverTime::apply();
}

UnstableAfflictionDot::UnstableAfflictionDot(Player* player) : DamageOverTime(player)
{
    name = "Unstable Affliction";
    duration = 18;
    tickTimerTotal = 3;
    dmg = 1050;
    school = SpellSchool::SHADOW;
    coefficient = 1.2;
    minimumDuration = 18;
    setup();
}

SiphonLifeDot::SiphonLifeDot(Player* player) : DamageOverTime(player)
{
    name = "Siphon Life";
    duration = 30;
    tickTimerTotal = 3;
    dmg = 630;
    school = SpellSchool::SHADOW;
    coefficient = 1;
    minimumDuration = 30;
    setup();
}

ImmolateDot::ImmolateDot(Player* player) : DamageOverTime(player)
{
    name = "Immolate";
    duration = 15;
    tickTimerTotal = 3;
    dmg = 615;
    school = SpellSchool::FIRE;
    coefficient = 0.65;
    minimumDuration = 15;
    t5BonusModifier = 1;
    setup();
}

void ImmolateDot::apply()
{
    // T5 4pc bonus modifier
    t5BonusModifier = 1;
    DamageOverTime::apply();
}

CurseOfAgonyDot::CurseOfAgonyDot(Player* player) : DamageOverTime(player)
{
    name = "Curse of Agony";
    duration = 24;
    tickTimerTotal = 3;
    dmg = 1356;
    school = SpellSchool::SHADOW;
    coefficient = 1.2;
    minimumDuration = 24;
    setup();
}

double CurseOfAgonyDot::getModifier()
{
    double modifier = getModifier();
    // Remove bonus from Shadow Mastery and add bonus from Shadow Mastery + Contagion + Improved Curse of Agony
    modifier /= (1 + (player->talents->shadowMastery * 0.02));
    modifier *= (1 * (1 + ((player->talents->shadowMastery * 0.02) + (player->talents->contagion / 100) + (player->talents->improvedCurseOfAgony * 0.05))));
    return modifier;
}

CurseOfDoomDot::CurseOfDoomDot(Player* player) : DamageOverTime(player)
{
    name = "Curse of Doom";
    duration = 60;
    tickTimerTotal = 60;
    dmg = 4200;
    school = SpellSchool::SHADOW;
    coefficient = 2;
    setup();
}