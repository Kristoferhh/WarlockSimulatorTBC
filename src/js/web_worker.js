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
    module._startSimulation()
  })

  const player = new Player(e.data.player, e.data.itemSlot || null, e.data.itemSubSlot || '', e.data.itemId || null, e.data.customStat || null, e.data.customStatValue || 0)
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
  simulation.start()
}
