#pragma once

struct Player;

struct PetStats
{
    Player* player;
    int stamina;
    int intellect;
    int strength;
    int agility;
    int maxMana;
    int attackPower;
    int spellPower;
    int mana;
    int mp5;
    int spirit;
    double meleeCritChance;
    double meleeHitChance;
    double spellCritChance;
    double spellHitChance;
    double spellCritRating;
    double hastePercent;
    double staminaModifier;
    double intellectModifier;
    double strengthModifier;
    double agilityModifier;
    double spiritModifier;
    double attackPowerModifier;
    double damageModifier;

    PetStats(Player* player = nullptr);
};