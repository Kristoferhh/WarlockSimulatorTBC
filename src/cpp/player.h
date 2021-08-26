#pragma once

struct Spell; // dirty fix for circular reference when we were including "spell.h" before

#include <map>
#include <vector>
#include <string>
#include "playerSettings.h"
#include "auras.h"
#include "talents.h"
#include "sets.h"

struct Player
{
  Auras* auras;
  Talents* talents;
  Sets* sets;
  std::map<std::string, Spell*> spells;
  std::vector<std::string> combatLogEntries;
  double castTimeRemaining;
  double totalManaRegenerated;
  double gcdRemaining;
  double gcdValue;
  double spellDelay;
  double totalDuration;
  int iterationDamage;
  double fightTime;
  int iteration;

  Player(PlayerSettings* settings);
  void initialize();
  double getGcdValue(std::string varName);
  double getSpellPower();
  void combatLog(std::string& entry);
};