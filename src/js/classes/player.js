class Player {
  static getSettings () {
    return {
      auras: auras,
      items: selectedItems,
      sets: JSON.parse(localStorage.setBonuses),
      enchants: selectedEnchants,
      gems: selectedGems,
      talents: talents,
      stats: characterStats,
      simSettings: settings,
      enemy: {
        level: parseInt($("input[name='target-level']").val()),
        shadowResist: parseInt($("input[name='target-shadow-resistance']").val()),
        fireResist: parseInt($("input[name='target-fire-resistance']").val()),
        armor: parseInt($("input[name='enemyArmor']").val())
      },
      rotation: rotation,
      selectedItemSlot: $("#item-slot-selection-list li[data-selected='true']").attr('data-slot')
    }
  }

  constructor (settings, customItemSlot, customItemSubSlot, customItemId, customStat, customStatValue) {
    this.stats = JSON.parse(JSON.stringify(settings.stats)) // Create a deep-copy of the character's stats since we need to modify the values.
    this.items = JSON.parse(JSON.stringify(settings.items))
    this.enemy = settings.enemy
    this.rotation = settings.rotation
    this.talents = settings.talents
    this.simSettings = settings.simSettings
    this.level = 70
    this.gcdValue = 1.5
    this.shattrathFaction = settings.simSettings.shattrathFaction // Aldor or Scryers
    this.exaltedWithShattrathFaction = settings.simSettings.shattrathFactionReputation == 'yes'
    this.itemId = customItemId || settings.items[settings.selectedItemSlot] || 0
    this.sets = JSON.parse(JSON.stringify(settings.sets))
    this.selectedAuras = settings.auras
    this.minimumGcdValue = 1
    // I don't know if this formula only works for bosses or not, so for the moment I'm only using it for lvl >=73 targets.
    const enemyBaseResistance = settings.enemy.level >= 73 ? (6 * this.level * 5) / 75 : 0
    this.enemy.shadowResist = Math.max(this.enemy.shadowResist, enemyBaseResistance, 0)
    this.enemy.fireResist = Math.max(this.enemy.fireResist, enemyBaseResistance, 0)
    this.enemy.natureResist = Math.max(enemyBaseResistance, 0)
    this.enemy.arcaneResist = Math.max(enemyBaseResistance, 0)
    this.enemy.frostResist = Math.max(enemyBaseResistance, 0)
    this.combatlog = []
    this.critMultiplier = 1.5
    this.importantAuraCounter = 0 // counts the amount of active "important" auras such as trinket procs, on-use trinket uses, power infusion etc.
    this.totalManaRegenerated = 0
    this.simChoosingRotation = settings.simSettings.rotationOption == 'simChooses'
    // The amount to increase spell cast times by.
    // This will not have any actual effect on the dps result because of how small the value is, but it will make things a lot more realistic
    // because cast times, dot ticks, and such will fall out of sync with each other (which is what happens when a real player is playing)
    // and it will make it easier to, for example, pre-cast Immolate to reapply it right after it expires.
    // If Immolate has 1.5 seconds remaining, the sim won't start casting immolate because it would refresh before it expires
    // so Immolate would need to have ~1.49999s left of its duration for it to start casting.
    // But with this delay, Immolate's cast time would be ~1.500001 which would allow it to reapply Immolate when it has 1.5 seconds left
    // This should solve that problem if I'm thinking through this correctly.
    this.spellDelay = 0.0001
    this.customStat = {
      stat: customStat,
      value: customStatValue
    }

    // Check if we're finding stat weights
    if (this.customStat.stat && this.stats.hasOwnProperty(this.customStat.stat)) {
      this.stats[this.customStat.stat] += this.customStat.value
    }

    this.metaGemIds = []
    // Get the meta gem ID
    if (settings.gems && settings.gems.head && settings.items.head) {
      for (const gemSocket in settings.gems.head[settings.items.head]) {
        if (settings.gems.head[settings.items.head][gemSocket]) {
          const gemId = settings.gems.head[settings.items.head][gemSocket][1]
          if (gems.meta[gemId]) {
            this.metaGemIds.push(gemId)
          }
        }
      }
    }

    // If the player is equipped with a custom item then remove the stats from the currently equipped item and add stats from the custom item
    if (customItemSlot && customItemId && customItemId !== settings.items[customItemSlot + customItemSubSlot]) {
      // Loop through all items in the custom item slot
      for (const item in items[customItemSlot]) {
        // Check if this is the currently equipped item
        if (items[customItemSlot][item].id == settings.items[customItemSlot + customItemSubSlot]) {
          // Remove stats from currently equipped item
          for (const stat in items[customItemSlot][item]) {
            if (this.stats.hasOwnProperty(stat)) {
              this.stats[stat] -= items[customItemSlot][item][stat]
            }
          }
          // Decrement the counter for the set id if it is part of a set
          if (items[customItemSlot][item].hasOwnProperty('setId')) {
            this.sets[items[customItemSlot][item].setId]--
          }
          // Remove stats from gems in the equipped item if there are any
          // Also check if the item's socket bonus is active and remove the stats from it if so
          if (settings.gems[customItemSlot] && settings.gems[customItemSlot][settings.items[customItemSlot + customItemSubSlot]]) {
            let socketBonusActive = true
            // Loop through each socket in the equipped item
            for (const socket in settings.gems[customItemSlot][settings.items[customItemSlot + customItemSubSlot]]) {
              if (settings.gems[customItemSlot][settings.items[customItemSlot + customItemSubSlot]][socket]) {
                const socketColor = settings.gems[customItemSlot][settings.items[customItemSlot + customItemSubSlot]][socket][0]
                const gemId = settings.gems[customItemSlot][settings.items[customItemSlot + customItemSubSlot]][socket][1]
                // Find the gem's color since the socket and gem colors might not match
                for (const gemColor in gems) {
                  if (gems[gemColor][gemId]) {
                    // Check whether the gem color is in the array of gem colors that are valid for this socket color (e.g. checks whether a 'purple' gem is valid for a 'blue' socket)
                    if (!socketInfo[socketColor].gems.includes(gemColor)) {
                      socketBonusActive = false
                    }
                    if (gemColor == 'meta') {
                      if (this.metaGemIds.includes(gemId)) {
                        this.metaGemIds.splice(this.metaGemIds.indexOf(gemId), 1)
                      }
                    }
                    // Loop through the gem's stats and remove them from the player
                    for (const stat in gems[gemColor][gemId]) {
                      if (this.stats.hasOwnProperty(stat)) {
                        this.stats[stat] -= gems[gemColor][gemId][stat]
                      }
                    }
                  }
                }
              } else { // No gem in this socket
                socketBonusActive = false
              }
            }
            // If the socket bonus is active then remove the socket bonus stats
            if (socketBonusActive && items[customItemSlot][item].socketBonus) {
              for (const stat in items[customItemSlot][item].socketBonus) {
                if (this.stats.hasOwnProperty(stat)) {
                  this.stats[stat] -= items[customItemSlot][item].socketBonus[stat]
                } else {
                  console.log("Can't remove stat '" + stat + "' from player (trying to remove socket bonus)")
                }
              }
            }
          }
        }
        // Check if this is the custom item
        else if (items[customItemSlot][item].id == customItemId) {
          // Add stats from the item
          for (const stat in items[customItemSlot][item]) {
            if (this.stats.hasOwnProperty(stat)) {
              this.stats[stat] += items[customItemSlot][item][stat]
            }
          }
          // Increment the counter for the set id if it is part of a set
          if (items[customItemSlot][item].hasOwnProperty('setId')) {
            this.sets[items[customItemSlot][item].setId] = this.sets[items[customItemSlot][item].setId] + 1 || 1 // Have a default value of '1' in case the set id is undefined in the sets array
          }
          // Add the item's id to its slot in this.items
          // This is required for items that are on-use or have a proc such as Band of the Eternal Sage since they check if the item's ID is equipped.
          this.items[customItemSlot + customItemSubSlot] = customItemId
          // Add stats from any gems equipped in the custom item
          if (settings.gems[customItemSlot] && settings.gems[customItemSlot][customItemId]) {
            // Boolean to keep track of whether the item's socket bonus is active or not
            let socketBonusActive = true
            for (const socket in settings.gems[customItemSlot][customItemId]) {
              if (settings.gems[customItemSlot][customItemId][socket]) {
                const socketColor = settings.gems[customItemSlot][customItemId][socket][0]
                const gemId = settings.gems[customItemSlot][customItemId][socket][1]
                // Check for meta gem
                if (customItemSlot == 'head' && gems.meta[gemId]) {
                  this.metaGemIds.push(gemId)
                }
                // Find the gem's color since it might not match the socket color
                for (const gemColor in gems) {
                  if (gems[gemColor][gemId]) {
                    // Check whether the gem color is in the array of gem colors that are valid for this socket color (e.g. checks whether a 'purple' gem is valid for a 'blue' socket)
                    if (!socketInfo[socketColor].gems.includes(gemColor)) {
                      socketBonusActive = false
                    }
                    // Add stats from the gem equipped in this socket
                    for (const stat in gems[gemColor][gemId]) {
                      if (this.stats.hasOwnProperty(stat)) {
                        this.stats[stat] += gems[gemColor][gemId][stat]
                      }
                    }
                  }
                }
              } else { // No gem equipped in this socket
                socketBonusActive = false
              }
            }
            // If the socket bonus is active then remove the socket bonus stats
            if (socketBonusActive && items[customItemSlot][item].socketBonus) {
              for (const stat in items[customItemSlot][item].socketBonus) {
                if (this.stats.hasOwnProperty(stat)) {
                  this.stats[stat] += items[customItemSlot][item].socketBonus[stat]
                } else {
                  console.log("Can't add stat '" + stat + "' to player (trying to add socket bonus)")
                }
              }
            }
          }
        }
      }
      if (this.metaGemIds.length > 1) {
        console.log(this.metaGemIds.length + ' meta gems equipped, somehow.')
      }
    }

    this.stats.health = (this.stats.health + (this.stats.stamina * this.stats.staminaModifier) * healthPerStamina) * (1 + (0.01 * settings.talents.felStamina))
    this.stats.maxMana = (this.stats.mana + (this.stats.intellect * this.stats.intellectModifier) * manaPerInt) * (1 + (0.01 * settings.talents.felIntellect))
    this.stats.shadowModifier *= (1 + (0.02 * settings.talents.shadowMastery))
    this.spellTravelTime = 1

    // The Twin Stars 2-set bonus
    if (this.sets['667'] == 2) this.stats.spellPower += 15

    // Crit chance
    this.stats.critChanceMultiplier = 1000
    if (settings.auras.powerOfTheGuardianMage) this.stats.critRating += 28 * this.simSettings.mageAtieshAmount
    this.stats.critChance = baseCritChancePercent + (this.stats.critRating / critRatingPerPercent) + settings.talents.devastation + settings.talents.backlash + settings.talents.demonicTactics
    if (settings.auras.moonkinAura) this.stats.critChance += 5
    if (settings.auras.judgementOfTheCrusader) this.stats.critChance += 3
    if (settings.auras.totemOfWrath) this.stats.critChance += (3 * settings.simSettings.totemOfWrathAmount)
    if (settings.auras.chainOfTheTwilightOwl) this.stats.critChance += 2

    // Hit chance
    this.stats.hitChanceMultiplier = 1000
    if (settings.sets['658'] >= 2) this.stats.hitRating += 35 // Mana-Etched Regalia 2-set bonus (35 hit rating)
    this.stats.extraHitChance = this.stats.hitRating / hitRatingPerPercent // hit percent from hit rating
    if (settings.auras.totemOfWrath) this.stats.extraHitChance += (3 * settings.simSettings.totemOfWrathAmount)
    if (settings.auras.inspiringPresence === true) this.stats.extraHitChance += 1
    this.stats.hitChance = Math.round(getBaseHitChance(this.level, parseInt(this.enemy.level))) // The player's chance of hitting the enemy, usually between 83% and 99%

    // Add bonus damage % from Demonic Sacrifice
    if (settings.talents.demonicSacrifice === 1 && settings.simSettings.sacrificePet == 'yes') {
      if (settings.simSettings.petChoice == PetName.IMP) {
        this.stats.fireModifier *= 1.15
      } else if (settings.simSettings.petChoice == PetName.SUCCUBUS) {
        this.stats.shadowModifier *= 1.15
      } else if (settings.simSettings.petChoice == PetName.FELGUARD) {
        this.stats.shadowModifier *= 1.1
        // todo add aura to regen mana and add Felhunter sacrifice
      }
    } else {
      // Add damage % multiplier from Master Demonologist and Soul Link
      if (settings.talents.soulLink > 0) {
        this.stats.shadowModifier *= 1.05
        this.stats.fireModifier *= 1.05
      }
      if (settings.talents.masterDemonologist > 0) {
        if (settings.simSettings.petChoice == PetName.SUCCUBUS) {
          this.stats.shadowModifier *= 1.1
          this.stats.fireModifier *= 1.1
        } else if (settings.simSettings.petChoice == PetName.FELGUARD) {
          this.stats.shadowModifier *= 1.05
          this.stats.fireModifier *= 1.05
        }
      }
    }
    // Ferocious Inspiration
    if (settings.auras.ferociousInspiration) {
      this.stats.shadowModifier *= Math.pow(1.03, settings.simSettings.ferociousInspirationAmount)
      this.stats.fireModifier *= Math.pow(1.03, settings.simSettings.ferociousInspirationAmount)
    }
    // Add % dmg modifiers from Curse of the Elements + Malediction
    if (settings.auras.curseOfTheElements) {
      this.stats.shadowModifier *= 1.1 + (0.01 * (settings.simSettings.improvedCurseOfTheElements || 0))
      this.stats.fireModifier *= 1.1 + (0.01 * (settings.simSettings.improvedCurseOfTheElements || 0))
    }
    // Add fire dmg % from Emberstorm
    if (settings.talents.emberstorm > 0) this.stats.fireModifier *= 1 + (0.02 * settings.talents.emberstorm)
    // Add spell power from Fel Armor
    if (settings.auras.felArmor) {
      this.stats.spellPower += (100 * (1 + 0.1 * this.talents.demonicAegis))
    }
    // If using a custom isb uptime % then just add to the shadow modifier % (this assumes 5/5 ISB giving 20% shadow damage)
    if (settings.simSettings.customIsbUptime == 'yes') {
      this.stats.shadowModifier *= (1 + 0.2 * (settings.simSettings.customIsbUptimeValue / 100))
    }
    // Add spell power from Improved Divine Spirit
    this.stats.spiritModifier *= (1 - (0.01 * settings.talents.demonicEmbrace))
    if (settings.auras.prayerOfSpirit && settings.simSettings.improvedDivineSpirit) {
      this.stats.spellPower += (this.stats.spirit * this.stats.spiritModifier * (0 + (settings.simSettings.improvedDivineSpirit / 20)))
    }
    // Add extra stamina from Blood Pact from Improved Imp
    if (settings.auras.bloodPact) {
      this.stats.stamina += 70 * (0.1 * settings.simSettings.improvedImp)
    }
    // Add stamina from Demonic Embrace
    this.stats.stamina *= 1 + (0.03 * this.talents.demonicEmbrace)
    // Add mp5 from Vampiric Touch (add 25% instead of 5% since we're adding it to the mana per 5 seconds variable)
    if (settings.auras.vampiricTouch) {
      this.stats.mp5 += settings.simSettings.shadowPriestDps * 0.25
    }
    if (settings.auras.powerOfTheGuardianWarlock) {
      this.stats.spellPower += 33 * settings.simSettings.warlockAtieshAmount
    }

    // Enemy Armor Reduction
    if (settings.auras.faerieFire) this.enemy.armor -= 610
    if ((settings.auras.sunderArmor && settings.auras.exposeArmor && settings.simSettings.improvedExposeArmor == 2) || (settings.auras.exposeArmor && !settings.auras.sunderArmor)) this.enemy.armor -= 2050 * (1 + 0.25 * settings.simSettings.improvedExposeArmor)
    else if (settings.auras.sunderArmor) this.enemy.armor -= 520 * 5
    if (settings.auras.curseOfRecklessness) this.enemy.armor -= 800
    if (settings.auras.annihilator) this.enemy.armor -= 600
    this.enemy.armor = Math.max(0, this.enemy.armor)

    // Assign the filler spell.
    this.filler = null
    for (const spell in settings.rotation.filler) {
      if (settings.rotation.filler[spell]) {
        this.filler = spell
        break
      }
    }
    if (this.filler == null) {
      this.filler = 'shadowBolt'
    }

    // Assign the curse (if selected)
    this.curse = null
    for (const spell in settings.rotation.curse) {
      if (settings.rotation.curse[spell]) {
        this.curse = spell
        break
      }
    }

    // Records all information about damage done for each spell such as crit %, miss %, average damage per cast etc.
    this.damageBreakdown = {}
    // Records all information about auras such as how often it was applied and the uptime %.
    this.auraBreakdown = {}
    // Records all information about mana gain abilities like Life Tap, Mana Pots, and Demonic Runes
    this.manaGainBreakdown = { mp5: { name: 'Mp5' } }
    if (this.selectedAuras.judgementOfWisdom) this.manaGainBreakdown.judgementOfWisdom = { name: 'Judgement of Wisdom' }

    // Pet
    this.demonicKnowledgeSp = 0 // Spell Power from the Demonic Knowledge talent
    if (settings.simSettings.sacrificePet == 'no' || settings.talents.demonicSacrifice == 0) {
      const selectedPet = settings.simSettings.petChoice
      if (selectedPet == PetName.IMP) this.pet = new Imp(this, settings)
      else if (selectedPet == PetName.VOIDWALKER) this.pet = new Voidwalker(this, settings)
      else if (selectedPet == PetName.SUCCUBUS) this.pet = new Succubus(this, settings)
      else if (selectedPet == PetName.FELHUNTER) this.pet = new Felhunter(this, settings)
      else if (selectedPet == PetName.FELGUARD && settings.talents.summonFelguard > 0) this.pet = new Felguard(this, settings)
    }

    this.combatlog.push('---------------- Player stats ----------------')
    this.combatlog.push('Health: ' + Math.round(this.stats.health))
    this.combatlog.push('Mana: ' + Math.round(this.stats.maxMana))
    this.combatlog.push('Stamina: ' + Math.round(this.stats.stamina * this.stats.staminaModifier))
    this.combatlog.push('Intellect: ' + Math.round(this.stats.intellect * this.stats.intellectModifier))
    this.combatlog.push('Spell Power: ' + this.getSpellPower())
    this.combatlog.push('Shadow Power: ' + this.stats.shadowPower)
    this.combatlog.push('Fire Power: ' + this.stats.firePower)
    this.combatlog.push('Crit Chance: ' + Math.round(this.getCritChance('destruction') * 100) / 100 + '%')
    this.combatlog.push('Hit Chance: ' + Math.min(16, Math.round((this.stats.extraHitChance) * 100) / 100) + '%')
    this.combatlog.push('Haste: ' + Math.round((this.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 + '%')
    this.combatlog.push('Shadow Modifier: ' + Math.round(this.stats.shadowModifier * 100) + '%')
    this.combatlog.push('Fire Modifier: ' + Math.round(this.stats.fireModifier * 100) + '%')
    this.combatlog.push('MP5: ' + this.stats.mp5)
    this.combatlog.push('Spell Penetration: ' + this.stats.spellPen)
    if (this.pet) {
      let petAp = this.pet.stats.ap * this.pet.stats.apModifier
      // Divide away the hidden 10% Felguard attack power bonus for the combat log to avoid confusion
      if (this.pet.pet == PetName.FELGUARD) {
        petAp /= 1.1
      } else if (this.pet.pet == PetName.SUCCUBUS) {
        petAp /= 1.05
      }
      this.combatlog.push('---------------- Pet stats ----------------')
      this.combatlog.push('Stamina: ' + Math.round(this.pet.stats.stamina * this.pet.stats.staminaModifier))
      this.combatlog.push('Intellect: ' + Math.round(this.pet.stats.intellect * this.pet.stats.intellectModifier))
      this.combatlog.push('Strength: ' + Math.round((this.pet.stats.baseStats.strength + this.pet.stats.buffs.strength) * this.pet.stats.strengthModifier))
      this.combatlog.push('Agility: ' + Math.round(this.pet.stats.agility * this.pet.stats.agilityModifier))
      this.combatlog.push('Spirit: ' + Math.round((this.pet.stats.baseStats.spirit + this.pet.stats.spirit) * this.pet.stats.spiritModifier))
      this.combatlog.push('Attack Power: ' + Math.round(petAp))
      this.combatlog.push('Spell Power: ' + Math.round(this.pet.stats.spellPower))
      this.combatlog.push('Mana: ' + Math.round(this.pet.stats.maxMana))
      this.combatlog.push('MP5: ' + Math.round(this.pet.stats.mp5))
      if (this.pet.pet !== PetName.IMP) {
        this.combatlog.push('Physical Hit Chance: ' + Math.round(this.pet.getMeleeHitChance() * 100) / 100 + '%')
        this.combatlog.push('Physical Crit Chance: ' + Math.round(this.pet.getMeleeCritChance() * 100) / 100 + '% (' + this.pet.critSuppression + '% Crit Suppression Applied)')
        this.combatlog.push('Glancing Blow Chance: ' + Math.round(this.pet.glancingBlowChance * 100) / 100 + '%')
      }
      if (this.pet.pet === PetName.IMP || this.pet.pet === PetName.SUCCUBUS) {
        this.combatlog.push('Spell Hit Chance: ' + Math.round(this.pet.getSpellHitChance() * 100) / 100 + '%')
        this.combatlog.push('Spell Crit Chance: ' + Math.round(this.pet.getSpellCritChance() * 100) / 100 + '%')
      }
      this.combatlog.push('Damage Modifier: ' + Math.round(this.pet.stats.damageModifier * 100) + '%')
    }
    this.combatlog.push('---------------- Enemy stats ----------------')
    this.combatlog.push('Level: ' + this.enemy.level)
    this.combatlog.push('Shadow Resistance: ' + this.enemy.shadowResist)
    this.combatlog.push('Fire Resistance: ' + this.enemy.fireResist)
    if (this.pet && this.pet.pet != PetName.IMP) {
      this.combatlog.push('Dodge Chance: ' + this.pet.enemyDodgeChance + '%')
      this.combatlog.push('Armor: ' + this.enemy.armor)
      this.combatlog.push('Damage Reduction From Armor: ' + Math.round((1 - this.pet.armorMultiplier) * 10000) / 100 + '%')
    }
    this.combatlog.push('---------------------------------------------')
  }

  initialize () {
    // Trinkets
    this.trinkets = []
    this.trinketIds = [this.items.trinket1, this.items.trinket2]
    if (this.trinketIds.includes(32483)) this.trinkets.push(new SkullOfGuldan(this))
    if (this.trinketIds.includes(34429)) this.trinkets.push(new ShiftingNaaruSliver(this))
    if (this.trinketIds.includes(33829)) this.trinkets.push(new HexShrunkenHead(this))
    if (this.trinketIds.includes(29370)) this.trinkets.push(new IconOfTheSilverCrescent(this))
    if (this.trinketIds.includes(29132)) this.trinkets.push(new ScryersBloodgem(this))
    if (this.trinketIds.includes(23046)) this.trinkets.push(new RestrainedEssenceOfSapphiron(this))
    if (this.trinketIds.includes(29179)) this.trinkets.push(new XirisGift(this))
    if (this.trinketIds.includes(25620)) this.trinkets.push(new AncientCrystalTalisman(this))
    if (this.trinketIds.includes(28223)) this.trinkets.push(new ArcanistsStone(this))
    if (this.trinketIds.includes(25936)) this.trinkets.push(new TerokkarTabletOfVim(this))
    if (this.trinketIds.includes(28040)) this.trinkets.push(new VengeanceOfTheIllidari(this))
    if (this.trinketIds.includes(24126)) this.trinkets.push(new FigurineLivingRubySerpent(this))
    if (this.trinketIds.includes(29376)) this.trinkets.push(new EssenceOfTheMartyr(this))
    if (this.trinketIds.includes(30340)) this.trinkets.push(new StarkillersBauble(this))

    // Spells
    this.spells = {
      lifeTap: new LifeTap(this)
    }

    if (this.simSettings.fightType == "aoe") {
      this.spells.seedOfCorruption = new SeedOfCorruption(this)
    } else {
      if (this.rotation.filler.shadowBolt || this.filler == 'shadowBolt' || this.talents.nightfall > 0 || this.simChoosingRotation) this.spells.shadowBolt = new ShadowBolt(this)
      if (this.rotation.filler.incinerate  || this.simChoosingRotation) this.spells.incinerate = new Incinerate(this)
      if (this.rotation.filler.searingPain  || this.simChoosingRotation) this.spells.searingPain = new SearingPain(this)
      if (this.rotation.dot.corruption  || this.simChoosingRotation) this.spells.corruption = new Corruption(this)
      if (this.talents.unstableAffliction && (this.rotation.dot.unstableAffliction || this.simChoosingRotation)) this.spells.unstableAffliction = new UnstableAffliction(this)
      if (this.talents.siphonLife && (this.rotation.dot.siphonLife || this.simChoosingRotation)) this.spells.siphonLife = new SiphonLife(this)
      if (this.rotation.dot.immolate  || this.simChoosingRotation) this.spells.immolate = new Immolate(this)
      if (this.rotation.curse.curseOfAgony || this.rotation.curse.curseOfDoom) this.spells.curseOfAgony = new CurseOfAgony(this)
      if (this.rotation.curse.curseOfTheElements) this.spells.curseOfTheElements = new CurseOfTheElements(this)
      if (this.rotation.curse.curseOfRecklessness) this.spells.curseOfRecklessness = new CurseOfRecklessness(this)
      if (this.rotation.curse.curseOfDoom) this.spells.curseOfDoom = new CurseOfDoom(this)
      if (this.talents.conflagrate == 1 && (this.rotation.finisher.conflagrate || this.simChoosingRotation)) this.spells.conflagrate = new Conflagrate(this)
      if (this.talents.shadowburn > 0 && (this.rotation.finisher.shadowburn || this.simChoosingRotation)) this.spells.shadowburn = new Shadowburn(this)
      if (this.rotation.finisher.deathCoil || this.simChoosingRotation) this.spells.deathCoil = new DeathCoil(this)
      if (this.talents.shadowfury == 1 && (this.rotation.other.shadowfury || this.simChoosingRotation)) this.spells.shadowfury = new Shadowfury(this)
      if (this.talents.amplifyCurse == 1 && (this.rotation.other.amplifyCurse || this.simChoosingRotation)) this.spells.amplifyCurse = new AmplifyCurse(this)
    }

    if (this.talents.darkPact == 1 && (this.rotation.other.darkPact || this.simChoosingRotation)) this.spells.darkPact = new DarkPact(this)
    if (this.selectedAuras.destructionPotion) this.spells.destructionPotion = new DestructionPotion(this)
    if (this.selectedAuras.superManaPotion) this.spells.superManaPotion = new SuperManaPotion(this)
    if (this.selectedAuras.demonicRune) this.spells.demonicRune = new DemonicRune(this)
    if (this.selectedAuras.flameCap) this.spells.flameCap = new FlameCap(this)
    if (this.simSettings.race == 'orc') this.spells.bloodFury = new BloodFury(this)
    if (this.selectedAuras.drumsOfBattle) this.spells.drumsOfBattle = new DrumsOfBattle(this)
    else if (this.selectedAuras.drumsOfWar) this.spells.drumsOfWar = new DrumsOfWar(this)
    else if (this.selectedAuras.drumsOfRestoration) this.spells.drumsOfRestoration = new DrumsOfRestoration(this)
    if (this.items.mainhand == 31336) this.spells.bladeOfWizardry = new BladeOfWizardry(this)
    if (this.items.neck == 34678) this.spells.shatteredSunPendantOfAcumen = new ShatteredSunPendantOfAcumen(this)
    if (this.items.chest == 28602) this.spells.robeOfTheElderScribes = new RobeOfTheElderScribes(this)
    if (this.metaGemIds.includes(25893)) this.spells.mysticalSkyfireDiamond = new MysticalSkyfireDiamond(this)
    if (this.metaGemIds.includes(25901)) this.spells.insightfulEarthstormDiamond = new InsightfulEarthstormDiamond(this)
    if (this.trinketIds.includes(34470)) this.spells.timbalsFocusingCrystal = new TimbalsFocusingCrystal(this)
    if (this.trinketIds.includes(27922)) this.spells.markOfDefiance = new MarkOfDefiance(this)
    if (this.trinketIds.includes(28785)) this.spells.theLightningCapacitor = new TheLightningCapacitor(this)
    if (this.trinketIds.includes(27683)) this.spells.quagmirransEye = new QuagmirransEye(this)
    if (this.trinketIds.includes(28418)) this.spells.shiffarsNexusHorn = new ShiffarsNexusHorn(this)
    if (this.trinketIds.includes(30626)) this.spells.sextantOfUnstableCurrents = new SextantOfUnstableCurrents(this)
    if ([this.items.ring1, this.items.ring2].includes(29305)) this.spells.bandOfTheEternalSage = new BandOfTheEternalSage(this)
    if (this.selectedAuras.powerInfusion) {
      this.spells.powerInfusion = []
      for (let i = 0; i < this.simSettings.powerInfusionAmount; i++) {
        this.spells.powerInfusion.push(new PowerInfusion(this))
      }
    }
    if (this.selectedAuras.bloodlust) {
      this.spells.bloodlust = []
      for (let i = 0; i < this.simSettings.bloodlustAmount; i++) {
        this.spells.bloodlust.push(new Bloodlust(this))
      }
    }
    if (this.selectedAuras.innervate) {
      this.spells.innervate = []
      for (let i = 0; i < this.simSettings.innervateAmount; i++) {
        this.spells.innervate.push(new Innervate(this))
      }
    }

    // Auras
    this.auras = {}
    
    if (this.simSettings.fightType == "singleTarget") {
      if (this.talents.improvedShadowBolt > 0) this.auras.improvedShadowBolt = new ImprovedShadowBolt(this)
      if (this.spells.corruption) this.auras.corruption = new CorruptionDot(this)
      if (this.spells.unstableAffliction) this.auras.unstableAffliction = new UnstableAfflictionDot(this)
      if (this.spells.siphonLife) this.auras.siphonLife = new SiphonLifeDot(this)
      if (this.spells.immolate) this.auras.immolate = new ImmolateDot(this)
      if (this.spells.curseOfAgony) this.auras.curseOfAgony = new CurseOfAgonyDot(this)
      if (this.rotation.curse.curseOfTheElements) this.auras.curseOfTheElements = new CurseOfTheElementsAura(this)
      if (this.rotation.curse.curseOfRecklessness) this.auras.curseOfRecklessness = new CurseOfRecklessnessAura(this)
      if (this.rotation.curse.curseOfDoom) this.auras.curseOfDoom = new CurseOfDoomDot(this)
      if (this.talents.nightfall > 0) this.auras.shadowTrance = new ShadowTrance(this)
      if (this.spells.amplifyCurse) this.auras.amplifyCurse = new AmplifyCurseAura(this)
    }

    if (this.selectedAuras.powerInfusion) this.auras.powerInfusion = new PowerInfusionAura(this)
    if (this.selectedAuras.innervate) this.auras.innervate = new InnervateAura(this)
    if (this.simSettings.race == 'orc') this.auras.bloodFury = new BloodFuryAura(this)
    if (this.selectedAuras.destructionPotion) this.auras.destructionPotion = new DestructionPotionAura(this)
    if (this.selectedAuras.flameCap) this.auras.flameCap = new FlameCapAura(this)
    if (this.selectedAuras.bloodlust) this.auras.bloodlust = new BloodlustAura(this)
    if (this.selectedAuras.drumsOfBattle) this.auras.drumsOfBattle = new DrumsOfBattleAura(this)
    else if (this.selectedAuras.drumsOfWar) this.auras.drumsOfWar = new DrumsOfWarAura(this)
    else if (this.selectedAuras.drumsOfRestoration) this.auras.drumsOfRestoration = new DrumsOfRestorationAura(this)
    if ([this.items.ring1, this.items.ring2].includes(29305)) this.auras.bandOfTheEternalSage = new BandOfTheEternalSageAura(this)
    if ([this.items.ring1, this.items.ring2].includes(21190)) this.auras.wrathOfCenarius = new WrathOfScenarius(this)
    if (this.items.mainhand == 31336) this.auras.bladeOfWizardry = new BladeOfWizardryAura(this)
    if (this.items.neck == 34678 && this.shattrathFaction == 'Aldor') this.auras.shatteredSunPendantOfAcumen = new ShatteredSunPendantOfAcumenAura(this)
    if (this.items.chest == 28602) this.auras.robeOfTheElderScribes = new RobeOfTheElderScribesAura(this)
    if (this.metaGemIds.includes(25893)) this.auras.mysticalSkyfireDiamond = new MysticalSkyfireDiamondAura(this)
    if (this.trinketIds.includes(28789)) this.auras.eyeOfMagtheridon = new EyeOfMagtheridon(this)
    if (this.trinketIds.includes(30626)) this.auras.sextantOfUnstableCurrents = new SextantOfUnstableCurrentsAura(this)
    if (this.trinketIds.includes(27683)) this.auras.quagmirransEye = new QuagmirransEyeAura(this)
    if (this.trinketIds.includes(28418)) this.auras.shiffarsNexusHorn = new ShiffarsNexusHornAura(this)
    if (this.trinketIds.includes(32493)) this.auras.ashtongueTalismanOfShadows = new AshtongueTalismanOfShadows(this)
    if (this.trinketIds.includes(31856)) this.auras.darkmoonCardCrusade = new DarkmoonCardCrusadeAura(this)
    if (this.trinketIds.includes(28785)) this.auras.theLightningCapacitor = new TheLightningCapacitorAura(this)
    if (this.sets['645'] >= 2) {
      this.auras.flameshadow = new Flameshadow(this)
      this.auras.shadowflame = new Shadowflame(this)
    }
    if (this.sets['559'] == 2) this.auras.spellstrikeProc = new SpellstrikeProc(this)
    if (this.sets['658'] >= 4) this.auras.manaEtched4Set = new ManaEtched4Set(this)
  }

  reset () {
    this.castTimeRemaining = 0
    this.gcdRemaining = 0
    this.mana = this.stats.maxMana
    this.mp5Timer = 5
    this.fiveSecondRuleTimer = 5
    this.importantAuraCounter = 0
  }

  cast (spell, predictedDamage = 0) {
    if (this.gcdRemaining > 0 && this.spells[spell].onGcd)
      throw "Attempting to cast a spell (" + spell + ") on GCD with " + this.gcdRemaining + " cooldown remaining."
    this.spells[spell].startCast(predictedDamage)
  }

  areAnyCooldownsReady () {
    if (this.spells.bloodlust && !this.auras.bloodlust.active) {
      for (let i = 0; i < this.spells.bloodlust.length; i++) {
        if (this.spells.bloodlust[i].ready()) {
          return true
        }
      }
    }
    if (this.auras.powerInfusion && !this.auras.powerInfusion.active) {
      for (let i = 0; i < this.spells.powerInfusion.length; i++) {
        if (this.spells.powerInfusion[i].ready()) {
          return true
        }
      }
    }
    if (this.auras.innervate && !this.auras.innervate.active) {
      for (let i = 0; i < this.spells.innervate.length; i++) {
        if (this.spells.innervate[i].ready()) {
          return true
        }
      }
    }
    if (this.spells.destructionPotion && this.spells.destructionPotion.ready()) return true
    for (let i = 0; i < this.trinkets.length; i++) {
      if (this.trinkets[i].ready()) return true
    }
    if (this.spells.bloodFury && this.spells.bloodFury.ready()) return true

    return false
  }

  useCooldowns () {
    if (this.spells.bloodlust && !this.auras.bloodlust.active) {
      for (let i = 0; i < this.spells.bloodlust.length; i++) {
        if (this.spells.bloodlust[i].ready()) {
          this.spells.bloodlust[i].startCast()
          break
        }
      }
    }
    if (this.spells.powerInfusion && !this.auras.powerInfusion.active) {
      for (let i = 0; i < this.spells.powerInfusion.length; i++) {
        if (this.spells.powerInfusion[i].ready()) {
          this.spells.powerInfusion[i].startCast()
          break
        }
      }
    }
    if (this.spells.innervate && !this.auras.innervate.active) {
      for (let i = 0; i < this.spells.innervate.length; i++) {
        if (this.spells.innervate[i].ready()) {
          this.spells.innervate[i].startCast()
          break
        }
      }
    }
    if (this.spells.destructionPotion && this.spells.destructionPotion.ready()) {
      this.cast('destructionPotion')
    }
    if (this.spells.flameCap && this.spells.flameCap.ready()) {
      this.cast('flameCap')
    }
    if (this.spells.bloodFury && this.spells.bloodFury.ready()) {
      this.cast('bloodFury')
    }
    for (let i = 0; i < this.trinkets.length; i++) {
      if (this.trinkets[i] && this.trinkets[i].ready()) {
        this.trinkets[i].use()
        // Set the other on-use trinket (if another is equipped) on cooldown for the duration of the trinket just used if the trinkets share cooldown
        const otherTrinketSlot = i == 1 ? 2 : 1
        if (this.trinkets[otherTrinketSlot] && this.trinkets[otherTrinketSlot].sharesCooldown && this.trinkets[i].sharesCooldown) {
          this.trinkets[otherTrinketSlot].cooldownRemaining = Math.max(this.trinkets[otherTrinketSlot].cooldownRemaining, this.trinkets[i].duration)
        }
      }
    }
  }

  castLifeTapOrDarkPact() {
    if (this.spells.darkPact && this.spells.darkPact.ready()) {
      this.cast('darkPact')
    } else {
      this.cast('lifeTap')
    }
  }

  getGcdValue(spellVarName = '') {
    // As far as I know, Shadowfury doesn't trigger a global cooldown
    if (!this.spells.shadowfury || this.spells.shadowfury.varName !== spellVarName) {
      return Math.max(this.minimumGcdValue, Math.round((this.gcdValue / (1 + ((this.stats.hasteRating / hasteRatingPerPercent + this.stats.hastePercent) / 100))) * 10000) / 10000)
    }
    return 0
  }

  getHitChance(isAfflictionSpell) {
    let hitChance = this.stats.hitChance + this.stats.extraHitChance
    if (isAfflictionSpell) {
      hitChance += this.talents.suppression * 2
    }
    return Math.min(99, hitChance)
  }

  isHit (isAfflictionSpell) {
    const hit = (random(1, 100 * this.stats.hitChanceMultiplier) <= this.getHitChance(isAfflictionSpell) * this.stats.hitChanceMultiplier)

    // Eye of Magtheridon
    if (!hit && this.trinketIds.includes(28789)) {
      this.auras.eyeOfMagtheridon.apply()
    }

    return hit
  }

  getSpellPower() {
    let spellPower = this.stats.spellPower + this.demonicKnowledgeSp
    // Spellfire 3-set bonus
    if (this.sets['552'] == 3) {
      spellPower += this.stats.intellect * this.stats.intellectModifier * 0.07
    }
    return Math.round(spellPower)
  }

  // Returns the crit percentage of the player.
  // Since crit gains a bonus from intellect, and intellect could fluctuate during the fight (through procs and such), it's better to calculate it by calling a function like this.
  getCritChance (spellType) {
    let critChance = this.stats.critChance + ((this.stats.intellect * this.stats.intellectModifier) * critPerInt)
    
    // Remove the 5% crit chance from Devastation if it isn't a Destruction spell
    if (spellType !== 'destruction') {
      critChance -= this.talents.devastation
    }

    return critChance
  }

  isCrit (spellType, extraCrit = 0) {
    return (random(1, (100 * this.stats.critChanceMultiplier)) <= ((this.getCritChance(spellType) + extraCrit) * this.stats.critChanceMultiplier))
  }

  // The formula is (75 * resistance) / (playerLevel * 5) which gives the number to multiply the damage with (between 0 and 1) to simulate the average partial resist mitigation.
  getPartialResistMultiplier (resist) {
    return 1 - ((75 * resist) / (this.level * 5)) / 100
  }

  combatLog (info) {
    if (this.iteration == 2) {
      this.combatlog.push('|' + (Math.round(this.fightTime * 10000) / 10000).toFixed(4) + '| ' + info)
    }
  }
}
