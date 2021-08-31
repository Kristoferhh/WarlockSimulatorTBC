#pragma once

struct AuraStats
{
    int spellPower;
    int hasteRating;
    int shadowPower;
    int firePower;
    int hastePercent;
    double manaCostModifier;

    AuraStats(int spellPower, int shadowPower, int firePower, int hasteRating, int hastePercent, double manaCostModifier);
};