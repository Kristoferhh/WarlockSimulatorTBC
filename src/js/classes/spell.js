class Spell {
	constructor(player) {
		this.castTime = 0;
		this.manaCost = 0;
		this.cooldown = 0;
		this.cooldownRemaining = 0;
		this.player = player;
		this.casting = false;
		this.dot = false;
	}

	ready() {
		return this.player.gcdRemaining <= 0 && this.player.castTimeRemaining <= 0 && this.manaCost <= this.player.mana && this.cooldownRemaining <= 0;
	}

	// Starts the cast of a spell.
	startCast() {
		this.player.gcdRemaining = this.player.gcdValue;

		if (this.castTime > 0) {
			this.casting = true
			this.player.castTimeRemaining = this.castTime;
			this.player.combatLog("Started casting " + this.name);
		} else {
			this.player.combatLog("Cast " + this.name);
			this.cast();
		}
	}

	// Called when a spell finishes casting or immediately called if the spell has no cast time.
	cast() {
		this.player.mana -= this.manaCost;
		this.cooldownRemaining = this.cooldown;
		if (this.castTime > 0) this.player.combatLog("Finished casting " + this.name);
			
		// Only return this.damage() if the spell is not a dot. If it's a dot then we want to enable the Aura at the end of the cast rather than doing damage.
		if (!this.dot) {
			this.casting = false;
			return this.damage();
		}
	}

	damage() {
		return 0;
	}

	tick(t) {
		this.player.castTimeRemaining = Math.max(0, this.player.castTimeRemaining - t);

		if (this.player.castTimeRemaining <= 0 && this.casting) {
			return this.cast();
		}
		return 0;
	}
}

class ShadowBolt extends Spell {
	constructor(player) {
		super(player);
		this.name = "Shadow Bolt";
		player.damageBreakdown.shadowBolt = player.damageBreakdown.shadowBolt || {"name": this.name};
		this.manaCost = 420 * (1 - (player.talents.cataclysm / 100));
		this.castTime = 3 - (player.talents.bane / 10);
		this.coefficient = (3 / 3.5) + (0.04 * player.talents.shadowAndFlame);
		this.dmg = 633; // Average rank 11 Shadow Bolt base damage
	}

	damage() {
		this.player.damageBreakdown.shadowBolt.casts = this.player.damageBreakdown.shadowBolt.casts + 1 || 1;
		if (!this.player.isHit(false)) {
			this.player.combatLog(this.name + " *miss*");
			this.player.damageBreakdown.shadowBolt.misses = this.player.damageBreakdown.shadowBolt.misses + 1 || 1;
			return 0;
		}

		let dmg = (this.dmg + ((this.player.stats.spellPower + this.player.stats.shadowPower) * this.coefficient)) * this.player.stats.shadowModifier;

		// Checks if the spell is a crit
		let isCrit = this.player.isCrit();
		// Improved Shadow Bolt
		if (this.player.talents.improvedShadowBolt > 0 && this.player.auras.improvedShadowBolt.active) {
			dmg *= this.player.auras.improvedShadowBolt.modifier;
			if (!isCrit) this.player.auras.improvedShadowBolt.decrementStacks();
		}
		if (isCrit) {
			this.player.damageBreakdown.shadowBolt.crits = this.player.damageBreakdown.shadowBolt.crits + 1 || 1;
			dmg *= (1.5 + 0.5 * this.player.talents.ruin);

			// Apply ISB debuff
			if (this.player.talents.improvedShadowBolt > 0) {
				this.player.auras.improvedShadowBolt.apply();
			}
		}

		dmg = ~~(dmg *  (1 - 0.0025 * this.player.enemy.shadowResist));
		if (isCrit) this.player.combatLog(this.name + " *" + dmg + "*");
		else this.player.combatLog(this.name + " " + dmg);

		this.player.damageBreakdown.shadowBolt.damage = this.player.damageBreakdown.shadowBolt.damage + dmg || dmg;
		return dmg;
	}
}

class LifeTap extends Spell {
	constructor(player) {
		super(player);
		this.name = "Life Tap";
		this.manaReturn = 582;
		this.coefficient = 0.8;
		this.modifier = 1 + (0.1 * this.player.talents.improvedLifeTap);
	}

	startCast() {
		super.startCast();
		let manaGain = (this.manaReturn + ((this.player.stats.spellPower + this.player.stats.shadowPower) * this.coefficient)) * this.modifier;
		this.player.combatLog(this.name + " " + Math.round(manaGain));
		if (this.player.mana + manaGain > this.player.stats.maxMana) console.log("Life Tap used at too high mana (mana wasted)"); // Warning for if the simulation ever decides to use life tap when it would overcap the player on mana.
		this.player.mana = Math.min(this.player.stats.maxMana, this.player.mana + manaGain);
	}
}

class Corruption extends Spell {
	constructor(player) {
		super(player);
		this.name = "Corruption";
		player.damageBreakdown.corruption = player.damageBreakdown.corruption || {"name": this.name};
		this.manaCost = 370;
		this.castTime = 2 - (0.4 * player.talents.improvedCorruption);
		this.dot = true;
	}

	cast() {
		super.cast();
		this.player.damageBreakdown.corruption.casts = this.player.damageBreakdown.corruption.casts + 1 || 1;

		// Check if the Corruption hit
		if (this.player.isHit(true)) {
			this.player.auras.corruption.apply(this.player.stats.spellPower + this.player.stats.shadowPower);
		} else {
			this.player.combatLog(this.name + " *miss*");
			this.player.damageBreakdown.corruption.misses = this.player.damageBreakdown.corruption.misses + 1 || 1;
		}
	}
}