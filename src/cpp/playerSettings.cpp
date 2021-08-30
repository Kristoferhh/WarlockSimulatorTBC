#include "playerSettings.h"
#include "auras.h"
#include "talents.h"
#include "sets.h"
#include "characterStats.h"

PlayerSettings::PlayerSettings(Auras* auras, Talents* talents, Sets* sets, CharacterStats* stats, int itemId, int metaGemId, bool isAldor, int enemyLevel, int enemyShadowResist, int enemyFireResist
    , int mageAtieshAmount, int totemOfWrathAmount, bool sacrificingPet, bool petIsImp, bool petIsSuccubus, bool petIsFelguard, int ferociousInspirationAmount, int improvedCurseOfTheElements
    , bool usingCustomIsbUptime, int customIsbUptimeValue, int improvedDivineSpirit, int improvedImp, int shadowPriestDps, int warlockAtieshAmount, int improvedExposeArmor, bool isSingleTarget, int enemyAmount
    , bool isOrc, int powerInfusionAmount, bool bloodlustAmount, bool innervateAmount, int enemyArmor, int exposeWeaknessUptime, bool improvedFaerieFire, bool infinitePlayerMana, bool infinitePetMana
    , bool usingLashOfPainOnCooldown, bool petIsAggressive, bool prepopBlackBook, bool randomizeValues, bool userChoosingRotation, bool exaltedWithShattrathFaction, int survivalHunterAgility)
  : auras(auras), talents(talents), sets(sets), stats(stats), itemId(itemId), metaGemId(metaGemId), isAldor(isAldor), enemyLevel(enemyLevel), enemyShadowResist(enemyShadowResist), enemyFireResist(enemyFireResist),
  mageAtieshAmount(mageAtieshAmount), totemOfWrathAmount(totemOfWrathAmount), sacrificingPet(sacrificingPet), petIsImp(petIsImp), petIsSuccubus(petIsSuccubus), petIsFelguard(petIsFelguard),
  ferociousInspirationAmount(ferociousInspirationAmount), improvedCurseOfTheElements(improvedCurseOfTheElements), usingCustomIsbUptime(usingCustomIsbUptime), customIsbUptimeValue(customIsbUptimeValue),
  improvedDivineSpirit(improvedDivineSpirit), improvedImp(improvedImp), shadowPriestDps(shadowPriestDps), warlockAtieshAmount(warlockAtieshAmount), improvedExposeArmor(improvedExposeArmor),
  isSingleTarget(isSingleTarget), enemyAmount(enemyAmount), isOrc(isOrc), powerInfusionAmount(powerInfusionAmount), bloodlustAmount(bloodlustAmount), innervateAmount(innervateAmount), enemyArmor(enemyArmor),
  exposeWeaknessUptime(exposeWeaknessUptime), improvedFaerieFire(improvedFaerieFire), infinitePlayerMana(infinitePlayerMana), infinitePetMana(infinitePetMana), usingLashOfPainOnCooldown(usingLashOfPainOnCooldown),
  petIsAggressive(petIsAggressive), prepopBlackBook(prepopBlackBook), randomizeValues(randomizeValues), userChoosingRotation(userChoosingRotation), exaltedWithShattrathFaction(exaltedWithShattrathFaction),
  survivalHunterAgility(survivalHunterAgility)
  {}