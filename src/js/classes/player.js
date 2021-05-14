class Player {
	static getSettings() {
		return {
			"auras": auras,
			"items": selectedItems,
			"sets": JSON.parse(localStorage.setBonuses),
			"enchants": selectedEnchants,
			"gems": selectedGems,
			"talents": talents,
			"stats": characterStats,
			"simSettings": settings,
			"enemy": {
				"level": parseInt($("input[name='target-level']").val()),
				"shadowResist": parseInt($("input[name='target-shadow-resistance']").val()),
				"fireResist": parseInt($("input[name='target-fire-resistance']").val()),
				"armor": parseInt($("input[name='enemyArmor']").val())
			},
			"rotation": rotation,
			"selectedItemSlot": $("#item-slot-selection-list li[data-selected='true']").attr('data-slot')
		}
	}

	constructor(settings, customItemSlot = null, customItemSubSlot = "", customItemId = null) {
		this.stats = JSON.parse(JSON.stringify(settings.stats)); // Create a deep-copy of the character's stats since we need to modify the values.
		this.stats.manaCostModifier = 1;
		this.enemy = settings.enemy;
		this.rotation = settings.rotation;
		this.talents = settings.talents;
		this.simSettings = settings.simSettings;
		this.level = 70;
		this.itemId = customItemId || settings.items[settings.selectedItemSlot] || 0;
		this.sets = settings.sets;
		this.selectedAuras = settings.auras;
		this.enemy.shadowResist = Math.max(this.enemy.shadowResist, (this.enemy.level - this.level) * 8, 0);
		this.enemy.fireResist = Math.max(this.enemy.fireResist, (this.enemy.level - this.level) * 8, 0);
		this.trinketIds = [settings.items['trinket1'],settings.items['trinket2']];
		this.combatlog = [];
		this.importantAuraCounter = 0; // counts the amount of active "important" auras such as trinket procs, on-use trinket uses, power infusion etc.

		// If the player is equipped with a custom item then remove the stats from the currently equipped item and add stats from the custom item
		if (customItemSlot && customItemId && customItemId !== settings.items[customItemSlot + customItemSubSlot]) {
			// Loop through all items in the custom item slot
			for (let item in items[customItemSlot]) {
				// Check if this is the currently equipped item
				if (items[customItemSlot][item].id == settings.items[customItemSlot + customItemSubSlot]) {
					// Remove stats from currently equipped item
					for (let stat in items[customItemSlot][item]) {
						if (this.stats.hasOwnProperty(stat)) {
							this.stats[stat] -= items[customItemSlot][item][stat];
						}
					}
					if (items[customItemSlot][item].hasOwnProperty("setId")) {
						this.sets[items[customItemSlot][item].setId]--;
					}
					// If the item we're unequipping is a trinket we have equipped, then set its ID to null in the trinketIds array
					if (this.trinketIds.includes(items[customItemSlot][item].id)) {
						this.trinketIds[this.trinketIds.indexOf(items[customItemSlot][item].id)] = null;
					}
					// Remove stats from gems in the equipped item if there are any
					if (settings.gems[customItemSlot] && settings.gems[customItemSlot][settings.items[customItemSlot + customItemSubSlot]]) {
						// Loop through each socket in the equipped item
						for (let socket in settings.gems[customItemSlot][settings.items[customItemSlot + customItemSubSlot]]) {
							// Find the gem that is equipped in the socket by looking for its ID
							for (let socketColor in gems) {
								for (let gem in gems[socketColor]) {
									// Check if the ID matches
									if (gems[socketColor][gem].id == settings.gems[customItemSlot][settings.items[customItemSlot + customItemSubSlot]][socket]) {
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
				else if (items[customItemSlot][item].id == customItemId) {
					// Add stats from the item
					for (let stat in items[customItemSlot][item]) {
						if (this.stats.hasOwnProperty(stat)) {
							this.stats[stat] += items[customItemSlot][item][stat];
						}
					}
					if (items[customItemSlot][item].hasOwnProperty("setId")) {
						this.sets[items[customItemSlot][item].setId]++;
					}
					// Add stats from any gems equipped in the custom item
					if (settings.gems[customItemSlot] && settings.gems[customItemSlot][customItemId]) {
						for (let socket in settings.gems[customItemSlot][customItemId]) {
							for (let socketColor in gems) {
								for (let gem in gems[socketColor]) {
									if (gems[socketColor][gem].id == settings.gems[customItemSlot][customItemId][socket]) {
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
		
		// If neither of the trinkets' ids are null then either the test item is not a trinket or it's the item we already have equipped
		if (this.trinketIds.indexOf(null) !== -1) {
			this.trinketIds[this.trinketIds.indexOf(null)] = customItemId;
		}

		this.stats.health = (this.stats.health + (this.stats.stamina * this.stats.staminaModifier) * healthPerStamina) * (1 + (0.01 * settings.talents.felStamina));
		this.stats.maxMana = (this.stats.mana + (this.stats.intellect * this.stats.intellectModifier) * manaPerInt) * (1 + (0.01 * settings.talents.felIntellect));
		this.stats.shadowModifier *= (1 + (0.02 * settings.talents.shadowMastery));
		this.spellTravelTime = 1;

		// The Twin Stars 2-set bonus
		if (this.sets['667'] == 2) this.stats.spellPower += 15;

		// Crit chance
		this.stats.critChanceMultiplier = 1000;
		if (settings.auras.powerOfTheGuardianMage) this.stats.critRating += 28 * this.simSettings.mageAtieshAmount;
		this.stats.critChance = baseCritChancePercent + ((this.stats.critRating + ((this.stats.intellect * this.stats.intellectModifier) * critPerInt)) / critRatingPerPercent) + settings.talents.devastation + settings.talents.backlash + settings.talents.demonicTactics;
		if (settings.auras.moonkinAura) this.stats.critChance += 5;
		if (settings.auras.judgementOfTheCrusader) this.stats.critChance += 3;
		if (settings.auras.totemOfWrath) this.stats.critChance += 3;

		// Hit chance
		this.stats.hitChanceMultiplier = 1000;
		if (settings.sets['658'] >= 2) this.stats.hitRating += 35; // Mana-Etched Regalia 2-set bonus (35 hit rating)
		this.stats.extraHitChance = this.stats.hitRating / hitRatingPerPercent; // hit percent from hit rating
		if (settings.auras.inspiringPresence === true) this.stats.extraHitChance += 1;
		this.stats.hitChance = Math.round(this.getHitChance()); // The player's chance of hitting the enemy, usually between 83% and 99%

		// Add bonus damage % from Demonic Sacrifice
		if (settings.talents.demonicSacrifice === 1 && settings.simSettings.sacrificePet == 'yes') {
			if (settings.simSettings.petChoice == PetName.IMP) {
				this.stats.fireModifier *= 1.15;
			} else if (settings.simSettings.petChoice == PetName.SUCCUBUS) {
				this.stats.shadowModifier *= 1.15;
			} else if (settings.simSettings.petChoice == PetName.FELGUARD) {
				this.stats.shadowModifier *= 1.1;
				//todo add aura to regen mana and add Felhunter sacrifice
			}
		} else {
			// Add damage % multiplier from Master Demonologist and Soul Link
			if (settings.talents.soulLink > 0) {
				this.stats.shadowModifier *= 1.05;
				this.stats.fireModifier *= 1.05;
			}
			if (settings.talents.masterDemonologist > 0) {
				if ( settings.simSettings.petChoice == PetName.SUCCUBUS) {
					this.stats.shadowModifier *= 1.1;
					this.stats.fireModifier *= 1.1;
				} else if (settings.simSettings.petChoice == PetName.FELGUARD) {
					this.stats.shadowModifier *= 1.05;
					this.stats.fireModifier *= 1.05;
				}
			}
		}
		// Add % dmg modifiers from Curse of the Elements + Malediction
		if (settings.auras.curseOfTheElements) {
			this.stats.shadowModifier *= 1.1 + (0.01 * (settings.simSettings.improvedCurseOfTheElements || 0));
			this.stats.fireModifier *= 1.1 + (0.01 * (settings.simSettings.improvedCurseOfTheElements || 0));
		}
		// Add fire dmg % from Emberstorm
		if (settings.talents.emberstorm > 0) this.stats.fireModifier *= 1 + (0.02 * settings.talents.emberstorm);
		// Add spell power from Fel Armor
		if (settings.auras.felArmor) {
			this.stats.spellPower += (100 * (1 + 0.1 * this.talents.demonicAegis));
		}
		// Add spell power from Improved Divine Spirit
		this.stats.spiritModifier *= (1 - (0.01 * settings.talents.demonicEmbrace));
		if (settings.auras.prayerOfSpirit && settings.simSettings.improvedDivineSpirit) this.stats.spellPower += (this.stats.spirit * this.stats.spiritModifier * (0 + ((Number(settings.simSettings.improvedDivineSpirit) || 0) / 10)));
		// Add stamina from blood pact (if stamina is ever needed for the sim)
		// Add mp5 from Vampiric Touch
		if (settings.auras.vampiricTouch) {
			this.stats.mp5 += settings.simSettings.shadowPriestDps * 0.05;
		}
		if (settings.auras.powerOfTheGuardianWarlock) {
			this.stats.spellPower += 33 * settings.simSettings.warlockAtieshAmount;
		}

		// Trinkets
		this.trinkets = [];
		if (this.trinketIds.includes(32483)) this.trinkets.push(new SkullOfGuldan(this));
		if (this.trinketIds.includes(34429)) this.trinkets.push(new ShiftingNaaruSliver(this));
		if (this.trinketIds.includes(33829)) this.trinkets.push(new HexShrunkenHead(this));
		if (this.trinketIds.includes(29370)) this.trinkets.push(new IconOfTheSilverCrescent(this));
		if (this.trinketIds.includes(29132)) this.trinkets.push(new ScryersBloodgem(this));
		if (this.trinketIds.includes(23046)) this.trinkets.push(new RestrainedEssenceOfSapphiron(this));
		if (this.trinketIds.includes(29179)) this.trinkets.push(new XirisGift(this));
		if (this.trinketIds.includes(25620)) this.trinkets.push(new AncientCrystalTalisman(this));
		if (this.trinketIds.includes(28223)) this.trinkets.push(new ArcanistsStone(this));
		if (this.trinketIds.includes(25936)) this.trinkets.push(new TerokkarTabletOfVim(this));
		if (this.trinketIds.includes(28040)) this.trinkets.push(new VengeanceOfTheIllidari(this));
		if (this.trinketIds.includes(24126)) this.trinkets.push(new FigurineLivingRubySerpent(this));

		// Assign the filler spell.
		this.filler = null;
		for (let spell in settings.rotation.filler) {
			if (settings.rotation.filler[spell]) {
				this.filler = spell;
				break;
			}
		}
		if (this.filler == null) {
			this.filler = "shadowBolt";
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

		// Records all information about damage done for each spell such as crit %, miss %, average damage per cast etc.
		this.damageBreakdown = {};

		// Pet
		this.demonicKnowledgeSp = 0; // Spell Power from the Demonic Knowledge talent
		if (settings.simSettings.sacrificePet == 'no' || settings.talents.demonicSacrifice == 0) {
			let selectedPet = settings.simSettings.petChoice;
			if (selectedPet == PetName.IMP) this.pet = new Imp(this, settings);
			else if (selectedPet == PetName.VOIDWALKER) this.pet = new Voidwalker(this, settings);
			else if (selectedPet == PetName.SUCCUBUS) this.pet = new Succubus(this, settings);
			else if (selectedPet == PetName.FELHUNTER) this.pet = new Felhunter(this, settings);
			else if (selectedPet == PetName.FELGUARD && settings.talents.summonFelguard > 0) this.pet = new Felguard(this, settings);
			if (this.pet) {
				this.pet.initialize();
			}
		}

		this.combatlog.push('---------------- Player stats ----------------');
		this.combatlog.push("Health: " + Math.round(this.stats.health));
		this.combatlog.push("Mana: " + Math.round(this.stats.maxMana));
		this.combatlog.push("Stamina: " + Math.round(this.stats.stamina * this.stats.staminaModifier));
		this.combatlog.push("Intellect: " + Math.round(this.stats.intellect * this.stats.intellectModifier));
		this.combatlog.push("Spell Power: " + Math.round(this.stats.spellPower) + " + " + Math.round(this.demonicKnowledgeSp) + " from Demonic Knowledge");
		this.combatlog.push("Shadow Power: " + this.stats.shadowPower);
		this.combatlog.push("Fire Power: " + this.stats.firePower);
		this.combatlog.push("Crit Chance: " + Math.round(this.stats.critChance * 100) / 100 + "%");
		this.combatlog.push("Hit Chance: " + Math.round((this.stats.extraHitChance) * 100) / 100 + "%");
		this.combatlog.push("Haste: " + Math.round((this.stats.hasteRating / hasteRatingPerPercent) * 100) / 100 + "%");
		this.combatlog.push("Shadow Modifier: " + Math.round(this.stats.shadowModifier * 100) + "%");
		this.combatlog.push("Fire Modifier: " + Math.round(this.stats.fireModifier * 100) + "%");
		this.combatlog.push("MP5: " + this.stats.mp5);
		this.combatlog.push("Spell Penetration: " + this.stats.spellPen);
		if (this.pet) {
			this.combatlog.push('---------------- Pet stats ----------------');
			this.combatlog.push("Stamina: " + Math.round(this.pet.stats.stamina * this.pet.stats.staminaModifier));
			this.combatlog.push("Intellect: " + Math.round(this.pet.stats.intellect * this.pet.stats.intellectModifier));
			this.combatlog.push("Strength: " + Math.round((this.pet.stats.baseStats.strength + this.pet.stats.buffs.strength) * this.pet.stats.strengthModifier));
			this.combatlog.push("Agility: " + Math.round(this.pet.stats.agility * this.pet.stats.agilityModifier));
			this.combatlog.push("Spirit: " + Math.round(this.pet.stats.spirit * this.pet.stats.spiritModifier));
			this.combatlog.push("Attack Power: " + Math.round(this.pet.stats.ap * this.pet.stats.apModifier));
			this.combatlog.push("Spell Power: " + Math.round(this.pet.stats.spellPower));
			this.combatlog.push("Mana: " + Math.round(this.pet.stats.maxMana));
			this.combatlog.push("MP5: " + Math.round(this.pet.stats.mp5));
			this.combatlog.push("Hit Chance: " + this.pet.stats.hitChance + "%");
			this.combatlog.push("Crit Chance: " + Math.round(this.pet.stats.critChance) + "%");
			this.combatlog.push("Spell Hit Chance: " + this.pet.stats.spellHitChance + "%");
			this.combatlog.push("Spell Crit Chance: " + this.pet.stats.spellCritChance + "%");
			this.combatlog.push("Damage Modifier: " + Math.round(this.pet.stats.damageModifier * 100) + "%");
			if (this.pet.spells.lashOfPain) this.combatlog.push("Lash of Pain modifier: " + Math.round(this.pet.spells.lashOfPain.modifier * 100) + "% (as well as the normal pet damage % modifier)");
			if (this.pet.type == PetType.MELEE) this.combatlog.push("Enemy Armor: " + Math.round(this.enemy.armor) + " = " + Math.round((1 - this.pet.armorMultiplier) * 100) + "% physical damage reduction");
		}
	}

	initialize() {
		this.spells = {
			"lifeTap": new LifeTap(this)
		}
		if (this.rotation.filler.shadowBolt || this.filler == "shadowBolt") this.spells.shadowBolt = new ShadowBolt(this);
		else if (this.rotation.filler.searingPain) this.spells.searingPain = new SearingPain(this);
		else if (this.rotation.filler.incinerate) this.spells.incinerate = new Incinerate(this);
		if (this.rotation.dot.corruption) this.spells.corruption = new Corruption(this);
		if (this.rotation.dot.unstableAffliction) this.spells.unstableAffliction = new UnstableAffliction(this);
		if (this.rotation.dot.siphonLife) this.spells.siphonLife = new SiphonLife(this);
		if (this.rotation.dot.immolate) this.spells.immolate = new Immolate(this);
		if (this.rotation.curse.curseOfAgony || this.rotation.curse.curseOfDoom) this.spells.curseOfAgony = new CurseOfAgony(this);
		if (this.rotation.curse.curseOfTheElements) this.spells.curseOfTheElements = new CurseOfTheElements(this);
		if (this.rotation.curse.curseOfRecklessness) this.spells.curseOfRecklessness = new CurseOfRecklessness(this);
		if (this.rotation.curse.curseOfDoom) this.spells.curseOfDoom = new CurseOfDoom(this);
		if (this.rotation.finisher.shadowburn) this.spells.shadowburn = new Shadowburn(this);
		if (this.rotation.finisher.deathCoil) this.spells.deathCoil = new DeathCoil(this);
		if (this.selectedAuras.destructionPotion) this.spells.destructionPotion = new DestructionPotion(this);
		if (this.selectedAuras.superManaPotion) this.spells.superManaPotion = new SuperManaPotion(this);
		if (this.selectedAuras.demonicRune) this.spells.demonicRune = new DemonicRune(this);
		if (this.selectedAuras.flameCap) this.spells.flameCap = new FlameCap(this);
		if (this.simSettings.race == "orc") this.spells.bloodFury = new BloodFury(this);
		if (this.selectedAuras.bloodlust) this.spells.bloodlust = new Bloodlust(this);
		if (this.selectedAuras.drumsOfBattle) this.spells.drumsOfBattle = new DrumsOfBattle(this);
		if (this.selectedAuras.drumsOfWar) this.spells.drumsOfWar = new DrumsOfWar(this);
		if (this.selectedAuras.drumsOfRestoration) this.spells.drumsOfRestoration = new DrumsOfRestoration(this);

		this.auras = {};
		if (this.selectedAuras.powerInfusion) this.auras.powerInfusion = new PowerInfusion(this);
		if (this.simSettings.race == "orc") this.auras.bloodFury = new BloodFuryAura(this);
		if (this.talents.improvedShadowBolt > 0) this.auras.improvedShadowBolt = new ImprovedShadowBolt(this);
		if (this.rotation.dot.corruption) this.auras.corruption = new CorruptionDot(this);
		if (this.rotation.dot.unstableAffliction) this.auras.unstableAffliction = new UnstableAfflictionDot(this);
		if (this.rotation.dot.siphonLife) this.auras.siphonLife = new SiphonLifeDot(this);
		if (this.rotation.dot.immolate) this.auras.immolate = new ImmolateDot(this);
		if (this.rotation.curse.curseOfAgony || this.rotation.curse.curseOfDoom) this.auras.curseOfAgony = new CurseOfAgonyDot(this);
		if (this.rotation.curse.curseOfTheElements) this.auras.curseOfTheElements = new CurseOfTheElementsAura(this);
		if (this.rotation.curse.curseOfRecklessness) this.auras.curseOfRecklessness = new CurseOfRecklessnessAura(this);
		if (this.rotation.curse.curseOfDoom) this.auras.curseOfDoom = new CurseOfDoomDot(this);
		if (this.talents.nightfall > 0) this.auras.shadowTrance = new ShadowTrance(this);
		if (this.selectedAuras.destructionPotion) this.auras.destructionPotion = new DestructionPotionAura(this);
		if (this.selectedAuras.flameCap) this.auras.flameCap = new FlameCapAura(this);
		if (this.selectedAuras.bloodlust) this.auras.bloodlust = new BloodlustAura(this);
		if (this.selectedAuras.drumsOfBattle) this.auras.drumsOfBattle = new DrumsOfBattleAura(this);
		if (this.selectedAuras.drumsOfWar) this.auras.drumsOfWar = new DrumsOfWarAura(this);
		if (this.selectedAuras.drumsOfRestoration) this.auras.drumsOfRestoration = new DrumsOfRestorationAura(this);
		if (this.trinketIds.includes(28789)) this.auras.eyeOfMagtheridon = new EyeOfMagtheridon(this);
		if (this.trinketIds.includes(30626)) this.auras.sextantOfUnstableCurrents = new SextantOfUnstableCurrents(this);
		if (this.trinketIds.includes(27683)) this.auras.quagmirransEye = new QuagmirransEye(this);
		if (this.trinketIds.includes(28418)) this.auras.shiffarsNexusHorn = new ShiffarsNexusHorn(this);
		if (this.sets['645'] >= 2) {
			this.auras.shadowFlameshadow = new ShadowFlameShadow(this);
			this.auras.shadowFlamefire = new ShadowFlameFire(this);
		}
		if (this.sets['559'] == 2) this.auras.spellstrikeProc = new SpellstrikeProc(this);
		if (this.sets['658'] >= 4) this.auras.manaEtched4Set = new ManaEtched4Set(this);
	}

	reset() {
		this.castTimeRemaining = 0;
		this.gcdValue = 1.5;
		this.gcdRemaining = 0;
		this.mana = this.stats.maxMana;
		this.mp5Timer = 5;
	}

	cast(spell) {
		this.spells[spell].startCast();
	}

	areAnyCooldownsReady() {
		if (this.spells.bloodlust && this.spells.bloodlust.ready()) return true;
		if (this.auras.powerInfusion && this.auras.powerInfusion.ready()) return true;
		if (this.spells.destructionPotion && this.spells.destructionPotion.ready()) return true;
		for (let i = 0; i < this.trinkets.length; i++) {
			if (this.trinkets[i].ready()) return true;
		}
		if (this.spells.bloodFury && this.spells.bloodFury.ready()) return true;

		return false;
	}

	useCooldownsIfAvailable() {
		if (this.spells.bloodlust && this.spells.bloodlust.ready()) {
			this.cast("bloodlust");
		}
		if (this.auras.powerInfusion && this.auras.powerInfusion.ready()) {
			this.auras.powerInfusion.apply();
		}
		if (this.spells.destructionPotion && this.spells.destructionPotion.ready()) {
			this.cast("destructionPotion");
		}
		if (this.spells.flameCap && this.spells.flameCap.ready()) {
			this.cast("flameCap");
		}
		if (this.spells.bloodFury && this.spells.bloodFury.ready()) {
			this.cast("bloodFury");
		}
		for (let i = 0; i < this.trinkets.length; i++) {
			if (this.trinkets[i] && this.trinkets[i].ready()) {
				this.trinkets[i].use();
				// Set the other on-use trinket (if another is equipped) on cooldown for the duration of the trinket just used
				let otherTrinketSlot = i == 1 ? 2 : 1;
				if (this.trinkets[otherTrinketSlot]) {
					this.trinkets[otherTrinketSlot].cooldownRemaining = Math.max(this.trinkets[otherTrinketSlot].cooldownRemaining,this.trinkets[i].duration);
				}
			}
		}
	}

	isHit(isAfflictionSpell) {
		let hit;
		if (isAfflictionSpell) {
			hit = (random(1,100 * this.stats.hitChanceMultiplier) <= (Math.min(99 * this.stats.hitChanceMultiplier,(this.stats.hitChance + this.stats.extraHitChance + this.talents.suppression * 2) * this.stats.hitChanceMultiplier)));
		} else {
			hit = (random(1,100 * this.stats.hitChanceMultiplier) <= Math.min(99 * this.stats.hitChanceMultiplier,(this.stats.hitChance + this.stats.extraHitChance) * this.stats.hitChanceMultiplier));
		}

		// Eye of Magtheridon
		if (!hit && this.trinketIds.includes(28789)) {
			this.auras.eyeOfMagtheridon.apply();
		}

		return hit;
	}

	isCrit(extraCrit = 0) {
		return (random(1,(100 * this.stats.critChanceMultiplier)) <= ((this.stats.critChance + extraCrit) * this.stats.critChanceMultiplier));
	}

	// formula from https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/ && https://royalgiraffe.github.io/resist-guide
	getHitChance() {
		let levelDifference = parseInt(this.enemy.level) - this.level;
		if (levelDifference <= 2) {
			return Math.min(99, 100 - levelDifference - 4);
		} else if (levelDifference == 3) { // target 3 levels above
			return 83;
		} else if (levelDifference >= 4) { // target 4 or more levels above
			return 83 - 11 * levelDifference;
		}
	}

	combatLog(info) {
		if (this.iteration == 2) {
			this.combatlog.push("|" + (Math.round(this.fightTime * 10000) / 10000).toFixed(4) + "| " + info);
		}
	}
}