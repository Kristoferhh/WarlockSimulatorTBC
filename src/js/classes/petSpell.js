const SpellTypes = Object.freeze({
  MAGICAL: 0,
  PHYSICAL: 1
})

class PetSpell {
  constructor (pet) {
    this.pet = pet
    this.casting = false
    this.canCrit = true
    this.cooldownRemaining = 0
    this.castTime = 0
    this.manaCost = 0
    this.modifier = 1
    this.school = null
    this.cooldown = 0
    this.resetsFiveSecondRule = true // true if it's a spell that resets the five second rule for mana regen from spirit (true for everything but melee attacks)
  }

  ready () {
    return this.cooldownRemaining <= 0 && this.pet.stats.mana >= this.manaCost && this.pet.castTimeRemaining <= 0
  }

  getBaseDamage() {
    return this.dmg
  }

  getCastTime() {
    // todo: figure out how much haste % pets get from each haste rating point
    return Math.round((this.castTime / (1 + this.pet.stats.hastePercent / 100)) * 10000) / 10000
  }

  getCooldown() {
    return this.cooldown
  }

  tick (t) {
    if (this.casting && this.pet.castTimeRemaining <= 0) {
      this.casting = false
      this.cast()
    }
    if (this.cooldownRemaining > 0) {
      this.cooldownRemaining = Math.max(0, this.cooldownRemaining - t)
    }
  }

  startCast () {
    if (this.castTime > 0) {
      this.casting = true
      this.pet.castTimeRemaining = this.getCastTime()
      this.pet.player.combatLog(this.pet.name + ' started casting ' + this.name + '. Cast time: ' + this.pet.castTimeRemaining + ' (' + Math.round((this.pet.stats.hastePercent) * 10000) / 10000 + '% haste at a base cast speed of ' + this.castTime + ')')
    } else {
      this.cast()
    }
  }

  cast () {
    if (this.resetsFiveSecondRule) {
      this.pet.fiveSecondRuleTimerRemaining = 5
    }
    this.cooldownRemaining = this.getCooldown()
    // Combat log entry about the cast
    let combatLogEntry = this.pet.name
    if (this.castTime > 0) {
      combatLogEntry += ' finished casting ' + this.name
    } else {
      combatLogEntry += ' casts ' + this.name
      if (this.varName === 'melee') {
        combatLogEntry += '. Attack Speed: ' + this.pet.spells.melee.getCooldown().toFixed(2) + ' (' + Math.round(this.pet.stats.hastePercent * 10000) / 10000 + '% haste at a base attack speed of ' + this.pet.spells.melee.cooldown.toFixed(2) + ')'
      }
    }
    if (this.manaCost > 0 && this.pet.player.simSettings.infinitePetMana !== 'yes') {
      this.pet.stats.mana = Math.max(0, this.pet.stats.mana - this.manaCost)
      combatLogEntry += '. Pet mana: ' + Math.round(this.pet.stats.mana) + '/' + Math.round(this.pet.stats.maxMana)
    }
    this.pet.player.combatLog(combatLogEntry)
    this.pet.player.damageBreakdown[this.varName].casts = this.pet.player.damageBreakdown[this.varName].casts + 1 || 1

    // Physical dmg spell
    if (this.type == SpellTypes.PHYSICAL) {
      let isCrit, isGlancing = false
      let critChance = this.pet.getMeleeCritChance() * this.pet.player.stats.hitChanceMultiplier
      let dodgeChance = critChance + this.pet.enemyDodgeChance * this.pet.player.stats.hitChanceMultiplier
      let missChance = dodgeChance + (100 - this.pet.getMeleeHitChance()) * this.pet.player.stats.hitChanceMultiplier
      let glancingChance = missChance
      // Only check for a glancing blow if it's a normal melee attack
      if (this.varName === 'melee') {
        glancingChance += this.pet.glancingBlowChance * this.pet.player.stats.hitChanceMultiplier
      }

      // Check whether the attack is a crit, dodge, miss, glancing, or just a normal hit.
      const attackRoll = random(1,100 * this.pet.player.stats.hitChanceMultiplier)
      // Crit
      if (attackRoll <= critChance) {
        this.pet.player.damageBreakdown[this.varName].crits = this.pet.player.damageBreakdown[this.varName].crits + 1 || 1
        isCrit = true
      }
      // Dodge
      else if (attackRoll <= dodgeChance) {
        this.pet.player.damageBreakdown[this.varName].dodges = this.pet.player.damageBreakdown[this.varName].dodges + 1 || 1
        this.pet.player.combatLog(this.pet.name + ' ' + this.name + ' *dodge*')
        return
      }
      // Miss
      else if (attackRoll <= missChance) {
        this.pet.player.damageBreakdown[this.varName].misses = this.pet.player.damageBreakdown[this.varName].misses + 1 || 1
        this.pet.player.combatLog(this.pet.name + ' ' + this.name + ' *miss*')
        return
      }
      // Glancing Blow
      else if (attackRoll <= glancingChance && this.varName === 'melee') {
        this.pet.player.damageBreakdown[this.varName].glancingBlows = this.pet.player.damageBreakdown[this.varName].glancingBlows + 1 || 1
        isGlancing = true
      }

      this.damage(isCrit, isGlancing)
    }
    // Magic dmg spell
    else if (this.type == SpellTypes.MAGICAL) {
      // Check for resist
      if (!this.pet.isHit(this.type)) {
        this.pet.player.damageBreakdown[this.varName].misses = this.pet.player.damageBreakdown[this.varName].misses + 1 || 1
        this.pet.player.combatLog(this.pet.name + ' ' + this.name + ' *resist*')
      } else {
        // Check for crit
        let isCrit = false
        if (this.canCrit && this.pet.isCrit(this.type)) {
          isCrit = true
          this.pet.player.damageBreakdown[this.varName].crits = this.pet.player.damageBreakdown[this.varName].crits + 1 || 1
        }
        this.damage(isCrit, false)
      }
    }
  }

  damage (isCrit, isGlancing) {
    const baseDamage = this.getBaseDamage()
    let dmg = baseDamage
    let modifier = this.modifier
    
    // Add damage from Spell Power
    if (this.type == SpellTypes.MAGICAL) {
      dmg += (this.pet.stats.spellPower * this.coefficient)
    }
    // Multiply if it's a crit
    if (isCrit) {
      dmg *= this.pet.critMultiplier
    }
    // Magic damage multipliers
    if (this.type == SpellTypes.MAGICAL) {
      // Curse of the Elements
      if (this.pet.playerAuras.curseOfTheElements && ['shadow','fire','frost','arcane'].includes(this.school)) {
        modifier *= (1.1 + 0.01 * this.pet.simSettings.improvedCurseOfTheElements)
      }
      // Misery
      if (this.pet.playerAuras.misery) {
        modifier *= 1.05
      }
      // Shadow damage multipliers
      if (this.school == 'shadow') {
        // Shadow Weaving
        if (this.pet.playerAuras.shadowWeaving) {
          modifier *= 1.1 
        }
        // ISB
        if ((this.pet.player.auras.improvedShadowBolt && this.pet.player.auras.improvedShadowBolt.active) || this.pet.player.simSettings.customIsbUptime == 'yes') {
          // Custom ISB Uptime
          if (this.pet.player.simSettings.customIsbUptime == 'yes') {
            modifier *= (1 + 0.2 * (this.pet.player.simSettings.customIsbUptimeValue / 100))
          } 
          // Normal ISB
          else {
            modifier *= this.pet.player.auras.improvedShadowBolt.modifier
            this.pet.player.auras.improvedShadowBolt.decrementStacks()
          }
        }
      }
      // Fire damage multipliers
      else if (this.school == 'fire') {
        // Improved Scorch
        if (this.pet.playerAuras.improvedScorch) {
          modifier *= 1.15
        }
      }
    }
    // Physical damage multipliers
    else if (this.type == SpellTypes.PHYSICAL) {
      // Add damage from the Blood Frenzy debuff
      if (this.pet.playerAuras.bloodFrenzy) {
        modifier *= 1.04
      }
      // Armor damage reduction
      dmg *= this.pet.armorMultiplier
    }
    // Pet damage modifier (from Unholy Power, Master Demonologist, and such)
    modifier *= this.pet.stats.damageModifier

    if (isGlancing) {
      dmg *= this.pet.glancingBlowMultiplier
    }
    
    dmg *= modifier

    // Partial resist reduction
    const partialResistMultiplier = this.pet.player.getPartialResistMultiplier(this.pet.player.enemy[this.school + 'Resist'])
    if (this.type == SpellTypes.MAGICAL) {
      dmg *= partialResistMultiplier
    }

    // Combat log message
    dmg = Math.round(dmg)
    let combatLogMsg = this.pet.name + ' ' + this.name + ' '
    if (isCrit) combatLogMsg += '*'
    combatLogMsg += dmg
    if (isCrit) combatLogMsg += '*'
    if (isGlancing) combatLogMsg += ' Glancing'
    combatLogMsg += ' (' + Math.round(baseDamage) + ' Base Damage'
    if (this.type == SpellTypes.MAGICAL) {
      combatLogMsg += ' - ' + Math.round(this.coefficient * 1000) / 1000 + ' Coefficient'
      combatLogMsg += ' - ' + Math.round(this.pet.stats.spellPower) + ' Spell Power'
      combatLogMsg += ' - ' + Math.round(partialResistMultiplier * 1000) / 10 + '% Partial Resist Multiplier'
    } else if (this.type == SpellTypes.PHYSICAL) {
      if (isGlancing) combatLogMsg += ' - ' + this.pet.glancingBlowMultiplier * 100 + '% Glancing Blow Multiplier'
      combatLogMsg += ' - ' + Math.round(this.pet.getAttackPower()) + ' Attack Power'
      combatLogMsg += ' - ' + (Math.round(this.pet.armorMultiplier * 10000) / 100) + '% Damage Modifier (Armor)'
    }
    if (isCrit) combatLogMsg += ' - ' + (this.pet.critMultiplier * 100) + '% Crit Multiplier'
    combatLogMsg += ' - ' + Math.round(modifier * 10000) / 100 + '% Damage Modifier'
    combatLogMsg += ')'
    this.pet.player.combatLog(combatLogMsg)
    this.pet.player.damageBreakdown[this.varName].damage = this.pet.player.damageBreakdown[this.varName].damage + dmg || dmg
    this.pet.player.iterationDamage += dmg

    if (this.pet.pet == PetName.FELGUARD) {
      this.pet.auras.demonicFrenzy.apply()
    }
  }
}

class ImpFirebolt extends PetSpell {
  constructor (pet) {
    super(pet)
    this.name = 'Firebolt'
    this.castTime = 2 - (0.25 * pet.player.talents.improvedFirebolt)
    this.manaCost = 145
    this.dmg = 119.5 * (1 + 0.1 * pet.player.talents.improvedImp)
    this.coefficient = 2 / 3.5
    this.varName = 'firebolt'
    this.school = 'fire'
    this.pet.player.damageBreakdown[this.varName] = { name: 'Firebolt (Imp)' }
    this.type = SpellTypes.MAGICAL
  }
}

class Melee extends PetSpell {
  constructor (pet) {
    super(pet)
    this.cooldown = 2
    this.name = 'Melee'
    this.varName = 'melee'
    this.pet.player.damageBreakdown[this.varName] = { name: 'Melee (' + this.pet.name + ')' }
    this.resetsFiveSecondRule = false
    this.type = SpellTypes.PHYSICAL
  }

  getBaseDamage() {
    return this.pet.dmg
  }

  getCooldown() {
    return Math.round((this.cooldown / (1 + this.pet.stats.hastePercent / 100)) * 10000) / 10000
  }
}

class FelguardCleave extends PetSpell {
  constructor (pet) {
    super(pet)
    this.cooldown = 6
    this.manaCost = 417
    this.name = 'Cleave'
    this.varName = 'cleave'
    this.pet.player.damageBreakdown[this.varName] = { name: 'Cleave (Felguard)' }
    this.type = SpellTypes.PHYSICAL
  }

  getBaseDamage () {
    return this.pet.spells.melee.getBaseDamage() + 78
  }
}

class SuccubusLashOfPain extends PetSpell {
  constructor (pet) {
    super(pet)
    this.cooldown = 12
    this.manaCost = 190
    this.name = 'Lash of Pain'
    this.varName = 'lashOfPain'
    this.dmg = 123
    this.school = 'shadow'
    this.coefficient = 0.429
    this.pet.player.damageBreakdown[this.varName] = { name: 'Lash of Pain (Succubus)' }
    this.type = SpellTypes.MAGICAL
    this.canCrit = true
    this.resetsFiveSecondRule = true
    this.modifier = 1
  }
}
