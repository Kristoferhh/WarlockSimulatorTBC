#pragma once

#include "spell.h"

struct SpellProc : public Spell {
  SpellProc(Entity& entity, std::shared_ptr<Aura> aura = nullptr);
  virtual bool ShouldProc(Spell* spell);
};