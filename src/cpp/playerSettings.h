#pragma once

#include "auras.h"
#include "talents.h"
#include "sets.h"
#include "characterStats.h"

struct PlayerSettings
{
  Auras* auras;
  Talents* talents;
  Sets* sets;
  CharacterStats* stats;

  PlayerSettings(Auras* auras, Talents* talents, Sets* sets, CharacterStats* stats);
};