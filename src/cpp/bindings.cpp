#include "bindings.h"
#include "simulation.h"
#include "constant.h"
#include <random>

void dpsUpdate(double dps)
{
#ifdef EMSCRIPTEN
    EM_ASM({
        postMessage({
            event: "dpsUpdate",
            data: {
                dps: $0
            }
        })
    }, dps);
#endif
}

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

void postCombatLogBreakdownVector(const char* name, int manaGain, int damage)
{
#ifdef EMSCRIPTEN
    EM_ASM({
        postMessage({
            event: "combatLogVector",
            data: {
                name: UTF8ToString($0),
                manaGain: $1,
                damage: $2
            }
        })
    }, name, manaGain, damage);
#endif
}

void postCombatLogBreakdown(const char* name, int casts, int crits, int misses, int count, double uptime, int dodges, int glancingBlows)
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
                count: $4,
                uptime: $5,
                dodges: $6,
                glancingBlows: $7,
                damage: 0,
                manaGain: 0
            }
        })
    }, name, casts, crits, misses, count, uptime, dodges, glancingBlows);
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

void simulationUpdate(int iteration, int iterationAmount, double medianDps, int itemId, const char* customStat)
{
#ifdef EMSCRIPTEN
    EM_ASM({
        postMessage({
            event: "update",
            data: {
                medianDps: $0,
                iteration: $1,
                iterationAmount: $2,
                itemId: $3,
                customStat: UTF8ToString($4)
            }
        })
    }, medianDps, iteration, iterationAmount, itemId, customStat);
#else
    std::cout << "Iteration: " << std::to_string(iteration) << "/" << std::to_string(iterationAmount) << ". Median DPS: " << std::to_string(medianDps) << std::endl;
#endif
}

void simulationEnd(double medianDps, double minDps, double maxDps, int itemId, int iterationAmount, int totalDuration, const char* customStat)
{
#ifdef EMSCRIPTEN
    EM_ASM({
        postMessage({
            event: "end",
            data: {
                medianDps: $0,
                minDps: $1,
                maxDps: $2,
                itemId: $3,
                iterationAmount: $4,
                totalDuration: $5,
                customStat: UTF8ToString($6)
            }
        })
    }, medianDps, minDps, maxDps, itemId, iterationAmount, totalDuration, customStat);
#else
    std::cout << "Median DPS: " << std::to_string(medianDps) << ". Min DPS: " << std::to_string(minDps) << ". Max DPS: " << std::to_string(maxDps) << std::endl;
#endif
}

std::shared_ptr<uint32_t[]> allocRandomSeeds(int amountOfSeeds)
{
    srand(time(nullptr));
    std::shared_ptr<uint32_t[]> seeds = std::make_shared<uint32_t[]>(10000);

    for (int i = 0; i < amountOfSeeds; i++)
    {
        seeds[i] = rand();
    }

    return seeds;
}

std::shared_ptr<Items> allocItems(int head, int neck, int shoulders, int back, int chest, int bracers, int gloves, int belt, int legs, int boots, int finger1, int finger2, int trinket1, int trinket2, int mainHand, int offHand, int twoHand, int wand)
{
    std::cout << "allocItems" << std::endl;
    std::cout << "head: " << std::to_string(head) << std::endl;
    return std::make_shared<Items>(head, neck, shoulders, back, chest, bracers, gloves, belt, legs, boots, finger1, finger2, trinket1, trinket2, mainHand, offHand, twoHand, wand);
}

std::shared_ptr<Auras> allocAuras(bool felArmor, bool blessingOfKings, bool blessingOfWisdom, bool judgementOfWisdom, bool manaSpringTotem, bool wrathOfAirTotem, bool totemOfWrath, bool markOfTheWild, bool arcaneIntellect, bool prayerOfFortitude, bool prayerOfSpirit, bool bloodPact, bool inspiringPresence, bool moonkinAura, bool powerInfusion, bool powerOfTheGuardianWarlock, bool powerOfTheGuardianMage, bool eyeOfTheNight, bool chainOfTheTwilightOwl, bool jadePendantOfBlasting, bool idolOfTheRavenGoddess, bool drumsOfBattle, bool drumsOfWar, bool drumsOfRestoration, bool bloodlust, bool ferociousInspiration, bool innervate, bool curseOfTheElements, bool shadowWeaving, bool improvedScorch, bool misery, bool judgementOfTheCrusader, bool vampiricTouch, bool faerieFire, bool sunderArmor, bool exposeArmor, bool curseOfRecklessness, bool bloodFrenzy, bool exposeWeakness, bool annihilator, bool improvedHuntersMark, bool flaskOfPureDeath, bool elixirOfMajorShadowPower, bool elixirOfMajorFirepower, bool greaterArcaneElixir, bool adeptsElixir, bool elixirOfDraenicWisdom, bool elixirOfMajorMageblood, bool superManaPotion, bool destructionPotion, bool brilliantWizardOil, bool superiorWizardOil, bool blessedWizardOil, bool demonicRune, bool flameCap, bool rumseyRumBlackLabel, bool kreegsStoutBeatdown, bool blackenedBasilisk, bool skullfishSoup, bool veryBerryCream, bool midsummerSausage, bool bloodthistle, bool petBlessingOfKings, bool petBlessingOfWisdom, bool petBlessingOfMight, bool petArcaneIntellect, bool petMarkOfTheWild, bool petPrayerOfFortitude, bool petPrayerOfSpirit, bool petKiblersBits, bool petHeroicPresence, bool petStrengthOfEarthTotem, bool petGraceOfAirTotem, bool petBattleShout, bool petTrueshotAura, bool petLeaderOfThePack, bool petUnleashedRage, bool petStaminaScroll, bool petIntellectScroll, bool petStrengthScroll, bool petAgilityScroll, bool petSpiritScroll)
{
    return std::make_shared<Auras>(felArmor, blessingOfKings, blessingOfWisdom, judgementOfWisdom, manaSpringTotem, wrathOfAirTotem, totemOfWrath, markOfTheWild, arcaneIntellect, prayerOfFortitude, prayerOfSpirit, bloodPact, inspiringPresence, moonkinAura, powerInfusion, powerOfTheGuardianWarlock, powerOfTheGuardianMage, eyeOfTheNight, chainOfTheTwilightOwl, jadePendantOfBlasting, idolOfTheRavenGoddess, drumsOfBattle, drumsOfWar, drumsOfRestoration, bloodlust, ferociousInspiration, innervate, curseOfTheElements, shadowWeaving, improvedScorch, misery, judgementOfTheCrusader, vampiricTouch, faerieFire, sunderArmor, exposeArmor, curseOfRecklessness, bloodFrenzy, exposeWeakness, annihilator, improvedHuntersMark, flaskOfPureDeath, elixirOfMajorShadowPower, elixirOfMajorFirepower, greaterArcaneElixir, adeptsElixir, elixirOfDraenicWisdom, elixirOfMajorMageblood, superManaPotion, destructionPotion, brilliantWizardOil, superiorWizardOil, blessedWizardOil, demonicRune, flameCap, rumseyRumBlackLabel, kreegsStoutBeatdown, blackenedBasilisk, skullfishSoup, veryBerryCream, midsummerSausage, bloodthistle, petBlessingOfKings, petBlessingOfWisdom, petBlessingOfMight, petArcaneIntellect, petMarkOfTheWild, petPrayerOfFortitude, petPrayerOfSpirit, petKiblersBits, petHeroicPresence, petStrengthOfEarthTotem, petGraceOfAirTotem, petBattleShout, petTrueshotAura, petLeaderOfThePack, petUnleashedRage, petStaminaScroll, petIntellectScroll, petStrengthScroll, petAgilityScroll, petSpiritScroll);
}

std::shared_ptr<Talents> allocTalents(int suppression, int improvedCorruption, int improvedLifeTap, int improvedCurseOfAgony, int amplifyCurse, int nightfall, int empoweredCorruption, int siphonLife, int shadowMastery, int contagion, int darkPact, int unstableAffliction, int improvedImp, int demonicEmbrace, int felIntellect, int felStamina, int improvedSuccubus, int demonicAegis, int unholyPower, int demonicSacrifice, int manaFeed, int masterDemonologist, int soulLink, int demonicKnowledge, int demonicTactics, int felguard, int improvedShadowBolt, int cataclysm, int bane, int improvedFirebolt, int improvedLashOfPain, int devastation, int shadowburn, int improvedSearingPain, int improvedImmolate, int ruin, int emberstorm, int backlash, int conflagrate, int shadowAndFlame, int shadowfury)
{
    return std::make_shared<Talents>(suppression, improvedCorruption, improvedLifeTap, improvedCurseOfAgony, amplifyCurse, nightfall, empoweredCorruption, siphonLife, shadowMastery, contagion, darkPact, unstableAffliction, improvedImp, demonicEmbrace, felIntellect, felStamina, improvedSuccubus, demonicAegis, unholyPower, demonicSacrifice, manaFeed, masterDemonologist, soulLink, demonicKnowledge, demonicTactics, felguard, improvedShadowBolt, cataclysm, bane, improvedFirebolt, improvedLashOfPain, devastation, shadowburn, improvedSearingPain, improvedImmolate, ruin, emberstorm, backlash, conflagrate, shadowAndFlame, shadowfury);
}

std::shared_ptr<Sets> allocSets(int plagueheart, int spellfire, int frozenShadoweave, int spellstrike, int oblivion, int manaEtched, int twinStars, int t4, int t5, int t6)
{
    return std::make_shared<Sets>(plagueheart, spellfire, frozenShadoweave, spellstrike, oblivion, manaEtched, twinStars, t4, t5, t6);
}

std::shared_ptr<CharacterStats> allocStats(int health, int mana, double stamina, double intellect, double spirit, double spellPower, int shadowPower, int firePower, int hasteRating, int hitRating, int critRating, int mp5, int spellPen, double fireModifier, double frostModifier, double shadowModifier, double staminaModifier, double intellectModifier, double spiritModifier, double arcaneModifier, double natureModifier, int natureResist, int arcaneResist, int fireResist, int frostResist, int shadowResist)
{
    return std::make_shared<CharacterStats>(health, mana, stamina, intellect, spirit, spellPower, shadowPower, firePower, hasteRating, hitRating, critRating, mp5, spellPen, fireModifier, frostModifier, shadowModifier, staminaModifier, intellectModifier, spiritModifier, arcaneModifier, natureModifier, natureResist, arcaneResist, fireResist, frostResist, shadowResist);
}

std::shared_ptr<PlayerSettings> allocPlayerSettings(std::shared_ptr<Auras> auras, std::shared_ptr<Talents> talents, std::shared_ptr<Sets> sets, std::shared_ptr<CharacterStats> stats, std::shared_ptr<Items> items, int itemId, int metaGemId, bool equippedItemSimulation, bool recordingCombatLogBreakdown, bool simmingStamina, bool simmingIntellect, bool simmingSpirit, bool simmingSpellPower, bool simmingShadowPower, bool simmingFirePower, bool simmingHitRating, bool simmingCritRating, bool simmingHasteRating, bool simmingMp5, bool isAldor, int enemyLevel, int enemyShadowResist, int enemyFireResist, int mageAtieshAmount, int totemOfWrathAmount, bool sacrificingPet, bool petIsImp, bool petIsSuccubus, bool petIsFelguard, int ferociousInspirationAmount, int improvedCurseOfTheElements, bool usingCustomIsbUptime, int customIsbUptimeValue, int improvedDivineSpirit, int improvedImp, int shadowPriestDps, int warlockAtieshAmount, int improvedExposeArmor, bool isSingleTarget, int enemyAmount, bool isOrc, int powerInfusionAmount, bool bloodlustAmount, bool innervateAmount, int enemyArmor, int exposeWeaknessUptime, bool improvedFaerieFire, bool infinitePlayerMana, bool infinitePetMana, bool usingLashOfPainOnCooldown, bool petIsAggressive, bool prepopBlackBook, bool randomizeValues, bool userChoosingRotation, bool exaltedWithShattrathFaction, int survivalHunterAgility, bool hasImmolate, bool hasCorruption, bool hasSiphonLife, bool hasUnstableAffliction, bool hasSearingPain, bool hasShadowBolt, bool hasIncinerate, bool hasCurseOfRecklessness, bool hasCurseOfTheElements, bool hasCurseOfAgony, bool hasCurseOfDoom, bool hasDeathCoil, bool hasShadowburn, bool hasConflagrate, bool hasShadowfury, bool hasAmplifyCurse, bool hasDarkPact, bool hasElementalShamanT4Bonus)
{
    return std::make_shared<PlayerSettings>(auras, talents, sets, stats, items, itemId, metaGemId, equippedItemSimulation, recordingCombatLogBreakdown, simmingStamina, simmingIntellect, simmingSpirit, simmingSpellPower, simmingShadowPower, simmingFirePower, simmingHitRating, simmingCritRating, simmingHasteRating, simmingMp5, isAldor, enemyLevel, enemyShadowResist, enemyFireResist, mageAtieshAmount, totemOfWrathAmount, sacrificingPet, petIsImp, petIsSuccubus, petIsFelguard, ferociousInspirationAmount, improvedCurseOfTheElements, usingCustomIsbUptime, customIsbUptimeValue, improvedDivineSpirit, improvedImp, shadowPriestDps, warlockAtieshAmount, improvedExposeArmor, isSingleTarget, enemyAmount, isOrc, powerInfusionAmount, bloodlustAmount, innervateAmount, enemyArmor, exposeWeaknessUptime, improvedFaerieFire, infinitePlayerMana, infinitePetMana, usingLashOfPainOnCooldown, petIsAggressive, prepopBlackBook, randomizeValues, userChoosingRotation, exaltedWithShattrathFaction, survivalHunterAgility, hasImmolate, hasCorruption, hasSiphonLife, hasUnstableAffliction, hasSearingPain, hasShadowBolt, hasIncinerate, hasCurseOfRecklessness, hasCurseOfTheElements, hasCurseOfAgony, hasCurseOfDoom, hasDeathCoil, hasShadowburn, hasConflagrate, hasShadowfury, hasAmplifyCurse, hasDarkPact, hasElementalShamanT4Bonus);
}

std::shared_ptr<Player> allocPlayer(std::shared_ptr<PlayerSettings> settings)
{
    return std::make_shared<Player>(settings);
}

std::shared_ptr<SimulationSettings> allocSimSettings(int iterations, int minTime, int maxTime, std::shared_ptr<uint32_t[]> randomSeeds, SimulationType simulationType)
{
    return std::make_shared<SimulationSettings>(iterations, minTime, maxTime, randomSeeds, simulationType);
}

std::shared_ptr<Simulation> allocSim(std::shared_ptr<Player> player, std::shared_ptr<SimulationSettings> simulationSettings)
{
    return std::make_shared<Simulation>(player, simulationSettings);
}

#ifdef EMSCRIPTEN
EMSCRIPTEN_BINDINGS(module)
{
    emscripten::class_<Player>("Player")
        .smart_ptr<std::shared_ptr<Player>>("Player");

    emscripten::class_<Simulation>("Simulation")
        .smart_ptr<std::shared_ptr<Simulation>>("Simulation")
        .property("player", &Simulation::player)
        .property("simSettings", &Simulation::settings)
        .function("start", &Simulation::start);

    emscripten::class_<Items>("Items")
        .smart_ptr<std::shared_ptr<Items>>("Items")
        .property("head", &Items::head)
        .property("neck", &Items::neck)
        .property("shoulders", &Items::shoulders)
        .property("back", &Items::back)
        .property("chest", &Items::chest)
        .property("bracer", &Items::bracers)
        .property("gloves", &Items::gloves)
        .property("belt", &Items::belt)
        .property("legs", &Items::legs)
        .property("boots", &Items::boots)
        .property("ring1", &Items::ring1)
        .property("ring2", &Items::ring2)
        .property("trinket1", &Items::trinket1)
        .property("trinket2", &Items::trinket2)
        .property("mainhand", &Items::mainHand)
        .property("offhand", &Items::offHand)
        .property("twohand", &Items::twoHand)
        .property("wand", &Items::wand);

    emscripten::class_<Auras>("Auras")
        .smart_ptr<std::shared_ptr<Auras>>("Auras")
        .property("felArmor", &Auras::felArmor)
        .property("blessingOfKings", &Auras::blessingOfKings)
        .property("blessingOfWisdom", &Auras::blessingOfWisdom)
        .property("judgementOfWisdom", &Auras::judgementOfWisdom)
        .property("manaSpringTotem", &Auras::manaSpringTotem)
        .property("wrathOfAirTotem", &Auras::wrathOfAirTotem)
        .property("totemOfWrath", &Auras::totemOfWrath)
        .property("markOfTheWild", &Auras::markOfTheWild)
        .property("arcaneIntellect", &Auras::arcaneIntellect)
        .property("prayerOfFortitude", &Auras::prayerOfFortitude)
        .property("prayerOfSpirit", &Auras::prayerOfSpirit)
        .property("bloodPact", &Auras::bloodPact)
        .property("inspiringPresence", &Auras::inspiringPresence)
        .property("moonkinAura", &Auras::moonkinAura)
        .property("powerInfusion", &Auras::powerInfusion)
        .property("powerOfTheGuardianWarlock", &Auras::powerOfTheGuardianWarlock)
        .property("powerOfTheGuardianMage", &Auras::powerOfTheGuardianMage)
        .property("eyeOfTheNight", &Auras::eyeOfTheNight)
        .property("chainOfTheTwilightOwl", &Auras::chainOfTheTwilightOwl)
        .property("jadePendantOfBlasting", &Auras::jadePendantOfBlasting)
        .property("idolOfTheRavenGoddess", &Auras::idolOfTheRavenGoddess)
        .property("drumsOfBattle", &Auras::drumsOfBattle)
        .property("drumsOfWar", &Auras::drumsOfWar)
        .property("drumsOfRestoration", &Auras::drumsOfRestoration)
        .property("bloodlust", &Auras::bloodlust)
        .property("ferociousInspiration", &Auras::ferociousInspiration)
        .property("innervate", &Auras::innervate)
        .property("curseOfTheElements", &Auras::curseOfTheElements)
        .property("shadowWeaving", &Auras::shadowWeaving)
        .property("improvedScorch", &Auras::improvedScorch)
        .property("misery", &Auras::misery)
        .property("judgementOfTheCrusader", &Auras::judgementOfTheCrusader)
        .property("vampiricTouch", &Auras::vampiricTouch)
        .property("faerieFire", &Auras::faerieFire)
        .property("sunderArmor", &Auras::sunderArmor)
        .property("exposeArmor", &Auras::exposeArmor)
        .property("curseOfRecklessness", &Auras::curseOfRecklessness)
        .property("bloodFrenzy", &Auras::bloodFrenzy)
        .property("exposeWeakness", &Auras::exposeWeakness)
        .property("annihilator", &Auras::annihilator)
        .property("improvedHuntersMark", &Auras::improvedHuntersMark)
        .property("flaskOfPureDeath", &Auras::flaskOfPureDeath)
        .property("elixirOfMajorShadowPower", &Auras::elixirOfMajorShadowPower)
        .property("elixirOfMajorFirepower", &Auras::elixirOfMajorFirepower)
        .property("greaterArcaneElixir", &Auras::greaterArcaneElixir)
        .property("adeptsElixir", &Auras::adeptsElixir)
        .property("elixirOfDraenicWisdom", &Auras::elixirOfDraenicWisdom)
        .property("elixirOfMajorMageblood", &Auras::elixirOfMajorMageblood)
        .property("superManaPotion", &Auras::superManaPotion)
        .property("destructionPotion", &Auras::destructionPotion)
        .property("brilliantWizardOil", &Auras::brilliantWizardOil)
        .property("superiorWizardOil", &Auras::superiorWizardOil)
        .property("blessedWizardOil", &Auras::blessedWizardOil)
        .property("demonicRune", &Auras::demonicRune)
        .property("flameCap", &Auras::flameCap)
        .property("rumseyRumBlackLabel", &Auras::rumseyRumBlackLabel)
        .property("kreegsStoutBeatdown", &Auras::kreegsStoutBeatdown)
        .property("blackenedBasilisk", &Auras::blackenedBasilisk)
        .property("skullfishSoup", &Auras::skullfishSoup)
        .property("veryBerryCream", &Auras::veryBerryCream)
        .property("midsummerSausage", &Auras::midsummerSausage)
        .property("bloodthistle", &Auras::bloodthistle)
        .property("petBlessingOfKings", &Auras::petBlessingOfKings)
        .property("petBlessingOfWisdom", &Auras::petBlessingOfWisdom)
        .property("petBlessingOfMight", &Auras::petBlessingOfMight)
        .property("petArcaneIntellect", &Auras::petArcaneIntellect)
        .property("petMarkOfTheWild", &Auras::petMarkOfTheWild)
        .property("petPrayerOfFortitude", &Auras::petPrayerOfFortitude)
        .property("petPrayerOfSpirit", &Auras::petPrayerOfSpirit)
        .property("petKiblersBits", &Auras::petKiblersBits)
        .property("petHeroicPresence", &Auras::petHeroicPresence)
        .property("petStrengthOfEarthTotem", &Auras::petStrengthOfEarthTotem)
        .property("petGraceOfAirTotem", &Auras::petGraceOfAirTotem)
        .property("petBattleShout", &Auras::petBattleShout)
        .property("petTrueshotAura", &Auras::petTrueshotAura)
        .property("petLeaderOfThePack", &Auras::petLeaderOfThePack)
        .property("petUnleashedRage", &Auras::petUnleashedRage)
        .property("petStaminaScroll", &Auras::petStaminaScroll)
        .property("petIntellectScroll", &Auras::petIntellectScroll)
        .property("petStrengthScroll", &Auras::petStrengthScroll)
        .property("petAgilityScroll", &Auras::petAgilityScroll)
        .property("petSpiritScroll", &Auras::petSpiritScroll);

    emscripten::class_<Talents>("Talents")
        .smart_ptr<std::shared_ptr<Talents>>("Talents")
        .property("suppression", &Talents::suppression)
        .property("improvedCorruption", &Talents::improvedCorruption)
        .property("improvedLifeTap", &Talents::improvedLifeTap)
        .property("improvedCurseOfAgony", &Talents::improvedCurseOfAgony)
        .property("amplifyCurse", &Talents::amplifyCurse)
        .property("nightfall", &Talents::nightfall)
        .property("empoweredCorruption", &Talents::empoweredCorruption)
        .property("siphonLife", &Talents::siphonLife)
        .property("shadowMastery", &Talents::shadowMastery)
        .property("contagion", &Talents::contagion)
        .property("darkPact", &Talents::darkPact)
        .property("unstableAffliction", &Talents::unstableAffliction)
        .property("improvedImp", &Talents::improvedImp)
        .property("demonicEmbrace", &Talents::demonicEmbrace)
        .property("felIntellect", &Talents::felIntellect)
        .property("felStamina", &Talents::felStamina)
        .property("improvedSuccubus", &Talents::improvedSuccubus)
        .property("demonicAegis", &Talents::demonicAegis)
        .property("unholyPower", &Talents::unholyPower)
        .property("demonicSacrifice", &Talents::demonicSacrifice)
        .property("manaFeed", &Talents::manaFeed)
        .property("masterDemonologist", &Talents::masterDemonologist)
        .property("soulLink", &Talents::soulLink)
        .property("demonicKnowledge", &Talents::demonicKnowledge)
        .property("demonicTactics", &Talents::demonicTactics)
        .property("felguard", &Talents::felguard)
        .property("improvedShadowBolt", &Talents::improvedShadowBolt)
        .property("cataclysm", &Talents::cataclysm)
        .property("bane", &Talents::bane)
        .property("improvedFirebolt", &Talents::improvedFirebolt)
        .property("improvedLashOfPain", &Talents::improvedLashOfPain)
        .property("devastation", &Talents::devastation)
        .property("shadowburn", &Talents::shadowburn)
        .property("improvedSearingPain", &Talents::improvedSearingPain)
        .property("improvedImmolate", &Talents::improvedImmolate)
        .property("ruin", &Talents::ruin)
        .property("emberstorm", &Talents::emberstorm)
        .property("backlash", &Talents::backlash)
        .property("conflagrate", &Talents::conflagrate)
        .property("shadowAndFlame", &Talents::shadowAndFlame)
        .property("shadowfury", &Talents::shadowfury);

    emscripten::class_<Sets>("Sets")
        .smart_ptr<std::shared_ptr<Sets>>("Sets")
        .property("plagueheart", &Sets::plagueheart)
        .property("spellfire", &Sets::spellfire)
        .property("frozenShadoweave", &Sets::frozenShadoweave)
        .property("spellstrike", &Sets::spellstrike)
        .property("oblivion", &Sets::oblivion)
        .property("manaEtched", &Sets::manaEtched)
        .property("twinStars", &Sets::twinStars)
        .property("t4", &Sets::t4)
        .property("t5", &Sets::t5)
        .property("t6", &Sets::t6);

    emscripten::class_<CharacterStats>("CharacterStats")
        .smart_ptr<std::shared_ptr<CharacterStats>>("CharacterStats")
        .property("health", &CharacterStats::health)
        .property("mana", &CharacterStats::mana)
        .property("maxMana", &CharacterStats::maxMana)
        .property("stamina", &CharacterStats::stamina)
        .property("intellect", &CharacterStats::intellect)
        .property("spirit", &CharacterStats::spirit)
        .property("spellPower", &CharacterStats::spellPower)
        .property("shadowPower", &CharacterStats::shadowPower)
        .property("firePower", &CharacterStats::firePower)
        .property("hasteRating", &CharacterStats::hasteRating)
        .property("hitRating", &CharacterStats::hitRating)
        .property("critRating", &CharacterStats::critRating)
        .property("mp5", &CharacterStats::mp5)
        .property("spellPen", &CharacterStats::spellPen)
        .property("fireModifier", &CharacterStats::fireModifier)
        .property("frostModifier", &CharacterStats::frostModifier)
        .property("shadowModifier", &CharacterStats::shadowModifier)
        .property("staminaModifier", &CharacterStats::staminaModifier)
        .property("intellectModifier", &CharacterStats::intellectModifier)
        .property("spiritModifier", &CharacterStats::spiritModifier)
        .property("arcaneModifier", &CharacterStats::arcaneModifier)
        .property("natureModifier", &CharacterStats::natureModifier)
        .property("natureResist", &CharacterStats::natureResist)
        .property("arcaneResist", &CharacterStats::arcaneResist)
        .property("fireResist", &CharacterStats::fireResist)
        .property("frostResist", &CharacterStats::frostResist)
        .property("shadowResist", &CharacterStats::shadowResist);

    emscripten::class_<PlayerSettings>("PlayerSettings")
        .smart_ptr<std::shared_ptr<PlayerSettings>>("PlayerSettings")
        .property("auras", &PlayerSettings::auras)
        .property("talents", &PlayerSettings::talents)
        .property("sets", &PlayerSettings::sets)
        .property("stats", &PlayerSettings::stats)
        .property("items", &PlayerSettings::items)
        .property("itemId", &PlayerSettings::itemId)
        .property("metaGemId", &PlayerSettings::metaGemId)
        .property("equippedItemSimulation", &PlayerSettings::equippedItemSimulation)
        .property("recordingCombatLogBreakdown", &PlayerSettings::recordingCombatLogBreakdown)
        .property("simmingStamina", &PlayerSettings::simmingStamina)
        .property("simmingIntellect", &PlayerSettings::simmingIntellect)
        .property("simmingSpirit", &PlayerSettings::simmingSpirit)
        .property("simmingSpellPower", &PlayerSettings::simmingSpellPower)
        .property("simmingShadowPower", &PlayerSettings::simmingShadowPower)
        .property("simmingFirePower", &PlayerSettings::simmingFirePower)
        .property("simmingHitRating", &PlayerSettings::simmingHitRating)
        .property("simmingCritRating", &PlayerSettings::simmingCritRating)
        .property("simmingHasteRating", &PlayerSettings::simmingHasteRating)
        .property("simmingMp5", &PlayerSettings::simmingMp5)
        .property("isAldor", &PlayerSettings::isAldor)
        .property("enemyLevel", &PlayerSettings::enemyLevel)
        .property("enemyShadowResist", &PlayerSettings::enemyShadowResist)
        .property("enemyFireResist", &PlayerSettings::enemyFireResist)
        .property("mageAtieshAmount", &PlayerSettings::mageAtieshAmount)
        .property("totemOfWrathAmount", &PlayerSettings::totemOfWrathAmount)
        .property("sacrificingPet", &PlayerSettings::sacrificingPet)
        .property("petIsImp", &PlayerSettings::petIsImp)
        .property("petIsSuccubus", &PlayerSettings::petIsSuccubus)
        .property("petIsFelguard", &PlayerSettings::petIsFelguard)
        .property("ferociousInspirationAmount", &PlayerSettings::ferociousInspirationAmount)
        .property("improvedCurseOfTheElements", &PlayerSettings::improvedCurseOfTheElements)
        .property("usingCustomIsbUptime", &PlayerSettings::usingCustomIsbUptime)
        .property("customIsbUptimeValue", &PlayerSettings::customIsbUptimeValue)
        .property("improvedDivineSpirit", &PlayerSettings::improvedDivineSpirit)
        .property("improvedImp", &PlayerSettings::improvedImp)
        .property("shadowPriestDps", &PlayerSettings::shadowPriestDps)
        .property("warlockAtieshAmount", &PlayerSettings::warlockAtieshAmount)
        .property("improvedExposeArmor", &PlayerSettings::improvedExposeArmor)
        .property("isSingleTarget", &PlayerSettings::isSingleTarget)
        .property("enemyAmount", &PlayerSettings::enemyAmount)
        .property("isOrc", &PlayerSettings::isOrc)
        .property("powerInfusionAmount", &PlayerSettings::powerInfusionAmount)
        .property("bloodlustAmount", &PlayerSettings::bloodlustAmount)
        .property("innervateAmount", &PlayerSettings::innervateAmount)
        .property("enemyArmor", &PlayerSettings::enemyArmor)
        .property("exposeWeaknessUptime", &PlayerSettings::exposeWeaknessUptime)
        .property("improvedFaerieFire", &PlayerSettings::improvedFaerieFire)
        .property("infinitePlayerMana", &PlayerSettings::infinitePlayerMana)
        .property("infinitePetMana", &PlayerSettings::infinitePetMana)
        .property("usingLashOfPainOnCooldown", &PlayerSettings::usingLashOfPainOnCooldown)
        .property("petIsAggressive", &PlayerSettings::petIsAggressive)
        .property("prepopBlackBook", &PlayerSettings::prepopBlackBook)
        .property("randomizeValues", &PlayerSettings::randomizeValues)
        .property("simChoosingRotation", &PlayerSettings::simChoosingRotation)
        .property("exaltedWithShattrathFaction", &PlayerSettings::exaltedWithShattrathFaction)
        .property("survivalHunterAgility", &PlayerSettings::survivalHunterAgility)
        .property("hasImmolate", &PlayerSettings::hasImmolate)
        .property("hasCorruption", &PlayerSettings::hasCorruption)
        .property("hasSiphonLife", &PlayerSettings::hasSiphonLife)
        .property("hasUnstableAffliction", &PlayerSettings::hasUnstableAffliction)
        .property("hasSearingPain", &PlayerSettings::hasSearingPain)
        .property("hasShadowBolt", &PlayerSettings::hasShadowBolt)
        .property("hasIncinerate", &PlayerSettings::hasIncinerate)
        .property("hasCurseOfRecklessness", &PlayerSettings::hasCurseOfRecklessness)
        .property("hasCurseOfTheElements", &PlayerSettings::hasCurseOfTheElements)
        .property("hasCurseOfAgony", &PlayerSettings::hasCurseOfAgony)
        .property("hasCurseOfDoom", &PlayerSettings::hasCurseOfDoom)
        .property("hasDeathCoil", &PlayerSettings::hasDeathCoil)
        .property("hasShadowburn", &PlayerSettings::hasShadowburn)
        .property("hasConflagrate", &PlayerSettings::hasConflagrate)
        .property("hasShadowfury", &PlayerSettings::hasShadowfury)
        .property("hasAmplifyCurse", &PlayerSettings::hasAmplifyCurse)
        .property("hasDarkPact", &PlayerSettings::hasDarkPact)
        .property("hasElementalShamanT4Bonus", &PlayerSettings::hasElementalShamanT4Bonus);

    emscripten::class_<SimulationSettings>("SimulationSettings")
        .smart_ptr<std::shared_ptr<SimulationSettings>>("SimulationSettings")
        .property("iterations", &SimulationSettings::iterations)
        .property("minTime", &SimulationSettings::minTime)
        .property("maxTime", &SimulationSettings::maxTime)
        .property("randomSeeds", &SimulationSettings::randomSeeds)
        .property("simulationType", &SimulationSettings::simulationType);

    emscripten::enum_<SimulationType>("SimulationType")
        .value("normal", SimulationType::NORMAL)
        .value("allItems", SimulationType::ALL_ITEMS)
        .value("statWeights", SimulationType::STAT_WEIGHTS);

    emscripten::function("allocItems", &allocItems);
    emscripten::function("allocAuras", &allocAuras);
    emscripten::function("allocTalents", &allocTalents);
    emscripten::function("allocSets", &allocSets);
    emscripten::function("allocStats", &allocStats);
    emscripten::function("allocPlayerSettings", &allocPlayerSettings);
    emscripten::function("allocPlayer", &allocPlayer);
    emscripten::function("allocSimSettings", &allocSimSettings);
    emscripten::function("allocSim", &allocSim);
    emscripten::function("allocRandomSeeds", &allocRandomSeeds);

    /*emscripten::enum_<Constant>("Constant")
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
        ;*/
}
#endif