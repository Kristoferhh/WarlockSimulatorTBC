#pragma once

struct AuraStats
{
    int spellPower;
    int hasteRating;
    int shadowPower;
    int firePower;
    double hastePercent;
    double manaCostModifier;

    AuraStats(int spellPower, int shadowPower, int firePower, int hasteRating, double hastePercent, double manaCostModifier);
};