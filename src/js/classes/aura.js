class Aura {
  constructor (player) {
    this.durationTotal = 0
    this.durationRemaining = 0
    this.hiddenCooldown = 0
    this.player = player
    this.active = false
    this.isImportant = false
    this.hasDuration = true
    this.groupWide = false // true if it's an aura that applies to everyone in the group (will apply to pets as well then)
    this.stats = {}
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
      let recalculatePetStats = false
      // Keep a timestamp of when the aura was applied so we can calculate the uptime when it fades
      this.player.auraBreakdown[this.varName].appliedAt = this.player.fightTime
      // Add stats from the aura if it has any
      for (const stat in this.stats) {
        if (this.player.stats.hasOwnProperty(stat)) {
          this.player.combatLog(stat + ' + ' + this.stats[stat] + ' (' + Math.round(this.player.stats[stat]) + ' -> ' + Math.round(this.player.stats[stat] + this.stats[stat]) + ')')
          this.player.stats[stat] += this.stats[stat]
          // Add stats to pet
          if (this.player.pet) {
            if (this.player.pet.stats.hasOwnProperty(stat)) {
              this.player.pet.stats[stat] += this.stats[stat]
            }
            if (this.player.pet.stats.buffs.hasOwnProperty(stat)) {
              recalculatePetStats = true
            }
          }
        }
      }
      if (recalculatePetStats) {
        this.player.pet.calculateStatsFromPlayer()
      }
      this.player.combatLog(this.name + ' applied')
      if (this.isImportant) {
        this.player.importantAuraCounter++
        this.player.combatLog(this.player.importantAuraCounter + ' important auras active')
      }
    }
    this.player.auraBreakdown[this.varName].count = this.player.auraBreakdown[this.varName].count + 1 || 1
    this.durationRemaining = this.durationTotal
    this.active = true
  }

  fade (endOfIteration = false) {
    if (endOfIteration) {
      this.hiddenCooldownRemaining = 0
    }
    if (this.active) {
      // Remove the aura's stats
      let recalculatePetStats = false
      for (const stat in this.stats) {
        if (this.player.stats.hasOwnProperty(stat)) {
          if (!endOfIteration) {
            this.player.combatLog(stat + ' - ' + this.stats[stat] + ' (' + Math.round(this.player.stats[stat]) + ' -> ' + Math.round(this.player.stats[stat] - this.stats[stat]) + ')')
          }
          this.player.stats[stat] -= this.stats[stat]
          // Remove stats from pet
          if (this.player.pet) {
            if (this.player.pet.stats.hasOwnProperty(stat)) {
              this.player.pet.stats[stat] -= this.stats[stat]
            }
            if (!endOfIteration && this.player.pet.stats.buffs.hasOwnProperty(stat)) {
              recalculatePetStats = true
            }
          }
        }
      }
      this.active = false
      if (!endOfIteration) {
        this.player.combatLog(this.name + ' faded')
        if (recalculatePetStats) {
          this.player.pet.calculateStatsFromPlayer()
        }
      }
      if (this.isImportant) {
        this.player.importantAuraCounter--
        if (!endOfIteration) {
          this.player.combatLog(this.player.importantAuraCounter + ' important auras active')
        }
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

  decrementStacks () {
    this.stacks--

    if (this.stacks <= 0) {
      this.fade()
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
    this.stats = {
      shadowPower: 135
    }
    this.setup()
  }
}

class Shadowflame extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Shadowflame (T4 2pc bonus)'
    this.durationTotal = 15
    this.procChance = 5
    this.isImportant = true
    this.stats = {
      firePower: 135
    }
    this.setup()
  }
}

class SpellstrikeProc extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Spellstrike Proc'
    this.durationTotal = 10
    this.procChance = 5
    this.isImportant = true
    this.stats = {
      spellPower: 92
    }
    this.setup()
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
      if (!endOfIteration) {
        this.player.combatLog('Haste % + ' + this.hasteModifier + ' (' + Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 + '% -> ' + Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 - this.hasteModifier + '%)')
      }
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
    this.stats = {
      spellPower: 170
    }
    this.setup()
  }
}

class SextantOfUnstableCurrentsAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Sextant of Unstable Currents'
    this.durationTotal = 15
    this.isImportant = true
    this.stats = {
      spellPower: 190
    }
    this.setup()
  }
}

class QuagmirransEyeAura extends Aura {
  constructor (player) {
    super(player)
    this.name = "Quagmirran's Eye"
    this.durationTotal = 6
    this.isImportant = true
    this.stats = {
      hasteRating: 320
    }
    this.setup()
  }
}

class ShiffarsNexusHornAura extends Aura {
  constructor (player) {
    super(player)
    this.name = "Shiffar's Nexus-Horn"
    this.durationTotal = 10
    this.isImportant = true
    this.stats = {
      spellPower: 225
    }
    this.setup()
  }
}

class ManaEtched4Set extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Mana-Etched 4-Set Bonus'
    this.durationTotal = 15
    this.procChance = 2
    this.isImportant = true
    this.stats = {
      spellPower: 110
    }
    this.setup()
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
        this.player.combatLog('Spell Power + 120 (' + Math.round(this.player.getSpellPower()) + ' -> ' + Math.round(this.player.getSpellPower() + 120) + ')')
        this.player.combatLog('Crit Chance + 2% (' + Math.round(this.player.stats.critChance * 100) / 100 + '% -> ' + Math.round((this.player.stats.critChance + 2) * 100) / 100 + '%)')
      }
      this.player.stats.spellPower += 120
      this.player.stats.critChance += 2
      if (this.player.pet) {
        this.player.pet.calculateStatsFromPlayer()
      }
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) {
        this.player.combatLog('Spell Power - 120 (' + Math.round(this.player.getSpellPower()) + ' -> ' + Math.round(this.player.getSpellPower() - 120) + ')')
        this.player.combatLog('Crit Chance - 2% (' + Math.round(this.player.stats.critChance * 100) / 100 + '% -> ' + Math.round((this.player.stats.critChance - 2) * 100) / 100 + '%)')
      }
      this.player.stats.spellPower -= 120
      this.player.stats.critChance -= 2
      if (!endOfIteration && this.player.pet) {
        this.player.pet.calculateStatsFromPlayer()
      }
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
    this.stats = {
      firePower: 80
    }
    this.setup()
  }
}

class BloodFuryAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Blood Fury'
    this.durationTotal = 15
    this.isImportant = true
    this.stats = {
      spellPower: 140
    }
    this.setup()
  }
}

class BloodlustAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Bloodlust'
    this.durationTotal = 40
    this.isImportant = true
    this.groupWide = true
    this.setup()
  }

  apply () {
    if (!this.active) {
      this.player.combatLog('Haste + 30% (' + (Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100) + '% -> ' + (Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) + 30) * 100) / 100 + '%)')
      this.player.stats.hasteRating += 30 * hasteRatingPerPercent
      if (this.player.pet) {
        this.player.pet.stats.hasteRating += 30 * hasteRatingPerPercent
      }
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) this.player.combatLog('Haste - 30% (' + (Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 100) / 100) + '% -> ' + (Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) - 30) * 100) / 100 + '%)')
      this.player.stats.hasteRating -= 30 * hasteRatingPerPercent
      if (this.player.pet) {
        this.player.pet.stats.hasteRating -= 30 * hasteRatingPerPercent
      }
      super.fade(endOfIteration)
    }
  }
}

class DrumsOfBattleAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Drums of Battle'
    this.durationTotal = 30
    this.groupWide = true
    this.stats = {
      hasteRating: 80
    }
    this.setup()
  }
}

class DrumsOfWarAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Drums of War'
    this.durationTotal = 30
    this.groupWide = true
    this.stats = {
      spellPower: 30
    }
    this.setup()
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
    this.groupWide = true
    this.ticksTotal = Math.round(this.durationTotal / this.tickTimerTotal)
    this.manaGain = 600 / (this.durationTotal / this.tickTimerTotal)
    this.setup()
    this.player.manaGainBreakdown[this.varName] = this.player.manaGainBreakdown[this.varName] || { name: 'Drums of Restoration' }
  }

  apply () {
    this.tickTimerRemaining = this.tickTimerTotal
    this.ticksRemaining = this.ticksTotal
    super.apply()
  }

  tick (t) {
    if (this.active) {
      this.tickTimerRemaining = Math.max(0, this.tickTimerRemaining - t)

      if (this.tickTimerRemaining == 0) {
        // Player
        const currentMana = this.player.mana
        this.player.mana = Math.min(this.player.stats.maxMana, this.player.mana + this.manaGain)
        const manaGained = Math.round(this.player.mana - currentMana)
        this.player.combatLog('Player gains ' + manaGained + ' mana from Drums of Restoration (' + Math.round(currentMana) + ' -> ' + Math.round(this.player.mana) + ')')
        this.player.manaGainBreakdown[this.varName].casts = this.player.manaGainBreakdown[this.varName].casts + 1 || 1
        this.player.manaGainBreakdown[this.varName].manaGain = this.player.manaGainBreakdown[this.varName].manaGain + manaGained || manaGained
        // Pet
        if (this.player.pet) {
          const currentMana = this.player.pet.stats.mana
          this.player.pet.stats.mana = Math.min(this.player.pet.stats.maxMana, this.player.pet.stats.mana + this.manaGain)
          this.player.combatLog(this.player.pet.name + ' gains ' + (this.player.pet.stats.mana - currentMana) + ' mana from Drums of Restoration (' + Math.round(currentMana) + ' -> ' + Math.round(this.player.pet.stats.mana) + ')')
        }
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
    this.stats = {
      spellPower: 220
    }
    this.setup()
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
      this.player.combatLog('Spell Power + ' + this.spPerStack + ' (' + Math.round(this.player.getSpellPower()) + ' -> ' + Math.round(this.player.getSpellPower() + this.spPerStack) + ')')
      this.player.stats.spellPower += this.spPerStack
      this.stacks++
      if (this.player.pet) {
        this.player.pet.calculateStatsFromPlayer()
      }
    }
    super.apply()
  }

  fade (endOfIteration = false) {
    if (this.active) {
      if (!endOfIteration) {
        this.player.combatLog('Spell Power - ' + (this.spPerStack * this.stacks) + ' (' + Math.round(this.player.getSpellPower()) + ' -> ' + Math.round(this.player.getSpellPower() - this.spPerStack * this.stacks) + ')')
        if (this.player.pet) {
          this.player.pet.calculateStatsFromPlayer()
        }
      }
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
    this.stats = {
      spellPower: 95
    }
    this.setup()
  }
}

class BladeOfWizardryAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Blade of Wizardry'
    this.durationTotal = 6
    this.procChance = 15
    this.isImportant = true
    this.stats = {
      hasteRating: 280
    }
    this.setup()
  }
}

class ShatteredSunPendantOfAcumenAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Shattered Sun Pendant of Acumen (Aldor)'
    this.durationTotal = 10
    this.isImportant = true
    this.stats = {
      spellPower: 120
    }
    this.setup()
  }
}

class RobeOfTheElderScribesAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Robe of the Elder Scribes'
    this.durationTotal = 10
    this.isImportant = true
    this.stats = {
      spellPower: 130
    }
    this.setup()
  }
}

class MysticalSkyfireDiamondAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Mystical Skyfire Diamond'
    this.durationTotal = 4
    this.isImportant = true
    this.stats = {
      hasteRating: 320
    }
    this.setup()
  }
}

class AmplifyCurseAura extends Aura {
  constructor (player) {
    super(player)
    this.name = 'Amplify Curse'
    this.durationTotal = 30
    this.setup()
  }
}