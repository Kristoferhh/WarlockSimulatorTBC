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

		if (this.stacks == 0) {
			this.active = false;
			this.player.combatLog(this.name + " faded");
		} else {
			this.player.combatLog(this.name + "(" + this.stacks + ")")
		}
	}
}