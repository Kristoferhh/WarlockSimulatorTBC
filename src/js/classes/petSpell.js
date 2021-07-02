const SpellTypes = Object.freeze({
  MAGICAL: 0,
  PHYSICAL: 1
})

class PetSpell {
  constructor (pet) {
    this.pet = pet
    this.casting = false
    this.casting = false
    this.canCrit = true
    this.cooldownRemaining = 0
    this.castTime = 0
    this.manaCost = 0
    this.modifier = 1
    this.cooldown = 0
    this.resetsFiveSecondRule = true // true if it's a spell that resets the five second rule for mana regen from spirit (true for everything but melee attacks)
  }

  ready () {
    return this.cooldownRemaining <= 0 && this.pet.stats.mana >= this.manaCost && this.pet.castTimeRemaining <= 0
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
      this.pet.player.combatLog(this.pet.name + ' started casting ' + this.name)
    } else {
      this.cast()
    }
  }

  cast () {
    if (this.resetsFiveSecondRule) {
      this.pet.fiveSecondRuleTimerRemaining = 5
    }
    this.cooldownRemaining = this.cooldown
    let combatLogEntry = this.pet.name
    if (this.castTime > 0) combatLogEntry += ' finished casting ' + this.name
    else combatLogEntry += ' casts ' + this.name
    if (this.manaCost > 0) {
      this.pet.stats.mana = Math.max(0, this.pet.stats.mana - this.manaCost)
      combatLogEntry += '. Pet mana: ' + Math.round(this.pet.stats.mana) + '/' + Math.round(this.pet.stats.maxMana)
    }
    this.pet.player.combatLog(combatLogEntry)
    this.pet.player.damageBreakdown[this.varName].casts = this.pet.player.damageBreakdown[this.varName].casts + 1 || 1

    // Check for crit
    let isCrit = false
    if (this.canCrit) {
      isCrit = this.pet.isCrit(this.type)
      if (isCrit) {
        this.pet.player.damageBreakdown[this.varName].crits = this.pet.player.damageBreakdown[this.varName].crits + 1 || 1
      }
    }
    // Check for miss
    // todo change for spell hit
    const isMiss = (this.type == SpellTypes.PHYSICAL && random(1, 100) > this.pet.stats.hitChance) || (this.type == SpellTypes.MAGICAL && random(1, 100) > this.pet.stats.spellHitChance)
    if (isMiss) {
      this.pet.player.damageBreakdown[this.varName].misses = this.pet.player.damageBreakdown[this.varName].misses + 1 || 1
      this.pet.player.combatLog(this.pet.name + ' ' + this.name + ' *miss*')
    }
    // Check for dodge if melee
    const isDodge = this.type == SpellTypes.PHYSICAL && random(1, 100) <= this.pet.enemyDodgeChance
    if (isDodge) {
      this.pet.player.damageBreakdown[this.varName].dodges = this.pet.player.damageBreakdown[this.varName].dodges + 1 || 1
      if (!isMiss) this.pet.player.combatLog(this.pet.name + ' ' + this.name + ' *dodge*')
    }

    if (isMiss || isDodge) return

    // Calculate damage
    let dmg = this.calculateDamage() * this.pet.stats.damageModifier
    if (isCrit) {
      dmg *= 1.5
    }
    // Add damage from ISB if it's Lash of Pain
    if (this.varName == 'lashOfPain' && this.pet.player.auras.improvedShadowBolt && this.pet.player.auras.improvedShadowBolt.active) {
      dmg *= this.pet.player.auras.improvedShadowBolt.modifier
      this.pet.player.auras.improvedShadowBolt.decrementStacks()
    }
    // Add damage from Blood Frenzy debuff
    if (this.pet.playerAuras.bloodFrenzy) {
      dmg *= 1.04
    }
    // Armor damage reduction if physical
    if (this.type == SpellTypes.PHYSICAL) {
      dmg *= this.pet.armorMultiplier
    }

    this.pet.player.iterationDamage += dmg
    if (isCrit) this.pet.player.combatLog(this.pet.name + ' ' + this.name + ' *' + Math.round(dmg) + '*')
    else this.pet.player.combatLog(this.pet.name + ' ' + this.name + ' ' + Math.round(dmg))
    this.pet.player.damageBreakdown[this.varName].damage = this.pet.player.damageBreakdown[this.varName].damage + dmg || dmg

    if (this.pet.pet == PetName.FELGUARD) {
      this.pet.auras.demonicFrenzy.apply()
    }
  }
}

//todo fix so that debuffs on boss increase damage of pet abilities, such as shadow weaving and improved scorch
class Firebolt extends PetSpell {
  constructor(pet) {
    super(pet)
    this.name = 'Firebolt'
    this.castTime = 1.5 - (0.25 * pet.player.talents.improvedFirebolt)
    this.manaCost = 145
    this.damage = 119.5
    this.coefficient = 2 / 3.5
    this.varName = 'firebolt'
    this.pet.player.damageBreakdown[this.varName] = { name: 'Firebolt (Imp)' }
    this.type = SpellTypes.MAGICAL
  }

  calculateDamage() {
    return this.damage + (this.pet.stats.spellPower * this.coefficient) * this.modifier
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

  calculateDamage () {
    let ap = this.pet.stats.ap * this.pet.stats.apModifier
    if (this.pet.pet == PetName.FELGUARD) {
      ap *= (1 + 0.05 * this.pet.auras.demonicFrenzy.stacks)
    }
    return this.pet.dmg + (ap / 7)
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

  calculateDamage () {
    return this.pet.spells.melee.calculateDamage() + 78
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
    this.coefficient = 0.429
    this.pet.player.damageBreakdown[this.varName] = { name: 'Lash of Pain (Succubus)' }
    this.type = SpellTypes.MAGICAL
    this.modifier = 1

    // Check for shadow damage modifying debuffs on the boss
    if (this.pet.playerAuras.curseOfTheElements) this.modifier *= (1.1 + 0.01 * this.pet.simSettings.improvedCurseOfTheElements)
    if (this.pet.playerAuras.shadowWeaving) this.modifier *= 1.1
    if (this.pet.playerAuras.misery) this.modifier *= 1.05
  }

  calculateDamage () {
    return (this.dmg + this.pet.stats.spellPower * this.coefficient) * this.modifier
  }
}
