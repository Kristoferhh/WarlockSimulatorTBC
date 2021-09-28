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
#include "PlayerSpells.h"
#include "PlayerAuras.h"
#include "combatLogBreakdown.h"
#include <random>
#include "pet.h"

struct Player
{
  const double hitRatingPerPercent = 12.62;
  const double critRatingPerPercent = 22.08;
  const double hasteRatingPerPercent = 15.77;
  const double manaPerInt = 15;
  const double healthPerStamina = 10;
  const double critPerInt = 1 / 81.95; // Crit % per point of intellect
  const double baseCritChancePercent = 1.701;
  std::unique_ptr<Pet> pet;
  Auras* selectedAuras;
  Talents* talents;
  Sets* sets;
  CharacterStats* stats;
  Items* items;
  PlayerSettings* settings;
  std::unique_ptr<PlayerSpells> spells;
  std::unique_ptr<PlayerAuras> auras;
  std::vector<std::unique_ptr<Trinket>> trinkets;
  std::shared_ptr<Spell> filler;
  std::shared_ptr<Spell> curseSpell;
  std::shared_ptr<Aura> curseAura;
  std::vector<std::string> combatLogEntries;
  std::map<std::string, std::unique_ptr<CombatLogBreakdown>> combatLogBreakdown;
  uint32_t iterationDamage;
  int critChanceMultiplier = 1000;
  std::mt19937 gen;
  std::uniform_int_distribution<> randomNum{1, 100 * critChanceMultiplier};
  double castTimeRemaining;
  double gcdRemaining;
  double gcdValue;
  double spellDelay;
  double totalDuration;
  int level;
  double fightTime;
  int iteration;
  double minimumGcdValue;
  double mp5Timer;
  double fiveSecondRuleTimer;
  double demonicKnowledgeSpellPower;
  double critMultiplier;

  Player(PlayerSettings* settings);
  void initialize();
  void reset();
  void throwError(const std::string& error);
  double getGcdValue(const std::shared_ptr<Spell>& spell);
  double getSpellPower(SpellSchool school = SpellSchool::NO_SCHOOL);
  double getHastePercent();
  bool isCrit(SpellType spellType, double extraCrit = 0);
  bool isHit(SpellType spellType);
  double getCritChance(SpellType spellType);
  double getHitChance(SpellType spellType);
  double getPartialResistMultiplier(SpellSchool school);
  double getBaseHitChance(int playerLevel, int enemyLevel);
  void useCooldowns();
  int getRand();
  void castLifeTapOrDarkPact();
  bool shouldWriteToCombatLog();
  void addIterationDamageAndMana(const std::string& spellName, int manaGain, int damage);
  void postIterationDamageAndMana(const std::string& spellName);
  void sendCombatLogEntries();
  void combatLog(const std::string& entry);
};