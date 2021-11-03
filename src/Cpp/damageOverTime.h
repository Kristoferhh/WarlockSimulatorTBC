#pragma once

struct Player; // Can't include player.h cause of circular reference
#include <iostream>
#include "enums.h"
#include "aura.h"

struct DamageOverTime
{
    Player* player;
    SpellSchool school;
    int duration; // Total duration of the dot
    int originalDuration; // Used for T4 4pc since we're increasing the duration by 3 seconds but need to know what the original duration was
    int tickTimerTotal; // Total duration of each tick (default is 3 seconds between ticks)
    double tickTimerRemaining; // Time until next tick
    int ticksRemaining; // Amount of ticks remaining before the dot expires
    int ticksTotal;
    int spellPower; // Spell Power amount when dot was applied
    int minimumDuration; // Minimum time left of the fight for the sim to consider casting the dot
    double dmg;
    double modifier;
    double coefficient;
    double t5BonusModifier; // T5 4pc damage modifier
    bool active;
    bool amplified; // Applied while the Amplify Curse aura was active
    bool isbActive; // Siphon Life
    std::string name;

    DamageOverTime(Player* player);
    void setup();
    virtual void apply();
    virtual double getModifier();
    void fade(bool endOfIteration = false);
    void tick(double time);
    std::vector<double> getConstantDamage();
    double predictDamage();
};

struct CorruptionDot : public DamageOverTime
{
    double getModifier();
    void apply();

    CorruptionDot(Player* player);
};

struct UnstableAfflictionDot : public DamageOverTime
{
    UnstableAfflictionDot(Player* player);
};

struct SiphonLifeDot : public DamageOverTime
{
    SiphonLifeDot(Player* player);
};

struct ImmolateDot : public DamageOverTime
{
    ImmolateDot(Player* player);
    void apply();
};

struct CurseOfAgonyDot : public DamageOverTime
{
    CurseOfAgonyDot(Player* player);
    double getModifier();
};

struct CurseOfDoomDot : public DamageOverTime
{
    CurseOfDoomDot(Player* player);
    double getModifier();
};

