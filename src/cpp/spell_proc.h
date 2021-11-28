#ifndef WARLOCK_SIMULATOR_TBC_SPELL_PROC
#define WARLOCK_SIMULATOR_TBC_SPELL_PROC

#include "spell.h"

struct SpellProc : public Spell {
  SpellProc(Player& player, std::shared_ptr<Aura> aura = nullptr);
  virtual bool ShouldProc(Spell* spell);
};

#endif