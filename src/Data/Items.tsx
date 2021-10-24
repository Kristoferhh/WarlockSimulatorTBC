// Made up item IDs that I use for items such as the bracers/boots from the Karazhan basement bosses where all the items use the same item id but they have different stats

import { Item, ItemSlotKey, ItemSource, SocketColor } from "../Types"

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
  archmageSlippersOfShadowWrath: -17,
  archmageSlippersOfFireWrath: -18,
  elementalistBootsOfShadowWrath: -19,
  elementalistBootsOfFireWrath: -20,
  illidariCloakOfShadowWrath: -21,
  illidariCloakOfFireWrath: -22,
  ravagersCuffsOfShadowWrath: -23
}

export const Items: {[key in ItemSlotKey]: Item[]} = {
  head: [
    {
      name: "Dark Conjuror's Collar",
      varName: 'darkConjurorsCollar',
      stamina: 51,
      intellect: 42,
      sockets: [SocketColor.Meta, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 75,
      hasteRating: 30,
      critRating: 38,
      id: 34340,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Helm of Arcane Purity',
      varName: 'helmOfArcanePurity',
      stamina: 51,
      intellect: 42,
      spirit: 38,
      sockets: [SocketColor.Meta, SocketColor.Red],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 75,
      critRating: 30,
      id: 34405,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Annihilator Holo-gogs',
      varName: 'annihilatorHologogs',
      stamina: 41,
      intellect: 37,
      critRating: 42,
      sockets: [SocketColor.Meta, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 81,
      id: 34847,
      source: ItemSource.Engineering,
      phase: 5
    },
    {
      name: 'Hood of Hexing',
      varName: 'hoodOfHexing',
      stamina: 24,
      intellect: 33,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 56,
      hitRating: 31,
      critRating: 24,
      id: 33453,
      source: ItemSource.ZulAman,
      phase: 4
    },
    {
      name: "Vengeful Gladiator's Felweave Cowl",
      varName: 'vengefulGladiatorsFelweaveCowl',
      stamina: 67,
      intellect: 15,
      sockets: [SocketColor.Meta, SocketColor.Red],
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 47,
      critRating: 22,
      resilienceRating: 33,
      id: 33683,
      source: ItemSource.Arena,
      phase: 4 // confirm
    },
    {
      name: "Vengeful Gladiator's Dreadweave Hood",
      varName: 'vengefulGladiatorsDreadweaveHood',
      stamina: 73,
      intellect: 25,
      sockets: [SocketColor.Meta, SocketColor.Red],
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 42,
      resilienceRating: 33,
      id: 33677,
      source: ItemSource.Arena,
      phase: 4 // confirm
    },
    {
      name: 'Cowl of the Illidari Highlord',
      varName: 'cowlOfTheIllidariHighlord',
      stamina: 33,
      intellect: 31,
      sockets: [SocketColor.Meta, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 64,
      hitRating: 21,
      critRating: 47,
      id: 32525,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: "Merciless Gladiator's Felweave Cowl",
      varName: 'mercilessGladiatorsFelweaveCowl',
      stamina: 60,
      intellect: 11,
      sockets: [SocketColor.Meta, SocketColor.Red],
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 42,
      critRating: 18,
      resilienceRating: 33,
      id: 31980,
      source: ItemSource.Arena,
      phase: 3 // confirm
    },
    {
      name: "Merciless Gladiator's Dreadweave Cowl",
      varName: 'mercilessGladiatorsDreadweaveCowl',
      stamina: 66,
      intellect: 20,
      sockets: [SocketColor.Meta, SocketColor.Red],
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 36,
      resilienceRating: 33,
      id: 31974,
      source: ItemSource.Arena,
      phase: 3 // confirm
    },
    {
      name: 'Hood of the Malefic',
      varName: 'hoodOfTheMalefic',
      stamina: 55,
      intellect: 36,
      sockets: [SocketColor.Meta, SocketColor.Yellow],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 63,
      hitRating: 16,
      critRating: 32,
      setId: 670,
      id: 31051,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: 'Mana-Binders Cowl',
      varName: 'manabindersCowl',
      stamina: 38,
      intellect: 29,
      sockets: [SocketColor.Meta, SocketColor.Yellow],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 34,
      critRating: 15,
      id: 32089,
      source: ItemSource.BadgeOfJustice,
      phase: 1
    },
    {
      name: 'Destruction Holo-gogs',
      varName: 'destructionHologogs',
      stamina: 22,
      intellect: 24,
      sockets: [SocketColor.Meta, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 64,
      critRating: 29,
      id: 32494,
      source: ItemSource.Engineering,
      phase: 2
    },
    {
      name: 'Hood of the Corruptor',
      varName: 'hoodOfTheCorruptor',
      stamina: 57,
      intellect: 33,
      sockets: [SocketColor.Meta, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 55,
      critRating: 31,
      setId: 646,
      id: 30212,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Cowl of the Grand Engineer',
      varName: 'cowlOfTheGrandEngineer',
      stamina: 22,
      intellect: 27,
      sockets: [SocketColor.Yellow, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 53,
      hitRating: 16,
      critRating: 35,
      id: 29986,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: "Collar of Cho'gall",
      varName: 'collarOfChogall',
      stamina: 42,
      intellect: 36,
      spellPower: 68,
      id: 28804,
      source: ItemSource.GruulsLair,
      phase: 1
    },
    {
      name: 'Uni-Mind Headdress',
      varName: 'unimindHeaddress',
      stamina: 31,
      intellect: 40,
      spellPower: 46,
      hitRating: 19,
      critRating: 25,
      id: 28744,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Wicked Witch's Hat",
      varName: 'wickedWitchsHat',
      stamina: 38,
      intellect: 37,
      spellPower: 43,
      critRating: 32,
      id: 28586,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Voidheart Crown',
      varName: 'voidheartCrown',
      stamina: 39,
      intellect: 36,
      sockets: [SocketColor.Meta, SocketColor.Red],
      socketBonus: {
        hitRating: 4
      },
      spellPower: 40,
      critRating: 19,
      setId: 645,
      id: 28963,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Spellstrike Hood',
      varName: 'spellstrikeHood',
      stamina: 16,
      intellect: 12,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        stamina: 6
      },
      spellPower: 46,
      critRating: 24,
      hitRating: 16,
      setId: 559,
      id: 24266,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: "Incanter's Cowl",
      varName: 'incantersCowl',
      stamina: 15,
      intellect: 27,
      spirit: 17,
      sockets: [SocketColor.Meta, SocketColor.Yellow],
      socketBonus: {
        spirit: 4
      },
      spellPower: 29,
      critRating: 19,
      id: 28278,
      source: ItemSource.Mechanar,
      phase: 1
    },
    {
      name: "Gladiator's Felweave Cowl",
      varName: 'gladiatorsFelweaveCowl',
      stamina: 54,
      intellect: 15,
      sockets: [SocketColor.Meta, SocketColor.Red],
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 37,
      critRating: 18,
      resilienceRating: 30,
      setId: 615,
      id: 30187,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: "Gladiator's Dreadweave Hood",
      varName: 'gladiatorsDreadweaveHood',
      stamina: 60,
      intellect: 20,
      sockets: [SocketColor.Meta, SocketColor.Red],
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 32,
      resilienceRating: 30,
      setId: 568,
      id: 24553,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Mana-Etched Crown',
      varName: 'manaEtchedCrown',
      stamina: 27,
      intellect: 20,
      sockets: [SocketColor.Meta, SocketColor.Red],
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 34,
      spellPenetration: 15,
      setId: 658,
      id: 28193,
      source: ItemSource.BlackMorass,
      phase: 1
    },
    {
      name: 'Hood of Oblivion',
      varName: 'hoodOfOblivion',
      stamina: 27,
      intellect: 32,
      sockets: [SocketColor.Meta, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 40,
      setId: 644,
      id: 28415,
      source: ItemSource.Arcatraz,
      phase: 1
    },
    {
      name: "Mag'hari Ritualist's Horns (Horde only)",
      varName: 'maghariRitualistsHorns',
      stamina: 18,
      intellect: 16,
      spellPower: 50,
      critRating: 15,
      hitRating: 12,
      id: 28169,
      source: ItemSource.NagrandQuestHordeOnly,
      phase: 1
    },
    {
      name: 'Mage-Collar of the Firestorm',
      varName: 'mageCollarOfTheFirestorm',
      stamina: 32,
      intellect: 33,
      spellPower: 39,
      critRating: 23,
      id: 27488,
      source: ItemSource.HeroicBloodFurnace,
      phase: 1
    },
    {
      name: "Evoker's Helmet of Second Sight",
      varName: 'evokersHelmetOfSecondSight',
      stamina: 12,
      intellect: 15,
      spirit: 8,
      sockets: [SocketColor.Yellow, SocketColor.Blue, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 35,
      critRating: 24,
      id: 31104,
      source: ItemSource.ShadowmoonValleyQuest,
      phase: 1
    },
    {
      name: 'Demonfang Ritual Helm',
      varName: 'demonfangRitualHelm',
      stamina: 39,
      intellect: 30,
      spellPower: 36,
      hitRating: 19,
      id: 27781,
      source: ItemSource.HeroicUnderbog,
      phase: 1
    },
    {
      name: "Hydromancer's Headwrap",
      varName: 'hydromancersHeadwrap',
      stamina: 21,
      intellect: 27,
      sockets: [SocketColor.Meta, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 33,
      id: 28183,
      source: ItemSource.SteamvaultsQuest,
      phase: 1
    },
    {
      name: 'Headdress of Alacrity',
      varName: 'headdressOfAlacrity',
      stamina: 25,
      intellect: 33,
      spirit: 18,
      spellPower: 33,
      critRating: 17,
      id: 27466,
      source: ItemSource.HeroicHellfireRamparts,
      phase: 1
    },
    {
      name: 'Plagueheart Circlet',
      varName: 'plagueheartCirclet',
      stamina: 28,
      intellect: 25,
      spellPower: 33,
      critRating: 28,
      hitRating: 8,
      setId: 529,
      id: 22506,
      source: ItemSource.Naxxramas,
      phase: 0
    }
  ],
  neck: [
    {
      name: 'Pendant of Sunfire',
      varName: 'pendantOfSunfire',
      stamina: 27,
      intellect: 19,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 34,
      critRating: 25,
      hasteRating: 25,
      id: 34359,
      source: ItemSource.Jewelcrafting,
      phase: 5
    },
    {
      name: 'Amulet of Unfettered Magics',
      varName: 'amuletOfUnfetteredMagics',
      stamina: 24,
      intellect: 17,
      spellPower: 39,
      hasteRating: 32,
      hitRating: 15,
      id: 34204,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: "Sin'dorei Pendant of Conquest",
      varName: 'sindoreiPendantOfConquest',
      stamina: 18,
      intellect: 19,
      sockets: [SocketColor.Blue],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 34,
      critRating: 19,
      resilienceRating: 18,
      id: 35290,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Shattered Sun Pendant of Acumen',
      varName: 'shatteredSunPendantOfAcumen',
      stamina: 19,
      intellect: 18,
      spellPower: 37,
      id: 34678,
      source: ItemSource.ShatteredSunOffensiveExalted,
      phase: 5
    },
    {
      name: 'Loop of Cursed Bones',
      varName: 'loopOfCursedBones',
      stamina: 19,
      intellect: 20,
      spellPower: 32,
      hasteRating: 27,
      id: 33466,
      source: ItemSource.ZulAman,
      phase: 4
    },
    {
      name: "Vindicator's Pendant of Subjugation",
      varName: 'vindicatorsPendantOfSubjugation',
      stamina: 31,
      intellect: 15,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        stamina: 3
      },
      hasteRating: 21,
      resilienceRating: 18,
      spellPower: 25,
      id: 35319,
      source: ItemSource.PVP,
      phase: 3
    },
    {
      name: 'Translucent Spellthread Necklace',
      varName: 'translucentSpellthreadNecklace',
      spellPower: 46,
      critRating: 24,
      hitRating: 15,
      id: 32349,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Hellfire-Encased Pendant',
      varName: 'hellfireEncasedPendant',
      stamina: 16,
      intellect: 17,
      spirit: 12,
      firePower: 51,
      critRating: 24,
      id: 32589,
      source: ItemSource.BlackTempleAndMountHyjal,
      phase: 3
    },
    {
      name: "Nadina's Pendant of Purity",
      varName: 'nadinasPendantOfPurity',
      stamina: 16,
      intellect: 14,
      critRating: 19,
      spellPower: 27,
      mp5: 8,
      id: 32370,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: "Veteran's Pendant of Conquest",
      varName: 'veteransPendantOfConquest',
      stamina: 27,
      intellect: 12,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        stamina: 3
      },
      spellPower: 21,
      critRating: 18,
      resilienceRating: 18,
      id: 33067,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: "Veteran's Pendant of Dominance",
      varName: 'veteransPendantOfDominance',
      stamina: 31,
      intellect: 16,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        stamina: 3
      },
      spellPower: 26,
      resilienceRating: 18,
      id: 33065,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: "The Sun King's Talisman",
      varName: 'theSunKingsTalisman',
      stamina: 22,
      intellect: 16,
      spellPower: 41,
      critRating: 24,
      id: 30015,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: 'Pendant of the Lost Ages',
      varName: 'pendantOfTheLostAges',
      stamina: 27,
      intellect: 17,
      spellPower: 36,
      id: 30008,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: "Ritssyn's Lost Pendant",
      varName: 'ritssynsLostPendant',
      stamina: 24,
      shadowPower: 51,
      id: 30666,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Manasurge Pendant',
      varName: 'manasurgePendant',
      stamina: 24,
      intellect: 22,
      spellPower: 28,
      id: 29368,
      source: ItemSource.BadgeOfJustice,
      phase: 1
    },
    {
      name: 'Adornment of Stolen Souls',
      varName: 'adornmentOfStolenSouls',
      stamina: 18,
      intellect: 20,
      spellPower: 28,
      critRating: 23,
      id: 28762,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Warp Engineer's Prismatic Chain",
      varName: 'warpEngineersPrismaticChain',
      stamina: 17,
      intellect: 18,
      spellPower: 19,
      critRating: 16,
      id: 28254,
      source: ItemSource.Mechanar,
      phase: 1
    },
    {
      name: 'Hydra-fang Necklace',
      varName: 'hydraFangNecklace',
      stamina: 17,
      intellect: 16,
      spellPower: 19,
      hitRating: 16,
      id: 27758,
      source: ItemSource.HeroicUnderbog,
      phase: 1
    },
    {
      name: 'Brooch of Heightened Potential',
      varName: 'broochOfHeightenedPotential',
      stamina: 15,
      intellect: 14,
      spellPower: 22,
      critRating: 14,
      hitRating: 9,
      id: 28134,
      source: ItemSource.ShadowLabyrinth,
      phase: 1
    },
    {
      name: 'Brooch of Unquenchable Fury',
      varName: 'broochOfUnquenchableFury',
      stamina: 24,
      intellect: 21,
      spellPower: 26,
      hitRating: 15,
      id: 28530,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Eye of the Night',
      varName: 'eyeOfTheNight',
      critRating: 26,
      hitRating: 16,
      spellPenetration: 15,
      id: 24116,
      source: ItemSource.JewelcraftingBoE,
      phase: 1
    },
    {
      name: 'Chain of the Twilight Owl',
      varName: 'chainOfTheTwilightOwl',
      intellect: 19,
      spellPower: 21,
      id: 24121,
      source: ItemSource.JewelcraftingBoE,
      phase: 1
    },
    {
      name: 'Luminous Pearls of Insight',
      varName: 'luminousPearlsOfInsight',
      intellect: 15,
      spellPower: 25,
      critRating: 11,
      id: 24462,
      source: ItemSource.Underbog,
      phase: 1
    },
    {
      name: "Charlotte's Ivy",
      varName: 'charlottesIvy',
      stamina: 18,
      intellect: 19,
      spirit: 14,
      spellPower: 23,
      setId: 667,
      id: 31338,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: "Natasha's Ember Necklace",
      varName: 'natashasEmberNecklace',
      intellect: 15,
      spellPower: 29,
      critRating: 10,
      id: 31692,
      source: ItemSource.BladesEdgeMountainsQuest,
      phase: 1
    },
    {
      name: 'Torc of the Sethekk Prophet',
      varName: 'torcOfTheSethekkProphet',
      intellect: 18,
      spellPower: 19,
      critRating: 21,
      id: 29333,
      source: ItemSource.SethekkHallsQuest,
      phase: 1
    },
    {
      name: "Omor's Unyielding Will",
      varName: 'omorsUnyieldingWill',
      intellect: 19,
      stamina: 19,
      spellPower: 25,
      id: 27464,
      source: ItemSource.HeroicHellfireRamparts,
      phase: 1
    },
    {
      name: 'Talisman of the Breaker',
      varName: 'talismanOfTheBreaker',
      stamina: 18,
      intellect: 17,
      spellPower: 23,
      id: 29347,
      source: ItemSource.HeroicBloodFurnace,
      phase: 1
    },
    {
      name: "Amulet of Vek'nilash",
      varName: 'amuletOfVeknilash',
      stamina: 9,
      intellect: 5,
      spellPower: 27,
      critRating: 14,
      id: 21608,
      source: ItemSource.AQ40,
      phase: 0
    },
    {
      name: 'Gem of Trapper Innocents',
      varName: 'gemOfTrappedInnocents',
      stamina: 9,
      intellect: 7,
      spellPower: 15,
      critRating: 28,
      id: 23057,
      source: ItemSource.Naxxramas,
      phase: 0
    }
  ],
  shoulders: [
    {
      name: 'Amice of the Convoker',
      varName: 'amiceOfTheConvoker',
      stamina: 36,
      intellect: 28,
      sockets: [SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 53,
      critRating: 22,
      hasteRating: 30,
      id: 34210,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: "Shoulderpads of Knowledge's Pursuit",
      varName: 'shoulderpadsOfKnowledgesPursuit',
      stamina: 33,
      intellect: 33,
      spirit: 22,
      sockets: [SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 53,
      critRating: 26,
      id: 34393,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Fel-Tinged Mantle',
      varName: 'felTingedMantle',
      stamina: 18,
      intellect: 20,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 35,
      critRating: 21,
      id: 34607,
      source: ItemSource.HeroicMagistersTerrace,
      phase: 5
    },
    {
      name: 'Duskhallow Mantle',
      varName: 'duskhallowMantle',
      stamina: 12,
      intellect: 10,
      sockets: [SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 29,
      critRating: 24,
      id: 34788,
      source: ItemSource.MagistersTerrace,
      phase: 5
    },
    {
      name: 'Mantle of Ill Intent',
      varName: 'mantleOfIllIntent',
      stamina: 28,
      intellect: 24,
      spellPower: 40,
      hasteRating: 33,
      id: 33489,
      source: ItemSource.ZulAman,
      phase: 4
    },
    {
      name: 'Mantle of Nimble Thought',
      varName: 'mantleOfNimbleThought',
      stamina: 37,
      intellect: 26,
      spellPower: 44,
      hasteRating: 38,
      id: 32587,
      source: ItemSource.Tailoring,
      phase: 3
    },
    {
      name: 'Hatefury Mantle',
      varName: 'hatefuryMantle',
      stamina: 15,
      intellect: 18,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        critRating: 3
      },
      spellPower: 55,
      spellPenetration: 23,
      critRating: 24,
      id: 30884,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: 'Mantle of the Malefic',
      varName: 'mantleOfTheMalefic',
      stamina: 45,
      intellect: 22,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 46,
      hitRating: 21,
      critRating: 13,
      setId: 670,
      id: 31054,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Blood-cursed Shoulderpads',
      varName: 'bloodcursedShoulderpads',
      stamina: 25,
      intellect: 19,
      spellPower: 55,
      critRating: 25,
      hitRating: 18,
      id: 32338,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: "Merciless Gladiator's Felweave Amice",
      varName: 'mercilessGladiatorsFelweaveAmice',
      stamina: 46,
      intellect: 10,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        resilienceRating: 3
      },
      spellPower: 36,
      critRating: 14,
      resilienceRating: 23,
      source: ItemSource.PVP,
      id: 31979,
      setId: 615,
      phase: 2
    },
    {
      name: "Merciless Gladiator's Dreadweave Mantle",
      varName: 'mercilessGladiatorsDreadweaveMantle',
      stamina: 45,
      intellect: 10,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        resilienceRating: 3
      },
      spellPower: 36,
      resilienceRating: 21,
      hitRating: 13,
      source: ItemSource.PVP,
      id: 31976,
      setId: 568,
      phase: 2
    },
    {
      name: 'Mantle of the Corruptor',
      varName: 'mantleOfTheCorruptor',
      stamina: 37,
      intellect: 24,
      spellPower: 41,
      hitRating: 18,
      sockets: [SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        spellPower: 4
      },
      setId: 646,
      id: 30215,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: 'Mantle of the Elven Kings',
      varName: 'mantleOfTheElvenKings',
      stamina: 27,
      intellect: 18,
      spirit: 17,
      spellPower: 39,
      critRating: 25,
      hitRating: 18,
      id: 30024,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: 'Illidari Shoulderpads',
      varName: 'illidariShoulderpads',
      stamina: 34,
      intellect: 23,
      sockets: [SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 39,
      critRating: 16,
      id: 30079,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Voidheart Mantle',
      varName: 'voidheartMantle',
      stamina: 26,
      intellect: 22,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 37,
      hitRating: 14,
      setId: 645,
      id: 28967,
      source: ItemSource.GruulsLair,
      phase: 1
    },
    {
      name: 'HWL/GM Dreadweave Mantle',
      varName: 'HWLGMDreadweaveMantle',
      stamina: 19,
      intellect: 15,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        resilienceRating: 3
      },
      id: 28820,
      hitRating: 6,
      resilienceRating: 17,
      spellPower: 26,
      setId: 592,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Frozen Shadoweave Shoulders',
      varName: 'frozenShadoweaveShoulders',
      stamina: 21,
      intellect: 15,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        hitRating: 3
      },
      shadowPower: 50,
      setId: 553,
      id: 21869,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: 'Mantle of the Mind Flayer',
      varName: 'mantleOfTheMindFlayer',
      stamina: 33,
      intellect: 29,
      spellPower: 35,
      spellPenetration: 23,
      id: 28726,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Spaulders of the Torn-Heart',
      varName: 'spauldersOfTheTornHeart',
      stamina: 10,
      intellect: 7,
      spirit: 8,
      spellPower: 40,
      critRating: 18,
      id: 30925,
      source: ItemSource.ShadowmoonValleyQuest,
      phase: 1
    },
    {
      name: "Gladiator's Felweave Amice",
      varName: 'gladiatorsFelweaveAmice',
      stamina: 42,
      intellect: 14,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        resilienceRating: 3
      },
      spellPower: 32,
      critRating: 12,
      resilienceRating: 21,
      setId: 615,
      id: 30186,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: "Gladiator's Dreadweave Mantle",
      varName: 'gladiatorsDreadweaveMantle',
      stamina: 42,
      intellect: 14,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        resilienceRating: 3
      },
      spellPower: 32,
      resilienceRating: 21,
      hitRating: 12,
      setId: 568,
      id: 24554,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Mana-Sphere Shoulderguards',
      varName: 'manaSphereShoulderguards',
      stamina: 23,
      intellect: 26,
      spirit: 17,
      spellPower: 29,
      id: 28374,
      source: ItemSource.Arcatraz,
      phase: 1
    },
    {
      name: "Incanter's Pauldrons",
      varName: 'incantersPauldrons',
      stamina: 24,
      intellect: 17,
      spirit: 16,
      sockets: [SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        critRating: 3
      },
      spellPower: 20,
      id: 27738,
      source: ItemSource.Steamvaults,
      phase: 1
    },
    {
      name: 'Mana-Etched Spaulders',
      varName: 'manaEtchedSpaulders',
      stamina: 25,
      intellect: 17,
      sockets: [SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        resilienceRating: 3
      },
      spellPower: 20,
      critRating: 16,
      setId: 658,
      id: 27796,
      source: ItemSource.HeroicSlavePens,
      phase: 1
    },
    {
      name: 'Spaulders of Oblivion',
      varName: 'spauldersOfOblivion',
      stamina: 25,
      intellect: 17,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        hitRating: 3
      },
      spellPower: 29,
      setId: 644,
      id: 27778,
      source: ItemSource.ShadowLabyrinth,
      phase: 1
    },
    {
      name: 'Pauldrons of Arcane Rage',
      varName: 'pauldronsOfArcaneRage',
      stamina: 18,
      intellect: 18,
      spirit: 12,
      spellPower: 27,
      id: 24024,
      source: ItemSource.HellfireRamparts,
      phase: 1
    },
    {
      name: 'Mantle of Three Terrors',
      varName: 'mantleOfThreeTerrors',
      stamina: 29,
      intellect: 25,
      spellPower: 29,
      hitRating: 12,
      id: 27994,
      source: ItemSource.BlackMorass,
      phase: 1
    },
    {
      name: 'Mindrage Pauldrons',
      varName: 'mindragePauldrons',
      stamina: 22,
      intellect: 15,
      spellPower: 34,
      spellPenetration: 10,
      id: 27816,
      source: ItemSource.HeroicManaTombs,
      phase: 1
    },
    {
      name: 'Plagueheart Shoulderpads',
      varName: 'plagueheartShoulderpads',
      stamina: 22,
      intellect: 12,
      spellPower: 36,
      hitRating: 8,
      setId: 529,
      id: 22507,
      source: ItemSource.Naxxramas,
      phase: 0
    }
  ],
  back: [
    {
      name: 'Tattered Cape of Antonidas',
      varName: 'tatteredCapeOfAntonidas',
      stamina: 25,
      intellect: 26,
      sockets: [SocketColor.Red],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 42,
      hasteRating: 32,
      id: 34242,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Cloak of the Betrayed',
      varName: 'cloakOfTheBetrayed',
      stamina: 12,
      intellect: 12,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 23,
      hitRating: 13,
      id: 34792,
      source: ItemSource.MagistersTerrace,
      phase: 5
    },
    {
      name: "Shadow Caster's Drape",
      varName: 'shadowcastersDrape',
      stamina: 22,
      intellect: 20,
      spellPower: 27,
      hasteRating: 25,
      id: 33591,
      source: ItemSource.ZulAman,
      phase: 4
    },
    {
      name: 'Nethervoid Cloak',
      varName: 'nethervoidCloak',
      stamina: 27,
      intellect: 18,
      shadowPower: 53,
      hitRating: 18,
      id: 32590,
      source: ItemSource.BlackTempleAndMountHyjal,
      phase: 3
    },
    {
      name: 'Cloak of the Illidari Council',
      varName: 'cloakOfTheIllidariCouncil',
      stamina: 24,
      intellect: 16,
      spellPower: 42,
      critRating: 25,
      id: 32331,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Shroud of the Highborne',
      varName: 'shroudOfTheHighborne',
      stamina: 24,
      intellect: 23,
      spellPower: 23,
      hasteRating: 32,
      id: 32524,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Royal Cloak of the Sunstriders',
      varName: 'royalCloakOfTheSunstriders',
      stamina: 27,
      intellect: 22,
      spellPower: 44,
      id: 29992,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: 'Illidari Cloak of Shadow Wrath',
      varName: 'illidariCloakOfShadowWrath',
      shadowPower: 47,
      displayId: 31201,
      id: fakeItemIds.illidariCloakOfShadowWrath,
      source: ItemSource.NetherstormRareSpawn,
      phase: 1
    },
    {
      name: 'Illidari Cloak of Fire Wrath',
      varName: 'illidariCloakOfFireWrath',
      firePower: 47,
      displayId: 31201,
      id: fakeItemIds.illidariCloakOfFireWrath,
      source: ItemSource.NetherstormRareSpawn,
      phase: 1
    },
    {
      name: 'Brute Cloak of the Ogre-Magi',
      varName: 'bruteCloakOfTheOgreMagi',
      stamina: 18,
      intellect: 20,
      spellPower: 28,
      critRating: 23,
      id: 28797,
      source: ItemSource.GruulsLair,
      phase: 1
    },
    {
      name: 'Sethekk Oracle Cloak',
      varName: 'sethekkOracleCloak',
      stamina: 18,
      intellect: 18,
      spellPower: 22,
      hitRating: 12,
      id: 27981,
      source: ItemSource.SethekkHalls,
      phase: 1
    },
    {
      name: 'Ruby Drape of the Mysticant',
      varName: 'rubyDrapeOfTheMysticant',
      stamina: 22,
      intellect: 21,
      spellPower: 30,
      hitRating: 18,
      id: 28766,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Cloak of Entropy',
      varName: 'cloakOfEntropy',
      intellect: 11,
      spellPower: 25,
      hitRating: 10,
      id: 31140,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: "Baba's Cloak of Arcanistry",
      varName: 'babasCloakOfArcanistry',
      stamina: 15,
      intellect: 15,
      spellPower: 22,
      critRating: 14,
      id: 28269,
      source: ItemSource.Mechanar,
      phase: 1
    },
    {
      name: 'Cloak of the Black Void',
      varName: 'cloakOfTheBlackVoid',
      intellect: 11,
      spellPower: 35,
      id: 24252,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: 'Ancient Spellcloak of the Highborne',
      varName: 'ancientSpellcloakOfTheHighborne',
      intellect: 15,
      spellPower: 36,
      critRating: 19,
      id: 30735,
      source: ItemSource.DoomLordKazzak,
      phase: 1
    },
    {
      name: 'Cloak of Woven Energy',
      varName: 'cloakOfWovenEnergy',
      stamina: 6,
      intellect: 13,
      spirit: 3,
      spellPower: 29,
      critRating: 6,
      id: 29813,
      source: ItemSource.NetherstormQuest,
      phase: 1
    },
    {
      name: 'Shawl of Shifting Probabilities',
      varName: 'shawlOfShiftingProbabilities',
      stamina: 18,
      intellect: 16,
      spellPower: 21,
      critRating: 22,
      id: 29369,
      source: ItemSource.BadgeOfJustice,
      phase: 1
    },
    {
      name: 'Shadow-Cloak of Dalaran',
      varName: 'shadowCloakOfDalaran',
      stamina: 19,
      intellect: 18,
      spellPower: 36,
      id: 28570,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Spell-slinger's Protector",
      varName: 'spellSlingersProtector',
      stamina: 15,
      intellect: 14,
      spirit: 9,
      spellPower: 16,
      critRating: 13,
      id: 28030,
      source: ItemSource.NagrandQuest,
      phase: 1
    },
    {
      name: 'Embroidered Cape of Mysteries',
      varName: 'embroideredCapeOfMysteries',
      stamina: 18,
      intellect: 20,
      spellPower: 25,
      id: 27485,
      source: ItemSource.HeroicBloodFurnace,
      phase: 1
    },
    {
      name: 'Spore-Soaked Vaneer',
      varName: 'sporeSoakedVaneer',
      stamina: 15,
      intellect: 15,
      spellPower: 19,
      critRating: 11,
      id: 24362,
      source: ItemSource.SlavePens,
      phase: 1
    },
    {
      name: 'Cloak of the Necropolis',
      varName: 'cloakOfTheNecropolis',
      stamina: 12,
      intellect: 11,
      spellPower: 26,
      critRating: 14,
      hitRating: 8,
      id: 23050,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: 'Cloak of the Devoured',
      varName: 'cloakOfTheDevoured',
      stamina: 11,
      intellect: 10,
      spellPower: 30,
      hitRating: 8,
      id: 22731,
      source: ItemSource.AQ40,
      phase: 0
    }
  ],
  chest: [
    {
      name: "Tormented Demonsoul Robes",
      varName: 'tormentedDemonsoulRobes',
      stamina: 39,
      intellect: 38,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      critRating: 50,
      spellPower: 62,
      id: 34936,
      source: ItemSource.BadgeOfJustice,
      phase: 5
    },
    {
      name: "Shroud of the Lore`nial",
      varName: 'shroudOfTheLorenial',
      stamina: 34,
      intellect: 35,
      spirit: 33,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      hitRating: 29,
      spellPower: 61,
      id: 34917,
      source: ItemSource.BadgeOfJustice,
      phase: 5
    },
    {
      name: 'Sunfire Robe',
      varName: 'sunfireRobe',
      stamina: 36,
      intellect: 34,
      sockets: [SocketColor.Red, SocketColor.Red, SocketColor.Red],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 71,
      critRating: 40,
      hasteRating: 40,
      id: 34364,
      source: ItemSource.Tailoring,
      phase: 5
    },
    {
      name: 'Fel Conquerer Remains',
      varName: 'felConquererRemains',
      stamina: 60,
      intellect: 41,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 71,
      critRating: 24,
      hasteRating: 33,
      id: 34232,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Robes of Ghostly Hatred',
      varName: 'robesOfGhostlyHatred',
      stamina: 39,
      intellect: 40,
      spirit: 32,
      sockets: [SocketColor.Red, SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 71,
      critRating: 26,
      hasteRating: 27,
      id: 34399,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: "Scarlet Sin'dorei Robes",
      varName: 'scarletSindoreiRobes',
      stamina: 31,
      intellect: 22,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 51,
      critRating: 36,
      id: 34610,
      source: ItemSource.HeroicMagistersTerrace,
      phase: 5
    },
    {
      name: 'Robe of Departed Spirits',
      varName: 'robeOfDepartedSpirits',
      stamina: 34,
      intellect: 31,
      spirit: 30,
      spellPower: 54,
      hasteRating: 35,
      id: 33317,
      source: ItemSource.ZulAman,
      phase: 4
    },
    {
      name: 'Robes of Rhonin',
      varName: 'robesOfRhonin',
      stamina: 55,
      intellect: 38,
      spellPower: 81,
      hitRating: 27,
      critRating: 24,
      id: 30913,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: 'Robe of the Malefic',
      varName: 'robeOfTheMalefic',
      stamina: 66,
      intellect: 29,
      sockets: [SocketColor.Yellow, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 63,
      hitRating: 28,
      setId: 670,
      id: 31052,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Robe of the Shadow Council',
      varName: 'robeOfTheShadowCouncil',
      stamina: 37,
      intellect: 36,
      spirit: 26,
      spellPower: 73,
      critRating: 28,
      id: 32327,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: "Merciless Gladiator's Felweave Raiment",
      varName: 'mercilessGladiatorsFelweaveRaiment',
      stamina: 57,
      intellect: 12,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        critRating: 4
      },
      spellPower: 35,
      resilienceRating: 26,
      critRating: 30,
      id: 31982,
      source: ItemSource.PVP,
      setId: 615,
      phase: 2
    },
    {
      name: "Merciless Gladiator's Dreadweave Robe",
      varName: 'mercilessGladiatorsDreadweaveRobe',
      stamina: 69,
      intellect: 18,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 44,
      resilienceRating: 24,
      id: 31977,
      setId: 568,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: 'Robe of Hateful Echoes',
      varName: 'robeOfHatefulEchoes',
      stamina: 34,
      intellect: 36,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        stamina: 6
      },
      spellPower: 50,
      critRating: 25,
      id: 30056,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Vestments of the Sea-Witch',
      varName: 'vestmentsOfTheSeaWitch',
      stamina: 28,
      intellect: 28,
      sockets: [SocketColor.Yellow, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 57,
      critRating: 31,
      hitRating: 27,
      id: 30107,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Robe of the Corruptor',
      varName: 'robeOfTheCorruptor',
      stamina: 48,
      intellect: 33,
      sockets: [SocketColor.Yellow, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 55,
      hitRating: 23,
      setId: 646,
      id: 30214,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: 'Drakeweave Raiment of Shadow Wrath',
      varName: 'drakeweaveRaimentOfShadowWrath',
      shadowPower: 85,
      displayId: 31158,
      id: fakeItemIds.drakeweaveRaimentOfShadowWrath,
      source: ItemSource.BladesEdgeMountainsRareSpawn,
      phase: 1
    },
    {
      name: 'Drakeweave Raiment of Fire Wrath',
      varName: 'drakeweaveRaimentOfFireWrath',
      firePower: 85,
      displayId: 31158,
      id: fakeItemIds.drakeweaveRaimentOfFireWrath,
      source: ItemSource.BladesEdgeMountainsRareSpawn,
      phase: 1
    },
    {
      name: 'Voidheart Robe',
      varName: 'voidheartRobe',
      stamina: 42,
      intellect: 33,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        stamina: 6
      },
      spellPower: 42,
      hitRating: 13,
      setId: 645,
      id: 28964,
      source: ItemSource.MagtheridonsLair,
      phase: 1
    },
    {
      name: 'Frozen Shadoweave Robe',
      varName: 'frozenShadoweaveRobe',
      stamina: 30,
      intellect: 20,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        hitRating: 3
      },
      shadowPower: 72,
      setId: 553,
      id: 21871,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: 'Spellfire Robe',
      varName: 'spellfireRobe',
      intellect: 17,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        stamina: 4
      },
      firePower: 72,
      critRating: 28,
      setId: 552,
      id: 21848,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: "Gladiator's Felweave Raiment",
      varName: 'gladiatorsFelweaveRaiment',
      stamina: 51,
      intellect: 16,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        critRating: 4
      },
      spellPower: 32,
      resilienceRating: 24,
      critRating: 27,
      setId: 615,
      id: 30200,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: "Gladiator's Dreadweave Robe",
      varName: 'gladiatorsDreadweaveRobe',
      stamina: 57,
      intellect: 19,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        resilienceRating: 4
      },
      spellPower: 36,
      resilienceRating: 24,
      setId: 568,
      id: 24552,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Will of Edward the Odd',
      varName: 'willOfEdwardTheOdd',
      intellect: 30,
      spellPower: 53,
      critRating: 30,
      id: 31340,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: 'Warp Infused Drape',
      varName: 'warpInfusedDrape',
      stamina: 27,
      intellect: 28,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        critRating: 4
      },
      spellPower: 30,
      hitRating: 12,
      id: 28342,
      source: ItemSource.Botanica,
      phase: 1
    },
    {
      name: 'Bloodfyre Robes of Annihilation',
      varName: 'bloodfyreRobesOfAnnihilation',
      stamina: 27,
      intellect: 27,
      spellPower: 54,
      id: 28252,
      source: ItemSource.Mechanar,
      phase: 1
    },
    {
      name: "Anchorite's Robes",
      varName: 'anchoritesRobes',
      stamina: 16,
      intellect: 38,
      spirit: 18,
      sockets: [SocketColor.Yellow, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        mp5: 2
      },
      spellPower: 29,
      id: 29129,
      source: ItemSource.AldorHonored,
      phase: 1
    },
    {
      name: 'Vermillion Robes of the Dominant',
      varName: 'vermillionRobesOfTheDominant',
      stamina: 37,
      intellect: 33,
      spellPower: 42,
      hitRating: 12,
      id: 27799,
      source: ItemSource.Steamvaults,
      phase: 1
    },
    {
      name: 'Mana-Etched Vestments',
      varName: 'manaEtchedVestments',
      stamina: 25,
      intellect: 25,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 29,
      critRating: 17,
      setId: 658,
      id: 28191,
      source: ItemSource.HeroicOldHillsbradFoothills,
      phase: 1
    },
    {
      name: 'Robe of the Crimson Order',
      varName: 'robeOfTheCrimsonOrder',
      intellect: 23,
      spellPower: 50,
      hitRating: 30,
      id: 31297,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: 'Robe of the Great Dark Beyond',
      varName: 'robeOfTheGreatDarkBeyond',
      stamina: 30,
      intellect: 25,
      spirit: 18,
      spellPower: 39,
      critRating: 23,
      id: 27824,
      source: ItemSource.ManaTombs,
      phase: 1
    },
    {
      name: 'Robe of the Elder Scribes',
      varName: 'robeOfTheElderScribes',
      stamina: 27,
      intellect: 29,
      spirit: 24,
      spellPower: 32,
      critRating: 24,
      id: 28602,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Robes of the Augurer',
      varName: 'robesOfTheAugurer',
      stamina: 18,
      intellect: 18,
      spirit: 11,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        critRating: 4
      },
      spellPower: 28,
      id: 24481,
      source: ItemSource.Underbog,
      phase: 1
    },
    {
      name: "Auchenai Anchorite's Robe",
      varName: 'auchenaiAnchoritesRobe',
      intellect: 24,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        critRating: 4
      },
      spellPower: 28,
      hitRating: 23,
      id: 29341,
      source: ItemSource.AuchenaiCryptsQuest,
      phase: 1
    },
    {
      name: 'Robe of Oblivion',
      varName: 'robeOfOblivion',
      stamina: 30,
      intellect: 20,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        stamina: 6
      },
      spellPower: 40,
      setId: 644,
      id: 28232,
      source: ItemSource.ShadowLabyrinth,
      phase: 1
    },
    {
      name: "Incanter's Robe",
      varName: 'incantersRobe',
      stamina: 24,
      intellect: 22,
      spirit: 22,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        intellect: 4
      },
      spellPower: 29,
      critRating: 8,
      id: 28229,
      source: ItemSource.Botanica,
      phase: 1
    },
    {
      name: "Windchanneler's Tunic of the Invoker",
      varName: 'windchannelersTunicOfTheInvoker',
      intellect: 33,
      spellPower: 38,
      critRating: 33,
      id: 31554,
      source: ItemSource.HeroicManaTombs,
      phase: 1
    },
    {
      name: 'Goldweave Tunic',
      varName: 'goldweaveTunic',
      stamina: 13,
      intellect: 15,
      spirit: 8,
      spellPower: 42,
      critRating: 14,
      id: 28052,
      source: ItemSource.HellfirePeninsulaQuest,
      phase: 1
    },
    {
      name: 'Plagueheart Robe',
      varName: 'plagueheartRobe',
      stamina: 27,
      intellect: 22,
      spellPower: 51,
      hitRating: 8,
      critRating: 14,
      setId: 529,
      id: 22504,
      source: ItemSource.Naxxramas,
      phase: 0
    }
  ],
  bracer: [
    {
      name: 'Bracers of the Malefic',
      varName: 'bracersOfTheMalefic',
      stamina: 18,
      intellect: 22,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 39,
      critRating: 18,
      hasteRating: 21,
      setId: 670,
      id: 34436,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: "Vindicator's Dreadweave Cuffs",
      varName: 'vindicatorsDreadweaveCuffs',
      stamina: 34,
      intellect: 20,
      spellPower: 29,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      id: 33883,
      source: ItemSource.PVP,
      phase: 5
    },
    {
      name: 'Bindings of Raging Fire',
      varName: 'bindingsOfRagingFire',
      stamina: 9,
      intellect: 10,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 22,
      hasteRating: 18,
      id: 34697,
      source: ItemSource.MagistersTerrace,
      phase: 5
    },
    {
      name: 'Fury of the Ursine',
      varName: 'furyOfTheUrsine',
      stamina: 12,
      intellect: 17,
      spirit: 16,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 29,
      critRating: 17,
      id: 33285,
      source: ItemSource.ZulAman,
      phase: 4
    },
    {
      name: 'Bracers of Nimble Thought',
      varName: 'bracersOfNimbleThought',
      stamina: 27,
      intellect: 20,
      spellPower: 34,
      hasteRating: 28,
      id: 32586,
      source: ItemSource.TailoringBoE,
      phase: 3
    },
    {
      name: 'Focused Mana Bindings',
      varName: 'focusedManaBindings',
      stamina: 27,
      intellect: 20,
      spellPower: 42,
      hitRating: 19,
      id: 32270,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Cuffs of Devastation',
      varName: 'cuffsOfDevastation',
      stamina: 22,
      intellect: 20,
      spirit: 19,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        stamina: 3
      },
      spellPower: 34,
      critRating: 14,
      id: 30870,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: 'Bracers of Martyrdom',
      varName: 'bracersOfMartyrdom',
      stamina: 15,
      intellect: 20,
      spirit: 28,
      sockets: [SocketColor.Blue],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 22,
      id: 30871,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: "Crystalweave Bracers",
      varName: 'crystalweaveBracers',
      intellect: 16,
      sockets: [SocketColor.Red],
      socketBonus: {
        spellPower: 2
      },
      critRating: 12,
      spellPower: 23,
      id: 32655,
      source: ItemSource.ApexisShard,
      phase: 2
    },
    {
      name: 'Mindstorm Wristbands',
      varName: 'mindstormWristbands',
      stamina: 13,
      intellect: 13,
      spellPower: 36,
      critRating: 23,
      id: 29918,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: "Veteran's Silk Cuffs",
      varName: 'veteransSilkCuffs',
      stamina: 25,
      intellect: 18,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 22,
      critRating: 14,
      resilienceRating: 13,
      id: 32820,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: "Veteran's Dreadweave Cuffs",
      varName: 'veteransDreadweaveCuffs',
      stamina: 31,
      intellect: 16,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 25,
      resilienceRating: 17,
      id: 32811,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: 'Crimson Bracers of Gloom',
      varName: 'crimsonBracersOfGloom',
      stamina: 18,
      intellect: 18,
      spellPower: 22,
      hitRating: 12,
      id: 27462,
      source: ItemSource.HeroicHellfireRamparts,
      phase: 1
    },
    {
      name: "Ravager's Cuffs of Shadow Wrath",
      varName: 'ravagersCuffsOfShadowWrath',
      shadowPower: 58,
      displayId: 30684,
      id: fakeItemIds.ravagersCuffsOfShadowWrath,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Ravager's Cuffs of Fire Wrath",
      varName: 'ravagersCuffsOfFireWrath',
      firePower: 58,
      displayId: 30684,
      id: fakeItemIds.ravagersCuffsOfFireWrath,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Ravager's Cuffs of the Sorcerer",
      varName: 'ravagersCuffsOfTheSorcerer',
      stamina: 32,
      intellect: 22,
      spellPower: 25,
      displayId: 30684,
      id: fakeItemIds.ravagersCuffsOfTheSorcerer,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Ravager's Cuffs of the Invoker",
      varName: 'ravagersCuffsOfTheInvoker',
      intellect: 22,
      spellPower: 25,
      critRating: 22,
      displayId: 30684,
      id: fakeItemIds.ravagersCuffsOfTheInvoker,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Illidari Bracers of the Invoker',
      varName: 'illidariBracers',
      intellect: 17,
      spellPower: 20,
      critRating: 17,
      id: 31224,
      source: ItemSource.ShadowmoonValleyRareSpawn,
      phase: 1
    },
    {
      name: "Marshal's Silk Cuffs",
      varName: 'marshalsSilkCuffs',
      stamina: 22,
      intellect: 17,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 20,
      resilienceRating: 11,
      critRating: 12,
      id: 29002,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Harbinger Bands',
      varName: 'harbingerBands',
      stamina: 21,
      intellect: 21,
      spirit: 14,
      spellPower: 26,
      id: 28477,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Marshal's Dreadweave Cuffs",
      varName: 'marshalsDreadweaveCuffs',
      stamina: 25,
      intellect: 14,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 22,
      resilienceRating: 14,
      id: 28981,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Bands of Nefarious Deeds',
      varName: 'bandsOfNefariousDeeds',
      stamina: 27,
      intellect: 22,
      spellPower: 32,
      id: 28515,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Arcanium Signet Bands',
      varName: 'arcaniumSignetBands',
      stamina: 14,
      intellect: 15,
      spellPower: 30,
      id: 27746,
      source: ItemSource.HeroicUnderbog,
      phase: 1
    },
    {
      name: 'Bracers of Havok',
      varName: 'bracersOfHavok',
      intellect: 12,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        critRating: 2
      },
      spellPower: 30,
      id: 24250,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: 'Shattrath Wraps',
      varName: 'shattrathWraps',
      stamina: 15,
      intellect: 15,
      sockets: [SocketColor.Red],
      socketBonus: {
        stamina: 3
      },
      spellPower: 21,
      id: 28174,
      source: ItemSource.AuchindonQuest,
      phase: 1
    },
    {
      name: 'Arcing Bracers',
      varName: 'arcingBracers',
      stamina: 15,
      intellect: 15,
      spirit: 10,
      spellPower: 18,
      id: 24392,
      source: ItemSource.BloodFurnace,
      phase: 1
    },
    {
      name: 'Bands of Netherkurse',
      varName: 'bandsOfNetherkurse',
      intellect: 18,
      spirit: 13,
      spellPower: 21,
      spellPenetration: 15,
      id: 27517,
      source: ItemSource.ShatteredHalls,
      phase: 1
    },
    {
      name: 'Bands of Negotion',
      varName: 'bandsOfNegotion',
      stamina: 25,
      intellect: 22,
      spellPower: 29,
      id: 29240,
      source: ItemSource.HeroicManaTombs,
      phase: 1
    },
    {
      name: 'Rockfury Bracers',
      varName: 'rockfuryBracers',
      stamina: 7,
      spellPower: 27,
      hitRating: 8,
      id: 21186,
      source: ItemSource.SilithusQuest,
      phase: 0
    },
    {
      name: 'Plagueheart Bindings',
      varName: 'plagueheartBindings',
      stamina: 23,
      intellect: 14,
      spellPower: 23,
      setId: 529,
      id: 22511,
      source: ItemSource.Naxxramas,
      phase: 0
    }
  ],
  gloves: [
    {
      name: "Enslaved Doomguard Soulgrips",
      varName: 'enslavedDoomguardSoulgrips',
      stamina: 33,
      intellect: 27,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      critRating: 30,
      spellPower: 47,
      id: 34938,
      source: ItemSource.BadgeOfJustice,
      phase: 5
    },
    {
      name: "Barbed Gloves of the Sage",
      varName: 'barbedGlovesOfTheSage',
      stamina: 28,
      intellect: 30,
      spirit: 25,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      hitRating: 15,
      spellPower: 45,
      id: 34904,
      source: ItemSource.BadgeOfJustice,
      phase: 5
    },
    {
      name: 'Sunfire Handwraps',
      varName: 'sunfireHandwraps',
      stamina: 33,
      intellect: 30,
      spellPower: 53,
      critRating: 37,
      id: 34366,
      source: ItemSource.TailoringBoE,
      phase: 5
    },
    {
      name: "Gloves of Tyri's Power",
      varName: 'glovesOfTyrisPower',
      stamina: 33,
      intellect: 32,
      spirit: 27,
      sockets: [SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 47,
      hasteRating: 36,
      id: 34406,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Handguards of Defiled Worlds',
      varName: 'handguardsOfDefiledWorlds',
      stamina: 33,
      intellect: 32,
      sockets: [SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 47,
      hitRating: 27,
      hasteRating: 36,
      id: 34344,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Gloves of Arcane Acuity',
      varName: 'glovesOfArcaneAcuity',
      stamina: 16,
      intellect: 20,
      sockets: [SocketColor.Red, SocketColor.Blue],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 34,
      hitRating: 18,
      id: 34808,
      source: ItemSource.MagistersTerrace,
      phase: 5
    },
    {
      name: 'Gloves of the Malefic',
      varName: 'glovesOfTheMalefic',
      stamina: 57,
      intellect: 27,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        hitRating: 2
      },
      spellPower: 46,
      critRating: 19,
      hitRating: 11,
      setId: 670,
      id: 31050,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: "Merciless Gladiator's Felweave Handguards",
      varName: 'mercilessGladiatorsFelweaveHandguards',
      stamina: 46,
      intellect: 15,
      spellPower: 36,
      critRating: 19,
      resilienceRating: 23,
      setId: 615,
      id: 31981,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: "Merciless Gladiator's Dreadweave Gloves",
      varName: 'mercilessGladiatorsDreadweaveGloves',
      stamina: 45,
      intellect: 25,
      spellPower: 36,
      resilienceRating: 21,
      id: 31973,
      setId: 568,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: 'Gloves of the Corruptor',
      varName: 'glovesOfTheCorruptor',
      stamina: 50,
      intellect: 24,
      spellPower: 42,
      critRating: 25,
      setId: 646,
      id: 30211,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Gauntlets of the Sun King',
      varName: 'gauntletsOfTheSunKing',
      stamina: 28,
      intellect: 29,
      spirit: 20,
      spellPower: 42,
      critRating: 28,
      id: 29987,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: 'Voidheart Gloves',
      varName: 'voidheartGloves',
      stamina: 34,
      intellect: 28,
      spellPower: 35,
      critRating: 22,
      setId: 645,
      id: 28968,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Soul-Eater's Handwraps",
      varName: 'soulEatersHandwraps',
      stamina: 31,
      intellect: 24,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 36,
      critRating: 21,
      id: 28780,
      source: ItemSource.MagtheridonsLair,
      phase: 1
    },
    {
      name: 'Handwraps of Flowing Thought',
      varName: 'handwrapsOfFlowingThought',
      stamina: 24,
      intellect: 22,
      spellPower: 35,
      hitRating: 14,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        hitRating: 3
      },
      id: 28507,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Spellfire Gloves',
      varName: 'spellfireGloves',
      intellect: 10,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        stamina: 4
      },
      firePower: 50,
      critRating: 23,
      setId: 552,
      id: 21847,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: 'Gloves of the Deadwatcher',
      varName: 'glovesOfTheDeadwatcher',
      stamina: 24,
      intellect: 24,
      spellPower: 29,
      hitRating: 18,
      id: 27493,
      source: ItemSource.HeroicAuchenaiCrypts,
      phase: 1
    },
    {
      name: 'Gloves of Pandemonium',
      varName: 'glovesOfPandemonium',
      intellect: 15,
      spellPower: 25,
      critRating: 22,
      hitRating: 10,
      id: 31149,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: "Gladiator's Felweave Handguards",
      varName: 'gladiatorsFelweaveHandguards',
      stamina: 39,
      intellect: 14,
      spellPower: 32,
      critRating: 21,
      resilienceRating: 21,
      setId: 615,
      id: 30188,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Anger-Spark Gloves',
      varName: 'angerSparkGloves',
      sockets: [SocketColor.Red, SocketColor.Red],
      socketBonus: {
        critRating: 3
      },
      spellPower: 30,
      critRating: 25,
      hitRating: 20,
      id: 30725,
      source: ItemSource.Doomwalker,
      phase: 1
    },
    {
      name: "Gladiator's Dreadweave Gloves",
      varName: 'gladiatorsDreadweaveGloves',
      stamina: 39,
      intellect: 25,
      spellPower: 32,
      resilienceRating: 21,
      setId: 568,
      id: 24556,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Manaspark Gloves',
      varName: 'manasparkGloves',
      stamina: 14,
      intellect: 14,
      spirit: 10,
      sockets: [SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        critRating: 3
      },
      spellPower: 16,
      hitRating: 15,
      id: 24450,
      source: ItemSource.Underbog,
      phase: 1
    },
    {
      name: 'Mana-Etched Gloves',
      varName: 'manaEtchedGloves',
      stamina: 25,
      intellect: 17,
      sockets: [SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        resilienceRating: 1
      },
      spellPower: 20,
      critRating: 16,
      setId: 658,
      id: 27465,
      source: ItemSource.HeroicHellfireRamparts,
      phase: 1
    },
    {
      name: 'Gloves of Oblivion',
      varName: 'glovesOfOblivion',
      stamina: 33,
      intellect: 21,
      spellPower: 26,
      hitRating: 20,
      setId: 644,
      id: 27537,
      source: ItemSource.ShatteredHalls,
      phase: 1
    },
    {
      name: "Tempest's Touch",
      varName: 'tempestsTouch',
      stamina: 10,
      intellect: 20,
      spirit: 6,
      sockets: [SocketColor.Blue, SocketColor.Blue],
      socketBonus: {
        critRating: 3
      },
      spellPower: 27,
      spellPenetration: 9,
      id: 29317,
      source: ItemSource.CavernsOfTimeQuest,
      phase: 1
    },
    {
      name: 'Grips of the Void',
      varName: 'gripsOfTheVoid',
      stamina: 18,
      intellect: 11,
      spellPower: 35,
      critRating: 10,
      id: 30930,
      source: ItemSource.ShadowmoonValleyQuest,
      phase: 1
    },
    {
      name: 'Energist Armwraps',
      varName: 'energistArmwraps',
      stamina: 27,
      intellect: 26,
      spellPower: 34,
      id: 28317,
      source: ItemSource.Botanica,
      phase: 1
    },
    {
      name: "Incanter's Gloves",
      varName: 'incantersGloves',
      stamina: 21,
      intellect: 24,
      spirit: 12,
      spellPower: 29,
      critRating: 14,
      id: 27508,
      source: ItemSource.Steamvaults,
      phase: 1
    },
    {
      name: 'Hands of the Sun',
      varName: 'handsOfTheSun',
      stamina: 22,
      intellect: 23,
      firePower: 34,
      critRating: 21,
      id: 27764,
      source: ItemSource.HeroicUnderbog,
      phase: 1
    },
    {
      name: 'Dark Storm Gauntlets',
      varName: 'darkStormGauntlets',
      stamina: 19,
      intellect: 15,
      spellPower: 37,
      hitRating: 8,
      id: 21585,
      source: ItemSource.AQ40,
      phase: 0
    },
    {
      name: 'Plagueheart Gloves',
      varName: 'plagueheartGloves',
      stamina: 25,
      intellect: 17,
      spellPower: 26,
      critRating: 14,
      setId: 529,
      id: 22509,
      source: ItemSource.Naxxramas,
      phase: 0
    }
  ],
  belt: [
    {
      name: "Aftershock Waistguard",
      varName: 'aftershockWaistguard',
      stamina: 27,
      intellect: 27,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        stamina: 3
      },
      hasteRating: 34,
      spellPower: 47,
      id: 34935,
      source: ItemSource.BadgeOfJustice,
      phase: 5
    },
    {
      name: 'Belt of the Malefic',
      varName: 'beltOfTheMalefic',
      stamina: 25,
      intellect: 29,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 50,
      hasteRating: 29,
      critRating: 20,
      hitRating: 20,
      setId: 670,
      id: 34541,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: "Anetheron's Noose",
      varName: 'anetheronsNoose',
      stamina: 22,
      intellect: 23,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 55,
      critRating: 24,
      id: 30888,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: 'Waistwrap of Infinity',
      varName: 'waistwrapOfInfinity',
      stamina: 31,
      intellect: 22,
      spellPower: 56,
      hasteRating: 32,
      id: 32256,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: "Angelista's Sash",
      varName: 'angelistasSash',
      stamina: 29,
      intellect: 30,
      spellPower: 28,
      hasteRating: 37,
      id: 30895,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: "Veteran's Silk Belt",
      varName: 'veteransSilkBelt',
      stamina: 39,
      intellect: 27,
      spellPower: 32,
      resilienceRating: 26,
      critRating: 27,
      id: 32807,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: "Veteran's Dreadweave Belt",
      varName: 'veteransDreadweaveBelt',
      stamina: 45,
      intellect: 30,
      spellPower: 36,
      resilienceRating: 31,
      id: 32799,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: 'Belt of Blasting',
      varName: 'beltOfBlasting',
      spellPower: 50,
      hitRating: 23,
      critRating: 30,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 4
      },
      id: 30038,
      source: ItemSource.Tailoring,
      phase: 2
    },
    {
      name: 'Cord of Screaming Terrors',
      varName: 'cordOfScreamingTerrors',
      stamina: 34,
      intellect: 15,
      sockets: [SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        stamina: 4
      },
      spellPower: 50,
      hitRating: 24,
      id: 30064,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Fire-Cord of the Magus',
      varName: 'fireCordOfTheMagus',
      stamina: 21,
      intellect: 23,
      firePower: 60,
      critRating: 30,
      id: 30020,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: 'Sash of Sealed Fate',
      varName: 'sashOfSealedFate',
      intellect: 15,
      spellPower: 35,
      critRating: 23,
      id: 31283,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: "Lurker's Cord of Shadow Wrath",
      varName: 'lurkersCordOfShadowWrath',
      shadowPower: 78,
      displayId: 30675,
      id: fakeItemIds.lurkersCordOfShadowWrath,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Lurker's Cord of Fire Wrath",
      varName: 'lurkersCordOfFireWrath',
      firePower: 78,
      displayId: 30675,
      id: fakeItemIds.lurkersCordOfFireWrath,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Lurker's Cord of the Sorcerer",
      varName: 'lurkersCordOfTheSorcerer',
      stamina: 43,
      intellect: 29,
      spellPower: 34,
      displayId: 30675,
      id: fakeItemIds.lurkersCordOfTheSorcerer,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Lurker's Cord of the Invoker",
      varName: 'lurkersCordOfTheInvoker',
      intellect: 28,
      spellPower: 33,
      critRating: 28,
      displayId: 30675,
      id: fakeItemIds.lurkersCordOfTheInvoker,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Nethershard Girdle',
      varName: 'nethershardGirdle',
      stamina: 22,
      intellect: 30,
      spirit: 22,
      spellPower: 35,
      id: 28565,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Belt of Divine Inspiration',
      varName: 'beltOfDivineInspiration',
      stamina: 27,
      intellect: 26,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 43,
      id: 28799,
      source: ItemSource.GruulsLair,
      phase: 1
    },
    {
      name: 'Malefic Girdle',
      varName: 'maleficGirdle',
      stamina: 27,
      intellect: 26,
      spellPower: 37,
      critRating: 21,
      id: 28654,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Inferno Waist Cord',
      varName: 'infernoWaistCord',
      intellect: 18,
      firePower: 59,
      critRating: 24,
      id: 30673,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Spellfire Belt',
      varName: 'spellfireBelt',
      intellect: 18,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        stamina: 4
      },
      firePower: 50,
      critRating: 18,
      setId: 552,
      id: 21846,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: 'Girdle of Ruination',
      varName: 'girdleOfRuination',
      stamina: 18,
      intellect: 13,
      sockets: [SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        stamina: 4
      },
      spellPower: 39,
      critRating: 20,
      id: 24256,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: 'Sash of Serpentra',
      varName: 'sashOfSerpentra',
      stamina: 31,
      intellect: 21,
      spellPower: 25,
      hitRating: 17,
      id: 27795,
      source: ItemSource.Steamvaults,
      phase: 1
    },
    {
      name: "Marshal's Silk Belt",
      varName: 'marshalsSilkBelt',
      stamina: 33,
      intellect: 22,
      spellPower: 28,
      critRating: 24,
      resilienceRating: 24,
      id: 29001,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: "Marshal's Dreadweave Belt",
      varName: 'marshalsDreadweaveBelt',
      stamina: 39,
      intellect: 27,
      spellPower: 32,
      resilienceRating: 27,
      id: 28980,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Belt of Depravity',
      varName: 'beltOfDepravity',
      stamina: 31,
      intellect: 27,
      spellPower: 34,
      hitRating: 17,
      id: 29241,
      source: ItemSource.HeroicArcatraz,
      phase: 1
    },
    {
      name: 'Mage-Fury Girdle',
      varName: 'mageFuryGirdle',
      stamina: 22,
      intellect: 23,
      spellPower: 28,
      critRating: 20,
      id: 27742,
      source: ItemSource.HeroicSlavePens,
      phase: 1
    },
    {
      name: 'Glyph-Lined Sash',
      varName: 'glyphLinedSash',
      stamina: 21,
      intellect: 23,
      sockets: [SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 30,
      critRating: 9,
      id: 27843,
      source: ItemSource.HeroicManaTombs,
      phase: 1
    },
    {
      name: "A'dal's Gift",
      varName: 'adalsGift',
      intellect: 25,
      spellPower: 34,
      critRating: 21,
      id: 31461,
      source: ItemSource.TempestKeepQuest,
      phase: 1
    },
    {
      name: 'Orcale Belt of Timeless Mystery',
      varName: 'oracleBeltOfTimelessMystery',
      stamina: 22,
      intellect: 24,
      spirit: 17,
      spellPower: 29,
      id: 27768,
      source: ItemSource.HeroicUnderbog,
      phase: 1
    },
    {
      name: 'Mindfire Waistband',
      varName: 'mindfireWaistband',
      stamina: 10,
      intellect: 14,
      spirit: 8,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        hitRating: 3
      },
      spellPower: 21,
      critRating: 11,
      id: 24395,
      source: ItemSource.BloodFurnace,
      phase: 1
    },
    {
      name: "Consortium Prince's Wrap",
      varName: 'consortiumPrincesWrap',
      spellPower: 30,
      spellPenetration: 20,
      critRating: 22,
      id: 29328,
      source: ItemSource.ManaTombs,
      phase: 1
    },
    {
      name: 'Sash of Arcane Visions',
      varName: 'sashOfArcaneVisions',
      stamina: 18,
      intellect: 23,
      spirit: 19,
      spellPower: 28,
      critRating: 22,
      id: 29257,
      source: ItemSource.HeroicAuchenaiCrypts,
      phase: 1
    },
    {
      name: 'Eyestalk Waist Cord',
      varName: 'eyestalkWaistCord',
      stamina: 10,
      intellect: 9,
      spellPower: 41,
      critRating: 14,
      id: 22730,
      source: ItemSource.AQ40,
      phase: 0
    },
    {
      name: 'Plagueheart Belt',
      varName: 'plagueheartBelt',
      stamina: 23,
      intellect: 12,
      spellPower: 34,
      critRating: 14,
      setId: 529,
      id: 22510,
      source: ItemSource.Naxxramas,
      phase: 0
    }
  ],
  legs: [
    {
      name: "Legwraps of Sweltering Flame",
      varName: 'legwrapsOfSwelteringFlame',
      stamina: 37,
      intellect: 36,
      spirit: 30,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 4
      },
      hitRating: 25,
      spellPower: 63,
      id: 34918,
      source: ItemSource.BadgeOfJustice,
      phase: 5
    },
    {
      name: "Corrupted Soulcloth Pantaloons",
      varName: 'corruptedSoulclothPantaloons',
      stamina: 37,
      intellect: 33,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 4
      },
      critRating: 43,
      spellPower: 62,
      id: 32937,
      source: ItemSource.BadgeOfJustice,
      phase: 5
    },
    {
      name: 'Pantaloons of Growing Strife',
      varName: 'pantaloonsOfGrowingStrife',
      stamina: 29,
      intellect: 36,
      spirit: 25,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 71,
      hasteRating: 42,
      id: 34386,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Leggings of Calamity',
      varName: 'leggingsOfCalamity',
      stamina: 48,
      intellect: 41,
      sockets: [SocketColor.Red, SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 71,
      hasteRating: 32,
      critRating: 33,
      id: 34181,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Legs of Channeled Elements',
      varName: 'leggingsOfChanneledElements',
      stamina: 25,
      intellect: 28,
      spirit: 28,
      sockets: [SocketColor.Yellow, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 59,
      hitRating: 18,
      critRating: 34,
      id: 30916,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: 'Leggings of Devastation',
      varName: 'leggingsOfDevastation',
      stamina: 40,
      intellect: 42,
      sockets: [SocketColor.Yellow, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 3
      },
      spellPower: 60,
      hitRating: 26,
      id: 32367,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Leggings of the Malefic',
      varName: 'leggingsOfTheMalefic',
      stamina: 55,
      intellect: 44,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        hitRating: 2
      },
      spellPower: 62,
      critRating: 37,
      hitRating: 19,
      setId: 670,
      id: 31053,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: "Merciless Gladiator's Felweave Trousers",
      varName: 'mercilessGladiatorsFelweaveTrousers',
      stamina: 60,
      intellect: 20,
      spellPower: 49,
      critRating: 29,
      resilienceRating: 30,
      source: ItemSource.PVP,
      id: 31983,
      setId: 615,
      phase: 2
    },
    {
      name: "Merciless Gladiator's Dreadweave Trousers",
      varName: 'mercilessGladiatorsDreadweaveLeggings',
      stamina: 69,
      intellect: 27,
      spellPower: 53,
      resilienceRating: 33,
      source: ItemSource.PVP,
      id: 31975,
      setId: 568,
      phase: 2
    },
    {
      name: 'Leggings of the Corruptor',
      varName: 'leggingsOfTheCorruptor',
      stamina: 48,
      intellect: 32,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        stamina: 3
      },
      spellPower: 55,
      hitRating: 24,
      critRating: 32,
      setId: 646,
      id: 30213,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Trousers of the Astromancer',
      varName: 'trousersOfTheAstromancer',
      stamina: 33,
      intellect: 36,
      spirit: 22,
      sockets: [SocketColor.Yellow, SocketColor.Blue, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 54,
      id: 29972,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: 'Voidheart Leggings',
      varName: 'voidheartLeggings',
      stamina: 42,
      intellect: 38,
      spellPower: 49,
      critRating: 25,
      hitRating: 17,
      setId: 645,
      id: 28966,
      source: ItemSource.GruulsLair,
      phase: 1
    },
    {
      name: 'Spellstrike Pants',
      varName: 'spellstrikePants',
      stamina: 12,
      intellect: 8,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        stamina: 6
      },
      spellPower: 46,
      critRating: 26,
      hitRating: 22,
      setId: 559,
      id: 24262,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: "Gladiator's Felweave Trousers",
      varName: 'gladiatorsFelweaveTrousers',
      stamina: 54,
      intellect: 25,
      spellPower: 42,
      resilienceRating: 30,
      critRating: 28,
      setId: 615,
      id: 30201,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: "Gladiator's Dreadweave Leggings",
      varName: 'gladiatorsDreadweaveLeggings',
      stamina: 60,
      intellect: 30,
      spellPower: 49,
      resilienceRating: 30,
      id: 24555,
      setId: 568,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Leggings of the Seventh Circle',
      varName: 'leggingsOfTheSeventhCircle',
      intellect: 22,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 50,
      critRating: 25,
      hitRating: 18,
      id: 30734,
      source: ItemSource.DoomLordKazzak,
      phase: 1
    },
    {
      name: 'Devil-Stitched Legs',
      varName: 'devilStitchedLegs',
      stamina: 32,
      intellect: 28,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 29,
      id: 28338,
      source: ItemSource.Botanica,
      phase: 1
    },
    {
      name: 'Princely Reign Leggings',
      varName: 'princelyReignLeggings',
      stamina: 18,
      intellect: 28,
      spirit: 12,
      spellPower: 33,
      hitRating: 18,
      id: 24359,
      source: ItemSource.SlavePens,
      phase: 1
    },
    {
      name: "Aran's Sorcerous Slacks",
      varName: 'aransSorcerousSlacks',
      stamina: 29,
      intellect: 28,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 23,
      critRating: 21,
      id: 28212,
      source: ItemSource.HeroicOldHillsbradFoothills,
      phase: 1
    },
    {
      name: 'Leggings of the Skettis Elite',
      varName: 'leggingsOfTheSkettisElite',
      intellect: 33,
      spirit: 33,
      spellPower: 39,
      id: 30836,
      source: ItemSource.LowerCityRevered,
      phase: 1
    },
    {
      name: 'Trial-Fire Trousers',
      varName: 'trialFireTrousers',
      stamina: 42,
      intellect: 40,
      sockets: [SocketColor.Yellow, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 49,
      id: 28594,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Breeches of the Occulist',
      varName: 'breechesOfTheOccultist',
      stamina: 37,
      intellect: 22,
      sockets: [SocketColor.Yellow, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 36,
      critRating: 23,
      id: 30531,
      source: ItemSource.HeroicBlackMorass,
      phase: 1
    },
    {
      name: 'Stormreaver Shadow-Kilt',
      varName: 'stormreaverShadowKilt',
      stamina: 19,
      intellect: 26,
      spirit: 14,
      spellPower: 30,
      critRating: 25,
      id: 27418,
      source: ItemSource.OldHillsbradFoothills,
      phase: 1
    },
    {
      name: 'Trousers of Oblivion',
      varName: 'trousersOfOblivion',
      stamina: 42,
      intellect: 33,
      spellPower: 39,
      hitRating: 12,
      setId: 644,
      id: 27948,
      source: ItemSource.SethekkHalls,
      phase: 1
    },
    {
      name: 'Pantaloons of Flaming Wrath',
      varName: 'pantaloonsOfFlamingWrath',
      intellect: 28,
      spellPower: 33,
      critRating: 42,
      id: 30709,
      source: ItemSource.ShatteredHalls,
      phase: 1
    },
    {
      name: "Khadgar's Kilt of Abjuration",
      varName: 'khadgarsKiltOfAbjuration',
      stamina: 20,
      intellect: 22,
      spirit: 15,
      sockets: [SocketColor.Yellow, SocketColor.Blue, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 36,
      id: 28185,
      source: ItemSource.BlackMorass,
      phase: 1
    },
    {
      name: 'Mana-Etched Pantaloons',
      varName: 'manaEtchedPantaloons',
      stamina: 34,
      intellect: 32,
      spellPower: 33,
      critRating: 21,
      spellPenetration: 18,
      setId: 658,
      id: 27907,
      source: ItemSource.HeroicUnderbog,
      phase: 1
    },
    {
      name: "Kirin Tor Master's Trousers",
      varName: 'kirinTorMastersTrousers',
      stamina: 27,
      intellect: 29,
      spirit: 25,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        hitRating: 4
      },
      spellPower: 36,
      id: 30532,
      source: ItemSource.HeroicShadowLabyrinth,
      phase: 1
    },
    {
      name: "Incanter's Trousers",
      varName: 'incantersTrousers',
      stamina: 25,
      intellect: 30,
      spirit: 17,
      spellPower: 42,
      critRating: 18,
      id: 27838,
      source: ItemSource.SethekkHalls,
      phase: 1
    },
    {
      name: 'Deadly Borer Leggings',
      varName: 'deadlyBorerLeggings',
      stamina: 21,
      intellect: 23,
      spirit: 15,
      spellPower: 27,
      critRating: 22,
      id: 25711,
      source: ItemSource.BloodFurnaceQuest,
      phase: 1
    },
    {
      name: 'Leggings of Polarity',
      varName: 'leggingsOfPolarity',
      stamina: 20,
      intellect: 14,
      spellPower: 44,
      critRating: 28,
      id: 23070,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: 'Plagueheart Leggings',
      varName: 'plagueheartLeggings',
      stamina: 30,
      intellect: 25,
      spellPower: 37,
      critRating: 14,
      spellPenetration: 10,
      setId: 529,
      id: 22505,
      source: ItemSource.Naxxramas,
      phase: 0
    }
  ],
  boots: [
    {
      name: "Boots of Incantations",
      varName: 'bootsOfIncantations',
      stamina: 37,
      intellect: 26,
      spirit: 23,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      hitRating: 17,
      spellPower: 48,
      id: 34919,
      source: ItemSource.BadgeOfJustice,
      phase: 5
    },
    {
      name: 'Boots of the Malefic',
      varName: 'bootsOfTheMalefic',
      stamina: 24,
      intellect: 26,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 50,
      hasteRating: 29,
      critRating: 16,
      hitRating: 28,
      setId: 670,
      id: 34564,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Footpads of Madness',
      varName: 'footpadsOfMadness',
      stamina: 25,
      intellect: 22,
      spellPower: 50,
      hasteRating: 25,
      id: 33357,
      source: ItemSource.ZulAman,
      phase: 4
    },
    {
      name: 'Blue Suede Shoes',
      varName: 'blueSuedeShoes',
      stamina: 37,
      intellect: 32,
      spellPower: 56,
      hitRating: 18,
      id: 30894,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: 'Slippers of the Seacaller',
      varName: 'slippersOfTheSeacaller',
      stamina: 25,
      intellect: 18,
      spirit: 18,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 4
      },
      spellPower: 44,
      critRating: 29,
      id: 32239,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: "Veteran's Silk Footguards",
      varName: 'veteransSilkFootguards',
      stamina: 39,
      intellect: 27,
      spellPower: 32,
      resilienceRating: 26,
      critRating: 27,
      id: 32795,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: "Veteran's Dreadweave Stalkers",
      varName: 'veteransDreadweaveStalkers',
      stamina: 45,
      intellect: 30,
      spellPower: 36,
      resilienceRating: 31,
      id: 32787,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: 'Boots of the Shifting Nightmare',
      varName: 'bootsOfTheShiftingNightmare',
      stamina: 41,
      intellect: 22,
      shadowPower: 59,
      hitRating: 18,
      id: 30050,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Velvet Boots of the Guardian',
      varName: 'velvetBootsOfTheGuardian',
      stamina: 21,
      intellect: 21,
      spirit: 15,
      spellPower: 49,
      critRating: 24,
      id: 30067,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Boots of Blasting',
      varName: 'bootsOfBlasting',
      stamina: 25,
      intellect: 25,
      spellPower: 39,
      hitRating: 18,
      critRating: 25,
      id: 30037,
      source: ItemSource.Tailoring,
      phase: 2
    },
    {
      name: "Glider's Boots of Shadow Wrath",
      varName: 'glidersBootsOfShadowWrath',
      shadowPower: 78,
      displayId: 30680,
      id: fakeItemIds.glidersBootsOfShadowWrath,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Glider's Boots of Fire Wrath",
      varName: 'glidersBootsOfFireWrath',
      firePower: 78,
      displayId: 30680,
      id: fakeItemIds.glidersBootsOfFireWrath,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Glider's Boots of the Invoker",
      varName: 'glidersBootsOfTheInvoker',
      intellect: 28,
      spellPower: 33,
      critRating: 28,
      displayId: 30680,
      id: fakeItemIds.glidersBootsOfTheInvoker,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Elementalist Boots of Shadow Wrath',
      varName: 'elementalistBootsOfShadowWrath',
      shadowPower: 60,
      id: fakeItemIds.elementalistBootsOfShadowWrath,
      displayId: 24686,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: 'Elementalist Boots of Fire Wrath',
      varName: 'elementalistBootsOfFireWrath',
      firePower: 60,
      id: fakeItemIds.elementalistBootsOfFireWrath,
      displayId: 24686,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: 'Archmage Slippers of Shadow Wrath',
      varName: 'archmageSlippersOfShadowWrath',
      shadowPower: 58,
      id: fakeItemIds.archmageSlippersOfShadowWrath,
      displayId: 24678,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: 'Archmage Slippers of Fire Wrath',
      varName: 'archmageSlippersOfFireWrath',
      firePower: 58,
      id: fakeItemIds.archmageSlippersOfFireWrath,
      displayId: 24678,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: 'Frozen Shadoweave Boots',
      varName: 'frozenShadoweaveBoots',
      stamina: 15,
      intellect: 9,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        hitRating: 3
      },
      shadowPower: 57,
      setId: 553,
      id: 21870,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: 'Boots of Blaphemy',
      varName: 'bootsOfBlasphemy',
      stamina: 36,
      intellect: 29,
      spellPower: 36,
      id: 29242,
      source: ItemSource.HeroicSlavePens,
      phase: 1
    },
    {
      name: 'Boots of Foretelling',
      varName: 'bootsOfForetelling',
      stamina: 27,
      intellect: 23,
      sockets: [SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        intellect: 3
      },
      spellPower: 26,
      critRating: 19,
      id: 28517,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Boots of the Infernal Coven',
      varName: 'bootsOfTheInfernalCoven',
      stamina: 27,
      intellect: 27,
      spirit: 23,
      spellPower: 34,
      id: 28670,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Ruby Slippers',
      varName: 'rubySlippers',
      stamina: 33,
      intellect: 29,
      spellPower: 35,
      hitRating: 16,
      id: 28585,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Marshal's Silk Footguards",
      varName: 'marshalsSilkFootguards',
      stamina: 33,
      intellect: 23,
      spellPower: 28,
      resilienceRating: 24,
      critRating: 24,
      id: 29003,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: "Marshal's Dreadweave Stalkers",
      varName: 'marshalsDreadweaveStalkers',
      stamina: 40,
      intellect: 27,
      spellPower: 32,
      resilienceRating: 27,
      id: 28982,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Shattrath Jumpers',
      varName: 'shattrathJumpers',
      stamina: 25,
      intellect: 17,
      sockets: [SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        intellect: 3
      },
      spellPower: 29,
      id: 28179,
      source: ItemSource.AuchindonQuest,
      phase: 1
    },
    {
      name: 'Boots of Ethereal Manipulation',
      varName: 'bootsOfEtherealManipulation',
      stamina: 27,
      intellect: 27,
      spirit: 21,
      spellPower: 33,
      id: 29258,
      source: ItemSource.HeroicBotanica,
      phase: 1
    },
    {
      name: 'Sigil-Laced Boots',
      varName: 'sigilLacedBoots',
      stamina: 24,
      intellect: 18,
      sockets: [SocketColor.Red, SocketColor.Yellow],
      socketBonus: {
        intellect: 3
      },
      spellPower: 20,
      critRating: 17,
      id: 28406,
      source: ItemSource.Arcatraz,
      phase: 1
    },
    {
      name: 'Boots of the Darkwalker',
      varName: 'bootsOfTheDarkwalker',
      stamina: 36,
      intellect: 25,
      shadowPower: 37,
      id: 27451,
      source: ItemSource.HeroicHellfireRamparts,
      phase: 1
    },
    {
      name: 'Extravagant Boots of Malice',
      varName: 'extravagantBootsOfMalice',
      stamina: 27,
      intellect: 24,
      spellPower: 30,
      hitRating: 14,
      id: 27821,
      source: ItemSource.HeroicManaTombs,
      phase: 1
    },
    {
      name: 'Embroidered Spellpyre Boots',
      varName: 'embroideredSpellpyreBoots',
      stamina: 21,
      intellect: 21,
      spellPower: 41,
      id: 27848,
      source: ItemSource.HeroicBloodFurnace,
      phase: 1
    },
    {
      name: 'Boots of the Nexus Warden',
      varName: 'bootsOfTheNexusWarden',
      stamina: 27,
      intellect: 17,
      spellPower: 21,
      hitRating: 18,
      id: 30519,
      source: ItemSource.NetherstormQuest,
      phase: 1
    },
    {
      name: 'Ethereal Boots of the Skyrider',
      varName: 'etherealBootsOfTheSkystrider',
      stamina: 19,
      intellect: 19,
      spirit: 12,
      spellPower: 26,
      critRating: 17,
      id: 25957,
      source: ItemSource.ManaTombs,
      phase: 1
    },
    {
      name: 'Silent Slippers of Meditation',
      varName: 'silentSlippersOfMeditation',
      stamina: 24,
      intellect: 25,
      spirit: 21,
      spellPower: 26,
      id: 27902,
      source: ItemSource.ShadowLabyrinth,
      phase: 1
    },
    {
      name: 'Slippers of Serenity',
      varName: 'slippersOfSerenity',
      stamina: 10,
      intellect: 22,
      spirit: 15,
      sockets: [SocketColor.Red, SocketColor.Blue, SocketColor.Blue],
      socketBonus: {
        resilienceRating: 3
      },
      spellPower: 12,
      id: 27411,
      source: ItemSource.AuchenaiCrypts,
      phase: 1
    },
    {
      name: 'Plagueheart Sandals',
      varName: 'plagueheartSandals',
      stamina: 20,
      intellect: 16,
      spellPower: 32,
      critRating: 14,
      setId: 529,
      id: 22508,
      source: ItemSource.Naxxramas,
      phase: 0
    }
  ],
  ring: [
    {
      name: "Fused Nethergon Band",
      varName: 'fusedNethergonBand',
      stamina: 27,
      intellect: 19,
      hitRating: 28,
      spellPower: 36,
      id: 34889,
      source: ItemSource.BadgeOfJustice,
      phase: 5
    },
    {
      name: 'Loop of Forged Power',
      varName: 'loopOfForgedPower',
      stamina: 27,
      intellect: 28,
      spellPower: 34,
      hitRating: 19,
      hasteRating: 30,
      unique: true,
      id: 34362,
      source: ItemSource.JewelcraftingBoE,
      phase: 5
    },
    {
      name: 'Ring of Omnipotence',
      varName: 'ringOfOmnipotence',
      stamina: 21,
      intellect: 14,
      spellPower: 40,
      critRating: 22,
      hasteRating: 31,
      unique: true,
      id: 34230,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: "Sin'dorei Band of Dominance",
      varName: 'sindoreiBandOfDominance',
      stamina: 22,
      intellect: 15,
      spellPower: 34,
      critRating: 28,
      resilienceRating: 15,
      unique: true,
      id: 35282,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Band of Arcane Alacrity',
      varName: 'bandOfArcaneAlactrity',
      stamina: 18,
      intellect: 12,
      spellPower: 22,
      hasteRating: 18,
      unique: true,
      id: 34704,
      source: ItemSource.MagistersTerrace,
      phase: 5
    },
    {
      name: 'Signet of Ancient Magics',
      varName: 'signetOfAncientMagics',
      stamina: 12,
      intellect: 17,
      sockets: [SocketColor.Blue],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 29,
      mp5: 5,
      unique: true,
      id: 33293,
      source: ItemSource.ZulAman,
      phase: 4
    },
    {
      name: 'Mana Attuned Band',
      varName: 'manaAttunedBand',
      intellect: 19,
      spellPower: 34,
      hitRating: 18,
      hasteRating: 29,
      unique: true,
      id: 33497,
      source: ItemSource.ZulAman,
      phase: 4
    },
    {
      name: 'Ring of Captured Storms',
      varName: 'ringOfCapturedStorms',
      spellPower: 42,
      critRating: 29,
      hitRating: 19,
      unique: true,
      id: 32247,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Ring of Ancient Knowledge',
      varName: 'ringOfAncientKnowledge',
      stamina: 30,
      intellect: 20,
      spellPower: 39,
      hasteRating: 31,
      id: 32527,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Blessed Band of Karabor',
      varName: 'blessedBandOfKarabor',
      stamina: 20,
      intellect: 20,
      spellPower: 25,
      hasteRating: 30,
      mp5: 6,
      unique: true,
      id: 32528,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Band of the Eternal Sage',
      varName: 'bandOfTheEternalSage',
      stamina: 28,
      intellect: 25,
      spellPower: 34,
      critRating: 24,
      unique: true,
      id: 29305,
      source: ItemSource.ScaleOfTheSandsExalted,
      phase: 3
    },
    {
      name: 'Band of Eternity (Revered)',
      varName: 'bandOfEternityRevered',
      stamina: 28,
      intellect: 25,
      spellPower: 34,
      critRating: 24,
      unique: true,
      id: 29304,
      source: ItemSource.ScaleOfTheSandsRevered,
      phase: 3
    },
    {
      name: 'Band of Eternity (Honored)',
      varName: 'bandOfEternityHonored',
      stamina: 25,
      intellect: 23,
      spellPower: 32,
      critRating: 22,
      unique: true,
      id: 29303,
      source: ItemSource.ScaleOfTheSandsHonored,
      phase: 3
    },
    {
      name: 'Band of Eternity (Friendly)',
      varName: 'bandOfEternityFriendly',
      stamina: 24,
      intellect: 22,
      spellPower: 29,
      critRating: 21,
      unique: true,
      id: 29302,
      source: ItemSource.ScaleOfTheSandsFriendly,
      phase: 2
    },
    {
      name: "Veteran's Band of Dominance",
      varName: 'veteransBandOfDominance',
      stamina: 27,
      intellect: 12,
      spellPower: 29,
      resilienceRating: 22,
      spellPenetration: 10,
      unique: true,
      id: 33056,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: 'The Seal of Danzalar',
      varName: 'theSealOfDanzalar',
      stamina: 33,
      spellPower: 25,
      resilienceRating: 21,
      id: 33054,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Ring of Endless Coils',
      varName: 'ringOfEndlessCoils',
      stamina: 31,
      spellPower: 37,
      critRating: 22,
      unique: true,
      id: 30109,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: "Band of Al'ar",
      varName: 'bandOfAlar',
      stamina: 24,
      intellect: 23,
      spellPower: 37,
      unique: true,
      id: 29922,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: 'Band of Dominion',
      varName: 'bandOfDominion',
      spellPower: 28,
      critRating: 21,
      id: 31290,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: 'Manastorm Band',
      varName: 'manastormBand',
      intellect: 15,
      spellPower: 29,
      critRating: 10,
      unique: true,
      id: 30366,
      source: ItemSource.NetherstormQuest,
      phase: 1
    },
    {
      name: 'Ring of Recurrence',
      varName: 'ringOfRecurrence',
      stamina: 15,
      intellect: 15,
      spellPower: 32,
      critRating: 19,
      unique: true,
      id: 28753,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Band of Crimson Fury',
      varName: 'bandOfCrimsonFury',
      stamina: 22,
      intellect: 22,
      spellPower: 28,
      hitRating: 16,
      unique: true,
      id: 28793,
      source: ItemSource.MagtheridonsLair,
      phase: 1
    },
    {
      name: 'Seal of the Exorcist',
      varName: 'sealOfTheExorcist',
      stamina: 24,
      spellPower: 28,
      hitRating: 12,
      resilienceRating: 11,
      unique: true,
      id: 28555,
      source: ItemSource.SpiritShard,
      phase: 1
    },
    {
      name: "Seer's Signet",
      varName: 'seersSignet',
      stamina: 24,
      spellPower: 34,
      critRating: 12,
      id: 29126,
      source: ItemSource.ScryersExalted,
      phase: 1
    },
    {
      name: 'Ring of Cryptic Dreams',
      varName: 'ringOfCrypticDreams',
      stamina: 16,
      intellect: 17,
      spellPower: 23,
      critRating: 20,
      unique: true,
      id: 29367,
      source: ItemSource.BadgeOfJustice,
      phase: 1
    },
    {
      name: "Ryngo's Band of Ingenuity",
      varName: 'ryngosBandOfIngenuity',
      stamina: 12,
      intellect: 14,
      spellPower: 25,
      critRating: 14,
      id: 28394,
      source: ItemSource.Arcatraz,
      phase: 1
    },
    {
      name: 'Arcane Netherband',
      varName: 'arcaneNetherband',
      stamina: 18,
      intellect: 18,
      spellPower: 21,
      spellPenetration: 15,
      id: 28327,
      source: ItemSource.Botanica,
      phase: 1
    },
    {
      name: 'Violet Signet (Friendly)',
      varName: 'violetSignetFriendly',
      stamina: 18,
      intellect: 18,
      spellPower: 22,
      critRating: 12,
      unique: true,
      id: 29284,
      source: ItemSource.VioletEyeFriendly,
      phase: 1
    },
    {
      name: 'Violet Signet (Honored)',
      varName: 'violetSignetHonored',
      stamina: 19,
      intellect: 21,
      spellPower: 26,
      critRating: 15,
      unique: true,
      id: 29285,
      source: ItemSource.VioletEyeHonored,
      phase: 1
    },
    {
      name: 'Violet Signet (Revered)',
      varName: 'violetSignetRevered',
      stamina: 22,
      intellect: 22,
      spellPower: 28,
      critRating: 17,
      unique: true,
      id: 29286,
      source: ItemSource.VioletEyeRevered,
      phase: 1
    },
    {
      name: 'Violet Signet of the Archmage (Exalted)',
      varName: 'violetSignetOfTheArchmage',
      stamina: 24,
      intellect: 23,
      spellPower: 29,
      critRating: 17,
      unique: true,
      id: 29287,
      source: ItemSource.VioletEyeExalted,
      phase: 1
    },
    {
      name: "Lola's Eve",
      varName: 'lolasEve',
      stamina: 15,
      intellect: 14,
      spirit: 13,
      spellPower: 29,
      setId: 667,
      id: 31339,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: "Ashyen's Gift",
      varName: 'ashyensGift',
      stamina: 30,
      spellPower: 23,
      hitRating: 21,
      unique: true,
      id: 29172,
      source: ItemSource.CenarionExpeditionExalted,
      phase: 1
    },
    {
      name: 'Sparking Arcanite Ring',
      varName: 'sparkingArcaniteRing',
      stamina: 13,
      intellect: 14,
      spellPower: 22,
      critRating: 14,
      hitRating: 10,
      unique: true,
      id: 28227,
      source: ItemSource.HeroicOldHillsbradFoothills,
      phase: 1
    },
    {
      name: 'Spectral Band of Innervation',
      varName: 'spectralBandOfInnervation',
      stamina: 22,
      intellect: 24,
      spellPower: 29,
      id: 28510,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Band of the Guardian',
      varName: 'bandOfTheGuardian',
      intellect: 11,
      spellPower: 23,
      critRating: 17,
      spellPenetration: 15,
      unique: true,
      id: 29320,
      source: ItemSource.CavernsOfTimeQuest,
      phase: 1
    },
    {
      name: "Sage's Band",
      varName: 'sagesBand',
      intellect: 15,
      spellPower: 18,
      critRating: 14,
      unique: true,
      id: 25826,
      source: ItemSource.HonorHoldOrThrallmarHonored,
      phase: 1
    },
    {
      name: "Exarch's Diamond Band",
      varName: 'exarchsDiamondBand',
      stamina: 19,
      intellect: 19,
      spellPower: 25,
      unique: true,
      id: 27523,
      source: ItemSource.HeroicAuchenaiCrypts,
      phase: 1
    },
    {
      name: 'Cobalt Band of Tyrigosa',
      varName: 'cobaltBandOfTyrigosa',
      stamina: 19,
      intellect: 17,
      spellPower: 35,
      unique: true,
      id: 29352,
      source: ItemSource.HeroicManaTombs,
      phase: 1
    },
    {
      name: 'Scintillating Coral Band',
      varName: 'scintillatingCoralBand',
      stamina: 14,
      intellect: 15,
      spellPower: 21,
      critRating: 17,
      id: 27784,
      source: ItemSource.Steamvaults,
      phase: 1
    },
    {
      name: 'Ring of Conflict Survival',
      varName: 'ringOfConflictSurvival',
      stamina: 28,
      spellPower: 23,
      critRating: 20,
      unique: true,
      id: 31922,
      source: ItemSource.HeroicManaTombs,
      phase: 1
    },
    {
      name: "Yor's Collapsing Band",
      varName: 'yorsCollapsingBand',
      intellect: 20,
      spirit: 19,
      spellPower: 23,
      unique: true,
      id: 31921,
      source: ItemSource.HeroicManaTombs,
      phase: 1
    },
    {
      name: 'Sigil of Shaffar',
      varName: 'sigilOfShaffar',
      stamina: 18,
      intellect: 16,
      spellPower: 21,
      unique: true,
      id: 25954,
      source: ItemSource.ManaTombs,
      phase: 1
    },
    {
      name: 'Witching Band',
      varName: 'witchingBand',
      stamina: 16,
      intellect: 14,
      spellPower: 21,
      id: 24154,
      source: ItemSource.HellfireRamparts,
      phase: 1
    },
    {
      name: 'Ring of the Fallen God',
      varName: 'ringOfTheFallenGod',
      stamina: 5,
      intellect: 6,
      spellPower: 37,
      hitRating: 8,
      unique: true,
      id: 21709,
      source: ItemSource.AQ40,
      phase: 0
    },
    {
      name: 'Ring of the Eternal Flame',
      varName: 'ringOfTheEternalFlame',
      intellect: 10,
      firePower: 34,
      critRating: 14,
      id: 23237,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: 'Band of the Inevitable',
      varName: 'bandOfTheInevitable',
      spellPower: 36,
      hitRating: 8,
      unique: true,
      id: 23031,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: 'Seal of the Damned',
      varName: 'sealOfTheDamned',
      stamina: 17,
      spellPower: 21,
      hitRating: 8,
      critRating: 14,
      unique: true,
      id: 23025,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: 'Plagueheart Ring',
      varName: 'plagueheartRing',
      stamina: 24,
      spellPower: 29,
      setId: 529,
      unique: true,
      id: 23063,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: 'Wrath of Cenarius',
      varName: 'wrathOfCenarius',
      unique: true,
      id: 21190,
      source: ItemSource.SilithusQuest,
      phase: 0
    }
  ],
  trinket: [
    {
      name: 'Shifting Naaru Sliver',
      varName: 'shiftingNaaruSliver',
      hasteRating: 54,
      unique: true,
      id: 34429,
      source: ItemSource.Sunwell,
      phase: 5 // confirm
    },
    {
      name: "Timbal's Focusing Crystal",
      varName: 'timbalsFocusingCrystal',
      spellPower: 44,
      unique: true,
      id: 34470,
      source: ItemSource.HeroicMagistersTerrace,
      phase: 5
    },
    {
      name: 'Hex Shrunken Head',
      varName: 'hexShrunkenHead',
      spellPower: 53,
      unique: true,
      id: 33829,
      source: ItemSource.ZulAman,
      phase: 4 // confirm
    },
    {
      name: "The Skull of Gul'dan",
      varName: 'theSkullOfGuldan',
      hitRating: 25,
      spellPower: 55,
      unique: true,
      id: 32483,
      source: ItemSource.BlackTemple,
      phase: 3 // confirm
    },
    {
      name: 'Ashtongue Talisman of Shadows',
      varName: 'ashtongueTalismanOfShadows',
      unique: true,
      id: 32493,
      source: ItemSource.AshtongueDeathswornExalted,
      phase: 3
    },
    {
      name: 'Darkmoon Card: Crusade',
      varName: 'darkmoonCardCrusade',
      unique: true,
      id: 31856,
      source: ItemSource.BlessingsDeck,
      phase: 3
    },
    {
      name: "Dark Iron Smoking Pipe",
      varName: 'darkIronSmokingPipe',
      spellPower: 43,
      id: 38290,
      source: ItemSource.BlackrockDepths,
      phase: 2
    },
    {
      name: 'Sextant of Unstable Currents',
      varName: 'sextantOfUnstableCurrents',
      critRating: 40,
      unique: true,
      id: 30626,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Voidstar Talisman',
      varName: 'voidStarTalisman',
      spellPower: 48,
      unique: true,
      id: 30449,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: 'Eye of Magtheridon',
      varName: 'eyeOfMagtheridon',
      spellPower: 54,
      unique: true,
      id: 28789,
      source: ItemSource.MagtheridonsLair,
      phase: 1
    },
    {
      name: "Starkiller's Bauble",
      varName: 'starkillersBauble',
      hitRating: 26,
      unique: true,
      id: 30340,
      source: ItemSource.NetherstormQuest,
      phase: 1
    },
    {
      name: 'Essence of the Martyr',
      varName: 'essenceOfTheMartyr',
      spellPower: 28,
      unique: true,
      id: 29376,
      source: ItemSource.BadgeOfJustice,
      phase: 1
    },
    {
      name: 'Icon of the Silver Crescent',
      varName: 'iconOfTheSilverCrescent',
      spellPower: 43,
      unique: true,
      id: 29370,
      source: ItemSource.BadgeOfJustice,
      phase: 1
    },
    {
      name: "Quagmirran's Eye",
      varName: 'quagmirransEye',
      spellPower: 37,
      unique: true,
      id: 27683,
      source: ItemSource.HeroicSlavePens,
      phase: 1
    },
    {
      name: "Scryer's Bloodgem",
      varName: 'scryersBloodgem',
      hitRating: 32,
      unique: true,
      id: 29132,
      source: ItemSource.ScryersRevered,
      phase: 1
    },
    {
      name: 'The Lightning Capacitor',
      varName: 'theLightningCapacitor',
      unique: true,
      id: 28785,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Arcanist's Stone",
      varName: 'arcanistsStone',
      hitRating: 25,
      unique: true,
      id: 28223,
      source: ItemSource.HeroicOldHillsbradFoothills,
      phase: 1
    },
    {
      name: 'Oculus of the Hidden Eye',
      varName: 'oculusOfTheHiddenEye',
      spellPower: 33,
      unique: true,
      id: 26055,
      source: ItemSource.AuchenaiCrypts,
      phase: 1
    },
    {
      name: 'Mark of Defiance',
      varName: 'markOfDefiance',
      spellPower: 32,
      unique: true,
      id: 27922,
      source: ItemSource.MarkOfHonorHoldOrThrallmar,
      phase: 1
    },
    {
      name: "Xi'ri's Gift",
      varName: 'xirisGift',
      critRating: 32,
      unique: true,
      id: 29179,
      source: ItemSource.ShatarRevered,
      phase: 1
    },
    {
      name: "Shiffar's Nexus-Horn",
      varName: 'shiffarsNexusHorn',
      critRating: 30,
      unique: true,
      id: 28418,
      source: ItemSource.Arcatraz,
      phase: 1
    },
    {
      name: 'Figurine - Living Ruby Serpent',
      varName: 'figurineLivingRubySerpent',
      stamina: 33,
      intellect: 22,
      unique: true,
      id: 24126,
      source: ItemSource.Jewelcrafting,
      phase: 1
    },
    {
      name: 'Vengeance of the Illidari',
      varName: 'vengeanceOfTheIllidari',
      critRating: 26,
      unique: true,
      id: 28040,
      source: ItemSource.HellfirePeninsulaQuest,
      phase: 1
    },
    {
      name: 'Ancient Crystal Talisman',
      varName: 'ancientCrystalTalisman',
      spellPower: 26,
      unique: true,
      id: 25620,
      source: ItemSource.ZangarmarshQuest,
      phase: 1
    },
    {
      name: 'Terokkar Tablet of Vim',
      varName: 'terokkarTabletOfVim',
      hitRating: 22,
      unique: true,
      id: 25936,
      source: ItemSource.TerokkarForestQuest,
      phase: 1
    },
    {
      name: 'The Restrained Essence of Sapphiron',
      varName: 'theRestrainedEssenceOfSapphiron',
      spellPower: 40,
      unique: true,
      id: 23046,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: 'Mark of the Champion',
      varName: 'markOfTheChampion',
      spellPower: 85,
      unique: true,
      id: 23207,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: "Neltharion's Tear",
      varName: 'neltharionsTear',
      spellPower: 44,
      hitRating: 16,
      unique: true,
      id: 19379,
      source: ItemSource.BlackwingLair,
      phase: 0
    }
  ],
  mainhand: [
    {
      name: 'Sunflare',
      varName: 'sunflare',
      stamina: 17,
      intellect: 20,
      spellPower: 292,
      critRating: 30,
      hasteRating: 23,
      id: 34336,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Jaded Crystal Dagger',
      varName: 'jadedCrystalDagger',
      stamina: 22,
      intellect: 19,
      spellPower: 185,
      hasteRating: 18,
      id: 34604,
      source: ItemSource.HeroicMagistersTerrace,
      phase: 5
    },
    {
      name: "Archmage's Guile",
      varName: 'archmagesGuile',
      stamina: 12,
      intellect: 11,
      spellPower: 130,
      critRating: 20,
      id: 34667,
      source: ItemSource.ShatteredSunOffensiveRevered,
      phase: 5
    },
    {
      name: 'Blade of Twisted Visions',
      varName: 'bladeOfTwistedVisions',
      stamina: 33,
      intellect: 21,
      spellPower: 229,
      hasteRating: 21,
      id: 33467,
      source: ItemSource.ZulAman,
      phase: 4
    },
    {
      name: "Wub's Cursed Hexblade",
      varName: 'wubsCursedHexblade',
      intellect: 21,
      spellPower: 217,
      critRating: 20,
      hitRating: 13,
      mp5: 6,
      id: 33354,
      source: ItemSource.ZulAman,
      phase: 4
    },
    {
      name: "Vengeful Gladiator's Spellblade",
      varName: 'vengefulGladiatorsSpellblade',
      stamina: 30,
      intellect: 20,
      spellPower: 247,
      hitRating: 17,
      resilienceRating: 17,
      id: 33763,
      source: ItemSource.Arena,
      phase: 4 // possibly phase 3
    },
    {
      name: 'Tempest of Chaos',
      varName: 'tempestOfChaos',
      stamina: 30,
      intellect: 22,
      spellPower: 259,
      critRating: 24,
      hitRating: 17,
      id: 30910,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: 'The Maelstrom Fury',
      varName: 'theMaelstromFury',
      stamina: 33,
      intellect: 21,
      spellPower: 236,
      critRating: 22,
      id: 32237,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: "Merciless Gladiator's Spellblade",
      varName: 'mercilessGladiatorsSpellblade',
      stamina: 27,
      intellect: 18,
      spellPower: 225,
      resilienceRating: 18,
      hitRating: 15,
      id: 32053,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: 'Fang of the Leviathan',
      varName: 'fangOfTheLeviathan',
      stamina: 28,
      intellect: 20,
      spellPower: 221,
      critRating: 21,
      id: 30095,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Bloodmaw Magus-Blade',
      varName: 'bloodmawMagusBlade',
      stamina: 16,
      intellect: 15,
      spellPower: 203,
      critRating: 25,
      id: 28802,
      source: ItemSource.GruulsLair,
      phase: 1
    },
    {
      name: 'Nathrezim Mindblade',
      varName: 'nathrezimMindblade',
      stamina: 18,
      intellect: 18,
      spellPower: 203,
      critRating: 23,
      id: 28770,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Talon of the Tempest',
      varName: 'talonOfTheTempest',
      intellect: 10,
      sockets: [SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        intellect: 3
      },
      spellPower: 194,
      critRating: 19,
      hitRating: 9,
      id: 30723,
      source: ItemSource.Doomwalker,
      phase: 1
    },
    {
      name: 'Eternium Runed Blade',
      varName: 'eterniumRunedBlade',
      intellect: 19,
      spellPower: 168,
      critRating: 21,
      id: 23554,
      source: ItemSource.BlacksmithingBoE,
      phase: 1
    },
    {
      name: "Gladiator's Spellblade",
      varName: 'gladiatorsSpellblade',
      stamina: 28,
      intellect: 18,
      spellPower: 199,
      resilienceRating: 18,
      id: 28297,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Stormcaller',
      varName: 'stormcaller',
      stamina: 12,
      intellect: 12,
      spellPower: 159,
      critRating: 21,
      id: 29155,
      source: ItemSource.ThrallmarExalted,
      phase: 1
    },
    {
      name: 'Blade of the Archmage',
      varName: 'bladeOfTheArchmage',
      stamina: 13,
      intellect: 11,
      spellPower: 159,
      critRating: 21,
      id: 29153,
      source: ItemSource.HonorHoldExalted,
      phase: 1
    },
    {
      name: 'Blade of Wizardry',
      varName: 'bladeOfWizardry',
      spellPower: 159,
      id: 31336,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: 'Mana Wrath',
      varName: 'manaWrath',
      stamina: 24,
      intellect: 18,
      spellPower: 126,
      id: 27899,
      source: ItemSource.Mechanar,
      phase: 1
    },
    {
      name: 'Greatsword of Horrid Dreams',
      varName: 'greatswordOfHorridDreams',
      stamina: 15,
      intellect: 14,
      spellPower: 130,
      hitRating: 14,
      id: 27905,
      source: ItemSource.ShadowLabyrinth,
      phase: 1
    },
    {
      name: 'Time-Shifted Dagger',
      varName: 'timeShiftedDagger',
      stamina: 15,
      intellect: 15,
      spellPower: 85,
      critRating: 13,
      id: 27431,
      source: ItemSource.OldHillsbradFoothills,
      phase: 1
    },
    {
      name: 'Starlight Dagger',
      varName: 'starlightDagger',
      stamina: 15,
      intellect: 15,
      spellPower: 121,
      hitRating: 16,
      id: 27543,
      source: ItemSource.HeroicSlavePens,
      phase: 1
    },
    {
      name: 'The Willbreaker',
      varName: 'theWillbreaker',
      stamina: 15,
      intellect: 14,
      spellPower: 121,
      critRating: 17,
      id: 27512,
      source: ItemSource.HeroicBloodFurnace,
      phase: 1
    },
    {
      name: 'Continuum Blade',
      varName: 'continuumBlade',
      stamina: 30,
      intellect: 11,
      spellPower: 121,
      hitRating: 8,
      id: 29185,
      source: ItemSource.KeepersOfTimeRevered,
      phase: 1
    },
    {
      name: 'Runesong Dagger',
      varName: 'runesongDagger',
      stamina: 12,
      intellect: 11,
      spellPower: 121,
      critRating: 20,
      id: 27868,
      source: ItemSource.ShatteredHalls,
      phase: 1
    },
    {
      name: 'Zangartooth Shortblade',
      varName: 'zangartoothShortblade',
      stamina: 13,
      intellect: 14,
      spellPower: 61,
      hitRating: 12,
      id: 24453,
      source: ItemSource.Underbog,
      phase: 1
    },
    {
      name: 'Spellfire Longsword',
      varName: 'spellfireLongsword',
      stamina: 15,
      intellect: 14,
      spellPower: 56,
      hitRating: 10,
      id: 24361,
      source: ItemSource.SlavePens,
      phase: 1
    },
    {
      name: 'Wraith Blade',
      varName: 'wraithBlade',
      stamina: 10,
      intellect: 8,
      spellPower: 95,
      hitRating: 8,
      critRating: 14,
      id: 22807,
      source: ItemSource.Naxxramas,
      phase: 0
    }
  ],
  offhand: [
    {
      name: 'Heart of the Pit',
      varName: 'heartOfThePit',
      stamina: 33,
      intellect: 21,
      spellPower: 39,
      hasteRating: 32,
      id: 34179,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Fetish of the Primal Gods',
      varName: 'fetishOfThePrimalGods',
      stamina: 24,
      intellect: 17,
      hasteRating: 17,
      spellPower: 37,
      id: 33334,
      source: ItemSource.ZulAman,
      phase: 4,
    },
    {
      name: 'Chronicle of Dark Secrets',
      varName: 'chronicleOfDarkSecrets',
      stamina: 16,
      intellect: 12,
      spellPower: 42,
      hitRating: 17,
      critRating: 23,
      id: 30872,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: 'Blind-Seers Icon',
      varName: 'blindSeersIcon',
      stamina: 25,
      intellect: 16,
      spellPower: 42,
      hitRating: 24,
      id: 32361,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Touch of Inspiration',
      varName: 'touchOfInspiration',
      stamina: 24,
      intellect: 21,
      spellPower: 22,
      mp5: 12,
      id: 32350,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Scepter of Purification',
      varName: 'scepterOfPurification',
      stamina: 24,
      intellect: 17,
      spirit: 25,
      spellPower: 26,
      id: 30911,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: "Merciless Gladiator's Endgame",
      varName: 'mercilessGladiatorsEndgame',
      stamina: 27,
      intellect: 19,
      spellPower: 33,
      resilienceRating: 27,
      id: 31978,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: 'Fathomstone',
      varName: 'fathomstone',
      stamina: 16,
      intellect: 12,
      spellPower: 36,
      critRating: 23,
      id: 30049,
      source: ItemSource.SerpentshrineCavern,
      phase: 2
    },
    {
      name: 'Talisman of Nightbane',
      varName: 'talismanOfNightbane',
      stamina: 19,
      intellect: 19,
      spellPower: 28,
      critRating: 17,
      id: 28603,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Orb of the Soul-Eater',
      varName: 'orbOfTheSoulEater',
      stamina: 18,
      shadowPower: 51,
      id: 29272,
      source: ItemSource.BadgeOfJustice,
      phase: 1
    },
    {
      name: 'Flametongue Seal',
      varName: 'flametongueSeal',
      firePower: 49,
      critRating: 17,
      id: 29270,
      source: ItemSource.BadgeOfJustice,
      phase: 1
    },
    {
      name: 'Jewel of Infinite Possibilities',
      varName: 'jewelOfInfinitePossibilities',
      stamina: 19,
      intellect: 18,
      spellPower: 23,
      hitRating: 21,
      id: 28734,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Khadgar's Knapsack",
      varName: 'khadgarsKnapsack',
      spellPower: 49,
      id: 29273,
      source: ItemSource.BadgeOfJustice,
      phase: 1
    },
    {
      name: 'Karaborian Talisman',
      varName: 'karaborianTalisman',
      stamina: 23,
      intellect: 23,
      spellPower: 35,
      id: 28781,
      source: ItemSource.MagtheridonsLair,
      phase: 1
    },
    {
      name: 'Manual of the Nethermancer',
      varName: 'manualOfTheNethermancer',
      stamina: 12,
      intellect: 15,
      spellPower: 21,
      critRating: 19,
      id: 28260,
      source: ItemSource.Mechanar,
      phase: 1
    },
    {
      name: "Gladiator's Endgame",
      varName: 'gladiatorsEndgame',
      stamina: 21,
      intellect: 14,
      spellPower: 19,
      resilienceRating: 15,
      id: 28346,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Lamp of Peaceful Radiance',
      varName: 'lampOfPeacefulRadiance',
      stamina: 13,
      intellect: 14,
      spellPower: 21,
      critRating: 13,
      hitRating: 12,
      id: 28412,
      source: ItemSource.Arcatraz,
      phase: 1
    },
    {
      name: 'Star-Heart Lamp',
      varName: 'starHeartLamp',
      stamina: 17,
      intellect: 18,
      spellPower: 22,
      hitRating: 12,
      id: 28187,
      source: ItemSource.BlackMorass,
      phase: 1
    },
    {
      name: "Hortus' Seal of Brilliance",
      varName: 'hortusSealOfBrilliance',
      stamina: 18,
      intellect: 20,
      spellPower: 23,
      id: 27534,
      source: ItemSource.ShatteredHalls,
      phase: 1
    },
    {
      name: 'Lordaeron Medical Guide',
      varName: 'lordaeronMedicalGuide',
      stamina: 12,
      intellect: 10,
      spirit: 16,
      spellPower: 16,
      id: 28213,
      source: ItemSource.HeroicOldHillsbradFoothills,
      phase: 1
    },
    {
      name: 'Swamplight Lantern',
      varName: 'swamplightLantern',
      intellect: 22,
      spellPower: 14,
      mp5: 6,
      id: 27714,
      source: ItemSource.HeroicSlavePens,
      phase: 1
    },
    {
      name: 'The Saga of Terokk',
      varName: 'sagaOfTerokk',
      intellect: 23,
      spellPower: 28,
      id: 29330,
      source: ItemSource.SethekkHallsQuest,
      phase: 1
    },
    {
      name: "Sapphiron's Left Eye",
      varName: 'sapphironsLeftEye',
      stamina: 12,
      intellect: 8,
      spellPower: 26,
      critRating: 14,
      hitRating: 8,
      id: 23049,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: 'Scepter of Interminable Focus',
      varName: 'scepterOfInterminableFocus',
      spellPower: 9,
      critRating: 14,
      hitRating: 8,
      id: 22329,
      source: ItemSource.StratholmeLive,
      phase: 0
    }
  ],
  twohand: [
    {
      name: "Grand Magister's Staff of Torrents",
      varName: 'grandMagistersStaffOfTorrents',
      stamina: 57,
      intellect: 52,
      sockets: [SocketColor.Yellow, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 266,
      hitRating: 50,
      critRating: 49,
      id: 34182,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: 'Sun-infused Focus Staff',
      varName: 'suninfusedFocusStaff',
      stamina: 37,
      intellect: 27,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Yellow],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 121,
      hitRating: 23,
      id: 34797,
      source: ItemSource.MagistersTerrace,
      phase: 5
    },
    {
      name: 'Amani Divining Staff',
      varName: 'amaniDiviningStaff',
      stamina: 58,
      intellect: 47,
      sockets: [SocketColor.Red, SocketColor.Yellow, SocketColor.Blue],
      socketBonus: {
        spellPower: 5
      },
      spellPower: 217,
      critRating: 31,
      id: 33494,
      source: ItemSource.ZulAman,
      phase: 4
    },
    {
      name: "Zhar'doom, Greatstaff of the Devourer",
      varName: 'zhardoomGreatstaffOfTheDevourer',
      stamina: 70,
      intellect: 47,
      spellPower: 259,
      critRating: 36,
      hasteRating: 55,
      id: 32374,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Staff of Immaculate Recovery',
      varName: 'staffOfImmaculateRecovery',
      stamina: 73,
      intellect: 51,
      spirit: 35,
      spellPower: 148,
      mp5: 14,
      id: 32344,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: 'Apostle of Argus',
      varName: 'apostleOfArgus',
      stamina: 62,
      intellect: 59,
      spellPower: 162,
      mp5: 23,
      id: 30908,
      source: ItemSource.MountHyjal,
      phase: 3
    },
    {
      name: "Merciless Gladiator's War Staff",
      varName: 'mercilessGladiatorsWarStaff',
      stamina: 55,
      intellect: 42,
      spellPower: 225,
      critRating: 42,
      hitRating: 24,
      resilienceRating: 29,
      id: 32055,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: "Staff of Disintegration",
      varName: 'staffOfDisintegration',
      stamina: 75,
      intellect: 50,
      critRating: 75,
      spellPower: 325,
      id: 30313,
      source: ItemSource.KaelThasEncounter,
      phase: 2
    },
    {
      name: 'The Nexus Key',
      varName: 'theNexusKey',
      stamina: 76,
      intellect: 52,
      spellPower: 236,
      critRating: 51,
      id: 29988,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: 'Frostscythe of Lord Ahune',
      varName: 'frostscytheOfLordAhune',
      stamina: 40,
      intellect: 35,
      spellPower: 176,
      mp5: 13,
      critRating: 33,
      id: 35514,
      source: ItemSource.LordAhune,
      phase: 1
    },
    {
      name: "Gladiator's War Staff",
      varName: 'gladiatorsWarStaff',
      stamina: 48,
      intellect: 35,
      spellPower: 199,
      hitRating: 21,
      critRating: 36,
      resilienceRating: 25,
      id: 24557,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: "Terokk's Shadowstaff",
      varName: 'terokksShadowstaff',
      stamina: 40,
      intellect: 42,
      spellPower: 168,
      critRating: 37,
      id: 29355,
      source: ItemSource.HeroicSethekkHalls,
      phase: 1
    },
    {
      name: 'Auchenai Staff',
      varName: 'auchenaiStaff',
      intellect: 46,
      spellPower: 121,
      hitRating: 19,
      critRating: 26,
      id: 29130,
      source: ItemSource.AldorRevered,
      phase: 1
    },
    {
      name: 'Staff of Infinite Mysteries',
      varName: 'staffOfInfiniteMysteries',
      stamina: 61,
      intellect: 51,
      spellPower: 185,
      hitRating: 23,
      id: 28633,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: 'Warpstaff of Arcanum',
      varName: 'warpstaffOfArcanum',
      stamina: 37,
      intellect: 38,
      spellPower: 121,
      critRating: 26,
      hitRating: 16,
      id: 28341,
      source: ItemSource.Botanica,
      phase: 1
    },
    {
      name: 'Grand Scepter of the Nexus-Kings',
      varName: 'grandScepterOfTheNexusKings',
      stamina: 45,
      intellect: 43,
      spellPower: 121,
      hitRating: 19,
      id: 27842,
      source: ItemSource.HeroicManaTombs,
      phase: 1
    },
    {
      name: 'Bloodfire Greatstaff',
      varName: 'bloodfireGreatstaff',
      stamina: 42,
      intellect: 42,
      spellPower: 121,
      critRating: 28,
      id: 28188,
      source: ItemSource.BlackMorass,
      phase: 1
    },
    {
      name: 'Staff of Polarities',
      varName: 'staffOfPolarities',
      stamina: 34,
      intellect: 33,
      spellPower: 67,
      hitRating: 28,
      id: 25950,
      source: ItemSource.ManaTombs,
      phase: 1
    },
    {
      name: 'Atiesh, Greatstaff of the Guardian',
      varName: 'atieshGreatstaffOfTheGuardian',
      stamina: 30,
      intellect: 29,
      spellPower: 183,
      critRating: 28,
      id: 22630,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: 'Brimstone Staff',
      varName: 'brimstoneStaff',
      stamina: 31,
      intellect: 30,
      spellPower: 113,
      hitRating: 16,
      critRating: 14,
      id: 22800,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: 'Soulseeker',
      varName: 'soulseeker',
      stamina: 30,
      intellect: 31,
      spellPower: 126,
      critRating: 28,
      spellPenetration: 25,
      id: 22799,
      source: ItemSource.Naxxramas,
      phase: 0
    }
  ],
  wand: [
    {
      name: 'Wand of the Demonsoul',
      varName: 'wandOfTheDemonsoul',
      stamina: 9,
      intellect: 10,
      sockets: [SocketColor.Yellow],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 22,
      hasteRating: 18,
      id: 34347,
      source: ItemSource.Sunwell,
      phase: 5
    },
    {
      name: "Carved Witch Doctor's Stick",
      varName: 'carvedWitchDoctorsStick',
      stamina: 9,
      intellect: 15,
      spellPower: 18,
      sockets: [SocketColor.Blue],
      socketBonus: {
        spellPower: 2
      },
      id: 33192,
      source: ItemSource.BadgeOfJustice,
      phase: 4 // confirm
    },
    {
      name: 'Wand of Prismatic Focus',
      varName: 'wandOfPrismaticFocus',
      stamina: 21,
      spellPower: 25,
      hitRating: 13,
      id: 32343,
      source: ItemSource.BlackTemple,
      phase: 3
    },
    {
      name: "Merciless Gladiator's Touch of Defeat",
      varName: 'mercilessGladiatorsTouchOfDefeat',
      stamina: 15,
      intellect: 13,
      spellPower: 16,
      resilienceRating: 14,
      id: 32962,
      source: ItemSource.PVP,
      phase: 2
    },
    {
      name: 'Wand of the Forgotten Star',
      varName: 'wandOfTheForgottenStar',
      spellPower: 23,
      critRating: 14,
      hitRating: 11,
      id: 29982,
      source: ItemSource.TempestKeep,
      phase: 2
    },
    {
      name: 'Eredar Wand of Obliteration',
      varName: 'eredarWandOfObliteration',
      stamina: 10,
      intellect: 11,
      spellPower: 16,
      critRating: 14,
      id: 28783,
      source: ItemSource.MagtheridonsLair,
      phase: 1
    },
    {
      name: 'Tirisfal Wand of Ascendancy',
      varName: 'tirisfalWandOfAscendancy',
      stamina: 10,
      intellect: 9,
      spellPower: 15,
      hitRating: 11,
      id: 28673,
      source: ItemSource.Karazhan,
      phase: 1
    },
    {
      name: "Nether Core's Control Rod",
      varName: 'netherCoresControlRod',
      stamina: 9,
      intellect: 10,
      spellPower: 13,
      hitRating: 8,
      id: 28386,
      source: ItemSource.Arcatraz,
      phase: 1
    },
    {
      name: "Gladiator's Touch of Defeat",
      varName: 'gladiatorsTouchOfDefeat',
      stamina: 15,
      intellect: 11,
      spellPower: 14,
      resilienceRating: 12,
      id: 28320,
      source: ItemSource.PVP,
      phase: 1
    },
    {
      name: 'Illidari Rod of Discipline',
      varName: 'illidariRodOfDiscipline',
      stamina: 9,
      sockets: [SocketColor.Blue],
      socketBonus: {
        spellPower: 2
      },
      spellPower: 13,
      id: 32872,
      source: ItemSource.ShadowmoonValleyQuest,
      phase: 1
    },
    {
      name: 'The Black Stalk',
      varName: 'theBlackStalk',
      stamina: 10,
      spellPower: 20,
      critRating: 11,
      id: 29350,
      source: ItemSource.HeroicUnderbog,
      phase: 1
    },
    {
      name: 'Voidfire Wand',
      varName: 'voidfireWand',
      stamina: 9,
      intellect: 9,
      spellPower: 11,
      hitRating: 7,
      id: 25939,
      source: ItemSource.ManaTombs,
      phase: 1
    },
    {
      name: 'Nesingwary Safari Stick',
      varName: 'nesingwarySafariStick',
      spellPower: 14,
      critRating: 12,
      id: 25640,
      source: ItemSource.NagrandQuest,
      phase: 1
    },
    {
      name: 'Flawless Wand of Shadow Wrath',
      varName: 'flawlessWandOfShadowWrath',
      shadowPower: 25,
      id: fakeItemIds.flawlessWandOfShadowWrath,
      displayId: 25295,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: 'Flawless Wand of Fire Wrath',
      varName: 'flawlessWandOfFireWrath',
      firePower: 25,
      id: fakeItemIds.flawlessWandOfFireWrath,
      displayId: 25295,
      source: ItemSource.WorldDrop,
      phase: 1
    },
    {
      name: 'Nexus Torch',
      varName: 'nexusTorch',
      stamina: 9,
      intellect: 10,
      spellPower: 8,
      critRating: 11,
      id: 27540,
      source: ItemSource.ShatteredHalls,
      phase: 1
    },
    {
      name: 'Wand of the Netherwing',
      varName: 'wandOfTheNetherwing',
      stamina: 19,
      spellPower: 16,
      id: 27890,
      source: ItemSource.ShadowLabyrinth,
      phase: 1
    },
    {
      name: "Nethekurse's Rod of Torment",
      varName: 'nethekursesRodOfTorment',
      intellect: 10,
      spellPower: 11,
      critRating: 10,
      id: 25806,
      source: ItemSource.ShatteredHallsQuest,
      phase: 1
    },
    {
      name: 'Master Firestone',
      varName: 'masterFirestone',
      firePower: 30,
      id: 22128,
      source: ItemSource.WarlockSpell,
      phase: 1
    },
    {
      name: 'Master Spellstone',
      varName: 'masterSpellstone',
      critRating: 20,
      id: 22646,
      source: ItemSource.WarlockSpell,
      phase: 1
    },
    {
      name: 'Wand of Fates',
      varName: 'wandOfFates',
      stamina: 7,
      intellect: 7,
      spellPower: 12,
      hitRating: 8,
      id: 22820,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: 'Doomfinger',
      varName: 'doomfinger',
      spellPower: 16,
      critRating: 14,
      id: 22821,
      source: ItemSource.Naxxramas,
      phase: 0
    }
  ]
}
