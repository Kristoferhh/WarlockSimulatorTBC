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
        // else if (this.player.pet.type == PetType.RANGED && this.player.pet.castTimeRemaining > 0 && this.player.pet.castTimeRemaining < time) time = this.player.pet.castTimeRemaining;

        if (this.player.pet.pet == PetName.IMP) {

        } else if (this.player.pet.pet == PetName.VOIDWALKER) {

        } else if (this.player.pet.pet == PetName.SUCCUBUS) {
          if (this.player.pet.spells.lashOfPain.cooldownRemaining > 0 && this.player.pet.spells.lashOfPain.cooldownRemaining < time) time = this.player.pet.spells.lashOfPain.cooldownRemaining
        } else if (this.player.pet.pet == PetName.FELHUNTER) {

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
    if (this.player.spells.bloodlust) {
      for (let i = 0; i < this.player.spells.bloodlust.length; i++) {
        if (this.player.spells.bloodlust[i].cooldownRemaining > 0 && this.player.spells.bloodlust[i].cooldownRemaining < time) time = this.player.spells.bloodlust[i].cooldownRemaining
      }
      if (this.player.auras.bloodlust.active && this.player.auras.bloodlust.durationRemaining < time) time = this.player.auras.bloodlust.durationRemaining
    }
    if (this.player.spells.timbalsFocusingCrystal && this.player.spells.timbalsFocusingCrystal.cooldownRemaining > 0 && this.player.spells.timbalsFocusingCrystal.cooldownRemaining < time) time = this.player.spells.timbalsFocusingCrystal.cooldownRemaining
    if (this.player.spells.markOfDefiance && this.player.spells.markOfDefiance.cooldownRemaining > 0 && this.player.spells.markOfDefiance.cooldownRemaining < time) time = this.player.spells.markOfDefiance.cooldownRemaining
    if (this.player.spells.drumsOfBattle && this.player.spells.drumsOfBattle.cooldownRemaining > 0 && this.player.spells.drumsOfBattle.cooldownRemaining < time) time = this.player.spells.drumsOfBattle.cooldownRemaining
    if (this.player.spells.drumsOfWar && this.player.spells.drumsOfWar.cooldownRemaining > 0 && this.player.spells.drumsOfWar.cooldownRemaining < time) time = this.player.spells.drumsOfWar.cooldownRemaining
    if (this.player.spells.drumsOfRestoration && this.player.spells.drumsOfRestoration.cooldownRemaining > 0 && this.player.spells.drumsOfRestoration.cooldownRemaining < time) time = this.player.spells.drumsOfRestoration.cooldownRemaining
    if (this.player.auras.drumsOfBattle && this.player.auras.drumsOfBattle.active && this.player.auras.drumsOfBattle.durationRemaining < time) time = this.player.auras.drumsOfBattle.durationRemaining
    if (this.player.auras.drumsOfWar && this.player.auras.drumsOfWar.active && this.player.auras.drumsOfWar.durationRemaining < time) time = this.player.auras.drumsOfWar.durationRemaining
    if (this.player.auras.drumsOfRestoration && this.player.auras.drumsOfRestoration.active && this.player.auras.drumsOfRestoration.tickTimerRemaining < time) time = this.player.auras.drumsOfRestoration.tickTimerRemaining
    if (this.player.auras.shadowTrance && this.player.auras.shadowTrance.active && this.player.auras.shadowTrance.durationRemaining < time) time = this.player.auras.shadowTrance.durationRemaining
    if (this.player.auras.flameshadow && this.player.auras.flameshadow.active && this.player.auras.flameshadow.durationRemaining < time) time = this.player.auras.flameshadow.durationRemaining
    if (this.player.auras.shadowflame && this.player.auras.shadowflame.active && this.player.auras.shadowflame.durationRemaining < time) time = this.player.auras.shadowflame.durationRemaining
    if (this.player.auras.spellstrikeProc && this.player.auras.spellstrikeProc.active && this.player.auras.spellstrikeProc.durationRemaining < time) time = this.player.auras.spellstrikeProc.durationRemaining
    if (this.player.auras.powerInfusion && this.player.auras.powerInfusion.cooldownRemaining > 0 && this.player.auras.powerInfusion.cooldownRemaining < time) time = this.player.auras.powerInfusion.cooldownRemaining
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

    // Pass time
    // This needs to be the first modified value since the time in combat needs to be updated before spells start dealing damage/auras expiring etc. for the combat logging.
    this.player.fightTime += time
    this.player.castTimeRemaining = Math.max(0, this.player.castTimeRemaining - time)

    // Pet
    if (this.player.pet) this.player.pet.tick(time)

    // Spells
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
    if (this.player.spells.bloodlust) {
      for (let i = 0; i < this.player.spells.bloodlust.length; i++) {
        if (this.player.spells.bloodlust[i].cooldownRemaining > 0) this.player.spells.bloodlust[i].tick(time)
      }
    }
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

    // Auras
    if (this.player.auras.powerInfusion) this.player.auras.powerInfusion.tick(time)
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

    // Trinkets
    if (this.player.trinkets[0]) this.player.trinkets[0].tick(time)
    if (this.player.trinkets[1]) this.player.trinkets[1].tick(time)

    this.player.gcdRemaining = Math.max(0, this.player.gcdRemaining - time)
    this.player.mp5Timer = Math.max(0, this.player.mp5Timer - time)
    if (this.player.mp5Timer == 0) {
      this.player.mp5Timer = 5
      if (this.player.stats.mp5 > 0) {
        this.player.totalManaRegenerated += this.player.stats.mp5
        this.player.manaGainBreakdown.mp5.casts = this.player.manaGainBreakdown.mp5.casts + 1 || 1
        this.player.manaGainBreakdown.mp5.manaGain = this.player.manaGainBreakdown.mp5.manaGain + this.player.stats.mp5 || this.player.stats.mp5
        this.player.mana = Math.min(this.player.stats.maxMana, this.player.mana + this.player.stats.mp5)
        this.player.combatLog('Player gains ' + Math.round(this.player.stats.mp5) + ' mana from MP5. Player mana: ' + Math.round(this.player.mana) + '/' + Math.round(this.player.stats.maxMana))
      }
      if (this.player.pet && this.player.pet.stats.mp5 > 0) {
        if (this.player.pet.stats.mana < this.player.pet.stats.maxMana) {
          this.player.combatLog(this.player.pet.name + ' gains ' + this.player.pet.stats.mp5 + ' mana from MP5. Pet mana: ' + Math.round(this.player.pet.stats.mana) + '/' + Math.round(this.player.pet.stats.maxMana))
        }
        this.player.pet.stats.mana = Math.min(this.player.pet.stats.maxMana, this.player.pet.stats.mana + this.player.pet.stats.mp5)
      }
    }

    return time
  }

  start () {
    let totalDamage = 0
    let totalDuration = 0
    let minDps = 9999
    let maxDps = 0
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
      if (this.player.auras.powerInfusion) this.player.auras.powerInfusion.reset()
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
      if (this.player.spells.bloodlust) {
        for (let i = 0; i < this.player.spells.bloodlust.length; i++) {
          this.player.spells.bloodlust[i].reset()
        }
      }
      this.player.reset() // Resets mana, global cooldown etc.
      if (this.player.pet) this.player.pet.reset()

      this.player.iterationDamage = 0
      this.player.fightTime = 0
      const fightLength = random(this.minTime, this.maxTime)
      this.player.combatLog('Fight length: ' + fightLength + ' seconds')

      while (this.player.fightTime < fightLength) {
        // Use Drums
        if (this.player.auras.drumsOfBattle && !this.player.auras.drumsOfBattle.active && this.player.spells.drumsOfBattle.ready()) this.player.spells.drumsOfBattle.startCast()
        if (this.player.auras.drumsOfWar && !this.player.auras.drumsOfWar.active && this.player.spells.drumsOfWar.ready()) this.player.spells.drumsOfWar.startCast()
        if (this.player.auras.drumsOfRestoration && !this.player.auras.drumsOfRestoration.active && this.player.spells.drumsOfRestoration.ready()) this.player.spells.drumsOfRestoration.startCast()

        // Pet
        if (this.player.pet && this.player.pet.mode == PetMode.AGGRESSIVE) {
          if (this.player.pet.type == PetType.MELEE) {
            if (this.player.pet.spells.melee.ready()) {
              this.player.pet.cast('melee')
            }
            if (this.player.pet.spells.cleave && this.player.pet.spells.cleave.ready()) {
              this.player.pet.cast('cleave')
            }
            if (this.player.pet.spells.lashOfPain && this.player.pet.spells.lashOfPain.ready() && (!this.player.auras.improvedShadowBolt || this.player.simSettings.lashOfPainUsage === 'onCooldown' || (this.player.simSettings.lashOfPainUsage == 'noISB' && !this.player.auras.improvedShadowBolt.active))) {
              this.player.pet.cast('lashOfPain')
            }
          } /* else if (this.player.pet.type == PetType.RANGED) {

					} */
        }
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
            const timeRemaining = fightLength - this.player.fightTime
            // Not enough time left to cast another filler spell.
            if ((this.player.rotation.finisher.deathCoil || this.player.rotation.finisher.shadowburn) && timeRemaining <= (this.player.spells[this.player.filler].castTime + this.player.spells[this.player.filler].travelTime)) {
              this.player.useCooldownsIfAvailable()
              // Cast Death Coil if there's time to cast both Death Coil and Shadowburn (need to cast Death Coil first because of the travel time). Otherwise only cast Shadowburn
              if (this.player.rotation.finisher.deathCoil && this.player.spells.deathCoil.ready() && (timeRemaining - this.player.gcdValue > this.player.spells.deathCoil.travelTime)) {
                this.player.cast('deathCoil')
              } else if (this.player.rotation.finisher.shadowburn && this.player.spells.shadowburn.ready()) {
                this.player.cast('shadowburn')
              } else {
                this.player.cast('lifeTap')
              }
            } else {
              // Checks if a curse is assigned and if the curse is not up.
              // If the curse is not up, it checks if the curse is Curse of Doom. If the curse if Curse of Doom then it checks if Curse of Agony is not up as well (used when there's <60sec left of the fight).
              // If the curse is not Curse of Doom or the curse is Curse of Doom *and* both CoD and CoA are down (and if there's enough time left for them to do enough damage to warrant casting them) then it casts the curse.
              if (this.player.curse && !this.player.auras[this.player.curse].active && (this.player.curse !== 'curseOfDoom' || (this.player.curse == 'curseOfDoom' && !this.player.auras.curseOfAgony.active && (timeRemaining > 60 || timeRemaining >= this.player.auras.curseOfAgony.minimumDuration)))) {
                if (this.player.curse !== 'curseOfDoom' || (this.player.curse == 'curseOfDoom' && timeRemaining > 60 && this.player.spells.curseOfDoom.ready())) {
                  this.player.cast(this.player.curse)
                } else if (timeRemaining >= this.player.auras.curseOfAgony.minimumDuration) {
                  this.player.cast('curseOfAgony')
                }
              } else {
                this.player.useCooldownsIfAvailable()
                if (this.player.spells.corruption && (!this.player.auras.corruption.active || (this.player.auras.corruption.ticksRemaining == 1 && this.player.auras.corruption.tickTimerRemaining < this.player.spells.corruption.getCastTime())) && this.player.spells.corruption.ready() && (timeRemaining - this.player.spells.corruption.castTime) >= this.player.auras.corruption.minimumDuration) {
                  this.player.cast('corruption')
                } else if (this.player.spells.shadowBolt && this.player.auras.shadowTrance && this.player.auras.shadowTrance.active && this.player.auras.corruption.active) {
                  this.player.cast('shadowBolt')
                } else if (this.player.spells.curseOfAgony && !this.player.auras.curseOfAgony.active && this.player.spells.curseOfAgony.ready() && timeRemaining >= this.player.auras.curseOfAgony.minimumDuration) {
                  this.player.cast('curseOfAgony')
                } else if (this.player.spells.unstableAffliction && (!this.player.auras.unstableAffliction.active || (this.player.auras.unstableAffliction.ticksRemaining == 1 && this.player.auras.unstableAffliction.tickTimerRemaining < this.player.spells.unstableAffliction.getCastTime())) && this.player.spells.unstableAffliction.ready() && (timeRemaining - this.player.spells.unstableAffliction.castTime) >= this.player.auras.unstableAffliction.minimumDuration) {
                  this.player.cast('unstableAffliction')
                } else if (this.player.spells.siphonLife && !this.player.auras.siphonLife.active && this.player.spells.siphonLife.ready() && /*(!this.player.auras.improvedShadowBolt || this.player.auras.improvedShadowBolt.active) &&*/ timeRemaining >= this.player.auras.siphonLife.minimumDuration) {
                  this.player.cast('siphonLife')
                } else if (this.player.spells.immolate && (!this.player.auras.immolate.active || (this.player.auras.immolate.ticksRemaining == 1 && this.player.auras.immolate.tickTimerRemaining < this.player.spells.immolate.getCastTime())) && this.player.spells.immolate.ready() && (timeRemaining - this.player.spells.immolate.castTime) >= this.player.auras.immolate.minimumDuration) {
                  this.player.cast('immolate')
                }
                // Use Life Tap instead of the filler spell even if the player is at high mana to avoid having to Life Tap when the player is out of mana and a cooldown is active such as a trinket proc or Power Infusion
                // It uses Life Tap if Life Tap would not over cap the player on mana, there's more than 20 seconds left of the fight, and the player's mana percentage is less than the amount of time left in the fight (as a percentage) e.g. if the player is at 50% mana but there's 70% left of the fight.
                // This can be improved further but I think this is a good method of making sure the player doesn't have to Life Tap while important cooldowns are active.
                else if (this.player.importantAuraCounter == 0 && timeRemaining > 20 && (timeRemaining / fightLength) > (this.player.mana / this.player.stats.maxMana) && !this.player.areAnyCooldownsReady() && (this.player.spells.lifeTap.manaGain() + this.player.mana < this.player.stats.maxMana)) {
                  this.player.cast('lifeTap')
                } else if (this.player.spells[this.player.filler].ready()) {
                  this.player.cast(this.player.filler)
                } else {
                  this.player.cast('lifeTap')
                }
              }
            }
          }
        }

        // If passTime() returns 0 then the simulation somehow got stuck in an endless loop. This should never happen, so the best solution is to fix the reason it returned 0 rather than setting a minimum value for it to return.
        if (this.passTime() == 0) {
          throw "The simulation got stuck in an endless loop. If you'd like to help with fixing this bug then please mention which spells you had selected and which gear you had equipped to Kristofer#8003 on Discord."
        }
      }

      totalDuration += fightLength
      totalDamage += this.player.iterationDamage

      const iterationDps = this.player.iterationDamage / fightLength
      if (iterationDps > maxDps) maxDps = iterationDps
      else if (iterationDps < minDps) minDps = iterationDps

      // Send an update to the sim worker for every 1% of progress
      if (this.player.iteration % ~~(this.iterations / 100) == 0) {
        this.simulationUpdate({
          avgDps: Math.round((totalDamage / totalDuration) * 100) / 100,
          percent: Math.round((this.player.iteration / this.iterations) * 100),
          itemId: this.player.itemId,
          customStat: this.player.customStat
        })
      }

      // Reset/end all active auras and spell cooldowns
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
      for (let i = 0; i < this.player.trinkets.length; i++) {
        if (this.player.trinkets[i]) {
          this.player.trinkets[i].fade(true)
          this.player.trinkets[i].reset()
        }
      }
      // End all damage over time effects to log their uptime
      if (this.player.spells.corruption && this.player.auras.corruption.active) this.player.auras.corruption.fade(true)
      if (this.player.spells.immolate && this.player.auras.immolate.active) this.player.auras.immolate.fade(true)
      if (this.player.spells.siphonLife && this.player.auras.siphonLife.active) this.player.auras.siphonLife.fade(true)
      if (this.player.spells.unstableAffliction && this.player.auras.unstableAffliction.active) this.player.auras.unstableAffliction.fade(true)
    }
    if (this.timeTotal > 0) console.log(this.timeTotal)
    if (this.player.pet && this.player.pet.mode == PetMode.AGGRESSIVE) console.log(JSON.stringify(this.player.pet.damageBreakdown))

    this.simulationEnd({
      minDps: Math.round(minDps * 100) / 100,
      maxDps: Math.round(maxDps * 100) / 100,
      length: (performance.now() - startTime) / 1000,
      damageBreakdown: this.player.damageBreakdown,
      auraBreakdown: this.player.auraBreakdown,
      manaGainBreakdown: this.player.manaGainBreakdown,
      combatlog: this.player.combatlog,
      iterations: this.iterations,
      totalDamage: totalDamage,
      totalDuration: totalDuration,
      avgDps: Math.round((totalDamage / totalDuration) * 100) / 100,
      totalManaRegenerated: this.player.totalManaRegenerated,
      itemId: this.player.itemId,
      customStat: this.player.customStat
    })
  }
}
