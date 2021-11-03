class Spell {
  constructor (player) {
    this.player = player
    this.castTime = 0
    this.manaCost = 0
    this.cooldown = 0
    this.name = null
    this.varName = null // Same as this.name except it's written in camelCase
    this.isDot = false
    this.modifier = 1
    this.doesDamage = false
    this.canCrit = false
    this.school = null // The school that the spell belongs to such as shadow or fire
    this.type = null // affliction or destruction
    this.travelTime = 0
    this.bonusCrit = 0
    this.cooldownRemaining = 0
    this.isNonWarlockAbility = false
    this.casting = false
    this.isItem = false
    this.onGcd = true
    this.isProc = false
    this.breakdownTable = 'damage'
  }

  reset () {
    this.cooldownRemaining = 0
    this.casting = false
  }

  setup () {
    this.varName = camelCase(this.name)
    this.player[this.breakdownTable + 'Breakdown'][this.varName] = this.player[this.breakdownTable + 'Breakdown'][this.varName] || { name: this.name }
    if (this.minDmg && this.maxDmg) {
      this.dmg = (this.minDmg + this.maxDmg) / 2
    }
    if (this.minMana && this.maxMana) {
      this.avgManaValue = (this.minMana + this.maxMana) / 2
    }
  }

  ready () {
    return this.canCast() && this.hasEnoughMana()
  }

  canCast() {
    return (!this.onGcd || this.isNonWarlockAbility || this.player.gcdRemaining <= 0) && (this.isProc || this.isNonWarlockAbility || this.player.castTimeRemaining <= 0) && this.cooldownRemaining <= 0
  }

  hasEnoughMana() {
    return this.manaCost <= this.player.mana
  }

  getCastTime () {
    let hastePercent = this.player.stats.hastePercent
    
    // If both Bloodlust and Power Infusion are active then remove the 20% bonus from Power Infusion since they don't stack
    if (this.player.auras.powerInfusion && this.player.auras.bloodlust && this.player.auras.powerInfusion.active && this.player.auras.bloodlust.active) {
      hastePercent /= 1.2
    }

    return Math.round(this.castTime / ((1 + (this.player.stats.hasteRating / hasteRatingPerPercent / 100)) * hastePercent) * 10000) / 10000 + this.player.spellDelay
  }

  startCast (predictedDamage = 0) {
    if (this.onGcd && !this.isNonWarlockAbility) {
      this.player.gcdRemaining = this.player.getGcdValue(this.varName)
    }

    let combatLogMsg = ''
    if (this.castTime > 0) {
      this.casting = true
      this.player.castTimeRemaining = this.getCastTime()
      if (!this.isProc) {
        combatLogMsg += 'Started casting ' + this.name + ' - Cast time: ' + (this.player.castTimeRemaining - this.player.spellDelay) + ' (' + Math.round((((1 + (this.player.stats.hasteRating / hasteRatingPerPercent / 100)) * this.player.stats.hastePercent) - 1) * 10000) / 100 + '% haste at a base cast speed of ' + this.castTime + ').'
      }
    } else {
      if (!this.isProc) {
        combatLogMsg += 'Cast ' + this.name
      }
      this.cast()
    }
    if (this.onGcd && !this.isNonWarlockAbility) {
      combatLogMsg += ' - Global cooldown: ' + this.player.gcdRemaining
    }
    if (predictedDamage > 0) {
      combatLogMsg += ' - Estimated damage / Cast Time: ' + Math.round(predictedDamage)
    }
    this.player.combatLog(combatLogMsg)
  }

  // Called when a spell finishes casting or immediately called if the spell has no cast time.
  cast () {
    const currentMana = this.player.mana
    if (!this.isAura) {
      this.player[this.breakdownTable + 'Breakdown'][this.varName].casts = this.player[this.breakdownTable + 'Breakdown'][this.varName].casts + 1 || 1
    }
    // Infinite mana setting
    // For some reason the setting sometimes has the value 'on' if it isn't enabled when loading settings from previous sessions.
    // The setting is only enabled when it is equal to true, so lower the player's mana if it's equal to false or 'on'
    if (this.player.simSettings.infinitePlayerMana !== 'yes') {
      this.player.mana -= (this.manaCost * this.player.stats.manaCostModifier)
    }
    if (this.manaCost > 0) {
      this.player.fiveSecondRuleTimer = 5
    }
    this.cooldownRemaining = this.cooldown
    this.casting = false
    if (this.castTime > 0) {
      this.player.combatLog('Finished casting ' + this.name + ' - Mana: ' + Math.round(currentMana) + ' -> ' + Math.round(this.player.mana) + ' - Mana Cost: ' + Math.round(this.manaCost) + ' - Mana Cost Modifier: ' + Math.round(this.player.stats.manaCostModifier * 100) + '%')
    }

    let isCrit = false  
    if (this.canCrit) {
      // Checks if the spell is a crit.
      isCrit = this.player.isCrit(this.type, this.bonusCrit)
      if (isCrit) {
        // Increment the crit counter whether the spell hits or not so that the crit % on the damage breakdown is correct. Otherwise the crit % will be lower due to lost crits when the spell misses.
        this.player[this.breakdownTable + 'Breakdown'][this.varName].crits = this.player[this.breakdownTable + 'Breakdown'][this.varName].crits + 1 || 1
      }
    }

    // Check if the spell hits or misses
    if (((!this.isItem && !this.isNonWarlockAbility && this.varName !== 'amplifyCurse') || this.doesDamage) && !this.player.isHit(this.type === 'affliction')) {
      this.player.combatLog(this.name + ' *resist*')
      this.player[this.breakdownTable + 'Breakdown'][this.varName].misses = this.player[this.breakdownTable + 'Breakdown'][this.varName].misses + 1 || 1
      return
    }

    if (this.isDot || this.isAura) {
      this.player.auras[this.varName].apply()
    }

    if (this.doesDamage) {
      this.damage(isCrit)
    }

    // If it's an item such as mana potion, demonic rune, destruction potion, or if it's a proc with a hidden cooldown like Blade of Wizardry or Robe of the Elder Scribes then jump out of the method
    if (this.isItem || this.isProc || this.isNonWarlockAbility || this.varName == 'amplifyCurse') {
      return
    }

    // Judgement of Wisdom (50% proc rate)
    if (this.player.selectedAuras.judgementOfWisdom && random(1, 100) <= 50) {
      const manaVal = 74
      const currentMana = this.player.mana
      const manaGained = Math.min(this.player.stats.maxMana - currentMana, manaVal)
      this.player.manaGainBreakdown.judgementOfWisdom.casts = this.player.manaGainBreakdown.judgementOfWisdom.casts + 1 || 1
      this.player.manaGainBreakdown.judgementOfWisdom.manaGain = this.player.manaGainBreakdown.judgementOfWisdom.manaGain + manaGained || manaGained
      this.player.totalManaRegenerated += manaGained
      this.player.mana = Math.min(this.player.stats.maxMana, currentMana + manaGained)
      this.player.combatLog('Player gains ' + manaGained + ' mana from Judgement of Wisdom (' + Math.round(currentMana) + ' -> ' + Math.round(this.player.mana) + ')')
    }

    // T4 2-set
    if (this.player.sets['645'] >= 2 && ['shadow', 'fire'].includes(this.school) && random(1, 100) <= this.player.auras.shadowflame.procChance) {
      if (this.school == 'shadow') {
        this.player.auras.flameshadow.apply()
      } else if (this.school == 'fire') {
        this.player.auras.shadowflame.apply()
      }
    }

    // Spellstrike 2-set
    if (this.player.sets['559'] == 2 && random(1, 100) <= this.player.auras.spellstrikeProc.procChance) {
      this.player.auras.spellstrikeProc.apply()
    }

    // Quagmirran's Eye
    if (this.player.spells.quagmirransEye && this.player.spells.quagmirransEye.ready() && random(1, 100) <= this.player.spells.quagmirransEye.procChance) {
      this.player.spells.quagmirransEye.cast()
    }

    // Mana-Etched Regalia 4-set (2% proc chance)
    if (this.player.sets['658'] >= 4 && random(1, 100) <= this.player.auras.manaEtched4Set.procChance) {
      this.player.auras.manaEtched4Set.apply()
    }

    // Mark of Defiance
    if (this.player.spells.markOfDefiance && random(1, 100) <= this.player.spells.markOfDefiance.procChance) {
      this.player.spells.markOfDefiance.cast()
    }

    // Darkmoon Card: Crusade
    if (this.player.auras.darkmoonCardCrusade) {
      this.player.auras.darkmoonCardCrusade.apply()
    }

    // Band of the Eternal Sage
    if (this.player.spells.bandOfTheEternalSage && this.player.spells.bandOfTheEternalSage.ready() && random(1, 100) <= this.player.spells.bandOfTheEternalSage.procChance) {
      this.player.spells.bandOfTheEternalSage.cast()
    }

    // Blade of Wizardry
    if (this.player.spells.bladeOfWizardry && this.player.spells.bladeOfWizardry.ready() && random(1, 100) <= this.player.auras.bladeOfWizardry.procChance) {
      this.player.spells.bladeOfWizardry.cast()
    }

    // Mystical Skyfire Diamond
    if (this.player.spells.mysticalSkyfireDiamond && this.player.spells.mysticalSkyfireDiamond.ready() && random(1, 100) <= this.player.spells.mysticalSkyfireDiamond.procChance) {
      this.player.spells.mysticalSkyfireDiamond.cast()
    }

    // Robe of the Elder Scribes
    if (this.player.spells.robeOfTheElderScribes && this.player.spells.robeOfTheElderScribes.ready() && random(1, 100) <= this.player.spells.robeOfTheElderScribes.procChance) {
      this.player.spells.robeOfTheElderScribes.cast()
    }

    // Insightful Earthstorm Diamond
    if (this.player.spells.insightfulEarthstormDiamond && this.player.spells.insightfulEarthstormDiamond.ready() && random(1, 100) <= this.player.spells.insightfulEarthstormDiamond.procChance) {
      this.player.spells.insightfulEarthstormDiamond.cast()
    }

    // Wrath of Cenarius
    if (this.player.auras.wrathOfCenarius && random(1,100) <= this.player.auras.wrathOfCenarius.procChance) {
      this.player.auras.wrathOfCenarius.apply()
    }
  }

  getModifier () {
    return this.player.stats[this.school + 'Modifier'] * this.modifier
  }

  damage (isCrit) {
    // [baseDamage, dmg, modifier, partialResistMultiplier, spellPower]
    const constantDamage = this.getConstantDamage()
    // Make a variable for the base damage to output into the combat log
    const baseDamage = constantDamage[0]
    let dmg = constantDamage[1]
    const modifier = constantDamage[2]
    const partialResistMultiplier = constantDamage[3]
    const spellPower = constantDamage[4]
    let critMultiplier = this.player.critMultiplier

    if (isCrit) {
      critMultiplier = this.getCritMultiplier(critMultiplier)
      dmg *= critMultiplier
      this.onCritProcs()
    } else {
      // Decrement the Improved Shadow Bolt stacks if it's not a crit
      if (this.school == 'shadow' && !this.isDot && this.player.auras.improvedShadowBolt && this.player.auras.improvedShadowBolt.active && this.player.simSettings.customIsbUptime == 'no') {
        this.player.auras.improvedShadowBolt.decrementStacks()
      }
    }

    this.onDamageProcs()

    // Combat Log
    let combatLogMsg = this.name + ' '
    if (isCrit) combatLogMsg += '*'
    combatLogMsg += Math.round(dmg)
    if (isCrit) combatLogMsg += '*'
    combatLogMsg += ' (' + baseDamage + ' Base Damage - ' + Math.round(this.coefficient * 1000) / 1000 + ' Coefficient - ' + Math.round(spellPower) + ' Spell Power - '
    if (isCrit) combatLogMsg += critMultiplier.toFixed(2) + '% Crit Multiplier - '
    combatLogMsg += Math.round(modifier * 10000) / 100 + '% Damage Modifier - ' + Math.round(partialResistMultiplier * 1000) / 10 + '% Partial Resist Multiplier)'
    this.player.combatLog(combatLogMsg)
    this.player[this.breakdownTable + 'Breakdown'][this.varName].damage = this.player[this.breakdownTable + 'Breakdown'][this.varName].damage + dmg || dmg
    this.player.iterationDamage += dmg

    // T5 4pc. Increase Corruption & Immolate DoT dmg when Shadow Bolt/Incinerate hits
    if (this.player.sets['646'] >= 4) {
      if (this.varName == 'shadowBolt' && this.player.auras.corruption && this.player.auras.corruption.active) {
        this.player.auras.corruption.t5BonusModifier *= 1.1
      } else if (this.varName == 'incinerate' && this.player.auras.immolate && this.player.auras.immolate.active) {
        this.player.auras.immolate.t5BonusModifier *= 1.1
      }
    }
  }

  // Returns the non-RNG damage of the spell (basically just the base damage + spell power + damage modifiers, no crit/miss etc.)
  // todo: investigate this noRNG variable
  getConstantDamage(noRNG = false) {
    let dmg = this.player.simSettings.randomizeValues === 'yes' && this.minDmg && this.maxDmg && !noRNG ? random(this.minDmg, this.maxDmg) : this.dmg
    const baseDamage = dmg // Creating a variable for the base damage just for the combat log
    const spellPower = this.player.getSpellPower() + (this.school == 'shadow' ? this.player.stats.shadowPower : this.school == 'fire' ? this.player.stats.firePower : 0)
    let modifier = this.getModifier()
    const partialResistMultiplier = this.player.getPartialResistMultiplier(this.player.enemy[this.school + 'Resist'])

    // If casting Incinerate and Immolate is up, add the bonus damage.
    if (this.varName == 'incinerate' && this.player.auras.immolate && this.player.auras.immolate.active) {
      dmg += this.player.simSettings.randomizeValues === 'yes' && !noRNG ? random(this.bonusDamageFromImmolateMin, this.bonusDamageFromImmolateMax) : this.bonusDamageFromImmolate
    }

    // Add damage from Spell Power
    dmg += spellPower * this.coefficient

    // Improved Shadow Bolt
    if (this.school == 'shadow' && this.player.auras.improvedShadowBolt && this.player.auras.improvedShadowBolt.active && this.player.simSettings.customIsbUptime == 'no') {
      modifier *= this.player.auras.improvedShadowBolt.modifier
    }

    dmg *= modifier * partialResistMultiplier

    return [baseDamage, dmg, modifier, partialResistMultiplier, spellPower]
  }

  // Returns the number that the damage of the spell will be multiplied with if it crits
  getCritMultiplier(critMult) {
    let critMultiplier = critMult

    // Chaotic Skyfire Diamond meta gem
    if (this.player.metaGemIds.includes(34220)) {
      critMultiplier *= 1.03
    }
    // Ruin
    if (this.type == 'destruction' && this.player.talents.ruin > 0) {
      // Ruin doubles the *bonus* of your crits, not the damage of the crit itself
      // So if your crit damage % is e.g. 154.5% it would become 209% because the 54.5% is being doubled
      critMultiplier -= 1
      critMultiplier *= 2
      critMultiplier += 1
    }

    return critMultiplier
  }

  // Predicts how much damage the spell will do based on crit % and miss %
  predictDamage() {
    const normalDamage = this.getConstantDamage()[1] || 0
    let critDamage = 0, critChance = 0, chanceToNotCrit = 0

    if (this.canCrit) {
      critDamage = normalDamage * this.getCritMultiplier(this.player.critMultiplier)
      critChance = this.player.getCritChance(this.type) / 100
      chanceToNotCrit = 1 - critChance
    }
    const hitChance = this.player.getHitChance(this.type == 'affliction') / 100
    let estimatedDamage = this.canCrit ? (normalDamage * chanceToNotCrit) + (critDamage * critChance) : normalDamage

    // Add the predicted damage of the DoT over its full duration
    if (this.isDot) {
      estimatedDamage += this.player.auras[this.varName].predictDamage()
    }

    // If the player is not using a custom ISB uptime, they have the ISB talent selected, but the ISB aura is not active, then give some % modifier as an "average" for the damage.
    // Without this, the sim will choose Incinerate over Shadow Bolt because it basically just doesn't know that ISB exists
    if (this.school == 'shadow' && this.player.simSettings.customIsbUptime !== 'yes' && this.player.auras.improvedShadowBolt && !this.player.auras.improvedShadowBolt.active) {
      // If this isn't the player's first iteration then check what their ISB uptime is and add that %
      if (this.player.iteration > 1) {
        estimatedDamage *= (1 + 0.2 * this.player.auras.improvedShadowBolt.uptimeSoFar)
      }
      // If it's the first iteration where we don't have enough data to assume what the player's ISB uptime is, then add a fixed amount
      else {
        estimatedDamage *= 1.15;
      }
    }

    return (estimatedDamage * hitChance) / Math.max(this.player.getGcdValue(this.varName), this.getCastTime())
  }

  tick (t) {
    if (this.cooldownRemaining > 0 && this.cooldownRemaining - t <= 0) {
      this.player.combatLog(this.name + ' is off cooldown')
    }
    this.cooldownRemaining = Math.max(0, this.cooldownRemaining - t)

    if (this.casting && this.player.castTimeRemaining <= 0) {
      this.cast()
    }
  }

  onCritProcs() {
    // Apply ISB debuff if casting Shadow Bolt
    if (this.player.simSettings.customIsbUptime == 'no' && this.varName == 'shadowBolt' && this.player.talents.improvedShadowBolt > 0) {
      this.player.auras.improvedShadowBolt.apply()
    }
    // The Lightning Capacitor
    if (this.player.spells.theLightningCapacitor) {
      this.player.spells.theLightningCapacitor.startCast()
    }
    // Sextant of Unstable Currents
    if (this.player.trinketIds.includes(30626) && this.player.spells.sextantOfUnstableCurrents.ready() && random(1, 100) <= this.player.spells.sextantOfUnstableCurrents.procChance) {
      this.player.spells.sextantOfUnstableCurrents.cast()
    }
    // Shiffar's Nexus-Horn
    if (this.player.trinketIds.includes(28418) && this.player.spells.shiffarsNexusHorn.ready() && random(1, 100) <= this.player.spells.shiffarsNexusHorn.procChance) {
      this.player.spells.shiffarsNexusHorn.cast()
    }
  }

  onDamageProcs() {
    // Confirm that this procs on dealing damage
    // Shattered Sun Pendant of Acumen
    if (this.player.exaltedWithShattrathFaction && this.player.spells.shatteredSunPendantOfAcumen && this.player.spells.shatteredSunPendantOfAcumen.cooldownRemaining <= 0 && random(1, 100) <= this.player.spells.shatteredSunPendantOfAcumen.procChance) {
      this.player.spells.shatteredSunPendantOfAcumen.cast()
    }
  }
}

class ShadowBolt extends Spell {
  constructor (player) {
    super(player)
    this.castTime = this.calculateCastTime()
    this.manaCost = 420 * (1 - 0.01 * player.talents.cataclysm)
    this.coefficient = (3 / 3.5) + (0.04 * player.talents.shadowAndFlame)
    this.minDmg = 544
    this.maxDmg = 607
    this.name = 'Shadow Bolt'
    this.doesDamage = true
    this.canCrit = true
    this.school = 'shadow'
    this.type = 'destruction'
    this.travelTime = player.spellTravelTime
    this.setup()

     // T6 4pc
    if (player.sets['670'] >= 4) {
      this.modifier *= 1.06
    }
  }

  startCast () {
    if (this.player.auras.shadowTrance && this.player.auras.shadowTrance.active) {
      this.castTime = 0
    }
    super.startCast()
    if (this.player.auras.shadowTrance && this.player.auras.shadowTrance.active) {
      this.castTime = this.calculateCastTime()
      this.player.auras.shadowTrance.fade()
    }
  }

  calculateCastTime () {
    return 3 - (0.1 * this.player.talents.bane)
  }
}

class Incinerate extends Spell {
  constructor (player) {
    super(player)
    this.castTime = Math.round((2.5 * (1 - 0.02 * player.talents.emberstorm)) * 100) / 100
    this.manaCost = 355 * (1 - 0.01 * player.talents.cataclysm)
    this.coefficient = (2.5 / 3.5) + (0.04 * player.talents.shadowAndFlame)
    this.minDmg = 444
    this.maxDmg = 514
    this.bonusDamageFromImmolateMin = 111
    this.bonusDamageFromImmolateMax = 128
    this.bonusDamageFromImmolate = (this.bonusDamageFromImmolateMin + this.bonusDamageFromImmolateMax) / 2 // The bonus damage gained when Immolate is up on the target
    this.name = 'Incinerate'
    this.doesDamage = true
    this.canCrit = true
    this.school = 'fire'
    this.type = 'destruction'
    this.travelTime = player.spellTravelTime
    this.setup()

    // T6 4pc
    if (player.sets['670'] >= 4) {
      this.modifier *= 1.06
    }
  }
}

class SearingPain extends Spell {
  constructor (player) {
    super(player)
    this.castTime = 1.5
    this.manaCost = 205 * (1 - 0.01 * player.talents.cataclysm)
    this.coefficient = 1.5 / 3.5
    this.minDmg = 270
    this.maxDmg = 320
    this.name = 'Searing Pain'
    this.doesDamage = true
    this.canCrit = true
    this.school = 'fire'
    this.type = 'destruction'
    this.bonusCrit = 4 * player.talents.improvedSearingPain
    this.setup()
  }
}

class SoulFire extends Spell {
  constructor (player) {
    super(player)
    this.castTime = 6 - (0.4 * player.talents.bane)
    this.manaCost = 250 * (1 - 0.01 * player.talents.cataclysm)
    this.coefficient = 1.15
    this.minDmg = 1003
    this.maxDmg = 1257
    this.name = 'Soul Fire'
    this.doesDamage = true
    this.canCrit = true
    this.school = 'fire'
    this.type = 'destruction'
    this.travelTime = player.spellTravelTime
    this.setup()
  }
}

class Shadowburn extends Spell {
  constructor (player) {
    super(player)
    this.cooldown = 15
    this.manaCost = 515 * (1 - 0.01 * player.talents.cataclysm)
    this.coefficient = 0.22
    this.minDmg = 597
    this.maxDmg = 665
    this.name = 'Shadowburn'
    this.doesDamage = true
    this.canCrit = true
    this.school = 'shadow'
    this.type = 'destruction'
    this.setup()
  }
}

class DeathCoil extends Spell {
  constructor (player) {
    super(player)
    this.cooldown = 120
    this.manaCost = 600
    this.coefficient = 0.4286
    this.dmg = 526
    this.name = 'Death Coil'
    this.doesDamage = true
    this.canCrit = false // confirm
    this.school = 'shadow'
    this.type = 'affliction'
    this.travelTime = player.spellTravelTime
    this.setup()
  }
}

class Shadowfury extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Shadowfury'
    this.minDmg = 612
    this.maxDmg = 728
    this.manaCost = 710 * (1 - 0.01 * player.talents.cataclysm)
    this.castTime = 0.5
    this.doesDamage = true
    this.school = 'shadow'
    this.type = 'destruction'
    this.cooldown = 20
    this.coefficient = 0.195
    this.canCrit = true // confirm
    this.setup()
  }
}

// https://web.archive.org/web/20081023033855/http://elitistjerks.com/f47/t25902-aoe_spell_cap_mechanics/#Warlock_AoE_Spells
class SeedOfCorruption extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Seed of Corruption'
    this.minDmg = 1110
    this.maxDmg = 1290
    this.manaCost = 882
    this.castTime = 2
    this.dmgCap = 13580
    this.doesDamage = true
    this.school = 'shadow'
    this.type = 'affliction'
    this.coefficient = 0.22
    this.setup()
  }

  damage() {
    let baseDamage = this.player.simSettings.randomizeValues === 'yes' && this.minDmg && this.maxDmg ? random(this.minDmg, this.maxDmg) : this.dmg
    let enemyAmount = this.player.simSettings.enemyAmount - 1 // Minus one because the enemy that Seed is being cast on doesn't get hit
    let resistAmount = 0
    let critAmount = 0
    let spellPower = this.player.getSpellPower() + this.player.stats.shadowPower
    let modifier = this.player.stats.shadowModifier

    for (let i = 0; i < enemyAmount; i++) {
      // Check for a resist
      if (!this.player.isHit(true)) {
        resistAmount++
        this.onDamageProcs()
      }
      // Check for a crit
      else if (this.player.isCrit(this.type)) {
        critAmount++
        this.onCritProcs()
      }
    }

    let individualSeedDamage = baseDamage + (spellPower * this.coefficient)
    // Oblivion Raiment (dungeon set) 4pc bonus
    if (this.player.sets['644'] && this.player.sets['644'] >= 4) {
      individualSeedDamage += 180
    }
    individualSeedDamage *= modifier

    let enemiesHit = enemyAmount - resistAmount
    let totalSeedDamage = individualSeedDamage * enemiesHit
    // If the total damage goes above the aoe cap then we need to reduce the amount of each seed's damage
    if (totalSeedDamage > this.dmgCap) {
      // Set the damage of each individual seed to the aoe cap divided by the amount of enemies hit
      // There's a bug with Seed of Corruption where if you hit the AoE cap,
      // the number used to divide here is 1 higher because it's including the enemy that Seed is being cast on,
      // even though that enemy doesn't actually get damaged by the Seed. Nice game :)
      individualSeedDamage = this.dmgCap / (enemiesHit + 1)
      // Re-calculate the total damage done by all the seed hits
      totalSeedDamage = individualSeedDamage * enemiesHit
    }
    // Add damage from the Seed crits
    let individualSeedCrit = individualSeedDamage * this.getCritMultiplier(this.player.critMultiplier)
    let bonusDamageFromCrit = individualSeedCrit - individualSeedDamage
    totalSeedDamage += bonusDamageFromCrit * critAmount
    // Partial resists (probably need to calculate a partial resist for each seed hit, not sure how it interacts for the aoe cap)
    let partialResistMultiplier = this.player.getPartialResistMultiplier(this.player.enemy.shadowResist)
    totalSeedDamage *= partialResistMultiplier

    this.player.combatLog(this.name + " " + Math.round(totalSeedDamage) + " (" + enemyAmount + " Enemies (" + resistAmount + " Resists & " + critAmount + " Crits) - " + baseDamage + " Base Damage - " + this.coefficient + " Coefficient - " + spellPower + " Spell Power - " + Math.round(modifier * 1000) / 1000 + "% Modifier - " + partialResistMultiplier + "% Partial Resist Multiplier)")
    this.player[this.breakdownTable + 'Breakdown'][this.varName].damage = this.player[this.breakdownTable + 'Breakdown'][this.varName].damage + totalSeedDamage || totalSeedDamage
    this.player[this.breakdownTable + "Breakdown"][this.varName].crits = this.player[this.breakdownTable + "Breakdown"][this.varName].crits + critAmount || critAmount
    this.player[this.breakdownTable + "Breakdown"][this.varName].misses = this.player[this.breakdownTable + "Breakdown"][this.varName].misses + resistAmount || resistAmount
    // the cast() function already adds 1 to the amount of casts so we only need to add enemiesHit - 1 to the cast amount
    this.player[this.breakdownTable + "Breakdown"][this.varName].casts = this.player[this.breakdownTable + "Breakdown"][this.varName].casts + (enemiesHit - 1) || (enemiesHit - 1)
    this.player.iterationDamage += totalSeedDamage
  }
}

class LifeTap extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Life Tap'
    this.manaReturn = 582
    this.coefficient = 0.8
    this.modifier = 1 * (1 + 0.1 * this.player.talents.improvedLifeTap)
    this.breakdownTable = 'manaGain'
    this.setup()
  }

  ready() {
    return super.ready() && this.manaGain() + this.player.mana < this.player.stats.maxMana
  }

  manaGain () {
    return (this.manaReturn + ((this.player.getSpellPower() + this.player.stats.shadowPower) * this.coefficient)) * this.modifier
  }

  cast () {
    this.player[this.breakdownTable + 'Breakdown'][this.varName].casts = this.player[this.breakdownTable + 'Breakdown'][this.varName].casts + 1 || 1
    const manaGain = this.manaGain()
    this.player.combatLog(this.name + ' ' + Math.round(manaGain) + ' (' + Math.round(this.player.getSpellPower() + this.player.stats.shadowPower) + ' Spell Power - ' + Math.round(this.modifier * 10000) / 100 + '% Mana Gain Modifier)')
    this.player.totalManaRegenerated += manaGain
    this.player[this.breakdownTable + 'Breakdown'][this.varName].manaGain = this.player[this.breakdownTable + 'Breakdown'][this.varName].manaGain + manaGain || manaGain
    // Warning for if Life Tap is used while there are important auras active
    if (this.player.importantAuraCounter > 0) {
      this.player.combatLog('Life Tap used while there are cooldowns active')
    }
    // Warning for if the simulation ever decides to use life tap when it would overcap the player on mana.
    if (this.player.mana + manaGain > this.player.stats.maxMana) {
      this.player.combatLog('Life Tap used at too high mana (mana wasted)')
    }
    this.player.mana = Math.min(this.player.stats.maxMana, this.player.mana + manaGain)
    if (this.player.pet && this.player.talents.manaFeed > 0) {
      const petManaGain = manaGain * (this.player.talents.manaFeed / 3)
      const currentPetMana = this.player.pet.stats.mana
      this.player.pet.stats.mana = Math.min(this.player.pet.stats.maxMana, this.player.pet.stats.mana + petManaGain)
      this.player.combatLog(this.player.pet.name + ' gains ' + Math.round(this.player.pet.stats.mana - currentPetMana) + ' mana from Mana Feed (' + Math.round(currentPetMana) + ' -> ' + Math.round(this.player.pet.stats.mana) + ')')
    }
  }
}

class DarkPact extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Dark Pact'
    this.manaReturn = 700
    this.coefficient = 0.96
    this.breakdownTable = 'manaGain'
    this.setup()
  }

  manaGain() {
    return this.manaReturn + (this.player.getSpellPower() + this.player.stats.shadowPower) * this.coefficient
  }

  ready() {
    return super.ready() && this.player.pet.stats.mana >= this.manaGain() + this.player.mana
  }

  cast () {
    this.casting = false
    const manaGain = this.manaGain()
    const currentPlayerMana = this.player.mana
    const currentPetMana = this.player.pet.stats.mana
    this.player.mana = Math.min(this.player.stats.maxMana, this.player.mana + manaGain)
    const manaGained = this.player.mana - currentPlayerMana
    this.player.totalManaRegenerated += manaGained
    this.player[this.breakdownTable + 'Breakdown'][this.varName].casts = this.player[this.breakdownTable + 'Breakdown'][this.varName].casts + 1 || 1
    this.player[this.breakdownTable + 'Breakdown'][this.varName].manaGain = this.player[this.breakdownTable + 'Breakdown'][this.varName].manaGain + manaGained || manaGained
    let combatLogMsg = this.name + ' ' + Math.round(manaGained) + '. Player mana: ' + Math.round(currentPlayerMana) + ' -> ' + Math.round(this.player.mana)
    if (this.player.simSettings.infinitePetMana !== 'yes') {
      this.player.pet.stats.mana = Math.max(0, this.player.pet.stats.mana - manaGain)
      combatLogMsg += '. Pet mana: ' + Math.round(currentPetMana) + ' -> ' + Math.round(this.player.pet.stats.mana)
    }
    combatLogMsg += ' (' + this.manaReturn + ' Base Mana Return - ' + this.player.getSpellPower() + ' Spell Power - ' + this.coefficient + ' Coefficient)'
    this.player.combatLog(combatLogMsg)
    if (currentPlayerMana + manaGain > this.player.stats.maxMana) {
      console.log('Dark Pact used at too high mana (mana wasted)')
    }
  }
}

class Corruption extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Corruption'
    this.manaCost = 370
    this.castTime = Math.round((2 - (0.4 * player.talents.improvedCorruption)) * 100) / 100
    this.isDot = true
    this.school = 'shadow'
    this.type = 'affliction'
    this.setup()
  }
}

class UnstableAffliction extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Unstable Affliction'
    this.manaCost = 400
    this.castTime = 1.5
    this.isDot = true
    this.school = 'shadow'
    this.type = 'affliction'
    this.setup()
  }
}

class SiphonLife extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Siphon Life'
    this.manaCost = 410
    this.isDot = true
    this.school = 'shadow'
    this.type = 'affliction'
    this.setup()
  }
}

class Immolate extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Immolate'
    this.manaCost = 445 * (1 - 0.01 * player.talents.cataclysm)
    this.castTime = 2 - (0.1 * player.talents.bane)
    this.isDot = true
    this.doesDamage = true
    this.canCrit = true
    this.dmg = 331
    this.coefficient = 0.2
    this.school = 'fire'
    this.type = 'destruction'
    this.setup()
  }

  getModifier () {
    let modifier = super.getModifier()
    if (this.player.talents.emberstorm > 0) {
      modifier /= (1 + 0.02 * this.player.talents.emberstorm)
    }
    modifier *= (1 + (0.02 * this.player.talents.emberstorm + 0.05 * this.player.talents.improvedImmolate))
    return modifier
  }
}

class CurseOfAgony extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Curse of Agony'
    this.manaCost = 265
    this.isDot = true
    this.school = 'shadow'
    this.type = 'affliction'
    this.setup()
  }
}

class CurseOfTheElements extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Curse of the Elements'
    this.manaCost = 260
    this.type = 'affliction'
    this.isAura = true
    this.setup()
  }
}

class CurseOfRecklessness extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Curse of Recklessness'
    this.manaCost = 160
    this.type = 'affliction'
    this.isAura = true
    this.setup()
  }
}

class CurseOfDoom extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Curse of Doom'
    this.manaCost = 380
    this.cooldown = 60
    this.school = 'shadow'
    this.type = 'affliction'
    this.isDot = true
    this.setup()
  }
}

class Conflagrate extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Conflagrate'
    this.manaCost = 305 * (1 - 0.01 * player.talents.cataclysm)
    this.cooldown = 10
    this.minDmg = 579
    this.maxDmg = 721
    this.coefficient = 1.5/3.5
    this.doesDamage = true
    this.canCrit = true
    this.school = 'fire'
    this.type = 'destruction'
    this.setup()
  }

  startCast() {
    if (this.player.auras.immolate && this.player.auras.immolate.active) {
      super.startCast()
      this.player.auras.immolate.active = false
    }
  }
}

class DestructionPotion extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Destruction Potion'
    this.cooldown = 120
    this.isItem = true
    this.isAura = true
    this.onGcd = false
    this.setup()
  }
}

class SuperManaPotion extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Super Mana Potion'
    this.cooldown = 120
    this.isItem = true
    this.minMana = 1800
    this.maxMana = 3000
    this.onGcd = false
    this.breakdownTable = 'manaGain'
    this.setup()
  }

  cast () {
    super.cast()
    const currentPlayerMana = this.player.mana
    const manaGain = this.player.simSettings.randomizeValues === 'yes' ? random(this.minMana, this.maxMana) : this.avgManaValue
    this.player.totalManaRegenerated += manaGain
    this.player[this.breakdownTable + 'Breakdown'][this.varName].manaGain = this.player[this.breakdownTable + 'Breakdown'][this.varName].manaGain + manaGain || manaGain
    this.player.mana = Math.min(this.player.stats.maxMana, currentPlayerMana + manaGain)
    this.player.combatLog('Player gains ' + Math.round(this.player.mana - currentPlayerMana) + ' mana from Super Mana Potion (' + Math.round(currentPlayerMana) + ' -> ' + Math.round(this.player.mana) + ')')
  }
}

class DemonicRune extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Demonic Rune'
    this.cooldown = 120
    this.isItem = true
    this.minMana = 900
    this.maxMana = 1500
    this.onGcd = false
    this.breakdownTable = 'manaGain'
    this.setup()
  }

  cast () {
    super.cast()
    const currentPlayerMana = this.player.mana
    const manaGain = this.player.simSettings.randomizeValues === 'yes' ? random(this.minMana, this.maxMana) : this.avgManaValue
    this.player.totalManaRegenerated += manaGain
    this.player[this.breakdownTable + 'Breakdown'][this.varName].manaGain = this.player[this.breakdownTable + 'Breakdown'][this.varName].manaGain + manaGain || manaGain
    this.player.mana = Math.min(this.player.stats.maxMana, currentPlayerMana + manaGain)
    this.player.combatLog('Player gains ' + Math.round(this.player.mana - currentPlayerMana) + ' mana from Demonic Rune (' + Math.round(currentPlayerMana) + ' -> ' + Math.round(this.player.mana) + ')')
  }
}

class FlameCap extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Flame Cap'
    this.cooldown = 180
    this.isItem = true
    this.isAura = true
    this.onGcd = false
    this.setup()
  }
}

class BloodFury extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Blood Fury'
    this.cooldown = 120
    this.isAura = true
    this.onGcd = false
    this.setup()
  }
}

class Bloodlust extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Bloodlust'
    this.cooldown = 600
	  this.isItem = true
    this.isAura = true
    this.onGcd = false
    this.setup()
  }
}

class DrumsOfBattle extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Drums of Battle'
    this.cooldown = 120
    this.isAura = true
    this.onGcd = false
    this.isNonWarlockAbility = true
    this.isItem = true
    this.setup()
  }

  ready () {
    return this.cooldownRemaining <= 0
  }
}

class DrumsOfWar extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Drums of War'
    this.cooldown = 120
    this.isAura = true
    this.onGcd = false
    this.isNonWarlockAbility = true
    this.isItem = true
    this.setup()
  }

  ready () {
    return this.cooldownRemaining <= 0
  }
}

class DrumsOfRestoration extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Drums of Restoration'
    this.cooldown = 120
    this.isAura = true
    this.onGcd = false
    this.isNonWarlockAbility = true
    this.isItem = true
    this.setup()
  }

  ready () {
    return this.cooldownRemaining <= 0
  }
}

class TimbalsFocusingCrystal extends Spell {
  constructor (player) {
    super(player)
    this.name = "Timbal's Focusing Crystal"
    this.cooldown = 15
    this.onGcd = false
    this.procChance = 10
    this.minDmg = 285
    this.maxDmg = 475
    this.doesDamage = true
    this.coefficient = 0
    this.school = 'shadow'
    this.canCrit = true
    this.setup()
  }
}

class MarkOfDefiance extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Mark of Defiance'
    this.cooldown = 17
    this.procChance = 15
    this.onGcd = false
    this.minMana = 128
    this.maxMana = 172
    this.breakdownTable = 'manaGain'
    this.setup()
  }

  cast () {
    if (this.cooldownRemaining <= 0) {
      this.player[this.breakdownTable + 'Breakdown'][this.varName].casts = this.player[this.breakdownTable + 'Breakdown'][this.varName].casts + 1 || 1
      this.player[this.breakdownTable + 'Breakdown'][this.varName].manaGain = this.player[this.breakdownTable + 'Breakdown'][this.varName].manaGain + this.avgManaValue || this.avgManaValue
      this.player.totalManaRegenerated += this.avgManaValue
      this.player.combatLog(this.name + ' +' + this.avgManaValue + ' mana')
      this.player.mana = Math.min(this.player.stats.maxMana, this.player.mana + this.avgManaValue)
      this.cooldownRemaining = this.cooldown
    }
  }
}

class TheLightningCapacitor extends Spell {
  constructor (player) {
    super(player)
    this.name = 'The Lightning Capacitor'
    this.cooldown = 2.5
    this.onGcd = false
    this.minDmg = 694
    this.maxDmg = 806
    this.doesDamage = true
    this.coefficient = 0
    this.canCrit = true
    this.school = 'nature' // confirm
    this.setup()
  }

  startCast () {
    if (this.cooldownRemaining <= 0) {
      this.player.auras.theLightningCapacitor.apply()
      if (this.player.auras.theLightningCapacitor.stacks == 3) {
        this.cooldownRemaining = this.cooldown
        this.player.auras.theLightningCapacitor.fade()
        super.startCast()
      }
    }
  }
}

class BladeOfWizardry extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Blade of Wizardry'
    this.cooldown = 50
    this.onGcd = false
    this.isItem = true
    this.isProc = true
    this.isAura = true
    this.breakdownTable = 'aura'
    this.setup()
  }
}

class ShatteredSunPendantOfAcumen extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Shattered Sun Pendant of Acumen'
    this.cooldown = 45
    this.procChance = 15
    this.onGcd = false
    this.isItem = true
    if (this.player.shattrathFaction == 'Aldor') {
      this.isProc = true
      this.isAura = true
      this.breakdownTable = 'aura'
    } else if (this.player.shattrathFaction == 'Scryers') {
      this.doesDamage = true
      this.canCrit = true
      this.coefficient = 0
      this.dmg = 333 // confirm
      this.school = 'arcane'
    }
    this.setup()
  }
}

class RobeOfTheElderScribes extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Robe of the Elder Scribes'
    this.cooldown = 50
    this.procChance = 20
    this.onGcd = false
    this.isItem = true
    this.isProc = true
    this.isAura = true
    this.breakdownTable = 'aura'
    this.setup()
  }
}

class QuagmirransEye extends Spell {
  constructor (player) {
    super(player)
    this.name = "Quagmirran's Eye"
    this.cooldown = 45
    this.procChance = 10
    this.onGcd = false
    this.isItem = true
    this.isAura = true
    this.breakdownTable = 'aura'
    this.setup()
  }
}

class ShiffarsNexusHorn extends Spell {
  constructor (player) {
    super(player)
    this.name = "Shiffar's Nexus-Horn"
    this.cooldown = 45
    this.procChance = 20
    this.onGcd = false
    this.isItem = true
    this.isAura = true
    this.breakdownTable = 'aura'
    this.setup()
  }
}

class SextantOfUnstableCurrents extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Sextant of Unstable Currents'
    this.cooldown = 45
    this.procChance = 20
    this.onGcd = false
    this.isItem = true
    this.isAura = true
    this.breakdownTable = 'aura'
    this.setup()
  }
}

class BandOfTheEternalSage extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Band of the Eternal Sage'
    this.cooldown = 60
    this.procChance = 10
    this.onGcd = false
    this.isItem = true
    this.isAura = true
    this.breakdownTable = 'aura'
    this.setup()
  }
}

class MysticalSkyfireDiamond extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Mystical Skyfire Diamond'
    this.cooldown = 35
    this.procChance = 15
    this.onGcd = false
    this.isProc = true
    this.isItem = true
    this.isAura = true
    this.breakdownTable = 'aura'
    this.setup()
  }
}

class InsightfulEarthstormDiamond extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Insightful Earthstorm Diamond'
    this.cooldown = 15
    this.procChance = 5
    this.onGcd = false
    this.isProc = true
    this.isItem = true
    this.breakdownTable = 'manaGain'
    this.manaGain = 300
    this.setup()
  }

  cast() {
    super.cast()
    const currentPlayerMana = this.player.mana
    this.player.totalManaRegenerated += this.manaGain
    this.player[this.breakdownTable + 'Breakdown'][this.varName].manaGain = this.player[this.breakdownTable + 'Breakdown'][this.varName].manaGain + this.manaGain || this.manaGain
    this.player.mana = Math.min(this.player.stats.maxMana, currentPlayerMana + this.manaGain)
    this.player.combatLog('Player gains ' + Math.round(this.player.mana - currentPlayerMana) + ' mana from ' + this.name + ' (' + Math.round(currentPlayerMana) + ' -> ' + Math.round(this.player.mana) + ')')
  }
}

class AmplifyCurse extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Amplify Curse'
    this.cooldown = 180
    this.isAura = true
    this.onGcd = false
    this.setup()
  }
}

class PowerInfusion extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Power Infusion'
    this.cooldown = 180
    this.isAura = true
    this.isNonWarlockAbility = true
    this.setup()
  }
}

class Innervate extends Spell {
  constructor (player) {
    super(player)
    this.name = 'Innervate'
    this.cooldown = 360
    this.isAura = true
    this.isNonWarlockAbility = true
    this.setup()
  }
}