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
	var simulation = new Simulation(player, e.data.simulation, (simResult) => {
		this.postMessage({
			"event": "end",
			"data": simResult
		});
	});
	simulation.start();
}