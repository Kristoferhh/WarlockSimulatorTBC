const socketInfo = {
  meta: {
    iconName: 'UI-EmptySocket-Meta',
    gems: ['meta']
  },
  red: {
    iconName: 'UI-EmptySocket-Red',
    gems: ['red', 'orange', 'purple']
  },
  yellow: {
    iconName: 'UI-EmptySocket-Yellow',
    gems: ['yellow', 'orange', 'green']
  },
  blue: {
    iconName: 'UI-EmptySocket-Blue',
    gems: ['blue', 'green', 'purple']
  }
}

const gems = {
  meta: {
    35503: {
      name: 'Ember Skyfire Diamond',
      spellPower: 14,
      intellectModifier: 1.02,
      iconName: 'inv_misc_gem_diamond_07',
      phase: 1
    },
    34220: {
      name: 'Chaotic Skyfire Diamond',
      critRating: 12,
      iconName: 'inv_misc_gem_diamond_07',
      phase: 1
    },
    32641: {
      name: 'Imbued Unstable Diamond',
      spellPower: 14,
      iconName: 'inv_misc_gem_diamond_06',
      phase: 1
    },
    28557: {
      name: 'Swift Starfire Diamond',
      spellPower: 12,
      iconName: 'inv_misc_gem_diamond_07',
      phase: 1
    },
    25901: {
      name: 'Insightful Earthstorm Diamond',
      intellect: 12,
      iconName: 'inv_misc_gem_diamond_06',
      phase: 1
    },
    25893: {
      name: 'Mystical Skyfire Diamond',
      iconName: 'inv_misc_gem_diamond_07',
      phase: 1
    }
  },
  red: {
    35488: {
      name: 'Runed Crimson Spinel',
      spellPower: 12,
      iconName: 'inv_jewelcrafting_crimsonspinel_02',
      phase: 3
    },
    33133: {
      name: "Don Julio's Heart",
      spellPower: 14,
      iconName: 'inv_jewelcrafting_crimsonspinel_02',
      phase: 3
    },
    27777: {
      name: 'Stark Blood Garnet',
      spellPower: 8,
      iconName: 'inv_misc_gem_ruby_01',
      phase: 1
    },
    24030: {
      name: 'Runed Living Ruby',
      spellPower: 9,
      iconName: 'inv_jewelcrafting_livingruby_03',
      phase: 2
    },
    23096: {
      name: 'Runed Blood Garnet',
      spellPower: 7,
      iconName: 'inv_misc_gem_bloodgem_02',
      phase: 1
    }
  },
  yellow: {
    35761: {
      name: 'Quick Lionseye',
      hasteRating: 10,
      iconName: 'inv_jewelcrafting_lionseye_02',
      phase: 5
    },
    35315: {
      name: 'Quick Dawnstone',
      hasteRating: 8,
      iconName: 'inv_jewelcrafting_dawnstone_03',
      phase: 5
    },
    33141: {
      name: 'Great Bladestone',
      hitRating: 12,
      iconName: 'inv_jewelcrafting_lionseye_02',
      phase: 3
    },
    33140: {
      name: 'Blood of Amber',
      critRating: 12,
      iconName: 'inv_jewelcrafting_lionseye_02',
      phase: 3
    },
    32210: {
      name: 'Great Lionseye',
      hitRating: 10,
      iconName: 'inv_jewelcrafting_lionseye_02',
      phase: 3
    },
    32207: {
      name: 'Gleaming Lionseye',
      critRating: 10,
      iconName: 'inv_jewelcrafting_lionseye_02',
      phase: 3
    },
    32204: {
      name: 'Brilliant Lionseye',
      intellect: 10,
      iconName: 'inv_jewelcrafting_lionseye_02',
      phase: 3
    },
    31861: {
      name: 'Great Dawnstone',
      hitRating: 8,
      iconName: 'inv_jewelcrafting_dawnstone_03',
      phase: 2
    },
    31860: {
      name: 'Great Golden Draenite',
      hitRating: 6,
      iconName: 'inv_misc_gem_goldendraenite_02',
      phase: 2
    },
    24050: {
      name: 'Gleaming Dawnstone',
      critRating: 8,
      iconName: 'inv_jewelcrafting_dawnstone_03',
      phase: 2
    },
    24047: {
      name: 'Brilliant Dawnstone',
      intellect: 8,
      iconName: 'inv_jewelcrafting_dawnstone_03',
      phase: 2
    },
    23114: {
      name: 'Gleaming Golden Draenite',
      critRating: 6,
      iconName: 'inv_misc_gem_goldendraenite_02',
      phase: 1
    },
    23113: {
      name: 'Brilliant Golden Draenite',
      intellect: 6,
      iconName: 'inv_misc_gem_goldendraenite_02',
      phase: 1
    }
  },
  blue: {
    32203: {
      name: 'Stormy Empyrean Sapphire',
      spellPen: 13,
      iconName: 'inv_jewelcrafting_empyreansapphire_02',
      phase: 3
    },
    24039: {
      name: 'Stormy Star of Elune',
      spellPen: 10,
      iconName: 'inv_jewelcrafting_starofelune_03',
      phase: 2
    },
    23120: {
      name: 'Stormy Azure Moonstone',
      spellPen: 8,
      iconName: 'inv_misc_gem_azuredraenite_02',
      phase: 1
    }
  },
  orange: {
    35760: {
      name: 'Reckless Pyrestone',
      spellPower: 6,
      hasteRating: 5,
      iconName: 'inv_jewelcrafting_pyrestone_02',
      phase: 5
    },
    35316: {
      name: 'Reckless Noble Topaz',
      spellPower: 5,
      hasteRating: 4,
      iconName: 'inv_jewelcrafting_nobletopaz_03',
      phase: 5
    },
    32638: {
      name: 'Unstable Topaz',
      spellPower: 5,
      intellect: 4,
      iconName: 'inv_jewelcrafting_nobletopaz_02',
      phase: 2
    },
    32221: {
      name: 'Veiled Pyrestone',
      spellPower: 6,
      hitRating: 5,
      iconName: 'inv_jewelcrafting_pyrestone_02',
      phase: 3
    },
    32218: {
      name: 'Potent Pyrestone',
      spellPower: 6,
      critRating: 5,
      iconName: 'inv_jewelcrafting_pyrestone_02',
      phase: 3
    },
    31867: {
      name: 'Veiled Noble Topaz',
      spellPower: 5,
      hitRating: 4,
      iconName: 'inv_jewelcrafting_nobletopaz_03',
      phase: 2
    },
    31866: {
      name: 'Veiled Flame Spessarite',
      spellPower: 4,
      hitRating: 3,
      iconName: 'inv_misc_gem_flamespessarite_02',
      phase: 2
    },
    30588: {
      name: 'Potent Fire Opal',
      spellPower: 6,
      critRating: 4,
      iconName: 'inv_jewelcrafting_nobletopaz_03',
      phase: 1
    },
    30564: {
      name: 'Shining Fire Opal',
      spellPower: 6,
      hitRating: 5,
      iconName: 'inv_jewelcrafting_nobletopaz_03',
      phase: 1
    },
    30573: {
      name: 'Mysterious Fire Opal',
      spellPower: 6,
      spellPen: 5,
      iconName: 'inv_jewelcrafting_nobletopaz_03',
      phase: 1
    },
    30551: {
      name: 'Infused Fire Opal',
      spellPower: 6,
      intellect: 4,
      iconName: 'inv_jewelcrafting_nobletopaz_03',
      phase: 1
    },
    28123: {
      name: 'Potent Ornate Topaz',
      spellPower: 6,
      critRating: 5,
      iconName: 'inv_misc_gem_opal_02',
      phase: 1
    },
    24059: {
      name: 'Potent Noble Topaz',
      critRating: 4,
      spellPower: 5,
      iconName: 'inv_jewelcrafting_nobletopaz_03',
      phase: 2
    },
    23101: {
      name: 'Potent Flame Spessarite',
      spellPower: 4,
      critRating: 3,
      iconName: 'inv_misc_gem_flamespessarite_02',
      phase: 1
    }
  },
  green: {
    35759: {
      name: 'Forceful Seaspray Emerald',
      stamina: 7,
      hasteRating: 5,
      iconName: 'inv_jewelcrafting_seasprayemerald_02',
      phase: 5
    },
    35318: {
      name: 'Forceful Talasite',
      stamina: 6,
      hasteRating: 4,
      iconName: 'inv_jewelcrafting_talasite_03',
      phase: 5
    },
    32639: {
      name: 'Unstable Talasite',
      stamina: 4,
      critRating: 4,
      iconName: 'inv_jewelcrafting_talasite_01',
      phase: 2
    },
    32225: {
      name: 'Dazzling Seaspray Emerald',
      intellect: 5,
      mp5: 2,
      iconName: 'inv_jewelcrafting_seasprayemerald_02'
    },
    32224: {
      name: 'Radiant Seaspray Emerald',
      critRating: 5,
      spellPen: 6,
      iconName: 'inv_jewelcrafting_seasprayemerald_02',
      phase: 3
    },
    30608: {
      name: 'Radiant Chrysoprase',
      critRating: 5,
      spellPen: 5,
      iconName: 'inv_jewelcrafting_talasite_03',
      phase: 1
    },
    30606: {
      name: 'Lambent Chrysoprase',
      hitRating: 5,
      mp5: 2,
      iconName: 'inv_jewelcrafting_talasite_03'
    },
    30605: {
      name: 'Vivid Chrysoprase',
      stamina: 6,
      hitRating: 5,
      iconName: 'inv_jewelcrafting_talasite_03',
      phase: 1
    },
    30560: {
      name: 'Rune Covered Chrysoprase',
      critRating: 5,
      mp5: 2,
      iconName: 'inv_jewelcrafting_talasite_03'
    },
    30548: {
      name: 'Polished Chrysoprase',
      stamina: 6,
      critRating: 5,
      iconName: 'inv_jewelcrafting_talasite_03',
      phase: 1
    },
    27785: {
      name: 'Notched Deep Peridot',
      stamina: 3,
      critRating: 4,
      iconName: 'inv_misc_gem_deepperidot_03',
      phase: 1
    },
    24066: {
      name: 'Radiant Talasite',
      critRating: 4,
      spellPen: 5,
      iconName: 'inv_jewelcrafting_talasite_03',
      phase: 2
    },
    23103: {
      name: 'Radiant Deep Peridot',
      critRating: 3,
      spellPen: 4,
      iconName: 'inv_misc_gem_deepperidot_02',
      phase: 1
    }
  },
  purple: {
    32215: {
      name: 'Glowing Shadowsong Amethyst',
      stamina: 7,
      spellPower: 6,
      iconName: 'inv_jewelcrafting_shadowsongamethyst_02',
      phase: 3
    },
    30600: {
      name: 'Fluorescent Tanzanite',
      spellPower: 6,
      spirit: 4,
      iconName: 'inv_jewelcrafting_nightseye_03',
      phase: 1
    },
    30555: {
      name: 'Glowing Tanzanite',
      stamina: 6,
      spellPower: 6,
      iconName: 'inv_jewelcrafting_nightseye_03',
      phase: 1
    },
    24056: {
      name: 'Glowing Nightseye',
      stamina: 6,
      spellPower: 5,
      iconName: 'inv_jewelcrafting_nightseye_03',
      phase: 2
    },
    23108: {
      name: 'Glowing Shadow Draenite',
      stamina: 4,
      spellPen: 4,
      iconName: 'inv_misc_gem_ebondraenite_02',
      phase: 1
    }
  }
}
