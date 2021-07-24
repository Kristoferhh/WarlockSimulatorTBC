// Made up item IDs that I use for items such as the bracers/boots from the Karazhan basement bosses where all the items use the same item id but they have different stats
// But each item in the sim needs to have a unique ID so I just make one up here and use that
const fakeItemIds = {
  lurkersCordOfShadowWrath: -1,
  lurkersCordOfFireWrath: -2,
  lurkersCordOfTheSorcerer: -3,
  lurkersCordOfTheInvoker: -4,
  ravagersCuffsOfFireWrath: -5,
  ravagersCuffsOfTheSorcerer: -6,
  ravagersCuffsOfTheInvoker: -7,
  glidersBootsOfShadowWrath: -8,
  glidersBootsOfFireWrath: -9,
  glidersBootsOfTheInvoker: -10,
  drakeweaveRaimentOfShadowWrath: -11,
  drakeweaveRaimentOfFireWrath: -12,
  drakeweaveRaimentOfTheSorcerer: -13,
  drakeweaveRaimentOfTheInvoker: -14,
  flawlessWandOfShadowWrath: -15,
  flawlessWandOfFireWrath: -16,
}

const items = {
  head: {
    darkConjurorsCollar: {
      name: "Dark Conjuror's Collar",
      stamina: 51,
      intellect: 42,
      meta: 1,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 75,
      hasteRating: 30,
      critRating: 38,
      id: 34340,
      source: 'Sunwell Plateau',
      phase: 5
    },
    helmOfArcanePurity: {
      name: 'Helm of Arcane Purity',
      stamina: 51,
      intellect: 42,
      spirit: 38,
      meta: 1,
      red: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 75,
      critRating: 30,
      id: 34405,
      source: 'Sunwell Plateau',
      phase: 5
    },
    annihilatorHologogs: {
      name: 'Annihilator Holo-gogs',
      stamina: 41,
      intellect: 37,
      critRating: 42,
      meta: 1,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 81,
      critRating: 42,
      id: 34847,
      source: 'Engineering',
      phase: 5
    },
    hoodOfHexing: {
      name: 'Hood of Hexing',
      stamina: 24,
      intellect: 33,
      red: 1,
      yellow: 1,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 56,
      hitRating: 31,
      critRating: 24,
      id: 33453,
      source: "Zul'Aman",
      phase: 4
    },
    vengefulGladiatorsFelweaveCowl: {
      name: "Vengeful Gladiator's Felweave Cowl",
      stamina: 67,
      intellect: 15,
      meta: 1,
      red: 1,
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 47,
      critRating: 22,
      resilienceRating: 33,
      id: 33683,
      source: 'Arena',
      phase: 4 // confirm
    },
    vengefulGladiatorsDreadweaveHood: {
      name: "Vengeful Gladiator's Dreadweave Hood",
      stamina: 73,
      intellect: 25,
      meta: 1,
      red: 1,
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 42,
      resilienceRating: 33,
      id: 33677,
      source: 'Arena',
      phase: 4 // confirm
    },
    cowlOfTheIllidariHighlord: {
      name: 'Cowl of the Illidari Highlord',
      stamina: 33,
      intellect: 31,
      meta: 1,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 64,
      hitRating: 21,
      critRating: 47,
      id: 32525,
      source: 'Black Temple',
      phase: 3
    },
    mercilessGladiatorsFelweaveCowl: {
      name: "Merciless Gladiator's Felweave Cowl",
      stamina: 60,
      intellect: 11,
      meta: 1,
      red: 1,
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 42,
      critRating: 18,
      resilienceRating: 33,
      id: 31980,
      source: 'Arena',
      phase: 3 // confirm
    },
    mercilessGladiatorsDreadweaveCowl: {
      name: "Merciless Gladiator's Dreadweave Cowl",
      stamina: 66,
      intellect: 20,
      meta: 1,
      red: 1,
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 36,
      resilienceRating: 33,
      id: 31974,
      source: 'Arena',
      phase: 3 // confirm
    },
    hoodOfTheMalefic: {
      name: 'Hood of the Malefic',
      stamina: 55,
      intellect: 36,
      meta: 1,
      yellow: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 63,
      hitRating: 16,
      critRating: 32,
      setId: 670,
      id: 31051,
      source: 'Hyjal Summit',
      phase: 3
    },
    manabindersCowl: {
      name: 'Mana-Binders Cowl',
      stamina: 38,
      intellect: 29,
      meta: 1,
      yellow: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 34,
      critRating: 15,
      id: 32089,
      source: "50 Badge of Justice - G'eras",
      phase: 1
    },
    destructionHologogs: {
      name: 'Destruction Holo-gogs',
      stamina: 22,
      intellect: 24,
      meta: 1,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 64,
      critRating: 29,
      id: 32494,
      source: 'Engineering',
      phase: 2
    },
    hoodOfTheCorruptor: {
      name: 'Hood of the Corruptor',
      stamina: 57,
      intellect: 33,
      meta: 1,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 55,
      critRating: 31,
      setId: 646,
      id: 30212,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    cowlOfTheGrandEngineer: {
      name: 'Cowl of the Grand Engineer',
      stamina: 22,
      intellect: 27,
      yellow: 2,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 53,
      hitRating: 16,
      critRating: 35,
      id: 29986,
      source: 'The Eye',
      phase: 2
    },
    gladiatorsFelweaveCowl: {
      name: "Gladiator's Felweave Cowl",
      stamina: 54,
      intellect: 15,
      meta: 1,
      red: 1,
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 37,
      critRating: 18,
      resilienceRating: 30,
      id: 30187,
      source: 'Arena',
      phase: 2 // confirm
    },
    gladiatorsDreadweaveHood: {
      name: "Gladiator's Dreadweave Hood",
      stamina: 60,
      intellect: 20,
      meta: 1,
      red: 1,
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 32,
      resilienceRating: 30,
      id: 24553,
      source: 'Arena',
      phase: 2 // confirm
    },
    collarOfChogall: {
      name: "Collar of Cho'gall",
      stamina: 42,
      intellect: 36,
      spellPower: 68,
      id: 28804,
      source: "Gruul's Lair",
      phase: 1
    },
    unimindHeaddress: {
      name: 'Uni-Mind Headdress',
      stamina: 31,
      intellect: 40,
      spellPower: 46,
      hitRating: 19,
      critRating: 25,
      id: 28744,
      source: 'Karazhan',
      phase: 1
    },
    wickedWitchsHat: {
      name: "Wicked Witch's Hat",
      stamina: 38,
      intellect: 37,
      spellPower: 43,
      critRating: 32,
      id: 28586,
      source: 'Karazhan',
      phase: 1
    },
    voidheartCrown: {
      name: 'Voidheart Crown',
      stamina: 39,
      intellect: 36,
      meta: 1,
      red: 1,
      socketBonus: {
        hitRating: 4
      },
      spellPower: 40,
      critRating: 19,
      setId: 645,
      id: 28963,
      source: 'Karazhan',
      phase: 1
    },
    spellstrikeHood: {
      name: 'Spellstrike Hood',
      stamina: 16,
      intellect: 12,
      red: 1,
      blue: 1,
      yellow: 1,
      socketBonus: {
        stamina: 6
      },
      spellPower: 46,
      critRating: 24,
      hitRating: 16,
      setId: 559,
      id: 24266,
      source: 'Tailoring',
      phase: 1
    },
    incantersCowl: {
      name: "Incanter's Cowl",
      stamina: 15,
      intellect: 27,
      spirit: 17,
      meta: 1,
      yellow: 1,
      socketBonus: {
        spirit: 4
      },
      spellPower: 29,
      critRating: 19,
      id: 28278,
      source: 'The Mechanar',
      phase: 1
    },
    gladiatorsFelweaveCowl: {
      name: "Gladiator's Felweave Cowl",
      stamina: 54,
      intellect: 15,
      meta: 1,
      red: 1,
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 37,
      critRating: 18,
      resilienceRating: 30,
      setId: 615,
      id: 30187,
      source: 'PVP',
      phase: 1
    },
    gladiatorsDreadweaveHood: {
      name: "Gladiator's Dreadweave Hood",
      stamina: 60,
      intellect: 20,
      meta: 1,
      red: 1,
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 32,
      resilienceRating: 30,
      setId: 568,
      id: 24553,
      source: 'PVP',
      phase: 1
    },
    manaEtchedCrown: {
      name: 'Mana-Etched Crown',
      stamina: 27,
      intellect: 20,
      meta: 1,
      red: 1,
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 34,
      spellPen: 15,
      setId: 658,
      id: 28193,
      source: 'Black Morass',
      phase: 1
    },
    hoodOfOblivion: {
      name: 'Hood of Oblivion',
      stamina: 27,
      intellect: 32,
      meta: 1,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 40,
      setId: 644,
      id: 28415,
      source: 'Arcatraz',
      phase: 1
    },
    maghariRitualistsHorns: {
      name: "Mag'hari Ritualist's Horns (Horde only)",
      stamina: 18,
      intellect: 16,
      spellPower: 50,
      critRating: 15,
      hitRating: 12,
      id: 28169,
      source: 'Nagrand Quest (Horde)',
      phase: 1
    },
    mageCollarOfTheFirestorm: {
      name: 'Mage-Collar of the Firestorm',
      stamina: 32,
      intellect: 33,
      spellPower: 39,
      critRating: 23,
      id: 27488,
      source: 'Heroic Blood Furnace',
      phase: 1
    },
    evokersHelmetOfSecondSight: {
      name: "Evoker's Helmet of Second Sight",
      stamina: 12,
      intellect: 15,
      spirit: 8,
      blue: 2,
      yellow: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 35,
      critRating: 24,
      id: 31104,
      source: 'Shadowmoon Valley Quest',
      phase: 1
    },
    demonfangRitualHelm: {
      name: 'Demonfang Ritual Helm',
      stamina: 39,
      intellect: 30,
      spellPower: 36,
      hitRating: 19,
      id: 27781,
      source: 'Heroic Underbog',
      phase: 1
    },
    hydromancersHeadwrap: {
      name: "Hydromancer's Headwrap",
      stamina: 21,
      intellect: 27,
      meta: 1,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 33,
      id: 28183,
      source: 'Steamvaults Quest',
      phase: 1
    },
    headdressOfAlacrity: {
      name: 'Headdress of Alacrity',
      stamina: 25,
      intellect: 33,
      spirit: 18,
      spellPower: 33,
      critRating: 17,
      id: 27466,
      source: 'Heroic Hellfire Ramparts',
      phase: 1
    },
    plagueheartCirclet: {
      name: 'Plagueheart Circlet',
      stamina: 28,
      intellect: 25,
      spellPower: 33,
      critRating: 28,
      hitRating: 8,
      setId: 529,
      id: 22506,
      source: 'Naxxramas',
      phase: 0
    }
  },
  neck: {
    pendantOfSunfire: {
      name: 'Pendant of Sunfire',
      stamina: 27,
      intellect: 19,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 34,
      critRating: 25,
      hasteRating: 25,
      id: 34359,
      source: 'Jewelcrafting',
      phase: 5
    },
    amuletOfUnfetteredMagics: {
      name: 'Amulet of Unfettered Magics',
      stamina: 24,
      intellect: 17,
      spellPower: 39,
      hasteRating: 32,
      hitRating: 15,
      id: 34204,
      source: 'Sunwell Plateau',
      phase: 5
    },
    sindoreiPendantOfConquest: {
      name: "Sin'dorei Pendant of Conquest",
      stamina: 18,
      intellect: 19,
      blue: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 34,
      critRating: 19,
      resilienceRating: 18,
      id: 35290,
      source: 'Sunwell Plateau',
      phase: 5
    },
    shatteredSunPendantOfAcumen: {
      name: 'Shattered Sun Pendant of Acumen',
      stamina: 19,
      intellect: 18,
      spellPower: 37,
      id: 34678,
      source: 'Shattered Sun Offensive - Exalted',
      phase: 5
    },
    loopOfCursedBones: {
      name: 'Loop of Cursed Bones',
      stamina: 19,
      intellect: 20,
      spellPower: 32,
      hasteRating: 27,
      id: 33466,
      source: "Zul'Aman",
      phase: 4
    },
    veteransPendantOfConquest: {
      name: "Veteran's Pendant of Conquest",
      stamina: 27,
      intellect: 12,
      yellow: 1,
      socketBonus: {
        stamina: 3
      },
      spellPower: 21,
      critRating: 18,
      resilienceRating: 18,
      id: 33067,
      source: 'PVP',
      phase: 2
    },
    veteransPendantOfDominance: {
      name: "Veteran's Pendant of Dominance",
      stamina: 31,
      intellect: 16,
      yellow: 1,
      socketBonus: {
        stamina: 3
      },
      spellPower: 26,
      resilienceRating: 18,
      id: 33065,
      source: 'PVP',
      phase: 2
    },
    translucentSpellthreadNecklace: {
      name: 'Translucent Spellthread Necklace',
      spellPower: 46,
      critRating: 24,
      hitRating: 15,
      id: 32349,
      source: 'Black Temple',
      phase: 3
    },
    hellfireEncasedPendant: {
      name: 'Hellfire-Encased Pendant',
      stamina: 16,
      intellect: 17,
      spirit: 12,
      firePower: 51,
      critRating: 24,
      id: 32589,
      source: 'Hyjal Summit & Black Temple',
      phase: 3
    },
    nadinasPendantOfPurity: {
      name: "Nadina's Pendant of Purity",
      stamina: 16,
      intellect: 14,
      critRating: 19,
      spellPower: 27,
      mp5: 8,
      id: 32370,
      source: 'Black Temple',
      phase: 3
    },
    theSunKingsTalisman: {
      name: "The Sun King's Talisman",
      stamina: 22,
      intellect: 16,
      spellPower: 41,
      critRating: 24,
      id: 30015,
      source: 'The Eye',
      phase: 2
    },
    pendantOfTheLostAges: {
      name: 'Pendant of the Lost Ages',
      stamina: 27,
      intellect: 17,
      spellPower: 36,
      id: 30008,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    ritssynsLostPendant: {
      name: "Ritssyn's Lost Pendant",
      stamina: 24,
      shadowPower: 51,
      id: 30666,
      source: 'Karazhan',
      phase: 1
    },
    manasurgePendant: {
      name: 'Manasurge Pendant',
      stamina: 24,
      intellect: 22,
      spellPower: 28,
      id: 29368,
      source: '25 Badge of Justice',
      phase: 1
    },
    adornmentOfStolenSouls: {
      name: 'Adornment of Stolen Souls',
      stamina: 18,
      intellect: 20,
      spellPower: 28,
      critRating: 23,
      id: 28762,
      source: 'Karazhan',
      phase: 1
    },
    warpEngineersPrismaticChain: {
      name: "Warp Engineer's Prismatic Chain",
      stamina: 17,
      intellect: 18,
      spellPower: 19,
      critRating: 16,
      id: 28254,
      source: 'The Mechanar',
      phase: 1
    },
    hydraFangNecklace: {
      name: 'Hydra-fang Necklace',
      stamina: 17,
      intellect: 16,
      spellPower: 19,
      hitRating: 16,
      id: 27758,
      source: 'Heroic Underbog',
      phase: 1
    },
    broochOfHeightenedPotential: {
      name: 'Brooch of Heightened Potential',
      stamina: 15,
      intellect: 14,
      spellPower: 22,
      critRating: 14,
      hitRating: 9,
      id: 28134,
      source: 'Shadow Labyrinth',
      phase: 1
    },
    broochOfUnquenchableFury: {
      name: 'Brooch of Unquenchable Fury',
      stamina: 24,
      intellect: 21,
      spellPower: 26,
      hitRating: 15,
      id: 28530,
      source: 'Karazhan',
      phase: 1
    },
    eyeOfTheNight: {
      name: 'Eye of the Night',
      critRating: 26,
      hitRating: 16,
      spellPen: 15,
      id: 24116,
      source: 'Jewelcrafting BoE',
      phase: 1
    },
    chainOfTheTwilightOwl: {
      name: 'Chain of the Twilight Owl',
      intellect: 19,
      spellPower: 21,
      id: 24121,
      source: 'Jewelcrafting BoE',
      phase: 1
    },
    luminousPearlsOfInsight: {
      name: 'Luminous Pearls of Insight',
      intellect: 15,
      spellPower: 25,
      critRating: 11,
      id: 24462,
      source: 'The Underbog',
      phase: 1
    },
    charlottesIvy: {
      name: "Charlotte's Ivy",
      stamina: 18,
      intellect: 19,
      spirit: 14,
      spellPower: 23,
      setId: 667,
      id: 31338,
      source: 'World Drop',
      phase: 1
    },
    natashasEmberNecklace: {
      name: "Natasha's Ember Necklace",
      intellect: 15,
      spellPower: 29,
      critRating: 10,
      id: 31692,
      source: "Blade's Edge Mountains Quest",
      phase: 1
    },
    torcOfTheSethekkProphet: {
      name: 'Torc of the Sethekk Prophet',
      intellect: 18,
      spellPower: 19,
      critRating: 21,
      id: 29333,
      source: 'Sethekk Halls Quest',
      phase: 1
    },
    omorsUnyieldingWill: {
      name: "Omor's Unyielding Will",
      intellect: 19,
      stamina: 19,
      spellPower: 25,
      id: 27464,
      source: 'Heroic Hellfire Ramparts',
      phase: 1
    },
    talismanOfTheBreaker: {
      name: 'Talisman of the Breaker',
      stamina: 18,
      intellect: 17,
      spellPower: 23,
      id: 29347,
      source: 'Heroic Blood Furnace',
      phase: 1
    },
    amuletOfVeknilash: {
      name: "Amulet of Vek'nilash",
      stamina: 9,
      intellect: 5,
      spellPower: 27,
      critRating: 14,
      id: 21608,
      source: 'AQ40',
      phase: 0
    },
    gemOfTrappedInnocents: {
      name: 'Gem of Trapper Innocents',
      stamina: 9,
      intellect: 7,
      spellPower: 15,
      critRating: 28,
      id: 23057,
      source: 'Naxxramas',
      phase: 0
    }
  },
  shoulders: {
    amiceOfTheConvoker: {
      name: 'Amice of the Convoker',
      stamina: 36,
      intellect: 28,
      red: 1,
      yellow: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 53,
      critRating: 22,
      hasteRating: 30,
      id: 34210,
      source: 'Sunwell Plateau',
      phase: 5
    },
    shoulderpadsOfKnowledgesPursuit: {
      name: "Shoulderpads of Knowledge's Pursuit",
      stamina: 33,
      intellect: 33,
      spirit: 22,
      red: 1,
      yellow: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 53,
      critRating: 26,
      id: 34393,
      source: 'Sunwell Plateau',
      phase: 5
    },
    felTingedMantle: {
      name: 'Fel-Tinged Mantle',
      stamina: 18,
      intellect: 20,
      yellow: 1,
      blue: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 35,
      critRating: 21,
      id: 34607,
      source: "Heroic Magisters' Terrace",
      phase: 5
    },
    duskhallowMantle: {
      name: 'Duskhallow Mantle',
      stamina: 12,
      intellect: 10,
      red: 1,
      yellow: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 29,
      critRating: 24,
      id: 34788,
      source: "Magisters' Terrace",
      phase: 5
    },
    mantleOfIllIntent: {
      name: 'Mantle of Ill Intent',
      stamina: 28,
      intellect: 24,
      spellPower: 40,
      hasteRating: 33,
      id: 33489,
      source: "Zul'Aman",
      phase: 4
    },
    mantleOfNimbleThought: {
      name: 'Mantle of Nimble Thought',
      stamina: 37,
      intellect: 26,
      spellPower: 44,
      hasteRating: 38,
      id: 32587,
      source: 'Tailoring',
      phase: 3
    },
    hatefuryMantle: {
      name: 'Hatefury Mantle',
      stamina: 15,
      intellect: 18,
      yellow: 1,
      blue: 1,
      socketBonus: {
        critRating: 3
      },
      spellPower: 55,
      spellPen: 23,
      critRating: 24,
      id: 30884,
      source: 'Hyjal Summit',
      phase: 3
    },
    mantleOfTheMalefic: {
      name: 'Mantle of the Malefic',
      stamina: 45,
      intellect: 22,
      blue: 1,
      yellow: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 46,
      hitRating: 21,
      critRating: 13,
      setId: 670,
      id: 31054,
      source: 'Black Temple',
      phase: 3
    },
    bloodcursedShoulderpads: {
      name: 'Blood-cursed Shoulderpads',
      stamina: 25,
      intellect: 19,
      spellPower: 55,
      critRating: 25,
      hitRating: 18,
      id: 32338,
      source: 'Black Temple',
      phase: 3
    },
    mercilessGladiatorsFelweaveAmice: {
      name: "Merciless Gladiator's Felweave Amice",
      stamina: 46,
      intellect: 10,
      blue: 1,
      yellow: 1,
      socketBonus: {
        resilienceRating: 3
      },
      spellPower: 36,
      critRating: 14,
      resilienceRating: 23,
      source: 'PVP',
      id: 31979,
      setId: 615,
      phase: 2
    },
    mercilessGladiatorsDreadweaveMantle: {
      name: "Merciless Gladiator's Dreadweave Mantle",
      stamina: 45,
      intellect: 10,
      blue: 1,
      yellow: 1,
      socketBonus: {
        resilienceRating: 3
      },
      spellPower: 36,
      resilienceRating: 21,
      hitRating: 13,
      source: 'PVP',
      id: 31976,
      setId: 568,
      phase: 2
    },
    mantleOfTheCorruptor: {
      name: 'Mantle of the Corruptor',
      stamina: 37,
      intellect: 24,
      spellPower: 41,
      hitRating: 18,
      yellow: 2,
      socketBonus: {
        spellPower: 4
      },
      setId: 646,
      id: 30215,
      source: 'The Eye',
      phase: 2
    },
    mantleOfTheElvenKings: {
      name: 'Mantle of the Elven Kings',
      stamina: 27,
      intellect: 18,
      spirit: 17,
      spellPower: 39,
      critRating: 25,
      hitRating: 18,
      id: 30024,
      source: 'The Eye',
      phase: 2
    },
    illidariShoulderpads: {
      name: 'Illidari Shoulderpads',
      stamina: 34,
      intellect: 23,
      yellow: 2,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 39,
      critRating: 16,
      id: 30079,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    voidheartMantle: {
      name: 'Voidheart Mantle',
      stamina: 26,
      intellect: 22,
      blue: 1,
      yellow: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 37,
      hitRating: 14,
      setId: 645,
      id: 28967,
      source: "Gruul's Lair",
      phase: 1
    },
    HWLGMDreadweaveMantle: {
      name: 'HWL/GM Dreadweave Mantle',
      stamina: 19,
      intellect: 15,
      yellow: 1,
      blue: 1,
      socketBonus: {
        resilienceRating: 3
      },
      id: 28820,
      hitRating: 6,
      resilienceRating: 17,
      spellPower: 26,
      setId: 592,
      source: 'PVP',
      phase: 1
    },
    frozenShadoweaveShoulders: {
      name: 'Frozen Shadoweave Shoulders',
      stamina: 21,
      intellect: 15,
      blue: 1,
      yellow: 1,
      socketBonus: {
        hitRating: 3
      },
      shadowPower: 50,
      setId: 553,
      id: 21869,
      source: 'Tailoring',
      phase: 1
    },
    mantleOfTheMindFlayer: {
      name: 'Mantle of the Mind Flayer',
      stamina: 33,
      intellect: 29,
      spellPower: 35,
      spellPen: 23,
      id: 28726,
      source: 'Karazhan',
      phase: 1
    },
    spauldersOfTheTornHeart: {
      name: 'Spaulders of the Torn-Heart',
      stamina: 10,
      intellect: 7,
      spirit: 8,
      spellPower: 40,
      critRating: 18,
      id: 30925,
      source: 'Shadowmoon Valley Quest',
      phase: 1
    },
    gladiatorsFelweaveAmice: {
      name: "Gladiator's Felweave Amice",
      stamina: 42,
      intellect: 14,
      blue: 1,
      yellow: 1,
      socketBonus: {
        resilienceRating: 3
      },
      spellPower: 32,
      critRating: 12,
      resilienceRating: 21,
      setId: 615,
      id: 30186,
      source: 'PVP',
      phase: 1
    },
    gladiatorsDreadweaveMantle: {
      name: "Gladiator's Dreadweave Mantle",
      stamina: 42,
      intellect: 14,
      blue: 1,
      yellow: 1,
      socketBonus: {
        resilienceRating: 3
      },
      spellPower: 32,
      resilienceRating: 21,
      hitRating: 12,
      setId: 568,
      id: 24554,
      source: 'PVP',
      phase: 1
    },
    manaSphereShoulderguards: {
      name: 'Mana-Sphere Shoulderguards',
      stamina: 23,
      intellect: 26,
      spirit: 17,
      spellPower: 29,
      id: 28374,
      source: 'The Arcatraz',
      phase: 1
    },
    incantersPauldrons: {
      name: "Incanter's Pauldrons",
      stamina: 24,
      intellect: 17,
      spirit: 16,
      red: 1,
      yellow: 1,
      socketBonus: {
        critRating: 3
      },
      spellPower: 20,
      id: 27738,
      source: 'The Steamvaults',
      phase: 1
    },
    manaEtchedSpaulders: {
      name: 'Mana-Etched Spaulders',
      stamina: 25,
      intellect: 17,
      yellow: 1,
      red: 1,
      socketBonus: {
        resilienceRating: 3
      },
      spellPower: 20,
      critRating: 16,
      setId: 658,
      id: 27796,
      source: 'Heroic Slave Pens',
      phase: 1
    },
    spauldersOfOblivion: {
      name: 'Spaulders of Oblivion',
      stamina: 25,
      intellect: 17,
      blue: 1,
      yellow: 1,
      socketBonus: {
        hitRating: 3
      },
      spellPower: 29,
      setId: 644,
      id: 27778,
      source: 'Shadow Labyrinth',
      phase: 1
    },
    pauldronsOfArcaneRage: {
      name: 'Pauldrons of Arcane Rage',
      stamina: 18,
      intellect: 18,
      spirit: 12,
      spellPower: 27,
      id: 24024,
      source: 'Hellfire Ramparts',
      phase: 1
    },
    mantleOfThreeTerrors: {
      name: 'Mantle of Three Terrors',
      stamina: 29,
      intellect: 25,
      spellPower: 29,
      hitRating: 12,
      id: 27994,
      source: 'Black Morass',
      phase: 1
    },
    mindragePauldrons: {
      name: 'Mindrage Pauldrons',
      stamina: 22,
      intellect: 15,
      spellPower: 34,
      spellPen: 10,
      id: 27816,
      source: 'Heroic Mana-Tombs',
      phase: 1
    },
    plagueheartShoulderpads: {
      name: 'Plagueheart Shoulderpads',
      stamina: 22,
      intellect: 12,
      spellPower: 36,
      hitRating: 8,
      setId: 529,
      id: 22507,
      source: 'Naxxramas',
      phase: 0
    }
  },
  back: {
    tatteredCapeOfAntonidas: {
      name: 'Tattered Cape of Antonidas',
      stamina: 25,
      intellect: 26,
      red: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 42,
      hasteRating: 32,
      id: 34242,
      source: 'Sunwell Plateau',
      phase: 5
    },
    cloakOfTheBetrayed: {
      name: 'Cloak of the Betrayed',
      stamina: 12,
      intellect: 12,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 23,
      hitRating: 13,
      id: 34792,
      source: "Magisters' Terrace",
      phase: 5
    },
    shadowcastersDrape: {
      name: "Shadow Caster's Drape",
      stamina: 22,
      intellect: 20,
      spellPower: 27,
      hasteRating: 25,
      id: 33591,
      source: "Zul'Aman",
      phase: 4
    },
    nethervoidCloak: {
      name: 'Nethervoid Cloak',
      stamina: 27,
      intellect: 18,
      shadowPower: 53,
      hitRating: 18,
      id: 32590,
      source: 'Hyjal Summit & Black Temple',
      phase: 3
    },
    cloakOfTheIllidariCouncil: {
      name: 'Cloak of the Illidari Council',
      stamina: 24,
      intellect: 16,
      spellPower: 42,
      critRating: 25,
      id: 32331,
      source: 'Black Temple',
      phase: 3
    },
    shroudOfTheHighborne: {
      name: 'Shroud of the Highborne',
      stamina: 24,
      intellect: 23,
      spellPower: 23,
      hasteRating: 32,
      id: 32524,
      source: 'Black Temple',
      phase: 3
    },
    royalCloakOfTheSunstriders: {
      name: 'Royal Cloak of the Sunstriders',
      stamina: 27,
      intellect: 22,
      spellPower: 44,
      id: 29992,
      source: 'The Eye',
      phase: 2
    },
    bruteCloakOfTheOgreMagi: {
      name: 'Brute Cloak of the Ogre-Magi',
      stamina: 18,
      intellect: 20,
      spellPower: 28,
      critRating: 23,
      id: 28797,
      source: "Gruul's Lair",
      phase: 1
    },
    sethekkOracleCloak: {
      name: 'Sethekk Oracle Cloak',
      stamina: 18,
      intellect: 18,
      spellPower: 22,
      hitRating: 12,
      id: 27981,
      source: 'Sethekk Halls',
      phase: 1
    },
    rubyDrapeOfTheMysticant: {
      name: 'Ruby Drape of the Mysticant',
      stamina: 22,
      intellect: 21,
      spellPower: 30,
      hitRating: 18,
      id: 28766,
      source: 'Karazhan',
      phase: 1
    },
    cloakOfEntropy: {
      name: 'Cloak of Entropy',
      intellect: 11,
      spellPower: 25,
      hitRating: 10,
      id: 31140,
      source: 'BoE World Drop',
      phase: 1
    },
    babasCloakOfArcanistry: {
      name: "Baba's Cloak of Arcanistry",
      stamina: 15,
      intellect: 15,
      spellPower: 22,
      critRating: 14,
      id: 28269,
      source: 'The Mechanar',
      phase: 1
    },
    cloakOfTheBlackVoid: {
      name: 'Cloak of the Black Void',
      intellect: 11,
      spellPower: 35,
      id: 24252,
      source: 'Tailoring',
      phase: 1
    },
    ancientSpellcloakOfTheHighborne: {
      name: 'Ancient Spellcloak of the Highborne',
      intellect: 15,
      spellPower: 36,
      critRating: 19,
      id: 30735,
      source: 'Doom Lord Kazzak',
      phase: 1
    },
    cloakOfWovenEnergy: {
      name: 'Cloak of Woven Energy',
      stamina: 6,
      intellect: 13,
      spirit: 3,
      spellPower: 29,
      critRating: 6,
      id: 29813,
      source: 'Netherstorm Quest',
      phase: 1
    },
    shawlOfShiftingProbabilities: {
      name: 'Shawl of Shifting Probabilities',
      stamina: 18,
      intellect: 16,
      spellPower: 21,
      critRating: 22,
      id: 29369,
      source: "25 Badge of Justice - G'eras",
      phase: 1
    },
    shadowCloakOfDalaran: {
      name: 'Shadow-Cloak of Dalaran',
      stamina: 19,
      intellect: 18,
      spellPower: 36,
      id: 28570,
      source: 'Karazhan',
      phase: 1
    },
    spellSlingersProtector: {
      name: "Spell-slinger's Protector",
      stamina: 15,
      intellect: 14,
      spirit: 9,
      spellPower: 16,
      critRating: 13,
      id: 28030,
      source: 'Nagrand Quest',
      phase: 1
    },
    embroideredCapeOfMysteries: {
      name: 'Embroidered Cape of Mysteries',
      stamina: 18,
      intellect: 20,
      spellPower: 25,
      id: 27485,
      source: 'Heroic Blood Furnace',
      phase: 1
    },
    sporeSoakedVaneer: {
      name: 'Spore-Soaked Vaneer',
      stamina: 15,
      intellect: 15,
      spellPower: 19,
      critRating: 11,
      id: 24362,
      source: 'Slave Pens',
      phase: 1
    },
    cloakOfTheNecropolis: {
      name: 'Cloak of the Necropolis',
      stamina: 12,
      intellect: 11,
      spellPower: 26,
      critRating: 14,
      hitRating: 8,
      id: 23050,
      source: 'Naxxramas',
      phase: 0
    },
    cloakOfTheDevoured: {
      name: 'Cloak of the Devoured',
      stamina: 11,
      intellect: 10,
      spellPower: 30,
      hitRating: 8,
      id: 22731,
      source: 'AQ40',
      phase: 0
    }
  },
  chest: {
    sunfireRobe: {
      name: 'Sunfire Robe',
      stamina: 36,
      intellect: 34,
      red: 3,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 71,
      critRating: 40,
      hasteRating: 40,
      id: 34364,
      source: 'Tailoring',
      phase: 5
    },
    felConquererRemains: {
      name: 'Fel Conquerer Remains',
      stamina: 60,
      intellect: 41,
      red: 1,
      yellow: 2,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 71,
      critRating: 24,
      hasteRating: 33,
      id: 34232,
      source: 'Sunwell Plateau',
      phase: 5
    },
    robesOfGhostlyHatred: {
      name: 'Robes of Ghostly Hatred',
      stamina: 39,
      intellect: 40,
      spirit: 32,
      red: 2,
      yellow: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 71,
      critRating: 26,
      hasteRating: 27,
      id: 34399,
      source: 'Sunwell Plateau',
      phase: 5
    },
    scarletSindoreiRobes: {
      name: "Scarlet Sin'dorei Robes",
      stamina: 31,
      intellect: 22,
      red: 1,
      blue: 1,
      yellow: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 51,
      critRating: 36,
      id: 34610,
      source: "Heroic Magisters' Terrace",
      phase: 5
    },
    robeOfDepartedSpirits: {
      name: 'Robe of Departed Spirits',
      stamina: 34,
      intellect: 31,
      spirit: 30,
      spellPower: 54,
      hasteRating: 35,
      id: 33317,
      source: "Zul'Aman",
      phase: 4
    },
    robesOfRhonin: {
      name: 'Robes of Rhonin',
      stamina: 55,
      intellect: 38,
      spellPower: 81,
      hitRating: 27,
      critRating: 24,
      id: 30913,
      source: 'Hyjal Summit',
      phase: 3
    },
    robeOfTheMalefic: {
      name: 'Robe of the Malefic',
      stamina: 66,
      intellect: 29,
      yellow: 2,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 63,
      hitRating: 28,
      setId: 670,
      id: 31052,
      source: 'Black Temple',
      phase: 3
    },
    robeOfTheShadowCouncil: {
      name: 'Robe of the Shadow Council',
      stamina: 37,
      intellect: 36,
      spirit: 26,
      spellPower: 73,
      critRating: 28,
      id: 32327,
      source: 'Black Temple',
      phase: 3
    },
    mercilessGladiatorsFelweaveRaiment: {
      name: "Merciless Gladiator's Felweave Raiment",
      stamina: 57,
      intellect: 12,
      red: 1,
      yellow: 2,
      socketBonus: {
        critRating: 4
      },
      spellPower: 35,
      resilienceRating: 26,
      critRating: 30,
      id: 31982,
      source: 'PVP',
      setId: 615,
      phase: 2
    },
    mercilessGladiatorsDreadweaveRobe: {
      name: "Merciless Gladiator's Dreadweave Robe",
      stamina: 69,
      intellect: 18,
      red: 1,
      yellow: 2,
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 44,
      resilienceRating: 24,
      id: 31977,
      setId: 568,
      source: 'PVP',
      phase: 2
    },
    robeOfHatefulEchoes: {
      name: 'Robe of Hateful Echoes',
      stamina: 34,
      intellect: 36,
      red: 1,
      yellow: 2,
      socketBonus: {
        stamina: 6
      },
      spellPower: 50,
      critRating: 25,
      id: 30056,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    vestmentsOfTheSeaWitch: {
      name: 'Vestments of the Sea-Witch',
      stamina: 28,
      intellect: 28,
      yellow: 2,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 57,
      critRating: 31,
      hitRating: 27,
      id: 30107,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    robeOfTheCorruptor: {
      name: 'Robe of the Corruptor',
      stamina: 48,
      intellect: 33,
      yellow: 3,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 55,
      hitRating: 23,
      setId: 646,
      id: 30214,
      source: 'The Eye',
      phase: 2
    },
    drakeweaveRaimentOfShadowWrath: {
      name: 'Drakeweave Raiment of Shadow Wrath',
      shadowPower: 85,
      displayId: 31158,
      id: fakeItemIds.drakeweaveRaimentOfShadowWrath,
      source: 'Hemathion - BEM Rare',
      phase: 1
    },
    drakeweaveRaimentOfFireWrath: {
      name: 'Drakeweave Raiment of Fire Wrath',
      firePower: 85,
      displayId: 31158,
      id: fakeItemIds.drakeweaveRaimentOfFireWrath,
      source: 'Hemathion - BEM Rare',
      phase: 1
    },
    /*drakeweaveRaimentOfTheSorcerer: {
      name: 'Drakeweave Raiment of the Sorcerer',
      stamina: 47,
      intellect: 32,
      spellPower: 37,
      displayId: 31158,
      id: fakeItemIds.drakeweaveRaimentOfTheSorcerer,
      source: 'Hemathion - BEM Rare',
      phase: 1
    },
    drakeweaveRaimentOfTheInvoker: {
      name: 'Drakeweave Raiment of the Invoker',
      intellect: 32,
      spellPower: 37,
      critRating: 32,
      displayId: 31158,
      id: fakeItemIds.drakeweaveRaimentOfTheInvoker,
      source: 'Hemathion - BEM Rare',
      phase: 1
    },*/
    voidheartRobe: {
      name: 'Voidheart Robe',
      stamina: 42,
      intellect: 33,
      yellow: 2,
      red: 1,
      socketBonus: {
        stamina: 6
      },
      spellPower: 42,
      hitRating: 13,
      setId: 645,
      id: 28964,
      source: "Magtheridon's Lair",
      phase: 1
    },
    frozenShadoweaveRobe: {
      name: 'Frozen Shadoweave Robe',
      stamina: 30,
      intellect: 20,
      blue: 1,
      yellow: 1,
      socketBonus: {
        hitRating: 3
      },
      shadowPower: 72,
      setId: 553,
      id: 21871,
      source: 'Tailoring',
      phase: 1
    },
    spellfireRobe: {
      name: 'Spellfire Robe',
      intellect: 17,
      yellow: 1,
      blue: 1,
      socketBonus: {
        stamina: 4
      },
      firePower: 72,
      critRating: 28,
      setId: 552,
      id: 21848,
      source: 'Tailoring',
      phase: 1
    },
    gladiatorsFelweaveRaiment: {
      name: "Gladiator's Felweave Raiment",
      stamina: 51,
      intellect: 16,
      red: 1,
      yellow: 2,
      socketBonus: {
        critRating: 4
      },
      spellPower: 32,
      resilienceRating: 24,
      critRating: 27,
      setId: 615,
      id: 30200,
      source: 'PVP',
      phase: 1
    },
    gladiatorsDreadweaveRobe: {
      name: "Gladiator's Dreadweave Robe",
      stamina: 57,
      intellect: 19,
      red: 1,
      yellow: 2,
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 36,
      resilienceRating: 24,
      setId: 568,
      id: 24552,
      source: 'PVP',
      phase: 1
    },
    willOfEdwardTheOdd: {
      name: 'Will of Edward the Odd',
      intellect: 30,
      spellPower: 53,
      critRating: 30,
      id: 31340,
      source: 'World Drop',
      phase: 1
    },
    warpInfusedDrape: {
      name: 'Warp Infused Drape',
      stamina: 27,
      intellect: 28,
      red: 1,
      yellow: 1,
      blue: 1,
      socketBonus: {
        critRating: 4
      },
      spellPower: 30,
      hitRating: 12,
      id: 28342,
      source: 'The Botanica',
      phase: 1
    },
    bloodfyreRobesOfAnnihilation: {
      name: 'Bloodfyre Robes of Annihilation',
      stamina: 27,
      intellect: 27,
      spellPower: 54,
      id: 28252,
      source: 'The Mechanar',
      phase: 1
    },
    anchoritesRobes: {
      name: "Anchorite's Robes",
      stamina: 16,
      intellect: 38,
      spirit: 18,
      yellow: 2,
      blue: 1,
      socketBonus: {
        mp5: 2
      },
      spellPower: 29,
      id: 29129,
      source: 'The Aldor - Honored',
      phase: 1
    },
    vermillionRobesOfTheDominant: {
      name: 'Vermillion Robes of the Dominant',
      stamina: 37,
      intellect: 33,
      spellPower: 42,
      hitRating: 12,
      id: 27799,
      source: 'The Steamvaults',
      phase: 1
    },
    manaEtchedVestments: {
      name: 'Mana-Etched Vestments',
      stamina: 25,
      intellect: 25,
      red: 1,
      blue: 1,
      yellow: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 29,
      critRating: 17,
      setId: 658,
      id: 28191,
      source: 'Heroic Old Hillsbrad Foothills',
      phase: 1
    },
    robeOfTheCrimsonOrder: {
      name: 'Robe of the Crimson Order',
      intellect: 23,
      spellPower: 50,
      hitRating: 30,
      id: 31297,
      source: 'BoE World Drop',
      phase: 1
    },
    robeOfTheGreatDarkBeyond: {
      name: 'Robe of the Great Dark Beyond',
      stamina: 30,
      intellect: 25,
      spirit: 18,
      spellPower: 39,
      critRating: 23,
      id: 27824,
      source: 'Mana-Tombs',
      phase: 1
    },
    robeOfTheElderScribes: {
      name: 'Robe of the Elder Scribes',
      stamina: 27,
      intellect: 29,
      spirit: 24,
      spellPower: 32,
      critRating: 24,
      id: 28602,
      source: 'Karazhan',
      phase: 1
    },
    robesOfTheAugurer: {
      name: 'Robes of the Augurer',
      stamina: 18,
      intellect: 18,
      spirit: 11,
      red: 1,
      blue: 1,
      yellow: 1,
      socketBonus: {
        critRating: 4
      },
      spellPower: 28,
      id: 24481,
      source: 'The Underbog',
      phase: 1
    },
    auchenaiAnchoritesRobe: {
      name: "Auchenai Anchorite's Robe",
      intellect: 24,
      red: 1,
      yellow: 2,
      socketBonus: {
        critRating: 4
      },
      spellPower: 28,
      hitRating: 23,
      id: 29341,
      source: 'Auchenai Crypts Quest',
      phase: 1
    },
    robeOfOblivion: {
      name: 'Robe of Oblivion',
      stamina: 30,
      intellect: 20,
      red: 1,
      blue: 1,
      yellow: 1,
      socketBonus: {
        stamina: 6
      },
      spellPower: 40,
      setId: 644,
      id: 28232,
      source: 'Shadow Labyrinth',
      phase: 1
    },
    incantersRobe: {
      name: "Incanter's Robe",
      stamina: 24,
      intellect: 22,
      spirit: 22,
      yellow: 2,
      red: 1,
      socketBonus: {
        intellect: 4
      },
      spellPower: 29,
      critRating: 8,
      id: 28229,
      source: 'Botanica',
      phase: 1
    },
    windchannelersTunicOfTheInvoker: {
      name: "Windchanneler's Tunic of the Invoker",
      intellect: 33,
      spellPower: 38,
      critRating: 33,
      id: 31554,
      source: 'Heroic Mana-Tombs',
      phase: 1
    },
    plagueheartRobe: {
      name: 'Plagueheart Robe',
      stamina: 27,
      intellect: 22,
      spellPower: 51,
      hitRating: 8,
      critRating: 14,
      setId: 529,
      id: 22504,
      source: 'Naxxramas',
      phase: 0
    }
  },
  bracer: {
    bracersOfTheMalefic: {
      name: 'Bracers of the Malefic',
      stamina: 18,
      intellect: 22,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 39,
      critRating: 18,
      hasteRating: 21,
      setId: 670,
      id: 34436,
      source: 'Sunwell Plateau',
      phase: 5
    },
    vindicatorsDreadweaveCuffs: {
      name: "Vindicator's Dreadweave Cuffs",
      stamina: 34,
      intellect: 20,
      spellPower: 29,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      id: 33883,
      source: 'PVP',
      phase: 5
    },
    bindingsOfRagingFire: {
      name: 'Bindings of Raging Fire',
      stamina: 9,
      intellect: 10,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 22,
      hasteRating: 18,
      id: 34697,
      source: "Magisters' Terrace",
      phase: 5
    },
    furyOfTheUrsine: {
      name: 'Fury of the Ursine',
      stamina: 12,
      intellect: 17,
      spirit: 16,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 29,
      critRating: 17,
      id: 33285,
      source: "Zul'Aman",
      phase: 4
    },
    bracersOfNimbleThought: {
      name: 'Bracers of Nimble Thought',
      stamina: 27,
      intellect: 20,
      spellPower: 34,
      hasteRating: 28,
      id: 32586,
      source: 'Tailoring BoE',
      phase: 3
    },
    focusedManaBindings: {
      name: 'Focused Mana Bindings',
      stamina: 27,
      intellect: 20,
      spellPower: 42,
      hitRating: 19,
      id: 32270,
      source: 'Black Temple',
      phase: 3
    },
    cuffsOfDevastation: {
      name: 'Cuffs of Devastation',
      stamina: 22,
      intellect: 20,
      spirit: 19,
      yellow: 1,
      socketBonus: {
        stamina: 3
      },
      spellPower: 34,
      critRating: 14,
      id: 30870,
      source: 'Hyjal Summit',
      phase: 3
    },
    bracersOfMartyrdom: {
      name: 'Bracers of Martyrdom',
      stamina: 15,
      intellect: 20,
      spirit: 28,
      blue: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 22,
      id: 30871,
      source: 'Hyjal Summit',
      phase: 3
    },
    mindstormWristbands: {
      name: 'Mindstorm Wristbands',
      stamina: 13,
      intellect: 13,
      spellPower: 36,
      critRating: 23,
      id: 29918,
      source: 'The Eye',
      phase: 2
    },
    veteransSilkCuffs: {
      name: "Veteran's Silk Cuffs",
      stamina: 25,
      intellect: 18,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 22,
      critRating: 14,
      resilienceRating: 13,
      id: 32820,
      source: 'PVP',
      phase: 2
    },
    veteransDreadweaveCuffs: {
      name: "Veteran's Dreadweave Cuffs",
      stamina: 31,
      intellect: 16,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 25,
      resilienceRating: 17,
      id: 32811,
      source: 'PVP',
      phase: 2
    },
    crimsonBracersOfGloom: {
      name: 'Crimson Bracers of Gloom',
      stamina: 18,
      intellect: 18,
      spellPower: 22,
      hitRating: 12,
      id: 27462,
      source: 'Heroic Hellfire Ramparts',
      phase: 1
    },
    ravagersCuffsOfShadowWrath: {
      name: "Ravager's Cuffs of Shadow Wrath",
      shadowPower: 58,
      id: 30684,
      source: 'Karazhan',
      phase: 1
    },
    ravagersCuffsOfFireWrath: {
      name: "Ravager's Cuffs of Fire Wrath",
      firePower: 58,
      displayId: 30684,
      id: fakeItemIds.ravagersCuffsOfFireWrath,
      source: 'Karazhan',
      phase: 1
    },
    ravagersCuffsOfTheSorcerer: {
      name: "Ravager's Cuffs of the Sorcerer",
      stamina: 32,
      intellect: 22,
      spellPower: 25,
      displayId: 30684,
      id: fakeItemIds.ravagersCuffsOfTheSorcerer,
      source: 'Karazhan',
      phase: 1
    },
    ravagersCuffsOfTheInvoker: {
      name: "Ravager's Cuffs of the Invoker",
      intellect: 22,
      spellPower: 25,
      critRating: 22,
      displayId: 30684,
      id: fakeItemIds.ravagersCuffsOfTheInvoker,
      source: 'Karazhan',
      phase: 1
    },
    illidariBracers: {
      name: 'Illidari Bracers of the Invoker',
      intellect: 17,
      spellPower: 20,
      critRating: 17,
      id: 31224,
      source: 'Ambassador Jerrikar - SMV Rare',
      phase: 1
    },
    marshalsSilkCuffs: {
      name: "Marshal's Silk Cuffs",
      stamina: 22,
      intellect: 17,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 20,
      resilienceRating: 11,
      critRating: 12,
      id: 29002,
      source: 'PVP',
      phase: 1
    },
    harbingerBands: {
      name: 'Harbinger Bands',
      stamina: 21,
      intellect: 21,
      spirit: 14,
      spellPower: 26,
      id: 28477,
      source: 'Karazhan',
      phase: 1
    },
    marshalsDreadweaveCuffs: {
      name: "Marshal's Dreadweave Cuffs",
      stamina: 25,
      intellect: 14,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 22,
      resilienceRating: 14,
      id: 28981,
      source: 'PVP',
      phase: 1
    },
    bandsOfNefariousDeeds: {
      name: 'Bands of Nefarious Deeds',
      stamina: 27,
      intellect: 22,
      spellPower: 32,
      id: 28515,
      source: 'Karazhan',
      phase: 1
    },
    arcaniumSignetBands: {
      name: 'Arcanium Signet Bands',
      stamina: 14,
      intellect: 15,
      spellPower: 30,
      id: 27746,
      source: 'Heroic Underbog',
      phase: 1
    },
    bracersOfHavok: {
      name: 'Bracers of Havok',
      intellect: 12,
      yellow: 1,
      socketBonus: {
        critRating: 2
      },
      spellPower: 30,
      id: 24250,
      source: 'Tailoring',
      phase: 1
    },
    shattrathWraps: {
      name: 'Shattrath Wraps',
      stamina: 15,
      intellect: 15,
      red: 1,
      socketBonus: {
        stamina: 3
      },
      spellPower: 21,
      id: 28174,
      source: 'Auchindon Quest',
      phase: 1
    },
    arcingBracers: {
      name: 'Arcing Bracers',
      stamina: 15,
      intellect: 15,
      spirit: 10,
      spellPower: 18,
      id: 24392,
      source: 'The Blood Furnace',
      phase: 1
    },
    bandsOfNetherkurse: {
      name: 'Bands of Netherkurse',
      intellect: 18,
      spirit: 13,
      spellPower: 21,
      spellPen: 15,
      id: 27517,
      source: 'The Shattered Halls',
      phase: 1
    },
    bandsOfNegotion: {
      name: 'Bands of Negotion',
      stamina: 25,
      intellect: 22,
      spellPower: 29,
      id: 29240,
      source: 'Heroic Mana-Tombs',
      phase: 1
    },
    rockfuryBracers: {
      name: 'Rockfury Bracers',
      stamina: 7,
      spellPower: 27,
      hitRating: 8,
      id: 21186,
      source: 'Silithus Quest',
      phase: 0
    },
    plagueheartBindings: {
      name: 'Plagueheart Bindings',
      stamina: 23,
      intellect: 14,
      spellPower: 23,
      setId: 529,
      id: 22511,
      source: 'Naxxramas',
      phase: 0
    }
  },
  gloves: {
    sunfireHandwraps: {
      name: 'Sunfire Handwraps',
      stamina: 33,
      intellect: 30,
      spellPower: 53,
      critRating: 37,
      id: 34366,
      source: 'Tailoring BoE',
      phase: 5
    },
    glovesOfTyrisPower: {
      name: "Gloves of Tyri's Power",
      stamina: 33,
      intellect: 32,
      spirit: 27,
      red: 1,
      yellow: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 47,
      hasteRating: 36,
      id: 34406,
      source: 'Sunwell Plateau',
      phase: 5
    },
    handguardsOfDefiledWorlds: {
      name: 'Handguards of Defiled Worlds',
      stamina: 33,
      intellect: 32,
      yellow: 1,
      red: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 47,
      hitRating: 27,
      hasteRating: 36,
      id: 34344,
      source: 'Sunwell Plateau',
      phase: 5
    },
    glovesOfArcaneAcuity: {
      name: 'Gloves of Arcane Acuity',
      stamina: 16,
      intellect: 20,
      red: 1,
      blue: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 34,
      hitRating: 18,
      id: 34808,
      source: "Magisters' Terrace",
      phase: 5
    },
    glovesOfTheMalefic: {
      name: 'Gloves of the Malefic',
      stamina: 57,
      intellect: 27,
      yellow: 1,
      socketBonus: {
        hitRating: 2
      },
      spellPower: 46,
      critRating: 19,
      hitRating: 11,
      setId: 670,
      id: 31050,
      source: 'Hyjal Summit',
      phase: 3
    },
    mercilessGladiatorsFelweaveHandguards: {
      name: "Merciless Gladiator's Felweave Handguards",
      stamina: 46,
      intellect: 15,
      spellPower: 36,
      critRating: 19,
      resilienceRating: 23,
      setId: 615,
      id: 31981,
      source: 'PVP',
      phase: 2
    },
    mercilessGladiatorsDreadweaveGloves: {
      name: "Merciless Gladiator's Dreadweave Gloves",
      stamina: 45,
      intellect: 25,
      spellPower: 36,
      resilienceRating: 21,
      id: 31973,
      setId: 568,
      source: 'PVP',
      phase: 2
    },
    glovesOfTheCorruptor: {
      name: 'Gloves of the Corruptor',
      stamina: 50,
      intellect: 24,
      spellPower: 42,
      critRating: 25,
      setId: 646,
      id: 30211,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    gauntletsOfTheSunKing: {
      name: 'Gauntlets of the Sun King',
      stamina: 28,
      intellect: 29,
      spirit: 20,
      spellPower: 42,
      critRating: 28,
      id: 29987,
      source: 'The Eye',
      phase: 2
    },
    voidheartGloves: {
      name: 'Voidheart Gloves',
      stamina: 34,
      intellect: 28,
      spellPower: 35,
      critRating: 22,
      setId: 645,
      id: 28968,
      source: 'Karazhan',
      phase: 1
    },
    soulEatersHandwraps: {
      name: "Soul-Eater's Handwraps",
      stamina: 31,
      intellect: 24,
      yellow: 1,
      blue: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 36,
      critRating: 21,
      id: 28780,
      source: "Magtheridon's Lair",
      phase: 1
    },
    handwrapsOfFlowingThought: {
      name: 'Handwraps of Flowing Thought',
      stamina: 24,
      intellect: 22,
      spellPower: 35,
      hitRating: 14,
      yellow: 1,
      blue: 1,
      socketBonus: {
        hitRating: 3
      },
      id: 28507,
      source: 'Karazhan',
      phase: 1
    },
    spellfireGloves: {
      name: 'Spellfire Gloves',
      intellect: 10,
      yellow: 1,
      blue: 1,
      socketBonus: {
        stamina: 4
      },
      firePower: 50,
      critRating: 23,
      setId: 552,
      id: 21847,
      source: 'Tailoring',
      phase: 1
    },
    glovesOfTheDeadwatcher: {
      name: 'Gloves of the Deadwatcher',
      stamina: 24,
      intellect: 24,
      spellPower: 29,
      hitRating: 18,
      id: 27493,
      source: 'Heroic Auchenai Crypts',
      phase: 1
    },
    glovesOfPandemonium: {
      name: 'Gloves of Pandemonium',
      intellect: 15,
      spellPower: 25,
      critRating: 22,
      hitRating: 10,
      id: 31149,
      source: 'BoE World Drop',
      phase: 1
    },
    gladiatorsFelweaveHandguards: {
      name: "Gladiator's Felweave Handguards",
      stamina: 39,
      intellect: 14,
      spellPower: 32,
      critRating: 21,
      resilienceRating: 21,
      setId: 615,
      id: 30188,
      source: 'PVP',
      phase: 1
    },
    angerSparkGloves: {
      name: 'Anger-Spark Gloves',
      red: 2,
      socketBonus: {
        critRating: 3
      },
      spellPower: 30,
      critRating: 25,
      hitRating: 20,
      id: 30725,
      source: 'Doomwalker',
      phase: 1
    },
    gladiatorsDreadweaveGloves: {
      name: "Gladiator's Dreadweave Gloves",
      stamina: 39,
      intellect: 25,
      spellPower: 32,
      resilienceRating: 21,
      setId: 568,
      id: 24556,
      source: 'PVP',
      phase: 1
    },
    manasparkGloves: {
      name: 'Manaspark Gloves',
      stamina: 14,
      intellect: 14,
      spirit: 10,
      red: 1,
      yellow: 1,
      socketBonus: {
        critRating: 3
      },
      spellPower: 16,
      hitRating: 15,
      id: 24450,
      source: 'The Underbog',
      phase: 1
    },
    manaEtchedGloves: {
      name: 'Mana-Etched Gloves',
      stamina: 25,
      intellect: 17,
      red: 1,
      yellow: 1,
      socketBonus: {
        resilienceRating: 1
      },
      spellPower: 20,
      critRating: 16,
      setId: 658,
      id: 27465,
      source: 'Heroic Hellfire Ramparts',
      phase: 1
    },
    glovesOfOblivion: {
      name: 'Gloves of Oblivion',
      stamina: 33,
      intellect: 21,
      spellPower: 26,
      hitRating: 20,
      setId: 644,
      id: 27537,
      source: 'Shattered Halls',
      phase: 1
    },
    tempestsTouch: {
      name: "Tempest's Touch",
      stamina: 10,
      intellect: 20,
      spirit: 6,
      blue: 2,
      socketBonus: {
        critRating: 3
      },
      spellPower: 27,
      spellPen: 9,
      id: 29317,
      source: 'Caverns of Time Quest',
      phase: 1
    },
    gripsOfTheVoid: {
      name: 'Grips of the Void',
      stamina: 18,
      intellect: 11,
      spellPower: 35,
      critRating: 10,
      id: 30930,
      source: 'Shadowmoon Valley Quest',
      phase: 1
    },
    energistArmwraps: {
      name: 'Energist Armwraps',
      stamina: 27,
      intellect: 26,
      spellPower: 34,
      id: 28317,
      source: 'The Botanica',
      phase: 1
    },
    incantersGloves: {
      name: "Incanter's Gloves",
      stamina: 21,
      intellect: 24,
      spirit: 12,
      spellPower: 29,
      critRating: 14,
      id: 27508,
      source: 'The Steamvaults',
      phase: 1
    },
    handsOfTheSun: {
      name: 'Hands of the Sun',
      stamina: 22,
      intellect: 23,
      firePower: 34,
      critRating: 21,
      id: 27764,
      source: 'Heroic Underbog',
      phase: 1
    },
    darkStormGauntlets: {
      name: 'Dark Storm Gauntlets',
      stamina: 19,
      intellect: 15,
      spellPower: 37,
      hitRating: 8,
      id: 21585,
      source: 'AQ40',
      phase: 0
    },
    plagueheartGloves: {
      name: 'Plagueheart Gloves',
      stamina: 25,
      intellect: 17,
      spellPower: 26,
      critRating: 14,
      setId: 529,
      id: 22509,
      source: 'Naxxramas',
      phase: 0
    }
  },
  belt: {
    beltOfTheMalefic: {
      name: 'Belt of the Malefic',
      stamina: 25,
      intellect: 29,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 50,
      hasteRating: 29,
      critRating: 20,
      hitRating: 20,
      setId: 670,
      id: 34541,
      source: 'Sunwell Plateau',
      phase: 5
    },
    anetheronsNoose: {
      name: "Anetheron's Noose",
      stamina: 22,
      intellect: 23,
      yellow: 1,
      blue: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 55,
      critRating: 24,
      id: 30888,
      source: 'Hyjal Summit',
      phase: 3
    },
    waistwrapOfInfinity: {
      name: 'Waistwrap of Infinity',
      stamina: 31,
      intellect: 22,
      spellPower: 56,
      hasteRating: 32,
      id: 32256,
      source: 'Black Temple',
      phase: 3
    },
    angelistasSash: {
      name: "Angelista's Sash",
      stamina: 29,
      intellect: 30,
      spellPower: 28,
      hasteRating: 37,
      id: 30895,
      source: 'Hyjal Summit',
      phase: 3
    },
    veteransSilkBelt: {
      name: "Veteran's Silk Belt",
      stamina: 39,
      intellect: 27,
      spellPower: 32,
      resilienceRating: 26,
      critRating: 27,
      id: 32807,
      source: 'PVP',
      phase: 2
    },
    veteransDreadweaveBelt: {
      name: "Veteran's Dreadweave Belt",
      stamina: 45,
      intellect: 30,
      spellPower: 36,
      resilienceRating: 31,
      id: 32799,
      source: 'PVP',
      phase: 2
    },
    beltOfBlasting: {
      name: 'Belt of Blasting',
      spellPower: 50,
      hitRating: 23,
      critRating: 30,
      blue: 1,
      yellow: 1,
      socketBonus: {
        spellPower: 4
      },
      id: 30038,
      source: 'Tailoring',
      phase: 2
    },
    cordOfScreamingTerrors: {
      name: 'Cord of Screaming Terrors',
      stamina: 34,
      intellect: 15,
      yellow: 2,
      socketBonus: {
        stamina: 4
      },
      spellPower: 50,
      hitRating: 24,
      id: 30064,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    fireCordOfTheMagus: {
      name: 'Fire-Cord of the Magus',
      stamina: 21,
      intellect: 23,
      firePower: 60,
      critRating: 30,
      id: 30020,
      source: 'The Eye',
      phase: 2
    },
    sashOfSealedFate: {
      name: 'Sash of Sealed Fate',
      intellect: 15,
      spellPower: 35,
      critRating: 23,
      id: 31283,
      source: 'World Drop',
      phase: 1
    },
    lurkersCordOfShadowWrath: {
      name: "Lurker's Cord of Shadow Wrath",
      shadowPower: 78,
      displayId: 30675,
      id: fakeItemIds.lurkersCordOfShadowWrath,
      source: 'Karazhan',
      phase: 1
    },
    lurkersCordOfFireWrath: {
      name: "Lurker's Cord of Fire Wrath",
      firePower: 78,
      displayId: 30675,
      id: fakeItemIds.lurkersCordOfFireWrath,
      source: 'Karazhan',
      phase: 1
    },
    lurkersCordOfTheSorcerer: {
      name: "Lurker's Cord of the Sorcerer",
      stamina: 43,
      intellect: 29,
      spellPower: 34,
      displayId: 30675,
      id: fakeItemIds.lurkersCordOfTheSorcerer,
      source: 'Karazhan',
      phase: 1
    },
    lurkersCordOfTheInvoker: {
      name: "Lurker's Cord of the Invoker",
      intellect: 28,
      spellPower: 33,
      critRating: 28,
      displayId: 30675,
      id: fakeItemIds.lurkersCordOfTheInvoker,
      source: 'Karazhan',
      phase: 1
    },
    nethershardGirdle: {
      name: 'Nethershard Girdle',
      stamina: 22,
      intellect: 30,
      spirit: 22,
      spellPower: 35,
      id: 28565,
      source: 'Karazhan',
      phase: 1
    },
    beltOfDivineInspiration: {
      name: 'Belt of Divine Inspiration',
      stamina: 27,
      intellect: 26,
      yellow: 1,
      blue: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 43,
      id: 28799,
      source: "Gruul's Lair",
      phase: 1
    },
    maleficGirdle: {
      name: 'Malefic Girdle',
      stamina: 27,
      intellect: 26,
      spellPower: 37,
      critRating: 21,
      id: 28654,
      source: 'Karazhan',
      phase: 1
    },
    infernoWaistCord: {
      name: 'Inferno Waist Cord',
      intellect: 18,
      firePower: 59,
      critRating: 24,
      id: 30673,
      source: 'Karazhan',
      phase: 1
    },
    spellfireBelt: {
      name: 'Spellfire Belt',
      intellect: 18,
      yellow: 1,
      blue: 1,
      socketBonus: {
        stamina: 4
      },
      firePower: 50,
      critRating: 18,
      setId: 552,
      id: 21846,
      source: 'Tailoring',
      phase: 1
    },
    girdleOfRuination: {
      name: 'Girdle of Ruination',
      stamina: 18,
      intellect: 13,
      red: 1,
      yellow: 1,
      socketBonus: {
        stamina: 4
      },
      spellPower: 39,
      critRating: 20,
      id: 24256,
      source: 'Tailoring',
      phase: 1
    },
    sashOfSerpentra: {
      name: 'Sash of Serpentra',
      stamina: 31,
      intellect: 21,
      spellPower: 25,
      hitRating: 17,
      id: 27795,
      source: 'Steamvaults',
      phase: 1
    },
    marshalsSilkBelt: {
      name: "Marshal's Silk Belt",
      stamina: 33,
      intellect: 22,
      spellPower: 28,
      critRating: 24,
      resilienceRating: 24,
      id: 29001,
      source: 'PVP',
      phase: 1
    },
    marshalsDreadweaveBelt: {
      name: "Marshal's Dreadweave Belt",
      stamina: 39,
      intellect: 27,
      spellPower: 32,
      resilienceRating: 27,
      id: 28980,
      source: 'PVP',
      phase: 1
    },
    beltOfDepravity: {
      name: 'Belt of Depravity',
      stamina: 31,
      intellect: 27,
      spellPower: 34,
      hitRating: 17,
      id: 29241,
      source: 'Heroic Arcatraz',
      phase: 1
    },
    mageFuryGirdle: {
      name: 'Mage-Fury Girdle',
      stamina: 22,
      intellect: 23,
      spellPower: 28,
      critRating: 20,
      id: 27742,
      source: 'Heroic Slave Pens',
      phase: 1
    },
    glyphLinedSash: {
      name: 'Glyph-Lined Sash',
      stamina: 21,
      intellect: 23,
      yellow: 2,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 30,
      critRating: 9,
      id: 27843,
      source: 'Heroic Mana-Tombs',
      phase: 1
    },
    adalsGift: {
      name: "A'dal's Gift",
      intellect: 25,
      spellPower: 34,
      critRating: 21,
      id: 31461,
      source: 'Tempest Keep Quest',
      phase: 1
    },
    oracleBeltOfTimelessMystery: {
      name: 'Orcale Belt of Timeless Mystery',
      stamina: 22,
      intellect: 24,
      spirit: 17,
      spellPower: 29,
      id: 27768,
      source: 'Heroic Underbog',
      phase: 1
    },
    mindfireWaistband: {
      name: 'Mindfire Waistband',
      stamina: 10,
      intellect: 14,
      spirit: 8,
      yellow: 1,
      blue: 1,
      socketBonus: {
        hitRating: 3
      },
      spellPower: 21,
      critRating: 11,
      id: 24395,
      source: 'The Blood Furnace',
      phase: 1
    },
    sashOfArcaneVisions: {
      name: 'Sash of Arcane Visions',
      stamina: 18,
      intellect: 23,
      spirit: 19,
      spellPower: 28,
      critRating: 22,
      id: 29257,
      source: 'Heroic Auchenai Crypts',
      phase: 1
    },
    eyestalkWaistCord: {
      name: 'Eyestalk Waist Cord',
      stamina: 10,
      intellect: 9,
      spellPower: 41,
      critRating: 14,
      id: 22730,
      source: 'AQ40',
      phase: 0
    },
    plagueheartBelt: {
      name: 'Plagueheart Belt',
      stamina: 23,
      intellect: 12,
      spellPower: 34,
      critRating: 14,
      setId: 529,
      id: 22510,
      source: 'Naxxramas',
      phase: 0
    }
  },
  legs: {
    pantaloonsOfGrowingStrife: {
      name: 'Pantaloons of Growing Strife',
      stamina: 29,
      intellect: 36,
      spirit: 25,
      red: 1,
      yellow: 2,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 71,
      hasteRating: 42,
      id: 34386,
      source: 'Sunwell Plateau',
      phase: 5
    },
    leggingsOfCalamity: {
      name: 'Leggings of Calamity',
      stamina: 48,
      intellect: 41,
      red: 2,
      yellow: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 71,
      hasteRating: 32,
      critRating: 33,
      id: 34181,
      source: 'Sunwell Plateau',
      phase: 5
    },
    leggingsOfChanneledElements: {
      name: 'Legs of Channeled Elements',
      stamina: 25,
      intellect: 28,
      spirit: 28,
      yellow: 2,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 59,
      hitRating: 18,
      critRating: 34,
      id: 30916,
      source: 'Hyjal Summit',
      phase: 3
    },
    leggingsOfDevastation: {
      name: 'Leggings of Devastation',
      stamina: 40,
      intellect: 42,
      yellow: 2,
      blue: 1,
      socketBonus: {
        spellPower: 3
      },
      spellPower: 60,
      hitRating: 26,
      id: 32367,
      source: 'Black Temple',
      phase: 3
    },
    leggingsOfTheMalefic: {
      name: 'Leggings of the Malefic',
      stamina: 55,
      intellect: 44,
      yellow: 1,
      socketBonus: {
        hitRating: 2
      },
      spellPower: 62,
      critRating: 37,
      hitRating: 19,
      setId: 670,
      id: 31053,
      source: 'Black Temple',
      phase: 3
    },
    mercilessGladiatorsFelweaveTrousers: {
      name: "Merciless Gladiator's Felweave Trousers",
      stamina: 60,
      intellect: 20,
      spellPower: 49,
      critRating: 29,
      resilienceRating: 30,
      source: 'PVP',
      id: 31983,
      setId: 615,
      phase: 2
    },
    mercilessGladiatorsDreadweaveLeggings: {
      name: "Merciless Gladiator's Dreadweave Trousers",
      stamina: 69,
      intellect: 27,
      spellPower: 53,
      resilienceRating: 33,
      source: 'PVP',
      id: 31975,
      setId: 568,
      phase: 2
    },
    leggingsOfTheCorruptor: {
      name: 'Leggings of the Corruptor',
      stamina: 48,
      intellect: 32,
      yellow: 1,
      socketBonus: {
        stamina: 3
      },
      spellPower: 55,
      hitRating: 24,
      critRating: 32,
      setId: 646,
      id: 30213,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    trousersOfTheAstromancer: {
      name: 'Trousers of the Astromancer',
      stamina: 33,
      intellect: 36,
      spirit: 22,
      blue: 2,
      yellow: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 54,
      id: 29972,
      source: 'The Eye',
      phase: 2
    },
    voidheartLeggings: {
      name: 'Voidheart Leggings',
      stamina: 42,
      intellect: 38,
      spellPower: 49,
      critRating: 25,
      hitRating: 17,
      setId: 645,
      id: 28966,
      source: "Gruul's Lair",
      phase: 1
    },
    spellstrikePants: {
      name: 'Spellstrike Pants',
      stamina: 12,
      intellect: 8,
      blue: 1,
      red: 1,
      yellow: 1,
      socketBonus: {
        stamina: 6
      },
      spellPower: 46,
      critRating: 26,
      hitRating: 22,
      setId: 559,
      id: 24262,
      source: 'Tailoring',
      phase: 1
    },
    gladiatorsFelweaveTrousers: {
      name: "Gladiator's Felweave Trousers",
      stamina: 54,
      intellect: 25,
      spellPower: 42,
      resilienceRating: 30,
      critRating: 28,
      setId: 615,
      id: 30201,
      source: 'PVP',
      phase: 1
    },
    gladiatorsDreadweaveLeggings: {
      name: "Gladiator's Dreadweave Leggings",
      stamina: 60,
      intellect: 30,
      spellPower: 49,
      resilienceRating: 30,
      id: 24555,
      setId: 568,
      source: 'PVP',
      phase: 1
    },
    leggingsOfTheSeventhCircle: {
      name: 'Leggings of the Seventh Circle',
      intellect: 22,
      red: 1,
      yellow: 2,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 50,
      critRating: 25,
      hitRating: 18,
      id: 30734,
      source: 'Doom Lord Kazzak',
      phase: 1
    },
    devilStitchedLegs: {
      name: 'Devil-Stitched Legs',
      stamina: 32,
      intellect: 28,
      red: 1,
      yellow: 1,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 29,
      id: 28338,
      source: 'The Botanica',
      phase: 1
    },
    princelyReignLeggings: {
      name: 'Princely Reign Leggings',
      stamina: 18,
      intellect: 28,
      spirit: 12,
      spellPower: 33,
      hitRating: 18,
      id: 24359,
      source: 'Slave Pens',
      phase: 1
    },
    aransSorcerousSlacks: {
      name: "Aran's Sorcerous Slacks",
      stamina: 29,
      intellect: 28,
      red: 1,
      blue: 1,
      yellow: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 23,
      critRating: 21,
      id: 28212,
      source: 'Heroic Old Hillsbrad Foothills',
      phase: 1
    },
    leggingsOfTheSkettisElite: {
      name: 'Leggings of the Skettis Elite',
      intellect: 33,
      spirit: 33,
      spellPower: 39,
      id: 30836,
      source: 'Lower City - Revered',
      phase: 1
    },
    trialFireTrousers: {
      name: 'Trial-Fire Trousers',
      stamina: 42,
      intellect: 40,
      yellow: 3,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 49,
      id: 28594,
      source: 'Karazhan',
      phase: 1
    },
    breechesOfTheOccultist: {
      name: 'Breeches of the Occulist',
      stamina: 37,
      intellect: 22,
      yellow: 2,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 36,
      critRating: 23,
      id: 30531,
      source: 'Heroic Black Morass',
      phase: 1
    },
    stormreaverShadowKilt: {
      name: 'Stormreaver Shadow-Kilt',
      stamina: 19,
      intellect: 26,
      spirit: 14,
      spellPower: 30,
      critRating: 25,
      id: 27418,
      source: 'Old Hillsbrad Foothills',
      phase: 1
    },
    trousersOfOblivion: {
      name: 'Trousers of Oblivion',
      stamina: 42,
      intellect: 33,
      spellPower: 39,
      hitRating: 12,
      setId: 644,
      id: 27948,
      source: 'Sethekk Halls',
      phase: 1
    },
    pantaloonsOfFlamingWrath: {
      name: 'Pantaloons of Flaming Wrath',
      intellect: 28,
      spellPower: 33,
      critRating: 42,
      id: 30709,
      source: 'The Shattered Halls',
      phase: 1
    },
    khadgarsKiltOfAbjuration: {
      name: "Khadgar's Kilt of Abjuration",
      stamina: 20,
      intellect: 22,
      spirit: 15,
      blue: 2,
      yellow: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 36,
      id: 28185,
      source: 'Black Morass',
      phase: 1
    },
    manaEtchedPantaloons: {
      name: 'Mana-Etched Pantaloons',
      stamina: 34,
      intellect: 32,
      spellPower: 33,
      critRating: 21,
      spellPen: 18,
      setId: 658,
      id: 27907,
      source: 'Heroic Underbog',
      phase: 1
    },
    kirinTorMastersTrousers: {
      name: "Kirin Tor Master's Trousers",
      stamina: 27,
      intellect: 29,
      spirit: 25,
      red: 1,
      blue: 1,
      yellow: 1,
      socketBonus: {
        hitRating: 4
      },
      spellPower: 36,
      id: 30532,
      source: 'Heroic Shadow Labyrinth',
      phase: 1
    },
    incantersTrousers: {
      name: "Incanter's Trousers",
      stamina: 25,
      intellect: 30,
      spirit: 17,
      spellPower: 42,
      critRating: 18,
      id: 27838,
      source: 'Sethekk Halls',
      phase: 1
    },
    deadlyBorerLeggings: {
      name: 'Deadly Borer Leggings',
      stamina: 21,
      intellect: 23,
      spirit: 15,
      spellPower: 27,
      critRating: 22,
      id: 25711,
      source: 'The Blood Furnace Quest',
      phase: 1
    },
    leggingsOfPolarity: {
      name: 'Leggings of Polarity',
      stamina: 20,
      intellect: 14,
      spellPower: 44,
      critRating: 28,
      id: 23070,
      source: 'Naxxramas',
      phase: 0
    },
    plagueheartLeggings: {
      name: 'Plagueheart Leggings',
      stamina: 30,
      intellect: 25,
      spellPower: 37,
      critRating: 14,
      spellPen: 10,
      setId: 529,
      id: 22505,
      source: 'Naxxramas',
      phase: 0
    }
  },
  boots: {
    bootsOfTheMalefic: {
      name: 'Boots of the Malefic',
      stamina: 24,
      intellect: 26,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 50,
      hasteRating: 29,
      critRating: 16,
      hitRating: 28,
      setId: 670,
      id: 34564,
      source: 'Sunwell Plateau',
      phase: 5
    },
    footpadsOfMadness: {
      name: 'Footpads of Madness',
      stamina: 25,
      intellect: 22,
      spellPower: 50,
      hasteRating: 25,
      id: 33357,
      source: "Zul'Aman",
      phase: 4
    },
    blueSuedeShoes: {
      name: 'Blue Suede Shoes',
      stamina: 37,
      intellect: 32,
      spellPower: 56,
      hitRating: 18,
      id: 30894,
      source: 'Hyjal Summit',
      phase: 3
    },
    slippersOfTheSeacaller: {
      name: 'Slippers of the Seacaller',
      stamina: 25,
      intellect: 18,
      spirit: 18,
      yellow: 1,
      blue: 1,
      socketBonus: {
        spellPower: 4
      },
      spellPower: 44,
      critRating: 29,
      id: 32239,
      source: 'Black Temple',
      phase: 3
    },
    veteransSilkFootguards: {
      name: "Veteran's Silk Footguards",
      stamina: 39,
      intellect: 27,
      spellPower: 32,
      resilienceRating: 26,
      critRating: 27,
      id: 32795,
      source: 'PVP',
      phase: 2
    },
    veteransDreadweaveStalkers: {
      name: "Veteran's Dreadweave Stalkers",
      stamina: 45,
      intellect: 30,
      spellPower: 36,
      resilienceRating: 31,
      id: 32787,
      source: 'PVP',
      phase: 2
    },
    bootsOfTheShiftingNightmare: {
      name: 'Boots of the Shifting Nightmare',
      stamina: 41,
      intellect: 22,
      shadowPower: 59,
      hitRating: 18,
      id: 30050,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    velvetBootsOfTheGuardian: {
      name: 'Velvet Boots of the Guardian',
      stamina: 21,
      intellect: 21,
      spirit: 15,
      spellPower: 49,
      critRating: 24,
      id: 30067,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    bootsOfBlasting: {
      name: 'Boots of Blasting',
      stamina: 25,
      intellect: 25,
      spellPower: 39,
      hitRating: 18,
      critRating: 25,
      id: 30037,
      source: 'Tailoring',
      phase: 2
    },
    glidersBootsOfShadowWrath: {
      name: "Glider's Boots of Shadow Wrath",
      shadowPower: 78,
      displayId: 30680,
      id: fakeItemIds.glidersBootsOfShadowWrath,
      source: 'Karazhan',
      phase: 1
    },
    glidersBootsOfFireWrath: {
      name: "Glider's Boots of Fire Wrath",
      firePower: 78,
      displayId: 30680,
      id: fakeItemIds.glidersBootsOfFireWrath,
      source: 'Karazhan',
      phase: 1
    },
    glidersBootsOfTheInvoker: {
      name: "Glider's Boots of the Invoker",
      intellect: 28,
      spellPower: 33,
      critRating: 28,
      displayId: 30680,
      id: fakeItemIds.glidersBootsOfTheInvoker,
      source: 'Karazhan',
      phase: 1
    },
    frozenShadoweaveBoots: {
      name: 'Frozen Shadoweave Boots',
      stamina: 15,
      intellect: 9,
      blue: 1,
      yellow: 1,
      socketBonus: {
        hitRating: 3
      },
      shadowPower: 57,
      setId: 553,
      id: 21870,
      source: 'Tailoring',
      phase: 1
    },
    bootsOfBlasphemy: {
      name: 'Boots of Blaphemy',
      stamina: 36,
      intellect: 29,
      spellPower: 36,
      id: 29242,
      source: 'Heroic Slave Pens',
      phase: 1
    },
    bootsOfForetelling: {
      name: 'Boots of Foretelling',
      stamina: 27,
      intellect: 23,
      red: 1,
      yellow: 1,
      socketBonus: {
        intellect: 3
      },
      spellPower: 26,
      critRating: 19,
      id: 28517,
      source: 'Karazhan',
      phase: 1
    },
    bootsOfTheInfernalCoven: {
      name: 'Boots of the Infernal Coven',
      stamina: 27,
      intellect: 27,
      spirit: 23,
      spellPower: 34,
      id: 28670,
      source: 'Karazhan',
      phase: 1
    },
    rubySlippers: {
      name: 'Ruby Slippers',
      stamina: 33,
      intellect: 29,
      spellPower: 35,
      hitRating: 16,
      id: 28585,
      source: 'Karazhan',
      phase: 1
    },
    marshalsSilkFootguards: {
      name: "Marshal's Silk Footguards",
      stamina: 33,
      intellect: 23,
      spellPower: 28,
      resilienceRating: 24,
      critRating: 24,
      id: 29003,
      source: 'PVP',
      phase: 1
    },
    marshalsDreadweaveStalkers: {
      name: "Marshal's Dreadweave Stalkers",
      stamina: 40,
      intellect: 27,
      spellPower: 32,
      resilienceRating: 27,
      id: 28982,
      source: 'PVP',
      phase: 1
    },
    shattrathJumpers: {
      name: 'Shattrath Jumpers',
      stamina: 25,
      intellect: 17,
      blue: 1,
      yellow: 1,
      socketBonus: {
        intellect: 3
      },
      spellPower: 29,
      id: 28179,
      source: 'Auchindon Quest',
      phase: 1
    },
    bootsOfEtherealManipulation: {
      name: 'Boots of Ethereal Manipulation',
      stamina: 27,
      intellect: 27,
      spirit: 21,
      spellPower: 33,
      id: 29258,
      source: 'Heroic Botanica',
      phase: 1
    },
    sigilLacedBoots: {
      name: 'Sigil-Laced Boots',
      stamina: 24,
      intellect: 18,
      red: 1,
      yellow: 1,
      socketBonus: {
        intellect: 3
      },
      spellPower: 20,
      critRating: 17,
      id: 28406,
      source: 'The Arcatraz',
      phase: 1
    },
    bootsOfTheDarkwalker: {
      name: 'Boots of the Darkwalker',
      stamina: 36,
      intellect: 25,
      shadowPower: 37,
      id: 27451,
      source: 'Heroic Hellfire Ramparts',
      phase: 1
    },
    extravagantBootsOfMalice: {
      name: 'Extravagant Boots of Malice',
      stamina: 27,
      intellect: 24,
      spellPower: 30,
      hitRating: 14,
      id: 27821,
      source: 'Heroic Mana-Tombs',
      phase: 1
    },
    embroideredSpellpyreBoots: {
      name: 'Embroidered Spellpyre Boots',
      stamina: 21,
      intellect: 21,
      spellPower: 41,
      id: 27848,
      source: 'Heroic Blood Furnace',
      phase: 1
    },
    bootsOfTheNexusWarden: {
      name: 'Boots of the Nexus Warden',
      stamina: 27,
      intellect: 17,
      spellPower: 21,
      hitRating: 18,
      id: 30519,
      source: 'Netherstorm Quest',
      phase: 1
    },
    etherealBootsOfTheSkystrider: {
      name: 'Ethereal Boots of the Skyrider',
      stamina: 19,
      intellect: 19,
      spirit: 12,
      spellPower: 26,
      critRating: 17,
      id: 25957,
      source: 'Mana-Tombs',
      phase: 1
    },
    silentSlippersOfMeditation: {
      name: 'Silent Slippers of Meditation',
      stamina: 24,
      intellect: 25,
      spirit: 21,
      spellPower: 26,
      id: 27902,
      source: 'Shadow Labyrinth',
      phase: 1
    },
    slippersOfSerenity: {
      name: 'Slippers of Serenity',
      stamina: 10,
      intellect: 22,
      spirit: 15,
      red: 1,
      blue: 2,
      socketBonus: {
        resilienceRating: 3
      },
      spellPower: 12,
      id: 27411,
      source: 'Auchenai Crypts',
      phase: 1
    },
    plagueheartSandals: {
      name: 'Plagueheart Sandals',
      stamina: 20,
      intellect: 16,
      spellPower: 32,
      critRating: 14,
      setId: 529,
      id: 22508,
      source: 'Naxxramas',
      phase: 0
    }
  },
  ring: {
    loopOfForgedPower: {
      name: 'Loop of Forged Power',
      stamina: 27,
      intellect: 28,
      spellPower: 34,
      hitRating: 19,
      hasteRating: 30,
      unique: true,
      id: 34362,
      source: 'Jewelcrafting BoE',
      phase: 5
    },
    ringOfOmnipotence: {
      name: 'Ring of Omnipotence',
      stamina: 21,
      intellect: 14,
      spellPower: 40,
      critRating: 22,
      hasteRating: 31,
      unique: true,
      id: 34230,
      source: 'Sunwell Plateau',
      phase: 5
    },
    sindoreiBandOfDominance: {
      name: "Sin'dorei Band of Dominance",
      stamina: 22,
      intellect: 15,
      spellPower: 34,
      critRating: 28,
      resilienceRating: 15,
      unique: true,
      id: 35282,
      source: 'Sunwell Plateau',
      phase: 5
    },
    bandOfArcaneAlactrity: {
      name: 'Band of Arcane Alacrity',
      stamina: 18,
      intellect: 12,
      spellPower: 22,
      hasteRating: 18,
      unique: true,
      id: 34704,
      source: "Magisters' Terrace",
      phase: 5
    },
    signetOfAncientMagics: {
      name: 'Signet of Ancient Magics',
      stamina: 12,
      intellect: 17,
      blue: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 29,
      mp5: 5,
      unique: true,
      id: 33293,
      source: "Zul'Aman",
      phase: 4
    },
    manaAttunedBand: {
      name: 'Mana Attuned Band',
      intellect: 19,
      spellPower: 34,
      hitRating: 18,
      hasteRating: 29,
      unique: true,
      id: 33497,
      source: "Zul'Aman",
      phase: 4
    },
    ringOfCapturedStorms: {
      name: 'Ring of Captured Storms',
      spellPower: 42,
      critRating: 29,
      hitRating: 19,
      unique: true,
      id: 32247,
      source: 'Black Temple',
      phase: 3
    },
    ringOfAncientKnowledge: {
      name: 'Ring of Ancient Knowledge',
      stamina: 30,
      intellect: 20,
      spellPower: 39,
      hasteRating: 31,
      id: 32527,
      source: 'Black Temple',
      phase: 3
    },
    blessedBandOfKarabor: {
      name: 'Blessed Band of Karabor',
      stamina: 20,
      intellect: 20,
      spellPower: 25,
      hasteRating: 30,
      mp5: 6,
      unique: true,
      id: 32528,
      source: 'Black Temple',
      phase: 3
    },
    bandOfTheEternalSage: {
      name: 'Band of the Eternal Sage',
      stamina: 28,
      intellect: 25,
      spellPower: 34,
      critRating: 24,
      unique: true,
      id: 29305,
      source: 'The Scale of the Sands - Exalted',
      phase: 3
    },
    bandOfEternityRevered: {
      name: 'Band of Eternity (Revered)',
      stamina: 28,
      intellect: 25,
      spellPower: 34,
      critRating: 24,
      unique: true,
      id: 29304,
      source: 'The Scale of the Sands - Revered',
      phase: 3
    },
    bandOfEternityHonored: {
      name: 'Band of Eternity (Honored)',
      stamina: 25,
      intellect: 23,
      spellPower: 32,
      critRating: 22,
      unique: true,
      id: 29303,
      source: 'The Scale of the Sands - Honored',
      phase: 3
    },
    bandOfEternityFriendly: {
      name: 'Band of Eternity (Friendly)',
      stamina: 24,
      intellect: 22,
      spellPower: 29,
      critRating: 21,
      unique: true,
      id: 29302,
      source: 'The Scale of the Sands - Friendly',
      phase: 3
    },
    veteransBandOfDominance: {
      name: "Veteran's Band of Dominance",
      stamina: 27,
      intellect: 12,
      spellPower: 29,
      resilienceRating: 22,
      spellPen: 10,
      unique: true,
      id: 33056,
      source: 'PVP',
      phase: 2
    },
    theSealOfDanzalar: {
      name: 'The Seal of Danzalar',
      stamina: 33,
      spellPower: 25,
      resilienceRating: 21,
      id: 33054,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    ringOfEndlessCoils: {
      name: 'Ring of Endless Coils',
      stamina: 31,
      spellPower: 37,
      critRating: 22,
      unique: true,
      id: 30109,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    bandOfAlar: {
      name: "Band of Al'ar",
      stamina: 24,
      intellect: 23,
      spellPower: 37,
      unique: true,
      id: 29922,
      source: 'The Eye',
      phase: 2
    },
    bandOfDominion: {
      name: 'Band of Dominion',
      spellPower: 28,
      critRating: 21,
      id: 31290,
      source: 'World Drop',
      phase: 1
    },
    ringOfRecurrence: {
      name: 'Ring of Recurrence',
      stamina: 15,
      intellect: 15,
      spellPower: 32,
      critRating: 19,
      unique: true,
      id: 28753,
      source: 'Karazhan',
      phase: 1
    },
    bandOfCrimsonFury: {
      name: 'Band of Crimson Fury',
      stamina: 22,
      intellect: 22,
      spellPower: 28,
      hitRating: 16,
      unique: true,
      id: 28793,
      source: "Magtheridon's Lair",
      phase: 1
    },
    sealOfTheExorcist: {
      name: 'Seal of the Exorcist',
      stamina: 24,
      spellPower: 28,
      hitRating: 12,
      resilienceRating: 11,
      unique: true,
      id: 28555,
      source: '50 Spirit Stones',
      phase: 1
    },
    seersSignet: {
      name: "Seer's Signet",
      stamina: 24,
      spellPower: 34,
      critRating: 12,
      id: 29126,
      source: 'The Scryers - Exalted',
      phase: 1
    },
    ringOfCrypticDreams: {
      name: 'Ring of Cryptic Dreams',
      stamina: 16,
      intellect: 17,
      spellPower: 23,
      critRating: 20,
      unique: true,
      id: 29367,
      source: "25 Badge of Justice - G'eras",
      phase: 1
    },
    ryngosBandOfIngenuity: {
      name: "Ryngo's Band of Ingenuity",
      stamina: 12,
      intellect: 14,
      spellPower: 25,
      critRating: 14,
      id: 28394,
      source: 'The Arcatraz',
      phase: 1
    },
    arcaneNetherband: {
      name: 'Arcane Netherband',
      stamina: 18,
      intellect: 18,
      spellPower: 21,
      spellPen: 15,
      id: 28327,
      source: 'The Botanica',
      phase: 1
    },
    violetSignetFriendly: {
      name: 'Violet Signet (Friendly)',
      stamina: 18,
      intellect: 18,
      spellPower: 22,
      critRating: 12,
      unique: true,
      id: 29284,
      source: 'The Violet Eye - Friendly',
      phase: 1
    },
    violetSignetHonored: {
      name: 'Violet Signet (Honored)',
      stamina: 19,
      intellect: 21,
      spellPower: 26,
      critRating: 15,
      unique: true,
      id: 29285,
      source: 'The Violet Eye - Honored',
      phase: 1
    },
    violetSignetRevered: {
      name: 'Violet Signet (Revered)',
      stamina: 22,
      intellect: 22,
      spellPower: 28,
      critRating: 17,
      unique: true,
      id: 29286,
      source: 'The Violet Eye - Revered',
      phase: 1
    },
    violetSignetOfTheArchmage: {
      name: 'Violet Signet of the Archmage (Exalted)',
      stamina: 24,
      intellect: 23,
      spellPower: 29,
      critRating: 17,
      unique: true,
      id: 29287,
      source: 'The Violet Eye - Exalted',
      phase: 1
    },
    lolasEve: {
      name: "Lola's Eve",
      stamina: 15,
      intellect: 14,
      spirit: 13,
      spellPower: 29,
      setId: 667,
      id: 31339,
      source: 'World Drop',
      phase: 1
    },
    ashyensGift: {
      name: "Ashyen's Gift",
      stamina: 30,
      spellPower: 23,
      hitRating: 21,
      unique: true,
      id: 29172,
      source: 'Cenarion Expedition - Exalted',
      phase: 1
    },
    sparkingArcaniteRing: {
      name: 'Sparking Arcanite Ring',
      stamina: 13,
      intellect: 14,
      spellPower: 22,
      critRating: 14,
      hitRating: 10,
      unique: true,
      id: 28227,
      source: 'Heroic Old Hillsbrad Foothills',
      phase: 1
    },
    spectralBandOfInnervation: {
      name: 'Spectral Band of Innervation',
      stamina: 22,
      intellect: 24,
      spellPower: 29,
      id: 28510,
      source: 'Karazhan',
      phase: 1
    },
    bandOfTheGuardian: {
      name: 'Band of the Guardian',
      intellect: 11,
      spellPower: 23,
      critRating: 17,
      spellPen: 15,
      unique: true,
      id: 29320,
      source: 'Caverns of Time Quest',
      phase: 1
    },
    sagesBand: {
      name: "Sage's Band",
      intellect: 15,
      spellPower: 18,
      critRating: 14,
      unique: true,
      id: 25826,
      source: 'Honor Hold/Thrallmar - Honored',
      phase: 1
    },
    exarchsDiamondBand: {
      name: "Exarch's Diamond Band",
      stamina: 19,
      intellect: 19,
      spellPower: 25,
      unique: true,
      id: 27523,
      source: 'Heroic Auchenai Crypts',
      phase: 1
    },
    cobaltBandOfTyrigosa: {
      name: 'Cobalt Band of Tyrigosa',
      stamina: 19,
      intellect: 17,
      spellPower: 35,
      unique: true,
      id: 29352,
      source: 'Heroic Mana-Tombs',
      phase: 1
    },
    scintillatingCoralBand: {
      name: 'Scintillating Coral Band',
      stamina: 14,
      intellect: 15,
      spellPower: 21,
      critRating: 17,
      id: 27784,
      source: 'The Steamvaults',
      phase: 1
    },
    ringOfConflictSurvival: {
      name: 'Ring of Conflict Survival',
      stamina: 28,
      spellPower: 23,
      critRating: 20,
      unique: true,
      id: 31922,
      source: 'Heroic Mana-Tombs',
      phase: 1
    },
    yorsCollapsingBand: {
      name: "Yor's Collapsing Band",
      intellect: 20,
      spirit: 19,
      spellPower: 23,
      unique: true,
      id: 31921,
      source: 'Heroic Mana-Tombs',
      phase: 1
    },
    sigilOfShaffar: {
      name: 'Sigil of Shaffar',
      stamina: 18,
      intellect: 16,
      spellPower: 21,
      unique: true,
      id: 25954,
      source: 'Mana-Tombs',
      phase: 1
    },
    witchingBand: {
      name: 'Witching Band',
      stamina: 16,
      intellect: 14,
      spellPower: 21,
      id: 24154,
      source: 'Hellfire Ramparts',
      phase: 1
    },
    ringOfTheFallenGod: {
      name: 'Ring of the Fallen God',
      stamina: 5,
      intellect: 6,
      spellPower: 37,
      hitRating: 8,
      unique: true,
      id: 21709,
      source: 'AQ40',
      phase: 0
    },
    ringOfTheEternalFlame: {
      name: 'Ring of the Eternal Flame',
      intellect: 10,
      firePower: 34,
      critRating: 14,
      id: 23237,
      source: 'Naxxramas',
      phase: 0
    },
    bandOfTheInevitable: {
      name: 'Band of the Inevitable',
      spellPower: 36,
      hitRating: 8,
      unique: true,
      id: 23031,
      source: 'Naxxramas',
      phase: 0
    },
    sealOfTheDamned: {
      name: 'Seal of the Damned',
      stamina: 17,
      spellPower: 21,
      hitRating: 8,
      critRating: 14,
      unique: true,
      id: 23025,
      source: 'Naxxramas',
      phase: 0
    },
    plagueheartRing: {
      name: 'Plagueheart Ring',
      stamina: 24,
      spellPower: 29,
      setId: 529,
      unique: true,
      id: 23063,
      source: 'Naxxramas',
      phase: 0
    },
    wrathOfCenarius: {
      name: 'Wrath of Cenarius',
      unique: true,
      id: 21190,
      source: 'Silithus Quest',
      phase: 0
    }
  },
  trinket: {
    shiftingNaaruSliver: {
      name: 'Shifting Naaru Sliver',
      hasteRating: 54,
      onUseSpellPower: 320,
      duration: 15,
      cooldown: 90,
      unique: true,
      id: 34429,
      source: 'Sunwell Plateau',
      phase: 5 // confirm
    },
    timbalsFocusingCrystal: {
      name: "Timbal's Focusing Crystal",
      spellPower: 44,
      unique: true,
      id: 34470,
      source: "Heroic Magisters' Terrace",
      phase: 5
    },
    hexShrunkenHead: {
      name: 'Hex Shrunken Head',
      spellPower: 53,
      onUseSpellPower: 211,
      duration: 20,
      cooldown: 120,
      unique: true,
      id: 33829,
      source: "Zul'Aman",
      phase: 4 // confirm
    },
    theSkullOfGuldan: {
      name: "The Skull of Gul'dan",
      hitRating: 25,
      spellPower: 55,
      onUseHasteRating: 175,
      duration: 20,
      cooldown: 120,
      unique: true,
      id: 32483,
      source: 'Black Temple',
      phase: 3 // confirm
    },
    ashtongueTalismanOfShadows: {
      name: 'Ashtongue Talisman of Shadows',
      unique: true,
      id: 32493,
      source: 'Ashtongue Deathsworn - Exalted',
      phase: 3
    },
    sextantOfUnstableCurrents: {
      name: 'Sextant of Unstable Currents',
      critRating: 40,
      unique: true,
      id: 30626,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    voidStarTalisman: {
      name: 'Voidstar Talisman',
      spellPower: 48,
      unique: true,
      id: 30449,
      source: 'The Eye',
      phase: 2
    },
    eyeOfMagtheridon: {
      name: 'Eye of Magtheridon',
      spellPower: 54,
      unique: true,
      id: 28789,
      source: "Magtheridon's Lair",
      phase: 1
    },
    darkmoonCardCrusade: {
      name: 'Darkmoon Card: Crusade',
      unique: true,
      id: 31856,
      source: 'Blessings Deck',
      phase: 2 // unsure?
    },
    starkillersBauble: {
      name: "Starkiller's Bauble",
      hitRating: 26,
      unique: true,
      id: 30340,
      source: 'Netherstorm Quest',
      phase: 1
    },
    essenceOfTheMartyr: {
      name: 'Essence of the Martyr',
      spellPower: 28,
      unique: true,
      id: 29376,
      source: "41 Badge of Justice - G'eras",
      phase: 1
    },
    iconOfTheSilverCrescent: {
      name: 'Icon of the Silver Crescent',
      spellPower: 43,
      onUseSpellPower: 155,
      duration: 20,
      cooldown: 120,
      unique: true,
      id: 29370,
      source: "41 Badge of Justice - G'eras",
      phase: 1
    },
    quagmirransEye: {
      name: "Quagmirran's Eye",
      spellPower: 37,
      unique: true,
      id: 27683,
      source: 'Heroic Slave Pens',
      phase: 1
    },
    scryersBloodgem: {
      name: "Scryer's Bloodgem",
      hitRating: 32,
      onUseSpellPower: 151,
      duration: 15,
      cooldown: 90,
      unique: true,
      id: 29132,
      source: 'The Scryers - Revered',
      phase: 1
    },
    theLightningCapacitor: {
      name: 'The Lightning Capacitor',
      unique: true,
      id: 28785,
      source: 'Karazhan',
      phase: 1
    },
    arcanistsStone: {
      name: "Arcanist's Stone",
      hitRating: 25,
      onUseSpellPower: 168,
      duration: 20,
      cooldown: 120,
      unique: true,
      id: 28223,
      source: 'Heroic Old Hillsbrad Foothills',
      phase: 1
    },
    oculusOfTheHiddenEye: {
      name: 'Oculus of the Hidden Eye',
      spellPower: 33,
      unique: true,
      id: 26055,
      source: 'Auchenai Crypts',
      phase: 1
    },
    markOfDefiance: {
      name: 'Mark of Defiance',
      spellPower: 32,
      unique: true,
      id: 27922,
      source: '30 Mark of Honor Hold / Thrallmar',
      phase: 1
    },
    xirisGift: {
      name: "Xi'ri's Gift",
      critRating: 32,
      onUseSpellPower: 151,
      duration: 15,
      cooldown: 90,
      unique: true,
      id: 29179,
      source: "The Sha'tar -  Revered",
      phase: 1
    },
    shiffarsNexusHorn: {
      name: "Shiffar's Nexus-Horn",
      critRating: 30,
      unique: true,
      id: 28418,
      source: 'Arcatraz',
      phase: 1
    },
    figurineLivingRubySerpent: {
      name: 'Figurine - Living Ruby Serpent',
      stamina: 33,
      intellect: 22,
      onUseSpellPower: 151,
      duration: 20,
      cooldown: 300,
      unique: true,
      id: 24126,
      source: 'Jewelcrafting',
      phase: 1
    },
    vengeanceOfTheIllidari: {
      name: 'Vengeance of the Illidari',
      critRating: 26,
      onUseSpellPower: 120,
      duration: 15,
      cooldown: 90,
      unique: true,
      id: 28040,
      source: 'Hellfire Peninsula Quest',
      phase: 1
    },
    ancientCrystalTalisman: {
      name: 'Ancient Crystal Talisman',
      spellPower: 26,
      onUseSpellPower: 105,
      duration: 20,
      cooldown: 120,
      unique: true,
      id: 25620,
      source: 'Zangarmarsh Quest',
      phase: 1
    },
    terokkarTabletOfVim: {
      name: 'Terokkar Tablet of Vim',
      hitRating: 22,
      onUseSpellPower: 84,
      duration: 15,
      cooldown: 90,
      unique: true,
      id: 25936,
      source: 'Terokkar Forest Quest',
      phase: 1
    },
    theRestrainedEssenceOfSapphiron: {
      name: 'The Restrained Essence of Sapphiron',
      spellPower: 40,
      onUseSpellPower: 130,
      duration: 20,
      cooldown: 120,
      unique: true,
      id: 23046,
      source: 'Naxxramas',
      phase: 0
    },
    markOfTheChampion: {
      name: 'Mark of the Champion',
      spellPower: 85,
      unique: true,
      id: 23207,
      source: 'Naxxramas',
      phase: 0
    },
    neltharionsTear: {
      name: "Neltharion's Tear",
      spellPower: 44,
      hitRating: 16,
      unique: true,
      id: 19379,
      source: 'Blackwing Lair',
      phase: 0
    }
  },
  mainhand: {
    sunflare: {
      name: 'Sunflare',
      stamina: 17,
      intellect: 20,
      spellPower: 292,
      critRating: 30,
      hasteRating: 23,
      id: 34336,
      source: 'Sunwell Plateau',
      phase: 5
    },
    jadedCrystalDagger: {
      name: 'Jaded Crystal Dagger',
      stamina: 22,
      intellect: 19,
      spellPower: 185,
      hasteRating: 18,
      id: 34604,
      source: "Heroic Magisters' Terrace",
      phase: 5
    },
    archmagesGuile: {
      name: "Archmage's Guile",
      stamina: 12,
      intellect: 11,
      spellPower: 130,
      critRating: 20,
      id: 34667,
      source: 'Shattered Sun Offensive - Revered',
      phase: 5
    },
    bladeOfTwistedVisions: {
      name: 'Blade of Twisted Visions',
      stamina: 33,
      intellect: 21,
      spellPower: 229,
      hasteRating: 21,
      id: 33467,
      source: "Zul'Aman",
      phase: 4
    },
    wubsCursedHexblade: {
      name: "Wub's Cursed Hexblade",
      intellect: 21,
      spellPower: 217,
      critRating: 20,
      hitRating: 13,
      mp5: 6,
      id: 33354,
      source: "Zul'Aman",
      phase: 4
    },
    vengefulGladiatorsSpellblade: {
      name: "Vengeful Gladiator's Spellblade",
      stamina: 30,
      intellect: 20,
      spellPower: 247,
      hitRating: 17,
      resilienceRating: 17,
      id: 33763,
      source: 'Arena',
      phase: 4 // possibly phase 3
    },
    tempestOfChaos: {
      name: 'Tempest of Chaos',
      stamina: 30,
      intellect: 22,
      spellPower: 259,
      critRating: 24,
      hitRating: 17,
      id: 30910,
      source: 'Hyjal Summit',
      phase: 3
    },
    theMaelstromFury: {
      name: 'The Maelstrom Fury',
      stamina: 33,
      intellect: 21,
      spellPower: 236,
      critRating: 22,
      id: 32237,
      source: 'Black Temple',
      phase: 3
    },
    mercilessGladiatorsSpellblade: {
      name: "Merciless Gladiator's Spellblade",
      stamina: 27,
      intellect: 18,
      spellPower: 225,
      resilienceRating: 18,
      hitRating: 15,
      id: 32053,
      source: 'PVP',
      phase: 2
    },
    fangOfTheLeviathan: {
      name: 'Fang of the Leviathan',
      stamina: 28,
      intellect: 20,
      spellPower: 221,
      critRating: 21,
      id: 30095,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    bloodmawMagusBlade: {
      name: 'Bloodmaw Magus-Blade',
      stamina: 16,
      intellect: 15,
      spellPower: 203,
      critRating: 25,
      id: 28802,
      source: "Gruul's Lair",
      phase: 1
    },
    nathrezimMindblade: {
      name: 'Nathrezim Mindblade',
      stamina: 18,
      intellect: 18,
      spellPower: 203,
      critRating: 23,
      id: 28770,
      source: 'Karazhan',
      phase: 1
    },
    talonOfTheTempest: {
      name: 'Talon of the Tempest',
      intellect: 10,
      yellow: 2,
      socketBonus: {
        intellect: 3
      },
      spellPower: 194,
      critRating: 19,
      hitRating: 9,
      id: 30723,
      source: 'Doomwalker',
      phase: 1
    },
    gladiatorsSpellblade: {
      name: "Gladiator's Spellblade",
      stamina: 28,
      intellect: 18,
      spellPower: 199,
      resilienceRating: 18,
      id: 28297,
      source: 'PVP',
      phase: 1
    },
    stormcallerBladeOfTheArchmage: {
      name: 'Stormcaller/Blade of the Archmage',
      stamina: 13,
      intellect: 11,
      spellPower: 159,
      critRating: 21,
      id: 29153,
      source: 'Honor Hold/Thrallmar - Exalted',
      phase: 1
    },
    bladeOfWizardry: {
      name: 'Blade of Wizardry',
      spellPower: 159,
      id: 31336,
      source: 'World Drop',
      phase: 1
    },
    manaWrath: {
      name: 'Mana Wrath',
      stamina: 24,
      intellect: 18,
      spellPower: 126,
      id: 27899,
      source: 'The Mechanar',
      phase: 1
    },
    greatswordOfHorridDreams: {
      name: 'Greatsword of Horrid Dreams',
      stamina: 15,
      intellect: 14,
      spellPower: 130,
      hitRating: 14,
      id: 27905,
      source: 'Shadow Labyrinth',
      phase: 1
    },
    timeShiftedDagger: {
      name: 'Time-Shifted Dagger',
      stamina: 15,
      intellect: 15,
      spellPower: 85,
      critRating: 13,
      id: 27431,
      source: 'Old Hillsbrad Foothills',
      phase: 1
    },
    starlightDagger: {
      name: 'Starlight Dagger',
      stamina: 15,
      intellect: 15,
      spellPower: 121,
      hitRating: 16,
      id: 27543,
      source: 'Heroic Slave Pens',
      phase: 1
    },
    theWillbreaker: {
      name: 'The Willbreaker',
      stamina: 15,
      intellect: 14,
      spellPower: 121,
      critRating: 17,
      id: 27512,
      source: 'Heroic Blood Furnace',
      phase: 1
    },
    continuumBlade: {
      name: 'Continuum Blade',
      stamina: 30,
      intellect: 11,
      spellPower: 121,
      hitRating: 8,
      id: 29185,
      source: 'Keepers of Time - Revered',
      phase: 1
    },
    runesongDagger: {
      name: 'Runesong Dagger',
      stamina: 12,
      intellect: 11,
      spellPower: 121,
      critRating: 20,
      id: 27868,
      source: 'The Shattered Halls',
      phase: 1
    },
    zangartoothShortblade: {
      name: 'Zangartooth Shortblade',
      stamina: 13,
      intellect: 14,
      spellPower: 61,
      hitRating: 12,
      id: 24453,
      source: 'The Underbog',
      phase: 1
    },
    spellfireLongsword: {
      name: 'Spellfire Longsword',
      stamina: 15,
      intellect: 14,
      spellPower: 56,
      hitRating: 10,
      id: 24361,
      source: 'Slave Pens',
      phase: 1
    },
    wraithBlade: {
      name: 'Wraith Blade',
      stamina: 10,
      intellect: 8,
      spellPower: 95,
      hitRating: 8,
      critRating: 14,
      id: 22807,
      source: 'Naxxramas',
      phase: 0
    }
  },
  offhand: {
    heartOfThePit: {
      name: 'Heart of the Pit',
      stamina: 33,
      intellect: 21,
      spellPower: 39,
      hasteRating: 32,
      id: 34179,
      source: 'Sunwell Plateau',
      phase: 5
    },
    chronicleOfDarkSecrets: {
      name: 'Chronicle of Dark Secrets',
      stamina: 16,
      intellect: 12,
      spellPower: 42,
      hitRating: 17,
      critRating: 23,
      id: 30872,
      source: 'Hyjal Summit',
      phase: 3
    },
    blindSeersIcon: {
      name: 'Blind-Seers Icon',
      stamina: 25,
      intellect: 16,
      spellPower: 42,
      hitRating: 24,
      id: 32361,
      source: 'Black Temple',
      phase: 3
    },
    touchOfInspiration: {
      name: 'Touch of Inspiration',
      stamina: 24,
      intellect: 21,
      spellPower: 22,
      mp5: 12,
      id: 32350,
      source: 'Black Temple',
      phase: 3
    },
    scepterOfPurification: {
      name: 'Scepter of Purification',
      stamina: 24,
      intellect: 17,
      spirit: 25,
      spellPower: 26,
      id: 30911,
      source: 'Hyjal Summit',
      phase: 3
    },
    mercilessGladiatorsEndgame: {
      name: "Merciless Gladiator's Endgame",
      stamina: 27,
      intellect: 19,
      spellPower: 33,
      resilienceRating: 27,
      id: 31978,
      source: 'PVP',
      phase: 2
    },
    fathomstone: {
      name: 'Fathomstone',
      stamina: 16,
      intellect: 12,
      spellPower: 36,
      critRating: 23,
      id: 30049,
      source: 'Serpentshrine Cavern',
      phase: 2
    },
    talismanOfNightbane: {
      name: 'Talisman of Nightbane',
      stamina: 19,
      intellect: 19,
      spellPower: 28,
      critRating: 17,
      id: 28603,
      source: 'Karazhan',
      phase: 1
    },
    orbOfTheSoulEater: {
      name: 'Orb of the Soul-Eater',
      stamina: 18,
      shadowPower: 51,
      id: 29272,
      source: "25 Badge of Justice - G'eras",
      phase: 1
    },
    flametongueSeal: {
      name: 'Flametongue Seal',
      firePower: 49,
      critRating: 17,
      id: 29270,
      source: "25 Badge of Justice - G'eras",
      phase: 1
    },
    jewelOfInfinitePossibilities: {
      name: 'Jewel of Infinite Possibilities',
      stamina: 19,
      intellect: 18,
      spellPower: 23,
      hitRating: 21,
      id: 28734,
      source: 'Karazhan',
      phase: 1
    },
    khadgarsKnapsack: {
      name: "Khadgar's Knapsack",
      spellPower: 49,
      id: 29273,
      source: "25 Badge of Justice - G'eras",
      phase: 1
    },
    karaborianTalisman: {
      name: 'Karaborian Talisman',
      stamina: 23,
      intellect: 23,
      spellPower: 35,
      id: 28781,
      source: "Magtheridon's Lair",
      phase: 1
    },
    manualOfTheNethermancer: {
      name: 'Manual of the Nethermancer',
      stamina: 12,
      intellect: 15,
      spellPower: 21,
      critRating: 19,
      id: 28260,
      source: 'The Mechanar',
      phase: 1
    },
    gladiatorsEndgame: {
      name: "Gladiator's Endgame",
      stamina: 21,
      intellect: 14,
      spellPower: 19,
      resilienceRating: 15,
      id: 28346,
      source: 'PVP',
      phase: 1
    },
    lampOfPeacefulRadiance: {
      name: 'Lamp of Peaceful Radiance',
      stamina: 13,
      intellect: 14,
      spellPower: 21,
      critRating: 13,
      hitRating: 12,
      id: 28412,
      source: 'Arcatraz',
      phase: 1
    },
    starHeartLamp: {
      name: 'Star-Heart Lamp',
      stamina: 17,
      intellect: 18,
      spellPower: 22,
      hitRating: 12,
      id: 28187,
      source: 'Black Morass',
      phase: 1
    },
    hortusSealOfBrilliance: {
      name: "Hortus' Seal of Brilliance",
      stamina: 18,
      intellect: 20,
      spellPower: 23,
      id: 27534,
      source: 'The Shattered Halls',
      phase: 1
    },
    lordaeronMedicalGuide: {
      name: 'Lordaeron Medical Guide',
      stamina: 12,
      intellect: 10,
      spirit: 16,
      spellPower: 16,
      id: 28213,
      source: 'Heroic Old Hillsbrad Foothills',
      phase: 1
    },
    swamplightLantern: {
      name: 'Swamplight Lantern',
      intellect: 22,
      spellPower: 14,
      mp5: 6,
      id: 27714,
      source: 'Heroic Slave Pens',
      phase: 1
    },
    masterSpellstone: {
      name: 'Master Spellstone',
      critRating: 20,
      id: 22646,
      source: 'Warlock Spell',
      phase: 1
    },
    sapphironsLeftEye: {
      name: "Sapphiron's Left Eye",
      stamina: 12,
      intellect: 8,
      spellPower: 26,
      critRating: 14,
      hitRating: 8,
      id: 23049,
      source: 'Naxxramas',
      phase: 0
    }
  },
  twohand: {
    grandMagistersStaffOfTorrents: {
      name: "Grand Magister's Staff of Torrents",
      stamina: 57,
      intellect: 52,
      yellow: 3,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 266,
      hitRating: 50,
      critRating: 49,
      id: 34182,
      source: 'Sunwell Plateau',
      phase: 5
    },
    suninfusedFocusStaff: {
      name: 'Sun-infused Focus Staff',
      stamina: 37,
      intellect: 27,
      red: 1,
      yellow: 2,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 121,
      hitRating: 23,
      id: 34797,
      source: "Magisters' Terrace",
      phase: 5
    },
    amaniDiviningStaff: {
      name: 'Amani Divining Staff',
      stamina: 58,
      intellect: 47,
      red: 1,
      yellow: 1,
      blue: 1,
      socketBonus: {
        spellPower: 5
      },
      spellPower: 217,
      critRating: 31,
      id: 33494,
      source: "Zul'Aman",
      phase: 4
    },
    zhardoomGreatstaffOfTheDevourer: {
      name: "Zhar'doom, Greatstaff of the Devourer",
      stamina: 70,
      intellect: 47,
      spellPower: 259,
      critRating: 36,
      hasteRating: 55,
      id: 32374,
      source: 'Black Temple',
      phase: 3
    },
    staffOfImmaculateRecovery: {
      name: 'Staff of Immaculate Recovery',
      stamina: 73,
      intellect: 51,
      spirit: 35,
      spellPower: 148,
      mp5: 14,
      id: 32344,
      source: 'Black Temple',
      phase: 3
    },
    apostleOfArgus: {
      name: 'Apostle of Argus',
      stamina: 62,
      intellect: 59,
      spellPower: 162,
      mp5: 23,
      id: 30908,
      source: 'Hyjal Summit',
      phase: 3
    },
    mercilessGladiatorsWarStaff: {
      name: "Merciless Gladiator's War Staff",
      stamina: 55,
      intellect: 42,
      spellPower: 225,
      critRating: 42,
      hitRating: 24,
      resilienceRating: 29,
      id: 32055,
      source: 'PVP',
      phase: 2
    },
    theNexusKey: {
      name: 'The Nexus Key',
      stamina: 76,
      intellect: 52,
      spellPower: 236,
      critRating: 51,
      id: 29988,
      source: 'The Eye',
      phase: 2
    },
    gladiatorsWarStaff: {
      name: "Gladiator's War Staff",
      stamina: 48,
      intellect: 35,
      spellPower: 199,
      hitRating: 21,
      critRating: 36,
      resilienceRating: 25,
      id: 24557,
      source: 'PVP',
      phase: 1
    },
    terokksShadowstaff: {
      name: "Terokk's Shadowstaff",
      stamina: 40,
      intellect: 42,
      spellPower: 168,
      critRating: 37,
      id: 29355,
      source: 'Heroic Sethekk Halls',
      phase: 1
    },
    auchenaiStaff: {
      name: 'Auchenai Staff',
      intellect: 46,
      spellPower: 121,
      hitRating: 19,
      critRating: 26,
      id: 29130,
      source: 'The Aldor - Revered',
      phase: 1
    },
    staffOfInfiniteMysteries: {
      name: 'Staff of Infinite Mysteries',
      stamina: 61,
      intellect: 51,
      spellPower: 185,
      hitRating: 23,
      id: 28633,
      source: 'Karazhan',
      phase: 1
    },
    warpstaffOfArcanum: {
      name: 'Warpstaff of Arcanum',
      stamina: 37,
      intellect: 38,
      spellPower: 121,
      critRating: 26,
      hitRating: 16,
      id: 28341,
      source: 'Botanica',
      phase: 1
    },
    grandScepterOfTheNexusKings: {
      name: 'Grand Scepter of the Nexus-Kings',
      stamina: 45,
      intellect: 43,
      spellPower: 121,
      hitRating: 19,
      id: 27842,
      source: 'Heroic Mana-Tombs',
      phase: 1
    },
    bloodfireGreatstaff: {
      name: 'Bloodfire Greatstaff',
      stamina: 42,
      intellect: 42,
      spellPower: 121,
      critRating: 28,
      id: 28188,
      source: 'Black Morass',
      phase: 1
    },
    staffOfPolarities: {
      name: 'Staff of Polarities',
      stamina: 34,
      intellect: 33,
      spellPower: 67,
      hitRating: 28,
      id: 25950,
      source: 'Mana-Tombs',
      phase: 1
    },
    atieshGreatstaffOfTheGuardian: {
      name: 'Atiesh, Greatstaff of the Guardian',
      stamina: 30,
      intellect: 29,
      spellPower: 183,
      critRating: 28,
      id: 22630,
      source: 'Naxxramas',
      phase: 0
    },
    brimstoneStaff: {
      name: 'Brimstone Staff',
      stamina: 31,
      intellect: 30,
      spellPower: 113,
      hitRating: 16,
      critRating: 14,
      id: 22800,
      source: 'Naxxramas',
      phase: 0
    },
    soulseeker: {
      name: 'Soulseeker',
      stamina: 30,
      intellect: 31,
      spellPower: 126,
      critRating: 28,
      spellPen: 25,
      id: 22799,
      source: 'Naxxramas',
      phase: 0
    }
  },
  wand: {
    wandOfTheDemonsoul: {
      name: 'Wand of the Demonsoul',
      stamina: 9,
      intellect: 10,
      yellow: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 22,
      hasteRating: 18,
      id: 34347,
      source: 'Sunwell Plateau',
      phase: 5
    },
    carvedWitchDoctorsStick: {
      name: "Carved Witch Doctor's Stick",
      stamina: 9,
      intellect: 15,
      spellPower: 18,
      blue: 1,
      socketBonus: {
        spellPower: 2
      },
      id: 33192,
      source: "25 Badge of Justice - G'eras",
      phase: 4 // confirm
    },
    wandOfPrismaticFocus: {
      name: 'Wand of Prismatic Focus',
      stamina: 21,
      spellPower: 25,
      hitRating: 13,
      id: 32343,
      source: 'Black Temple',
      phase: 3
    },
    mercilessGladiatorsTouchOfDefeat: {
      name: "Merciless Gladiator's Touch of Defeat",
      stamina: 15,
      intellect: 13,
      spellPower: 16,
      resilienceRating: 14,
      id: 32962,
      source: 'PVP',
      phase: 2,
    },
    wandOfTheForgottenStar: {
      name: 'Wand of the Forgotten Star',
      spellPower: 23,
      critRating: 14,
      hitRating: 11,
      id: 29982,
      source: 'The Eye',
      phase: 2
    },
    eredarWandOfObliteration: {
      name: 'Eredar Wand of Obliteration',
      stamina: 10,
      intellect: 11,
      spellPower: 16,
      critRating: 14,
      id: 28783,
      source: "Magtheridon's Lair",
      phase: 1
    },
    tirisfalWandOfAscendancy: {
      name: 'Tirisfal Wand of Ascendancy',
      stamina: 10,
      intellect: 9,
      spellPower: 15,
      hitRating: 11,
      id: 28673,
      source: 'Karazhan',
      phase: 1
    },
    netherCoresControlRod: {
      name: "Nether Core's Control Rod",
      damageLower: 163,
      damageUpper: 303,
      speed: 1.8,
      stamina: 9,
      intellect: 10,
      spellPower: 13,
      hitRating: 8,
      id: 28386,
      source: 'Arcatraz',
      phase: 1
    },
    gladiatorsTouchOfDefeat: {
      name: "Gladiator's Touch of Defeat",
      stamina: 15,
      intellect: 11,
      spellPower: 14,
      resilienceRating: 12,
      id: 28320,
      source: 'PVP',
      phase: 1
    },
    illidariRodOfDiscipline: {
      name: 'Illidari Rod of Discipline',
      damageLower: 107,
      damageUpper: 199,
      speed: 1.4,
      stamina: 9,
      blue: 1,
      socketBonus: {
        spellPower: 2
      },
      spellPower: 13,
      id: 32872,
      source: 'Shadowmoon Valley Quest',
      phase: 1
    },
    theBlackStalk: {
      name: 'The Black Stalk',
      stamina: 10,
      spellPower: 20,
      critRating: 11,
      id: 29350,
      source: 'Heroic Underbog',
      phase: 1
    },
    voidfireWand: {
      name: 'Voidfire Wand',
      damageLower: 138,
      damageUpper: 257,
      speed: 1.9,
      stamina: 9,
      intellect: 9,
      spellPower: 11,
      hitRating: 7,
      id: 25939,
      source: 'Mana-Tombs',
      phase: 1
    },
    nesingwarySafariStick: {
      name: 'Nesingwary Safari Stick',
      damageLower: 142,
      damageUpper: 265,
      speed: 1.8,
      spellPower: 14,
      critRating: 12,
      id: 25640,
      source: 'Nagrand Quest',
      phase: 1
    },
    flawlessWandOfShadowWrath: {
      name: 'Flawless Wand of Shadow Wrath',
      shadowPower: 25,
      id: fakeItemIds.flawlessWandOfShadowWrath,
      displayId: 25295,
      source: 'World Drop',
      phase: 1
    },
    flawlessWandOfFireWrath: {
      name: 'Flawless Wand of Fire Wrath',
      firePower: 25,
      id: fakeItemIds.flawlessWandOfFireWrath,
      displayId: 25295,
      source: 'World Drop',
      phase: 1
    },
    nexusTorch: {
      name: 'Nexus Torch',
      stamina: 9,
      intellect: 10,
      spellPower: 8,
      critRating: 11,
      id: 27540,
      source: 'The Shattered Halls',
      phase: 1
    },
    wandOfTheNetherwing: {
      name: 'Wand of the Netherwing',
      stamina: 19,
      spellPower: 16,
      id: 27890,
      source: 'Shadow Labyrinth',
      phase: 1
    },
    nethekursesRodOfTorment: {
      name: "Nethekurse's Rod of Torment",
      intellect: 10,
      spellPower: 11,
      critRating: 10,
      id: 25806,
      source: 'Shattered Halls Quest',
      phase: 1
    },
    masterFirestone: {
      name: 'Master Firestone',
      firePower: 30,
      id: 22128,
      source: 'Warlock Spell',
      phase: 1
    },
    wandOfFates: {
      name: 'Wand of Fates',
      stamina: 7,
      intellect: 7,
      spellPower: 12,
      hitRating: 8,
      id: 22820,
      source: 'Naxxramas',
      phase: 0
    },
    doomfinger: {
      name: 'Doomfinger',
      spellPower: 16,
      critRating: 14,
      id: 22821,
      source: 'Naxxramas',
      phase: 0
    }
  }
}
