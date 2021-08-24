#pragma once

#include <map>
#include <list>
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
  std::list<std::string> combatLogEntries;
  double castTimeRemaining;
  double totalManaRegenerated;
  double gcdRemaining;
  double gcdValue;
  double spellDelay;
  uint64_t totalDuration;
  int iterationDamage;
  double fightTime;
  int iteration;

  Player(PlayerSettings* settings);
  void initialize();
  double getGcdValue(std::string varName);
  double getSpellPower();
  void combatLog(std::string& entry);
};