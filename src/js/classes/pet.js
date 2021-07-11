const PetName = Object.freeze({
  IMP: 0,
  VOIDWALKER: 1,
  SUCCUBUS: 2,
  FELHUNTER: 3,
  FELGUARD: 4
})

const PetType = Object.freeze({
  MELEE: 0,
  RANGED: 1
})

const PetMode = Object.freeze({
  PASSIVE: 0,
  AGGRESSIVE: 1
})

class Pet {
  constructor (player, settings) {
    this.player = player
    this.simSettings = settings.simSettings
    this.playerAuras = settings.auras
    this.enemyDodgeChance = 5
    this.castTimeRemaining = 0
    this.fiveSecondRuleTimerRemaining = 5
    this.spiritTickTimerRemaining = 2
    this.mode = this.simSettings.petMode
    this.critMultiplier = 1.5
    this.glancingBlowMultiplier = 1 - (0.1 + (player.enemy.level * 5 - player.level * 5) / 100)
    this.glancingBlowChance = 10 + player.enemy.level * 5 - player.level * 5
    this.spells = {}
    this.auras = {}
    this.stats = {
      baseStats: {
        stamina: 0,
        intellect: 0,
        strength: 0,
        agility: 0,
        mana: 0,
        ap: 0
      },
      // certain stats from buffs need to be separated from the main stat so we can re-calculate the pet's stats in calculateStatsFromPlayer().
      buffs: {
        stamina: 0,
        intellect: 0,
        strength: 0,
        agility: 0,
        spellPower: 0,
        ap: 0,
        critChance: 0,
        spellCritChance: 0
      },
      mp5: 0,
      ap: 0,
      strength: 0,
      agility: 0,
      spirit: 0,
      stamina: 0,
      intellect: 0,
      hasteRating: 0,
      staminaModifier: 1 + (0.05 * player.talents.felStamina),
      intellectModifier: 1 + (0.05 * player.talents.felIntellect),
      strengthModifier: 1,
      agilityModifier: 1,
      spiritModifier: 1,
      apModifier: 1,
      damageModifier: 1 * (1 + (0.04 * player.talents.unholyPower)) * (1 + (0.05 * player.talents.soulLink)),
      currentMana: 0
    }
  }

  initialize () {
    if (this.type == PetType.MELEE) this.spells.melee = new Melee(this)
    if (this.player.simSettings.prepopBlackBook === 'yes') this.auras.blackBook = new BlackBook(this)
  }

  // Calculates stats from buffs/debuffs/pet buffs
  calculateStatsFromAuras () {
    // Calculate melee hit chance
    // Formula from https://wowwiki-archive.fandom.com/wiki/Hit?oldid=1584399
    if (this.player.enemy.level - this.player.level <= 2) {
      this.stats.hitChance = 100 - (5 + (this.player.enemy.level - this.player.level) * 0.5)
    } else if (this.player.enemy.level - this.player.level > 2) {
      this.stats.hitChance = 100 - (7 + (this.player.enemy.level - this.player.level - 2) * 2)
    }

    // Calculate spell hit chance
    // Formula from https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/
    // target 2 or fewer levels above
    if ((parseInt(this.player.enemy.level) - this.player.level) <= 2) {
      this.stats.spellHitChance = Math.min(99, 100 - (parseInt(this.player.enemy.level) - this.player.level) - 4)
    }
    // target 3 levels above
    else if ((parseInt(this.player.enemy.level) - this.player.level) == 3) {
      this.stats.spellHitChance = 83
    }
    // target 4 levels above
    else if ((parseInt(this.player.enemy.level) - this.player.level) == 4) {
      this.stats.spellHitChance = 72
    }
    // target 5+ levels above
    else {
      this.stats.spellHitChance = 61
    }

    // Configure stats based on selected auras
    if (this.playerAuras.blessingOfKings) {
      this.stats.staminaModifier *= 1.1
      this.stats.intellectModifier *= 1.1
      this.stats.agilityModifier *= 1.1
      this.stats.strengthModifier *= 1.1
      this.stats.spiritModifier *= 1.1
    }
    if (this.playerAuras.blessingOfWisdom) this.stats.mp5 += 41
    if (this.playerAuras.manaSpringTotem) this.stats.mp5 += 50
    if (this.playerAuras.wrathOfAirTotem) this.stats.buffs.spellPower += 101
    if (this.playerAuras.totemOfWrath) {
      const hitAndCritAmount = 3 * parseInt(this.simSettings.totemOfWrathAmount)
      this.stats.buffs.spellCritChance += hitAndCritAmount
      this.stats.spellHitChance = Math.min(99, this.stats.spellHitChance + hitAndCritAmount)
    }
    // todo implement improved motw
    if (this.playerAuras.markOfTheWild) {
      this.stats.buffsstamina += 14
      this.stats.buffs.intellect += 14
      this.stats.spirit += 14
    }
    // todo implement improved imp
    if (this.playerAuras.bloodPact) this.stats.buffs.stamina += 70
    if (this.playerAuras.arcaneIntellect) this.stats.buffs.intellect += 40
    if (this.playerAuras.prayerOfFortitude) this.stats.buffs.stamina += 79
    if (this.playerAuras.prayerOfSpirit) {
      const multiplier = parseInt(this.simSettings.improvedDivineSpirit)
      this.stats.spirit += 50 * (1 + (0.1 * multiplier))
    }
    if (this.playerAuras.inspiringPresence) this.stats.spellHitChance = Math.min(99, this.stats.spellHitChance + 1)
    if (this.playerAuras.moonkinAura) this.stats.buffs.spellCritChance += 5
    // todo add atiesh auras
    if (this.playerAuras.judgementOfTheCrusader) {
      this.stats.buffs.critChance += 3
      this.stats.buffs.spellCritChance += 3
    }
    if (this.playerAuras.vampiricTouch) this.stats.mp5 += this.simSettings.shadowPriestDps * 0.05
    // Improved Faerie Fire
    if (this.playerAuras.faerieFire && this.simSettings.improvedFaerieFire == 'yes') this.stats.hitChance += 3
    if (this.playerAuras.exposeWeakness) this.stats.buffs.ap += (this.simSettings.survivalHunterAgility * 0.25 * (this.simSettings.exposeWeaknessUptime / 100)) || 0
    if (this.playerAuras.heroicPresence) this.stats.hitChance++
    if (this.playerAuras.blessingOfMight) this.stats.buffs.ap += 220
    if (this.playerAuras.strengthOfEarthTotem) this.stats.buffs.strength += 86
    if (this.playerAuras.graceOfAirTotem) this.stats.buffs.agility += 67
    if (this.playerAuras.battleShout) this.stats.buffs.ap += 306
    if (this.playerAuras.trueshotAura) this.stats.buffs.ap += 300
    if (this.playerAuras.leaderOfThePack) this.stats.buffs.critChance += 5
    if (this.playerAuras.unleashedRage) this.stats.apModifier *= 1.1
    if (this.playerAuras.scrollOfStaminaV) this.stats.buffs.stamina += 20
    if (this.playerAuras.scrollOfIntellectV) this.stats.buffs.intellect += 20
    if (this.playerAuras.scrollOfStrengthV) this.stats.buffs.strength += 20
    if (this.playerAuras.scrollOfAgilityV) this.stats.buffs.agility += 20
    if (this.playerAuras.scrollOfSpiritV) this.stats.spirit += 20
    if (this.playerAuras.kiblersBits) {
      this.stats.buffs.strength += 20
      this.stats.spirit += 20
    }
    if (this.player.simSettings.race == 'orc') this.stats.damageModifier *= 1.05
    
    // Hidden ap modifiers (source: Max on warlock discord)
    if (this.pet == PetName.FELGUARD) {
      this.stats.apModifier *= 1.1
    } else if (this.pet == PetName.SUCCUBUS) {
      this.stats.apModifier *= 1.05
    }

    // Calculate armor
    if (this.type == PetType.MELEE) {
      // Formula from https://wowwiki-archive.fandom.com/wiki/Damage_reduction?oldid=807810
      if (this.player.enemy.level <= 59) {
        this.armorMultiplier = 1 - this.player.enemy.armor / (this.player.enemy.armor + 400 + 85 * this.player.enemy.level)
      } else {
        this.armorMultiplier = 1 - this.player.enemy.armor / (this.player.enemy.armor - 22167.5 + 467.5 * this.player.enemy.level)
      }
    }
    this.armorMultiplier = Math.max(0.25, this.armorMultiplier)
  }

  // Player -> Pet stat scaling info taken from https://wowwiki-archive.fandom.com/wiki/Warlock?oldid=1618728 and from Max's research on the warlock discord
  calculateStatsFromPlayer (announceInCombatlog = true) {
    this.stats.stamina = this.stats.baseStats.stamina + this.stats.buffs.stamina + (0.3 * this.player.stats.stamina * this.player.stats.staminaModifier)
    this.stats.intellect = this.stats.baseStats.intellect + this.stats.buffs.intellect + (0.3 * this.player.stats.intellect * this.player.stats.intellectModifier)
    this.player.demonicKnowledgeSp = ((this.stats.stamina * this.stats.staminaModifier) + (this.stats.intellect * this.stats.intellectModifier)) * (0.04 * this.player.talents.demonicKnowledge)
    this.stats.baseStats.ap = (this.stats.baseStats.strength + this.stats.buffs.strength) * 2 - 20
    this.stats.ap = this.stats.baseStats.ap + this.stats.buffs.ap + (this.player.getSpellPower() + Math.max(this.player.stats.shadowPower, this.player.stats.firePower)) * 0.57
    this.stats.agility = this.stats.baseStats.agility + this.stats.buffs.agility
    this.stats.critChance = this.player.talents.demonicTactics + (this.stats.agility * this.stats.agilityModifier) * 0.04 + 0.65 + this.stats.buffs.critChance
    this.stats.spellPower = this.stats.buffs.spellPower + (this.player.getSpellPower() + Math.max(this.player.stats.shadowPower, this.player.stats.firePower)) * 0.15
    if (this.type == PetType.MELEE) {
      // Halp, need confirmation that this is actually the right way to get its average melee damage.
      this.dmg = (this.getAttackPower() / 14 + 51.7) * this.baseMeleeSpeed
      this.stats.maxMana = this.stats.baseStats.mana + this.stats.intellect * 11.555 * this.stats.intellectModifier
    } else if (this.type == PetType.RANGED) {
      this.stats.maxMana = this.stats.baseStats.mana + this.stats.intellect * this.stats.intellectModifier * 4.95
      this.stats.spellCritChance = 0.0125 * (this.stats.intellect * this.stats.intellectModifier) + 0.91 + this.player.talents.demonicTactics + this.stats.buffs.spellCritChance
    }
    if (announceInCombatlog) {
      this.player.combatLog("Recalculated pet's stats")
    }
  }

  setup () {
    this.initialize()
    this.calculateStatsFromAuras()
    this.calculateStatsFromPlayer()
  }

  cast (spellName) {
    this.spells[spellName].startCast()
  }

  reset () {
    this.stats.mana = this.stats.maxMana
    this.fiveSecondRuleTimerRemaining = 5 // If higher than 0 then the pet can't gain mana from Spirit regen (July 2021 update: I have no idea what this comment means)
    this.spiritTickTimerRemaining = 2
    if (this.type == PetType.MELEE) this.spells.melee.cooldownRemaining = 0
    if (this.auras.blackBook && this.auras.blackBook.active) this.auras.blackBook.fade(true)
    // Black Book prepop
    if (this.player.simSettings.prepopBlackBook === 'yes') {
      this.auras.blackBook.apply(false)
    }
    this.calculateStatsFromPlayer(false)
  }

  getMeleeCritChance () {
    return this.stats.critChance
  }

  getSpellCritChance () {
    return this.stats.spellCritChance
  }

  isCrit (spellType) {
    if (spellType == SpellTypes.PHYSICAL) {
      return random(1, 100 * this.player.stats.critChanceMultiplier) <= (this.getMeleeCritChance() * this.player.stats.critChanceMultiplier)
    } else if (spellType == SpellTypes.MAGICAL) {
      return random(1, 100 * this.player.stats.critChanceMultiplier) <= (this.getSpellCritChance() * this.player.stats.critChanceMultiplier)
    }
  }

  getMeleeHitChance () {
    return this.stats.hitChance
  }

  getSpellHitChance () {
    return this.stats.spellHitChance
  }

  isHit (spellType) {
    if (spellType == SpellTypes.PHYSICAL) {
      return random(1, 100 * this.player.stats.hitChanceMultiplier) <= (this.stats.hitChance * this.player.stats.hitChanceMultiplier)
    } else if (spellType == SpellTypes.MAGICAL) {
      return random(1, 100 * this.player.stats.hitChanceMultiplier) <= (Math.min(99 * this.player.stats.hitChanceMultiplier, this.stats.spellHitChance * this.player.stats.hitChanceMultiplier))
    }
  }

  getAttackPower () {
    return this.stats.ap * this.stats.apModifier
  }

  tick (t) {
    if (this.spells.melee) this.spells.melee.tick(t)
    if (this.auras.blackBook && this.auras.blackBook.active) this.auras.blackBook.tick(t)

    this.castTimeRemaining = Math.max(0, this.castTimeRemaining - t)
    this.fiveSecondRuleTimerRemaining = Math.max(0, this.fiveSecondRuleTimerRemaining - t)
    this.spiritTickTimerRemaining = Math.max(0, this.spiritTickTimerRemaining - t)
    // Mp5 & Spirit mana regen
    if (this.spiritTickTimerRemaining <= 0) {
      this.spiritTickTimerRemaining = 5
      // Formulas from Max on the warlock discord https://discord.com/channels/253210018697052162/823476479550816266/836007015762886707 & https://discord.com/channels/253210018697052162/823476479550816266/839484387741138994
      let manaGain = this.stats.mp5
      // Mana regen from spirit
      if (this.fiveSecondRuleTimerRemaining <= 0) {
        if (this.pet == PetName.IMP) {
          manaGain += ((this.stats.baseStats.spirit + this.stats.spirit) * this.stats.spiritModifier) + 0.7 * (this.stats.intellect * this.stats.intellectModifier) - 258
        } else if (this.pet == PetName.FELGUARD || this.pet == PetName.SUCCUBUS) {
          manaGain += 0.75 * ((this.stats.baseStats.spirit + this.stats.spirit) * this.stats.spiritModifier) + 0.62 * (this.stats.intellect * this.stats.intellectModifier) - 108
        }
      }
      // Mana regen while the 5 second spirit regen timer is active (no bonus from spirit)
      else {
        if (this.pet == PetName.IMP) {
          manaGain += 0.375 * (this.stats.intellect * this.stats.intellectModifier) - 123
        } else if (this.pet == PetName.FELGUARD || this.pet == PetName.SUCCUBUS) {
          manaGain += 0.365 * (this.stats.intellect * this.stats.intellectModifier) - 48
        }
      }
      const currentMana = this.stats.mana
      this.stats.mana = Math.min(this.stats.maxMana, this.stats.mana + manaGain)
      if (this.stats.mana > currentMana) {
        this.player.combatLog(this.name + ' gains ' + Math.round(manaGain) + ' mana from Mp5/Spirit regeneration (' + Math.round(currentMana) + ' -> ' + Math.round(this.stats.mana) + ')')
      }
    }
  }
}

class Imp extends Pet {
  constructor (player, settings) {
    super(player, settings)
    this.name = 'Imp'
    this.type = PetType.RANGED
    this.stats.baseStats.stamina = 101
    this.stats.baseStats.intellect = 327
    this.stats.baseStats.mana = 756
    this.stats.baseStats.spirit = 263
    this.stats.baseStats.strength = 145
    this.stats.baseStats.agility = 38
    this.pet = PetName.IMP
    this.setup()
  }

  initialize () {
    this.spells.firebolt = new ImpFirebolt(this)
    super.initialize()
  }

  tick (t) {
    super.tick(t)
    this.spells.firebolt.tick(t)
  }
}

class Voidwalker extends Pet {
  constructor (player, settings) {
    super(player, settings)
    this.name = 'Voidwalker'
    this.type = PetType.MELEE
    this.pet = PetName.VOIDWALKER
  }
}

class Succubus extends Pet {
  constructor (player, settings) {
    super(player, settings)
    this.name = 'Succubus'
    this.pet = PetName.SUCCUBUS
    this.type = PetType.MELEE
    this.stats.baseStats.stamina = 280
    this.stats.baseStats.intellect = 133
    this.stats.baseStats.mana = 849
    this.stats.baseStats.spirit = 122
    this.stats.baseStats.strength = 153
    this.stats.baseStats.agility = 109
    this.baseMeleeSpeed = 2
    this.healthPerStamina = 7
    this.stats.damageModifier *= 1 + (0.02 * player.talents.masterDemonologist)
    this.setup()
  }

  reset () {
    this.spells.lashOfPain.cooldownRemaining = 0
    super.reset()
  }

  initialize () {
    this.spells.lashOfPain = new SuccubusLashOfPain(this)
    super.initialize()
  }

  tick (t) {
    super.tick(t)
    this.spells.lashOfPain.tick(t)
  }
}

class Felhunter extends Pet {
  constructor (player, settings) {
    super(player, settings)
    this.name = 'Felhunter'
    this.type = PetType.MELEE
    this.pet = PetName.FELHUNTER
  }
}

class Felguard extends Pet {
  constructor (player, settings) {
    super(player, settings)
    this.name = 'Felguard'
    this.type = PetType.MELEE
    this.pet = PetName.FELGUARD
    this.stats.baseStats.health = 440
    this.stats.baseStats.stamina = 280
    this.stats.baseStats.strength = 153
    this.stats.baseStats.agility = 108
    this.stats.baseStats.intellect = 133
    this.stats.baseStats.spirit = 122
    this.stats.baseStats.mana = 893
    this.baseMeleeSpeed = 2
    this.stats.damageModifier *= 1 + (0.01 * player.talents.masterDemonologist)
    this.player.damageBreakdown.melee = { name: 'Melee (Felguard)' }
    this.setup()
  }

  getAttackPower () {
    return super.getAttackPower() * (1 + 0.05 * this.auras.demonicFrenzy.stacks)
  }

  reset () {
    this.spells.cleave.cooldownRemaining = 0
    this.auras.demonicFrenzy.fade(true)
    super.reset()
  }

  initialize () {
    this.spells.cleave = new FelguardCleave(this)
    this.auras.demonicFrenzy = new DemonicFrenzy(this)
    super.initialize()
  }

  tick (t) {
    super.tick(t)
    this.spells.cleave.tick(t)
  }
}
