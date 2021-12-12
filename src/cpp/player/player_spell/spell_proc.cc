#include "spell_proc.h"

SpellProc::SpellProc(Player& player, std::shared_ptr<Aura> aura) : PlayerSpell(player, aura) {
  is_proc = true;
  on_gcd = false;
}

bool SpellProc::ShouldProc(PlayerSpell* spell) { return true; }