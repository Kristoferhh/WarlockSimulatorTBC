import { ItemSlotKeyToItemSlot, unequipItemSlot } from "./Common";
import { Gems } from "./data/Gems";
import { Items } from "./data/Items";
import { GemColor } from "./Types";
import WarlockSim from "./WASM/WarlockSim";

onmessage = function (e: MessageEvent) {
  fetch('./WarlockSim.wasm')
  .then(response => response.arrayBuffer())
  .then(binary => WarlockSim({ wasmBinary: binary }))
  .then(w => w.ready)
  .then(module => {
    let player = e.data.playerSettings;
    let customItemId = e.data.itemId;
    let customItemSlot = Items.find(i => i.id === customItemId)?.itemSlot;
    let customItemSubSlot = e.data.itemSubSlot;
    let equippedMetaGemId = 0;
    let customMetaGemId = 0;
    let equippedItemId = player.items[customItemSlot + customItemSubSlot]

    // Get the equipped meta gem id
    if (player.items.head && player.items.head !== 0 && player.gems.head && player.gems.head[player.items.head]) {
      player.gems.head[player.items.head].forEach((gemArray: [string, number]) => {
        const gem = Gems.find(e => e.id === gemArray[1]);
        if (gem?.color === GemColor.Meta) {
          equippedMetaGemId = gem.id;
        }
      });
    }

    // If the player is equipped with a custom item then remove the stats from the currently equipped item and add stats from the custom item
    if (customItemSlot && customItemId && customItemId !== equippedItemId) {
      unequipItemSlot({ playerObj: player, itemSlot: ItemSlotKeyToItemSlot(false, customItemSlot, e.data.itemSubSlot), updatingState: false });
    }

    // Add the item's id to its slot in player.items
    // This is required for items that are on-use or have a proc such as Band of the Eternal Sage since they check if the item's ID is equipped.
    player.items[customItemSlot + customItemSubSlot] = customItemId

    // Add/remove stats if doing a stat weight sim
    if (e.data.customStat && e.data.customStatValue) {
      player.stats[e.data.customStat] += e.data.customStatValue
    }

    // Mamma mia somebody clean up this mess please
    // Random seeds
    let randomSeeds = module._allocRandomSeeds(e.data.simulation.iterations)
    // Items
    let equippedItems = module._allocItems(player.items.head, player.items.neck, player.items.shoulders, player.items.back, player.items.chest, player.items.bracers, player.items.gloves, player.items.belt, player.items.legs, player.items.boots, player.items.ring1, player.items.ring2, player.items.trinket1, player.items.trinket2, player.items.mainhand, player.items.offhand, player.items.twohand, player.items.wand)
    // Auras
    let auras = module._allocAuras(player.auras.felArmor, player.auras.blessingOfKings, player.auras.blessingOfWisdom, player.auras.judgementOfWisdom, player.auras.manaSpringTotem, player.auras.wrathOfAirTotem, player.auras.totemOfWrath, player.auras.markOfTheWild, player.auras.arcaneIntellect
      , player.auras.prayerOfFortitude, player.auras.prayerOfSpirit, player.auras.bloodPact, player.auras.inspiringPresence, player.auras.moonkinAura, player.auras.powerInfusion, player.auras.powerOfTheGuardianWarlock, player.auras.powerOfTheGuardianMage, player.auras.eyeOfTheNight, player.auras.chainOfTheTwilightOwl
      , player.auras.jadePendantOfBlasting, player.auras.idolOfTheRavenGoddess, player.auras.drumsOfBattle, player.auras.drumsOfWar, player.auras.drumsOfRestoration, player.auras.bloodlust, player.auras.ferociousInspiration, player.auras.innervate, player.auras.curseOfTheElements, player.auras.shadowWeaving
      , player.auras.improvedScorch, player.auras.misery, player.auras.judgementOfTheCrusader, player.auras.vampiricTouch, player.auras.faerieFire, player.auras.sunderArmor, player.auras.exposeArmor, player.auras.curseOfRecklessness, player.auras.bloodFrenzy, player.auras.exposeWeakness, player.auras.annihilator, player.auras.improvedHuntersMark
      , player.auras.flaskOfPureDeath, player.auras.elixirOfMajorShadowPower, player.auras.elixirOfMajorFirepower, player.auras.greaterArcaneElixir, player.auras.adeptsElixir, player.auras.elixirOfDraenicWisdom, player.auras.elixirOfMajorMageblood, player.auras.superManaPotion, player.auras.destructionPotion
      , player.auras.brilliantWizardOil, player.auras.superiorWizardOil, player.auras.blessedWizardOil, player.auras.demonicRune, player.auras.flameCap, player.auras.rumseyRumBlackLabel, player.auras.kreegsStoutBeatdown, player.auras.blackenedBasilisk, player.auras.skullfishSoup, player.auras.veryBerryCream
      , player.auras.midsummerSausage, player.auras.bloodthistle, player.auras.blessingOfKingsPet, player.auras.blessingOfWisdomPet, player.auras.blessingOfMight, player.auras.arcaneIntellectPet, player.auras.markOfTheWildPet, player.auras.prayerOfFortitudePet, player.auras.prayerOfSpiritPet, player.auras.kiblersBits
      , player.auras.heroicPresence, player.auras.strengthOfEarthTotem, player.auras.graceOfAirTotem, player.auras.battleShout, player.auras.trueshotAura, player.auras.leaderOfThePack, player.auras.unleashedRage, player.auras.scrollOfStaminaV, player.auras.scrollOfIntellectV, player.auras.scrollOfStrengthV
      , player.auras.scrollOfAgilityV, player.auras.scrollOfSpiritV)
    // Talents
    let talents = module._allocTalents(player.talents.suppression, player.talents.improvedCorruption, player.talents.improvedLifeTap, player.talents.improvedCurseOfAgony, player.talents.amplifyCurse, player.talents.nightfall, player.talents.empoweredCorruption, player.talents.siphonLife, player.talents.shadowMastery
      , player.talents.contagion, player.talents.darkPact, player.talents.unstableAffliction, player.talents.improvedImp, player.talents.demonicEmbrace, player.talents.felIntellect, player.talents.felStamina, player.talents.improvedSuccubus, player.talents.demonicAegis, player.talents.unholyPower, player.talents.demonicSacrifice, player.talents.manaFeed
      , player.talents.masterDemonologist, player.talents.soulLink, player.talents.demonicKnowledge, player.talents.demonicTactics, player.talents.felguard, player.talents.improvedShadowBolt, player.talents.cataclysm, player.talents.bane, player.talents.improvedFirebolt, player.talents.improvedLashOfPain, player.talents.devastation, player.talents.shadowburn
      , player.talents.improvedSearingPain, player.talents.improvedImmolate, player.talents.ruin, player.talents.emberstorm, player.talents.backlash, player.talents.conflagrate, player.talents.shadowAndFlame, player.talents.shadowfury)
    // Sets
    let sets = module._allocSets(player.sets['529'], player.sets['552'], player.sets['553'], player.sets['559'], player.sets['644'], player.sets['658'], player.sets['667'], player.sets['645'], player.sets['646'], player.sets['670'])
    // Stats
    let stats = module._allocStats(player.stats.health, player.stats.mana, player.stats.stamina, player.stats.intellect, player.stats.spirit, player.stats.spellPower, player.stats.shadowPower, player.stats.firePower, player.stats.hasteRating, player.stats.hitRating, player.stats.critRating, player.stats.critPercent, player.stats.mp5, player.stats.spellPen
      , player.stats.fireModifier, player.stats.frostModifier, player.stats.hastePercent, player.stats.damageModifier, player.stats.shadowModifier, player.stats.staminaModifier, player.stats.intellectModifier, player.stats.spiritModifier, player.stats.manaCostModifier, player.stats.arcaneModifier, player.stats.natureModifier
      , player.stats.natureResist, player.stats.arcaneResist, player.stats.fireResist, player.stats.frostResist, player.stats.shadowResist)
    // Player settings
    let playerSettings = module._allocPlayerSettings(auras, talents, sets, stats, equippedItems, e.data.itemId, customMetaGemId, player.simSettings["automatically-open-sim-details"] === "yes"
    , e.data.customStat === "stamina", e.data.customStat === "intellect", e.data.customStat === "spirit", e.data.customStat === "spellPower", e.data.customStat === "shadowPower", e.data.customStat === "firePower", e.data.customStat === "hitRating"
    , e.data.customStat === "critRating", e.data.customStat === "hasteRating", e.data.customStat === "mp5", player.simSettings.shattrathFaction === "Aldor", parseInt(player.simSettings['target-level']), parseInt(player.simSettings['target-shadow-resistance'])
    , parseInt(player.simSettings['target-fire-resistance']), parseInt(player.simSettings.mageAtieshAmount), parseInt(player.simSettings.totemOfWrathAmount), player.simSettings.sacrificePet === "yes", player.simSettings.petChoice === "0", player.simSettings.petChoice === "2", player.simSettings.petChoice === "4"
    , parseInt(player.simSettings.ferociousInspirationAmount), parseInt(player.simSettings.improvedCurseOfTheElements), player.simSettings.customIsbUptime === "yes", parseInt(player.simSettings.customIsbUptimeValue), parseInt(player.simSettings.improvedDivineSpirit), parseInt(player.simSettings.improvedImpSetting)
    , parseInt(player.simSettings.shadowPriestDps), parseInt(player.simSettings.warlockAtieshAmount), parseInt(player.simSettings.improvedExposeArmor), player.simSettings.fightType === "singleTarget", parseInt(player.simSettings.enemyAmount), player.simSettings.race === "orc", parseInt(player.simSettings.powerInfusionAmount)
    , parseInt(player.simSettings.bloodlustAmount), parseInt(player.simSettings.innervateAmount), parseInt(player.simSettings.enemyArmor), parseInt(player.simSettings.exposeWeaknessUptime), player.simSettings.improvedFaerieFire === "yes", player.simSettings.infinitePlayerMana === "yes", player.simSettings.infinitePetMana === "yes"
    , player.simSettings.lashOfPainUsage === "onCooldown", player.simSettings.petMode === "1", player.simSettings.prepopBlackBook === "yes", player.simSettings.randomizeValues === "yes", player.simSettings.rotationOption === "simChooses", player.simSettings.shattrathFactionReputation === "yes"
    , parseInt(player.simSettings.survivalHunterAgility), player.rotation.dot.immolate, player.rotation.dot.corruption, player.rotation.dot.siphonLife, player.rotation.dot.unstableAffliction, player.rotation.filler.searingPain, player.rotation.filler.shadowBolt, player.rotation.filler.incinerate, player.rotation.curse.curseOfRecklessness
    , player.rotation.curse.curseOfTheElements, player.rotation.curse.curseOfAgony, player.rotation.curse.curseOfDoom, player.rotation.finisher.deathCoil, player.rotation.finisher.shadowburn, player.rotation.finisher.conflagrate, player.rotation.other.shadowfury, player.rotation.other.amplifyCurse, player.rotation.other.darkPact
    , player.simSettings.improvedWrathOfAirTotem === "yes")
    let playerPtr = module._allocPlayer(playerSettings)
    let simSettings = module._allocSimSettings(e.data.simulation.iterations, e.data.simulation.minTime, e.data.simulation.maxTime, randomSeeds, e.data.itemAmount > 1)
    let sim = module._allocSim(playerPtr, simSettings)
    module._startSimulation(sim)
    module._freeUnsignedIntArr(randomSeeds)
    module._freeItems(equippedItems)
    module._freeAuras(auras)
    module._freeTalents(talents)
    module._freeSets(sets)
    module._freeStats(stats)
    module._freePlayerSettings(playerSettings)
    module._freePlayer(playerPtr)
    module._freeSimSettings(simSettings)
    module._freeSim(sim)
  })
}