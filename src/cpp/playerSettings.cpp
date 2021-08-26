#include "playerSettings.h"
#include "auras.h"
#include "talents.h"
#include "sets.h"
#include "characterStats.h"

PlayerSettings::PlayerSettings(Auras* auras, Talents* talents, Sets* sets, CharacterStats* stats)
  : auras(auras), talents(talents), sets(sets), stats(stats) {}