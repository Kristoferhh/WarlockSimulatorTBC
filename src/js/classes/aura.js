class Aura {
	constructor(player) {
		this.durationTotal = 0;
		this.durationRemaining = 0;
		this.player = player;
		this.active = false;
	}

	tick(t) {
		if (this.active) {
			this.durationRemaining = Math.max(0, this.durationRemaining - t);

			if (this.durationRemaining == 0) {
				this.active = false;
			}
		}
	}

	apply() {
		this.durationRemaining = this.durationTotal;
		this.active = true;
	}
}

class ImprovedShadowBolt extends Aura {
	constructor(player) {
		super(player);
		this.name = "Improved Shadow Bolt";
		this.durationTotal = 12;
		this.stacks = 0;
		this.maxStacks = 4;
		this.modifier = 1 + (this.player.talents.improvedShadowBolt * 0.04);
	}

	apply() {
		if (this.active) this.player.combatLog(this.name + " refreshed");
		else this.player.combatLog(this.name + " applied");
		super.apply();
		this.stacks = this.maxStacks;
	}

	decrementStacks() {
		this.stacks--;

		if (this.stacks <= 0) {
			this.active = false;
			this.player.combatLog(this.name + " faded");
		} else {
			this.player.combatLog(this.name + "(" + this.stacks + ")")
		}
	}
}

class CorruptionDot extends Aura {
	constructor(player) {
		super(player);
		this.player = player;
		this.name = "Corruption";
		this.durationTotal = 18;
		this.tickTimerTotal = 3;
		this.tickTimerRemaining = 0;
		this.ticksTotal = this.durationTotal / this.tickTimerTotal; // The amount of times Corruption ticks over the course of its duration
		this.ticksRemaining = 0;
		this.spellPower = 0;
		this.coefficient = 0.936 + (0.12 * player.talents.empoweredCorruption);
		this.dmg = 1035;
		this.modifier = 1 + (0.01 * player.talents.contagion);
	}

	apply(spellPower) {
		this.active = true;
		this.tickTimerRemaining = this.tickTimerTotal;
		this.ticksRemaining = this.ticksTotal;
		this.spellPower = spellPower;
	}

	tick(t) {
		if (this.active) {
			this.tickTimerRemaining = Math.max(0,this.tickTimerRemaining - t);

			if (this.tickTimerRemaining == 0) {
				let dmg = ((this.dmg + this.spellPower * this.coefficient) * this.modifier * this.player.stats.shadowModifier) / this.ticksTotal;
				// Add bonus from ISB (without removing ISB stacks since it's a dot)
				if (this.player.talents.improvedShadowBolt > 0 && this.player.auras.improvedShadowBolt.active) {
					dmg *= this.player.auras.improvedShadowBolt.modifier;
				}

				this.player.iterationDamage += dmg;
				this.player.combatLog(this.name + " " + Math.round(dmg));
				this.ticksRemaining--;
				this.tickTimerRemaining = this.tickTimerTotal;

				if (this.ticksRemaining <= 0) {
					this.active = false;
				}
			}
		}
	}
}