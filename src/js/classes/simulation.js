class Simulation {
  static getSettings () {
    return {
      iterations: parseInt($("input[name='iterations']").val()),
      minTime: parseInt($("input[name='min-fight-length']").val()),
      maxTime: parseInt($("input[name='max-fight-length']").val())
    }
  }

  constructor (player, settings = Simulation.getSettings(), simulationEnd, simulationUpdate) {
    this.player = player
    this.iterations = settings.iterations
    this.minTime = settings.minTime
    this.maxTime = settings.maxTime
    this.simulationEnd = simulationEnd
    this.simulationUpdate = simulationUpdate
    if (player.customStat.value > 0) {
      this.iterations *= 1.5
    }
  }

  // >> IMPORTANT <<: The reason this function is so horribly hardcoded instead of using loops is because using loops made the simulation ~4-5 times slower.
  // That's not to say that this can't be improved, but the improvement won't be a regular loop through the spell/aura objects.
  passTime () {
    let time = this.player.castTimeRemaining
    if (time == 0 || (this.player.gcdRemaining > 0 && this.player.gcdRemaining < time)) time = this.player.gcdRemaining

    // Look for the shortest time until an action needs to be done
    if (this.player.pet) {
      if ((this.player.talents.darkPact > 0 || this.player.pet.mode == PetMode.AGGRESSIVE) && this.player.pet.spiritTickTimerRemaining < time) time = this.player.pet.spiritTickTimerRemaining

      // Pet's attacks/abilities and such
      if (this.player.pet.mode == PetMode.AGGRESSIVE) {
        if (this.player.pet.spells.melee && this.player.pet.spells.melee.cooldownRemaining < time) time = this.player.pet.spells.melee.cooldownRemaining
        else if (this.player.pet.type == PetType.RANGED && this.player.pet.castTimeRemaining > 0 && this.player.pet.castTimeRemaining < time) time = this.player.pet.castTimeRemaining

        if (this.player.pet.pet == PetName.SUCCUBUS) {
          if (this.player.pet.spells.lashOfPain.cooldownRemaining > 0 && this.player.pet.spells.lashOfPain.cooldownRemaining < time) time = this.player.pet.spells.lashOfPain.cooldownRemaining
        } else if (this.player.pet.pet == PetName.FELGUARD) {
          if (this.player.pet.spells.cleave.cooldownRemaining > 0 && this.player.pet.spells.cleave.cooldownRemaining < time) time = this.player.pet.spells.cleave.cooldownRemaining
        }
      }
    }

    if (this.player.auras.improvedShadowBolt && this.player.auras.improvedShadowBolt.active && this.player.auras.improvedShadowBolt.durationRemaining < time) time = this.player.auras.improvedShadowBolt.durationRemaining
    if (this.player.auras.corruption && this.player.auras.corruption.active && this.player.auras.corruption.tickTimerRemaining < time) time = this.player.auras.corruption.tickTimerRemaining
    if (this.player.auras.unstableAffliction && this.player.auras.unstableAffliction.active && this.player.auras.unstableAffliction.tickTimerRemaining < time) time = this.player.auras.unstableAffliction.tickTimerRemaining
    if (this.player.auras.siphonLife && this.player.auras.siphonLife.active && this.player.auras.siphonLife.tickTimerRemaining < time) time = this.player.auras.siphonLife.tickTimerRemaining
    if (this.player.auras.immolate && this.player.auras.immolate.active && this.player.auras.immolate.tickTimerRemaining < time) time = this.player.auras.immolate.tickTimerRemaining
    if (this.player.auras.curseOfAgony && this.player.auras.curseOfAgony.active && this.player.auras.curseOfAgony.tickTimerRemaining < time) time = this.player.auras.curseOfAgony.tickTimerRemaining
    if (this.player.auras.curseOfTheElements && this.player.auras.curseOfTheElements.active && this.player.auras.curseOfTheElements.durationRemaining < time) time = this.player.auras.curseOfTheElements.durationRemaining
    if (this.player.auras.curseOfRecklessness && this.player.auras.curseOfRecklessness.active && this.player.auras.curseOfRecklessness.durationRemaining < time) time = this.player.auras.curseOfRecklessness.durationRemaining
    if (this.player.auras.curseOfDoom) {
      if (this.player.auras.curseOfDoom.active && this.player.auras.curseOfDoom.durationRemaining < time) time = this.player.auras.curseOfDoom.durationRemaining
      if (this.player.spells.curseOfDoom.cooldownRemaining > 0 && this.player.spells.curseOfDoom.cooldownRemaining < time) time = this.player.spells.curseOfDoom.cooldownRemaining
    }
    if (this.player.auras.destructionPotion) {
      if (this.player.spells.destructionPotion.cooldownRemaining > 0 && this.player.spells.destructionPotion.cooldownRemaining < time) time = this.player.spells.destructionPotion.cooldownRemaining
      if (this.player.auras.destructionPotion.active && this.player.auras.destructionPotion.durationRemaining < time) time = this.player.auras.destructionPotion.durationRemaining
    }
    if (this.player.spells.flameCap) {
      if (this.player.spells.flameCap.cooldownRemaining > 0 && this.player.spells.flameCap.cooldownRemaining < time) time = this.player.spells.flameCap.cooldownRemaining
      if (this.player.auras.flameCap.active && this.player.auras.flameCap.durationRemaining < time) time = this.player.auras.flameCap.durationRemaining
    }
    if (this.player.spells.bloodFury) {
      if (this.player.spells.bloodFury.cooldownRemaining > 0 && this.player.spells.bloodFury.cooldownRemaining < time) time = this.player.spells.bloodFury.cooldownRemaining
      if (this.player.auras.bloodFury.active && this.player.auras.bloodFury.durationRemaining < time) time = this.player.auras.bloodFury.durationRemaining
    }
    if (this.player.spells.demonicRune && this.player.spells.demonicRune.cooldownRemaining > 0 && this.player.spells.demonicRune.cooldownRemaining < time) time = this.player.spells.demonicRune.cooldownRemaining
    if (this.player.spells.superManaPotion && this.player.spells.superManaPotion.cooldownRemaining > 0 && this.player.spells.superManaPotion.cooldownRemaining < time) time = this.player.spells.superManaPotion.cooldownRemaining
    if (this.player.spells.powerInfusion) {
      for (let i = 0; i < this.player.spells.powerInfusion.length; i++) {
        if (this.player.spells.powerInfusion[i].cooldownRemaining > 0 && this.player.spells.powerInfusion[i].cooldownRemaining < time) time = this.player.spells.powerInfusion[i].cooldownRemaining
      }
    }
    if (this.player.spells.bloodlust) {
      for (let i = 0; i < this.player.spells.bloodlust.length; i++) {
        if (this.player.spells.bloodlust[i].cooldownRemaining > 0 && this.player.spells.bloodlust[i].cooldownRemaining < time) time = this.player.spells.bloodlust[i].cooldownRemaining
      }
      if (this.player.auras.bloodlust.active && this.player.auras.bloodlust.durationRemaining < time) time = this.player.auras.bloodlust.durationRemaining
    }
    if (this.player.spells.innervate) {
      for (let i = 0; i < this.player.spells.innervate.length; i++) {
        if (this.player.spells.innervate[i].cooldownRemaining > 0 && this.player.spells.innervate[i].cooldownRemaining < time) time = this.player.spells.innervate[i].cooldownRemaining
      }
      if (this.player.auras.innervate.active && this.player.auras.innervate.durationRemaining < time) time = this.player.auras.innervate.durationRemaining
    }
    if (this.player.spells.mysticalSkyfireDiamond) {
      if (this.player.spells.mysticalSkyfireDiamond.cooldownRemaining > 0 && this.player.spells.mysticalSkyfireDiamond.cooldownRemaining < time) time = this.player.spells.mysticalSkyfireDiamond.cooldownRemaining
      if (this.player.auras.mysticalSkyfireDiamond.active && this.player.spells.mysticalSkyfireDiamond.durationRemaining < time) time = this.player.auras.mysticalSkyfireDiamond.durationRemaining
    }
    if (this.player.spells.amplifyCurse) {
      if (this.player.spells.amplifyCurse.cooldownRemaining > 0 && this.player.spells.amplifyCurse.cooldownRemaining < time) time = this.player.spells.amplifyCurse.cooldownRemaining
      if (this.player.auras.amplifyCurse.active && this.player.auras.amplifyCurse.durationRemaining < time) time = this.player.auras.amplifyCurse.durationRemaining
    }
    if (this.player.spells.shadowfury && this.player.spells.shadowfury.cooldownRemaining > 0 && this.player.spells.shadowfury.cooldownRemaining < time) time = this.player.spells.shadowfury.cooldownRemaining
    if (this.player.spells.insightfulEarthstormDiamond && this.player.spells.insightfulEarthstormDiamond.cooldownRemaining > 0 && this.player.spells.insightfulEarthstormDiamond.cooldownRemaining < time) time = this.player.spells.insightfulEarthstormDiamond.cooldownRemaining
    if (this.player.spells.timbalsFocusingCrystal && this.player.spells.timbalsFocusingCrystal.cooldownRemaining > 0 && this.player.spells.timbalsFocusingCrystal.cooldownRemaining < time) time = this.player.spells.timbalsFocusingCrystal.cooldownRemaining
    if (this.player.spells.markOfDefiance && this.player.spells.markOfDefiance.cooldownRemaining > 0 && this.player.spells.markOfDefiance.cooldownRemaining < time) time = this.player.spells.markOfDefiance.cooldownRemaining
    if (this.player.spells.drumsOfBattle && this.player.spells.drumsOfBattle.cooldownRemaining > 0 && this.player.spells.drumsOfBattle.cooldownRemaining < time) time = this.player.spells.drumsOfBattle.cooldownRemaining
    if (this.player.spells.drumsOfWar && this.player.spells.drumsOfWar.cooldownRemaining > 0 && this.player.spells.drumsOfWar.cooldownRemaining < time) time = this.player.spells.drumsOfWar.cooldownRemaining
    if (this.player.spells.drumsOfRestoration && this.player.spells.drumsOfRestoration.cooldownRemaining > 0 && this.player.spells.drumsOfRestoration.cooldownRemaining < time) time = this.player.spells.drumsOfRestoration.cooldownRemaining
    if (this.player.spells.conflagrate && this.player.spells.conflagrate.cooldownRemaining > 0 && this.player.spells.conflagrate.cooldownRemaining < time) time = this.player.spells.conflagrate.cooldownRemaining
    if (this.player.auras.drumsOfBattle && this.player.auras.drumsOfBattle.active && this.player.auras.drumsOfBattle.durationRemaining < time) time = this.player.auras.drumsOfBattle.durationRemaining
    if (this.player.auras.drumsOfWar && this.player.auras.drumsOfWar.active && this.player.auras.drumsOfWar.durationRemaining < time) time = this.player.auras.drumsOfWar.durationRemaining
    if (this.player.auras.drumsOfRestoration && this.player.auras.drumsOfRestoration.active && this.player.auras.drumsOfRestoration.tickTimerRemaining < time) time = this.player.auras.drumsOfRestoration.tickTimerRemaining
    if (this.player.auras.shadowTrance && this.player.auras.shadowTrance.active && this.player.auras.shadowTrance.durationRemaining < time) time = this.player.auras.shadowTrance.durationRemaining
    if (this.player.auras.wrathOfCenarius && this.player.auras.wrathOfCenarius.active && this.player.auras.wrathOfCenarius.durationRemaining < time) time = this.player.auras.wrathOfCenarius.durationRemaining
    if (this.player.auras.flameshadow && this.player.auras.flameshadow.active && this.player.auras.flameshadow.durationRemaining < time) time = this.player.auras.flameshadow.durationRemaining
    if (this.player.auras.shadowflame && this.player.auras.shadowflame.active && this.player.auras.shadowflame.durationRemaining < time) time = this.player.auras.shadowflame.durationRemaining
    if (this.player.auras.spellstrikeProc && this.player.auras.spellstrikeProc.active && this.player.auras.spellstrikeProc.durationRemaining < time) time = this.player.auras.spellstrikeProc.durationRemaining
    if (this.player.auras.powerInfusion && this.player.auras.powerInfusion.active && this.player.auras.powerInfusion.durationRemaining < time) time = this.player.auras.powerInfusion.durationRemaining
    if (this.player.auras.eyeOfMagtheridon && this.player.auras.eyeOfMagtheridon.active && this.player.auras.eyeOfMagtheridon.durationRemaining < time) time = this.player.auras.eyeOfMagtheridon.durationRemaining
    if (this.player.auras.sextantOfUnstableCurrents) {
      if (this.player.auras.sextantOfUnstableCurrents.active && this.player.auras.sextantOfUnstableCurrents.durationRemaining < time) time = this.player.auras.sextantOfUnstableCurrents.durationRemaining
      if (this.player.spells.sextantOfUnstableCurrents.cooldownRemaining > 0 && this.player.spells.sextantOfUnstableCurrents.cooldownRemaining < time) time = this.player.spells.sextantOfUnstableCurrents.cooldownRemaining
    }
    if (this.player.auras.quagmirransEye) {
      if (this.player.auras.quagmirransEye.active && this.player.auras.quagmirransEye.durationRemaining < time) time = this.player.auras.quagmirransEye.durationRemaining
      if (this.player.spells.quagmirransEye.cooldownRemaining > 0 && this.player.spells.quagmirransEye.cooldownRemaining < time) time = this.player.spells.quagmirransEye.cooldownRemaining
    }
    if (this.player.auras.shiffarsNexusHorn) {
      if (this.player.auras.shiffarsNexusHorn.active && this.player.auras.shiffarsNexusHorn.durationRemaining < time) time = this.player.auras.shiffarsNexusHorn.durationRemaining
      if (this.player.spells.shiffarsNexusHorn.cooldownRemaining > 0 && this.player.spells.shiffarsNexusHorn.cooldownRemaining < time) time = this.player.spells.shiffarsNexusHorn.cooldownRemaining
    }
    if (this.player.spells.bladeOfWizardry) {
      if (this.player.spells.bladeOfWizardry.cooldownRemaining > 0 && this.player.spells.bladeOfWizardry.cooldownRemaining < time) time = this.player.spells.bladeOfWizardry.cooldownRemaining
      if (this.player.auras.bladeOfWizardry.active && this.player.auras.bladeOfWizardry.durationRemaining < time) time = this.player.auras.bladeOfWizardry.durationRemaining
    }
    if (this.player.spells.robeOfTheElderScribes) {
      if (this.player.spells.robeOfTheElderScribes.cooldownRemaining > 0 && this.player.spells.robeOfTheElderScribes.cooldownRemaining < time) time = this.player.spells.robeOfTheElderScribes.cooldownRemaining
      if (this.player.auras.robeOfTheElderScribes.active && this.player.auras.robeOfTheElderScribes.durationRemaining < time) time = this.player.auras.robeOfTheElderScribes.durationRemaining
    }
    if (this.player.auras.ashtongueTalismanOfShadows && this.player.auras.ashtongueTalismanOfShadows.active && this.player.auras.ashtongueTalismanOfShadows.durationRemaining < time) time = this.player.auras.ashtongueTalismanOfShadows.durationRemaining
    if (this.player.auras.darkmoonCardCrusade && this.player.auras.darkmoonCardCrusade.active && this.player.auras.darkmoonCardCrusade.durationRemaining < time) time = this.player.auras.darkmoonCardCrusade.durationRemaining
    if (this.player.auras.manaEtched4Set && this.player.auras.manaEtched4Set.active && this.player.auras.manaEtched4Set.durationRemaining < time) time = this.player.auras.manaEtched4Set.durationRemaining
    if (this.player.spells.theLightningCapacitor && this.player.spells.theLightningCapacitor.cooldownRemaining > 0 && this.player.spells.theLightningCapacitor.cooldownRemaining < time) time = this.player.spells.theLightningCapacitor.cooldownRemaining
    if (this.player.auras.bandOfTheEternalSage) {
      if (this.player.auras.bandOfTheEternalSage.active && this.player.auras.bandOfTheEternalSage.durationRemaining < time) time = this.player.auras.bandOfTheEternalSage.durationRemaining
      if (this.player.spells.bandOfTheEternalSage.cooldownRemaining > 0 && this.player.spells.bandOfTheEternalSage.cooldownRemaining < time) time = this.player.spells.bandOfTheEternalSage.cooldownRemaining
    }
    if (this.player.spells.shatteredSunPendantOfAcumen) {
      if (this.player.spells.shatteredSunPendantOfAcumen.cooldownRemaining > 0 && this.player.spells.shatteredSunPendantOfAcumen.cooldownRemaining < time) time = this.player.spells.shatteredSunPendantOfAcumen.cooldownRemaining
      if (this.player.auras.shatteredSunPendantOfAcumen && this.player.auras.shatteredSunPendantOfAcumen.active && this.player.auras.shatteredSunPendantOfAcumen.durationRemaining < time) time = this.player.auras.shatteredSunPendantOfAcumen.durationRemaining
    }
    if (this.player.mp5Timer < time) time = this.player.mp5Timer
    for (let i = 0; i < 2; i++) {
      if (this.player.trinkets[i]) {
        if (this.player.trinkets[i].active && this.player.trinkets[i].durationRemaining < time) time = this.player.trinkets[i].durationRemaining
        if (this.player.trinkets[i].cooldownRemaining > 0 && this.player.trinkets[i].cooldownRemaining < time) time = this.player.trinkets[i].cooldownRemaining
      }
    }
    if (this.player.pet) {
      if (this.player.pet.auras.blackBook && this.player.pet.auras.blackBook.active && this.player.pet.auras.blackBook.durationRemaining < time) time = this.player.pet.auras.blackBook.durationRemaining
    }

    // Pass time
    // This needs to be the first modified value since the time in combat needs to be updated before spells start dealing damage/auras expiring etc. for the combat logging.
    this.player.fightTime += time
    this.player.castTimeRemaining = Math.max(0, this.player.castTimeRemaining - time)

    // Pet
    if (this.player.pet) this.player.pet.tick(time)

    // Auras need to tick before Spells because otherwise you'll, for example, finish casting Corruption and then immediately afterwards, in the same millisecond, immediately tick down the aura
    // This was also causing buffs like the t4 4pc buffs to expire sooner than they should.

    // Auras
    if (this.player.auras.powerInfusion && this.player.auras.powerInfusion.active) this.player.auras.powerInfusion.tick(time)
    if (this.player.auras.improvedShadowBolt && this.player.auras.improvedShadowBolt.active) this.player.auras.improvedShadowBolt.tick(time)
    if (this.player.auras.corruption && this.player.auras.corruption.active) this.player.auras.corruption.tick(time)
    if (this.player.auras.unstableAffliction && this.player.auras.unstableAffliction.active) this.player.auras.unstableAffliction.tick(time)
    if (this.player.auras.siphonLife && this.player.auras.siphonLife.active) this.player.auras.siphonLife.tick(time)
    if (this.player.auras.immolate && this.player.auras.immolate.active) this.player.auras.immolate.tick(time)
    if (this.player.auras.curseOfAgony && this.player.auras.curseOfAgony.active) this.player.auras.curseOfAgony.tick(time)
    if (this.player.auras.curseOfTheElements && this.player.auras.curseOfTheElements.active) this.player.auras.curseOfTheElements.tick(time)
    if (this.player.auras.curseOfRecklessness && this.player.auras.curseOfRecklessness.active) this.player.auras.curseOfRecklessness.tick(time)
    if (this.player.auras.curseOfDoom && (this.player.auras.curseOfDoom.active || this.player.auras.curseOfDoom.cooldownRemaining > 0)) this.player.auras.curseOfDoom.tick(time)
    if (this.player.auras.destructionPotion && this.player.auras.destructionPotion.active) this.player.auras.destructionPotion.tick(time)
    if (this.player.auras.flameCap && this.player.auras.flameCap.active) this.player.auras.flameCap.tick(time)
    if (this.player.auras.flameshadow && this.player.auras.flameshadow.active) this.player.auras.flameshadow.tick(time)
    if (this.player.auras.shadowflame && this.player.auras.shadowflame.active) this.player.auras.shadowflame.tick(time)
    if (this.player.auras.spellstrikeProc && this.player.auras.spellstrikeProc.active) this.player.auras.spellstrikeProc.tick(time)
    if (this.player.auras.eyeOfMagtheridon && this.player.auras.eyeOfMagtheridon.active) this.player.auras.eyeOfMagtheridon.tick(time)
    if (this.player.auras.sextantOfUnstableCurrents && (this.player.auras.sextantOfUnstableCurrents.active || this.player.auras.sextantOfUnstableCurrents.hiddenCooldownRemaining > 0)) this.player.auras.sextantOfUnstableCurrents.tick(time)
    if (this.player.auras.quagmirransEye && (this.player.auras.quagmirransEye.active || this.player.auras.quagmirransEye.hiddenCooldownRemaining > 0)) this.player.auras.quagmirransEye.tick(time)
    if (this.player.auras.shiffarsNexusHorn && this.player.auras.shiffarsNexusHorn.active) this.player.auras.shiffarsNexusHorn.tick(time)
    if (this.player.auras.manaEtched4Set && this.player.auras.manaEtched4Set.active) this.player.auras.manaEtched4Set.tick(time)
    if (this.player.auras.bloodFury && this.player.auras.bloodFury.active) this.player.auras.bloodFury.tick(time)
    if (this.player.auras.bloodlust && this.player.auras.bloodlust.active) this.player.auras.bloodlust.tick(time)
    if (this.player.auras.drumsOfBattle && this.player.auras.drumsOfBattle.active) this.player.auras.drumsOfBattle.tick(time)
    if (this.player.auras.drumsOfWar && this.player.auras.drumsOfWar.active) this.player.auras.drumsOfWar.tick(time)
    if (this.player.auras.drumsOfRestoration && this.player.auras.drumsOfRestoration.active) this.player.auras.drumsOfRestoration.tick(time)
    if (this.player.auras.ashtongueTalismanOfShadows && this.player.auras.ashtongueTalismanOfShadows.active) this.player.auras.ashtongueTalismanOfShadows.tick(time)
    if (this.player.auras.darkmoonCardCrusade && this.player.auras.darkmoonCardCrusade.active) this.player.auras.darkmoonCardCrusade.tick(time)
    if (this.player.auras.bladeOfWizardry && this.player.auras.bladeOfWizardry.active) this.player.auras.bladeOfWizardry.tick(time)
    if (this.player.auras.shatteredSunPendantOfAcumen && this.player.auras.shatteredSunPendantOfAcumen.active) this.player.auras.shatteredSunPendantOfAcumen.tick(time)
    if (this.player.auras.robeOfTheElderScribes && this.player.auras.robeOfTheElderScribes.active) this.player.auras.robeOfTheElderScribes.tick(time)
    if (this.player.auras.bandOfTheEternalSage && this.player.auras.bandOfTheEternalSage.active) this.player.auras.bandOfTheEternalSage.tick(time)
    if (this.player.auras.mysticalSkyfireDiamond && this.player.auras.mysticalSkyfireDiamond.active) this.player.auras.mysticalSkyfireDiamond.tick(time)
    if (this.player.auras.amplifyCurse && this.player.auras.amplifyCurse.active) this.player.auras.amplifyCurse.tick(time)
    if (this.player.auras.wrathOfCenarius && this.player.auras.wrathOfCenarius.active) this.player.auras.wrathOfCenarius.tick(time)
    if (this.player.auras.innervate && this.player.auras.innervate.active) this.player.auras.innervate.tick(time)

    // Spells
    if (this.player.spells.seedOfCorruption && this.player.spells.seedOfCorruption.casting) this.player.spells.seedOfCorruption.tick(time)
    if (this.player.spells.shadowBolt && this.player.spells.shadowBolt.casting) this.player.spells.shadowBolt.tick(time)
    if (this.player.spells.incinerate && this.player.spells.incinerate.casting) this.player.spells.incinerate.tick(time)
    if (this.player.spells.searingPain && this.player.spells.searingPain.casting) this.player.spells.searingPain.tick(time)
    if (this.player.spells.immolate && this.player.spells.immolate.casting) this.player.spells.immolate.tick(time)
    if (this.player.spells.corruption && this.player.spells.corruption.casting) this.player.spells.corruption.tick(time)
    if (this.player.spells.unstableAffliction && this.player.spells.unstableAffliction.casting) this.player.spells.unstableAffliction.tick(time)
    if (this.player.spells.curseOfDoom && this.player.spells.curseOfDoom.cooldownRemaining > 0) this.player.spells.curseOfDoom.tick(time)
    if (this.player.spells.deathCoil && this.player.spells.deathCoil.cooldownRemaining > 0) this.player.spells.deathCoil.tick(time)
    if (this.player.spells.shadowburn && this.player.spells.shadowburn.cooldownRemaining > 0) this.player.spells.shadowburn.tick(time)
    if (this.player.spells.destructionPotion && this.player.spells.destructionPotion.cooldownRemaining > 0) this.player.spells.destructionPotion.tick(time)
    if (this.player.spells.superManaPotion && this.player.spells.superManaPotion.cooldownRemaining > 0) this.player.spells.superManaPotion.tick(time)
    if (this.player.spells.demonicRune && this.player.spells.demonicRune.cooldownRemaining > 0) this.player.spells.demonicRune.tick(time)
    if (this.player.spells.flameCap && this.player.spells.flameCap.cooldownRemaining > 0) this.player.spells.flameCap.tick(time)
    if (this.player.spells.bloodFury && this.player.spells.bloodFury.cooldownRemaining > 0) this.player.spells.bloodFury.tick(time)
    if (this.player.spells.mysticalSkyfireDiamond && this.player.spells.mysticalSkyfireDiamond.cooldownRemaining > 0) this.player.spells.mysticalSkyfireDiamond.tick(time)
    if (this.player.spells.conflagrate && this.player.spells.conflagrate.cooldownRemaining > 0) this.player.spells.conflagrate.tick(time)
    if (this.player.spells.shadowfury && (this.player.spells.shadowfury.cooldownRemaining > 0 || this.player.spells.shadowfury.casting)) this.player.spells.shadowfury.tick(time)
    if (this.player.spells.amplifyCurse && this.player.spells.amplifyCurse.cooldownRemaining > 0) this.player.spells.amplifyCurse.tick(time)
    if (this.player.spells.drumsOfBattle && this.player.spells.drumsOfBattle.cooldownRemaining > 0) this.player.spells.drumsOfBattle.tick(time)
    if (this.player.spells.drumsOfWar && this.player.spells.drumsOfWar.cooldownRemaining > 0) this.player.spells.drumsOfWar.tick(time)
    if (this.player.spells.drumsOfRestoration && this.player.spells.drumsOfRestoration.cooldownRemaining > 0) this.player.spells.drumsOfRestoration.tick(time)
    if (this.player.spells.timbalsFocusingCrystal && this.player.spells.timbalsFocusingCrystal.cooldownRemaining > 0) this.player.spells.timbalsFocusingCrystal.tick(time)
    if (this.player.spells.markOfDefiance && this.player.spells.markOfDefiance.cooldownRemaining > 0) this.player.spells.markOfDefiance.tick(time)
    if (this.player.spells.theLightningCapacitor && this.player.spells.theLightningCapacitor.cooldownRemaining > 0) this.player.spells.theLightningCapacitor.tick(time)
    if (this.player.spells.bladeOfWizardry && this.player.spells.bladeOfWizardry.cooldownRemaining > 0) this.player.spells.bladeOfWizardry.tick(time)
    if (this.player.spells.shatteredSunPendantOfAcumen && this.player.spells.shatteredSunPendantOfAcumen.cooldownRemaining > 0) this.player.spells.shatteredSunPendantOfAcumen.tick(time)
    if (this.player.spells.robeOfTheElderScribes && this.player.spells.robeOfTheElderScribes.cooldownRemaining > 0) this.player.spells.robeOfTheElderScribes.tick(time)
    if (this.player.spells.quagmirransEye && this.player.spells.quagmirransEye.cooldownRemaining > 0) this.player.spells.quagmirransEye.tick(time)
    if (this.player.spells.shiffarsNexusHorn && this.player.spells.shiffarsNexusHorn.cooldownRemaining > 0) this.player.spells.shiffarsNexusHorn.tick(time)
    if (this.player.spells.sextantOfUnstableCurrents && this.player.spells.sextantOfUnstableCurrents.cooldownRemaining > 0) this.player.spells.sextantOfUnstableCurrents.tick(time)
    if (this.player.spells.bandOfTheEternalSage && this.player.spells.bandOfTheEternalSage.cooldownRemaining > 0) this.player.spells.bandOfTheEternalSage.tick(time)
    if (this.player.spells.insightfulEarthstormDiamond && this.player.spells.insightfulEarthstormDiamond.cooldownRemaining > 0) this.player.spells.insightfulEarthstormDiamond.tick(time)
    if (this.player.spells.powerInfusion) {
      for (let i = 0; i < this.player.spells.powerInfusion.length; i++) {
        if (this.player.spells.powerInfusion[i].cooldownRemaining > 0) this.player.spells.powerInfusion[i].tick(time)
      }
    }
    if (this.player.spells.bloodlust) {
      for (let i = 0; i < this.player.spells.bloodlust.length; i++) {
        if (this.player.spells.bloodlust[i].cooldownRemaining > 0) this.player.spells.bloodlust[i].tick(time)
      }
    }
    if (this.player.spells.innervate) {
      for (let i = 0; i < this.player.spells.innervate.length; i++) {
        if (this.player.spells.innervate[i].cooldownRemaining > 0) this.player.spells.innervate[i].tick(time)
      }
    }

    // Trinkets
    if (this.player.trinkets[0]) this.player.trinkets[0].tick(time)
    if (this.player.trinkets[1]) this.player.trinkets[1].tick(time)

    this.player.gcdRemaining = Math.max(0, this.player.gcdRemaining - time)
    this.player.mp5Timer = Math.max(0, this.player.mp5Timer - time)
    this.player.fiveSecondRuleTimer = Math.max(0, this.player.fiveSecondRuleTimer - time)
    if (this.player.mp5Timer == 0) {
      this.player.mp5Timer = 5
      if (this.player.stats.mp5 > 0 || this.player.fiveSecondRuleTimer <= 0 || (this.player.auras.innervate && this.player.auras.innervate.active)) {
        const currentPlayerMana = this.player.mana
        // MP5
        if (this.player.stats.mp5 > 0) {
          this.player.mana = Math.min(this.player.stats.maxMana, this.player.mana + this.player.stats.mp5)
        }
        // Spirit mana regen
        if (this.player.fiveSecondRuleTimer <= 0 || (this.player.auras.innervate && this.player.auras.innervate.active)) {
          // Formula from https://wowwiki-archive.fandom.com/wiki/Spirit?oldid=1572910
          let mp5FromSpirit = 5 * (0.001 + Math.sqrt(this.player.stats.intellect * this.player.stats.intellectModifier) * (this.player.stats.spirit * this.player.stats.spiritModifier) * 0.009327)
          if (this.player.auras.innervate && this.player.auras.innervate.active) {
            mp5FromSpirit *= 4
          }
          this.player.mana = Math.min(this.player.stats.maxMana, this.player.mana + mp5FromSpirit)
        }
        const manaGained = this.player.mana - currentPlayerMana
        this.player.totalManaRegenerated += manaGained
        this.player.manaGainBreakdown.mp5.casts = this.player.manaGainBreakdown.mp5.casts + 1 || 1
        this.player.manaGainBreakdown.mp5.manaGain = this.player.manaGainBreakdown.mp5.manaGain + manaGained || manaGained
        this.player.combatLog('Player gains ' + Math.round(manaGained) + ' mana from MP5 (' + Math.round(currentPlayerMana) + ' -> ' + Math.round(this.player.mana) + ')')
      }
    }

    return time
  }

  start () {
    let totalDamage = 0
    let dpsArray = []
    this.player.totalDuration = 0
    const startTime = performance.now()
    this.player.initialize()
    this.timeTotal = 0 // Used for benchmarking

    for (this.player.iteration = 1; this.player.iteration <= this.iterations; this.player.iteration++) {
      // Reset/initialize values for spells that have a cooldown or a cast time
      if (this.player.spells.shadowBolt) this.player.spells.shadowBolt.reset()
      if (this.player.spells.incinerate) this.player.spells.incinerate.reset()
      if (this.player.spells.searingPain) this.player.spells.searingPain.reset()
      if (this.player.spells.corruption) this.player.spells.corruption.reset()
      if (this.player.spells.unstableAffliction) this.player.spells.unstableAffliction.reset()
      if (this.player.spells.immolate) this.player.spells.immolate.reset()
      if (this.player.spells.curseOfDoom) this.player.spells.curseOfDoom.reset()
      if (this.player.spells.shadowburn) this.player.spells.shadowburn.reset()
      if (this.player.spells.deathCoil) this.player.spells.deathCoil.reset()
      if (this.player.spells.destructionPotion) this.player.spells.destructionPotion.reset()
      if (this.player.spells.superManaPotion) this.player.spells.superManaPotion.reset()
      if (this.player.spells.demonicRune) this.player.spells.demonicRune.reset()
      if (this.player.spells.flameCap) this.player.spells.flameCap.reset()
      if (this.player.spells.bloodFury) this.player.spells.bloodFury.reset()
      if (this.player.spells.timbalsFocusingCrystal) this.player.spells.timbalsFocusingCrystal.reset()
      if (this.player.spells.drumsOfBattle) this.player.spells.drumsOfBattle.reset()
      if (this.player.spells.drumsOfWar) this.player.spells.drumsOfWar.reset()
      if (this.player.spells.drumsOfRestoration) this.player.spells.drumsOfRestoration.reset()
      if (this.player.spells.markOfDefiance) this.player.spells.markOfDefiance.reset()
      if (this.player.spells.theLightningCapacitor) this.player.spells.theLightningCapacitor.reset()
      if (this.player.spells.bladeOfWizardry) this.player.spells.bladeOfWizardry.reset()
      if (this.player.spells.shatteredSunPendantOfAcumen) this.player.spells.shatteredSunPendantOfAcumen.reset()
      if (this.player.spells.robeOfTheElderScribes) this.player.spells.robeOfTheElderScribes.reset()
      if (this.player.spells.quagmirransEye) this.player.spells.quagmirransEye.reset()
      if (this.player.spells.shiffarsNexusHorn) this.player.spells.shiffarsNexusHorn.reset()
      if (this.player.spells.sextantOfUnstableCurrents) this.player.spells.sextantOfUnstableCurrents.reset()
      if (this.player.spells.bandOfTheEternalSage) this.player.spells.bandOfTheEternalSage.reset()
      if (this.player.spells.mysticalSkyfireDiamond) this.player.spells.mysticalSkyfireDiamond.reset()
      if (this.player.spells.insightfulEarthstormDiamond) this.player.spells.insightfulEarthstormDiamond.reset()
      if (this.player.spells.conflagrate) this.player.spells.conflagrate.reset()
      if (this.player.spells.shadowfury) this.player.spells.shadowfury.reset()
      if (this.player.spells.amplifyCurse) this.player.spells.amplifyCurse.reset()
      if (this.player.spells.powerInfusion) {
        for (let i = 0; i < this.player.spells.powerInfusion.length; i++) {
          this.player.spells.powerInfusion[i].reset()
        }
      }
      if (this.player.spells.bloodlust) {
        for (let i = 0; i < this.player.spells.bloodlust.length; i++) {
          this.player.spells.bloodlust[i].reset()
        }
      }
      if (this.player.spells.innervate) {
        for (let i = 0; i < this.player.spells.innervate.length; i++) {
          this.player.spells.innervate[i].reset()
        }
      }
      for (let i = 0; i < this.player.trinkets.length; i++) {
        if (this.player.trinkets[i]) {
          this.player.trinkets[i].reset()
        }
      }

      this.player.reset() // Resets mana, global cooldown etc.
      if (this.player.pet) {
        this.player.pet.reset()
      }

      this.player.iterationDamage = 0
      this.player.fightTime = 0
      const fightLength = random(this.minTime, this.maxTime)
      this.player.combatLog('Fight length: ' + fightLength + ' seconds')

      while (this.player.fightTime < fightLength) {
        // Use Drums
        if (this.player.auras.drumsOfBattle && !this.player.auras.drumsOfBattle.active && this.player.spells.drumsOfBattle.ready()) this.player.spells.drumsOfBattle.startCast()
        if (this.player.auras.drumsOfWar && !this.player.auras.drumsOfWar.active && this.player.spells.drumsOfWar.ready()) this.player.spells.drumsOfWar.startCast()
        if (this.player.auras.drumsOfRestoration && !this.player.auras.drumsOfRestoration.active && this.player.spells.drumsOfRestoration.ready()) this.player.spells.drumsOfRestoration.startCast()

        // Player
        if (this.player.castTimeRemaining <= 0) {
          // Spells not on the global cooldown
          // Demonic Rune
          if (this.player.spells.demonicRune && (this.player.stats.maxMana - this.player.mana) > this.player.spells.demonicRune.avgManaValue && this.player.spells.demonicRune.ready()) {
            this.player.cast('demonicRune')
          }
          // Super Mana Potion
          if (this.player.spells.superManaPotion && (this.player.stats.maxMana - this.player.mana) > this.player.spells.superManaPotion.avgManaValue && this.player.spells.superManaPotion.ready()) {
            this.player.cast('superManaPotion')
          }
          // Spells on the global cooldown
          if (this.player.gcdRemaining <= 0) {
            if (this.player.simSettings.fightType == "singleTarget") {
              const timeRemaining = fightLength - this.player.fightTime
              // Array where each element is a sub-array where the first element is the 'varName' of the spell and the second element is how much damage the spell is predicted to deal
              // This is then used to decide which spell would be best to cast if the player chose to have the sim choose the rotation for them
              let predictedDamageOfSpells = []

              // If the sim is choosing the rotation for the user then predict the damage of the three filler spells if they're available (maybe just skip Searing Pain to save time, there's no way it will ever be the best spell to cast)
              if (this.player.simChoosingRotation) {
                predictedDamageOfSpells.push([this.player.spells.shadowBolt.varName, this.player.spells.shadowBolt.predictDamage()])
                predictedDamageOfSpells.push([this.player.spells.incinerate.varName, this.player.spells.incinerate.predictDamage()])
                predictedDamageOfSpells.push([this.player.spells.searingPain.varName, this.player.spells.searingPain.predictDamage()])
              }

              // Not enough time left to cast another filler spell.
              if (timeRemaining <= this.player.spells[this.player.filler].getCastTime() && (this.player.rotation.finisher.deathCoil || this.player.rotation.finisher.shadowburn)) {
                this.player.useCooldowns()
                // todo: need to rework this thing to just choose the highest damage spell of the three (death coil, shadowburn, and conflag)
                if (this.player.spells.conflagrate && this.player.auras.immolate && this.player.auras.immolate.active && this.player.spells.conflagrate.ready()) {
                  this.player.cast('conflagrate')
                } else if (this.player.spells.shadowburn && this.player.spells.shadowburn.ready()) {
                  this.player.cast('shadowburn')
                } else if (this.player.spells.deathCoil && this.player.spells.deathCoil.ready()) {
                  this.player.cast('deathCoil')
                } else if (this.player.spells.darkPact && this.player.spells.darkPact.ready()) {
                  this.player.cast('darkPact')
                } else {
                  this.player.cast('lifeTap')
                }
              } else {
                // Cast selected curse if it's either CoR or CoE and it's not up
                if (this.player.curse && !this.player.auras[this.player.curse].active && (this.player.curse == 'curseOfRecklessness' || this.player.curse == 'curseOfTheElements') && this.player.spells[this.player.curse].ready()) {
                  this.player.cast(this.player.curse)
                } else {
                  this.player.useCooldowns()
                  // Cast Curse of Doom if there's more than 60 seconds remaining
                  if (this.player.curse && !this.player.auras[this.player.curse].active && this.player.curse == 'curseOfDoom' && timeRemaining > 60 && this.player.spells.curseOfDoom.canCast()) {
                    // If the sim is choosing the rotation for the player then predict the damage of the spell and put it in the array
                    if (this.player.simChoosingRotation) {
                      predictedDamageOfSpells.push([this.player.spells.curseOfDoom.varName, this.player.spells.curseOfDoom.predictDamage()])
                    }
                    // Else if the player is choosing the rotation themselves then just cast the highest priority spell that needs to be cast
                    else if(this.player.spells.curseOfDoom.hasEnoughMana()) {
                      if (this.player.spells.amplifyCurse && this.player.spells.amplifyCurse.ready()) {
                        this.player.cast('amplifyCurse')
                      }
                      this.player.cast('curseOfDoom')
                    }
                  }
                  // Cast Curse of Agony if CoA is the selected curse or if Curse of Doom is the selected curse and there's less than 60 seconds remaining of the fight
                  if (this.player.curse && this.player.auras.curseOfAgony && !this.player.auras.curseOfAgony.active && this.player.spells.curseOfAgony.canCast() && timeRemaining > this.player.auras.curseOfAgony.minimumDuration && ((this.player.curse == 'curseOfDoom' && !this.player.auras.curseOfDoom.active && (this.player.spells.curseOfDoom.cooldownRemaining > this.player.auras.curseOfAgony.minimumDuration || timeRemaining < 60)) || this.player.curse == 'curseOfAgony')) {
                    if (this.player.simChoosingRotation) {
                      predictedDamageOfSpells.push([this.player.spells.curseOfAgony.varName, this.player.spells.curseOfAgony.predictDamage()])
                    } else if (this.player.spells.curseOfAgony.hasEnoughMana()) {
                      if (this.player.spells.amplifyCurse && this.player.spells.amplifyCurse.ready()) {
                        this.player.cast('amplifyCurse')
                      }
                      this.player.cast('curseOfAgony')
                    }
                  }
                  // Cast Corruption if Corruption isn't up or if it will expire before the cast finishes (if no instant Corruption)
                  if (this.player.spells.corruption && (!this.player.auras.corruption.active || (this.player.auras.corruption.ticksRemaining == 1 && this.player.auras.corruption.tickTimerRemaining < this.player.spells.corruption.getCastTime())) && this.player.spells.corruption.canCast() && (timeRemaining - this.player.spells.corruption.getCastTime()) >= this.player.auras.corruption.minimumDuration) {
                    if (this.player.simChoosingRotation) {
                      predictedDamageOfSpells.push([this.player.spells.corruption.varName, this.player.spells.corruption.predictDamage()])
                    } else if (this.player.spells.corruption.hasEnoughMana()) {
                      this.player.cast('corruption')
                    }
                  }
                  // Cast Shadow Bolt if Shadow Trance (Nightfall) is active and Corruption is active as well to avoid potentially wasting another Nightfall proc
                  if (this.player.spells.shadowBolt && this.player.auras.shadowTrance && this.player.auras.shadowTrance.active && this.player.auras.corruption.active && this.player.spells.shadowBolt.canCast()) {
                    if (this.player.simChoosingRotation) {
                      // todo: check if the spell is already in the array before adding it
                      predictedDamageOfSpells.push([this.player.spells.shadowBolt.varName, this.player.spells.shadowBolt.predictDamage()])
                    } else if (this.player.spells.shadowBolt.hasEnoughMana()) {
                      this.player.cast('shadowBolt')
                    }
                  }
                  // Cast Unstable Affliction if it's not up or if it's about to expire
                  if (this.player.spells.unstableAffliction && this.player.spells.unstableAffliction.canCast() && (!this.player.auras.unstableAffliction.active || (this.player.auras.unstableAffliction.ticksRemaining == 1 && this.player.auras.unstableAffliction.tickTimerRemaining < this.player.spells.unstableAffliction.getCastTime())) && (timeRemaining - this.player.spells.unstableAffliction.getCastTime()) >= this.player.auras.unstableAffliction.minimumDuration) {
                    if (this.player.simChoosingRotation) {
                      predictedDamageOfSpells.push([this.player.spells.unstableAffliction.varName, this.player.spells.unstableAffliction.predictDamage()])
                    } else if (this.player.spells.unstableAffliction.hasEnoughMana()) {
                      this.player.cast('unstableAffliction')
                    }
                  }
                  // Cast Siphon Life if it's not up (todo: add option to only cast it while ISB is active if not using custom ISB uptime %)
                  if (this.player.spells.siphonLife && !this.player.auras.siphonLife.active && this.player.spells.siphonLife.canCast() && /* (!this.player.auras.improvedShadowBolt || this.player.auras.improvedShadowBolt.active) && */ timeRemaining >= this.player.auras.siphonLife.minimumDuration) {
                    if (this.player.simChoosingRotation) {
                      predictedDamageOfSpells.push([this.player.spells.siphonLife.varName, this.player.spells.siphonLife.predictDamage()])
                    } else if (this.player.spells.siphonLife.hasEnoughMana()) {
                      this.player.cast('siphonLife')
                    }
                  }
                  // Cast Immolate if it's not up or about to expire
                  if (this.player.spells.immolate && this.player.spells.immolate.canCast() && (!this.player.auras.immolate.active || (this.player.auras.immolate.ticksRemaining == 1 && this.player.auras.immolate.tickTimerRemaining < this.player.spells.immolate.getCastTime())) && (timeRemaining - this.player.spells.immolate.getCastTime()) >= this.player.auras.immolate.minimumDuration) {
                    if (this.player.simChoosingRotation) {
                      predictedDamageOfSpells.push([this.player.spells.immolate.varName, this.player.spells.immolate.predictDamage()])
                    } else if (this.player.spells.immolate.hasEnoughMana()) {
                      this.player.cast('immolate')
                    }
                  }
                  // Cast Shadow Bolt if Shadow Trance (Nightfall) is active
                  if (this.player.spells.shadowBolt && this.player.auras.shadowTrance && this.player.auras.shadowTrance.active && this.player.spells.shadowBolt.canCast()) {
                    if (this.player.simChoosingRotation) {
                      predictedDamageOfSpells.push([this.player.spells.shadowBolt.varName, this.player.spells.shadowBolt.predictDamage()])
                    } else if (this.player.spells.shadowBolt.hasEnoughMana()) {
                      this.player.cast('shadowBolt')
                    }
                  }
                  // Cast Shadowfury
                  if (this.player.spells.shadowfury && this.player.spells.shadowfury.canCast()) {
                    if (this.player.simChoosingRotation) {
                      predictedDamageOfSpells.push([this.player.spells.shadowfury.varName, this.player.spells.shadowfury.predictDamage()])
                    } else if (this.player.spells.shadowfury.hasEnoughMana()) {
                      this.player.cast('shadowfury')
                    }
                  }
                  // Cast filler spell if sim is not choosing the rotation for the user
                  if (!this.player.simChoosingRotation && this.player.spells[this.player.filler].ready()) {
                    this.player.cast(this.player.filler)
                  }
                  // Cast Dark Pact/Life Tap if nothing else is possible and the sim is not choosing the rotation for the user
                  if (!this.player.simChoosingRotation && this.player.spells.darkPact && this.player.spells.darkPact.ready()) {
                    this.player.cast('darkPact')
                  }
                  if (!this.player.simChoosingRotation && this.player.spells.lifeTap.ready()) {
                    this.player.cast('lifeTap')
                  }
                }
              }

              // If the sim is choosing the rotation for the player then check now which spell would be the best to cast.
              // We do another GCD check here, because we might have cast another spell (like curse) since the last check.
              if (this.player.simChoosingRotation && this.player.gcdRemaining <= 0) {
                // First index is the spell's varName and second index is the damage
                let maxDamage = ['', 0]
                for (const spell in predictedDamageOfSpells) {
                  if (predictedDamageOfSpells[spell][1] > maxDamage[1]) {
                    maxDamage = predictedDamageOfSpells[spell]
                  }
                }

                // If a max damage spell was not found or if the max damage spell isn't ready (no mana), then cast Life Tap
                if (maxDamage[0] !== '' && this.player.spells[maxDamage[0]].hasEnoughMana()) {
                  this.player.cast(maxDamage[0], maxDamage[1])
                } else {
                  this.player.castLifeTapOrDarkPact()
                }
              }
            }
            // AoE (currently just does Seed of Corruption by default)
            else {
              if (this.player.spells.seedOfCorruption.ready()) {
                this.player.useCooldowns()
                this.player.cast(this.player.spells.seedOfCorruption.varName)
              } else {
                this.player.castLifeTapOrDarkPact()
              }
            }
          }
        }

        // Pet
        if (this.player.pet && this.player.pet.mode == PetMode.AGGRESSIVE) {
          if (this.player.pet.type == PetType.MELEE) {
            if (this.player.pet.spells.melee.ready()) {
              this.player.pet.cast('melee')
            }
            if (this.player.pet.spells.cleave && this.player.pet.spells.cleave.ready()) {
              this.player.pet.cast('cleave')
            }
            if (this.player.pet.spells.lashOfPain && this.player.pet.spells.lashOfPain.ready() && (this.player.simSettings.lashOfPainUsage == 'onCooldown' || (this.player.simSettings.customIsbUptime == 'no' && (!this.player.auras.improvedShadowBolt || !this.player.auras.improvedShadowBolt.active)))) {
              this.player.pet.cast('lashOfPain')
            }
          } else if (this.player.pet.type == PetType.RANGED) {
            if (this.player.pet.spells.firebolt && this.player.pet.spells.firebolt.ready()) {
              this.player.pet.cast('firebolt')
            }
          }
        }

        // If passTime() returns 0 then the simulation somehow got stuck in an endless loop. This should never happen, so the best solution is to fix the reason it returned 0 rather than setting a minimum value for it to return.
        if (this.passTime() == 0) {
          throw "The simulation got stuck in an endless loop. If you'd like to help with fixing this bug then please export your current settings and send it to Kristofer#8003 on Discord."
        }
      }

      this.player.totalDuration += fightLength
      totalDamage += this.player.iterationDamage
      dpsArray.push(this.player.iterationDamage / fightLength)

      // Send an update to the sim worker for every 1% of progress
      if (this.player.iteration % ~~(this.iterations / 100) == 0) {
        dpsArray.sort()
        this.simulationUpdate({
          medianDps: Math.round(median(dpsArray) * 100) / 100,
          percent: Math.round((this.player.iteration / this.iterations) * 100),
          itemId: this.player.itemId,
          customStat: this.player.customStat
        })
      }

      // End all active auras
      if (this.player.auras.powerInfusion) this.player.auras.powerInfusion.fade(true)
      if (this.player.auras.improvedShadowBolt && this.player.auras.improvedShadowBolt.active) this.player.auras.improvedShadowBolt.fade(true)
      if (this.player.auras.corruption && this.player.auras.corruption.active) this.player.auras.corruption.fade(true)
      if (this.player.auras.unstableAffliction && this.player.auras.unstableAffliction.active) this.player.auras.unstableAffliction.fade(true)
      if (this.player.auras.siphonLife && this.player.auras.siphonLife.active) this.player.auras.siphonLife.fade(true)
      if (this.player.auras.immolate && this.player.auras.immolate.active) this.player.auras.immolate.fade(true)
      if (this.player.auras.curseOfAgony && this.player.auras.curseOfAgony.active) this.player.auras.curseOfAgony.fade(true)
      if (this.player.auras.curseOfTheElements && this.player.auras.curseOfTheElements.active) this.player.auras.curseOfTheElements.fade(true)
      if (this.player.auras.curseOfRecklessness && this.player.auras.curseOfRecklessness.active) this.player.auras.curseOfRecklessness.fade(true)
      if (this.player.auras.curseOfDoom && this.player.auras.curseOfDoom.active) this.player.auras.curseOfDoom.fade(true)
      if (this.player.auras.shadowTrance && this.player.auras.shadowTrance.active) this.player.auras.shadowTrance.fade(true)
      if (this.player.auras.flameshadow && this.player.auras.flameshadow.active) this.player.auras.flameshadow.fade(true)
      if (this.player.auras.shadowflame && this.player.auras.shadowflame.active) this.player.auras.shadowflame.fade(true)
      if (this.player.auras.spellstrikeProc && this.player.auras.spellstrikeProc.active) this.player.auras.spellstrikeProc.fade(true)
      if (this.player.auras.eyeOfMagtheridon && this.player.auras.eyeOfMagtheridon.active) this.player.auras.eyeOfMagtheridon.fade(true)
      if (this.player.auras.sextantOfUnstableCurrents && this.player.auras.sextantOfUnstableCurrents.active) this.player.auras.sextantOfUnstableCurrents.fade(true)
      if (this.player.auras.quagmirransEye && this.player.auras.quagmirransEye.active) this.player.auras.quagmirransEye.fade(true)
      if (this.player.auras.shiffarsNexusHorn && this.player.auras.shiffarsNexusHorn.active) this.player.auras.shiffarsNexusHorn.fade(true)
      if (this.player.auras.manaEtched4Set && this.player.auras.manaEtched4Set.active) this.player.auras.manaEtched4Set.fade(true)
      if (this.player.auras.destructionPotion && this.player.auras.destructionPotion.active) this.player.auras.destructionPotion.fade(true)
      if (this.player.auras.flameCap && this.player.auras.flameCap.active) this.player.auras.flameCap.fade(true)
      if (this.player.auras.bloodFury && this.player.auras.bloodFury.active) this.player.auras.bloodFury.fade(true)
      if (this.player.auras.bloodlust && this.player.auras.bloodlust.active) this.player.auras.bloodlust.fade(true)
      if (this.player.auras.drumsOfBattle && this.player.auras.drumsOfBattle.active) this.player.auras.drumsOfBattle.fade(true)
      if (this.player.auras.drumsOfWar && this.player.auras.drumsOfWar.active) this.player.auras.drumsOfWar.fade(true)
      if (this.player.auras.drumsOfRestoration && this.player.auras.drumsOfRestoration.active) this.player.auras.drumsOfRestoration.fade(true)
      if (this.player.auras.ashtongueTalismanOfShadows && this.player.auras.ashtongueTalismanOfShadows.active) this.player.auras.ashtongueTalismanOfShadows.fade(true)
      if (this.player.auras.darkmoonCardCrusade && this.player.auras.darkmoonCardCrusade.active) this.player.auras.darkmoonCardCrusade.fade(true)
      if (this.player.auras.theLightningCapacitor && this.player.auras.theLightningCapacitor.active) this.player.auras.theLightningCapacitor.fade(true)
      if (this.player.auras.bandOfTheEternalSage && this.player.auras.bandOfTheEternalSage.active) this.player.auras.bandOfTheEternalSage.fade(true)
      if (this.player.auras.bladeOfWizardry && this.player.auras.bladeOfWizardry.active) this.player.auras.bladeOfWizardry.fade(true)
      if (this.player.auras.shatteredSunPendantOfAcumen && this.player.auras.shatteredSunPendantOfAcumen.active) this.player.auras.shatteredSunPendantOfAcumen.fade(true)
      if (this.player.auras.robeOfTheElderScribes && this.player.auras.robeOfTheElderScribes.active) this.player.auras.robeOfTheElderScribes.fade(true)
      if (this.player.auras.mysticalSkyfireDiamond && this.player.auras.mysticalSkyfireDiamond.active) this.player.auras.mysticalSkyfireDiamond.fade(true)
      if (this.player.auras.amplifyCurse && this.player.auras.amplifyCurse.active) this.player.auras.amplifyCurse.fade(true)
      if (this.player.auras.wrathOfCenarius && this.player.auras.wrathOfCenarius.active) this.player.auras.wrathOfCenarius.fade(true)
      if (this.player.auras.innervate && this.player.auras.innervate.active) this.player.auras.innervate.fade(true)
      for (let i = 0; i < this.player.trinkets.length; i++) {
        if (this.player.trinkets[i]) {
          this.player.trinkets[i].fade(true)
        }
      }
      // End all damage over time effects to log their uptime
      if (this.player.spells.corruption && this.player.auras.corruption.active) this.player.auras.corruption.fade(true)
      if (this.player.spells.immolate && this.player.auras.immolate.active) this.player.auras.immolate.fade(true)
      if (this.player.spells.siphonLife && this.player.auras.siphonLife.active) this.player.auras.siphonLife.fade(true)
      if (this.player.spells.unstableAffliction && this.player.auras.unstableAffliction.active) this.player.auras.unstableAffliction.fade(true)
    }
    if (this.timeTotal > 0) {
      console.log(this.timeTotal)
    }

    dpsArray.sort((a, b) => a - b)
    this.simulationEnd({
      length: (performance.now() - startTime) / 1000,
      damageBreakdown: this.player.damageBreakdown,
      auraBreakdown: this.player.auraBreakdown,
      manaGainBreakdown: this.player.manaGainBreakdown,
      combatlog: this.player.combatlog,
      iterations: this.iterations,
      medianDps: Math.round(median(dpsArray) * 100) / 100,
      // The array is already sorted, so we can access the first and last elements respectively.
      minDps: Math.round(dpsArray[0] * 100) / 100,
      maxDps: Math.round(dpsArray[dpsArray.length - 1] * 100) / 100,
      totalDamage: totalDamage,
      totalDuration: this.player.totalDuration,
      totalManaRegenerated: this.player.totalManaRegenerated,
      itemId: this.player.itemId,
      customStat: this.player.customStat
    })
  }
}
