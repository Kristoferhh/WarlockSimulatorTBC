class Aura {
	constructor(player) {
		this.durationTotal = 0;
		this.durationRemaining = 0;
		this.player = player;
		this.active = false;
	}

	tick(t) {
		this.durationRemaining = Math.max(0, this.durationRemaining - t);

		if (this.durationRemaining == 0) {
			this.fade();
		}
	}

	fade() {
		if (this.active) {
			this.active = false;
			this.player.combatLog(this.name + " faded");
		}
	}

	apply() {
		if (this.active) this.player.combatLog(this.name + " refreshed");
		else this.player.combatLog(this.name + " applied");
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

class CurseOfTheElementsAura extends Aura {
	constructor(player) {
		super(player);
		this.name = "Curse of the Elements";
		this.durationTotal = 300;
	}
}

class CurseOfRecklessnessAura extends Aura {
	constructor(player) {
		super(player);
		this.name = "Curse of Recklessness";
		this.durationTotal = 120;
	}
}

class CurseOfDoomAura extends Aura {
	constructor(player) {
		super(player);
		this.name = "Curse of Doom";
		this.durationTotal = 60;
	}
}

class ShadowTrance extends Aura {
	constructor(player) {
		super(player);
		this.name = "Shadow Trance (Nightfall)";
		this.durationTotal = 10;
	}
}