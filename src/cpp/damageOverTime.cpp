#include "damageOverTime.h"
#include "common.h"
#include "player.h"

DamageOverTime::DamageOverTime(Player* player) : Aura(player)
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
    // todo breakdown tables
}

void DamageOverTime::apply()
{
    if (active && player->shouldWriteToCombatLog())
    {
        std::string msg = name + " refreshed before letting it expire";
    }
    else if (!active)
    {
        //todo breakdown table
    }
    bool isActive = active;
    active = true;
    tickTimerRemaining = tickTimerTotal;
    ticksRemaining = ticksTotal;
    int spellPower = player->getSpellPower(school);
    this->spellPower = spellPower;

    if (player->shouldWriteToCombatLog())
    {
        std::string refreshedOrApplied = isActive ? "refreshed" : "applied";
        std::string msg = name + " " + refreshedOrApplied + " (" + std::to_string(spellPower);
        player->combatLog(msg);
    }
    // Siphon Life snapshots the presence of ISB. So if ISB isn't up when it's cast, it doesn't get the benefit even if it comes up later during the duration.
    if (varName == "siphonLife")
    {
        isbActive = !player->settings->usingCustomIsbUptime && player->auras.count("improvedShadowBolt") > 0 && player->auras.at("improvedShadowBolt")->active;
    }
    // Amplify Curse
    if ((varName == "curseOfAgony" || varName == "curseOfDoom") && player->auras.count("amplifyCurse") > 0 && player->auras.at("amplifyCurse")->active)
    {
        amplified = true;
        player->auras.at("amplifyCurse")->fade();
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
    //todo aura uptime
    if (!endOfIteration)
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
    if ((school == SpellSchool::SHADOW && player->auras.count("improvedShadowBolt") > 0 && player->auras.at("improvedShadowBolt")->active && varName != "siphonLife") || (varName == "siphonLife" && isbActive))
    {
        dmgModifier *= player->auras.at("improvedShadowBolt")->modifier;
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

void DamageOverTime::tick(int t)
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
                player->auras.at("shadowTrance")->apply();
            }
        }

        //todo breakdown table
        player->iterationDamage += dmg;
        ticksRemaining--;
        tickTimerRemaining = tickTimerTotal;

        // Ashtongue Talisman of Shadows
        if (varName == "corruption" && player->auras.count("ashtongueTalismanOfShadows") > 0 && random(1, 100) <= player->auras.at("ashtongueTalismanOfShadows")->procChance)
        {
            player->auras.at("ashtongueTalismanOfShadows")->apply();
        }
        // Timbal's Focusing Crystal
        if (player->spells.count("timbalsFocusingCrystal") > 0 && player->spells.at("timbalsFocusingCrystal")->cooldownRemaining <= 0 && random(1, 100) <= player->spells.at("timbalsFocusingCrystal")->procChance)
        {
            player->spells.at("timbalsFocusingCrystal")->startCast();
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
    minimumDuration = 9;
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
    minimumDuration = 9;
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
    minimumDuration = 12;
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
    coefficient = 1.2;
    minimumDuration = 15;
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
    duration = 60;
    tickTimerTotal = 60;
    dmg = 4200;
    school = SpellSchool::SHADOW;
    coefficient = 2;
    setup();
}