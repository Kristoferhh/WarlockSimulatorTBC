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
  int minDmg = 0;
  int maxDmg = 0;
  int minMana = 0;
  int maxMana = 0;
  double castTime = 0;
  double manaCost = 0;
  double coefficient = 0;
  double manaReturn = 0;
  double cooldown = 0;
  double modifier = 1;
  double bonusCrit = 0;
  double cooldownRemaining = 0;
  double dmg = 0;
  double avgManaValue = 0;
  bool isDot = false;
  bool doesDamage = false;
  bool canCrit = false;
  bool isNonWarlockAbility = false;
  bool casting = false;
  bool isItem = false;
  bool onGcd = true;
  bool isProc = false;
  std::string varName = ""; // Same as 'name' except it's written in camelCase
  std::string name = "";
  std::string breakdownTable = "damage";

  Spell(Player* p) : player(p) {}

  void reset()
  {
    cooldownRemaining = 0;
    casting = false;
  }

  void setup()
  {
    varName = name; //todo implement camelCase()
    if (minDmg > 0 && maxDmg > 0)
    {
      dmg = (minDmg + maxDmg) / 2;
    }
    if (minMana > 0 && maxMana > 0)
    {
      avgManaValue = (minMana + maxMana) / 2;
    }
  }

  bool canCast()
  {
    return (!onGcd || isNonWarlockAbility || player->gcdRemaining <= 0) && (isProc || isNonWarlockAbility || player->castTimeRemaining <= 0) && cooldownRemaining <= 0;
  }

  bool hasEnoughMana()
  {
    return true;
    //return manaCost <= player->stats.mana;
  }

  virtual bool ready()
  {
    return canCast() && hasEnoughMana();
  }

  virtual double getCastTime()
  {
    return 0;
  }

  virtual void startCast(double predictedDamage = 0)
  {
    if (onGcd)
    {
      player->gcdRemaining = player->getGcdValue(varName);
    }

    std::string combatLogMsg = "";
    if (castTime > 0)
    {
      casting = true;
      player->castTimeRemaining = getCastTime();
      if (!isProc)
      {
        //combatLogMsg.append("Started casting " + name + " - Cast time: " + player->castTimeRemaining - player->spellDelay) + " (" + round((player->stats.hasteRating / hasteRatingPerPercent + player->stats.hastePercent) * 10000) / 10000 + "% haste at a base cast speed of " + castTime + ").";
      }
    }
    else
    {
      if (!isProc)
      {
        combatLogMsg.append("Cast " + name);
      }
      cast();
    }
    if (onGcd && !isNonWarlockAbility)
    {
      combatLogMsg.append(" - Global cooldown: " + std::to_string(player->gcdRemaining));
    }
    if (predictedDamage > 0)
    {
      combatLogMsg.append(" - Estimated damage / Cast time: " + std::to_string(round(predictedDamage)));
    }
    player->combatLog(combatLogMsg);
  }

  virtual void cast()
  {
    
  }

  double getModifier()
  {
    return 0;
  }

  double getConstantDamage(bool noRng = false)
  {
    return 0;
  }

  double getCritMultiplier()
  {
    return 0;
  }

  double predictDamage()
  {
    return 0;
  }

  void damage(bool isCrit)
  {
    
  }

  void tick(int t)
  {
    if (cooldownRemaining > 0 && cooldownRemaining - t <= 0)
    {
      player->combatLog(name + " is off cooldown");
    }
    cooldownRemaining -= t;

    if (casting && player->castTimeRemaining <= 0)
    {
      cast();
    }
  }
};