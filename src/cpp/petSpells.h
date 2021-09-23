#pragma once

#include "petSpell.h"

struct PetSpells
{
    std::unique_ptr<PetSpell> Melee;
    std::unique_ptr<PetSpell> Firebolt;
    std::unique_ptr<PetSpell> LashOfPain;
    std::unique_ptr<PetSpell> Cleave;
};