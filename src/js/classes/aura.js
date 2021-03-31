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
			if (!endOfIteration) {
				this.player.combatLog(this.name + " faded");
			}
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

	fade(endOfIteration = false) {
		this.stacks = 0;
		super.fade(endOfIteration);
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
		this.dmg = 4200;
		this.coefficient = 2;
	}

	apply() {
		this.spellPower = this.player.stats.spellPower + this.player.demonicKnowledgeSp + this.player.stats.shadowPower;
		super.apply();
	}

	fade(endOfIteration = false) {
		if (this.active && !endOfIteration) {
			this.player.spells.curseOfDoom.damage(false);
		}
		super.fade(endOfIteration);
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
		this.procChance = 10;
	}

	apply() {
		if (!this.active) {
			this.player.stats.shadowPower += 135;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
			super.apply();
		}
	}

	fade(endOfIteration = false) {
		if (this.active) {
			this.player.stats.shadowPower -= 135;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
		}

		super.fade(endOfIteration);
	}
}

class ShadowFlameFire extends Aura {
	constructor(player) {
		super(player);
		this.name = "Shadow Flame (fire) (T4 2pc bonus)";
		this.durationTotal = 15;
		this.procChance = 10;
	}

	apply() {
		if (!this.active) {
			this.player.stats.firePower += 135;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
			super.apply();
		}
	}

	fade(endOfIteration = false) {
		if (this.active) {
			this.player.stats.firePower -= 135;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
		}
		super.fade(endOfIteration);
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
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
			super.apply();
		}
	}

	fade(endOfIteration = false) {
		if (this.active) {
			this.player.stats.spellPower -= 92;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
		}
		super.fade(endOfIteration);
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

	reset() {
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

	fade(endOfIteration = false) {
		if (this.active) {
			this.player.stats.shadowModifier /= 1.2;
			this.player.stats.fireModifier /= 1.2;
			this.player.stats.manaCostModifier /= 0.8;
		}
		super.fade(endOfIteration);
	}

	tick(t) {
		this.cooldownRemaining = Math.max(0,this.cooldownRemaining - t);
		super.tick(t);
	}

	ready() {
		return this.cooldownRemaining <= 0;
	}
}

class EyeOfMagtheridon extends Aura {
	constructor(player) {
		super(player);
		this.name = "Eye of Magtheridon";
		this.durationTotal = 10;
	}

	apply() {
		if (!this.active) {
			this.player.stats.spellPower += 170;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
		}
		super.apply();
	}

	fade(endOfIteration = false) {
		if (this.active) {
			this.player.stats.spellPower -= 170;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
		}
		super.fade(endOfIteration);
	}
}

class SextantOfUnstableCurrents extends Aura {
	constructor(player) {
		super(player);
		this.name = "Sextant of Unstable Currents";
		this.durationTotal = 15;
		this.hiddenCooldown = 45;
		this.hiddenCooldownRemaining = 0;
		this.procChance = 20; // percent
	}

	apply() {
		if (!this.active && this.hiddenCooldownRemaining == 0) {
			this.player.stats.spellPower += 190;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
			this.hiddenCooldownRemaining = this.hiddenCooldown;
			super.apply();
		}
	}

	fade(endOfIteration = false) {
		if (this.active) {
			this.player.stats.spellPower -= 190;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
		}
		super.fade(endOfIteration);
	}

	tick(t) {
		this.hiddenCooldownRemaining = Math.max(0,this.hiddenCooldownRemaining - t);
		super.tick(t);
	}
}

class QuagmirransEye extends Aura {
	constructor(player) {
		super(player);
		this.name = "Quagmirran's Eye";
		this.hiddenCooldown = 45;
		this.hiddenCooldownRemaining = 0;
		this.durationTotal = 6;
		this.procChance = 10;
	}

	apply() {
		if (!this.active) {
			this.player.stats.hasteRating += 320;
			this.hiddenCooldownRemaining = this.hiddenCooldown;
			super.apply();
		}
	}

	fade(endOfIteration = false) {
		if (this.active) {
			this.player.stats.hasteRating -= 320;
		}
		super.fade(endOfIteration);
	}

	tick(t) {
		this.hiddenCooldownRemaining = Math.max(0,this.hiddenCooldownRemaining - t);
		super.tick(t);
	}
}

class ShiffarsNexusHorn extends Aura {
	constructor(player) {
		super(player);
		this.name = "Shiffar's Nexus-Horn";
		this.hiddenCooldown = 45;
		this.hiddenCooldownRemaining = 0;
		this.durationTotal = 10;
		this.procChance = 20;
	}

	apply() {
		if (!this.active) {
			this.player.stats.spellPower += 225;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
			this.hiddenCooldownRemaining = this.hiddenCooldown;
			super.apply();
		}
	}

	fade(endOfIteration = false) {
		if (this.active) {
			this.player.stats.spellPower -= 225;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
		}
		super.fade(endOfIteration);
	}

	tick(t) {
		this.hiddenCooldownRemaining = Math.max(0,this.hiddenCooldownRemaining - t);
		super.tick(t);
	}
}

class ManaEtched4Set extends Aura {
	constructor(player) {
		super(player);
		this.name = "Mana-Etched 4-Set Bonus";
		this.durationTotal = 15;
		this.procChance = 2;
	}

	apply() {
		if (!this.active) {
			this.player.stats.spellPower += 110;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
		}
		super.apply();
	}

	fade(endOfIteration = false) {
		if (this.active) {
			this.player.stats.spellPower -= 110;
			if (this.player.pet) this.player.pet.calculateStatsFromPlayer();
		}
		super.fade(endOfIteration);
	}
}