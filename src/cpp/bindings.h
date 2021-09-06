#pragma once
#include "simulation.h"
#include "auras.h"
#include "talents.h"
#include "sets.h"
#include "characterStats.h"
#include <chrono>
#include "constant.h"
#include "aura.h"
#include "items.h"
#include "combatLogBreakdown.h"

void postCombatLogBreakdown(const char* name, uint32_t casts, uint32_t crits, uint32_t misses, uint32_t manaGain, uint32_t damage, uint32_t count, double uptime);
void combatLogUpdate(const char* combatLogEntry);
void simulationUpdate(int iteration, int iterationAmount, double medianDps, int itemId);
void simulationEnd(double medianDps, double minDps, double maxDps, std::chrono::duration<double> duration, int itemId, int iterationAmount, uint32_t totalDuration, uint32_t totalManaRegenerated);

extern "C"
{
    unsigned int* allocRandomSeeds(int amountOfSeeds);
    Items* allocItems(int head, int neck, int shoulders, int back, int chest, int bracers, int gloves, int belt, int legs, int boots, int finger1, int finger2, int trinket1, int trinket2, int mainHand
    , int offHand, int twoHand, int wand);
    Auras* allocAuras(bool felArmor, bool blessingOfKings, bool blessingOfWisdom, bool judgementOfWisdom, bool manaSpringTotem, bool wrathOfAirTotem, bool totemOfWrath, bool markOfTheWild, bool arcaneIntellect
        , bool prayerOfFortitude, bool prayerOfSpirit, bool bloodPact, bool inspiringPresence, bool moonkinAura, bool powerInfusion, bool powerOfTheGuardianWarlock, bool powerOfTheGuardianMage, bool eyeOfTheNight
        , bool chainOfTheTwilightOwl, bool jadePendantOfBlasting, bool idolOfTheRavenGoddess, bool drumsOfBattle, bool drumsOfWar, bool drumsOfRestoration, bool bloodlust, bool ferociousInspiration
        , bool innervate, bool curseOfTheElements, bool shadowWeaving, bool improvedScorch, bool misery, bool judgementOfTheCrusader, bool vampiricTouch, bool flaskOfPureDeath, bool elixirOfMajorShadowPower
        , bool elixirOfMajorFirepower, bool greaterArcaneElixir, bool adeptsElixir, bool elixirOfDraenicWisdom, bool elixirOfMajorMageblood, bool superManaPotion, bool destructionPotion, bool brilliantWizardOil
        , bool superiorWizardOil, bool blessedWizardOil, bool demonicRune, bool flameCap, bool rumseyRumBlackLabel, bool kreegsStoutBeatdown, bool blackenedBasilisk, bool skullfishSoup, bool veryBerryCream
        , bool midsummerSausage, bool bloodthistle);
    Talents* allocTalents(int suppression, int improvedCorruption, int improvedLifeTap, int improvedCurseOfAgony, int amplifyCurse, int nightfall, int empoweredCorruption, int siphonLife, int shadowMastery
        , int contagion, int darkPact, int unstableAffliction, int improvedImp, int demonicEmbrace, int felIntellect, int felStamina, int improvedSuccubus, int demonicAegis, int unholyPower, int demonicSacrifice
        , int manaFeed, int masterDemonologist, int soulLink, int demonicKnowledge, int demonicTactics, int felguard, int improvedShadowBolt, int cataclysm, int bane, int improvedFirebolt, int improvedLashOfPain
        , int devastation, int shadowburn, int improvedSearingPain, int improvedImmolate, int ruin, int emberstorm, int backlash, int conflagrate, int shadowAndFlame, int shadowfury);
    Sets* allocSets(int plagueheart, int spellfire, int frozenShadoweave, int spellstrike, int oblivion, int manaEtched, int twinStars, int t4, int t5, int t6);
    CharacterStats* allocStats(int health, int mana, double stamina, double intellect, double spirit, double spellPower, int shadowPower, int firePower, int hasteRating, int hitRating, int critRating
        , double critPercent, int mp5, int spellPen, double fireModifier, double frostModifier, double hastePercent, double damageModifier, double shadowModifier, double staminaModifier, double intellectModifier
        , double spiritModifier, double manaCostModifier, double arcaneModifier, double natureModifier, int natureResist, int arcaneResist, int fireResist, int frostResist, int shadowResist);
    PlayerSettings* allocPlayerSettings(Auras* auras, Talents* talents, Sets* sets, CharacterStats* stats, Items* items, int itemId, int metaGemId, bool isAldor, int enemyLevel, int enemyShadowResist, int enemyFireResist
        , int mageAtieshAmount, int totemOfWrathAmount, bool sacrificingPet, bool petIsImp, bool petIsSuccubus, bool petIsFelguard, int ferociousInspirationAmount, int improvedCurseOfTheElements
        , bool usingCustomIsbUptime, int customIsbUptimeValue, int improvedDivineSpirit, int improvedImp, int shadowPriestDps, int warlockAtieshAmount, int improvedExposeArmor, bool isSingleTarget, int enemyAmount
        , bool isOrc, int powerInfusionAmount, bool bloodlustAmount, bool innervateAmount, int enemyArmor, int exposeWeaknessUptime, bool improvedFaerieFire, bool infinitePlayerMana, bool infinitePetMana
        , bool usingLashOfPainOnCooldown, bool petIsAggressive, bool prepopBlackBook, bool randomizeValues, bool userChoosingRotation, bool exaltedWithShattrathFaction, int survivalHunterAgility, bool hasImmolate
        , bool hasCorruption, bool hasSiphonLife, bool hasUnstableAffliction, bool hasSearingPain, bool hasShadowBolt, bool hasIncinerate, bool hasCurseOfRecklessness, bool hasCurseOfTheElements
        , bool hasCurseOfAgony, bool hasCurseOfDoom, bool hasDeathCoil, bool hasShadowburn, bool hasConflagrate, bool hasShadowfury, bool hasAmplifyCurse, bool hasDarkPact);
    Player* allocPlayer(PlayerSettings* settings);
    SimulationSettings* allocSimSettings(int iterations, int minTime, int maxTime, unsigned int* randomSeeds);
    Simulation* allocSim(Player* player, SimulationSettings* simulationSettings);
    void freeUnsignedInt(unsigned int* num);
    void freeItems(Items* items);
    void freeAuras(Auras* auras);
    void freeTalents(Talents* talents);
    void freeSets(Sets* sets);
    void freeStats(CharacterStats* stats);
    void freePlayerSettings(PlayerSettings* settings);
    void freePlayer(Player* player);
    void freeSimSettings(SimulationSettings* settings);
    void freeSim(Simulation* sim);
    void startSimulation(Simulation* sim);
}