#ifndef WARLOCK_SIMULATOR_TBC_SPELL_PROC
#define WARLOCK_SIMULATOR_TBC_SPELL_PROC

#include "player_spell.h"

struct SpellProc : public PlayerSpell {
  SpellProc(Player& player, std::shared_ptr<Aura> aura = nullptr);
  virtual bool ShouldProc(PlayerSpell* spell);
};

#endif