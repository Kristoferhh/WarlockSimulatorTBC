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
  int itemId;

  PlayerSettings(Auras* auras, Talents* talents, Sets* sets, CharacterStats* stats, int itemId);
};