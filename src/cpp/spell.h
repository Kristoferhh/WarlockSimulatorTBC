#pragma once

#include "player.h"
#include <string>
#include <cmath>

enum School {SHADOW, FIRE};
enum Type {AFFLICTION, DESTRUCTION};

struct Spell
{
  Player* player;
  int school;
  int type;
  int minDmg;
  int maxDmg;
  int minMana;
  int maxMana;
  double castTime;
  double manaCost;
  double coefficient;
  double manaReturn;
  double cooldown;
  double modifier;
  double bonusCrit;
  double cooldownRemaining;
  double dmg;
  double avgManaValue;
  bool isDot;
  bool doesDamage;
  bool canCrit;
  bool isNonWarlockAbility;
  bool casting;
  bool isItem;
  bool onGcd;
  bool isProc;
  std::string varName; // Same as 'name' except it's written in camelCase
  std::string name;
  std::string breakdownTable;

  Spell(Player* player);

  void reset();
  void setup();
  bool canCast();
  bool hasEnoughMana();
  virtual bool ready();
  virtual double getCastTime();
  virtual void startCast(double predictedDamage = 0);
  virtual void cast();
  double getModifier();
  double getConstantDamage(bool noRng);
  double getCritMultiplier();
  double predictDamage();
  void damage(bool isCrit);
  void tick(int t);
};

struct LifeTap : public Spell
{
    LifeTap(Player* player);
    int manaGain();
    void cast();
};

struct ShadowBolt : public Spell
{
    ShadowBolt(Player* player);
    void startCast(double predictedDamage);
    double calculateCastTime();
};