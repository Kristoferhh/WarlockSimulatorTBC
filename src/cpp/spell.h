#ifndef WARLOCK_SIMULATOR_TBC_SPELL
#define WARLOCK_SIMULATOR_TBC_SPELL

struct Entity;

#include "aura.h"
#include "enums.h"

struct Spell {
  Entity& entity;
  std::shared_ptr<Aura> aura_effect;
  SpellSchool school;
  std::string name;
  double min_dmg;
  double max_dmg;
  double base_damage;
  bool casting;
  bool can_crit;
  bool on_gcd;
  bool is_proc;
  bool limited_amount_of_casts;
  bool is_non_warlock_ability;
  double coefficient;
  double cooldown_remaining;
  double cast_time;
  double modifier;
  double cooldown;
  double mana_cost;
  double min_mana_gain;
  double max_mana_gain;
  double mana_gain;
  int amount_of_casts_per_fight;
  int amount_of_casts_this_fight;

  Spell(Entity& entity);
  virtual void Setup();
  virtual void Reset();
  virtual void Cast();
  virtual bool Ready();
  virtual bool CanCast();
  virtual double GetCastTime();
  virtual double GetCooldown();
  double GetManaCost();
  void Tick(double time);
  void StartCast(double predicted_damage = 0);
  bool HasEnoughMana();
};

#endif