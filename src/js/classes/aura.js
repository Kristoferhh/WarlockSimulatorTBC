class Aura {
  constructor (player) {
    this.durationTotal = 0
    this.durationRemaining = 0
    this.player = player
    this.active = false
    this.isImportant = false
    this.hasDuration = true
  }

  setup () {
    this.varName = camelCase(this.name)
    this.player.auraBreakdown[this.varName] = this.player.auraBreakdown[this.varName] || { name: this.name }
  }

  tick (t) {
    if (this.hasDuration) {
      this.durationRemaining = Math.max(0, this.durationRemaining - t)

      if (this.durationRemaining == 0) {
        this.fade()
      }
    }
  }

  apply () {
    if (this.active) {
      this.player.combatLog(this.name + ' refreshed')
    } else {
      // Keep a timestamp of when the aura was applied so we can calculate the uptime when it fades
      this.player.auraBreakdown[this.varName].appliedAt = this.player.fightTime
      this.player.combatLog(this.name + ' applied')
    }
    this.player.auraBreakdown[this.varName].count = this.player.auraBreakdown[this.varName].count + 1 || 1
    this.durationRemaining = this.durationTotal
    if (this.isImportant) {
      this.player.importantAuraCounter++
      this.player.combatLog(this.player.importantAuraCounter + ' important auras active')
    }
    this.active = true
  }

  fade (endOfIteration = false) {
    if (this.active) {
      this.active = false
      if (!endOfIteration) {
        this.player.combatLog(this.name + ' faded')
      }
      if (this.isImportant) {
        this.player.importantAuraCounter--
        if (!endOfIteration) this.player.combatLog(this.player.importantAuraCounter + ' important auras active')
      }
      const auraUptime = this.player.fightTime - this.player.auraBreakdown[this.varName].appliedAt
      this.player.auraBreakdown[this.varName].uptime = this.player.auraBreakdown[this.varName].uptime + auraUptime || auraUptime
    }
  }
}

class ImprovedShadowBolt extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Improved Shadow Bolt'
    this.durationTotal = 12
    this.stacks = 0
    this.maxStacks = 4
    this.modifier = 1 + (this.player.talents.improvedShadowBolt * 0.04)
    this.setup()
  }

  apply () {
    super.apply()
    this.stacks = this.maxStacks
  }

  fade (endOfIteration = false) {
    this.stacks = 0
    super.fade(endOfIteration)
  }

  decrementStacks () {
    this.stacks--

    if (this.stacks <= 0) {
      this.active = false
      this.player.combatLog(this.name + ' faded')
    } else {
      this.player.combatLog(this.name + '(' + this.stacks + ')')
    }
  }
}

class CurseOfTheElementsAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Curse of the Elements'
    this.durationTotal = 300
    this.setup()
  }
}

class CurseOfRecklessnessAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Curse of Recklessness'
    this.durationTotal = 120
    this.setup()
  }
}

class ShadowTrance extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Shadow Trance (Nightfall)'
    this.durationTotal = 10
    this.isImportant = true
    this.setup()
  }
}

class Flameshadow extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Flameshadow (T4 2pc bonus)'
    this.durationTotal = 15
    this.procChance = 5
    this.isImportant = true
    this.shadowPower = 135
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Shadow Power + ' + this.shadowPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + this.shadowPower) + ')')
      this.player.stats.shadowPower += this.shadowPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
      super.apply()
    }
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Shadow Power - ' + this.shadowPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - this.shadowPower) + ')')
      this.player.stats.shadowPower -= this.shadowPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }

    super.fade(endOfIteration)
  }
}

class Shadowflame extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Shadowflame (T4 2pc bonus)'
    this.durationTotal = 15
    this.procChance = 5
    this.isImportant = true
    this.firePower = 135
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Fire Power + ' + this.firePower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + this.firePower) + ')')
      this.player.stats.firePower += 135
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
      super.apply()
    }
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Fire Power - ' + this.firePower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - this.firePower) + ')')
      this.player.stats.firePower -= 135
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }
}

class SpellstrikeProc extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Spellstrike Proc'
    this.durationTotal = 10
    this.procChance = 5
    this.isImportant = true
    this.spellPower = 92
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Spell Power + ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + this.spellPower) + ')')
      this.player.stats.spellPower += this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
      super.apply()
    }
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Spell Power - ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - this.spellPower) + ')')
      this.player.stats.spellPower -= this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }
}

class PowerInfusion extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Power Infusion'
    this.durationTotal = 15
    this.cooldown = 180
    this.cooldownRemaining = 0
    this.hasteModifier = 20
    this.manaModifier = 0.8
    this.isImportant = true
    this.setup()
  }

  reset () {
    this.cooldownRemaining = 0
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Haste % + ' + this.hasteModifier + ' (' + Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 + '% -> ' + Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 + this.hasteModifier + '%)')
      this.player.stats.hasteRating += hasteRatingPerPercent * this.hasteModifier
      this.player.stats.manaCostModifier *= this.manaModifier
    }
    this.cooldownRemaining = this.cooldown
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Haste % + ' + this.hasteModifier + ' (' + Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 + '% -> ' + Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 - this.hasteModifier + '%)')
      this.player.stats.hasteRating -= hasteRatingPerPercent * this.hasteModifier
      this.player.stats.manaCostModifier /= this.manaModifier
    }
    super.fade(endOfIteration)
  }

  tick (t) {
    this.cooldownRemaining = Math.max(0, this.cooldownRemaining - t)
    super.tick(t)
  }

  ready () {
    return this.cooldownRemaining <= 0
  }
}

class EyeOfMagtheridon extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Eye of Magtheridon'
    this.durationTotal = 10
    this.isImportant = true
    this.spellPower = 170
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Spell Power + ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + this.spellPower) + ')')
      this.player.stats.spellPower += this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Spell Power - ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - this.spellPower) + ')')
      this.player.stats.spellPower -= this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }
}

class SextantOfUnstableCurrents extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Sextant of Unstable Currents'
    this.durationTotal = 15
    this.hiddenCooldown = 45
    this.hiddenCooldownRemaining = 0
    this.procChance = 20
    this.isImportant = true
    this.spellPower = 190
    this.setup()
  }

  apply () {
    if (!this.active && this.hiddenCooldownRemaining == 0) {
      this.player.combatLog('Spell Power + ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + this.spellPower) + ')')
      this.player.stats.spellPower += this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
      this.hiddenCooldownRemaining = this.hiddenCooldown
      super.apply()
    }
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Spell Power - ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - this.spellPower) + ')')
      this.player.stats.spellPower -= this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }

  tick (t) {
    this.hiddenCooldownRemaining = Math.max(0, this.hiddenCooldownRemaining - t)
    super.tick(t)
  }
}

class QuagmirransEye extends Aura {
  constructor (player) {
    super(player)
    this.name = "Quagmirran's Eye"
    this.hiddenCooldown = 45
    this.hiddenCooldownRemaining = 0
    this.durationTotal = 6
    this.procChance = 10
    this.isImportant = true
    this.hasteRating = 320
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Haste Rating + ' + this.hasteRating + ' (' + Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 + '% -> ' + Math.round(((this.player.stats.hasteRating + this.hasteRating) / hasteRatingPerPercent) * 100) / 100 + '%)')
      this.player.stats.hasteRating += this.hasteRating
      this.hiddenCooldownRemaining = this.hiddenCooldown
      super.apply()
    }
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Haste Rating - ' + this.hasteRating + ' (' + Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 + '% -> ' + Math.round(((this.player.stats.hasteRating - this.hasteRating) / hasteRatingPerPercent) * 100) / 100 + '%)')
      this.player.stats.hasteRating -= this.hasteRating
    }
    super.fade(endOfIteration)
  }

  tick (t) {
    this.hiddenCooldownRemaining = Math.max(0, this.hiddenCooldownRemaining - t)
    super.tick(t)
  }
}

class ShiffarsNexusHorn extends Aura {
  constructor (player) {
    super(player)
    this.name = "Shiffar's Nexus-Horn"
    this.hiddenCooldown = 45
    this.hiddenCooldownRemaining = 0
    this.durationTotal = 10
    this.procChance = 20
    this.isImportant = true
    this.spellPower = 225
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Spell Power + ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + this.spellPower) + ')')
      this.player.stats.spellPower += this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
      this.hiddenCooldownRemaining = this.hiddenCooldown
      super.apply()
    }
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Spell Power - ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - this.spellPower) + ')')
      this.player.stats.spellPower -= this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }

  tick (t) {
    this.hiddenCooldownRemaining = Math.max(0, this.hiddenCooldownRemaining - t)
    super.tick(t)
  }
}

class ManaEtched4Set extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Mana-Etched 4-Set Bonus'
    this.durationTotal = 15
    this.procChance = 2
    this.isImportant = true
    this.spellPower = 110
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Spell Power + ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + this.spellPower) + ')')
      this.player.stats.spellPower += this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Spell Power - ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - this.spellPower) + ')')
      this.player.stats.spellPower -= this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }
}

class DestructionPotionAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Destruction Potion'
    this.durationTotal = 15
    this.isImportant = true
    this.setup()
  }

  apply () {
    if (!this.active) {
      {
        this.player.combatLog('Spell Power + 120 (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + 120) + ')')
        this.player.combatLog('Crit Chance + 2% (' + Math.round(this.player.stats.critChance * 100) / 100 + '% -> ' + Math.round((this.player.stats.critChance + 2) * 100) / 100 + '%)')
      }
      this.player.stats.spellPower += 120
      this.player.stats.critChance += 2
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) {
        this.player.combatLog('Spell Power - 120 (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - 120) + ')')
        this.player.combatLog('Crit Chance - 2% (' + Math.round(this.player.stats.critChance * 100) / 100 + '% -> ' + Math.round((this.player.stats.critChance - 2) * 100) / 100 + '%)')
      }
      this.player.stats.spellPower -= 120
      this.player.stats.critChance -= 2
      if (!endOfIteration && this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade()
  }
}

class FlameCapAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Flame Cap'
    this.durationTotal = 60
    this.isImportant = true
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Fire Power + 80 (' + Math.round(this.player.stats.firePower) + ' -> ' + Math.round(this.player.stats.firePower + 80) + ')')
      this.player.stats.firePower += 80
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Fire Power - 80 (' + Math.round(this.player.stats.firePower) + ' -> ' + Math.round(this.player.stats.firePower - 80) + ')')
      this.player.stats.firePower -= 80
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }
}

class BloodFuryAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Blood Fury'
    this.durationTotal = 15
    this.isImportant = true
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Spell Power + 140 (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + 140) + ')')
      this.player.stats.spellPower += 140
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Spell Power - 80 (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - 140) + ')')
      this.player.stats.spellPower -= 140
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }
}

// todo add haste to pet and maybe make a separate variable for haste % instead of converting it to haste rating.
class BloodlustAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Bloodlust'
    this.durationTotal = 40
    this.isImportant = true
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Haste + 30% (' + (Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100) + '% -> ' + (Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) + 30) * 100) / 100 + '%)')
      this.player.stats.hasteRating += 30 * hasteRatingPerPercent
      // if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Haste - 30% (' + (Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100) + '% -> ' + (Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) - 30) * 100) / 100 + '%)')
      this.player.stats.hasteRating -= 30 * hasteRatingPerPercent
      // if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
    }
    super.fade(endOfIteration)
  }
}

// todo add effects to pet
class DrumsOfBattleAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Drums of Battle'
    this.durationTotal = 30
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Haste rating + 80 (' + (Math.round((this.player.stats.hasteRating) / hasteRatingPerPercent) * 100) / 100 + '% -> ' + (Math.round((this.player.stats.hasteRating + 80) / hasteRatingPerPercent) * 100) / 100 + '%)')
      this.player.stats.hasteRating += 80
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Haste rating - 80 (' + (Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100) + '% -> ' + (Math.round((this.player.stats.hasteRating - 80) / hasteRatingPerPercent) * 100) / 100 + '%)')
      this.player.stats.hasteRating -= 80
    }
    super.fade(endOfIteration)
  }
}

class DrumsOfWarAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Drums of War'
    this.durationTotal = 30
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Spell Power + 30 (' + this.player.stats.spellPower + ' -> ' + (this.player.stats.spellPower + 30) + ')')
      this.player.stats.spellPower += 30
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Spell Power - 30 (' + this.player.stats.spellPower + ' -> ' + (this.player.stats.spellPower - 30) + ')')
      this.player.stats.spellPower -= 30
    }
    super.fade(endOfIteration)
  }
}

class DrumsOfRestorationAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Drums of Restoration'
    this.durationTotal = 15
    this.tickTimerTotal = 3
    this.tickTimerRemaining = 0
    this.ticksRemaining = 0
    this.ticksTotal = Math.round(this.durationTotal / this.tickTimerTotal)
    this.manaGain = 600 / (this.durationTotal / this.tickTimerTotal)
    this.setup()
  }

  apply () {
    this.active = true
    this.tickTimerRemaining = this.tickTimerTotal
    this.ticksRemaining = this.ticksTotal
  }

  tick (t) {
    if (this.active) {
      this.tickTimerRemaining = Math.max(0, this.tickTimerRemaining - t)

      if (this.tickTimerRemaining == 0) {
        this.player.combatLog('Player gains ' + this.manaGain + ' mana from Drums of Restoration (' + Math.round(this.player.mana) + ' -> ' + Math.round(this.player.mana + this.manaGain) + ')')
        this.player.mana = Math.min(this.player.stats.maxMana, this.player.mana + this.manaGain)
        this.ticksRemaining--
        this.tickTimerRemaining = this.tickTimerTotal

        if (this.ticksRemaining <= 0) {
          super.fade()
        }
      }
    }
  }
}

class AshtongueTalismanOfShadows extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Ashtongue Talisman of Shadows'
    this.durationTotal = 5
    this.procChance = 20
    this.isImportant = true
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Spell Power + 220 (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + 220) + ')')
      this.player.stats.spellPower += 220
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Spell Power - 80 (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - 220) + ')')
      this.player.stats.spellPower -= 220
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }
}

class DarkmoonCardCrusadeAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Darkmoon Card: Crusade'
    this.durationTotal = 10
    this.maxStacks = 10
    this.stacks = 0
    this.spPerStack = 8
    this.setup()
  }

  apply () {
    if (this.stacks < this.maxStacks) {
      this.player.combatLog('Spell Power + ' + this.spPerStack + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + this.spPerStack) + ')')
      this.player.stats.spellPower += this.spPerStack
      this.stacks++
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Spell Power - ' + (this.spPerStack * this.stacks) + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - this.spPerStack * this.stacks) + ')')
      this.player.stats.spellPower -= this.spPerStack * this.stacks
      this.stacks = 0
    }
    super.fade(endOfIteration)
  }
}

class TheLightningCapacitorAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'The Lightning Capacitor'
    this.hasDuration = false
    this.stacks = 0
    this.maxStacks = 3
    this.setup()
  }

  apply () {
    if (this.stacks < this.maxStacks) this.stacks++
    this.active = true
    this.player.combatLog('The Lightning Capacitor + 1 stack (' + this.stacks + ')')
  }

  fade (endOfIteration = false) {
    this.stacks = 0
    super.fade(endOfIteration)
  }
}

class BandOfTheEternalSageAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Band of the Eternal Sage'
    this.durationTotal = 10
    this.procChance = 10
    this.spellPower = 95
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Spell Power + ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + this.spellPower) + ')')
      this.player.stats.spellPower += this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.apply()
  }

  fade (endOfIteration) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Spell Power - ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - this.spellPower) + ')')
      this.player.stats.spellPower -= this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }
}

class BladeOfWizardryAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Blade of Wizardry'
    this.hasteRating = 280
    this.durationTotal = 6
    this.procChance = 15
    this.isImportant = true
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Haste Rating + ' + this.hasteRating + ' (' + Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 + '% -> ' + Math.round(((this.player.stats.hasteRating + this.hasteRating) / hasteRatingPerPercent) * 100) / 100 + '%)')
      this.player.stats.hasteRating += this.hasteRating
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Haste Rating - ' + this.hasteRating + ' (' + Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 + '% -> ' + Math.round(((this.player.stats.hasteRating - this.hasteRating) / hasteRatingPerPercent) * 100) / 100 + '%)')
      this.player.stats.hasteRating -= this.hasteRating
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }
}

class ShatteredSunPendantOfAcumenAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Shattered Sun Pendant of Acumen (Aldor)'
    this.spellPower = 120
    this.durationTotal = 10
    this.isImportant = true
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Spell Power + ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + this.spellPower) + ')')
      this.player.stats.spellPower += this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Spell Power - ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - this.spellPower) + ')')
      this.player.stats.spellPower -= this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }
}

class RobeOfTheElderScribesAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Robe of the Elder Scribes'
    this.spellPower = 130
    this.durationTotal = 10
    this.isImportant = true
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Spell Power + ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower + this.spellPower) + ')')
      this.player.stats.spellPower += this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Spell Power - ' + this.spellPower + ' (' + Math.round(this.player.stats.spellPower) + ' -> ' + Math.round(this.player.stats.spellPower - this.spellPower) + ')')
      this.player.stats.spellPower -= this.spellPower
      if (this.player.pet) this.player.pet.calculateStatsFromPlayer()
    }
    super.fade(endOfIteration)
  }
}
