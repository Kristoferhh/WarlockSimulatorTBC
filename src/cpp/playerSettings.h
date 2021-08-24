#pragma once

#include "auras.h"
#include "talents.h"
#include "sets.h"

struct PlayerSettings
{
  Auras* auras;
  Talents* talents;
  Sets* sets;

  PlayerSettings(Auras* auras, Talents* talents, Sets* sets);
};