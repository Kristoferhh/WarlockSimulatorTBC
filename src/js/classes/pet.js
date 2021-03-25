const Pets = Object.freeze({
	IMP: 0,
	VOIDWALKER: 1,
	SUCCUBUS: 2,
	FELHUNTER: 3,
	FELGUARD: 4
});

const PetTypes = Object.freeze({
	MELEE: 0,
	RANGED: 1
});

const PetModes = Object.freeze({
	PASSIVE: 0,
	AGGRESSIVE: 1
});

class Pet {
	constructor(player) {
		this.player = player;
		this.statModifiers = {};
		this.ap = 0;
		this.spellPower = 0;
		this.swingTimerRemaining = 0;
		this.enemyDodgeChance = 5;
		this.castTimeRemaining = 0;
		this.critChance = 5 + player.talents.demonicTactics;
		this.spellCritChance = 5 + player.talents.demonicTactics;
		this.modifier = 1 * (1 + (0.04 * player.talents.unholyPower)) * (1 + (0.05 * player.talents.soulLink));
		this.player.damageBreakdown = {};
		// Formula from https://wowpedia.fandom.com/wiki/Armor
		if (player.level >= 60 && player.level <= 79) {
			this.armorMultiplier = 1 - player.enemy.armor / (player.enemy.armor + 400 + 85 * (player.level + 4.5 * (player.level - 59)));
		} else if (player.level >= 80 && player.level <= 84) {
			this.armorMultiplier = 1 - player.enemy.armor / (player.enemy.armor + 400 + 85 * player.level + (4.5 * (player.level - 59)) + (20 * (player.level - 80)));
		} else if (player.level >= 85) {
			this.armorMultiplier = 1 - player.enemy.armor / (player.enemy.armor + 400 + 85 * player.level + (4.5 * (player.level - 59)) + (20 * (player.level - 80)) + (22 * (player.level - 85)));
		}
		// Formula from https://wowwiki-archive.fandom.com/wiki/Hit?oldid=1584399
		if (this.player.enemy.level - this.player.level <= 2) {
			this.hitChance = 100 - (5 + (this.player.enemy.level - this.player.level) * 0.5);
		} else if (this.player.enemy.level - this.player.level > 2) {
			this.hitChance = 100 - (7 + (this.player.enemy.level - this.player.level - 2) * 2);
		}
	}

	reset() {
		this.mana = this.maxMana;
	}

	ready() {
		if (this.type == PetTypes.MELEE) {
			return this.swingTimerRemaining <= 0;
		} else if (this.type == PetTypes.RANGED) {
			return this.castTimeRemaining <= 0;
		}
	}

	attack() {
		if (this.type == PetTypes.MELEE) {
			this.swingTimerRemaining = this.swingTimer;
			this.player.damageBreakdown.melee.casts = this.player.damageBreakdown.melee.casts + 1 || 1;
			// Check for crit
			let isCrit = random(1,100) * this.player.stats.critChanceMultiplier <= this.critChance * this.player.stats.critChanceMultiplier;
			if (isCrit) {
				this.player.damageBreakdown.melee.crits = this.player.damageBreakdown.melee.crits + 1 || 1;
			}
			// Check for hit
			let isMiss = random(1,100) > this.hitChance;
			if (isMiss) {
				this.player.damageBreakdown.melee.misses = this.player.damageBreakdown.melee.misses + 1 || 1;
				// If the attack missed but it was calculated to be a crit (this is the way WoW calculates crits, by calculating it across all attacks, even misses)
				if (isCrit) {
					this.player.damageBreakdown.melee.missedCrits = this.player.damageBreakdown.melee.missedCrits + 1 || 1;
				}
				this.player.combatLog(this.name + " melee *miss*");
			}
			// Check for dodge
			let isDodge = random(1,100) <= this.enemyDodgeChance;
			if (isDodge) {
				this.player.damageBreakdown.melee.dodges = this.player.damageBreakdown.melee.dodges + 1 || 1;
				this.player.combatLog(this.name + " melee *dodge*");
			}
			if (isMiss || isDodge) return;

			// Calculate damage
			let dmg = this.damage + this.ap / 7;
			dmg *= this.modifier;
			if (isCrit) {
				dmg *= 1.5;
			}
			// Apply physical damage reduction from armor
			dmg *= this.armorMultiplier;

			this.player.iterationDamage += dmg;
			if (isCrit) this.player.combatLog(this.name + " melee *" + Math.round(dmg) + "*");
			else this.player.combatLog(this.name + " melee " + Math.round(dmg));
			this.player.damageBreakdown.melee.damage = this.player.damageBreakdown.melee.damage + dmg || dmg;
		}
	}

	tick(t) {
		if (this.type == PetTypes.MELEE) {
			this.swingTimerRemaining = Math.max(0, this.swingTimerRemaining - t);
		} else if (this.type == PetTypes.RANGED) {
			this.castTimeRemaining = Math.max(0, this.castTimeRemaining - t);
		}
	}
}

class Imp extends Pet {
	constructor(player) {
		super(player);
		this.name = "Imp";
		this.type = PetTypes.RANGED;
		this.stamina = 114;
		this.intellect = 327;
		this.maxMana = 849 + this.intellect * 4.7 * (1 + 0.05 * player.talents.felIntellect);
		this.pet = Pets.IMP;
	}
}

class Voidwalker extends Pet {
	constructor(player) {
		super(player);
		this.name = "Voidwalker";
		this.type = PetTypes.MELEE;
		this.pet = Pets.VOIDWALKER;
	}
}

class Succubus extends Pet {
	constructor(player) {
		super(player);
		this.name = "Succubus";
		this.type = PetTypes.MELEE;
		this.damage = 105;
		this.stamina = 114; // confirm
		this.intellect = 133;
		this.ap = 286;
		this.healthPerStamina = 7;
		this.modifier *= 1 + (0.02 * player.masterDemonologist);
		this.maxMana = 1109 + this.intellect * 10.6 * (1 + 0.05 * player.talents.felIntellect);
		this.pet = Pets.SUCCUBUS;
	}
}

class Felhunter extends Pet {
	constructor(player) {
		super(player);
		this.name = "Felhunter";
		this.type = PetTypes.MELEE;
		this.pet = Pets.FELHUNTER;
	}
}

class Felguard extends Pet {
	constructor(player) {
		super(player);
		this.name = "Felguard";
		this.type = PetTypes.MELEE;
		this.damage = 206; // base melee damage
		this.stamina = 322;
		this.intellect = 152;
		this.swingTimer = 2;
		this.maxMana = 849 + this.intellect * 4.7 * (1 + 0.05 * player.talents.felIntellect); // confirm
		this.modifier *= 1 + (0.01 * player.talents.masterDemonologist);
		this.pet = Pets.FELGUARD;
		this.player.damageBreakdown.melee = {"name": "Melee (Felguard)"};
	}

	initialize() {

	}
}