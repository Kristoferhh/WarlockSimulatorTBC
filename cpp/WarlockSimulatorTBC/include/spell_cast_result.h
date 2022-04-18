#pragma once

struct SpellCastResult {
  bool is_miss;
  bool is_crit;
  bool is_glancing;
  bool is_dodge;

  SpellCastResult(const bool kIsMiss, const bool kIsCrit, const bool kIsGlancing = false, const bool kIsDodge = false)
    : is_miss(kIsMiss),
      is_crit(kIsCrit),
      is_glancing(kIsGlancing),
      is_dodge(kIsDodge) {
  }
};
