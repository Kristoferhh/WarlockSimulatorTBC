#pragma once

struct Pet;
#include <iostream>
#include "enums.h"

struct PetSpell
{
    Pet* pet;
    SpellSchool school;
    AttackType type;
    std::string name;
    std::string varName;
    double dmg;
    bool casting;
    bool canCrit;
    double coefficient;
    double cooldownRemaining;
    double castTime;
    int manaCost;
    double modifier;
    int cooldown;

    PetSpell(Pet* pet);
    bool ready();
    void setup();
    virtual double getBaseDamage();
    double getCastTime();
    virtual double getCooldown();
    void tick(double time);
    void startCast();
    void cast();
    void damage(bool isCrit, bool isGlancing);
};

struct Melee : public PetSpell
{
    Melee(Pet* pet);
    double getBaseDamage();
    double getCooldown();
};

struct ImpFirebolt : public PetSpell
{
    ImpFirebolt(Pet* pet);
};

struct SuccubusLashOfPain : public PetSpell
{
    SuccubusLashOfPain(Pet* pet);
};

struct FelguardCleave : public PetSpell
{
    FelguardCleave(Pet* pet);
    double getBaseDamage();
};