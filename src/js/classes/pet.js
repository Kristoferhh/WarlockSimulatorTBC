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
		this.swingTimerRemaining = 0;
		this.enemyDodgeChance = 5;
		this.castTimeRemaining = 0;
		this.critChance = 5 + player.talents.demonicTactics;
		this.spellCritChance = 5 + player.talents.demonicTactics;
		this.modifier = 1 * (1 + (0.04 * player.talents.unholyPower)) * (1 + (0.05 * player.talents.soulLink));
		this.intellectModifier = 1 + (0.05 * player.talents.felIntellect);
		this.staminaModifier = 1;
		this.mp5 = 0;
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

	calculateStatsFromPlayer() {
		this.stamina = (this.stats.baseStamina + 0.3 * (this.player.stats.stamina * this.player.stats.staminaModifier)) * this.staminaModifier;
		this.ap = this.stats.baseAp + (this.player.stats.spellPower + Math.max(this.player.stats.shadowPower,this.player.stats.firePower)) * 0.57;
		this.spellPower = (this.player.stats.spellPower + Math.max(this.player.stats.shadowPower, this.player.stats.firePower)) * 0.15;
		this.intellect = (this.stats.baseIntellect + 0.3 * (this.player.stats.intellect * this.player.stats.intellectModifier)) * this.intellectModifier;
		this.maxMana = this.stats.baseMana + this.intellect * 4.7 * this.intellectModifier; // confirm
	}

	cast(spellName) {
		this.spells[spellName].startCast();
	}

	reset() {
		this.mana = this.maxMana;
	}

	tick(t) {
		if (this.type == PetTypes.MELEE) {
			this.spells.melee.tick(t);
		} /*else if (this.type == PetTypes.RANGED) {
			this.castTimeRemaining = Math.max(0, this.castTimeRemaining - t);
		}*/
	}
}

class Imp extends Pet {
	constructor(player) {
		super(player);
		this.name = "Imp";
		this.type = PetTypes.RANGED;
		this.stats = {
			"baseStamina": 114,
			"baseIntellect": 327,
			"baseAp": 0,
			"baseMana": 849
		};
		this.pet = Pets.IMP;
		this.calculateStatsFromPlayer();
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
		this.stats = {
			"baseStamina": 114,
			"baseIntellect": 133,
			"baseAp": 286,
			"baseMana": 1109
		}
		this.dmg = 105;
		this.healthPerStamina = 7;
		this.modifier *= 1 + (0.02 * player.masterDemonologist);
		this.pet = Pets.SUCCUBUS;
		this.calculateStatsFromPlayer();
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
		this.pet = Pets.FELGUARD;
		this.stats = {
			"baseStamina": 322,
			"baseAp": 286,
			"baseIntellect": 152,
			"baseMana": 849
		};
		this.dmg = 206; // base melee damage
		this.swingTimer = 2;
		this.modifier *= 1 + (0.01 * player.talents.masterDemonologist);
		this.player.damageBreakdown.melee = {"name": "Melee (Felguard)"};
		this.calculateStatsFromPlayer();
	}

	initialize() {
		this.spells = {
			"melee": new FelguardMelee(this),
			"cleave": new FelguardCleave(this)
		};
	}

	tick(t) {
		this.spells.cleave.tick(t);
		super.tick(t);
	}
}