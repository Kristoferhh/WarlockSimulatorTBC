#ifndef WARLOCK_SIMULATOR_TBC_DAMAGE_OVER_TIME
#define WARLOCK_SIMULATOR_TBC_DAMAGE_OVER_TIME

struct Player;
#include <iostream>

#include "../../aura.h"
#include "../../enums.h"

struct DamageOverTime {
  Player& player;
  SpellSchool school;
  int duration;                 // Total duration of the dot
  int original_duration;        // Used for T4 4pc since we're increasing the duration
                                // by 3 seconds but need to know what the original
                                // duration was
  int tick_timer_total;         // Total duration of each tick (default is 3 seconds
                                // between ticks)
  double tick_timer_remaining;  // Time until next tick
  int ticks_remaining;          // Amount of ticks remaining before the dot expires
  int ticks_total;
  double spell_power;  // Spell Power amount when dot was applied
  double base_damage;
  double modifier;
  double coefficient;
  double t5_bonus_modifier;  // T5 4pc damage modifier
  bool active;
  bool applied_with_amplify_curse;
  bool isb_is_active;  // Siphon Life
  std::string name;

  DamageOverTime(Player& player);
  void Setup();
  virtual void Apply();
  virtual double GetModifier();
  void Fade();
  void Tick(double time);
  std::vector<double> GetConstantDamage();
  double PredictDamage();
};

struct CorruptionDot : public DamageOverTime {
  double GetModifier();
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
  double GetModifier();
};

struct CurseOfDoomDot : public DamageOverTime {
  CurseOfDoomDot(Player& player);
  double GetModifier();
};

#endif