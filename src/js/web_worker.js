importScripts(
  './classes/player.js',
  './classes/pet.js',
  './player/stats.js',
  './classes/simulation.js',
  './classes/spell.js',
  './classes/petSpell.js',
  './classes/trinket.js',
  './functions.js',
  './classes/aura.js',
  './classes/petAura.js',
  './classes/damageOverTime.js',
  './items/items.js',
  './items/gems.js',
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
    let auras = module._allocAuras(a.felArmor, a.blessingOfKings, a.blessingOfWisdom, a.judgementOfWisdom, a.manaSpringTotem, a.wrathOfAirTotem, a.totemOfWrath, a.markOfTheWild, a.arcaneIntellect
      , a.prayerOfFortitude, a.prayerOfSpirit, a.bloodPact, a.inspiringPresence, a.moonkinAura, a.powerInfusion, a.powerOfTheGuardianWarlock, a.powerOfTheGuardianMage, a.eyeOfTheNight, a.chainOfTheTwilightOwl
      , a.jadePendantOfBlasting, a.idolOfTheRavenGoddess, a.drumsOfBattle, a.drumsOfWar, a.drumsOfRestoration, a.bloodlust, a.ferociousInspiration, a.innervate, a.curseOfTheElements, a.shadowWeaving
      , a.improvedScorch, a.misery, a.judgementOfTheCrusader, a.vampiricTouch, a.flaskOfPureDeath, a.elixirOfMajorShadowPower, a.elixirOfMajorFirepower, a.greaterArcaneElixir, a.adeptsElixir
      , a.elixirOfDraenicWisdom, a.elixirOfMajorMageblood, a.superManaPotion, a.destructionPotion, a.brilliantWizardOil, a.superiorWizardOil, a.blessedWizardOil, a.demonicRune, a.flameCap, a.rumseyRumBlackLabel
      , a.kreegsStoutBeatdown, a.blackenedBasilisk, a.skullfishSoup, a.veryBerryCream, a.midsummerSausage, a.bloodthistle)
    let talents = module._allocTalents(t.suppression, t.improvedCorruption, t.improvedLifeTap, t.improvedCurseOfAgony, t.amplifyCurse, t.nightfall, t.empoweredCorruption, t.siphonLife, t.shadowMastery
      , t.contagion, t.darkPact, t.unstableAffliction, t.improvedImp, t.demonicEmbrace, t.felIntellect, t.felStamina, t.improvedSuccubus, t.demonicAegis, t.unholyPower, t.demonicSacrifice, t.manaFeed
      , t.masterDemonologist, t.soulLink, t.demonicKnowledge, t.demonicTactics, t.felguard, t.improvedShadowBolt, t.cataclysm, t.bane, t.improvedFirebolt, t.improvedLashOfPain, t.devastation, t.shadowburn
      , t.improvedSearingPain, t.improvedImmolate, t.ruin, t.emberstorm, t.backlash, t.shadowAndFlame, t.shadowfury)
    let sets = module._allocSets(s['529'], s['552'], s['553'], s['559'], s['644'], s['658'], s['667'], s['645'], s['646'], s['670'])
    let stats = module._allocStats(b.health, b.mana, b.stamina, b.intellect, b.spirit, b.spellPower, b.shadowPower, b.firePower, b.hasteRating, b.hitRating, b.critRating, b.critPercent, b.mp5, b.spellPen
      , b.fireModifier, b.frostModifier, b.hastePercent, b.damageModifier, b.shadowModifier, b.staminaModifier, b.intellectModifier, b.spiritModifier, b.manaCostModifier, b.arcaneModifier, b.natureModifier
      , b.natureResist, b.arcaneResist, b.fireResist, b.frostResist, b.shadowResist)
    let playerSettings = module._allocPlayerSettings(auras, talents, sets, stats)
    let player = module._allocPlayer(playerSettings)
    let simSettings = module._allocSimSettings(e.data.simulation.iterations, e.data.simulation.minTime, e.data.simulation.maxTime)
    let sim = module._allocSim(player, simSettings)
    medianDps = module._startSimulation(sim)
    console.log(medianDps)
    module._freeAuras(auras)
    module._freeTalents(talents)
    module._freeSets(sets)
    module._freeStats(stats)
    module._freePlayerSettings(playerSettings)
    module._freePlayer(player)
    module._freeSimSettings(simSettings)
    module._freeSim(sim)
    this.postMessage({
      event: 'update',
      data: {
        medianDps: medianDps
      }
    })
  })

  /*const player = new Player(e.data.player, e.data.itemSlot || null, e.data.itemSubSlot || '', e.data.itemId || null, e.data.customStat || null, e.data.customStatValue || 0)
  const simulation = new Simulation(player, e.data.simulation,
    (simulationEnd) => {
      this.postMessage({
        event: 'end',
        data: simulationEnd
      })
    },
    (simulationUpdate) => {
      this.postMessage({
        event: 'update',
        data: simulationUpdate
      })
    }
  )
  simulation.start()*/
}
