import { Gem, GemColor } from '../Types';

export const Gems: Gem[] = [
  // Meta
  { phase: 1, id: 35503, color: GemColor.Meta, name: 'Ember Skyfire Diamond', iconName: 'inv_misc_gem_diamond_07', stats: { spellPower: 14, intellectModifier: 1.02 } },
  { phase: 1, id: 34220, color: GemColor.Meta, name: 'Chaotic Skyfire Diamond', iconName: 'inv_misc_gem_diamond_07', stats: { critRating: 12 } },
  { phase: 1, id: 32641, color: GemColor.Meta, name: 'Imbued Unstable Diamond', iconName: 'inv_misc_gem_diamond_06', stats: { spellPower: 14 } },
  { phase: 1, id: 28557, color: GemColor.Meta, name: 'Swift Starfire Diamond', iconName: 'inv_misc_gem_diamond_07', stats: { spellPower: 12 } },
  { phase: 1, id: 25901, color: GemColor.Meta, name: 'Insightful Earthstorm Diamond', iconName: 'inv_misc_gem_diamond_06', stats: { intellect: 12 } },
  { phase: 1, id: 25893, color: GemColor.Meta, name: 'Mystical Skyfire Diamond', iconName: 'inv_misc_gem_diamond_07' },

  // Red
  { phase: 3, id: 35488, color: GemColor.Red, name: 'Runed Crimson Spinel', iconName: 'inv_jewelcrafting_crimsonspinel_02', stats: { spellPower: 12 } },
  { phase: 1, id: 33133, color: GemColor.Red, name: "Don Julio's Heart", iconName: 'inv_jewelcrafting_crimsonspinel_02', stats: { spellPower: 14 } },
  { phase: 1, id: 28118, color: GemColor.Red, name: 'Runed Ornate Ruby', iconName: 'inv_misc_gem_bloodstone_01', stats: { spellPower: 12 } },
  { phase: 1, id: 27777, color: GemColor.Red, name: 'Stark Blood Garnet', iconName: 'inv_misc_gem_ruby_01', stats: { spellPower: 8 } },
  { phase: 1, id: 24030, color: GemColor.Red, name: 'Runed Living Ruby', iconName: 'inv_jewelcrafting_livingruby_03', stats: { spellPower: 9 } },
  { phase: 1, id: 23096, color: GemColor.Red, name: 'Runed Blood Garnet', iconName: 'inv_misc_gem_bloodgem_02', stats: { spellPower: 7 } },

  // Orange
  { phase: 5, id: 35316, color: GemColor.Orange, name: 'Reckless Noble Topaz', iconName: 'inv_jewelcrafting_nobletopaz_03', stats: { spellPower: 5, hasteRating: 4 } },
  { phase: 3, id: 35760, color: GemColor.Orange, name: 'Reckless Pyrestone', iconName: 'inv_jewelcrafting_pyrestone_02', stats: { spellPower: 6, hasteRating: 5 } },
  { phase: 2, id: 32638, color: GemColor.Orange, name: 'Unstable Topaz', iconName: 'inv_jewelcrafting_nobletopaz_02', stats: { spellPower: 5, intellect: 4 } },
  { phase: 3, id: 32221, color: GemColor.Orange, name: 'Veiled Pyrestone', iconName: 'inv_jewelcrafting_pyrestone_02', stats: { spellPower: 6, hitRating: 5 } },
  { phase: 3, id: 32218, color: GemColor.Orange, name: 'Potent Pyrestone', iconName: 'inv_jewelcrafting_pyrestone_02', stats: { spellPower: 6, critRating: 5 } },
  { phase: 1, id: 31867, color: GemColor.Orange, name: 'Veiled Noble Topaz', iconName: 'inv_jewelcrafting_nobletopaz_03', stats: { spellPower: 5, hitRating: 4 } },
  { phase: 1, id: 31866, color: GemColor.Orange, name: 'Veiled Flame Spessarite', iconName: 'inv_misc_gem_flamespessarite_02', stats: { spellPower: 4, hitRating: 3 } },
  { phase: 1, id: 30588, color: GemColor.Orange, name: 'Potent Fire Opal', iconName: 'inv_jewelcrafting_nobletopaz_03', stats: { spellPower: 6, critRating: 4 } },
  { phase: 1, id: 30573, color: GemColor.Orange, name: 'Mysterious Fire Opal', iconName: 'inv_jewelcrafting_nobletopaz_03', stats: { spellPower: 6, spellPenetration: 5 } },
  { phase: 1, id: 30564, color: GemColor.Orange, name: 'Shining Fire Opal', iconName: 'inv_jewelcrafting_nobletopaz_03', stats: { spellPower: 6, hitRating: 5 } },
  { phase: 1, id: 30551, color: GemColor.Orange, name: 'Infused Fire Opal', iconName: 'inv_jewelcrafting_nobletopaz_03', stats: { spellPower: 6, intellect: 4 } },
  { phase: 1, id: 28123, color: GemColor.Orange, name: 'Potent Ornate Topaz', iconName: 'inv_misc_gem_opal_02', stats: { spellPower: 6, critRating: 5 } },
  { phase: 1, id: 24059, color: GemColor.Orange, name: 'Potent Noble Topaz', iconName: 'inv_jewelcrafting_nobletopaz_03', stats: { critRating: 4, spellPower: 5 } },
  { phase: 1, id: 23101, color: GemColor.Orange, name: 'Potent Flame Spessarite', iconName: 'inv_misc_gem_flamespessarite_02', stats: { spellPower: 4, critRating: 3 } },

  // Yellow
  { phase: 5, id: 35761, color: GemColor.Yellow, name: 'Quick Lionseye', iconName: 'inv_jewelcrafting_lionseye_02', stats: { hasteRating: 10 } },
  { phase: 5, id: 35315, color: GemColor.Yellow, name: 'Quick Dawnstone', iconName: 'inv_jewelcrafting_dawnstone_03', stats: { hasteRating: 8 } },
  { phase: 1, id: 33141, color: GemColor.Yellow, name: 'Great Bladestone', iconName: 'inv_jewelcrafting_lionseye_02', stats: { hitRating: 12 } },
  { phase: 1, id: 33140, color: GemColor.Yellow, name: 'Blood of Amber', iconName: 'inv_jewelcrafting_lionseye_02', stats: { critRating: 12 } },
  { phase: 1, id: 32210, color: GemColor.Yellow, name: 'Great Lionseye', iconName: 'inv_jewelcrafting_lionseye_02', stats: { hitRating: 10 } },
  { phase: 1, id: 32207, color: GemColor.Yellow, name: 'Gleaming Lionseye', iconName: 'inv_jewelcrafting_lionseye_02', stats: { critRating: 10 } },
  { phase: 1, id: 32204, color: GemColor.Yellow, name: 'Brilliant Lionseye', iconName: 'inv_jewelcrafting_lionseye_02', stats: { intellect: 10 } },
  { phase: 1, id: 31861, color: GemColor.Yellow, name: 'Great Dawnstone', iconName: 'inv_jewelcrafting_dawnstone_03', stats: { hitRating: 8 } },
  { phase: 1, id: 31860, color: GemColor.Yellow, name: 'Great Golden Draenite', iconName: 'inv_misc_gem_goldendraenite_02', stats: { hitRating: 6 } },
  { phase: 1, id: 28120, color: GemColor.Yellow, name: 'Gleaming Ornate Dawnstone', iconName: 'inv_misc_gem_topaz_02', stats: { critRating: 10 } },
  { phase: 1, id: 24050, color: GemColor.Yellow, name: 'Gleaming Dawnstone', iconName: 'inv_jewelcrafting_dawnstone_03', stats: { critRating: 8 } },
  { phase: 1, id: 24047, color: GemColor.Yellow, name: 'Brilliant Dawnstone', iconName: 'inv_jewelcrafting_dawnstone_03', stats: { intellect: 8 } },
  { phase: 1, id: 23114, color: GemColor.Yellow, name: 'Gleaming Golden Draenite', iconName: 'inv_misc_gem_goldendraenite_02', stats: { critRating: 6 } },
  { phase: 1, id: 23113, color: GemColor.Yellow, name: 'Brilliant Golden Draenite', iconName: 'inv_misc_gem_goldendraenite_02', stats: { intellect: 6 } },

  // Purple
  { phase: 3, id: 32215, color: GemColor.Purple, name: 'Glowing Shadowsong Amethyst', iconName: 'inv_jewelcrafting_shadowsongamethyst_02', stats: { stamina: 7, spellPower: 6 } },
  { phase: 1, id: 31116, color: GemColor.Purple, name: 'Infused Amethyst', iconName: 'inv_jewelcrafting_nightseye_03', stats: { stamina: 6, spellPower: 6 } },
  { phase: 1, id: 30600, color: GemColor.Purple, name: 'Fluorescent Tanzanite', iconName: 'inv_jewelcrafting_nightseye_03', stats: { spellPower: 6, spirit: 4 } },
  { phase: 1, id: 30555, color: GemColor.Purple, name: 'Glowing Tanzanite', iconName: 'inv_jewelcrafting_nightseye_03', stats: { stamina: 6, spellPower: 6 } },
  { phase: 1, id: 24056, color: GemColor.Purple, name: 'Glowing Nightseye', iconName: 'inv_jewelcrafting_nightseye_03', stats: { stamina: 6, spellPower: 5 } },
  { phase: 1, id: 23108, color: GemColor.Purple, name: 'Glowing Shadow Draenite', iconName: 'inv_misc_gem_ebondraenite_02', stats: { stamina: 4, spellPower: 4 } },

  // Green
  { phase: 5, id: 35318, color: GemColor.Green, name: 'Forceful Talasite', iconName: 'inv_jewelcrafting_talasite_03', stats: { stamina: 6, hasteRating: 4 } },
  { phase: 3, id: 35759, color: GemColor.Green, name: 'Forceful Seaspray Emerald', iconName: 'inv_jewelcrafting_seasprayemerald_02', stats: { stamina: 7, hasteRating: 5 } },
  { phase: 2, id: 32639, color: GemColor.Green, name: 'Unstable Talasite', iconName: 'inv_jewelcrafting_talasite_01', stats: { stamina: 4, critRating: 4 } },
  { phase: 3, id: 32225, color: GemColor.Green, name: 'Dazzling Seaspray Emerald', iconName: 'inv_jewelcrafting_seasprayemerald_02', stats: { intellect: 5, mp5: 2 } },
  { phase: 3, id: 32224, color: GemColor.Green, name: 'Radiant Seaspray Emerald', iconName: 'inv_jewelcrafting_seasprayemerald_02', stats: { critRating: 5, spellPenetration: 6 } },
  { phase: 1, id: 30608, color: GemColor.Green, name: 'Radiant Chrysoprase', iconName: 'inv_jewelcrafting_talasite_03', stats: { critRating: 5, spellPenetration: 5 } },
  { phase: 1, id: 30606, color: GemColor.Green, name: 'Lambent Chrysoprase', iconName: 'inv_jewelcrafting_talasite_03', stats: { hitRating: 5, mp5: 2 } },
  { phase: 1, id: 30605, color: GemColor.Green, name: 'Vivid Chrysoprase', iconName: 'inv_jewelcrafting_talasite_03', stats: { stamina: 6, hitRating: 5 } },
  { phase: 1, id: 30560, color: GemColor.Green, name: 'Rune Covered Chrysoprase', iconName: 'inv_jewelcrafting_talasite_03', stats: { critRating: 5, mp5: 2 } },
  { phase: 1, id: 30548, color: GemColor.Green, name: 'Polished Chrysoprase', iconName: 'inv_jewelcrafting_talasite_03', stats: { stamina: 6, critRating: 5 } },
  { phase: 1, id: 27785, color: GemColor.Green, name: 'Notched Deep Peridot', iconName: 'inv_misc_gem_deepperidot_03', stats: { stamina: 3, critRating: 4 } },
  { phase: 1, id: 24066, color: GemColor.Green, name: 'Radiant Talasite', iconName: 'inv_jewelcrafting_talasite_03', stats: { critRating: 4, spellPenetration: 5 } },
  { phase: 1, id: 23103, color: GemColor.Green, name: 'Radiant Deep Peridot', iconName: 'inv_misc_gem_deepperidot_02', stats: { critRating: 3, spellPenetration: 4 } },

  // Blue
  { phase: 3, id: 32203, color: GemColor.Blue, name: 'Stormy Empyrean Sapphire', iconName: 'inv_jewelcrafting_empyreansapphire_02', stats: { spellPenetration: 13 } },
  { phase: 1, id: 24039, color: GemColor.Blue, name: 'Stormy Star of Elune', iconName: 'inv_jewelcrafting_starofelune_03', stats: { spellPenetration: 10 } },
  { phase: 1, id: 23120, color: GemColor.Blue, name: 'Stormy Azure Moonstone', iconName: 'inv_misc_gem_azuredraenite_02', stats: { spellPenetration: 8 } },
]