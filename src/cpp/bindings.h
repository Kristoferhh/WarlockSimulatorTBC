#pragma once
#include "simulation.h"
#include "auras.h"
#include "talents.h"
#include "sets.h"
#include "characterStats.h"
#include <chrono>

void simulationUpdate(int iteration, int iterationAmount, double medianDps, int itemId);
void simulationEnd(double medianDps, double minDps, double maxDps, std::chrono::duration<double> duration, int itemId);

extern "C"
{
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
        , int devastation, int shadowburn, int improvedSearingPain, int improvedImmolate, int ruin, int emberstorm, int backlash, int shadowAndFlame, int shadowfury);
    Sets* allocSets(int plagueheart, int spellfire, int frozenShadoweave, int spellstrike, int oblivion, int manaEtched, int twinStars, int t4, int t5, int t6);
    CharacterStats* allocStats(int health, int mana, double stamina, double intellect, double spirit, double spellPower, int shadowPower, int firePower, int hasteRating, int hitRating, int critRating
        , double critPercent, int mp5, int spellPen, double fireModifier, double frostModifier, double hastePercent, double damageModifier, double shadowModifier, double staminaModifier, double intellectModifier
        ,double spiritModifier, double manaCostModifier, double arcaneModifier, double natureModifier, int natureResist, int arcaneResist, int fireResist, int frostResist, int shadowResist);
    PlayerSettings* allocPlayerSettings(Auras* auras, Talents* talents, Sets* sets, CharacterStats* stats, int itemId);
    Player* allocPlayer(PlayerSettings* settings);
    SimulationSettings* allocSimSettings(int iterations, int minTime, int maxTime);
    Simulation* allocSim(Player* player, SimulationSettings* simulationSettings);
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