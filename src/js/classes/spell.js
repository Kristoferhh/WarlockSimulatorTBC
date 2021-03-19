class Spell {
	constructor(player) {
		this.player = player;
		this.castTime = 0;
		this.manaCost = 0;
		this.cooldown = 0;
		this.cooldownRemaining = 0;
		this.casting = false;
		this.name = null;
		this.varName = null; // Same as this.name except it's written in camelCase
		this.isDot = false;
		this.doesDamage = false;
		this.canCrit = false;
		this.school = null; // The school that the spell belongs to such as shadow or fire
		this.type = null; // affliction or destruction
		this.travelTime = 0;
	}

	setup() {
		this.varName = camelCase(this.name);
		this.player.damageBreakdown[this.varName] = this.player.damageBreakdown[this.varName] || {"name": this.name};
	}

	ready() {
		return this.player.gcdRemaining <= 0 && this.player.castTimeRemaining <= 0 && this.manaCost <= this.player.mana && this.cooldownRemaining <= 0;
	}

	startCast() {
		this.player.gcdRemaining = this.player.gcdValue;

		if (this.castTime > 0) {
			this.casting = true;
			this.player.castTimeRemaining = this.castTime;
			this.player.combatLog("Started casting " + this.name);
		} else {
			this.player.combatLog("Cast " + this.name);
			this.cast();
		}
	}

	// Called when a spell finishes casting or immediately called if the spell has no cast time.
	cast() {
		this.player.damageBreakdown[this.varName].casts = this.player.damageBreakdown[this.varName].casts + 1 || 1;
		this.player.mana -= this.manaCost;
		this.cooldownRemaining = this.cooldown;
		this.casting = false;
		if (this.castTime > 0) this.player.combatLog("Finished casting " + this.name);

		let isCrit = false;
		if (this.canCrit) {
			// Checks if the spell is a crit.
			isCrit = this.player.isCrit(); // todo add extra crit from Improved Searing Pain
			if (isCrit) {
				// Increment the crit counter whether the spell hits or not so that the crit % on the damage breakdown is correct. Otherwise the crit % will be lower due to lost crits when the spell misses.
				this.player.damageBreakdown[this.varName].crits = this.player.damageBreakdown[this.varName].crits + 1 || 1;
			}
		}

		// Check if the spell hits or misses
		if (!this.player.isHit(this.type === "affliction")) {
			this.player.combatLog(this.name + " *resist*");
			this.player.damageBreakdown[this.varName].misses = this.player.damageBreakdown[this.varName].misses + 1 || 1;
			return 0;
		}

		if (this.isDot) {
			this.player.auras[this.varName].apply(this.player.stats.spellPower + this.player.stats[this.school + "Power"]);
		}

		if (this.doesDamage) {
			return this.damage(isCrit);
		}
	}

	damage(isCrit) {
		// Calculate the damage
		let dmg = (this.dmg + ((this.player.stats.spellPower + this.player.stats[this.school + "Power"]) * this.coefficient)) * this.player.stats[this.school + "Modifier"];
		// Improved Shadow Bolt
		if (this.varName == "shadowBolt" && this.player.talents.improvedShadowBolt > 0 && this.player.auras.improvedShadowBolt.active) {
			dmg *= this.player.auras.improvedShadowBolt.modifier;
			if (!isCrit) this.player.auras.improvedShadowBolt.decrementStacks();
		}
		if (isCrit) {
			let critMultiplier = 1.5;
			if (this.type == "destruction" && this.player.talents.ruin > 0) {
				critMultiplier += 0.5;
			}
			dmg *= critMultiplier;

			// Apply ISB debuff if casting Shadow Bolt
			if (this.varName == "shadowBolt" && this.player.talents.improvedShadowBolt > 0) {
				this.player.auras.improvedShadowBolt.apply();
			}
		}
		dmg = ~~(dmg *  (1 - 0.0025 * this.player.enemy[this.school + "Resist"]));
		if (isCrit) this.player.combatLog(this.name + " *" + dmg + "*");
		else this.player.combatLog(this.name + " " + dmg);
		this.player.damageBreakdown[this.varName].damage = this.player.damageBreakdown[this.varName].damage + dmg || dmg;
		return dmg;
	}

	tick(t) {
		if (this.casting && this.player.castTimeRemaining <= 0) {
			return this.cast();
		}
	}
}

class ShadowBolt extends Spell {
	constructor(player) {
		super(player);
		this.castTime = 3 - (0.1 * player.talents.bane);
		this.manaCost = 420 * (1 - 0.1 * player.talents.cataclysm);
		this.coefficient = (3 / 3.5) + (0.04 * player.talents.shadowAndFlame);
		this.dmg = 633; // Average rank 11 Shadow Bolt base damage
		this.name = "Shadow Bolt";
		this.doesDamage = true;
		this.canCrit = true;
		this.school = "shadow";
		this.type = "destruction";
		this.travelTime = player.spellTravelTime;
		this.setup();
	}
}

class Incinerate extends Spell {
	constructor(player) {
		super(player);
		this.castTime = 2.5;
		this.manaCost = 355 * (1 - 0.1 * player.talents.cataclysm);
		this.coefficient = 0.7143 + (0.04 * player.talents.shadowAndFlame);
		this.dmg = 479;
		this.bonusDamageFromImmolate = 120; // The bonus damage gained when Immolate is up on the target
		this.name = "Incinerate";
		this.doesDamage = true;
		this.canCrit = true;
		this.school = "fire";
		this.type = "destruction";
		this.travelTime = player.spellTravelTime;
		this.setup();
	}
}

class SearingPain extends Spell {
	constructor(player) {
		super(player);
		this.castTime = 1.5;
		this.manaCost = 205 * (1 - 0.1 * player.talents.cataclysm);
		this.coefficient = 0.4286;
		this.dmg = 295;
		this.name = "Searing Pain";
		this.doesDamage = true;
		this.canCrit = true;
		this.school = "fire";
		this.type = "destruction";
		this.setup();
	}
}

class SoulFire extends Spell {
	constructor(player) {
		super(player);
		this.castTime = 6 - (0.4 * player.talents.bane);
		this.manaCost = 250 * (1 - 0.1 * player.talents.cataclysm);
		this.coefficient = 1.15;
		this.dmg = 1130;
		this.name = "Soul Fire";
		this.doesDamage = true;
		this.canCrit = true;
		this.school = "fire";
		this.type = "destruction";
		this.travelTime = player.spellTravelTime;
		this.setup();
	}
}

class Shadowburn extends Spell {
	constructor(player) {
		super(player);
		this.cooldown = 15;
		this.manaCost = 515 * (1 - 0.1 * player.talents.cataclysm);
		this.coefficient = 0.4286;
		this.dmg = 694;
		this.name = "Shadowburn";
		this.doesDamage = true;
		this.canCrit = true;
		this.school = "shadow";
		this.type = "destruction";
		this.setup();
	}
}

class DeathCoil extends Spell {
	constructor(player) {
		super(player);
		this.cooldown = 120;
		this.manaCost = 600;
		this.coefficient = 0.22;
		this.dmg = 578.5;
		this.name = "Death Coil";
		this.doesDamage = true;
		this.canCrit = false; // confirm
		this.school = "shadow";
		this.type = "affliction";
		this.travelTime = player.spellTravelTime;
		this.setup();
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

	cast() {
		this.casting = false;
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
		this.manaCost = 370;
		this.castTime = Math.round((2 - (0.4 * player.talents.improvedCorruption)) * 100) / 100;
		this.isDot = true;
		this.school = "shadow";
		this.type = "affliction";
		this.setup();
	}
}

class UnstableAffliction extends Spell {
	constructor(player) {
		super(player);
		this.name = "Unstable Affliction";
		this.manaCost = 400;
		this.castTime = 1.5;
		this.isDot = true;
		this.school = "shadow";
		this.type = "affliction";
		this.setup();
	}
}

class SiphonLife extends Spell {
	constructor(player) {
		super(player);
		this.name = "Siphon Life";
		this.manaCost = 410;
		this.castTime = 0;
		this.isDot = true;
		this.school = "shadow";
		this.type = "affliction";
		this.setup();
	}
}

class Immolate extends Spell {
	constructor(player) {
		super(player);
		this.name = "Immolate";
		this.manaCost = 445;
		this.castTime = 2 - (0.1 * player.talents.bane);
		this.isDot = true;
		this.doesDamage = true;
		this.canCrit = true;
		this.dmg = 331.5;
		this.coefficient = 0.2;
		this.school = "fire";
		this.type = "destruction";
		this.setup();
	}
}