importScripts(
  './items/gems.js',
  './items/items.js',
  './WarlockSim.js'
)

onmessage = function (e) {
  fetch('./WarlockSim.wasm')
  .then(response => response.arrayBuffer())
  .then(binary => WarlockSim({ wasmBinary: binary }))
  .then(w => w.ready)
  .then(module => {
    let a = e.data.player.auras
    let t = e.data.player.talents
    let s = e.data.player.sets
    let b = e.data.player.stats
    let c = e.data.player.simSettings
    let d = e.data.player.rotation
    let i = e.data.player.items

    let customItemSlot = e.data.itemSlot
    let customItemSubSlot = e.data.itemSubSlot
    let customItemId = e.data.itemId

    let metaGemIds = []
    // If the player is equipped with a custom item then remove the stats from the currently equipped item and add stats from the custom item
    if (customItemSlot && customItemId && customItemId !== i[customItemSlot + customItemSubSlot]) {
      // Loop through all items in the custom item slot
      for (const item in items[customItemSlot]) {
        // Check if this is the currently equipped item
        if (items[customItemSlot][item].id == i[customItemSlot + customItemSubSlot]) {
          // Remove stats from currently equipped item
          for (const stat in items[customItemSlot][item]) {
            if (b.hasOwnProperty(stat)) {
              b[stat] -= items[customItemSlot][item][stat]
            }
          }
          // Decrement the counter for the set id if it is part of a set
          if (items[customItemSlot][item].hasOwnProperty('setId')) {
            s[items[customItemSlot][item].setId]--
          }
          // Remove stats from gems in the equipped item if there are any
          // Also check if the item's socket bonus is active and remove the stats from it if so
          if (e.data.player.gems[customItemSlot] && e.data.player.gems[customItemSlot][i[customItemSlot + customItemSubSlot]]) {
            let socketBonusActive = true
            // Loop through each socket in the equipped item
            for (const socket in e.data.player.gems[customItemSlot][i[customItemSlot + customItemSubSlot]]) {
              if (e.data.player.gems[customItemSlot][i[customItemSlot + customItemSubSlot]][socket]) {
                const socketColor = e.data.player.gems[customItemSlot][i[customItemSlot + customItemSubSlot]][socket][0]
                const gemId = e.data.player.gems[customItemSlot][i[customItemSlot + customItemSubSlot]][socket][1]
                // Find the gem's color since the socket and gem colors might not match
                for (const gemColor in gems) {
                  if (gems[gemColor][gemId]) {
                    // Check whether the gem color is in the array of gem colors that are valid for this socket color (e.g. checks whether a 'purple' gem is valid for a 'blue' socket)
                    if (!socketInfo[socketColor].gems.includes(gemColor)) {
                      socketBonusActive = false
                    }
                    if (gemColor == 'meta') {
                      if (metaGemIds.includes(gemId)) {
                        metaGemIds.splice(metaGemIds.indexOf(gemId), 1)
                      }
                    }
                    // Loop through the gem's stats and remove them from the player
                    for (const stat in gems[gemColor][gemId]) {
                      if (b.hasOwnProperty(stat)) {
                        b[stat] -= gems[gemColor][gemId][stat]
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
                if (b.hasOwnProperty(stat)) {
                  b[stat] -= items[customItemSlot][item].socketBonus[stat]
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
            if (b.hasOwnProperty(stat)) {
              b[stat] += items[customItemSlot][item][stat]
            }
          }
          // Increment the counter for the set id if it is part of a set
          if (items[customItemSlot][item].hasOwnProperty('setId')) {
            s[items[customItemSlot][item].setId] = s[items[customItemSlot][item].setId] + 1 || 1
          }
          // Add the item's id to its slot in e.data.player.items
          // This is required for items that are on-use or have a proc such as Band of the Eternal Sage since they check if the item's ID is equipped.
          i[customItemSlot + customItemSubSlot] = customItemId
          // Add stats from any gems equipped in the custom item
          if (e.data.player.gems[customItemSlot] && e.data.player.gems[customItemSlot][customItemId]) {
            // Boolean to keep track of whether the item's socket bonus is active or not
            let socketBonusActive = true
            for (const socket in e.data.player.gems[customItemSlot][customItemId]) {
              if (e.data.player.gems[customItemSlot][customItemId][socket]) {
                const socketColor = e.data.player.gems[customItemSlot][customItemId][socket][0]
                const gemId = e.data.player.gems[customItemSlot][customItemId][socket][1]
                // Check for meta gem
                if (customItemSlot == 'head' && gems.meta[gemId]) {
                  metaGemIds.push(gemId)
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
                      if (b.hasOwnProperty(stat)) {
                        b[stat] += gems[gemColor][gemId][stat]
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
                if (b.hasOwnProperty(stat)) {
                  b[stat] += items[customItemSlot][item].socketBonus[stat]
                } else {
                  console.log("Can't add stat '" + stat + "' to player (trying to add socket bonus)")
                }
              }
            }
          }
        }
      }
    }

    // Mamma mia somebody clean up this mess please
    // Items
    let equippedItems = module._allocItems(i.head, i.neck, i.shoulders, i.back, i.chest, i.bracers, i.gloves, i.belt, i.legs, i.boots, i.ring1, i.ring2, i.trinket1, i.trinket2, i.mainhand, i.offhand, i.twohand, i.wand)
    // Auras
    let auras = module._allocAuras(a.felArmor, a.blessingOfKings, a.blessingOfWisdom, a.judgementOfWisdom, a.manaSpringTotem, a.wrathOfAirTotem, a.totemOfWrath, a.markOfTheWild, a.arcaneIntellect
      , a.prayerOfFortitude, a.prayerOfSpirit, a.bloodPact, a.inspiringPresence, a.moonkinAura, a.powerInfusion, a.powerOfTheGuardianWarlock, a.powerOfTheGuardianMage, a.eyeOfTheNight, a.chainOfTheTwilightOwl
      , a.jadePendantOfBlasting, a.idolOfTheRavenGoddess, a.drumsOfBattle, a.drumsOfWar, a.drumsOfRestoration, a.bloodlust, a.ferociousInspiration, a.innervate, a.curseOfTheElements, a.shadowWeaving
      , a.improvedScorch, a.misery, a.judgementOfTheCrusader, a.vampiricTouch, a.flaskOfPureDeath, a.elixirOfMajorShadowPower, a.elixirOfMajorFirepower, a.greaterArcaneElixir, a.adeptsElixir
      , a.elixirOfDraenicWisdom, a.elixirOfMajorMageblood, a.superManaPotion, a.destructionPotion, a.brilliantWizardOil, a.superiorWizardOil, a.blessedWizardOil, a.demonicRune, a.flameCap, a.rumseyRumBlackLabel
      , a.kreegsStoutBeatdown, a.blackenedBasilisk, a.skullfishSoup, a.veryBerryCream, a.midsummerSausage, a.bloodthistle)
    // Talents
    let talents = module._allocTalents(t.suppression, t.improvedCorruption, t.improvedLifeTap, t.improvedCurseOfAgony, t.amplifyCurse, t.nightfall, t.empoweredCorruption, t.siphonLife, t.shadowMastery
      , t.contagion, t.darkPact, t.unstableAffliction, t.improvedImp, t.demonicEmbrace, t.felIntellect, t.felStamina, t.improvedSuccubus, t.demonicAegis, t.unholyPower, t.demonicSacrifice, t.manaFeed
      , t.masterDemonologist, t.soulLink, t.demonicKnowledge, t.demonicTactics, t.felguard, t.improvedShadowBolt, t.cataclysm, t.bane, t.improvedFirebolt, t.improvedLashOfPain, t.devastation, t.shadowburn
      , t.improvedSearingPain, t.improvedImmolate, t.ruin, t.emberstorm, t.backlash, t.conflagrate, t.shadowAndFlame, t.shadowfury)
    // Sets
    let sets = module._allocSets(s['529'], s['552'], s['553'], s['559'], s['644'], s['658'], s['667'], s['645'], s['646'], s['670'])
    // Stats
    let stats = module._allocStats(b.health, b.mana, b.stamina, b.intellect, b.spirit, b.spellPower, b.shadowPower, b.firePower, b.hasteRating, b.hitRating, b.critRating, b.critPercent, b.mp5, b.spellPen
      , b.fireModifier, b.frostModifier, b.hastePercent, b.damageModifier, b.shadowModifier, b.staminaModifier, b.intellectModifier, b.spiritModifier, b.manaCostModifier, b.arcaneModifier, b.natureModifier
      , b.natureResist, b.arcaneResist, b.fireResist, b.frostResist, b.shadowResist)
    // Player settings
    let playerSettings = module._allocPlayerSettings(auras, talents, sets, stats, equippedItems, e.data.itemId, (metaGemIds[0] || -1), c.shattrathFaction === "Aldor", parseInt(c['target-level'])
      , parseInt(c['target-shadow-resistance']), parseInt(c['target-fire-resistance']), parseInt(c.mageAtieshAmount), parseInt(c.totemOfWrathAmount), c.sacrificePet === "yes", c.petChoice === "0"
      , c.petChoice === "2", c.petChoice === "4", parseInt(c.ferociousInspirationAmount), parseInt(c.improvedCurseOfTheElements), c.customIsbUptime === "yes", parseInt(c.customIsbUptimeValue)
      , parseInt(c.improvedDivineSpirit), parseInt(c.improvedImp), parseInt(c.shadowPriestDps), parseInt(c.warlockAtieshAmount), parseInt(c.improvedExposeArmor), c.fightType === "singleTarget"
      , parseInt(c.enemyAmount), c.race === "orc", parseInt(c.powerInfusionAmount), parseInt(c.bloodlustAmount), parseInt(c.innervateAmount), parseInt(c.enemyArmor), parseInt(c.exposeWeaknessUptime)
      , c.improvedFaerieFire === "yes", c.infinitePlayerMana === "yes", c.infinitePetMana === "yes", c.lashOfPainUsage === "onCooldown", c.petMode === "1", c.prepopBlackBook === "yes"
      , c.randomizeValues === "yes", c.rotationOption === "simChooses", c.shattrathFactionReputation === "yes", parseInt(c.survivalHunterAgility), d.dot.immolate, d.dot.corruption
      , d.dot.siphonLife, d.dot.unstableAffliction, d.filler.searingPain, d.filler.shadowBolt, d.filler.incinerate, d.curse.curseOfRecklessness, d.curse.curseOfTheElements, d.curse.curseOfAgony
      , d.curse.curseOfDoom, d.finisher.deathCoil, d.finisher.shadowburn, d.finisher.conflagrate, d.other.shadowfury, d.other.amplifyCurse, d.other.darkPact)
    let player = module._allocPlayer(playerSettings)
    let simSettings = module._allocSimSettings(e.data.simulation.iterations, e.data.simulation.minTime, e.data.simulation.maxTime)
    let sim = module._allocSim(player, simSettings)
    module._startSimulation(sim)
    module._freeItems(equippedItems)
    module._freeAuras(auras)
    module._freeTalents(talents)
    module._freeSets(sets)
    module._freeStats(stats)
    module._freePlayerSettings(playerSettings)
    module._freePlayer(player)
    module._freeSimSettings(simSettings)
    module._freeSim(sim)
  })
}
