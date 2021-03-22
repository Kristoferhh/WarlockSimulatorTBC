var _auras = {
	"buffs": {
		"felArmor": {
			"name": "Fel Armor",
			"iconName": "spell_shadow_felarmour",
			"id": 44520
		},
		"blessingOfKings": {
			"name": "Blessing of Kings",
			"staminaModifier": 1.1,
			"intellectModifier": 1.1,
			"spiritModifier": 1.1,
			"iconName": "spell_magic_greaterblessingofkings",
			"id": 25898,
		},
		"blessingOfWisdom": {
			"name": "Blessing of Wisdom",
			"mp5": 41,
			"iconName": "spell_holy_greaterblessingofwisdom",
			"id": 25918,
		},
		"judgementOfWisdom": {
			"name": "Judgement of Wisdom",
			"iconName": "spell_holy_righteousnessaura",
			"id": 20354
		},
		"manaSpringTotem": {
			"name": "Mana Spring Totem",
			"mp5": 50,
			"iconName": "spell_nature_manaregentotem",
			"id": 10497
		},
		"wrathOfAirTotem": {
			"name": "Wrath of Air Totem",
			"spellPower": 101,
			"iconName": "spell_nature_slowingtotem",
			"id": 3738
		},
		"totemOfWrath": {
			"name": "Totem of Wrath",
			"iconName": "spell_fire_totemofwrath",
			"id": 30706
		},
		"markOfTheWild": {
			"name": "Mark of the Wild",
			"stamina": 14,
			"intellect": 14,
			"arcaneResist": 25,
			"shadowResist": 25,
			"frostResist": 25,
			"fireResist": 25,
			"natureResist": 25,
			"iconName": "spell_nature_regeneration",
			"id": 9885,
		},
		"arcaneIntellect": {
			"name": "Arcane Intellect",
			"intellect": 40,
			"iconName": "spell_holy_arcaneintellect",
			"id": 23028,
		},
		"prayerOfSpirit": {
			"name": "Prayer of Spirit",
			"spirit": 50,
			"iconName": "spell_holy_prayerofspirit",
			"id": 27681
		},
		"inspiringPresence": {
			"name": "Inspiring Presence",
			"iconName": "inv_staff_23",
			"id": 28878
		},
		"moonkinAura": {
			"name": "Moonkin Aura",
			"iconName": "spell_nature_moonglow",
			"id": 24907
		},
		"powerInfusion": {
			"name": "Power Infusion",
			"iconName": "spell_holy_powerinfusion",
			"id": 10060,
		},
		"powerOfTheGuardianWarlock": {
			"name": "Power of the Guardian",
			"spellPower": 33,
			"iconName": "spell_nature_moonglow",
			"id": 28143
		},
		"powerOfTheGuardianMage": {
			"name": "Power of the Guardian",
			"critRating": 28,
			"iconName": "spell_nature_moonglow",
			"id": 28142
		},
		"drumsOfBattle": {
			"name": "Drums of Battle",
			"iconName": "inv_misc_drum_02",
			"id": 35476
		},
		"drumsOfWar": {
			"name": "Drums of War",
			"iconName": "inv_misc_drum_03",
			"id": 35475
		},
		"drumsOfRestoration": {
			"name": "Drums of Restoration",
			"iconName": "inv_misc_drum_07",
			"id": 35478
		},
		"bloodlust": {
			"name": "Bloodlust",
			"iconName": "spell_nature_bloodlust",
			"id": 2825
		}
	},
	"debuffs": {
		"curseOfTheElements": {
			"name": "Curse of the Elements",
			"fireModifier": 1.1,
			"shadowModifier": 1.1,
			"arcaneModifier": 1.1,
			"frostModifier": 1.1,
			"fireResistReduction": 88,
			"shadowResistReduction": 88,
			"arcaneResistReduction": 88,
			"frostResistReduction": 88,
			"iconName": "spell_shadow_chilltouch",
			"id": 11722
		},
		"shadowWeaving": {
			"name": "Shadow Weaving",
			"shadowModifier": 1.1,
			"iconName": "spell_shadow_blackplague",
			"id": 15334
		},
		"improvedScorch": {
			"name": "Improved Scorch",
			"fireModifier": 1.15,
			"iconName": "spell_fire_soulburn",
			"id": 12873
		},
		"misery": {
			"name": "Misery",
			"shadowModifier": 1.05,
			"fireModifier": 1.05,
			"arcaneModifier": 1.05,
			"frostModifier": 1.05,
			"natureModifier": 1.05,
			"iconName": "Spell_shadow_misery",
			"id": 33200
		},
		"judgementOfTheCrusader": {
			"name": "Judgement of the Crusader",
			"iconName": "spell_holy_holysmite",
			"id": 21183
		}
	},
	"consumables": {
		"flaskOfPureDeath": {
			"name": "Flask of Pure Death",
			"shadowPower": 80,
			"firePower": 80,
			"frostPower": 80,
			"iconName": "inv_potion_115",
			"id": 22866,
			"battleElixir": true,
			"guardianElixir": true
		},
		"elixirOfMajorShadowPower": {
			"name": "Elixir of Major Shadow Power",
			"shadowPower": 55,
			"iconName": "inv_potion_145",
			"id": 22835,
			"battleElixir": true
		},
		"elixirOfMajorFirepower": {
			"name": "Elixir of Major Firepower",
			"firePower": 55,
			"iconName": "inv_potion_146",
			"id": 22833,
			"battleElixir": true
		},
		"greaterArcaneElixir": {
			"name": "Greater Arcane Elixir",
			"spellPower": 35,
			"iconName": "inv_potion_25",
			"id": 13454,
			"battleElixir": true
		},
		"adeptsElixir": {
			"name": "Adept's Elixir",
			"spellPower": 24,
			"critRating": 24,
			"iconName": "inv_potion_96",
			"id": 28103,
			"battleElixir": true
		},
		"elixirOfDraenicWisdom": {
			"name": "Elixir of Draenic Wisdom",
			"intellect": 30,
			"spirit": 30,
			"iconName": "inv_potion_155",
			"id": 32067,
			"guardianElixir": true
		},
		"elixirOfMajorMageblood": {
			"name": "Elixir of Major Mageblood",
			"mp5": 16,
			"iconName": "inv_potion_151",
			"id": 22840,
			"guardianElixir": true
		},
		"superManaPotion": {
			"name": "Super Mana Potion",
			"iconName": "inv_potion_137",
			"id": 22832,
			"potion": true,
		},
		"destructionPotion": {
			"name": "Destruction Potion",
			"iconName": "inv_potion_107",
			"id": 22839,
			"potion": true,
		},
		"blackenedBasilisk": {
			"name": "Blackened Basilisk",
			"spellPower": 23,
			"spirit": 20,
			"iconName": "inv_misc_food_86_basilisk",
			"id": 27657,
			"foodBuff": true,
		},
		"brilliantWizardOil": {
			"name": "Brilliant Wizard Oil",
			"spellPower": 36,
			"critRating": 14,
			"iconName": "inv_potion_105",
			"id": 20749,
			"weaponOil": true,
		},
		"superiorWizardOil": {
			"name": "Superior Wizard Oil",
			"spellPower": 42,
			"iconName": "inv_potion_141",
			"id": 22522,
			"weaponOil": true,
		},
		"blessedWizardOil": {
			"name": "Blessed Wizard Oil",
			"spellPower": 60,
			"iconName": "inv_potion_26",
			"id": 23123,
			"weaponOil": true,
		}
	}
}