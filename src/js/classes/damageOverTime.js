// Damage over time auras
class DamageOverTime {
  constructor (player) {
    this.player = player
    this.durationTotal = 0 // Total duration of the dot
    this.tickTimerTotal = 3 // Total duration of each tick (default is 3 seconds between ticks)
    this.tickTimerRemaining = 0 // Time until next tick
    this.ticksRemaining = 0 // Amount of ticks remaining before the dot expires
    this.snapshots = true
    this.dmg = 0
    this.spellPower = 0 // Amount of Spell Power (plus Shadow or Fire Power) when the dot was applied
    this.modifier = 1
    this.active = false
    this.school = null
    this.name = null
    this.coefficient = 0
  }

  setup () {
    this.varName = camelCase(this.name)
    this.originalDurationTotal = this.durationTotal
    // T4 4pc
    if ((this.varName == 'immolate' || this.varName == 'corruption') && this.player.sets['645'] >= 4) {
      this.durationTotal += 3
    }
    this.ticksTotal = Math.round(this.durationTotal / this.tickTimerTotal)
    this.player.damageBreakdown[this.varName] = this.player.damageBreakdown[this.varName] || { name: this.name }
    this.player.auraBreakdown[this.varName] = this.player.auraBreakdown[this.varName] || { name: this.name }
  }

  apply (spellPower) {
    if (this.active) {
      this.player.combatLog(this.name + ' refreshed before letting it expire')
    }
    // Keep a timestamp of when the aura was applied so we can calculate the uptime when it fades
    this.player.auraBreakdown[this.varName].appliedAt = this.player.fightTime
    this.player.auraBreakdown[this.varName].count = this.player.auraBreakdown[this.varName].count + 1 || 1
    const refreshedOrApplied = this.active ? 'refreshed' : 'applied'
    this.player.combatLog(this.name + ' ' + refreshedOrApplied + ' (' + Math.round(spellPower) + ' Spell Power)')
    this.active = true
    this.tickTimerRemaining = this.tickTimerTotal
    this.ticksRemaining = this.ticksTotal
    // Spellfire 3-set bonus
    if (this.player.sets['552'] >= 3) {
      spellPower += (this.player.stats.intellect * this.player.stats.intellectModifier * 0.07)
    }
    this.spellPower = spellPower

    // Siphon Life snapshots the presence of ISB. So if ISB isn't up when it's cast, it doesn't get the benefit even if it comes up later during the duration.
    if (this.varName == 'siphonLife') {
      this.isbActive = (this.player.auras.improvedShadowBolt && this.player.auras.improvedShadowBolt.active)
    }
  }

  fade (endOfIteration = false) {
    this.active = false
    this.tickTimerRemaining = 0
    this.ticksRemaining = 0
    const auraUptime = this.player.fightTime - this.player.auraBreakdown[this.varName].appliedAt
    this.player.auraBreakdown[this.varName].uptime = this.player.auraBreakdown[this.varName].uptime + auraUptime || auraUptime
    if (!endOfIteration) {
      this.player.combatLog(this.name + ' faded')
    }
  }

  getModifier () {
    return this.modifier * this.player.stats[this.school + 'Modifier']
  }

  tick (t) {
    if (this.active) {
      this.tickTimerRemaining = Math.max(0, this.tickTimerRemaining - t)

      if (this.tickTimerRemaining == 0) {
        const sp = this.snapshots ? this.spellPower : this.player.stats.spellPower + this.player.demonicKnowledgeSp + this.player.stats[this.school + 'Power']
        let modifier = this.getModifier()

        // Add bonus from ISB (without removing ISB stacks since it's a dot)
        if ((this.school == 'shadow' && this.player.auras.improvedShadowBolt && this.player.auras.improvedShadowBolt.active && this.varName != 'siphonLife') || (this.varName == 'siphonLife' && this.isbActive)) {
          modifier *= this.player.auras.improvedShadowBolt.modifier
        }
        let dmg = ((this.dmg + sp * this.coefficient) * modifier) / (this.originalDurationTotal / this.tickTimerTotal)

        // Check for Nightfall proc
        if (this.varName == 'corruption' && this.player.talents.nightfall > 0) {
          if (random(1, 100) <= this.player.talents.nightfall * 2) {
            this.player.auras.shadowTrance.apply()
          }
        }

        // Partial resist damage reduction
        const partialResistMultiplier = this.player.getPartialResistMultiplier(this.player.enemy[this.school + 'Resist'])
        dmg = Math.round(dmg * partialResistMultiplier)
        this.player.damageBreakdown[this.varName].damage = this.player.damageBreakdown[this.varName].damage + dmg || dmg
        this.player.iterationDamage += dmg
        this.player.combatLog(this.name + ' Tick ' + Math.round(dmg) + ' (' + this.dmg + ' Base Damage - ' + sp + ' Spell Power - ' + Math.round(modifier * 10000) / 100 + '% Damage Modifier ' + Math.round(partialResistMultiplier * 1000) / 1000 + '% Partial Resist Multiplier)')
        this.ticksRemaining--
        this.tickTimerRemaining = this.tickTimerTotal

        // Ashtongue Talisman of Shadows
        if (this.varName == 'corruption' && this.player.auras.ashtongueTalismanOfShadows && random(1, 100) <= this.player.auras.ashtongueTalismanOfShadows.procChance) {
          this.player.auras.ashtongueTalismanOfShadows.apply()
        }

        // Timbal's Focusing Crystal
        if (this.player.spells.timbalsFocusingCrystal && this.player.spells.timbalsFocusingCrystal.cooldownRemaining <= 0 && random(1, 100) <= this.player.spells.timbalsFocusingCrystal.procChance) {
          this.player.spells.timbalsFocusingCrystal.startCast()
        }

        if (this.ticksRemaining <= 0) {
          this.fade()
        }
      }
    }
  }
}

class CorruptionDot extends DamageOverTime {
  constructor (player) {
    super(player)
    this.durationTotal = 18
    this.tickTimerTotal = 3
    this.dmg = 900
    this.school = 'shadow'
    this.name = 'Corruption'
    this.coefficient = 0.936 + (0.12 * player.talents.empoweredCorruption)
    this.minimumDuration = 9
    this.setup()

    // T3 4pc
    if (player.sets['529'] >= 4) this.modifier *= 1.12
  }

  getModifier () {
    let modifier = super.getModifier()
    if (this.player.talents.shadowMastery > 0 && this.player.talents.contagion > 0) {
      // Divide away the bonus from Shadow Mastery
      modifier /= (1 + (this.player.talents.shadowMastery * 0.02))
      // Multiply the modifier with the bonus from Shadow Mastery + Contagion
      modifier *= (1 * (1 + ((this.player.talents.shadowMastery * 0.02) + (this.player.talents.contagion / 100))))
    }
    // Add the t5 4-set bonus modifier
    modifier *= this.t5BonusModifier
    return modifier
  }

  apply (spellPower) {
    // T5 4-set bonus modifier
    this.t5BonusModifier = 1
    super.apply(spellPower)
  }
}

class UnstableAfflictionDot extends DamageOverTime {
  constructor (player) {
    super(player)
    this.durationTotal = 18
    this.tickTimerTotal = 3
    this.dmg = 1050
    this.school = 'shadow'
    this.name = 'Unstable Affliction'
    this.coefficient = 1.2
    this.minimumDuration = 9
    this.setup()
  }
}

class SiphonLifeDot extends DamageOverTime {
  constructor (player) {
    super(player)
    this.durationTotal = 30
    this.tickTimerTotal = 3
    this.dmg = 630
    this.school = 'shadow'
    this.name = 'Siphon Life'
    this.coefficient = 1
    this.minimumDuration = 30
    this.setup()
  }
}

class ImmolateDot extends DamageOverTime {
  constructor (player) {
    super(player)
    this.durationTotal = 15
    this.tickTimerTotal = 3
    this.dmg = 615
    this.school = 'fire'
    this.name = 'Immolate'
    this.coefficient = 0.65
    this.minimumDuration = 12
    this.setup()
  }

  getModifier () {
    return super.getModifier() * this.t5BonusModifier
  }

  apply (spellPower) {
    // T5 4-set bonus modifier
    this.t5BonusModifier = 1
    super.apply(spellPower)
  }
}

class CurseOfAgonyDot extends DamageOverTime {
  constructor (player) {
    super(player)
    this.durationTotal = 24
    this.tickTimerTotal = 3
    this.dmg = 1356
    this.school = 'shadow'
    this.name = 'Curse of Agony'
    this.coefficient = 1.2
    this.minimumDuration = 15
    this.setup()
  }

  getModifier () {
    let modifier = super.getModifier()
    // Remove bonus from Shadow Mastery and add bonus from Shadow Mastery + Contagion + Improved Curse of Agony
    modifier /= (1 + (this.player.talents.shadowMastery * 0.02))
    modifier *= (1 * (1 + ((this.player.talents.shadowMastery * 0.02) + (this.player.talents.contagion / 100) + (this.player.talents.improvedCurseOfAgony * 0.05))))
    return modifier
  }
}

class CurseOfDoomDot extends DamageOverTime {
  constructor (player) {
    super(player)
    this.durationTotal = 60
    this.tickTimerTotal = 60
    this.dmg = 4200
    this.school = 'shadow'
    this.name = 'Curse of Doom'
    this.coefficient = 2
    this.setup()
  }
}
