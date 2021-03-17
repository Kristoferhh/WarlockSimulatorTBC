var talentPointsRemaining = 61;
var _talents = {
	"affliction": {
		"suppression": {
			"name": "Suppression",
			"rankIDs": [18174,18175,18176,18177,18178],
			"iconName": "spell_shadow_unsummonbuilding",
			"row": 1,
			"column": 2
		},
		"improvedCorruption": {
			"name": "Improved Corruption",
			"rankIDs": [17810,17811,17812,17813,17814],
			"iconName": "spell_shadow_abominationexplosion",
			"row": 1,
			"column": 3
		},
		"improvedCurseOfWeakness": {
			"name": "Improved Curse of Weakness",
			"rankIDs": [18179,18180],
			"iconName": "spell_shadow_curseofmannoroth",
			"row": 2,
			"column": 1
		},
		"improvedDrainSoul": {
			"name": "Improved Drain Soul",
			"rankIDs": [18213,18372],
			"iconName": "spell_shadow_haunting",
			"row": 2,
			"column": 2
		},
		"improvedLifeTap": {
			"name": "Improved Life Tap",
			"rankIDs": [18182,18183],
			"iconName": "spell_shadow_burningspirit",
			"row": 2,
			"column": 3
		},
		"improvedDrainLife": {
			"name": "Improved Drain Life",
			"rankIDs": [17804,17805],
			"iconName": "spell_shadow_lifedrain02",
			"row": 2,
			"column": 4
		},
		"improvedCurseOfAgony": {
			"name": "Improved Curse of Agony",
			"rankIDs": [18827,18829],
			"iconName": "spell_shadow_curseofsargeras",
			"row": 3,
			"column": 1
		},
		"felConcentration": {
			"name": "Fel Concentration",
			"rankIDs": [17783,17784,17785,17786,17787],
			"iconName": "spell_shadow_fingerofdeath",
			"row": 3,
			"column": 2
		},
		"amplifyCurse": {
			"name": "Amplify Curse",
			"rankIDs": [18288],
			"iconName": "spell_shadow_contagion",
			"row": 3,
			"column": 3
		},
		"grimReach": {
			"name": "Grim Reach",
			"rankIDs": [18218,18219],
			"iconName": "spell_shadow_callofbone",
			"row": 4,
			"column": 1
		},
		"nightfall": {
			"name": "Nightfall",
			"rankIDs": [18094,18095],
			"iconName": "spell_shadow_twilight",
			"row": 4,
			"column": 2
		},
		"empoweredCorruption": {
			"name": "Empowered Corruption",
			"rankIDs": [17864,18393,0],
			"iconName": "spell_shadow_abominationexplosion",
			"row": 4,
			"column": 4
		},
		"shadowEmbrace": {
			"name": "Shadow Embrace",
			"rankIDs": [0,0,0,0,0],
			"iconName": "spell_shadow_shadowembrace",
			"row": 5,
			"column": 1
		},
		"siphonLife": {
			"name": "Siphon Life",
			"rankIDs": [18265],
			"iconName": "spell_shadow_requiem",
			"row": 5,
			"column": 2
		},
		"curseOfExhaustion": {
			"name": "Curse of Exhaustion",
			"rankIDs": [18223],
			"iconName": "spell_shadow_grimward",
			"row": 5,
			"column": 3,
			"requirement": 18288
		},
		"shadowMastery": {
			"name": "Shadow Mastery",
			"rankIDs": [18271,18272,18273,18274,18275],
			"iconName": "spell_shadow_shadetruesight",
			"row": 6,
			"column": 2,
			"requirement": 18265
		},
		"contagion": {
			"name": "Contagion",
			"rankIDs": [0,0,0,0,0],
			"iconName": "Spell_shadow_painfulafflictions",
			"row": 7,
			"column": 2
		},
		"darkPact": {
			"name": "Dark Pact",
			"rankIDs": [18220],
			"iconName": "spell_shadow_darkritual",
			"row": 7,
			"column": 3
		},
		"improvedHowlOfTerror": {
			"name": "Improved Howl of Terror",
			"rankIDs": [0,0],
			"iconName": "spell_shadow_deathscream",
			"row": 8,
			"column": 1
		},
		"malediction": {
			"name": "Malediction",
			"rankIDs": [0,0,0],
			"iconName": "spell_shadow_curseofachimonde",
			"row": 8,
			"column": 3
		},
		"unstableAffliction": {
			"name": "Unstable Affliction",
			"rankIDs": [0],
			"iconName": "spell_shadow_unstableaffliction_3",
			"row": 9,
			"column": 2,
			"requirement": 0 // set to 5/5 Contagion ID
		}
	},
	"demonology": {
		"improvedHealthstone": {
			"name": "Improved Healthstone",
			"rankIDs": [18692,18693],
			"iconName": "inv_stone_04",
			"row": 1,
			"column": 1
		},
		"improvedImp": {
			"name": "Improved Imp",
			"rankIDs": [18694,18695,18696],
			"iconName": "spell_shadow_summonimp",
			"row": 1,
			"column": 2
		},
		"demonicEmbrace": {
			"name": "Demonic Embrace",
			"rankIDs": [18697,18698,18699,18700,18701],
			"iconName": "spell_shadow_metamorphosis",
			"row": 1,
			"column": 3
		},
		"improvedHealthFunnel": {
			"name": "Improved Health Funnel",
			"rankIDs": [18703,18704],
			"iconName": "spell_shadow_lifedrain",
			"row": 2,
			"column": 1
		},
		"improvedVoidwalker": {
			"name": "Improved Voidwalker",
			"rankIDs": [18705,18706,18707],
			"iconName": "spell_shadow_summonvoidwalker",
			"row": 2,
			"column": 2
		},
		"felIntellect": {
			"name": "Fel Intellect",
			"rankIDs": [18731,18743,18744],
			"iconName": "spell_holy_magicalsentry",
			"row": 2,
			"column": 3
		},
		"improvedSuccubus": {
			"name": "Improved Succubus",
			"rankIDs": [18754,18755,18756],
			"iconName": "spell_shadow_summonsuccubus",
			"row": 3,
			"column": 1
		},
		"felDomination": {
			"name": "Fel Domination",
			"rankIDs": [18708],
			"iconName": "spell_nature_removecurse",
			"row": 3,
			"column": 2
		},
		"felStamina": {
			"name": "Fel Stamina",
			"rankIDs": [18748,18749,18750],
			"iconName": "spell_shadow_antishadow",
			"row": 3,
			"column": 3
		},
		"demonicAegis": {
			"name": "Demonic Aegis",
			"rankIDs": [18709,18710, 0],
			"iconName": "spell_shadow_ragingscream",
			"row": 3,
			"column": 4
		},
		"masterSummoner": {
			"name": "Master Summoner",
			"rankIDs": [18709,18710],
			"iconName": "spell_shadow_impphaseshift",
			"row": 4,
			"column": 2
		},
		"unholyPower": {
			"name": "Unholy Power",
			"rankIDs": [18769,18770,18771,18772,18773],
			"iconName": "spell_shadow_shadowworddominate",
			"row": 4,
			"column": 3
		},
		"improvedEnslaveDemon": {
			"name": "Improved Enslave Demon",
			"rankIDs": [18821,18822,18823,18824,18825],
			"iconName": "spell_shadow_enslavedemon",
			"row": 5,
			"column": 1
		},
		"demonicSacrifice":  {
			"name": "Demonic Sacrifice",
			"rankIDs": [18788],
			"iconName": "spell_shadow_psychicscream",
			"row": 5,
			"column": 2
		},
		"improvedFirestone": {
			"name": "Improved Firestone",
			"rankIDs": [18767,18768],
			"iconName": "inv_ammo_firetar",
			"row": 5,
			"column": 4
		},
		"manaFeed": {
			"name": "Mana Feed",
			"rankIDs": [0,0,0],
			"iconName": "spell_shadow_manafeed",
			"row": 6,
			"column": 1
		},
		"masterDemonologist": {
			"name": "Master Demonologist",
			"rankIDs": [23785,23822,23823,23824,23825],
			"iconName": "spell_shadow_shadowpact",
			"row": 6,
			"column": 3
		},
		"demonicResilience": {
			"name": "Demonic Resilience",
			"rankIDs": [0,0,0],
			"iconName": "spell_shadow_demonicfortitude",
			"row": 7,
			"column": 1
		},
		"soulLink": {
			"name": "Soul Link",
			"rankIDs": [19028],
			"iconName": "spell_shadow_gathershadows",
			"row": 7,
			"column": 2
		},
		"demonicKnowledge": {
			"name": "Demonic Knowledge",
			"rankIDs": [18774,18775,0],
			"iconName": "Spell_shadow_improvedvampiricembrace",
			"row": 7,
			"column": 3
		},
		"demonicTactics": {
			"name": "Demonic Tactics",
			"rankIDs": [0,0,0,0,0],
			"iconName": "Spell_shadow_demonictactics",
			"row": 8,
			"column": 2
		},
		"summonFelguard": {
			"name": "Summon Felguard",
			"rankIDs": [0],
			"iconName": "spell_shadow_summonfelguard",
			"row": 9,
			"column": 2
		}
	},
	"destruction": {
		"improvedShadowBolt": {
			"name": "Improved Shadow Bolt",
			"rankIDs": [17793,17796,17801,17802,17803],
			"iconName": "spell_shadow_shadowbolt",
			"row": 1,
			"column": 2
		},
		"cataclysm": {
			"name": "Cataclysm",
			"rankIDs": [17778,17779,17780,17781,17782],
			"iconName": "spell_fire_windsofwoe",
			"row": 1,
			"column": 3
		},
		"bane": {
			"name": "Bane",
			"rankIDs": [17788,17789,17790,17791,17792],
			"iconName": "spell_shadow_deathpact",
			"row": 2,
			"column": 2
		},
		"aftermath": {
			"name": "Aftermath",
			"rankIDs": [18119,18120,18121,18122,18123],
			"iconName": "spell_fire_fire",
			"row": 2,
			"column": 3
		},
		"improvedFirebolt": {
			"name": "Improved Firebolt",
			"rankIDs": [18126,18127],
			"iconName": "spell_fire_firebolt",
			"row": 3,
			"column": 1
		},
		"improvedLashOfPain": {
			"name": "Improved Lash of Pain",
			"rankIDs": [18128,18129],
			"iconName": "spell_shadow_curse",
			"row": 3,
			"column": 2
		},
		"devastation": {
			"name": "Devastation",
			"rankIDs": [18130,18131,18132,18133,18134],
			"iconName": "spell_fire_flameshock",
			"row": 3,
			"column": 3
		},
		"shadowburn": {
			"name": "Shadowburn",
			"rankIDs": [17877],
			"iconName": "spell_shadow_scourgebuild",
			"row": 3,
			"column": 4
		},
		"intensity": {
			"name": "Intensity",
			"rankIDs": [18135,18136],
			"iconName": "spell_fire_lavaspawn",
			"row": 4,
			"column": 1
		},
		"destructiveReach": {
			"name": "Destructive Reach",
			"rankIDs": [17917,17918],
			"iconName": "spell_shadow_corpseexplode",
			"row": 4,
			"column": 2
		},
		"improvedSearingPain": {
			"name": "Improved Searing Pain",
			"rankIDs": [17927,17929,17930],
			"iconName": "spell_fire_soulburn",
			"row": 4,
			"column": 4
		},
		"pyroclasm": {
			"name": "Pyroclasm",
			"rankIDs": [18096,18073],
			"iconName": "spell_fire_volcano",
			"row": 5,
			"column": 1
		},
		"improvedImmolate": {
			"name": "Improved Immolate",
			"rankIDs": [17815,17833,17834,17835,17836],
			"iconName": "spell_fire_immolation",
			"row": 5,
			"column": 2
		},
		"ruin": {
			"name": "Ruin",
			"rankIDs": [17959],
			"iconName": "spell_shadow_shadowwordpain",
			"row": 5,
			"column": 3
		},
		"netherProtection": {
			"name": "Nether Protection",
			"rankIDs": [0,0,0],
			"iconName": "spell_shadow_netherprotection",
			"row": 6,
			"column": 1
		},
		"emberstorm": {
			"name": "Emberstorm",
			"rankIDs": [17954,17955,17956,17957,17958],
			"iconName": "spell_fire_selfdestruct",
			"row": 6,
			"column": 3
		},
		"backlash": {
			"name": "Backlash",
			"rankIDs": [0,0,0],
			"iconName": "spell_fire_playingwithfire",
			"row": 7,
			"column": 1
		},
		"conflagrate": {
			"name": "Conflagrate",
			"rankIDs": [17962],
			"iconName": "spell_fire_fireball",
			"row": 7,
			"column": 2
		},
		"soulLeech": {
			"name": "Soul Leech",
			"rankIDs": [0,0,0],
			"iconName": "spell_shadow_soulleech_3",
			"row": 7,
			"column": 3
		},
		"shadowAndFlame": {
			"name": "Shadow and Flame",
			"rankIDs": [0,0,0,0,0],
			"iconName": "spell_shadow_shadowandflame",
			"row": 8,
			"column": 2
		},
		"shadowfury": {
			"name": "Shadowfury",
			"rankIDs": [0],
			"iconName": "spell_shadow_shadowfury",
			"row": 9,
			"column": 2
		}
	}
}