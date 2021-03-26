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
		if (this.manaCost > 0) this.pet.stats.currentMana = Math.max(0, this.pet.stats.currentMana - this.manaCost);
		if (this.castTime > 0) this.pet.player.combatLog(this.pet.name + " finished casting " + this.name);
		else this.pet.player.combatLog(this.pet.name + " casts " + this.name);
		this.pet.player.damageBreakdown[this.varName].casts = this.pet.player.damageBreakdown[this.varName].casts + 1 || 1;

		// Check for crit
		//todo fix so it uses spell crit for spells and melee hit for melee hits
		let isCrit = this.canCrit && random(1,100) * this.pet.player.stats.critChanceMultiplier <= this.pet.stats.critChance * this.pet.player.stats.critChanceMultiplier;
		if (isCrit) {
			this.pet.player.damageBreakdown[this.varName].crits = this.pet.player.damageBreakdown[this.varName].crits + 1 || 1;
		}
		// Check for miss
		let isMiss = random(1,100) > this.pet.stats.hitChance;
		if (isMiss) {
			this.pet.player.damageBreakdown[this.varName].misses = this.pet.player.damageBreakdown[this.varName].misses + 1 || 1;
			this.pet.player.combatLog(this.pet.name + " " + this.name + " *miss*");
		}
		// Check for dodge if melee
		let isDodge = this.pet.type == PetTypes.MELEE && random(1,100) <= this.pet.enemyDodgeChance;
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
		if (this.pet.type == PetTypes.MELEE) {
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

class FelguardMelee extends PetSpell {
	constructor(pet) {
		super(pet);
		this.cooldown = 2;
		this.name = "Melee";
		this.varName = "melee";
		this.pet.player.damageBreakdown[this.varName] = {"name": "Melee (Felguard)"};
		this.resetsFiveSecondRule = false;
	}

	calculateDamage() {
		return this.pet.dmg + (this.pet.stats.ap * this.pet.stats.apModifier * (1 + (0.05 * this.pet.auras.demonicFrenzy.stacks))) / 7;
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
	}

	calculateDamage() {
		return this.pet.spells.melee.calculateDamage() + 78;
	}
}