importScripts("./WarlockSim.js");

onmessage = (event) => {
  fetch("./WarlockSim.wasm")
    .then((response) => response.arrayBuffer())
    .then((binary) => WarlockSim({ wasmBinary: binary }))
    .then((w) => w.ready)
    .then((module) => {
      try {
        const playerData = event.data.playerSettings;
        const simulationData = event.data.simulationSettings;

        const items = module.allocItems();
        items.head = parseInt(playerData.items.head) || 0;
        items.neck = parseInt(playerData.items.neck) || 0;
        items.shoulders = parseInt(playerData.items.shoulders) || 0;
        items.back = parseInt(playerData.items.back) || 0;
        items.chest = parseInt(playerData.items.chest) || 0;
        items.bracer = parseInt(playerData.items.bracer) || 0;
        items.gloves = parseInt(playerData.items.gloves) || 0;
        items.belt = parseInt(playerData.items.belt) || 0;
        items.legs = parseInt(playerData.items.legs) || 0;
        items.boots = parseInt(playerData.items.boots) || 0;
        items.ring1 = parseInt(playerData.items.ring1) || 0;
        items.ring2 = parseInt(playerData.items.ring2) || 0;
        items.trinket1 = parseInt(playerData.items.trinket1) || 0;
        items.trinket2 = parseInt(playerData.items.trinket2) || 0;
        items.mainhand = parseInt(playerData.items.mainhand) || 0;
        items.offhand = parseInt(playerData.items.offhand) || 0;
        items.twohand = parseInt(playerData.items.twohand) || 0;
        items.wand = parseInt(playerData.items.wand) || 0;

        const auras = module.allocAuras();
        auras.felArmor = playerData.auras.felArmor || false;
        auras.judgementOfWisdom = playerData.auras.judgementOfWisdom || false;
        auras.manaSpringTotem = playerData.auras.manaSpringTotem || false;
        auras.wrathOfAirTotem = playerData.auras.wrathOfAirTotem || false;
        auras.totemOfWrath = playerData.auras.totemOfWrath || false;
        auras.markOfTheWild = playerData.auras.markOfTheWild || false;
        auras.prayerOfSpirit = playerData.auras.prayerOfSpirit || false;
        auras.bloodPact = playerData.auras.bloodPact || false;
        auras.inspiringPresence = playerData.auras.inspiringPresence || false;
        auras.moonkinAura = playerData.auras.moonkinAura || false;
        auras.powerInfusion = playerData.auras.powerInfusion || false;
        auras.powerOfTheGuardianWarlock =
          playerData.auras.powerOfTheGuardianWarlock || false;
        auras.powerOfTheGuardianMage =
          playerData.auras.powerOfTheGuardianMage || false;
        auras.eyeOfTheNight = playerData.auras.eyeOfTheNight || false;
        auras.chainOfTheTwilightOwl =
          playerData.auras.chainOfTheTwilightOwl || false;
        auras.jadePendantOfBlasting =
          playerData.auras.jadePendantOfBlasting || false;
        auras.drumsOfBattle = playerData.auras.drumsOfBattle || false;
        auras.drumsOfWar = playerData.auras.drumsOfWar || false;
        auras.drumsOfRestoration = playerData.auras.drumsOfRestoration || false;
        auras.bloodlust = playerData.auras.bloodlust || false;
        auras.ferociousInspiration =
          playerData.auras.ferociousInspiration || false;
        auras.innervate = playerData.auras.innervate || false;
        auras.manaTideTotem = playerData.auras.manaTideTotem || false;
        auras.airmansRibbonOfGallantry =
          playerData.auras.airmansRibbonOfGallantry || false;
        auras.curseOfTheElements = playerData.auras.curseOfTheElements || false;
        auras.shadowWeaving = playerData.auras.shadowWeaving || false;
        auras.improvedScorch = playerData.auras.improvedScorch || false;
        auras.misery = playerData.auras.misery || false;
        auras.judgementOfTheCrusader =
          playerData.auras.judgementOfTheCrusader || false;
        auras.vampiricTouch = playerData.auras.vampiricTouch || false;
        auras.faerieFire = playerData.auras.faerieFire || false;
        auras.sunderArmor = playerData.auras.sunderArmor || false;
        auras.exposeArmor = playerData.auras.exposeArmor || false;
        auras.curseOfRecklessness =
          playerData.auras.curseOfRecklessness || false;
        auras.bloodFrenzy = playerData.auras.bloodFrenzy || false;
        auras.exposeWeakness = playerData.auras.exposeWeakness || false;
        auras.annihilator = playerData.auras.annihilator || false;
        auras.improvedHuntersMark =
          playerData.auras.improvedHuntersMark || false;
        auras.superManaPotion = playerData.auras.superManaPotion || false;
        auras.destructionPotion = playerData.auras.destructionPotion || false;
        auras.demonicRune = playerData.auras.demonicRune || false;
        auras.flameCap = playerData.auras.flameCap || false;
        auras.chippedPowerCore = playerData.auras.chippedPowerCore || false;
        auras.crackedPowerCore = playerData.auras.crackedPowerCore || false;
        auras.petBlessingOfKings = playerData.auras.blessingOfKingsPet || false;
        auras.petBlessingOfWisdom =
          playerData.auras.blessingOfWisdomPet || false;
        auras.petBlessingOfMight = playerData.auras.blessingOfMight || false;
        auras.petBattleSquawk = playerData.auras.battleSquawk || false;
        auras.petArcaneIntellect = playerData.auras.arcaneIntellectPet || false;
        auras.petMarkOfTheWild = playerData.auras.markOfTheWildPet || false;
        auras.petPrayerOfFortitude =
          playerData.auras.prayerOfFortitudePet || false;
        auras.petPrayerOfSpirit = playerData.auras.prayerOfSpiritPet || false;
        auras.petKiblersBits = playerData.auras.kiblersBits || false;
        auras.petHeroicPresence = playerData.auras.heroicPresence || false;
        auras.petStrengthOfEarthTotem =
          playerData.auras.strengthOfEarthTotem || false;
        auras.petGraceOfAirTotem = playerData.auras.graceOfAirTotem || false;
        auras.petBattleShout = playerData.auras.battleShout || false;
        auras.petTrueshotAura = playerData.auras.trueshotAura || false;
        auras.petLeaderOfThePack = playerData.auras.leaderOfThePack || false;
        auras.petUnleashedRage = playerData.auras.unleashedRage || false;
        auras.petStaminaScroll = playerData.auras.scrollOfStaminaV || false;
        auras.petIntellectScroll = playerData.auras.scrollOfIntellectV || false;
        auras.petStrengthScroll = playerData.auras.scrollOfStrengthV || false;
        auras.petAgilityScroll = playerData.auras.scrollOfAgilityV || false;
        auras.petSpiritScroll = playerData.auras.scrollOfSpiritV || false;

        const talents = module.allocTalents();
        talents.suppression = parseInt(playerData.talents.suppression) || 0;
        talents.improvedCorruption =
          parseInt(playerData.talents.improvedCorruption) || 0;
        talents.improvedLifeTap =
          parseInt(playerData.talents.improvedLifeTap) || 0;
        talents.improvedCurseOfAgony =
          parseInt(playerData.talents.improvedCurseOfAgony) || 0;
        talents.amplifyCurse = parseInt(playerData.talents.amplifyCurse) || 0;
        talents.nightfall = parseInt(playerData.talents.nightfall) || 0;
        talents.empoweredCorruption =
          parseInt(playerData.talents.empoweredCorruption) || 0;
        talents.siphonLife = parseInt(playerData.talents.siphonLife) || 0;
        talents.shadowMastery = parseInt(playerData.talents.shadowMastery) || 0;
        talents.contagion = parseInt(playerData.talents.contagion) || 0;
        talents.darkPact = parseInt(playerData.talents.darkPact) || 0;
        talents.unstableAffliction =
          parseInt(playerData.talents.unstableAffliction) || 0;
        talents.improvedImp = parseInt(playerData.talents.improvedImp) || 0;
        talents.demonicEmbrace =
          parseInt(playerData.talents.demonicEmbrace) || 0;
        talents.felIntellect = parseInt(playerData.talents.felIntellect) || 0;
        talents.felStamina = parseInt(playerData.talents.felStamina) || 0;
        talents.improvedSuccubus =
          parseInt(playerData.talents.improvedSuccubus) || 0;
        talents.demonicAegis = parseInt(playerData.talents.demonicAegis) || 0;
        talents.unholyPower = parseInt(playerData.talents.unholyPower) || 0;
        talents.demonicSacrifice =
          parseInt(playerData.talents.demonicSacrifice) || 0;
        talents.manaFeed = parseInt(playerData.talents.manaFeed) || 0;
        talents.masterDemonologist =
          parseInt(playerData.talents.masterDemonologist) || 0;
        talents.soulLink = parseInt(playerData.talents.soulLink) || 0;
        talents.demonicKnowledge =
          parseInt(playerData.talents.demonicKnowledge) || 0;
        talents.demonicTactics =
          parseInt(playerData.talents.demonicTactics) || 0;
        talents.summonFelguard =
          parseInt(playerData.talents.summonFelguard) || 0;
        talents.improvedShadowBolt =
          parseInt(playerData.talents.improvedShadowBolt) || 0;
        talents.cataclysm = parseInt(playerData.talents.cataclysm) || 0;
        talents.bane = parseInt(playerData.talents.bane) || 0;
        talents.improvedFirebolt =
          parseInt(playerData.talents.improvedFirebolt) || 0;
        talents.improvedLashOfPain =
          parseInt(playerData.talents.improvedLashOfPain) || 0;
        talents.devastation = parseInt(playerData.talents.devastation) || 0;
        talents.shadowburn = parseInt(playerData.talents.shadowburn) || 0;
        talents.improvedSearingPain =
          parseInt(playerData.talents.improvedSearingPain) || 0;
        talents.improvedImmolate =
          parseInt(playerData.talents.improvedImmolate) || 0;
        talents.ruin = parseInt(playerData.talents.ruin) || 0;
        talents.emberstorm = parseInt(playerData.talents.emberstorm) || 0;
        talents.backlash = parseInt(playerData.talents.backlash) || 0;
        talents.conflagrate = parseInt(playerData.talents.conflagrate) || 0;
        talents.shadowAndFlame =
          parseInt(playerData.talents.shadowAndFlame) || 0;
        talents.shadowfury = parseInt(playerData.talents.shadowfury) || 0;

        const sets = module.allocSets();
        sets.plagueheart = parseInt(playerData.sets["529"]) || 0;
        sets.spellfire = parseInt(playerData.sets["552"]) || 0;
        sets.spellstrike = parseInt(playerData.sets["559"]) || 0;
        sets.oblivion = parseInt(playerData.sets["644"]) || 0;
        sets.manaEtched = parseInt(playerData.sets["658"]) || 0;
        sets.twinStars = parseInt(playerData.sets["667"]) || 0;
        sets.t4 = parseInt(playerData.sets["645"]) || 0;
        sets.t5 = parseInt(playerData.sets["646"]) || 0;
        sets.t6 = parseInt(playerData.sets["670"]) || 0;

        const stats = module.allocStats();
        stats.health = parseFloat(playerData.stats.health);
        stats.mana = parseFloat(playerData.stats.mana);
        stats.stamina = parseFloat(playerData.stats.stamina);
        stats.intellect = parseFloat(playerData.stats.intellect);
        stats.spirit = parseFloat(playerData.stats.spirit);
        stats.spellPower = parseFloat(playerData.stats.spellPower);
        stats.shadowPower = parseFloat(playerData.stats.shadowPower);
        stats.firePower = parseFloat(playerData.stats.firePower);
        stats.hasteRating = parseFloat(playerData.stats.hasteRating);
        stats.hitRating = parseFloat(playerData.stats.hitRating);
        stats.critRating = parseFloat(playerData.stats.critRating);
        stats.critChance = 0;
        stats.mp5 = parseFloat(playerData.stats.mp5);
        stats.manaCostModifier = 1;
        stats.spellPenetration = parseFloat(playerData.stats.spellPenetration);
        stats.fireModifier = parseFloat(playerData.stats.fireModifier);
        stats.shadowModifier = parseFloat(playerData.stats.shadowModifier);
        stats.staminaModifier = parseFloat(playerData.stats.staminaModifier);
        stats.intellectModifier = parseFloat(
          playerData.stats.intellectModifier
        );
        stats.spiritModifier = parseFloat(playerData.stats.spiritModifier);

        const playerSettings = module.allocPlayerSettings(
          auras,
          talents,
          sets,
          stats,
          items
        );
        playerSettings.randomSeeds = module.allocRandomSeeds(
          simulationData.iterations,
          event.data.randomSeed
        );
        playerSettings.itemId = parseInt(event.data.itemId);
        playerSettings.metaGemId = parseInt(
          event.data.playerSettings.metaGemId
        );
        playerSettings.equippedItemSimulation =
          event.data.equippedItemSimulation === true;
        playerSettings.recordingCombatLogBreakdown =
          playerData.simSettings["automatically-open-sim-details"] === "yes";
        playerSettings.customStat =
          module.EmbindConstant[event.data.customStat];
        playerSettings.shattrathFaction =
          playerData.simSettings.shattrathFaction === "Aldor"
            ? module.EmbindConstant.aldor
            : module.EmbindConstant.scryers;
        playerSettings.enemyLevel = parseInt(
          playerData.simSettings["target-level"]
        );
        playerSettings.enemyShadowResist = parseInt(
          playerData.simSettings["target-shadow-resistance"]
        );
        playerSettings.enemyFireResist = parseInt(
          playerData.simSettings["target-fire-resistance"]
        );
        playerSettings.mageAtieshAmount = parseInt(
          playerData.simSettings.mageAtieshAmount
        );
        playerSettings.totemOfWrathAmount = parseInt(
          playerData.simSettings.totemOfWrathAmount
        );
        playerSettings.chippedPowerCoreAmount = parseInt(
          playerData.simSettings.chippedPowerCoreAmount
        );
        playerSettings.crackedPowerCoreAmount = parseInt(
          playerData.simSettings.crackedPowerCoreAmount
        );
        playerSettings.sacrificingPet =
          playerData.simSettings.sacrificePet === "yes";
        if (playerData.simSettings.petChoice === "0") {
          playerSettings.selectedPet = module.EmbindConstant.imp;
        } else if (playerData.simSettings.petChoice === "2") {
          playerSettings.selectedPet = module.EmbindConstant.succubus;
        } else if (playerData.simSettings.petChoice === "3") {
          playerSettings.selectedPet = module.EmbindConstant.felhunter;
        } else if (playerData.simSettings.petChoice === "4") {
          playerSettings.selectedPet = module.EmbindConstant.felguard;
        }
        playerSettings.ferociousInspirationAmount = parseInt(
          playerData.simSettings.ferociousInspirationAmount
        );
        playerSettings.improvedCurseOfTheElements = parseInt(
          playerData.simSettings.improvedCurseOfTheElements
        );
        playerSettings.usingCustomIsbUptime =
          playerData.simSettings.customIsbUptime === "yes";
        playerSettings.customIsbUptimeValue = parseFloat(
          playerData.simSettings.customIsbUptimeValue
        );
        playerSettings.improvedDivineSpirit = parseInt(
          playerData.simSettings.improvedDivineSpirit
        );
        playerSettings.improvedImp = parseInt(
          playerData.simSettings.improvedImpSetting
        );
        playerSettings.shadowPriestDps = parseInt(
          playerData.simSettings.shadowPriestDps
        );
        playerSettings.warlockAtieshAmount = parseInt(
          playerData.simSettings.warlockAtieshAmount
        );
        playerSettings.improvedExposeArmor = parseInt(
          playerData.simSettings.improvedExposeArmor
        );
        playerSettings.fightType =
          !playerData.simSettings.fightType ||
          playerData.simSettings.fightType === "singleTarget"
            ? module.EmbindConstant.singleTarget
            : module.EmbindConstant.aoe;
        playerSettings.enemyAmount = parseInt(
          playerData.simSettings.enemyAmount
        );
        playerSettings.race =
          module.EmbindConstant[playerData.simSettings.race];
        playerSettings.powerInfusionAmount = parseInt(
          playerData.simSettings.powerInfusionAmount
        );
        playerSettings.bloodlustAmount = parseInt(
          playerData.simSettings.bloodlustAmount
        );
        playerSettings.innervateAmount = parseInt(
          playerData.simSettings.innervateAmount
        );
        playerSettings.battleSquawkAmount = parseInt(
          playerData.simSettings.battleSquawkAmount
        );
        playerSettings.enemyArmor = parseInt(playerData.simSettings.enemyArmor);
        playerSettings.exposeWeaknessUptime = parseFloat(
          playerData.simSettings.exposeWeaknessUptime
        );
        playerSettings.improvedFaerieFire =
          playerData.simSettings.improvedFaerieFire === "yes";
        playerSettings.infinitePlayerMana =
          playerData.simSettings.infinitePlayerMana === "yes";
        playerSettings.infinitePetMana =
          playerData.simSettings.infinitePetMana === "yes";
        playerSettings.lashOfPainUsage =
          playerData.simSettings.lashOfPainUsage === "onCooldown"
            ? module.EmbindConstant.onCooldown
            : module.EmbindConstant.noIsb;
        playerSettings.petMode =
          playerData.simSettings.petMode === "0"
            ? module.EmbindConstant.passive
            : module.EmbindConstant.aggressive;
        playerSettings.prepopBlackBook =
          playerData.simSettings.prepopBlackBook === "yes";
        playerSettings.randomizeValues =
          playerData.simSettings.randomizeValues === "yes";
        playerSettings.rotationOption =
          playerData.simSettings.rotationOption === "simChooses"
            ? module.EmbindConstant.simChooses
            : module.EmbindConstant.userChooses;
        playerSettings.exaltedWithShattrathFaction =
          playerData.simSettings.shattrathFactionReputation === "yes";
        playerSettings.survivalHunterAgility = parseInt(
          playerData.simSettings.survivalHunterAgility
        );
        playerSettings.hasImmolate =
          playerData.rotation.dot && playerData.rotation.dot.immolate;
        playerSettings.hasCorruption =
          playerData.rotation.dot && playerData.rotation.dot.corruption;
        playerSettings.hasSiphonLife =
          playerData.rotation.dot && playerData.rotation.dot.siphonLife;
        playerSettings.hasUnstableAffliction =
          playerData.rotation.dot && playerData.rotation.dot.unstableAffliction;
        playerSettings.hasSearingPain =
          playerData.rotation.filler && playerData.rotation.filler.searingPain;
        playerSettings.hasShadowBolt =
          playerData.rotation.filler && playerData.rotation.filler.shadowBolt;
        playerSettings.hasIncinerate =
          playerData.rotation.filler && playerData.rotation.filler.incinerate;
        playerSettings.hasCurseOfRecklessness =
          playerData.rotation.curse &&
          playerData.rotation.curse.curseOfRecklessness;
        playerSettings.hasCurseOfTheElements =
          playerData.rotation.curse &&
          playerData.rotation.curse.curseOfTheElements;
        playerSettings.hasCurseOfAgony =
          playerData.rotation.curse && playerData.rotation.curse.curseOfAgony;
        playerSettings.hasCurseOfDoom =
          playerData.rotation.curse && playerData.rotation.curse.curseOfDoom;
        playerSettings.hasDeathCoil =
          playerData.rotation.finisher &&
          playerData.rotation.finisher.deathCoil;
        playerSettings.hasShadowburn =
          playerData.rotation.finisher &&
          playerData.rotation.finisher.shadowburn;
        playerSettings.hasConflagrate =
          playerData.rotation.finisher &&
          playerData.rotation.finisher.conflagrate;
        playerSettings.hasShadowfury =
          playerData.rotation.other && playerData.rotation.other.shadowfury;
        playerSettings.hasAmplifyCurse =
          playerData.rotation.other && playerData.rotation.other.amplifyCurse;
        playerSettings.hasDarkPact =
          playerData.rotation.other && playerData.rotation.other.darkPact;
        playerSettings.hasElementalShamanT4Bonus =
          playerData.simSettings.improvedWrathOfAirTotem === "yes";

        const simulationSettings = module.allocSimSettings();
        simulationSettings.iterations = parseInt(simulationData.iterations);
        simulationSettings.minTime = parseInt(simulationData.minTime);
        simulationSettings.maxTime = parseInt(simulationData.maxTime);
        simulationSettings.simulationType = parseInt(event.data.simulationType);

        const player = module.allocPlayer(playerSettings);
        const simulation = module.allocSim(player, simulationSettings);
        simulation.start();
      } catch (exceptionPtr) {
        console.error(module.getExceptionMessage(exceptionPtr));
      }
    })
    .catch((e) => console.error(e));
};
