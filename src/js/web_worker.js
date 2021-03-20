importScripts(
	'./classes/player.js',
	'./player/stats.js',
	'./classes/simulation.js',
	'./classes/spell.js',
	'./functions.js',
	'./classes/aura.js',
	'./classes/damageOverTime.js'
);

onmessage = function(e) {
	var player = new Player(e.data.player);
	var simulation = new Simulation(player, e.data.simulation,
		(simulationEnd) => {
			this.postMessage({
				"event": "end",
				"data": simulationEnd
			});
		},
		(simulationUpdate) => {
			this.postMessage({
				"event": "update",
				"data": simulationUpdate
			});
		}
	);
	simulation.start();
}