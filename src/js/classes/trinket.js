class Trinket {
  constructor (player) {
    this.player = player
    this.cooldown = 0
    this.cooldownRemaining = 0
    this.active = false
    this.duration = 0
    this.durationRemaining = 0
    this.sharesCooldown = true
  }

  ready () {
    return this.cooldownRemaining <= 0
  }

  reset () {
    this.cooldownRemaining = 0
  }

  setup () {
    this.varName = camelCase(this.name)
    this.player.auraBreakdown[this.varName] = this.player.auraBreakdown[this.varName] || { name: this.name }
  }

  use () {
    if (!this.active) {
      let recalculatePetStats = false
      this.player.combatLog(this.name + ' used')
      this.player.auraBreakdown[this.varName].appliedAt = this.player.fightTime
      this.player.auraBreakdown[this.varName].count = this.player.auraBreakdown[this.varName].count + 1 || 1
      if (this.duration > 0) {
        for (const stat in this.stats) {
          if (this.player.stats.hasOwnProperty(stat)) {
            const combatLogStat = stat == 'spellPower' ? this.player.getSpellPower() : this.player.stats[stat]
            this.player.combatLog(stat + ' + ' + this.stats[stat] + ' (' + Math.round(combatLogStat) + ' -> ' + Math.round(combatLogStat + this.stats[stat]) + ')')
            this.player.stats[stat] += this.stats[stat]
            if (this.player.pet) {
              recalculatePetStats = true
            }
          }
        }
        if (recalculatePetStats) {
          this.player.pet.calculateStatsFromPlayer()
        }
        this.player.importantAuraCounter++
        //this.player.combatLog(this.player.importantAuraCounter + ' important auras active')
        this.active = true
        this.durationRemaining = this.duration
      }
      this.cooldownRemaining = this.cooldown
    }
  }

  fade (endOfIteration = false) {
    if (this.active) {
      let recalculatePetStats = false
      if (!endOfIteration) this.player.combatLog(this.name + ' faded')
      for (const stat in this.stats) {
        if (this.player.stats.hasOwnProperty(stat)) {
          if (!endOfIteration) {
            const combatLogStat = stat == 'spellPower' ? this.player.getSpellPower() : this.player.stats[stat]
            this.player.combatLog(stat + ' - ' + this.stats[stat] + ' (' + Math.round(combatLogStat) + ' -> ' + Math.round(combatLogStat - this.stats[stat]) + ')')
          }
          this.player.stats[stat] -= this.stats[stat]
          if (this.player.pet) {
            recalculatePetStats = true
          }
        }
      }
      if (recalculatePetStats) {
        this.player.pet.calculateStatsFromPlayer()
      }
      this.player.importantAuraCounter--
      this.active = false
      if (!endOfIteration) {
        //this.player.combatLog(this.player.importantAuraCounter + ' important auras active')
      }
      // Log the aura uptime
      const auraUptime = this.player.fightTime - this.player.auraBreakdown[this.varName].appliedAt
      this.player.auraBreakdown[this.varName].uptime = this.player.auraBreakdown[this.varName].uptime + auraUptime || auraUptime
    }
  }

  tick (t) {
    this.cooldownRemaining = Math.max(0, this.cooldownRemaining - t)
    this.durationRemaining = Math.max(0, this.durationRemaining - t)
    if (this.active && this.durationRemaining <= 0) {
      this.fade()
    }
  }
}

class RestrainedEssenceOfSapphiron extends Trinket {
  constructor (player) {
    super(player)
    this.name = 'Restrained Essence of Sapphiron'
    this.cooldown = 120
    this.duration = 20
    this.stats = {
      spellPower: 130
    }
    this.setup()
  }
}

class ShiftingNaaruSliver extends Trinket {
  constructor (player) {
    super(player)
    this.name = 'Shifting Naaru Sliver'
    this.cooldown = 90
    this.duration = 15
    this.stats = {
      spellPower: 320
    }
    this.setup()
  }
}

class SkullOfGuldan extends Trinket {
  constructor (player) {
    super(player)
    this.name = "The Skull of Gul'dan"
    this.cooldown = 120
    this.duration = 20
    this.stats = {
      hasteRating: 175
    }
    this.setup()
  }
}

class HexShrunkenHead extends Trinket {
  constructor (player) {
    super(player)
    this.name = 'Hex Shrunken Head'
    this.cooldown = 120
    this.duration = 20
    this.stats = {
      spellPower: 211
    }
    this.setup()
  }
}

class IconOfTheSilverCrescent extends Trinket {
  constructor (player) {
    super(player)
    this.name = 'Icon of the Silver Crescent'
    this.cooldown = 120
    this.duration = 20
    this.stats = {
      spellPower: 155
    }
    this.setup()
  }
}

class ScryersBloodgem extends Trinket {
  constructor (player) {
    super(player)
    this.name = "Scryer's Bloodgem"
    this.cooldown = 90
    this.duration = 15
    this.stats = {
      spellPower: 150
    }
    this.setup()
  }
}

class AncientCrystalTalisman extends Trinket {
  constructor (player) {
    super(player)
    this.name = 'Ancient Crystal Talisman'
    this.cooldown = 120
    this.duration = 20
    this.stats = {
      spellPower: 104
    }
    this.setup()
  }
}

class ArcanistsStone extends Trinket {
  constructor (player) {
	 	super(player)
	 	this.name = "Arcanist's Stone"
	 	this.cooldown = 120
	 	this.duration = 20
	 	this.stats = {
	 		spellPower: 167
	 	}
     this.setup()
  }
}

class TerokkarTabletOfVim extends Trinket {
  constructor (player) {
    super(player)
    this.name = 'Terokkar Tablet of Vim'
    this.cooldown = 90
    this.duration = 15
    this.stats = {
      spellPower: 84
    }
    this.setup()
  }
}

class XirisGift extends Trinket {
  constructor (player) {
    super(player)
    this.name = "Xi'ri's Gift"
    this.cooldown = 90
    this.duration = 15
    this.stats = {
      spellPower: 150
    }
    this.setup()
  }
}

class VengeanceOfTheIllidari extends Trinket {
  constructor (player) {
    super(player)
    this.name = 'Vengeance of the Illidari'
    this.cooldown = 90
    this.duration = 15
    this.stats = {
      spellPower: 120
    }
    this.setup()
  }
}

class FigurineLivingRubySerpent extends Trinket {
  constructor (player) {
    super(player)
    this.name = 'Figurine - Living Ruby Serpent'
    this.cooldown = 300
    this.duration = 20
    this.stats = {
      spellPower: 150
    }
    this.setup()
  }
}

class EssenceOfTheMartyr extends Trinket {
  constructor (player) {
    super(player)
    this.name = 'Essence of the Martyr'
    this.cooldown = 120
    this.duration = 20
    this.stats = {
      spellPower: 99
    }
    this.setup()
    this.sharesCooldown = false
  }
}

class StarkillersBauble extends Trinket {
  constructor (player) {
    super(player)
    this.name = "Starkiller's Bauble",
    this.cooldown = 90
    this.duration = 15
    this.stats = {
      spellPower: 125
    }
    this.setup()
  }
}