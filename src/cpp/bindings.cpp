#include "bindings.h"
#include "simulation.h"

#ifdef USE_EMSCRIPTEN
#include <emscripten.h>
#endif

double startSimulation(Simulation* sim)
{
    return sim->start();
}

Auras* allocAuras(bool felArmor, bool blessingOfKings, bool blessingOfWisdom, bool judgementOfWisdom, bool manaSpringTotem, bool wrathOfAirTotem, bool totemOfWrath, bool markOfTheWild, bool arcaneIntellect
    , bool prayerOfFortitude, bool prayerOfSpirit, bool bloodPact, bool inspiringPresence, bool moonkinAura, bool powerInfusion, bool powerOfTheGuardianWarlock, bool powerOfTheGuardianMage, bool eyeOfTheNight
    , bool chainOfTheTwilightOwl, bool jadePendantOfBlasting, bool idolOfTheRavenGoddess, bool drumsOfBattle, bool drumsOfWar, bool drumsOfRestoration, bool bloodlust, bool ferociousInspiration
    , bool innervate, bool curseOfTheElements, bool shadowWeaving, bool improvedScorch, bool misery, bool judgementOfTheCrusader, bool vampiricTouch, bool flaskOfPureDeath, bool elixirOfMajorShadowPower
    , bool elixirOfMajorFirepower, bool greaterArcaneElixir, bool adeptsElixir, bool elixirOfDraenicWisdom, bool elixirOfMajorMageblood, bool superManaPotion, bool destructionPotion, bool brilliantWizardOil
    , bool superiorWizardOil, bool blessedWizardOil, bool demonicRune, bool flameCap, bool rumseyRumBlackLabel, bool kreegsStoutBeatdown, bool blackenedBasilisk, bool skullfishSoup, bool veryBerryCream
    , bool midsummerSausage, bool bloodthistle)
{
    return new Auras(felArmor, blessingOfKings, blessingOfWisdom, judgementOfWisdom, manaSpringTotem, wrathOfAirTotem, totemOfWrath, markOfTheWild, arcaneIntellect, prayerOfFortitude, prayerOfSpirit, bloodPact
    , inspiringPresence, moonkinAura, powerInfusion, powerOfTheGuardianWarlock, powerOfTheGuardianMage, eyeOfTheNight, chainOfTheTwilightOwl, jadePendantOfBlasting, idolOfTheRavenGoddess, drumsOfBattle
    , drumsOfWar, drumsOfRestoration, bloodlust, ferociousInspiration, innervate, curseOfTheElements, shadowWeaving, improvedScorch, misery, judgementOfTheCrusader, vampiricTouch, flaskOfPureDeath
    , elixirOfMajorShadowPower, elixirOfMajorFirepower, greaterArcaneElixir, adeptsElixir, elixirOfDraenicWisdom, elixirOfMajorMageblood, superManaPotion, destructionPotion, brilliantWizardOil, superiorWizardOil
    , blessedWizardOil, demonicRune, flameCap, rumseyRumBlackLabel, kreegsStoutBeatdown, blackenedBasilisk, skullfishSoup, veryBerryCream, midsummerSausage, bloodthistle);
}

Talents* allocTalents(int suppression, int improvedCorruption, int improvedLifeTap, int improvedCurseOfAgony, int amplifyCurse, int nightfall, int empoweredCorruption, int siphonLife, int shadowMastery, int contagion
    , int darkPact, int unstableAffliction, int improvedImp, int demonicEmbrace, int felIntellect, int felStamina, int improvedSuccubus, int demonicAegis, int unholyPower, int demonicSacrifice, int manaFeed
    , int masterDemonologist, int soulLink, int demonicKnowledge, int demonicTactics, int felguard, int improvedShadowBolt, int cataclysm, int bane, int improvedFirebolt, int improvedLashOfPain, int devastation
    , int shadowburn, int improvedSearingPain, int improvedImmolate, int ruin, int emberstorm, int backlash, int shadowAndFlame, int shadowfury)
{
    return new Talents(suppression, improvedCorruption, improvedLifeTap, improvedCurseOfAgony, amplifyCurse, nightfall, empoweredCorruption, siphonLife, shadowMastery, contagion, darkPact, unstableAffliction
    , improvedImp, demonicEmbrace, felIntellect, felStamina, improvedSuccubus, demonicAegis, unholyPower, demonicSacrifice, manaFeed, masterDemonologist, soulLink, demonicKnowledge, demonicTactics
    , felguard, improvedShadowBolt, cataclysm, bane, improvedFirebolt, improvedLashOfPain, devastation, shadowburn, improvedSearingPain, improvedImmolate, ruin, emberstorm, backlash, shadowAndFlame, shadowfury);
}

Sets* allocSets(int plagueheart, int spellfire, int frozenShadoweave, int spellstrike, int oblivion, int manaEtched, int twinStars, int t4, int t5, int t6)
{
    return new Sets(plagueheart, spellfire, frozenShadoweave, spellstrike, oblivion, manaEtched, twinStars, t4, t5, t6);
}

PlayerSettings* allocPlayerSettings(Auras* auras, Talents* talents, Sets* sets)
{
    return new PlayerSettings(auras, talents, sets);
}

Player* allocPlayer(PlayerSettings* settings)
{
    return new Player(settings);
}

SimulationSettings* allocSimSettings(int iterations, int minTime, int maxTime)
{
    return new SimulationSettings(iterations, minTime, maxTime);
}

Simulation* allocSim(Player* player, SimulationSettings* simulationSettings)
{
    return new Simulation(player, simulationSettings);
}

void freeAuras(Auras* auras)
{
    delete auras;
}

void freeTalents(Talents* talents)
{
    delete talents;
}

void freeSets(Sets* sets)
{
    delete sets;
}

void freePlayerSettings(PlayerSettings* settings)
{
    delete settings;
}

void freePlayer(Player* player)
{
    delete player;
}

void freeSimSettings(SimulationSettings* settings)
{
    delete settings;
}

void freeSim(Simulation* sim)
{
    delete sim;
}