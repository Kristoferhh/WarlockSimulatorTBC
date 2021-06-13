// Damage over time auras
class DamageOverTime {
  constructor (player) {
    this.player = player
    this.durationTotal = 0
    this.tickTimerTotal = 3
    this.tickTimerRemaining = 0
    this.ticksRemaining = 0
    this.snapshots = true
    this.dmg = 0
    this.spellPower = 0
    this.modifier = 1
    this.active = false
    this.school = null
    this.name = null
    this.coefficient = 0
  }

  setup () {
    this.ticksTotal = Math.round(this.durationTotal / this.tickTimerTotal)
    this.originalDurationTotal = this.durationTotal
    this.varName = camelCase(this.name)
    this.player.damageBreakdown[this.varName] = this.player.damageBreakdown[this.varName] || { name: this.name }
  }

  apply (spellPower) {
    if (this.active) this.player.combatLog(this.name + ' refreshed')
    else this.player.combatLog(this.name + ' applied')
    this.active = true
    this.tickTimerRemaining = this.tickTimerTotal
    this.ticksRemaining = this.ticksTotal
    // Spellfire 3-set bonus
    if (this.player.sets['552'] >= 3) {
      spellPower += (this.player.stats.intellect * this.player.stats.intellectModifier * 0.07)
    }
    this.spellPower = spellPower

    // Siphon Life snapshots the presence of ISB. So if ISB isn't up when it's cast, it doesn't get the benefit even if it comes up later during the duration.
    if (this.varName == "siphonLife") {
      this.isbActive = (this.player.auras.improvedShadowBolt && this.player.auras.improvedShadowBolt.active)
    }
  }

  fade () {
    this.active = false
    this.tickTimerRemaining = 0
    this.ticksRemaining = 0
  }

  tick (t) {
    if (this.active) {
      this.tickTimerRemaining = Math.max(0, this.tickTimerRemaining - t)

      if (this.tickTimerRemaining == 0) {
        let sp = 0
        if (this.snapshots) {
          sp = this.spellPower
        } else {
          sp = this.player.stats.spellPower + this.player.demonicKnowledgeSp + this.player.stats[this.school + 'Power']
        }

        let dmg = ((this.dmg + sp * this.coefficient) * this.modifier * this.player.stats[this.school + 'Modifier']) / (this.originalDurationTotal / this.tickTimerTotal)
        // Add bonus from ISB (without removing ISB stacks since it's a dot)
        if (this.school == 'shadow' && this.player.auras.improvedShadowBolt && this.player.auras.improvedShadowBolt.active) {
          dmg *= this.player.auras.improvedShadowBolt.modifier
        }

        // Check for Nightfall proc
        if (this.varName == 'corruption' && this.player.talents.nightfall > 0) {
          if (random(1, 100) <= this.player.talents.nightfall * 2) {
            this.player.auras.shadowTrance.apply()
          }
        }

        // Partial resist damage reduction
        dmg = ~~(dmg * (1 - 0.0025 * this.player.enemy[this.school + 'Resist']))
        this.player.damageBreakdown[this.varName].damage = this.player.damageBreakdown[this.varName].damage + dmg || dmg
        this.player.iterationDamage += dmg
        this.player.combatLog(this.name + ' ' + Math.round(dmg))
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
          this.active = false
          this.player.combatLog(this.name + ' faded')
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
    this.modifier = 1 + (0.01 * this.player.talents.contagion)
    this.school = 'shadow'
    this.name = 'Corruption'
    this.coefficient = 0.936 + (0.12 * player.talents.empoweredCorruption)
    this.minimumDuration = 9
    this.boosted = false // Track whether the corruption's dmg has been boosted by T5 4pc or not
    this.setup()

    if (player.sets['529'] >= 4) this.modifier *= 1.12		// T3 4pc
    if (player.sets['645'] >= 4) this.durationTotal += 3	// T4 4pc
    if (player.sets['646'] >= 4) { this.ticksTotal = this.durationTotal / this.tickTimerTotal }
  }

  apply (spellPower) {
    // T5 4pc
    if (this.boosted) {
      this.modifier /= 1.1
      this.boosted = false
    }
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
    this.boosted = false // Track whether the Immolate's dmg has been boosted by T5 4pc or not
    this.setup()

    if (player.sets['645'] >= 4) this.durationTotal += 3	// T4 4pc
    this.ticksTotal = this.durationTotal / this.tickTimerTotal
  }

  apply (spellPower) {
    if (this.boosted) {
      this.modifier /= 1.1
      this.boosted = false
    }
    super.apply(spellPower)
  }
}

class CurseOfAgonyDot extends DamageOverTime {
  constructor (player) {
    super(player)
    this.durationTotal = 24
    this.tickTimerTotal = 3
    this.dmg = 1356 * (1 + 0.05 * player.talents.improvedCurseOfAgony)
    this.modifier = 1 + (0.01 * player.talents.contagion)
    this.school = 'shadow'
    this.name = 'Curse of Agony'
    this.coefficient = 1.2
    this.minimumDuration = 15
    this.setup()
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
