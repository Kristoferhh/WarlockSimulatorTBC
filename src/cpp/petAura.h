#pragma once

struct Pet;
struct Player;
#include <iostream>

struct PetAura
{
    Pet* pet;
    std::string name;
    std::string varName;
    int duration;
    double durationRemaining;
    bool active;
    // Demonic Frenzy
    int maxStacks;
    int stacks;

    PetAura(Pet* pet);
    void tick(double time);
    virtual void apply();
    void fade(bool endOfIteration = false);
};

struct DemonicFrenzy : public PetAura
{
    DemonicFrenzy(Pet* pet);
};

struct BlackBook : public PetAura
{
    BlackBook(Pet* pet);
    void apply(bool announceInCombatLog = true);
};