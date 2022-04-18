#pragma once
#include "spell.h"

struct SpellProc : Spell {
  explicit SpellProc(Entity& entity, std::shared_ptr<Aura> aura = nullptr);
  virtual bool ShouldProc(Spell* spell);
};
