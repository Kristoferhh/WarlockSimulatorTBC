const PetName = Object.freeze({
	IMP: 0,
	VOIDWALKER: 1,
	SUCCUBUS: 2,
	FELHUNTER: 3,
	FELGUARD: 4
});

const PetType = Object.freeze({
	MELEE: 0,
	RANGED: 1
});

const PetMode = Object.freeze({
	PASSIVE: 0,
	AGGRESSIVE: 1
});

class Pet {
	constructor(player, settings) {
		this.player = player;
		this.simSettings = settings.simSettings;
		this.playerAuras = settings.auras;
		this.enemyDodgeChance = 5;
		this.castTimeRemaining = 0;
		this.fiveSecondRuleTimerRemaining = 5;
		this.spiritTickTimerRemaining = 2;
		this.mode = this.simSettings.petMode;
		this.spells = {};
		this.auras = {};
		this.stats = {
			"baseStats": {
				"stamina": 0,
				"intellect": 0,
				"strength": 0,
				"agility": 0,
				"mana": 0,
				"ap": 0
			},
			// certain stats from buffs need to be separated from the main stat so we can re-calculate the pet's stats in calculateStatsFromPlayer().
			"buffs": {
				"stamina": 0,
				"intellect": 0,
				"strength": 0,
				"agility": 0,
				"spellPower": 0,
				"ap": 0,
				"critChance": 0,
				"spellCritChance": 0
			},
			"mp5": 0,
			"ap": 0,
			"strength": 0,
			"agility": 0,
			"spirit": 0,
			"stamina": 0,
			"intellect": 0,
			"staminaModifier": 1 + (0.05 * player.talents.felStamina),
			"intellectModifier": 1 + (0.05 * player.talents.felIntellect),
			"strengthModifier": 1,
			"agilityModifier": 1,
			"spiritModifier": 1,
			"apModifier": 1,
			"damageModifier": 1 * (1 + (0.04 * player.talents.unholyPower)) * (1 + (0.05 * player.talents.soulLink)),
			"currentMana": 0
		};
	}

	initialize() {
		if (this.type == PetType.MELEE) {
			this.spells.melee = new Melee(this);
		}
	}

	// Calculates stats from buffs/debuffs/pet buffs
	calculateStatsFromAuras() {
		// Calculate melee hit chance
		// Formula from https://wowwiki-archive.fandom.com/wiki/Hit?oldid=1584399
		if (this.player.enemy.level - this.player.level <= 2) {
			this.stats.hitChance = 100 - (5 + (this.player.enemy.level - this.player.level) * 0.5);
		} else if (this.player.enemy.level - this.player.level > 2) {
			this.stats.hitChance = 100 - (7 + (this.player.enemy.level - this.player.level - 2) * 2);
		}

		// Calculate spell hit chance
		// Formula from https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/
		if ((parseInt(this.player.enemy.level) - this.player.level) <= 2) {
			this.stats.spellHitChance = Math.min(99, 100 - (parseInt(this.player.enemy.level) - this.player.level) - 4);
		} else if ((parseInt(this.player.enemy.level) - this.player.level) == 3) { // target 3 levels above
			this.stats.spellHitChance = 83;
		} else if ((parseInt(this.player.enemy.level) - this.player.level) == 4) { // target 4 levels above
			this.stats.spellHitChance = 72;
		} else { // target 5+ levels above
			this.stats.spellHitChance = 61;
		}

		// Configure stats based on selected auras
		if (this.playerAuras.blessingOfKings) {
			this.stats.staminaModifier *= 1.1;
			this.stats.intellectModifier *= 1.1;
			this.stats.agilityModifier *= 1.1;
			this.stats.strengthModifier *= 1.1;
			this.stats.spiritModifier *= 1.1;
		}
		if (this.playerAuras.blessingOfWisdom) this.stats.mp5 += 41;
		if (this.playerAuras.manaSpringTotem) this.stats.mp5 += 50;
		if (this.playerAuras.wrathOfAirTotem) this.stats.buffs.spellPower += 101;
		if (this.playerAuras.totemOfWrath) this.stats.buffs.spellCritChance += 3 * parseInt(this.simSettings.totemOfWrathAmount);
		// todo implement improved motw
		if (this.playerAuras.markOfTheWild) {
			this.stats.buffsstamina += 14;
			this.stats.buffs.intellect += 14;
			this.stats.spirit += 14;
		}
		if (this.playerAuras.arcaneIntellect) this.stats.buffs.intellect += 40;
		if (this.playerAuras.prayerOfFortitude) this.stats.buffs.stamina += 79;
		if (this.playerAuras.prayerOfSpirit) {
			let multiplier = parseInt(this.simSettings.improvedDivineSpirit);
			this.stats.spirit += 50 * (1 + (0.1 * multiplier));
		}
		if (this.playerAuras.inspiringPresence) this.stats.spellHitChance = Math.min(99, this.stats.spellHitChance + 1);
		if (this.playerAuras.moonkinAura) this.stats.buffs.spellCritChance += 5;
		//todo add atiesh auras
		if (this.playerAuras.judgementOfTheCrusader) this.stats.buffs.critChance += 3; // add spell crit too?
		if (this.playerAuras.vampiricTouch) this.stats.mp5 += this.simSettings.shadowPriestDps * 0.05;
		if (this.playerAuras.faerieFire) {
			let improved = this.simSettings.improvedFaerieFire == "yes";
			if (improved) this.stats.hitChance += 3;
			this.player.enemy.armor = Math.max(0, this.player.enemy.armor - 610);
		}
		// Apply Expose Armor if both EA and SA are selected and user selected 2/2 Improved Expose armor, or if Expose Armor is selected but Sunder Armor is not
		if ((this.playerAuras.sunderArmor && this.playerAuras.exposeArmor && this.simSettings.improvedExposeArmor == 2) || (this.playerAuras.exposeArmor && !this.playerAuras.sunderArmor)) {
			this.player.enemy.armor = Math.max(0, this.player.enemy.armor - 2050 * (1 + 0.25 * this.simSettings.improvedExposeArmor));
		}
		// Else apply Sunder Armor
		else if (this.playerAuras.sunderArmor) {
			this.player.enemy.armor = Math.max(0, this.player.enemy.armor - 2600);
		}
		if (this.playerAuras.curseOfRecklessness) this.player.enemy.armor = Math.max(0, this.player.enemy.armor - 800);
		if (this.playerAuras.exposeWeakness) this.stats.buffs.ap += (this.simSettings.survivalHunterAgility * 0.25 * (this.simSettings.exposeWeaknessUptime / 100)) || 0;
		if (this.playerAuras.heroicPresence) this.stats.hitChance++;
		if (this.playerAuras.blessingOfMight) this.stats.buffs.ap += 220;
		if (this.playerAuras.strengthOfEarthTotem) this.stats.buffs.strength += 86; // need to find out how much AP demons get from Strength
		if (this.playerAuras.graceOfAirTotem) this.stats.buffs.agility += 67;
		if (this.playerAuras.battleShout) this.stats.buffs.ap += 306;
		if (this.playerAuras.trueshotAura) this.stats.buffs.ap += 300;
		if (this.playerAuras.leaderOfThePack) this.stats.buffs.critChance += 5;
		if (this.playerAuras.unleashedRage) this.stats.apModifier *= 1.1;
		if (this.playerAuras.scrollOfStaminaV) this.stats.buffs.stamina += 20;
		if (this.playerAuras.scrollOfIntellectV) this.stats.buffs.intellect += 20;
		if (this.playerAuras.scrollOfStrengthV) this.stats.buffs.strength += 20;
		if (this.playerAuras.scrollOfAgilityV) this.stats.buffs.agility += 20;
		if (this.playerAuras.scrollOfSpiritV) this.stats.spirit += 20;


		// Calculate armor
		if (this.type == PetType.MELEE) {
			// Formula from https://wowpedia.fandom.com/wiki/Armor
			if (this.player.level >= 60 && this.player.level <= 79) {
				this.armorMultiplier = 1 - this.player.enemy.armor / (this.player.enemy.armor + 400 + 85 * (this.player.level + 4.5 * (this.player.level - 59)));
			} else if (this.player.level >= 80 && this.player.level <= 84) {
				this.armorMultiplier = 1 - this.player.enemy.armor / (this.player.enemy.armor + 400 + 85 * this.player.level + (4.5 * (this.player.level - 59)) + (20 * (this.player.level - 80)));
			} else if (this.player.level >= 85) {
				this.armorMultiplier = 1 - this.player.enemy.armor / (this.player.enemy.armor + 400 + 85 * this.player.level + (4.5 * (this.player.level - 59)) + (20 * (this.player.level - 80)) + (22 * (this.player.level - 85)));
			}
		}
	}

	// Player -> Pet stat scaling info taken from https://wowwiki-archive.fandom.com/wiki/Warlock?oldid=1618728
	calculateStatsFromPlayer(stat = "stamina") {
		if (stat == "stamina" || stat == "intellect" || stat == "spellPower" || stat == "shadowPower" || stat == "firePower") {
			this.stats.stamina = this.stats.baseStats.stamina + this.stats.buffs.stamina + 0.3 * (this.player.stats.stamina * this.player.stats.staminaModifier);
			this.stats.intellect = this.stats.baseStats.intellect + this.stats.buffs.intellect + 0.3 * (this.player.stats.intellect * this.player.stats.intellectModifier);
			this.player.demonicKnowledgeSp = ((this.stats.stamina * this.stats.staminaModifier) + (this.stats.intellect * this.stats.intellectModifier)) * (0.04 * this.player.talents.demonicKnowledge);
			// Need confirmation that 1 strength = 1.869 ap
			this.stats.ap = this.stats.baseStats.ap + this.stats.buffs.ap + (this.stats.baseStats.strength + this.stats.buffs.strength) * 1.869 + (this.player.stats.spellPower + this.player.demonicKnowledgeSp + Math.max(this.player.stats.shadowPower,this.player.stats.firePower)) * 0.57;
			this.stats.agility = this.stats.baseStats.agility + this.stats.buffs.agility;
			this.stats.critChance = 1 + this.player.talents.demonicTactics + (this.stats.agility * this.stats.agilityModifier) / 33 + this.stats.buffs.critChance;
			this.stats.spellCritChance = 5 + this.player.talents.demonicTactics + this.stats.buffs.spellCritChance;
			this.stats.spellPower = this.stats.buffs.spellPower + (this.player.stats.spellPower + this.player.demonicKnowledgeSp + Math.max(this.player.stats.shadowPower, this.player.stats.firePower)) * 0.15;
			this.stats.maxMana = this.stats.baseStats.mana + this.stats.intellect * 4.7 * this.stats.intellectModifier; // confirm
			this.player.combatLog("Calculated pet's stats from player's current stats");
		}
	}

	setup() {
		this.calculateStatsFromAuras();
		this.calculateStatsFromPlayer();
	}

	cast(spellName) {
		this.spells[spellName].startCast();
	}

	reset() {
		this.stats.mana = this.stats.maxMana;
		this.fiveSecondRuleTimerRemaining = 5; // If higher than 0 then the pet can't gain mana from Spirit regen
		this.spiritTickTimerRemaining = 2; // Unsure what the initial value should be since the Felguard isnt supposed to gain a Spirit regen tick between every Cleave (confirm)
		if (this.type == PetType.MELEE) {
			this.spells.melee.cooldownRemaining = 0;
		}
		this.calculateStatsFromPlayer();
	}

	tick(t) {
		if (this.type == PetType.MELEE) {
			this.spells.melee.tick(t);
		} /*else if (this.type == PetType.RANGED) {
			this.castTimeRemaining = Math.max(0, this.castTimeRemaining - t);
		}*/

		this.fiveSecondRuleTimerRemaining = Math.max(0, this.fiveSecondRuleTimerRemaining - t);
		this.spiritTickTimerRemaining = Math.max(0, this.spiritTickTimerRemaining - t);
		if (this.spiritTickTimerRemaining <= 0) {
			this.spiritTickTimerRemaining = 2;
			if (this.fiveSecondRuleTimerRemaining <= 0) {
				// Fromula from https://wowwiki-archive.fandom.com/wiki/Spirit?oldid=1601392
				let manaGain = (5 * Math.sqrt(this.stats.intellect * this.stats.intellectModifier) * (this.stats.spirit * this.stats.spiritModifier) * 0.009327) / 2.5; // Divide by 2.5 since it calculates mana per five seconds but the tick is every 2 seconds
				this.stats.mana = Math.min(this.stats.maxMana, this.stats.mana + manaGain);
				this.player.combatLog(this.name + " gains " + Math.round(manaGain) + " mana from Spirit regeneration. Pet mana: " + Math.round(this.stats.mana) + "/" + Math.round(this.stats.maxMana));
			}
		}
	}
}

class Imp extends Pet {
	constructor(player, settings) {
		super(player, settings);
		this.name = "Imp";
		this.type = PetType.RANGED;
		this.stats.baseStats.stamina = 114;
		this.stats.baseStats.intellect = 327;
		this.stats.baseStats.mana = 849;
		this.pet = PetName.IMP;
		this.setup();
	}
}

class Voidwalker extends Pet {
	constructor(player, settings) {
		super(player, settings);
		this.name = "Voidwalker";
		this.type = PetType.MELEE;
		this.pet = PetName.VOIDWALKER;
	}
}

class Succubus extends Pet {
	constructor(player, settings) {
		super(player, settings);
		this.name = "Succubus";
		this.pet = PetName.SUCCUBUS;
		this.type = PetType.MELEE;
		this.stats.baseStats.stamina = 114;
		this.stats.baseStats.intellect = 133;
		this.stats.baseStats.mana = 1109;
		this.stats.baseStats.spirit = 122;
		this.stats.baseStats.strength = 153;
		this.stats.baseStats.agility = 109;
		this.stats.baseStats.ap = 104.5; // needs confirmation
		this.dmg = 105;
		this.healthPerStamina = 7;
		this.stats.damageModifier *= 1 + (0.02 * player.talents.masterDemonologist);
		this.setup();
	}

	reset() {
		this.spells.lashOfPain.cooldownRemaining = 0;
		super.reset();
	}

	initialize() {
		this.spells.lashOfPain = new SuccubusLashOfPain(this);
		super.initialize();
	}

	tick(t) {
		this.spells.lashOfPain.tick(t);
		super.tick(t);
	}
}

class Felhunter extends Pet {
	constructor(player, settings) {
		super(player, settings);
		this.name = "Felhunter";
		this.type = PetType.MELEE;
		this.pet = PetName.FELHUNTER;
	}
}

class Felguard extends Pet {
	constructor(player, settings) {
		super(player, settings);
		this.name = "Felguard";
		this.type = PetType.MELEE;
		this.pet = PetName.FELGUARD;
		this.stats.baseStats.stamina = 322;
		this.stats.baseStats.strength = 153;
		this.stats.baseStats.agility = 109;
		this.stats.baseStats.intellect = 152;
		this.stats.baseStats.spirit = 122;
		this.stats.baseStats.mana = 849;
		this.stats.baseStats.ap = 104.5; // needs confirmation
		this.dmg = 206; // base melee damage
		this.stats.damageModifier *= 1 + (0.01 * player.talents.masterDemonologist);
		this.player.damageBreakdown.melee = {"name": "Melee (Felguard)"};
		this.setup();
	}

	reset() {
		this.spells.cleave.cooldownRemaining = 0;
		this.auras.demonicFrenzy.fade(true);
		super.reset();
	}

	initialize() {
		this.spells.cleave = new FelguardCleave(this);
		this.auras.demonicFrenzy = new DemonicFrenzy(this);
		super.initialize();
	}

	tick(t) {
		this.spells.cleave.tick(t);
		super.tick(t);
	}
}