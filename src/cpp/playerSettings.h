#pragma once

#include "auras.h"
#include "talents.h"
#include "sets.h"
#include "characterStats.h"
#include "items.h"

struct PlayerSettings
{
    Auras* auras;
    Talents* talents;
    Sets* sets;
    CharacterStats* stats;
    Items* items;
    int itemId;
    int metaGemId;
    bool recordingCombatLogBreakdown;
    bool simmingStamina;
    bool simmingIntellect;
    bool simmingSpellPower;
    bool simmingShadowPower;
    bool simmingFirePower;
    bool simmingHitRating;
    bool simmingCritRating;
    bool simmingHasteRating;
    bool simmingMp5;
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
    bool simChoosingRotation;
    bool exaltedWithShattrathFaction;
    int survivalHunterAgility;
    bool hasImmolate;
    bool hasCorruption;
    bool hasSiphonLife;
    bool hasUnstableAffliction;
    bool hasSearingPain;
    bool hasShadowBolt;
    bool hasIncinerate;
    bool hasCurseOfRecklessness;
    bool hasCurseOfTheElements;
    bool hasCurseOfAgony;
    bool hasCurseOfDoom;
    bool hasDeathCoil;
    bool hasShadowburn;
    bool hasConflagrate;
    bool hasShadowfury;
    bool hasAmplifyCurse;
    bool hasDarkPact;
    bool hasElementalShamanT4Bonus;

    PlayerSettings(Auras* auras, Talents* talents, Sets* sets, CharacterStats* stats, Items* items, int itemId, int metaGemId, bool recordingCombatLogBreakdown, bool simmingStamina, bool simmingIntellect, bool simmingSpellPower
        , bool simmingShadowPower, bool simmingFirePower, bool simmingHitRating, bool simmingCritRating, bool simmingHasteRating, bool simmingMp5, bool isAldor, int enemyLevel, int enemyShadowResist
        , int enemyFireResist, int mageAtieshAmount, int totemOfWrathAmount, bool sacrificingPet, bool petIsImp, bool petIsSuccubus, bool petIsFelguard, int ferociousInspirationAmount, int improvedCurseOfTheElements
        , bool usingCustomIsbUptime, int customIsbUptimeValue, int improvedDivineSpirit, int improvedImp, int shadowPriestDps, int warlockAtieshAmount, int improvedExposeArmor, bool isSingleTarget, int enemyAmount
        , bool isOrc, int powerInfusionAmount, bool bloodlustAmount, bool innervateAmount, int enemyArmor, int exposeWeaknessUptime, bool improvedFaerieFire, bool infinitePlayerMana, bool infinitePetMana
        , bool usingLashOfPainOnCooldown, bool petIsAggressive, bool prepopBlackBook, bool randomizeValues, bool simChoosingRotation, bool exaltedWithShattrathFaction, int survivalHunterAgility
        , bool hasImmolate, bool hasCorruption, bool hasSiphonLife, bool hasUnstableAffliction, bool hasSearingPain, bool hasShadowBolt, bool hasIncinerate, bool hasCurseOfRecklessness
        , bool hasCurseOfTheElements, bool hasCurseOfAgony, bool hasCurseOfDoom, bool hasDeathCoil, bool hasShadowburn, bool hasConflagrate, bool hasShadowfury, bool hasAmplifyCurse
        , bool hasDarkPact, bool hasElementalShamanT4Bonus);
};