#pragma once

#include <map>
#include <vector>
#include <string>
#include "playerSettings.h"
#include "auras.h"
#include "talents.h"
#include "sets.h"
#include "spell.h"

struct Player
{
  const double hitRatingPerPercent = 12.6;
  const double critRatingPerPercent = 22.08;
  const double hasteRatingPerPercent = 15.77;
  const double manaPerInt = 15;
  const double healthPerStamina = 10;
  const double critPerInt = 1 / 81.95; // Crit % per point of intellect
  const double baseCritChancePercent = 1.701;
  Auras* auras;
  Talents* talents;
  Sets* sets;
  CharacterStats* stats;
  std::map<std::string, Spell*> spells;
  std::vector<std::string> combatLogEntries;
  double castTimeRemaining;
  double totalManaRegenerated;
  double gcdRemaining;
  double gcdValue;
  double spellDelay;
  double totalDuration;
  int level;
  int iterationDamage;
  double fightTime;
  int iteration;
  int itemId;
  double minimumGcdValue;
  double mp5Timer;
  double fiveSecondRuleTimer;
  int critChanceMultiplier;
  double demonicKnowledgeSpellPower;

  Player(PlayerSettings* settings);
  void initialize();
  void reset();
  double getGcdValue(std::string varName);
  double getSpellPower();
  bool isCrit(SpellType spellType, double extraCrit = 0);
  bool isHit(SpellType spellType);
  double getCritChance(SpellType spellType);
  double getHitChance(SpellType spellType);
  double getPartialResistMultiplier(int resist);
  void castLifeTapOrDarkPact();
  bool shouldWriteToCombatLog();
  void combatLog(std::string& entry);
};