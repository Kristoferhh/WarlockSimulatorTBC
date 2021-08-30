#pragma once

struct Player;
#include <iostream>

struct AuraStats
{
    int spellPower;
    int hasteRating;
    double manaCostModifier;

    AuraStats(int spellPower, int hasteRating, double manaCostModifier);
};

struct Aura
{
    Player* player;
    AuraStats* stats;
    std::string name;
    std::string varName;
    int duration;
    int durationRemaining;
    int hiddenCooldown;
    double modifier; // ISB
    bool active;
    bool hasDuration;
    bool groupWide; // true if it's an aura that applies to everyone in the group (will apply to pets as well then)

    Aura(Player* player);
    void setup();
    void tick(int time);
    void apply();
    void fade(bool endOfIteration = false);
    void decrementStacks(); // ISB
};

struct ImprovedShadowBolt : public Aura
{
    int stacks;
    int maxStacks;
    double modifier;
    double uptimeSoFar;

    ImprovedShadowBolt(Player* player);
    void apply();
    void fade(bool endOfIteration = false);
    void decrementStacks();
};