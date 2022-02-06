#pragma once

struct Player;
struct Spell;
#include <iostream>

#include "aura.h"
#include "enums.h"

struct DamageOverTime {
  Player& player;
  std::shared_ptr<Spell> parent_spell;
  SpellSchool school = SpellSchool::kNoSchool;
  int duration = 0;                 // Total duration of the dot
  int original_duration = 0;        // Used for T4 4pc since we're increasing the duration
                                    // by 3 seconds but need to know what the original
                                    // duration was
  int tick_timer_total = 3;         // Total duration of each tick (default is 3 seconds
                                    // between ticks)
  double tick_timer_remaining = 0;  // Time until next tick
  int ticks_remaining = 0;          // Amount of ticks remaining before the dot expires
  int ticks_total = 0;
  double spell_power = 0;           // Spell Power amount when dot was applied
  double base_damage = 0;
  double coefficient = 0;
  double t5_bonus_modifier = 0;     // T5 4pc damage modifier
  bool active = false;
  bool applied_with_amplify_curse = false;
  bool isb_is_active = false;       // Siphon Life
  std::string name;

  DamageOverTime(Player& player);
  void Setup();
  virtual void Apply();
  void Fade();
  void Tick(double time);
  std::vector<double> GetConstantDamage();
  double PredictDamage();
};

struct CorruptionDot : public DamageOverTime {
  void Apply();

  CorruptionDot(Player& player);
};

struct UnstableAfflictionDot : public DamageOverTime {
  UnstableAfflictionDot(Player& player);
};

struct SiphonLifeDot : public DamageOverTime {
  SiphonLifeDot(Player& player);
};

struct ImmolateDot : public DamageOverTime {
  ImmolateDot(Player& player);
  void Apply();
};

struct CurseOfAgonyDot : public DamageOverTime {
  CurseOfAgonyDot(Player& player);
};

struct CurseOfDoomDot : public DamageOverTime {
  CurseOfDoomDot(Player& player);
};