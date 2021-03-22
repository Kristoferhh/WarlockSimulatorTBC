class Player {
	static getSettings() {
		return {
			"auras": auras,
			"items": selectedItems,
			"enchants": selectedEnchants,
			"gems": selectedGems,
			"talents": talents,
			"stats": characterStats,
			"enemy": {
				"level": $("input[name='target-level']").val(),
				"shadowResist": $("input[name='target-shadow-resistance']").val(),
				"fireResist": $("input[name='target-fire-resistance']").val()
			},
			"rotation": rotation,
			"selectedItemSlot": $("#item-slot-selection-list li[data-selected='true']").attr('data-slot')
		}
	}

	constructor(settings = Player.getSettings(), customItemSlot = null, customItemID = null) {
		this.stats = JSON.parse(JSON.stringify(settings.stats)); // Create a deep-copy of the character's stats since we need to modify the values.
		this.enemy = settings.enemy;
		this.rotation = settings.rotation;
		this.talents = settings.talents;
		this.level = 70;
		this.itemID = customItemID || settings.items[settings.selectedItemSlot] || 0;

		// If the player is equipped with a custom item then remove the stats from the currently equipped item and add stats from the custom item
		if (customItemSlot && customItemID && customItemID !== settings.items[customItemSlot]) {
			// Loop through all items in the custom item slot
			for (let item in items[customItemSlot]) {
				// Check if this is the currently equipped item
				if (items[customItemSlot][item].id == settings.items[customItemSlot]) {
					// Remove stats from currently equipped item
					for (let stat in items[customItemSlot][item]) {
						if (this.stats.hasOwnProperty(stat)) {
							this.stats[stat] -= items[customItemSlot][item][stat];
						}
					}
					// Remove stats from gems in the equipped item if there are any
					if (settings.gems[customItemSlot] && settings.gems[customItemSlot][settings.items[customItemSlot]]) {
						// Loop through each socket in the equipped item
						for (let socket in settings.gems[customItemSlot][settings.items[customItemSlot]]) {
							// Find the gem that is equipped in the socket by looking for its ID
							for (let socketColor in gems) {
								for (let gem in gems[socketColor]) {
									// Check if the ID matches
									if (gems[socketColor][gem].id == settings.gems[customItemSlot][settings.items[customItemSlot]][socket]) {
										// Loop through the gem's stats and remove them from the player
										for (let stat in gems[socketColor][gem]) {
											if (this.stats.hasOwnProperty(stat)) {
												this.stats[stat] -= gems[socketColor][gem][stat];
											}
										}
									}
								}
							}
						}
					}
				}
				// Check if this is the custom item
				else if (items[customItemSlot][item].id == customItemID) {
					// Add stats from the item
					for (let stat in items[customItemSlot][item]) {
						if (this.stats.hasOwnProperty(stat)) {
							this.stats[stat] += items[customItemSlot][item][stat];
						}
					}
					// Add stats from any gems equipped in the custom item
					if (settings.gems[customItemSlot] && settings.gems[customItemSlot][customItemID]) {
						for (let socket in settings.gems[customItemSlot][customItemID]) {
							for (let socketColor in gems) {
								for (let gem in gems[socketColor]) {
									if (gems[socketColor][gem].id == settings.gems[customItemSlot][customItemID][socket]) {
										for (let stat in gems[socketColor][gem]) {
											if (this.stats.hasOwnProperty(stat)) {
												this.stats[stat] += gems[socketColor][gem][stat];
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		this.stats.health = (this.stats.health + (this.stats.stamina * this.stats.staminaModifier) * healthPerStamina) * (1 + (0.01 * settings.talents.felStamina));
		this.stats.maxMana = (this.stats.mana + (this.stats.intellect * this.stats.intellectModifier) * manaPerInt) * (1 + (0.01 * settings.talents.felIntellect));
		this.stats.shadowModifier *= (1 + (0.02 * settings.talents.shadowMastery));
		this.spellTravelTime = 1;

		// Crit chance
		this.stats.critChanceMultiplier = 1000;
		this.stats.critChance = baseCritChancePercent + ((this.stats.critRating + ((this.stats.intellect * this.stats.intellectModifier) * critPerInt)) / critRatingPerPercent) + settings.talents.devastation + settings.talents.backlash + settings.talents.demonicTactics;
		if (settings.auras.moonkinAura) this.stats.critChance += 5;
		if (settings.auras.judgementOfTheCrusader) this.stats.critChance += 3;
		this.stats.critChance = Math.round(this.stats.critChance * this.stats.critChanceMultiplier); // Multiply the crit chance in order to use a whole number for RNG calculations.

		// Hit chance
		this.stats.hitChanceMultiplier = 1000;
		this.stats.extraHitChance = this.stats.hitRating / hitRatingPerPercent; // hit percent from hit rating
		if (settings.auras.inspiringPresence === true) this.stats.extraHitChance += 1;
		this.stats.hitChance = Math.round(this.getHitChance(parseInt(this.enemy.level)) * this.stats.hitChanceMultiplier); // The player's chance of hitting the enemy, between 61% and 99%

		// Assign the filler spell.
		this.filler = null;
		for (let spell in settings.rotation.filler) {
			if (settings.rotation.filler[spell]) {
				this.filler = spell;
				break;
			}
		}

		// Assign the curse (if selected)
		this.curse = null;
		for (let spell in settings.rotation.curse) {
			// Ignore the curse if user selected Curse of Agony since this will be the highest cast priority.
			if (settings.rotation.curse[spell] && spell !== "curseOfAgony") {
				this.curse = spell;
				break;
			}
		}

		// Add bonus damage % from Demonic Sacrifice
		if (this.talents.demonicSacrifice === 1) {
			// Add 15% to shadow modifier if the user selected Shadow Bolt, otherwise add 15% to the fire modifier if they selected Incinerate or Searing Pain.
			if (this.rotation.filler.shadowBolt) {
				this.stats.shadowModifier *= 1.15;
			} else if (this.rotation.filler.incinerate || this.rotation.filler.searingPain) {
				this.stats.fireModifier *= 1.15;
			}
		}

		// Add spell power from Fel Armor
		if (settings.auras.felArmor) {
			this.stats.spellPower += (100 * (1 + 0.1 * this.talents.demonicAegis));
		}

		// Records all information about damage done for each spell such as crit %, miss %, average damage per cast etc.
		this.damageBreakdown = {};

		console.clear();
		console.log("Health: " + Math.round(this.stats.health));
		console.log("Mana: " + Math.round(this.stats.maxMana));
		console.log("Stamina: " + Math.round(this.stats.stamina * this.stats.staminaModifier));
		console.log("Intellect: " + Math.round(this.stats.intellect * this.stats.intellectModifier));
		console.log("Spell Power: " + this.stats.spellPower);
		console.log("Shadow Power: " + this.stats.shadowPower);
		console.log("Fire Power: " + this.stats.firePower);
		console.log("Crit Chance: " + Math.round((this.stats.critChance / this.stats.critChanceMultiplier) * 100) / 100 + "%");
		console.log("Hit Chance: " + Math.round((this.stats.extraHitChance) * 100) / 100 + "%");
		console.log("Haste: " + Math.round((this.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 + "%");
		console.log("Shadow Modifier: " + Math.round(this.stats.shadowModifier * 100) + "%");
		console.log("Fire Modifier: " + Math.round(this.stats.fireModifier * 100) + "%");
		console.log("MP5: " + this.stats.mp5);
		console.log("Spell Penetration: " + this.stats.spellPen);
	}

	initialize() {
		this.spells = {
			"lifeTap": new LifeTap(this)
		}
		if (this.rotation.filler.shadowBolt) this.spells.shadowBolt = new ShadowBolt(this);
		else if (this.rotation.filler.searingPain) this.spells.searingPain = new SearingPain(this);
		else if (this.rotation.filler.incinerate) this.spells.incinerate = new Incinerate(this);
		if (this.rotation.dot.corruption) this.spells.corruption = new Corruption(this);
		if (this.rotation.dot.unstableAffliction) this.spells.unstableAffliction = new UnstableAffliction(this);
		if (this.rotation.dot.siphonLife) this.spells.siphonLife = new SiphonLife(this);
		if (this.rotation.dot.immolate) this.spells.immolate = new Immolate(this);
		if (this.rotation.curse.curseOfAgony) this.spells.curseOfAgony = new CurseOfAgony(this);
		if (this.rotation.curse.curseOfTheElements) this.spells.curseOfTheElements = new CurseOfTheElements(this);
		if (this.rotation.curse.curseOfRecklessness) this.spells.curseOfRecklessness = new CurseOfRecklessness(this);
		if (this.rotation.curse.curseOfDoom) this.spells.curseOfDoom = new CurseOfDoom(this); this.spells.curseOfAgony = new CurseOfAgony(this);
		if (this.rotation.finisher.shadowburn) this.spells.shadowburn = new Shadowburn(this);
		if (this.rotation.finisher.deathCoil) this.spells.deathCoil = new DeathCoil(this);

		this.auras = {};
		if (this.talents.improvedShadowBolt > 0) this.auras.improvedShadowBolt = new ImprovedShadowBolt(this);
		if (this.rotation.dot.corruption) this.auras.corruption = new CorruptionDot(this);
		if (this.rotation.dot.unstableAffliction) this.auras.unstableAffliction = new UnstableAfflictionDot(this);
		if (this.rotation.dot.siphonLife) this.auras.siphonLife = new SiphonLifeDot(this);
		if (this.rotation.dot.immolate) this.auras.immolate = new ImmolateDot(this);
		if (this.rotation.curse.curseOfAgony) this.auras.curseOfAgony = new CurseOfAgonyDot(this);
		if (this.rotation.curse.curseOfTheElements) this.auras.curseOfTheElements = new CurseOfTheElementsAura(this);
		if (this.rotation.curse.curseOfRecklessness) this.auras.curseOfRecklessness = new CurseOfRecklessnessAura(this);
		if (this.rotation.curse.curseOfDoom) this.auras.curseOfDoom = new CurseOfDoomAura(this); this.auras.curseOfAgony = new CurseOfAgonyDot(this);
		if (this.talents.nightfall > 0) this.auras.shadowTrance = new ShadowTrance(this);

		this.castTimeRemaining = 0;
		this.gcdValue = 1.5;
		this.gcdRemaining = 0;
		this.mana = this.stats.maxMana;
		this.mp5Timer = 5;
	}

	cast(spell) {
		this.spells[spell].startCast();
	}

	isHit(isAfflictionSpell) {
		if (isAfflictionSpell) {
			return (random(1,100 * this.stats.hitChanceMultiplier) <= (Math.min(99 * this.stats.hitChanceMultiplier,this.stats.hitChance + this.talents.suppression * 2 * this.stats.hitChanceMultiplier)));
		} else {
			return (random(1,100 * this.stats.hitChanceMultiplier) <= Math.min(99 * this.stats.hitChanceMultiplier,this.stats.hitChance));
		}
	}

	isCrit(extraCrit = 0) {
		return (random(1,(100 * this.stats.critChanceMultiplier)) <= (this.stats.critChance + extraCrit * this.stats.critChanceMultiplier));
	}

	// formula from https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/
	getHitChance(targetLevel) {
		if ((targetLevel - this.level) <= 2) {
			return Math.min(99, 100 - (targetLevel - this.level) - 4 + this.stats.extraHitChance);
		} else if ((targetLevel - this.level) == 3) { // target 3 levels above
			return Math.min(99, 83 + this.stats.extraHitChance);
		} else if ((targetLevel - this.level) == 4) { // target 4 levels above
			return Math.min(99, 72 + this.stats.extraHitChance);
		} else { // target 5+ levels above
			return Math.min(99, 61 + this.stats.extraHitChance);
		}
	}

	combatLog(info) {
		if (this.iteration == 1) {
			console.log("|" + (Math.round(this.fightTime * 10) / 10) + "|\t" + info);
		}
	}
}