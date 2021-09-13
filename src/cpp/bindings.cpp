#include "bindings.h"
#include "simulation.h"
#include "constant.h"
#include <random>

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>
#include "emscripten.h"
#else
#include <iostream>
#define EMSCRIPTEN_KEEPALIVE
#endif

void errorCallback(const char* errorMsg)
{
#ifdef EMSCRIPTEN
    EM_ASM({
        postMessage({
            event: "errorCallback",
            data: {
                errorMsg: UTF8ToString($0)
            }
        })
    }, errorMsg);
#endif
}

void postCombatLogBreakdown(const char* name, uint32_t casts, uint32_t crits, uint32_t misses, uint32_t manaGain, uint32_t damage, uint32_t count, double uptime, uint32_t dodges, uint32_t glancingBlows)
{
#ifdef EMSCRIPTEN
    EM_ASM({
        postMessage({
            event: "combatLogBreakdown",
            data: {
                name: UTF8ToString($0),
                casts: $1,
                crits: $2,
                misses: $3,
                manaGain: $4,
                damage: $5,
                count: $6,
                uptime: $7,
                dodges: $8,
                glancingBlows: $9
            }
        })
    }, name, casts, crits, misses, manaGain, damage, count, uptime, dodges, glancingBlows);
#endif
}

void combatLogUpdate(const char* combatLogEntry)
{
#ifdef EMSCRIPTEN
    EM_ASM({
        postMessage({
            event: "combatLogUpdate",
            data: {
                combatLogEntry: UTF8ToString($0)
            }
        })
    }, combatLogEntry);
#endif
}

void simulationUpdate(int iteration, int iterationAmount, double medianDps, int itemId)
{
#ifdef EMSCRIPTEN
    EM_ASM({
        postMessage({
            event: "update",
            data: {
                medianDps: $0,
                iteration: $1,
                iterationAmount: $2,
                itemId: $3
            }
        })
    }, medianDps, iteration, iterationAmount, itemId);
#else
    std::cout << "Iteration: " << std::to_string(iteration) << "/" << std::to_string(iterationAmount) << ". Median DPS: " << std::to_string(medianDps) << std::endl;
#endif
}

void simulationEnd(double medianDps, double minDps, double maxDps, std::chrono::duration<double> duration, int itemId, int iterationAmount, uint32_t totalDuration, uint32_t totalManaRegenerated)
{
#ifdef EMSCRIPTEN
    EM_ASM({
        postMessage({
            event: "end",
            data: {
                medianDps: $0,
                minDps: $1,
                maxDps: $2,
                duration: $3,
                itemId: $4,
                iterationAmount: $5,
                totalDuration: $6,
                totalManaRegenerated: $7
            }
        })
    }, medianDps, minDps, maxDps, duration.count(), itemId, iterationAmount, totalDuration, totalManaRegenerated);
#else
    std::cout << "Median DPS: " << std::to_string(medianDps) << ". Min DPS: " << std::to_string(minDps) << ". Max DPS: " << std::to_string(maxDps) << ". Time: " << std::to_string(duration.count()) << std::endl;
#endif
}

EMSCRIPTEN_KEEPALIVE
void startSimulation(Simulation* sim)
{
    sim->start();
}

EMSCRIPTEN_KEEPALIVE
unsigned int* allocRandomSeeds(int amountOfSeeds)
{
    std::random_device rd;
    unsigned int* seeds = new unsigned int[amountOfSeeds];

    for (int i = 0; i < amountOfSeeds; i++)
    {
        seeds[i] = rd();
    }

    return seeds;
}

EMSCRIPTEN_KEEPALIVE
Items* allocItems(int head, int neck, int shoulders, int back, int chest, int bracers, int gloves, int belt, int legs, int boots
    , int finger1, int finger2, int trinket1, int trinket2, int mainHand, int offHand, int twoHand, int wand)
{
    return new Items(head, neck, shoulders, back, chest, bracers, gloves, belt, legs, boots, finger1, finger2, trinket1, trinket2, mainHand, offHand, twoHand, wand);
}

EMSCRIPTEN_KEEPALIVE
Auras* allocAuras(bool felArmor, bool blessingOfKings, bool blessingOfWisdom, bool judgementOfWisdom, bool manaSpringTotem, bool wrathOfAirTotem, bool totemOfWrath, bool markOfTheWild, bool arcaneIntellect
    , bool prayerOfFortitude, bool prayerOfSpirit, bool bloodPact, bool inspiringPresence, bool moonkinAura, bool powerInfusion, bool powerOfTheGuardianWarlock, bool powerOfTheGuardianMage, bool eyeOfTheNight
    , bool chainOfTheTwilightOwl, bool jadePendantOfBlasting, bool idolOfTheRavenGoddess, bool drumsOfBattle, bool drumsOfWar, bool drumsOfRestoration, bool bloodlust, bool ferociousInspiration
    , bool innervate, bool curseOfTheElements, bool shadowWeaving, bool improvedScorch, bool misery, bool judgementOfTheCrusader, bool vampiricTouch, bool faerieFire, bool sunderArmor, bool exposeArmor
    , bool curseOfRecklessness, bool bloodFrenzy, bool exposeWeakness, bool annihilator, bool improvedHuntersMark, bool flaskOfPureDeath, bool elixirOfMajorShadowPower, bool elixirOfMajorFirepower
    , bool greaterArcaneElixir, bool adeptsElixir, bool elixirOfDraenicWisdom, bool elixirOfMajorMageblood, bool superManaPotion, bool destructionPotion, bool brilliantWizardOil, bool superiorWizardOil
    , bool blessedWizardOil, bool demonicRune, bool flameCap, bool rumseyRumBlackLabel, bool kreegsStoutBeatdown, bool blackenedBasilisk, bool skullfishSoup, bool veryBerryCream, bool midsummerSausage
    , bool bloodthistle, bool petBlessingOfKings, bool petBlessingOfWisdom, bool petBlessingOfMight, bool petArcaneIntellect, bool petMarkOfTheWild, bool petPrayerOfFortitude, bool petPrayerOfSpirit
    , bool petKiblersBits, bool petHeroicPresence, bool petStrengthOfEarthTotem, bool petGraceOfAirTotem, bool petBattleShout, bool petTrueshotAura, bool petLeaderOfThePack, bool petUnleashedRage
    , bool petStaminaScroll, bool petIntellectScroll, bool petStrengthScroll, bool petAgilityScroll, bool petSpiritScroll)
{
    return new Auras(felArmor, blessingOfKings, blessingOfWisdom, judgementOfWisdom, manaSpringTotem, wrathOfAirTotem, totemOfWrath, markOfTheWild, arcaneIntellect, prayerOfFortitude, prayerOfSpirit, bloodPact
    , inspiringPresence, moonkinAura, powerInfusion, powerOfTheGuardianWarlock, powerOfTheGuardianMage, eyeOfTheNight, chainOfTheTwilightOwl, jadePendantOfBlasting, idolOfTheRavenGoddess, drumsOfBattle
    , drumsOfWar, drumsOfRestoration, bloodlust, ferociousInspiration, innervate, curseOfTheElements, shadowWeaving, improvedScorch, misery, judgementOfTheCrusader, vampiricTouch, faerieFire, sunderArmor
    , exposeArmor, curseOfRecklessness, bloodFrenzy, exposeWeakness, annihilator, improvedHuntersMark, flaskOfPureDeath, elixirOfMajorShadowPower, elixirOfMajorFirepower, greaterArcaneElixir, adeptsElixir
    , elixirOfDraenicWisdom, elixirOfMajorMageblood, superManaPotion, destructionPotion, brilliantWizardOil, superiorWizardOil, blessedWizardOil, demonicRune, flameCap, rumseyRumBlackLabel, kreegsStoutBeatdown
    , blackenedBasilisk, skullfishSoup, veryBerryCream, midsummerSausage, bloodthistle, petBlessingOfKings, petBlessingOfWisdom, petBlessingOfMight, petArcaneIntellect, petMarkOfTheWild, petPrayerOfFortitude
    , petPrayerOfSpirit, petKiblersBits, petHeroicPresence, petStrengthOfEarthTotem, petGraceOfAirTotem, petBattleShout, petTrueshotAura, petLeaderOfThePack, petUnleashedRage, petStaminaScroll, petIntellectScroll
    , petStrengthScroll, petAgilityScroll, petSpiritScroll);
}

EMSCRIPTEN_KEEPALIVE
Talents* allocTalents(int suppression, int improvedCorruption, int improvedLifeTap, int improvedCurseOfAgony, int amplifyCurse, int nightfall, int empoweredCorruption, int siphonLife, int shadowMastery, int contagion
    , int darkPact, int unstableAffliction, int improvedImp, int demonicEmbrace, int felIntellect, int felStamina, int improvedSuccubus, int demonicAegis, int unholyPower, int demonicSacrifice, int manaFeed
    , int masterDemonologist, int soulLink, int demonicKnowledge, int demonicTactics, int felguard, int improvedShadowBolt, int cataclysm, int bane, int improvedFirebolt, int improvedLashOfPain, int devastation
    , int shadowburn, int improvedSearingPain, int improvedImmolate, int ruin, int emberstorm, int backlash, int conflagrate, int shadowAndFlame, int shadowfury)
{
    return new Talents(suppression, improvedCorruption, improvedLifeTap, improvedCurseOfAgony, amplifyCurse, nightfall, empoweredCorruption, siphonLife, shadowMastery, contagion, darkPact, unstableAffliction
    , improvedImp, demonicEmbrace, felIntellect, felStamina, improvedSuccubus, demonicAegis, unholyPower, demonicSacrifice, manaFeed, masterDemonologist, soulLink, demonicKnowledge, demonicTactics
    , felguard, improvedShadowBolt, cataclysm, bane, improvedFirebolt, improvedLashOfPain, devastation, shadowburn, improvedSearingPain, improvedImmolate, ruin, emberstorm, backlash, conflagrate, shadowAndFlame, shadowfury);
}

EMSCRIPTEN_KEEPALIVE
Sets* allocSets(int plagueheart, int spellfire, int frozenShadoweave, int spellstrike, int oblivion, int manaEtched, int twinStars, int t4, int t5, int t6)
{
    return new Sets(plagueheart, spellfire, frozenShadoweave, spellstrike, oblivion, manaEtched, twinStars, t4, t5, t6);
}

EMSCRIPTEN_KEEPALIVE
CharacterStats* allocStats(int health, int mana, double stamina, double intellect, double spirit, double spellPower, int shadowPower, int firePower, int hasteRating, int hitRating, int critRating, double critPercent,
    int mp5, int spellPen, double fireModifier, double frostModifier, double hastePercent, double damageModifier, double shadowModifier, double staminaModifier, double intellectModifier,
    double spiritModifier, double manaCostModifier, double arcaneModifier, double natureModifier, int natureResist, int arcaneResist, int fireResist, int frostResist, int shadowResist)
{
    return new CharacterStats(health, mana, stamina, intellect, spirit, spellPower, shadowPower, firePower, hasteRating, hitRating, critRating, critPercent, mp5, spellPen, fireModifier, frostModifier, hastePercent
    , damageModifier, shadowModifier, staminaModifier, intellectModifier, spiritModifier, manaCostModifier, arcaneModifier, natureModifier, natureResist, arcaneResist, fireResist, frostResist, shadowResist);
}

EMSCRIPTEN_KEEPALIVE
PlayerSettings* allocPlayerSettings(Auras* auras, Talents* talents, Sets* sets, CharacterStats* stats, Items* items, int itemId, int metaGemId, bool isAldor, int enemyLevel, int enemyShadowResist, int enemyFireResist
        , int mageAtieshAmount, int totemOfWrathAmount, bool sacrificingPet, bool petIsImp, bool petIsSuccubus, bool petIsFelguard, int ferociousInspirationAmount, int improvedCurseOfTheElements
        , bool usingCustomIsbUptime, int customIsbUptimeValue, int improvedDivineSpirit, int improvedImp, int shadowPriestDps, int warlockAtieshAmount, int improvedExposeArmor, bool isSingleTarget, int enemyAmount
        , bool isOrc, int powerInfusionAmount, bool bloodlustAmount, bool innervateAmount, int enemyArmor, int exposeWeaknessUptime, bool improvedFaerieFire, bool infinitePlayerMana, bool infinitePetMana
        , bool usingLashOfPainOnCooldown, bool petIsAggressive, bool prepopBlackBook, bool randomizeValues, bool userChoosingRotation, bool exaltedWithShattrathFaction, int survivalHunterAgility, bool hasImmolate
        , bool hasCorruption, bool hasSiphonLife, bool hasUnstableAffliction, bool hasSearingPain, bool hasShadowBolt, bool hasIncinerate, bool hasCurseOfRecklessness, bool hasCurseOfTheElements, bool hasCurseOfAgony
        , bool hasCurseOfDoom, bool hasDeathCoil, bool hasShadowburn, bool hasConflagrate, bool hasShadowfury, bool hasAmplifyCurse, bool hasDarkPact)
{
    return new PlayerSettings(auras, talents, sets, stats, items, itemId, metaGemId, isAldor, enemyLevel, enemyShadowResist, enemyFireResist, mageAtieshAmount, totemOfWrathAmount, sacrificingPet, petIsImp, petIsSuccubus
        , petIsFelguard, ferociousInspirationAmount, improvedCurseOfTheElements, usingCustomIsbUptime, customIsbUptimeValue, improvedDivineSpirit, improvedImp, shadowPriestDps, warlockAtieshAmount
        , improvedExposeArmor, isSingleTarget, enemyAmount, isOrc, powerInfusionAmount, bloodlustAmount, innervateAmount, enemyArmor, exposeWeaknessUptime, improvedFaerieFire, infinitePlayerMana, infinitePetMana
        , usingLashOfPainOnCooldown, petIsAggressive, prepopBlackBook, randomizeValues, userChoosingRotation, exaltedWithShattrathFaction, survivalHunterAgility, hasImmolate, hasCorruption, hasSiphonLife
        , hasUnstableAffliction, hasSearingPain, hasShadowBolt, hasIncinerate, hasCurseOfRecklessness, hasCurseOfTheElements, hasCurseOfAgony, hasCurseOfDoom, hasDeathCoil, hasShadowburn, hasConflagrate
        , hasShadowfury, hasAmplifyCurse, hasDarkPact);
}

EMSCRIPTEN_KEEPALIVE
Player* allocPlayer(PlayerSettings* settings)
{
    return new Player(settings);
}

EMSCRIPTEN_KEEPALIVE
SimulationSettings* allocSimSettings(int iterations, int minTime, int maxTime, unsigned int* randomSeeds)
{
    return new SimulationSettings(iterations, minTime, maxTime, randomSeeds);
}

EMSCRIPTEN_KEEPALIVE
Simulation* allocSim(Player* player, SimulationSettings* simulationSettings)
{
    return new Simulation(player, simulationSettings);
}

EMSCRIPTEN_KEEPALIVE
void freeUnsignedInt(unsigned int* arr)
{
    delete arr;
}

EMSCRIPTEN_KEEPALIVE
void freeItems(Items* items)
{
    delete items;
}

EMSCRIPTEN_KEEPALIVE
void freeAuras(Auras* auras)
{
    delete auras;
}

EMSCRIPTEN_KEEPALIVE
void freeTalents(Talents* talents)
{
    delete talents;
}

EMSCRIPTEN_KEEPALIVE
void freeSets(Sets* sets)
{
    delete sets;
}

EMSCRIPTEN_KEEPALIVE
void freeStats(CharacterStats* stats)
{
    delete stats;
}

EMSCRIPTEN_KEEPALIVE
void freePlayerSettings(PlayerSettings* settings)
{
    delete settings;
}

EMSCRIPTEN_KEEPALIVE
void freePlayer(Player* player)
{
    delete player->spells;
    delete player->auras;
    if (player->pet != NULL)
    {
        delete player->pet->spells;
        delete player->pet->auras;
        delete player->pet->baseStats;
        delete player->pet->buffStats;
        delete player->pet->debuffStats;
        delete player->pet->stats;
        delete player->pet;
    }
    delete player;
}

EMSCRIPTEN_KEEPALIVE
void freeSimSettings(SimulationSettings* settings)
{
    delete settings;
}

EMSCRIPTEN_KEEPALIVE
void freeSim(Simulation* sim)
{
    delete sim;
}

// I gave up on trying to get embind to work, but it would be better to handle it this way than using a bunch of booleans in the PlayerSettings struct
/*EMSCRIPTEN_BINDINGS(module)
{
    emscripten::enum_<Constant>("Constant")
        .value("ALDOR", ALDOR)
        .value("Scryers", SCRYER)
        .value("yes", YES)
        .value("no", NO)
        .value("onCooldown", ON_COOLDOWN)
        .value("singleTarget", SINGLE_TARGET)
        .value("aoe", AOE)
        .value("noISB", NO_ISB)
        .value("human", HUMAN)
        .value("gnome", GNOME)
        .value("orc", ORC)
        .value("undead", UNDEAD)
        .value("bloodElf", BLOOD_ELF)
        .value("simChooses", SIM_CHOOSES)
        .value("userChooses", USER_CHOOSES)
        ;

    emscripten::value_object<AdditionalPlayerSettings>("AdditionalPlayerSettings")
        .field("shattrathFaction", &AdditionalPlayerSettings::shattrathFaction)
        .field("race", &AdditionalPlayerSettings::race)
        .field("sacrificePet", &AdditionalPlayerSettings::sacrificePet)
        .field("improvedFaerieFire", &AdditionalPlayerSettings::improvedFaerieFire)
        .field("lashOfPainUsage", &AdditionalPlayerSettings::lashOfPainUsage)
        .field("shattrathFactionReputation", &AdditionalPlayerSettings::exaltedWithShattrathFaction)
        .field("customIsbUptime", &AdditionalPlayerSettings::usingCustomIsbUptime)
        .field("randomizeValues", &AdditionalPlayerSettings::randomizeValues)
        .field("infinitePlayerMana", &AdditionalPlayerSettings::infinitePlayerMana)
        .field("infinitePetMana", &AdditionalPlayerSettings::infinitePetMana)
        .field("prepopBlackBook", &AdditionalPlayerSettings::prepopBlackBook)
        .field("rotationOption", &AdditionalPlayerSettings::rotationOption)
        .field("fightType", &AdditionalPlayerSettings::fightType)
        ;
}*/