#pragma once

struct Player;
#include "enums.h"
#include "petStats.h"
#include "petSpells.h"
#include "petAuras.h"
#include <iostream>

struct Pet
{
    Player* player;
    PetSpells* spells;
    PetAuras* auras;
    PetStats* baseStats;
    PetStats* buffStats; // Certain stats from buffs need to be separated from the main stat so we can re-calculate the pet's stats in calculateStatsFromPlayer().
    PetStats* debuffStats;
    PetStats* stats;
    PetName pet;
    PetMode mode;
    PetType petType;
    std::string name;
    double enemyDodgeChance;
    double castTimeRemaining;
    double fiveSecondRuleTimerRemaining;
    double spiritTickTimerRemaining;
    double critMultiplier;
    double glancingBlowMultiplier;
    double glancingBlowChance;
    double critSuppression;
    double armorMultiplier;
    double dmg;
    double baseMeleeSpeed;

    Pet(Player* player);
    void initialize();
    void calculateStatsFromAuras();
    void calculateStatsFromPlayer(bool announceInCombatLog = true);
    void setup();
    void cast();
    void reset();
    void tick(double t);
    int getAttackPower();
    double getMeleeCritChance();
    double getMeleeHitChance();
    double getSpellCritChance();
    double getSpellHitChance();
    bool isCrit(AttackType type);
    bool isHit(AttackType type);
};

struct Imp : public Pet
{
    Imp(Player* player);
};

struct Succubus : public Pet
{
    Succubus(Player* player);
};

struct Felguard : public Pet
{
    Felguard(Player* player);
};