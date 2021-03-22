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

	fade(endOfIteration = false) {
		if (this.active) {
			this.active = false;
			if (!endOfIteration) this.player.combatLog(this.name + " faded");
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

	fade() {
		this.stacks = 0;
		super.fade();
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

class ShadowFlameShadow extends Aura {
	constructor(player) {
		super(player);
		this.name = "Shadow Flame (shadow) (T4 2pc bonus)";
		this.durationTotal = 15;
	}

	apply() {
		if (!this.active) {
			this.player.stats.shadowPower += 135;
			super.apply();
		}
	}

	fade() {
		if (this.active) {
			this.player.stats.shadowPower -= 135;
		}

		super.fade();
	}
}

class ShadowFlameFire extends Aura {
	constructor(player) {
		super(player);
		this.name = "Shadow Flame (fire) (T4 2pc bonus)";
		this.durationTotal = 15;
	}

	apply() {
		if (!this.active) {
			this.player.stats.firePower += 135;
			super.apply();
		}
	}

	fade() {
		if (this.active) {
			this.player.stats.firePower -= 135;
		}
		super.fade();
	}
}

class SpellstrikeProc extends Aura {
	constructor(player) {
		super(player);
		this.name = "Spellstrike Proc";
		this.durationTotal = 10;
	}

	apply() {
		if (!this.active) {
			this.player.stats.spellPower += 92;
			super.apply();
		}
	}

	fade() {
		if (this.active) {
			this.player.stats.spellPower -= 92;
		}
		super.fade();
	}
}

class PowerInfusion extends Aura {
	constructor(player) {
		super(player);
		this.name = "Power Infusion";
		this.durationTotal = 15;
		this.cooldown = 180;
		this.cooldownRemaining = 0;
	}

	apply() {
		if (!this.active) {
			this.player.stats.shadowModifier *= 1.2;
			this.player.stats.fireModifier *= 1.2;
			this.player.stats.manaCostModifier *= 0.8;
		}
		this.cooldownRemaining = this.cooldown;
		super.apply();
	}

	fade() {
		if (this.active) {
			this.player.stats.shadowModifier /= 1.2;
			this.player.stats.fireModifier /= 1.2;
			this.player.stats.manaCostModifier /= 0.8;
		}
		super.fade();
	}

	tick(t) {
		this.cooldownRemaining = Math.max(0,this.cooldownRemaining - t);
		super.tick(t);
	}

	ready() {
		return this.cooldownRemaining <= 0;
	}
}