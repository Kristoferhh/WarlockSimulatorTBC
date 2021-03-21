var socketInfo = {
	"meta": {
		"iconName": "UI-EmptySocket-Meta",
		"gems": ['meta']
	},
	"red": {
		"iconName": "UI-EmptySocket-Red",
		"gems": ['red','orange','purple']
	},
	"yellow": {
		"iconName": "UI-EmptySocket-Yellow",
		"gems": ['yellow','orange','green']
	},
	"blue": {
		"iconName": "UI-EmptySocket-Blue",
		"gems": ['blue','green','purple']
	}
}
//todo add 'phase' to gems
var gems = {
	"meta": {
		"chaoticSkyfireDiamond": {
			"name": "Chaotic Skyfire Diamond",
			"critRating": 12,
			"id": 34220,
			"iconName": "inv_misc_gem_diamond_07"
		}
	},
	"red": {
		"teardropCrimsonSpinel": {
			"name": "Teardrop Crimson Spinel",
			"spellPower": 12,
			'iconName': "inv_jewelcrafting_crimsonspinel_02",
			"id": 32195
		},
		"runedLivingRuby": {
			"name": "Runed Living Ruby",
			"spellPower": 9,
			"iconName": "inv_jewelcrafting_livingruby_03",
			"id": 24030
		},
		"starkBloodGarnet": {
			"name": "Stark Blood Garnet",
			"spellPower": 8,
			"iconName": "inv_misc_gem_ruby_01",
			"id": 27777
		},
		"runedBloodGarnate": {
			"name": "Runed Blood Garnet",
			"spellPower": 7,
			"iconName": "inv_misc_gem_bloodgem_02",
			"id": 23096
		},
		"runedTourmaline": {
			"name": "Runed Tourmaline",
			"spellPower": 5,
			"iconName": "inv_misc_gem_ruby_03",
			"id": 28461
		}
	},
	"yellow": {
		"rigidLionseye": {
			"name": "Rigid Lionseye",
			"hitRating": 10,
			"iconName": "inv_jewelcrafting_lionseye_02",
			"id": 32206
		},
		"smoothLionseye": {
			"name": "Smooth Lionseye",
			"critRating": 10,
			"iconName": "inv_jewelcrafting_lionseye_02",
			"id": 32291
		},
		"quickLionseye": {
			"name": "Quick Lionseye",
			"hasteRating": 10,
			"iconName": "inv_jewelcrafting_lionseye_02",
			"id": 35761
		},
		"brilliantLionseye": {
			"name": "Brilliant Lionseye",
			"intellect": 10,
			"iconName": "inv_jewelcrafting_lionseye_02",
			"id": 32204
		}
	},
	"blue": {
		"lustruousEmpyreanSapphire": {
			"name": "Lustruous Empyrean Sapphire",
			"mp5": 4,
			"iconName": "inv_jewelcrafting_empyreansapphire_02",
			"id": 32202
		},
		"sparklingEmpyreanSapphire": {
			"name": "Sparkling Empyrean Sapphire",
			"spirit": 10,
			"iconName": "inv_jewelcrafting_empyreansapphire_02",
			"id": 32201
		},
		"stormyEmpyreanSapphire": {
			"name": "Stormy Empyrean Sapphire",
			"spellPen": 13,
			"iconName": "inv_jewelcrafting_empyreansapphire_02",
			"id": 32203
		}
	},
	"orange": {
		"potentOrnateTopaz": {
			"name": "Potent Ornate Topaz",
			"spellPower": 6,
			"critRating": 5,
			"iconName": "inv_misc_gem_opal_02",
			"id": 28123
		},
		"veiledPyrestone": {
			"name": "Veiled Pyrestone",
			"spellPower": 6,
			"hitRating": 5,
			"iconName": "inv_jewelcrafting_pyrestone_02",
			"id": 32221
		},
		"potentPyrestone": {
			"name": "Potent Pyrestone",
			"spellPower": 6,
			"critRating": 5,
			"iconName": "inv_jewelcrafting_pyrestone_02",
			"id": 32304
		},
		"iridescentFireOpal": {
			"name": "Iridescent Fire Opal",
			"spellPower": 6,
			"critRating": 4,
			"iconName": "inv_jewelcrafting_nobletopaz_03",
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
			"id": 30560,
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
		"glowingTanzanite": {
			"name": "Glowing Tanzanite",
			"stamina": 6,
			"spellPower": 6,
			"iconName": "inv_jewelcrafting_nightseye_03",
			"id": 30555
		},
		"glowingNightseye": {
			"name": "Glowing Nightseye",
			"stamina": 6,
			"spellPower": 5,
			"iconName": "inv_jewelcrafting_nightseye_03",
			"id": 24056
		},
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