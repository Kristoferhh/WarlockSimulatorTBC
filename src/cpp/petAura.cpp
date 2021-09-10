#include "petAura.h"
#include "player.h"

PetAura::PetAura(Pet* pet) : pet(pet)
{
    duration = 0;
    durationRemaining = 0;
    active = false;
}

void PetAura::tick(double t)
{
    durationRemaining -= t;

    if (active && durationRemaining <= 0)
    {
        fade();
    }
}

void PetAura::apply()
{
    active = true;
    durationRemaining = duration;

    if (pet->player->shouldWriteToCombatLog())
    {
        std::string msg = pet->name + " gains " + name;

        if (pet->auras->DemonicFrenzy != NULL)
        {
            msg.append(" (" + std::to_string(stacks) + ") - Current AP: " + std::to_string(round(pet->getAttackPower())) + ")");
        }

        pet->player->combatLog(msg);
    }
}

void PetAura::fade(bool endOfIteration)
{
    active = false;
    durationRemaining = 0;

    if (!endOfIteration)
    {
        std::string msg = name;

        if (pet->auras->DemonicFrenzy != NULL)
        {
            msg.append(" (" + std::to_string(stacks) + ")");
        }

        pet->player->combatLog(msg);
    }
}