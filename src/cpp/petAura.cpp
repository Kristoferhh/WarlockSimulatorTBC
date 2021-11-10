#include "petAura.h"
#include "player.h"

PetAura::PetAura(std::shared_ptr<Pet> pet) : pet(pet)
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

DemonicFrenzy::DemonicFrenzy(std::shared_ptr<Pet> pet) : PetAura(pet)
{
    name = "Demonic Frenzy";
    duration = 10;
    maxStacks = 10;
    stacks = 0;
}

void DemonicFrenzy::apply()
{
    if (stacks < maxStacks)
    {
        stacks++;
    }

    PetAura::apply();
}

void DemonicFrenzy::fade(bool endOfIteration)
{
    stacks = 0;
    PetAura::fade(endOfIteration);
}

BlackBook::BlackBook(std::shared_ptr<Pet> pet) : PetAura(pet)
{
    name = "Black Book";
    duration = 30;
}

void BlackBook::apply(bool announceInCombatLog)
{
    PetAura::apply();
    
    if (announceInCombatLog && pet->player->shouldWriteToCombatLog())
    {
        pet->player->combatLog(pet->name + " Spell Power + 200 (" + std::to_string(pet->stats->spellPower) + " -> " + std::to_string(pet->stats->spellPower + 200) + ")");
        pet->player->combatLog(pet->name + " Attack Power + 325 (" + std::to_string(pet->stats->attackPower) + " -> " + std::to_string(pet->stats->attackPower + 325) + ")");
    }
    pet->buffStats->spellPower += 200;
    pet->buffStats->attackPower += 325;
    pet->calculateStatsFromPlayer(announceInCombatLog);
}

void BlackBook::fade(bool endOfIteration)
{
    PetAura::fade();
    
    if (!endOfIteration && pet->player->shouldWriteToCombatLog())
    {
        pet->player->combatLog(pet->name + " Spell Power - 200 (" + std::to_string(pet->stats->spellPower) + " -> " + std::to_string(pet->stats->spellPower - 200) + ")");
        pet->player->combatLog(pet->name + " Attack Power - 325 (" + std::to_string(pet->stats->attackPower) + " -> " + std::to_string(pet->stats->attackPower - 325) + ")");
    }
    pet->buffStats->spellPower -= 200;
    pet->buffStats->attackPower -= 325;
    pet->calculateStatsFromPlayer(endOfIteration);
}