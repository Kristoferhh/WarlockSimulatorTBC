class Simulation {
	static getSettings() {
		return {
			"iterations": parseInt($("input[name='iterations']").val()),
			"minTime": parseInt($("input[name='min-fight-length']").val()),
			"maxTime": parseInt($("input[name='max-fight-length']").val())
		}
	}

	constructor(player, settings = Simulation.getSettings(), simulationEnd, simulationUpdate) {
		this.player = player;
		this.iterations = settings.iterations;
		this.minTime = settings.minTime;
		this.maxTime = settings.maxTime;
		this.simulationEnd = simulationEnd;
		this.simulationUpdate = simulationUpdate;
	}

	passTime() {
		let time = this.player.castTimeRemaining;
		if (time == 0) time = Math.max(0.1,this.player.gcdRemaining);

		if (this.player.auras.improvedShadowBolt && this.player.auras.improvedShadowBolt.active && this.player.auras.improvedShadowBolt.durationRemaining < time) time = this.player.auras.improvedShadowBolt.durationRemaining;
		if (this.player.auras.corruption && this.player.auras.corruption.active && this.player.auras.corruption.tickTimerRemaining < time) time = this.player.auras.corruption.tickTimerRemaining; 
		if (this.player.auras.unstableAffliction && this.player.auras.unstableAffliction.active && this.player.auras.unstableAffliction.tickTimerRemaining < time) time = this.player.auras.unstableAffliction.tickTimerRemaining;
		if (this.player.auras.siphonLife && this.player.auras.siphonLife.active && this.player.auras.siphonLife.tickTimerRemaining < time) time = this.player.auras.siphonLife.tickTimerRemaining;
		if (this.player.auras.immolate && this.player.auras.immolate.active && this.player.auras.immolate.tickTimerRemaining < time) time = this.player.auras.immolate.tickTimerRemaining;
		if (this.player.auras.curseOfAgony && this.player.auras.curseOfAgony.active && this.player.auras.curseOfAgony.tickTimerRemaining < time) time = this.player.auras.curseOfAgony.tickTimerRemaining;
		if (this.player.auras.curseOfTheElements && this.player.auras.curseOfTheElements.active && this.player.auras.curseOfTheElements.durationRemaining < time) time = this.player.auras.curseOfTheElements.durationRemaining;
		if (this.player.auras.curseOfRecklessness && this.player.auras.curseOfRecklessness.active && this.player.auras.curseOfRecklessness.durationRemaining < time) time = this.player.auras.curseOfRecklessness.durationRemaining;
		if (this.player.auras.shadowTrance && this.player.auras.shadowTrance.active && this.player.auras.shadowTrance.durationRemaining < time) time = this.player.auras.shadowTrance.durationRemaining;
		if (this.player.auras.shadowFlameshadow && this.player.auras.shadowFlameshadow.active && this.player.auras.shadowFlameshadow.durationRemaining < time) time = this.player.auras.shadowFlameshadow.durationRemaining;
		if (this.player.auras.shadowFlamefire && this.player.auras.shadowFlamefire.active && this.player.auras.shadowFlamefire.durationRemaining < time) time = this.player.auras.shadowFlamefire.durationRemaining;
		if (this.player.auras.spellstrikeProc && this.player.auras.spellstrikeProc.active && this.player.auras.spellstrikeProc.durationRemaining < time) time = this.player.auras.spellstrikeProc.durationRemaining;
		if (this.player.auras.powerInfusion && this.player.auras.powerInfusion.cooldownRemaining > 0 && this.player.auras.powerInfusion.cooldownRemaining < time) time = this.player.auras.powerInfusion.cooldownRemaining;
		if (this.player.auras.powerInfusion && this.player.auras.powerInfusion.active && this.player.auras.powerInfusion.durationRemaining < time) time = this.player.auras.powerInfusion.durationRemaining;
		if (this.player.mp5Timer < time) time = this.player.mp5Timer;
		for (let trinket in this.player.trinkets) {
			if (this.player.trinkets[trinket].active && this.player.trinkets[trinket].durationRemaining < time) time = this.player.trinkets[trinket].durationRemaining;
			if (this.player.trinkets[trinket].cooldownRemaining > 0 && this.player.trinkets[trinket].cooldownRemaining < time) time = this.player.trinkets[trinket].cooldownRemaining;
		}

		// This needs to be the first modified value since the time in combat needs to be updated before spells start dealing damage/auras expiring etc. for the combat logging.
		this.player.fightTime += time;
		this.player.castTimeRemaining = Math.max(0,this.player.castTimeRemaining - time);

		// Spells
		for (let spell in this.player.spells) {
			if (this.player.spells[spell].cooldownRemaining > 0 || this.player.spells[spell].casting) this.player.spells[spell].tick(time);
		}

		// Auras
		for (let aura in this.player.auras) {
			if (this.player.auras[aura].active || this.player.auras[aura].name == "Power Infusion") this.player.auras[aura].tick(time);
		}

		// Trinkets
		for (let trinket in this.player.trinkets) {
			this.player.trinkets[trinket].tick(time);
		}

		this.player.gcdRemaining = Math.max(0,this.player.gcdRemaining - time);
		this.player.mp5Timer = Math.max(0,this.player.mp5Timer - time);
		if (this.player.mp5Timer == 0) {
			this.player.mp5Timer = 5;
			if (this.player.stats.mp5 > 0) {
				this.player.mana = Math.min(this.player.stats.maxMana, this.player.mana + this.player.stats.mp5);
				this.player.combatLog(this.player.stats.mp5 + " mana gained from MP5");
			}
		}
	}

	start() {
		let totalDamage = 0;
		let totalDuration = 0;
		let minDps = 9999;
		let maxDps = 0;
		let startTime = new Date();

		console.log("------- Simualtion start -------");
		for(this.player.iteration = 1; this.player.iteration <= this.iterations; this.player.iteration++) {
			this.player.initialize();
			this.player.iterationDamage = 0;
			let fightLength = random(this.minTime, this.maxTime);

			for(this.player.fightTime = 0; this.player.fightTime < fightLength; this.passTime()) {
				if (this.player.castTimeRemaining <= 0) {
					// Spells on the global cooldown
					if (this.player.gcdRemaining <= 0) {
						let timeRemaining = fightLength - this.player.fightTime;
						// Not enough time left to cast another filler spell.
						if ((this.player.rotation.finisher.deathCoil || this.player.rotation.finisher.shadowburn) && timeRemaining <= (this.player.spells[this.player.filler].castTime + this.player.spells[this.player.filler].travelTime)) {
							// Use Power Infusion
							if (this.player.auras.powerInfusion && this.player.auras.powerInfusion.ready()) {
								this.player.auras.powerInfusion.apply();
							}
							// Cast Death Coil if there's time to cast both Death Coil and Shadowburn (need to cast Death Coil first because of the travel time). Otherwise only cast Shadowburn
							if (this.player.rotation.finisher.deathCoil && this.player.spells.deathCoil.ready() && (timeRemaining - this.player.gcdValue > this.player.spells.deathCoil.travelTime)) {
								this.player.cast("deathCoil");
							} else if (this.player.rotation.finisher.shadowburn && this.player.spells.shadowburn.ready()) {
								this.player.cast("shadowburn");
							} else {
								this.player.cast('lifeTap');
							}
						} else {
							if (this.player.curse && !this.player.auras[this.player.curse].active && this.player.spells[this.player.curse].ready()) {
								this.player.cast(this.player.curse);
							} else if (this.player.rotation.dot.unstableAffliction && !this.player.auras.unstableAffliction.active && this.player.spells.unstableAffliction.ready() && ((timeRemaining - this.player.spells.unstableAffliction.castTime) / this.player.auras.unstableAffliction.durationTotal) >= 9/this.player.auras.unstableAffliction.durationTotal) {
								this.player.cast("unstableAffliction");
							} else if (this.player.rotation.curse.curseOfAgony && !this.player.auras.curseOfAgony.active && this.player.spells.curseOfAgony.ready() && (timeRemaining / this.player.auras.curseOfAgony.durationTotal) >= 1) {
								this.player.cast("curseOfAgony");
							} else if (this.player.rotation.dot.siphonLife && !this.player.auras.siphonLife.active && this.player.spells.siphonLife.ready()  && (timeRemaining / this.player.auras.siphonLife.durationTotal) >= 1) {
								this.player.cast("siphonLife");
							} else if (this.player.rotation.dot.immolate && !this.player.auras.immolate.active && this.player.spells.immolate.ready()  && ((timeRemaining - this.player.spells.immolate.castTime) / this.player.auras.immolate.durationTotal) >= 12/this.player.auras.immolate.durationTotal) {
								this.player.cast("immolate");
							} else if (this.player.rotation.dot.corruption && !this.player.auras.corruption.active && this.player.spells.corruption.ready() && ((timeRemaining - this.player.spells.corruption.castTime) / this.player.auras.corruption.durationTotal) >= 9/this.player.auras.corruption.durationTotal) {
								this.player.cast("corruption");
							} else if (this.player.spells[this.player.filler].ready()) {
								// Use Power Infusion
								if (this.player.auras.powerInfusion && this.player.auras.powerInfusion.ready()) {
									this.player.auras.powerInfusion.apply();
								}
								for (let trinket in this.player.trinkets) {
									if (this.player.trinkets[trinket].ready()) {
										this.player.trinkets[trinket].use();
									}
								}
								this.player.cast(this.player.filler);
							} else {
								this.player.cast("lifeTap");
							}
						}
					}
				}
			}

			totalDuration += fightLength;
			totalDamage += this.player.iterationDamage;

			let iterationDps = this.player.iterationDamage / fightLength;
			if (iterationDps > maxDps) maxDps = iterationDps;
			else if (iterationDps < minDps) minDps = iterationDps;

			// Send an update to the sim worker for every 1% of progress
			if (this.player.iteration % ~~(this.iterations / 100) == 0) {
				this.simulationUpdate({
					"averageDamage": Math.round((totalDamage / totalDuration) * 100) / 100,
					"percent": Math.round((this.player.iteration / this.iterations) * 100),
					"itemID": this.player.itemID
				});
			}

			// Reset/end all active auras
			for (let aura in this.player.auras) {
				this.player.auras[aura].fade(true);
			}
		}

		console.log('------- Simulation end -------');
		this.simulationEnd({
			"minDps": Math.round(minDps * 100) / 100,
			"maxDps": Math.round(maxDps * 100) / 100,
			"length": (new Date() - startTime) / 1000,
			"damageBreakdown": this.player.damageBreakdown,
			"iterations": this.iterations,
			"totalDamage": totalDamage,
			"totalDuration": totalDuration,
			"itemID": this.player.itemID
		});
	}
}