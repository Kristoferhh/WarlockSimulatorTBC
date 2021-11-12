#pragma once

struct Pet;
struct Player;
#include <iostream>

struct PetAura
{
    std::shared_ptr<Pet> pet;
    std::string name;
    int duration;
    double durationRemaining;
    bool active;
    // Demonic Frenzy
    int maxStacks;
    int stacks;

    PetAura(std::shared_ptr<Pet> pet);
    void tick(double time);
    virtual void apply();
    virtual void fade(bool endOfIteration = false);
};

struct DemonicFrenzy : public PetAura
{
    DemonicFrenzy(std::shared_ptr<Pet> pet);
    void apply();
    void fade(bool endOfIteration = false);
};

struct BlackBook : public PetAura
{
    BlackBook(std::shared_ptr<Pet> pet);
    void apply(bool announceInCombatLog = true);
    void fade(bool endOfIteration = false);
};