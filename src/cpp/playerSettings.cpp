#include "playerSettings.h"
#include "auras.h"
#include "talents.h"
#include "sets.h"
#include "characterStats.h"
#include "items.h"

PlayerSettings::PlayerSettings(Auras* auras, Talents* talents, Sets* sets, CharacterStats* stats, Items* items, int itemId, int metaGemId, bool recordingCombatLogBreakdown, bool simmingStamina, bool simmingIntellect, bool simmingSpellPower
    , bool simmingShadowPower, bool simmingFirePower, bool simmingHitRating, bool simmingCritRating, bool simmingHasteRating, bool simmingMp5, bool isAldor, int enemyLevel, int enemyShadowResist
    , int enemyFireResist, int mageAtieshAmount, int totemOfWrathAmount, bool sacrificingPet, bool petIsImp, bool petIsSuccubus, bool petIsFelguard, int ferociousInspirationAmount, int improvedCurseOfTheElements
    , bool usingCustomIsbUptime, int customIsbUptimeValue, int improvedDivineSpirit, int improvedImp, int shadowPriestDps, int warlockAtieshAmount, int improvedExposeArmor, bool isSingleTarget, int enemyAmount
    , bool isOrc, int powerInfusionAmount, bool bloodlustAmount, bool innervateAmount, int enemyArmor, int exposeWeaknessUptime, bool improvedFaerieFire, bool infinitePlayerMana, bool infinitePetMana
    , bool usingLashOfPainOnCooldown, bool petIsAggressive, bool prepopBlackBook, bool randomizeValues, bool simChoosingRotation, bool exaltedWithShattrathFaction, int survivalHunterAgility
    , bool hasImmolate, bool hasCorruption, bool hasSiphonLife, bool hasUnstableAffliction, bool hasSearingPain, bool hasShadowBolt, bool hasIncinerate, bool hasCurseOfRecklessness
    , bool hasCurseOfTheElements, bool hasCurseOfAgony, bool hasCurseOfDoom, bool hasDeathCoil, bool hasShadowburn, bool hasConflagrate, bool hasShadowfury, bool hasAmplifyCurse
    , bool hasDarkPact, bool hasElementalShamanT4Bonus)
  : auras(auras), talents(talents), sets(sets), stats(stats), items(items), itemId(itemId), metaGemId(metaGemId), recordingCombatLogBreakdown(recordingCombatLogBreakdown), simmingStamina(simmingStamina), simmingIntellect(simmingIntellect)
    , simmingSpellPower(simmingSpellPower), simmingShadowPower(simmingShadowPower), simmingFirePower(simmingFirePower), simmingHitRating(simmingHitRating), simmingCritRating(simmingCritRating)
    , simmingHasteRating(simmingHasteRating), simmingMp5(simmingMp5), isAldor(isAldor), enemyLevel(enemyLevel), enemyShadowResist(enemyShadowResist), enemyFireResist(enemyFireResist)
    , mageAtieshAmount(mageAtieshAmount), totemOfWrathAmount(totemOfWrathAmount), sacrificingPet(sacrificingPet), petIsImp(petIsImp), petIsSuccubus(petIsSuccubus), petIsFelguard(petIsFelguard)
    , ferociousInspirationAmount(ferociousInspirationAmount), improvedCurseOfTheElements(improvedCurseOfTheElements), usingCustomIsbUptime(usingCustomIsbUptime), customIsbUptimeValue(customIsbUptimeValue)
    , improvedDivineSpirit(improvedDivineSpirit), improvedImp(improvedImp), shadowPriestDps(shadowPriestDps), warlockAtieshAmount(warlockAtieshAmount), improvedExposeArmor(improvedExposeArmor)
    , isSingleTarget(isSingleTarget), enemyAmount(enemyAmount), isOrc(isOrc), powerInfusionAmount(powerInfusionAmount), bloodlustAmount(bloodlustAmount), innervateAmount(innervateAmount), enemyArmor(enemyArmor)
    ,exposeWeaknessUptime(exposeWeaknessUptime), improvedFaerieFire(improvedFaerieFire), infinitePlayerMana(infinitePlayerMana), infinitePetMana(infinitePetMana), usingLashOfPainOnCooldown(usingLashOfPainOnCooldown)
    , petIsAggressive(petIsAggressive), prepopBlackBook(prepopBlackBook), randomizeValues(randomizeValues), simChoosingRotation(simChoosingRotation), exaltedWithShattrathFaction(exaltedWithShattrathFaction)
    , survivalHunterAgility(survivalHunterAgility), hasImmolate(hasImmolate), hasCorruption(hasCorruption), hasSiphonLife(hasSiphonLife), hasUnstableAffliction(hasUnstableAffliction)
    , hasSearingPain(hasSearingPain), hasShadowBolt(hasShadowBolt), hasIncinerate(hasIncinerate), hasCurseOfRecklessness(hasCurseOfRecklessness), hasCurseOfTheElements(hasCurseOfTheElements)
    , hasCurseOfAgony(hasCurseOfAgony), hasCurseOfDoom(hasCurseOfDoom), hasDeathCoil(hasDeathCoil), hasShadowburn(hasShadowburn), hasConflagrate(hasConflagrate), hasShadowfury(hasShadowfury)
    , hasAmplifyCurse(hasAmplifyCurse), hasDarkPact(hasDarkPact), hasElementalShamanT4Bonus(hasElementalShamanT4Bonus) {}