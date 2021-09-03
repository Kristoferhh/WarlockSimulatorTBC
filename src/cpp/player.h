#pragma once

#include <map>
#include <vector>
#include <string>
#include "auras.h"
#include "talents.h"
#include "sets.h"
#include "spell.h"
#include "aura.h"
#include "items.h"
#include "trinket.h"
#include "playerSettings.h"
#include "damageOverTime.h"

struct Player
{
  const double hitRatingPerPercent = 12.62;
  const double critRatingPerPercent = 22.08;
  const double hasteRatingPerPercent = 15.77;
  const double manaPerInt = 15;
  const double healthPerStamina = 10;
  const double critPerInt = 1 / 81.95; // Crit % per point of intellect
  const double baseCritChancePercent = 1.701;
  Auras* selectedAuras;
  Talents* talents;
  Sets* sets;
  CharacterStats* stats;
  Items* items;
  PlayerSettings* settings;
  std::string filler;
  std::string curse;
  std::vector<Spell*> spells;
  std::vector<std::vector<Spell*>> spellVectors;
  std::vector<Aura*> auras;
  std::vector<DamageOverTime*> dots;
  std::vector<Trinket*> trinkets;
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
  double minimumGcdValue;
  double mp5Timer;
  double fiveSecondRuleTimer;
  int critChanceMultiplier;
  double demonicKnowledgeSpellPower;
  double critMultiplier;

  Player(PlayerSettings* settings);
  void initialize();
  void reset();
  double getGcdValue(std::string varName);
  double getSpellPower(SpellSchool school = SpellSchool::NO_SCHOOL);
  bool isCrit(SpellType spellType, double extraCrit = 0);
  bool isHit(SpellType spellType);
  double getCritChance(SpellType spellType);
  double getHitChance(SpellType spellType);
  double getPartialResistMultiplier(SpellSchool school);
  double getBaseHitChance(int playerLevel, int enemyLevel);
  bool areAnyCooldownsReady();
  void useCooldowns();
  void castLifeTapOrDarkPact();
  bool shouldWriteToCombatLog();
  void combatLog(std::string& entry);
  Spell* getSpell(std::string spellName);
  Aura* getAura(std::string auraName);
  DamageOverTime* getDot(std::string dotName);
  std::vector<Spell*>& getSpellVector(std::string spellName);
  bool spellVectorExists(std::string spellName);
};