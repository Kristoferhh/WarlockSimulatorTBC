// States which color of gems can go into each of the 4 socket types
var socketColorToGemColor = {
	"meta": ['meta'],
	"red": ['red','orange','purple'],
	"yellow": ['yellow','orange','green'],
	"blue": ['blue','green','purple']
}
//todo add 'phase' to gems
var gems = {
	"meta": {
		"chaoticSkyfireDiamond": {
			"name": "Chaotic Skyfire Diamond",
			"spellPower": 12,
			"id": 34220,
			"iconName": "inv_misc_gem_diamond_07"
		}
	},
	"red": {
		"teardropCrimsonSpinel": {
			"name": "Teardrop Crimson Spinel",
			"spellPower": 12,
			"id": 32195
		}
	},
	"yellow": {
		"rigidLionseye": {
			"name": "Rigid Lionseye",
			"hitRating": 10,
			"id": 32206
		},
		"smoothLionseye": {
			"name": "Smooth Lionseye",
			"critRating": 10,
			"id": 32291
		},
		"quickLionseye": {
			"name": "Quick Lionseye",
			"hasteRating": 10,
			"id": 35761
		},
		"brilliantLionseye": {
			"name": "Brilliant Lionseye",
			"intellect": 10,
			"id": 32204
		}
	},
	"blue": {
		"lustruousEmpyreanSapphire": {
			"name": "Lustruous Empyrean Sapphire",
			"mp5": 4,
			"id": 32202
		},
		"sparklingEmpyreanSapphire": {
			"name": "Sparkling Empyrean Sapphire",
			"spirit": 10,
			"id": 32201
		},
		"stormyEmpyreanSapphire": {
			"name": "Stormy Empyrean Sapphire",
			"spellPen": 13,
			"id": 32203
		}
	},
	"orange": {
		"veiledPyrestone": {
			"name": "Veiled Pyrestone",
			"spellPower": 6,
			"hitRating": 5,
			"id": 32221
		},
		"potentPyrestone": {
			"name": "Potent Pyrestone",
			"spellPower": 6,
			"critRating": 5,
			"id": 32304
		},
		"iridescentFireOpal": {
			"name": "Iridescent Fire Opal",
			"spellPower": 6,
			"critRating": 4,
			"id": 30593
		},
		"recklessPyrestone": {
			"name": "Reckless Pyrestone",
			"spellPower": 6,
			"hasteRating": 5,
			"id": 35760,
			"iconName": "inv_jewelcrafting_pyrestone_02",
		},
		"luminousPyrestone": {
			"name": "Luminous Pyrestone",
			"spellPower": 6,
			"intellect": 5,
			"id": 32219,
			"iconName": "inv_jewelcrafting_pyrestone_02"
		},
		"infusedFireOpal": {
			"name": "Infused Fire Opal",
			"spellPower": 6,
			"intellect": 4,
			"id": 30551,
			"iconName": "inv_jewelcrafting_nobletopaz_03"
		},
		"mysteriousFireOpal": {
			"name": "Mysterious Fire Opal",
			"spellPower": 6,
			"spellPen": 5,
			"id": 30573,
			"iconName": "inv_jewelcrafting_nobletopaz_03"
		}
	},
	"green": {
		"lambentChrysoprase": {
			"name": "Lambent Chrysoprase",
			"hitRating": 5,
			"mp5": 2,
			"id": 30606,
			"iconName": "inv_jewelcrafting_talasite_03"
		},
		"runeCoveredChrysoprase": {
			"name": "Rune Covered Chrysoprase",
			"critRating": 5,
			"mp5": 2,
			"id": 30506,
			"iconName": "inv_jewelcrafting_talasite_03"
		},
		"radiantSeasprayEmerald": {
			"name": "Radiant Seaspray Emerald",
			"critRating": 5,
			"spellPen": 6,
			"id": 32224,
			"iconName": "inv_jewelcrafting_seasprayemerald_02"
		},
		"dazzlingSeasprayEmerald": {
			"name": "Dazzling Seaspray Emerald",
			"intellect": 5,
			"mp5": 2,
			"id": 32225,
			"iconName": "inv_jewelcrafting_seasprayemerald_02"
		}
	},
	"purple": {
		"royalShadowsongAmethyst": {
			"name": "Royal Shadowsong Amethyst",
			"spellPower": 6,
			"mp5": 2,
			"id": 32216,
			"iconName": "inv_jewelcrafting_shadowsongamethyst_02"
		},
		"purifiedShadowsongAmethyst": {
			"name": "Purified Shadowsong Amethyst",
			"spellPower": 6,
			"spirit": 5,
			"id": 37503,
			"iconName": "inv_jewelcrafting_shadowsongamethyst_02"
		},
		"fluorescentTanzanite": {
			"name": "Fluorescent Tanzanite",
			"spellPower": 6,
			"spirit": 4,
			"id": 30600,
			"iconName": "inv_jewelcrafting_nightseye_03"
		}
	}
}