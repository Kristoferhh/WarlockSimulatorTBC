const SpellTypes = Object.freeze({
	MAGICAL: 0,
	PHYSICAL: 1
});

class PetSpell {
	constructor(pet) {
		this.pet = pet;
		this.casting = false;
		this.casting = false;
		this.canCrit = true;
		this.cooldownRemaining = 0;
		this.castTime = 0;
		this.manaCost = 0;
		this.resetsFiveSecondRule = true; // true if it's a spell that resets the five second rule for mana regen from spirit (true for everything but melee attacks)
	}

	ready() {
		return this.cooldownRemaining <= 0 && this.pet.stats.currentMana >= this.manaCost;
	}

	tick(t) {
		if (this.casting && this.pet.castTimeRemaining <= 0) {
			this.casting = false;
			this.cast();
		}
		if (this.cooldownRemaining > 0) this.cooldownRemaining = Math.max(0, this.cooldownRemaining - t);
	}

	startCast() {
		if (this.castTime > 0) {
			this.casting = true;
			this.pet.player.combatLog(this.pet.name + " started casting " + this.name);
		} else {
			this.cast();
		}
	}

	cast() {
		this.cooldownRemaining = this.cooldown;
		if (this.resetsFiveSecondRule) this.pet.fiveSecondRuleTimerRemaining = 5;
		let combatLogEntry = this.pet.name;
		if (this.castTime > 0) combatLogEntry += " finished casting " + this.name;
		else combatLogEntry += " casts " + this.name;
		if (this.manaCost > 0) {
			this.pet.stats.currentMana = Math.max(0, this.pet.stats.currentMana - this.manaCost);
			combatLogEntry += ". Pet mana: " + Math.round(this.pet.stats.currentMana) + "/" + Math.round(this.pet.stats.maxMana);
		}
		this.pet.player.combatLog(combatLogEntry);
		this.pet.player.damageBreakdown[this.varName].casts = this.pet.player.damageBreakdown[this.varName].casts + 1 || 1;

		// Check for crit
		//todo fix so it uses spell crit for spells and melee hit for melee hits
		let isCrit = this.canCrit && ((this.type == SpellTypes.PHYSICAL && random(1,100) * this.pet.player.stats.critChanceMultiplier <= this.pet.stats.critChance * this.pet.player.stats.critChanceMultiplier) || (this.type == SpellTypes.MAGICAL && random(1,100) * this.pet.player.stats.critChanceMultiplier <= this.pet.stats.spellCritChance * this.pet.player.stats.critChanceMultiplier));
		if (isCrit) {
			this.pet.player.damageBreakdown[this.varName].crits = this.pet.player.damageBreakdown[this.varName].crits + 1 || 1;
		}
		// Check for miss
		// todo change for spell hit
		let isMiss = (this.type == SpellTypes.PHYSICAL && random(1,100) > this.pet.stats.hitChance) || (this.type == SpellTypes.MAGICAL && random(1,100) > this.pet.stats.spellHitChance);
		if (isMiss) {
			this.pet.player.damageBreakdown[this.varName].misses = this.pet.player.damageBreakdown[this.varName].misses + 1 || 1;
			this.pet.player.combatLog(this.pet.name + " " + this.name + " *miss*");
		}
		// Check for dodge if melee
		let isDodge = this.type == SpellTypes.PHYSICAL && random(1,100) <= this.pet.enemyDodgeChance;
		if (isDodge) {
			this.pet.player.damageBreakdown[this.varName].dodges = this.pet.player.damageBreakdown[this.varName].dodges + 1 || 1;
			if (!isMiss) this.pet.player.combatLog(this.pet.name + " " + this.name + " *dodge*");
		}

		if (isMiss || isDodge) return;

		// Calculate damage
		let dmg = this.calculateDamage() * this.pet.stats.damageModifier;
		if (isCrit) {
			dmg *= 1.5; // pets use different multiplier maybe?
		}
		// Armor damage reduction if melee
		if (this.type == SpellTypes.PHYSICAL) {
			dmg *= this.pet.armorMultiplier;
		}

		this.pet.player.iterationDamage += dmg;
		if (isCrit) this.pet.player.combatLog(this.pet.name + " " + this.name + " *" + Math.round(dmg) + "*");
		else this.pet.player.combatLog(this.pet.name + " " + this.name + " " + Math.round(dmg));
		this.pet.player.damageBreakdown[this.varName].damage = this.pet.player.damageBreakdown[this.varName].damage + dmg || dmg;

		if (this.pet.pet == Pets.FELGUARD) {
			this.pet.auras.demonicFrenzy.apply();
		}
	}
}

class Melee extends PetSpell {
	constructor(pet) {
		super(pet);
		this.cooldown = 2;
		this.name = "Melee";
		this.varName = "melee";
		this.pet.player.damageBreakdown[this.varName] = {"name": "Melee (" + this.pet.name + ")"};
		this.resetsFiveSecondRule = false;
		this.type = SpellTypes.PHYSICAL;
	}

	calculateDamage() {
		let ap = this.pet.stats.ap * this.pet.stats.apModifier;
		if (this.pet.pet == Pets.FELGUARD) {
			ap *= (1 + 0.05 * this.pet.auras.demonicFrenzy.stacks);
		}
		return this.pet.dmg + (ap / 7);
	}
}

class FelguardCleave extends PetSpell {
	constructor(pet) {
		super(pet);
		this.cooldown = 6;
		this.manaCost = 417;
		this.name = "Cleave";
		this.varName = "cleave";
		this.pet.player.damageBreakdown[this.varName] = {"name": "Cleave (Felguard)"};
		this.type = SpellTypes.PHYSICAL;
	}

	calculateDamage() {
		return this.pet.spells.melee.calculateDamage() + 78;
	}
}

class SuccubusLashOfPain extends PetSpell {
	constructor(pet) {
		super(pet);
		this.cooldown = 12;
		this.manaCost = 190;
		this.name = "Lash of Pain";
		this.varName = "lashOfPain";
		this.dmg = 123;
		this.coefficient = 0.429;
		this.pet.player.damageBreakdown[this.varName] = {"name": "Lash of Pain (Succubus)"};
		this.type = SpellTypes.MAGICAL;
		this.modifier = 1;

		// Check for shadow damage modifying debuffs on the boss
		if (this.pet.playerAuras.curseOfTheElements) this.modifier *= (1.1 + 0.01 * this.pet.simSettings.improvedCurseOfTheElements);
		if (this.pet.playerAuras.shadowWeaving) this.modifier *= 1.1;
		if (this.pet.playerAuras.misery) this.modifier *= 1.05;
	}

	calculateDamage() {
		return (this.dmg + this.pet.stats.spellPower * this.coefficient) * this.modifier;
	}
}