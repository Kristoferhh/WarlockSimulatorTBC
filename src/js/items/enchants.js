const enchants = {
  head: {
    glyphOfPower: {
      name: 'Glyph of Power',
      spellPower: 22,
      hitRating: 14,
      id: 35447,
      source: "The Sha'tar - Revered",
      phase: 1
    },
    glyphOfRenewal: {
      name: 'Glyph of Renewal',
      spellPower: 12,
      mp5: 7,
      id: 35445,
      source: 'Honor Hold/Thrallmar - Revered',
      phase: 1
    },
    glyphOfFireWarding: {
      name: 'Glyph of Fire Warding',
      fireResist: 20,
      id: 35456,
      source: 'Honor Hold/Thrallmar - Honored',
      phase: 1
    },
    glyphOfShadowWarding: {
      name: 'Glyph of Shadow Warding',
      shadowResist: 20,
      id: 35458,
      source: 'Lower City - Honored',
      phase: 1
    },
    glyphOfArcaneWarding: {
      name: 'Glyph of Arcane Warding',
      arcaneResist: 20,
      id: 35455,
      source: "The Sha'tar - Honored",
      phase: 1
    },
    glyphOfNatureWarding: {
      name: 'Glyph of Nature Warding',
      natureResist: 20,
      id: 35454,
      source: 'Cenarion Expedition - Honored',
      phase: 1
    },
    glyphOfFrostWarding: {
      name: 'Glyph of Frost Warding',
      frostResist: 20,
      id: 35457,
      source: 'Keepers of Time - Honored',
      phase: 1
    },
    glyphOfChromaticWarding: {
      name: 'Glyph of Chromatic Warding',
      arcaneResist: 8,
      fireResist: 8,
      natureResist: 8,
      shadowResist: 8,
      frostResist: 8,
      id: 37889,
      source: 'Lower City - Honored',
      phase: 1
    }, hoodooHex: {
      name: 'Hoodoo Hex',
      stamina: 10,
      spellPower: 18,
      id: 19788,
      source: "Zul'Gurub",
      phase: 0
    }
  },
  shoulders: {
    greaterInscriptionOfDiscipline: {
      name: 'Greater Inscription of Discipline',
      spellPower: 18,
      critRating: 10,
      id: 35406,
      source: 'The Aldor - Exalted',
      phase: 1
    },
    greaterInscriptionOfTheOrb: {
      name: 'Greater Inscription of the Orb',
      spellPower: 12,
      critRating: 15,
      id: 35437,
      source: 'The Scryers - Exalted',
      phase: 1
    },
    powerOfTheScourge: {
      name: 'Power of the Scourge',
      spellPower: 15,
      critRating: 14,
      id: 29467,
      source: 'Naxxramas',
      phase: 0
    },
    inscriptionOfEndurance: {
      name: 'Inscription of Endurance',
      fireResist: 7,
      shadowResist: 7,
      arcaneResist: 7,
      natureResist: 7,
      frostResist: 7,
      id: 35441,
      source: 'The Violet Eye - Honored',
      phase: 1
    }
  },
  back: {
    subtlety: {
      name: 'Subtlety',
      threatReduction: 0.02,
      id: 25084,
      source: 'Enchanting',
      phase: 0
    },
    spellPenetration: {
      name: 'Spell Penetration',
      spellPen: 20,
      id: 34003,
      source: 'Enchanting',
      phase: 1
    },
    majorResistance: {
      name: 'Major Resistance',
      fireResist: 7,
      shadowResist: 7,
      arcaneResist: 7,
      natureResist: 7,
      frostResist: 7,
      id: 27962,
      source: 'Enchanting',
      phase: 1
    },
    greaterArcaneResistance: {
      name: 'Greater Arcane Resistance',
      arcaneResist: 15,
      id: 34005,
      source: 'Enchanting',
      phase: 1
    },
    greaterFireResistance: {
      name: 'Greater Fire Resistance',
      fireResist: 15,
      id: 25081,
      source: 'Enchanting',
      phase: 0
    },
    greaterShadowResistance: {
      name: 'Greater Shadow Resistance',
      shadowResist: 15,
      id: 34006,
      source: 'Enchanting',
      phase: 1
    },
    greaterNatureResistance: {
      name: 'Greater Nature Resistance',
      natureResist: 15,
      id: 25082,
      source: 'Enchanting',
      phase: 0
    }
  },
  chest: {
    exceptionalStats: {
      name: 'Exceptional Stats',
      stamina: 6,
      intellect: 6,
      spirit: 6,
      id: 27960,
      source: 'Enchanting',
      phase: 1
    },
    majorResilience: {
      name: 'Major Resilience',
      resilience: 15,
      id: 33992,
      source: 'Enchanting',
      phase: 1
    },
    magistersArmorKit: {
      name: "Magister's Armor Kit",
      mp5: 3,
      id: 32399,
      source: 'Leatherworking',
      phase: 1
    },
    arcaneArmorKit: {
      name: 'Arcane Armor Kit',
      arcaneResist: 8,
      id: 35420,
      source: 'Leatherworking',
      phase: 1
    },
    flameArmorKit: {
      name: 'Flame Armor Kit',
      fireResist: 8,
      id: 35416,
      source: 'Leatherworking',
      phase: 1
    },
    shadowArmorKit: {
      name: 'Shadow Armor Kit',
      shadowResist: 8,
      id: 35415,
      source: 'Leatherworking',
      phase: 1
    },
    natureArmorKit: {
      name: 'Nature Armor Kit',
      natureResist: 8,
      id: 35419,
      source: 'Leatherworking',
      phase: 1
    },
    frostArmorKit: {
      name: 'Frost Armor Kit',
      frostResist: 8,
      id: 35418,
      source: 'Leatherworking',
      phase: 1
    },
    greaterStats: {
      name: 'Greater Stats',
      stamina: 4,
      intellect: 4,
      spirit: 4,
      id: 20025,
      source: 'Enchanting',
      phase: 0
    }
  },
  bracer: {
    spellpower: {
      name: 'Spellpower',
      spellPower: 15,
      id: 27917,
      source: 'Enchanting',
      phase: 1
    },
    majorIntellect: {
      name: 'Major Intellect',
      intellect: 12,
      id: 34001,
      source: 'Enchanting',
      phase: 1
    },
    stats: {
      name: 'Stats',
      stamina: 4,
      intellect: 4,
      spirit: 4,
      id: 27905,
      source: 'Enchanting',
      phase: 1
    }
  },
  gloves: {
    majorSpellpower: {
      name: 'Major Spellpower',
      spellPower: 20,
      id: 33997,
      source: 'Enchanting',
      phase: 1
    },
    spellStrike: {
      name: 'Spell Strike',
      hitRating: 15,
      id: 33994,
      source: 'Enchanting',
      phase: 1
    },
    blasting: {
      name: 'Blasting',
      critRating: 10,
      id: 33993,
      source: 'Enchanting',
      phase: 1
    },
    magistersArmorKit: {
      name: "Magister's Armor Kit",
      mp5: 3,
      id: 32399,
      source: 'Leatherworking',
      phase: 1
    },
    arcaneArmorKit: {
      name: 'Arcane Armor Kit',
      arcaneResist: 8,
      id: 35420,
      source: 'Leatherworking',
      phase: 1
    },
    flameArmorKit: {
      name: 'Flame Armor Kit',
      fireResist: 8,
      id: 35416,
      source: 'Leatherworking',
      phase: 1
    },
    shadowArmorKit: {
      name: 'Shadow Armor Kit',
      shadowResist: 8,
      id: 35415,
      source: 'Leatherworking',
      phase: 1
    },
    natureArmorKit: {
      name: 'Nature Armor Kit',
      natureResist: 8,
      id: 35419,
      source: 'Leatherworking',
      phase: 1
    },
    frostArmorKit: {
      name: 'Frost Armor Kit',
      frostResist: 8,
      id: 29486,
      source: 'Leatherworking',
      phase: 1
    }
  },
  legs: {
    runicSpellthread: {
      name: 'Runic Spellthread',
      stamina: 20,
      spellPower: 35,
      id: 31372,
      source: 'Tailoring',
      phase: 1
    },
    mysticSpellthread: {
      name: 'Mystic Spellthread',
      stamina: 15,
      spellPower: 25,
      id: 31371,
      source: 'Tailoring',
      phase: 1
    },
    magistersArmorKit: {
      name: "Magister's Armor Kit",
      mp5: 3,
      id: 32399,
      source: 'Leatherworking',
      phase: 1
    },
    arcaneArmorKit: {
      name: 'Arcane Armor Kit',
      arcaneResist: 8,
      id: 35420,
      source: 'Leatherworking',
      phase: 1
    },
    flameArmorKit: {
      name: 'Flame Armor Kit',
      fireResist: 8,
      id: 35416,
      source: 'Leatherworking',
      phase: 1
    },
    shadowArmorKit: {
      name: 'Shadow Armor Kit',
      shadowResist: 8,
      id: 35415,
      source: 'Leatherworking',
      phase: 1
    },
    natureArmorKit: {
      name: 'Nature Armor Kit',
      natureResist: 8,
      id: 35419,
      source: 'Leatherworking',
      phase: 1
    },
    frostArmorKit: {
      name: 'Frost Armor Kit',
      frostResist: 8,
      id: 29486,
      source: 'Leatherworking',
      phase: 1
    },
    hoodooHex: {
      name: 'Hoodoo Hex',
      stamina: 10,
      spellPower: 18,
      id: 19788,
      source: "Zul'Gurub",
      phase: 0
    }
  },
  boots: {
    fortitude: {
      name: 'Fortitude',
      stamina: 12,
      id: 46490,
      source: 'Enchanting',
      phase: 1
    },
    magistersArmorKit: {
      name: "Magister's Armor Kit",
      mp5: 3,
      id: 32399,
      source: 'Leatherworking',
      phase: 1
    },
    arcaneArmorKit: {
      name: 'Arcane Armor Kit',
      arcaneResist: 8,
      id: 35420,
      source: 'Leatherworking',
      phase: 1
    },
    flameArmorKit: {
      name: 'Flame Armor Kit',
      fireResist: 8,
      id: 35416,
      source: 'Leatherworking',
      phase: 1
    },
    shadowArmorKit: {
      name: 'Shadow Armor Kit',
      shadowResist: 8,
      id: 35415,
      source: 'Leatherworking',
      phase: 1
    },
    natureArmorKit: {
      name: 'Nature Armor Kit',
      natureResist: 8,
      id: 35419,
      source: 'Leatherworking',
      phase: 1
    },
    frostArmorKit: {
      name: 'Frost Armor Kit',
      frostResist: 8,
      id: 29486,
      source: 'Leatherworking',
      phase: 1
    }
  },
  ring: {
    spellpower: {
      name: 'Spellpower',
      spellPower: 12,
      id: 27924,
      source: 'Enchanting',
      phase: 1
    },
    stats: {
      name: 'Stats',
      stamina: 4,
      intellect: 4,
      spirit: 4,
      id: 27927,
      source: 'Enchanting',
      phase: 1
    }
  },
  weapon: {
    soulfrost: {
      name: 'Soulfrost',
      shadowPower: 54,
      id: 27982,
      source: 'Enchanting',
      phase: 1
    },
    majorSpellpower: {
      name: 'Major Spellpower',
      spellPower: 40,
      id: 27975,
      source: 'Enchanting',
      phase: 1
    },
    sunfire: {
      name: 'Sunfire',
      firePower: 50,
      id: 27981,
      source: 'Enchanting',
      phase: 1
    },
    majorIntellect: {
      name: 'Major Intellect',
      intellect: 30,
      id: 27968,
      source: 'Enchanting',
      phase: 1
    },
    spellPower: {
      name: 'Spell Power',
      spellPower: 30,
      id: 22749,
      source: 'Enchanting',
      phase: 0
    }
  }
}
