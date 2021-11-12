#ifndef WARLOCK_SIMULATOR_TBC_PET_STATS
#define WARLOCK_SIMULATOR_TBC_PET_STATS

#include "player.h"
#include <memory>

struct PetStats
{
    std::shared_ptr<Player> player;
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

    PetStats(std::shared_ptr<Player> player = nullptr);
};

#endif