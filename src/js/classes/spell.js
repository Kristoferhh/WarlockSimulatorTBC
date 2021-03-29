class Spell {
	constructor(player) {
		this.player = player;
		this.castTime = 0;
		this.manaCost = 0;
		this.cooldown = 0;
		this.name = null;
		this.varName = null; // Same as this.name except it's written in camelCase
		this.isDot = false;
		this.modifier = 1;
		this.doesDamage = false;
		this.canCrit = false;
		this.school = null; // The school that the spell belongs to such as shadow or fire
		this.type = null; // affliction or destruction
		this.travelTime = 0;
		this.bonusCrit = 0;
		this.cooldownRemaining = 0;
		this.casting = false;
	}

	reset() {
		this.cooldownRemaining = 0;
		this.casting = false;
	}

	setup() {
		this.varName = camelCase(this.name);
		this.player.damageBreakdown[this.varName] = this.player.damageBreakdown[this.varName] || {"name": this.name};
	}

	ready() {
		return this.player.gcdRemaining <= 0 && this.player.castTimeRemaining <= 0 && this.manaCost <= this.player.mana && this.cooldownRemaining <= 0;
	}

	startCast() {
		// 1 second is the minimum for global cooldown?
		this.player.gcdRemaining = Math.max(1,Math.round((this.player.gcdValue / (1 + ((this.player.stats.hasteRating / hasteRatingPerPercent) / 100))) * 10000) / 10000);

		if (this.castTime > 0) {
			this.casting = true;
			this.player.castTimeRemaining = Math.round((this.castTime / (1 + ((this.player.stats.hasteRating / hasteRatingPerPercent) / 100))) * 10000) / 10000;
			this.player.combatLog("Started casting " + this.name + ". Cast time: " + this.player.castTimeRemaining + " (" + Math.round((this.player.stats.hasteRating / hasteRatingPerPercent) * 10000) / 10000 + "% haste at a base cast speed of " + this.castTime + "). Global cooldown: " + this.player.gcdRemaining);
		} else {
			this.player.combatLog("Cast " + this.name);
			this.cast();
		}
	}

	// Called when a spell finishes casting or immediately called if the spell has no cast time.
	cast() {
		this.player.damageBreakdown[this.varName].casts = this.player.damageBreakdown[this.varName].casts + 1 || 1;
		this.player.mana -= (this.manaCost * this.player.stats.manaCostModifier);
		this.cooldownRemaining = this.cooldown;
		this.casting = false;
		if (this.castTime > 0) this.player.combatLog("Finished casting " + this.name);

		let isCrit = false;
		if (this.canCrit) {
			// Checks if the spell is a crit.
			isCrit = this.player.isCrit(this.bonusCrit);
			if (isCrit) {
				// Increment the crit counter whether the spell hits or not so that the crit % on the damage breakdown is correct. Otherwise the crit % will be lower due to lost crits when the spell misses.
				this.player.damageBreakdown[this.varName].crits = this.player.damageBreakdown[this.varName].crits + 1 || 1;
			}
		}

		// Check if the spell hits or misses
		if (!this.player.isHit(this.type === "affliction")) {
			this.player.combatLog(this.name + " *resist*");
			this.player.damageBreakdown[this.varName].misses = this.player.damageBreakdown[this.varName].misses + 1 || 1;
			return;
		}

		if (isCrit && this.canCrit) {
			if (this.player.trinketIds.includes(30626) && random(1,100) <= this.player.auras.sextantOfUnstableCurrents.procChance) this.player.auras.sextantOfUnstableCurrents.apply(); // Sextant of Unstable Currents
			if (this.player.trinketIds.includes(28418) && random(1,100) <= this.player.auras.shiffarsNexusHorn.procChance) this.player.auras.shiffarsNexusHorn.apply(); // Shiffar's Nexus-Horn
		}

		if (this.isDot) {
			this.player.auras[this.varName].apply(this.player.stats.spellPower + this.player.demonicKnowledgeSp + this.player.stats[this.school + "Power"]);
		} else if (this.isAura) {
			this.player.auras[this.varName].apply();
		}

		if (this.doesDamage) {
			this.damage(isCrit);
		}

		// T4 2pc
		// 10% proc rate on all shadow or fire spells (including when dots applied) (needs confirmation)
		if (this.player.sets['645'] >= 2 && ['shadow','fire'].includes(this.school) && random(1,100) <= this.player.auras['shadowFlame' + this.school].procChance) {
			this.player.auras['shadowFlame' + this.school].apply();	
		}

		// Quagmirran's Eye
		if (this.player.auras.quagmirransEye && this.player.auras.quagmirransEye.hiddenCooldownRemaining <= 0 && random(1,100) <= this.player.auras.quagmirransEye.procChance) {
			this.player.auras.quagmirransEye.apply();
		}
	}

	damage(isCrit, spellPower = this.player.stats.spellPower + this.player.demonicKnowledgeSp, shadowPower = this.player.stats.shadowPower, firePower = this.player.stats.firePower) {
		let dmg = this.dmg;
		// If casting Incinerate and Immolate is up, add the bonus damage.
		if (this.varName == "incinerate" && this.player.auras.immolate && this.player.auras.immolate.active) {
			dmg += this.bonusDamageFromImmolate;
		}
		// Spellfire 3-set bonus. Add spell power equal to 7% of the player's intellect
		if (this.player.sets['552'] >= 3) {
			spellPower += (this.player.stats.intellect * this.player.stats.intellectModifier * 0.07);
		}
		if (this.school == "shadow") dmg += ((spellPower + shadowPower) * this.coefficient);
		else if (this.school == "fire") dmg += ((spellPower + firePower) * this.coefficient);
		dmg *= this.player.stats[this.school + "Modifier"];
		dmg *= this.modifier;

		// Improved Shadow Bolt
		if (this.school == "shadow" && this.player.auras.improvedShadowBolt && this.player.auras.improvedShadowBolt.active) {
			dmg *= this.player.auras.improvedShadowBolt.modifier;
			if (!this.isDot) this.player.auras.improvedShadowBolt.decrementStacks();
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
		this.player.iterationDamage += dmg;

		// Check for Spellstrike proc
		if (this.player.sets['559'] == 2 && random(1,100) <= 5) {
			this.player.auras.spellstrikeProc.apply();
		}

		// T5 4pc. Increase Corruption & Immolate dmg when Shadow Bolt/Incinerate hits
		// The way it's set up to work is that when Corruption or Immolate is applied, its modifier is 100% (or whatever its default modifier is), then after it's applied, if a Shadow Bolt/Incinerate hits, it increases the damage of *that* Corruption/Immolate aura by 10%. If it's re-applied, the 10% dmg is gone until a new Shadow Bolt/Incinerate hits. Please correct if this is not the way it works.
		if (this.player.sets['646'] >= 4) {
			if (this.varName == "shadowBolt" && this.player.auras.corruption && !this.player.auras.corruption.boosted) {
				this.player.auras.corruption.boosted = true;
				this.player.auras.corruption.modifier *= 1.1;
			} else if (this.varName == "incinerate" && this.player.auras.immolate && !this.player.auras.immolate.boosted) {
				this.player.auras.immolate.boosted = true;
				this.player.auras.immolate.modifier *= 1.1;
			}
		}
	}

	tick(t) {
		this.cooldownRemaining = Math.max(0, this.cooldownRemaining - t);

		if (this.casting && this.player.castTimeRemaining <= 0) {
			this.cast();
		}
	}
}

class ShadowBolt extends Spell {
	constructor(player) {
		super(player);
		this.castTime = this.calculateCastTime();
		this.manaCost = 420 * (1 - 0.01 * player.talents.cataclysm);
		this.coefficient = (3 / 3.5) + (0.04 * player.talents.shadowAndFlame);
		this.dmg = 633; // Average rank 11 Shadow Bolt base damage
		this.name = "Shadow Bolt";
		this.doesDamage = true;
		this.canCrit = true;
		this.school = "shadow";
		this.type = "destruction";
		this.travelTime = player.spellTravelTime;
		this.setup();

		if (player.sets['670'] >= 4) this.modifier *= 1.06; // T6 4pc
	}

	startCast() {
		if (this.player.auras.shadowTrance && this.player.auras.shadowTrance.active) {
			this.castTime = 0;
		}
		super.startCast();
		if (this.player.auras.shadowTrance && this.player.auras.shadowTrance.active) {
			this.castTime = this.calculateCastTime();
			this.player.auras.shadowTrance.fade();
		}
	}

	calculateCastTime() {
		return 3 - (0.1 * this.player.talents.bane);
	}
}

class Incinerate extends Spell {
	constructor(player) {
		super(player);
		this.castTime = Math.round((2.5 * (1 - 0.02 * player.talents.emberstorm)) * 100) / 100;
		this.manaCost = 355 * (1 - 0.01 * player.talents.cataclysm);
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

		if (player.sets['670'] >= 4) this.modifier *= 1.06; // T6 4pc
	}
}

class SearingPain extends Spell {
	constructor(player) {
		super(player);
		this.castTime = 1.5;
		this.manaCost = 205 * (1 - 0.01 * player.talents.cataclysm);
		this.coefficient = 0.4286;
		this.dmg = 295;
		this.name = "Searing Pain";
		this.doesDamage = true;
		this.canCrit = true;
		this.school = "fire";
		this.type = "destruction";
		this.bonusCrit = 4 * player.talents.improvedSearingPain;
		this.setup();
	}
}

class SoulFire extends Spell {
	constructor(player) {
		super(player);
		this.castTime = 6 - (0.4 * player.talents.bane);
		this.manaCost = 250 * (1 - 0.01 * player.talents.cataclysm);
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
		this.manaCost = 515 * (1 - 0.01 * player.talents.cataclysm);
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

class Shadowfury extends Spell {
	constructor(player) {
		super(player);
		this.name = "Shadowfury";
		this.dmg = 670.5;
		this.manaCost = 710 * (1 - 0.01 * player.talents.cataclysm);
		this.doesDamage = true;
		this.school = "shadow";
		this.type = "destruction";
		this.cooldown = 20;
		this.coefficient = 0.195;
		this.canCrit = true; // confirm
		this.setup();
	}
}

class SeedOfCorruption extends Spell {
	constructor(player) {
		super(player);
		this.name = "Seed of Corruption";
		this.dmg = 1380;
		this.manaCost = 882;
		this.doesDamage = true;
		this.school = "shadow";
		this.type = "affliction";
		this.coefficient = 0.22;
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
		let manaGain = (this.manaReturn + ((this.player.stats.spellPower + this.player.demonicKnowledgeSp + this.player.stats.shadowPower) * this.coefficient)) * this.modifier;
		this.player.combatLog(this.name + " " + Math.round(manaGain));
		if (this.player.mana + manaGain > this.player.stats.maxMana) this.player.combatLog("Life Tap used at too high mana (mana wasted)"); // Warning for if the simulation ever decides to use life tap when it would overcap the player on mana.
		this.player.mana = Math.min(this.player.stats.maxMana, this.player.mana + manaGain);
		if (this.player.pet && this.player.talents.manaFeed > 0) {
			let petManaGain = manaGain * (this.player.talents.manaFeed / 3);
			this.player.pet.stats.currentMana = Math.min(this.player.pet.stats.maxMana, this.player.pet.stats.currentMana + petManaGain);
			this.player.combatLog(this.player.pet.name + " gains " + Math.round(petManaGain) + " mana from Mana Feed");
		}

	}
}

class DarkPact extends Spell {
	constructor(player) {
		super(player);
		this.name = "Dark Pact";
		this.manaReturn = 700;
		this.coefficient = 0.96;
	}

	cast() {
		this.casting = false;
		let manaGain = this.manaReturn + (this.player.stats.spellPower + this.player.demonicKnowledgeSp + this.player.stats.shadowPower) * this.coefficient;
		this.player.combatLog(this.name + " " + Math.round(manaGain));
		if (this.player.mana + manaGain > this.player.stats.maxMana) console.log("Dark Pact used at too high mana (mana wasted)");
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
		this.modifier = 1 + 0.05 * player.talents.improvedImmolate;
		this.school = "fire";
		this.type = "destruction";
		this.setup();
	}
}

class CurseOfAgony extends Spell {
	constructor(player) {
		super(player);
		this.name = "Curse of Agony";
		this.manaCost = 265;
		this.isDot = true;
		this.school = "shadow";
		this.type = "affliction";
		this.setup();
	}
}

class CurseOfTheElements extends Spell {
	constructor(player) {
		super(player);
		this.name = "Curse of the Elements";
		this.manaCost = 260;
		this.type = "affliction";
		this.isAura = true;
		this.setup();
	}
}

class CurseOfRecklessness extends Spell {
	constructor(player) {
		super(player);
		this.name = "Curse of Recklessness";
		this.manaCost = 160;
		this.type = "affliction";
		this.isAura = true;
		this.setup();
	}
}

class CurseOfDoom extends Spell {
	constructor(player) {
		super(player);
		this.name = "Curse of Doom";
		this.manaCost = 380;
		this.dmg = 4200;
		this.coefficient = 2;
		this.cooldown = 60;
		this.school = "shadow";
		this.type = "affliction";
		this.isAura = true;
		this.setup();
	}
}