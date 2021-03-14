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

		for (let spell in this.player.spells) {
			let damage = this.player.spells[spell].tick(time);
			if (damage > 0) {
				this.iterationDamage += damage;
				console.log(spell + " " + damage);
			}
		}

		return time;
	}

	start() {
		this.totalDamage = 0;
		this.totalDuration = 0;
		this.startTime = new Date();

		console.log("------- Simualtion start -------");
		for(var iteration = 1; iteration <= this.iterations; iteration++) {
			this.player.initialize();
			this.iterationDamage = 0;
			let fightLength = this.player.random(this.minTime, this.maxTime);

			for(var i = 0; i < fightLength; i += this.passTime()) {
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

		alert(this.totalDamage / this.totalDuration);
		console.log('------- Simulation end -------');
	}
}