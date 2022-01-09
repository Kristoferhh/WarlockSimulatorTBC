#pragma once

struct SpellCastResult {
  bool is_miss;
  bool is_crit;
  bool is_glancing;
  bool is_dodge;

  SpellCastResult(bool is_miss, bool is_crit, bool is_glancing = false, bool is_dodge = false)
      : is_miss(is_miss), is_crit(is_crit), is_glancing(is_glancing), is_dodge(is_dodge) {}
};