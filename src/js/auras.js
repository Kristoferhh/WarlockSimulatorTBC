var auras = {
	"buffs": {
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
		"moonkinAura": {
			"name": "Moonkin Aura",
			"critRating": 110.5, // double check this
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
		},
		"drumsOfWar": {
			"name": "Drums of War",
			"iconName": "inv_misc_drum_03"
		},
		"bloodlust": {
			"name": "Bloodlust",
			"iconName": "spell_nature_bloodlust",
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
			"iconName": "Spell_shadow_misery" // missing wowhead id
		},
		"judgementOfTheCrusader": {
			"name": "Judgement of the Crusader",
			"critRating": 66.3, // double check this
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
			"battleElixir": true,
			"guardianElixir": true
		},
		"superManaPotion": {
			"name": "Super Mana Potion",
			"iconName": "inv_potion_137",
			"potion": true,
		},
		"destructionPotion": {
			"name": "Destruction Potion",
			"iconName": "inv_potion_107",
			"potion": true,
		},
		"blackenedBasilisk": {
			"name": "Blackened Basilisk",
			"spellPower": 23,
			"spirit": 20,
			"iconName": "inv_misc_food_86_basilisk",
			"foodBuff": true,
		},
		"brilliantWizardOil": {
			"name": "Brilliant Wizard Oil",
			"spellPower": 36,
			"crit": 14,
			"iconName": "inv_potion_105",
			"id": 20749,
			"weaponOil": true,
		},
		"superiorWizardOil": {
			"name": "Superior Wizard Oil",
			"spellPower": 42,
			"iconName": "inv_potion_141",
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