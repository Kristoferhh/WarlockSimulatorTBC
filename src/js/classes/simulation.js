class Simulation {
	static getSettings() {
		return {
			"iterations": parseInt($("input[name='iterations']").val()),
			"minTime": parseInt($("input[name='min-fight-length']").val()),
			"maxTime": parseInt($("input[name='max-fight-length']").val())
		}
	}

	constructor(player, settings = Simulation.getSettings()) {
		this.player = player;
		this.iterations = settings.iterations;
		this.minTime = settings.minTime;
		this.maxTime = settings.maxTime;
	}

	passTime() {
		let time = Math.max(this.player.gcdRemaining, this.player.castTimeRemaining);

		this.player.fightTime += time; // This needs to be the first modified value since the time in combat needs to be updated before spells start dealing damage/auras expiring etc. for the combat logging.

		// Spells
		for (let spell in this.player.spells) {
			let damage = this.player.spells[spell].tick(time);
			if (damage > 0) {
				this.iterationDamage += damage;
			}
		}

		this.player.castTimeRemaining = Math.max(0,this.player.castTimeRemaining - time);
		this.player.gcdRemaining = Math.max(0,this.player.gcdRemaining - time);
		this.player.mp5Timer = Math.max(0,this.player.mp5Timer - time);
	}

	start() {
		this.totalDamage = 0;
		this.totalDuration = 0;
		this.startTime = new Date();

		console.log("------- Simualtion start -------");
		for(this.player.iteration = 1; this.player.iteration <= this.iterations; this.player.iteration++) {
			this.player.initialize();
			this.iterationDamage = 0;
			let fightLength = this.player.random(this.minTime, this.maxTime);

			for(this.player.fightTime = 0; this.player.fightTime < fightLength; this.passTime()) {
				if (this.player.castTimeRemaining <= 0) {
					if (this.player.gcdRemaining <= 0) {
						if (this.player.spells[this.player.filler].ready()) {
							this.player.cast(this.player.filler);
						} else {
							this.player.cast("lifeTap");
						}
					}
				}
			}

			this.totalDuration += fightLength;
			this.totalDamage += this.iterationDamage;
		}

		alert(Math.round(this.totalDamage / this.totalDuration));
		console.log('------- Simulation end -------');
	}
}