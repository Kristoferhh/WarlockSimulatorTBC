#include "playerSettings.h"
#include "auras.h"
#include "talents.h"
#include "sets.h"

PlayerSettings::PlayerSettings(Auras* auras, Talents* talents, Sets* sets)
  : auras(auras), talents(talents), sets(sets) {}