#include "../include/spell_proc.h"

#include <utility>

SpellProc::SpellProc(Entity& entity, std::shared_ptr<Aura> aura) : Spell(entity, std::move(aura)) {
  is_proc = true;
  on_gcd  = false;
}

bool SpellProc::ShouldProc(Spell*) {
  return true;
}