#pragma once

#include "auras.h"
#include "talents.h"
#include "sets.h"
#include "characterStats.h"

struct PlayerSettings
{
    Auras* auras;
    Talents* talents;
    Sets* sets;
    CharacterStats* stats;
    int itemId;
    int metaGemId;
    bool isAldor;
    int enemyLevel;
    int enemyShadowResist;
    int enemyFireResist;
    int mageAtieshAmount;
    int totemOfWrathAmount;
    bool sacrificingPet;
    bool petIsImp;
    bool petIsSuccubus;
    bool petIsFelguard;
    int ferociousInspirationAmount;
    int improvedCurseOfTheElements;
    bool usingCustomIsbUptime;
    int customIsbUptimeValue;
    int improvedDivineSpirit;
    int improvedImp;
    int shadowPriestDps;
    int warlockAtieshAmount;
    int improvedExposeArmor;
    bool isSingleTarget;
    int enemyAmount;
    bool isOrc;
    int powerInfusionAmount;
    bool bloodlustAmount;
    bool innervateAmount;
    int enemyArmor;
    int exposeWeaknessUptime;
    bool improvedFaerieFire;
    bool infinitePlayerMana;
    bool infinitePetMana;
    bool usingLashOfPainOnCooldown;
    bool petIsAggressive;
    bool prepopBlackBook;
    bool randomizeValues;
    bool userChoosingRotation;
    bool exaltedWithShattrathFaction;
    int survivalHunterAgility;

  PlayerSettings(Auras* auras, Talents* talents, Sets* sets, CharacterStats* stats, int itemId, int metaGemId, bool isAldor, int enemyLevel, int enemyShadowResist, int enemyFireResist
        , int mageAtieshAmount, int totemOfWrathAmount, bool sacrificingPet, bool petIsImp, bool petIsSuccubus, bool petIsFelguard, int ferociousInspirationAmount, int improvedCurseOfTheElements
        , bool usingCustomIsbUptime, int customIsbUptimeValue, int improvedDivineSpirit, int improvedImp, int shadowPriestDps, int warlockAtieshAmount, int improvedExposeArmor, bool isSingleTarget, int enemyAmount
        , bool isOrc, int powerInfusionAmount, bool bloodlustAmount, bool innervateAmount, int enemyArmor, int exposeWeaknessUptime, bool improvedFaerieFire, bool infinitePlayerMana, bool infinitePetMana
        , bool usingLashOfPainOnCooldown, bool petIsAggressive, bool prepopBlackBook, bool randomizeValues, bool userChoosingRotation, bool exaltedWithShattrathFaction, int survivalHunterAgility);
};