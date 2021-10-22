import { ItemSlot, Enchant, ItemSource } from '../Types';

export const Enchants: {[key in ItemSlot]: Enchant[]} = {
  head: [
    {
      name: 'Glyph of Power',
      varName: 'glyphOfPower',
      spellPower: 22,
      hitRating: 14,
      id: 35447,
      source: ItemSource.ShatarRevered,
      phase: 1
    },
    {
      name: 'Glyph of Renewal',
      varName: 'glyphOfRenewal',
      spellPower: 12,
      mp5: 7,
      id: 35445,
      source: ItemSource.HonorHoldOrThrallmarRevered,
      phase: 1
    },
    {
      name: 'Glyph of Fire Warding',
      varName: 'glyphOfFireWarding',
      fireResist: 20,
      id: 35456,
      source: ItemSource.HonorHoldOrThrallmarHonored,
      phase: 1
    },
    {
      name: 'Glyph of Shadow Warding',
      varName: 'glyphOfShadowWarding',
      shadowResist: 20,
      id: 35458,
      source: ItemSource.LowerCityHonored,
      phase: 1
    },
    {
      name: 'Glyph of Arcane Warding',
      varName: 'glyphOfArcaneWarding',
      arcaneResist: 20,
      id: 35455,
      source: ItemSource.ShatarHonored,
      phase: 1
    },
    {
      name: 'Glyph of Nature Warding',
      varName: 'glyphOfNatureWarding',
      natureResist: 20,
      id: 35454,
      source: ItemSource.CenarionExpeditionHonored,
      phase: 1
    },
    {
      name: 'Glyph of Frost Warding',
      varName: 'glyphOfFrostWarding',
      frostResist: 20,
      id: 35457,
      source: ItemSource.KeepersOfTimeHonored,
      phase: 1
    },
    {
      name: 'Glyph of Chromatic Warding',
      varName: 'glyphOfChromaticWarding',
      arcaneResist: 8,
      fireResist: 8,
      natureResist: 8,
      shadowResist: 8,
      frostResist: 8,
      id: 37889,
      source: ItemSource.LowerCityHonored,
      phase: 1
    },
    {
      name: 'Hoodoo Hex',
      varName: 'hoodooHex',
      stamina: 10,
      spellPower: 18,
      id: 24165,
      source: ItemSource.ZulGurub,
      phase: 0
    }
  ],
  neck: [],
  shoulders: [
    {
      name: 'Greater Inscription of Discipline',
      varName: 'greaterInscriptionOfDiscipline',
      spellPower: 18,
      critRating: 10,
      id: 35406,
      source: ItemSource.AldorExalted,
      phase: 1
    },
    {
      name: 'Greater Inscription of the Orb',
      varName: 'greaterInscriptionOfTheOrb',
      spellPower: 12,
      critRating: 15,
      id: 35437,
      source: ItemSource.ScryersExalted,
      phase: 1
    },
    {
      name: 'Inscription of Discipline',
      varName: 'inscriptionOfDiscipline',
      spellPower: 15,
      id: 35405,
      source: ItemSource.AldorHonored,
      phase: 1
    },
    {
      name: 'Inscription of the Orb',
      varName: 'inscriptionOfTheOrb',
      critRating: 13,
      id: 35436,
      source: ItemSource.ScryersHonored,
      phase: 1,
    },
    {
      name: 'Inscription of Endurance',
      varName: 'inscriptionOfEndurance',
      fireResist: 7,
      shadowResist: 7,
      arcaneResist: 7,
      natureResist: 7,
      frostResist: 7,
      id: 35441,
      source: ItemSource.VioletEyeHonored,
      phase: 1
    },
    {
      name: 'Power of the Scourge',
      varName: 'powerOfTheScourge',
      spellPower: 15,
      critRating: 14,
      id: 29467,
      source: ItemSource.Naxxramas,
      phase: 0
    },
    {
      name: 'Zandalar SIgnet of Mojo',
      varName: 'zandalarSignetOfMojo',
      spellPower: 18,
      id: 24421,
      source: ItemSource.ZandalarTribeExalted,
      phase: 0
    }
  ],
  back: [
    {
      name: 'Subtlety',
      varName: 'subtlety',
      threatReduction: 0.02,
      id: 25084,
      source: ItemSource.Enchanting,
      phase: 0
    },
    {
      name: 'Spell Penetration',
      varName: 'spellPenetration',
      spellPenetration: 20,
      id: 34003,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Major Resistance',
      varName: 'majorResistance',
      fireResist: 7,
      shadowResist: 7,
      arcaneResist: 7,
      natureResist: 7,
      frostResist: 7,
      id: 27962,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Greater Arcane Resistance',
      varName: 'greaterArcaneResistance',
      arcaneResist: 15,
      id: 34005,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Greater Fire Resistance',
      varName: 'greaterFireResistance',
      fireResist: 15,
      id: 25081,
      source: ItemSource.Enchanting,
      phase: 0
    },
    {
      name: 'Greater Shadow Resistance',
      varName: 'greaterShadowResistance',
      shadowResist: 15,
      id: 34006,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Greater Nature Resistance',
      varName: 'greaterNatureResistance',
      natureResist: 15,
      id: 25082,
      source: ItemSource.Enchanting,
      phase: 0
    }
  ],
  chest: [
    {
      name: 'Exceptional Stats',
      varName: 'exceptionalStats',
      stamina: 6,
      intellect: 6,
      spirit: 6,
      id: 27960,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Major Resilience',
      varName: 'majorResilience',
      resilienceRating: 15,
      id: 33992,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: "Magister's Armor Kit",
      varName: 'magistersArmorKit',
      mp5: 3,
      id: 32399,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Arcane Armor Kit',
      varName: 'arcaneArmorKit',
      arcaneResist: 8,
      id: 35420,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Flame Armor Kit',
      varName: 'flameArmorKit',
      fireResist: 8,
      id: 35416,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Shadow Armor Kit',
      varName: 'shadowArmorKit',
      shadowResist: 8,
      id: 35415,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Nature Armor Kit',
      varName: 'natureArmorKit',
      natureResist: 8,
      id: 35419,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Frost Armor Kit',
      varName: 'frostArmorKit',
      frostResist: 8,
      id: 35418,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Greater Stats',
      varName: 'greaterStats',
      stamina: 4,
      intellect: 4,
      spirit: 4,
      id: 20025,
      source: ItemSource.Enchanting,
      phase: 0
    }
  ],
  bracer: [
    {
      name: 'Spellpower',
      varName: 'spellpower',
      spellPower: 15,
      id: 27917,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Superior Healing',
      varName: 'superiorHealing',
      spellPower: 10,
      id: 27911,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Major Intellect',
      varName: 'majorIntellect',
      intellect: 12,
      id: 34001,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Stats',
      varName: 'stats',
      stamina: 4,
      intellect: 4,
      spirit: 4,
      id: 27905,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Healing Power',
      varName: 'healingPower',
      spellPower: 8,
      id: 23802,
      source: ItemSource.Enchanting,
      phase: 0
    }
  ],
  gloves: [
    {
      name: 'Major Spellpower',
      varName: 'majorSpellpower',
      spellPower: 20,
      id: 33997,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Spell Strike',
      varName: 'spellStrike',
      hitRating: 15,
      id: 33994,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Blasting',
      varName: 'blasting',
      critRating: 10,
      id: 33993,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: "Magister's Armor Kit",
      varName: 'magistersArmorKit',
      mp5: 3,
      id: 32399,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Arcane Armor Kit',
      varName: 'arcaneArmorKit',
      arcaneResist: 8,
      id: 35420,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Flame Armor Kit',
      varName: 'flameArmorKit',
      fireResist: 8,
      id: 35416,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Shadow Armor Kit',
      varName: 'shadowArmorKit',
      shadowResist: 8,
      id: 35415,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Nature Armor Kit',
      varName: 'natureArmorKit',
      natureResist: 8,
      id: 35419,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Frost Armor Kit',
      varName: 'frostArmorKit',
      frostResist: 8,
      id: 29486,
      source: ItemSource.Leatherworking,
      phase: 1
    }
  ],
  belt: [],
  legs: [
    {
      name: 'Runic Spellthread',
      varName: 'runicSpellthread',
      stamina: 20,
      spellPower: 35,
      id: 31372,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: 'Mystic Spellthread',
      varName: 'mysticSpellthread',
      stamina: 15,
      spellPower: 25,
      id: 31371,
      source: ItemSource.Tailoring,
      phase: 1
    },
    {
      name: "Magister's Armor Kit",
      varName: 'magistersArmorKit',
      mp5: 3,
      id: 32399,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Arcane Armor Kit',
      varName: 'arcaneArmorKit',
      arcaneResist: 8,
      id: 35420,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Flame Armor Kit',
      varName: 'flameArmorKit',
      fireResist: 8,
      id: 35416,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Shadow Armor Kit',
      varName: 'shadowArmorKit',
      shadowResist: 8,
      id: 35415,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Nature Armor Kit',
      varName: 'natureArmorKit',
      natureResist: 8,
      id: 35419,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Frost Armor Kit',
      varName: 'frostArmorKit',
      frostResist: 8,
      id: 29486,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Hoodoo Hex',
      varName: 'hoodooHex',
      stamina: 10,
      spellPower: 18,
      id: 24165,
      source: ItemSource.ZulGurub,
      phase: 0
    }
  ],
  boots: [
    {
      name: 'Fortitude',
      varName: 'fortitude',
      stamina: 12,
      id: 46490,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: "Boar's Speed",
      varName: 'boarsSpeed',
      stamina: 9,
      id: 34008,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: "Magister's Armor Kit",
      varName: 'magistersArmorKit',
      mp5: 3,
      id: 32399,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Arcane Armor Kit',
      varName: 'arcaneArmorKit',
      arcaneResist: 8,
      id: 35420,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Flame Armor Kit',
      varName: 'flameArmorKit',
      fireResist: 8,
      id: 35416,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Shadow Armor Kit',
      varName: 'shadowArmorKit',
      shadowResist: 8,
      id: 35415,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Nature Armor Kit',
      varName: 'natureArmorKit',
      natureResist: 8,
      id: 35419,
      source: ItemSource.Leatherworking,
      phase: 1
    },
    {
      name: 'Frost Armor Kit',
      varName: 'frostArmorKit',
      frostResist: 8,
      id: 29486,
      source: ItemSource.Leatherworking,
      phase: 1
    }
  ],
  ring: [
    {
      name: 'Spellpower',
      varName: 'spellpower',
      spellPower: 12,
      id: 27924,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Stats',
      varName: 'stats',
      stamina: 4,
      intellect: 4,
      spirit: 4,
      id: 27927,
      source: ItemSource.Enchanting,
      phase: 1
    }
  ],
  trinket: [],
  mainhand: [
    {
      name: 'Soulfrost',
      varName: 'soulfrost',
      shadowPower: 54,
      id: 27982,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Major Spellpower',
      varName: 'majorSpellpower',
      spellPower: 40,
      id: 27975,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Sunfire',
      varName: 'sunfire',
      firePower: 50,
      id: 27981,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Major Intellect',
      varName: 'majorIntellect',
      intellect: 30,
      id: 27968,
      source: ItemSource.Enchanting,
      phase: 1
    },
    {
      name: 'Spell Power',
      varName: 'spellPower',
      spellPower: 30,
      id: 22749,
      source: ItemSource.Enchanting,
      phase: 0
    }
  ],
  offhand: [],
  twohand: [],
  wand: [],
}
