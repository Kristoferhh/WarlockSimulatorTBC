import { Gem, GemColor } from '../Types';

export const Gems: Gem[] = [
  // Meta
  { name: 'Ember Skyfire Diamond', id: 35503, color: GemColor.Meta, iconName: 'inv_misc_gem_diamond_07', phase: 1, stats: { spellPower: 14, intellectModifier: 1.02 } },
  { name: 'Chaotic Skyfire Diamond', id: 34220, color: GemColor.Meta, iconName: 'inv_misc_gem_diamond_07', phase: 1, stats: { critRating: 12 } },
  { name: 'Imbued Unstable Diamond', id: 32641, color: GemColor.Meta, iconName: 'inv_misc_gem_diamond_06', phase: 1, stats: { spellPower: 14 } },
  { name: 'Swift Starfire Diamond', id: 28557, color: GemColor.Meta, iconName: 'inv_misc_gem_diamond_07', phase: 1, stats: { spellPower: 12 } },
  { name: 'Insightful Earthstorm Diamond', id: 25901, color: GemColor.Meta, iconName: 'inv_misc_gem_diamond_06', phase: 1, stats: { intellect: 12 } },
  { name: 'Mystical Skyfire Diamond', id: 25893, color: GemColor.Meta, iconName: 'inv_misc_gem_diamond_07', phase: 1 },

  // Red
  { name: 'Runed Crimson Spinel', id: 35488, color: GemColor.Red, iconName: 'inv_jewelcrafting_crimsonspinel_02', phase: 3, stats: { spellPower: 12 } },
  { name: "Don Julio's Heart", id: 33133, color: GemColor.Red, iconName: 'inv_jewelcrafting_crimsonspinel_02', phase: 3, stats: { spellPower: 14 } },
  { name: 'Runed Ornate Ruby', id: 28118, color: GemColor.Red, iconName: 'inv_misc_gem_bloodstone_01', phase: 1, stats: { spellPower: 12 } },
  { name: 'Stark Blood Garnet', id: 27777, color: GemColor.Red, iconName: 'inv_misc_gem_ruby_01', phase: 1, stats: { spellPower: 8 } },
  { name: 'Runed Living Ruby', id: 24030, color: GemColor.Red, iconName: 'inv_jewelcrafting_livingruby_03', phase: 2, stats: { spellPower: 9 } },
  { name: 'Runed Blood Garnet', id: 23096, color: GemColor.Red, iconName: 'inv_misc_gem_bloodgem_02', phase: 1, stats: { spellPower: 7 } },

  // Orange
  { name: 'Reckless Pyrestone', id: 35760, color: GemColor.Orange, iconName: 'inv_jewelcrafting_pyrestone_02', phase: 5, stats: { spellPower: 6, hasteRating: 5 } },
  { name: 'Reckless Noble Topaz', id: 35316, color: GemColor.Orange, iconName: 'inv_jewelcrafting_nobletopaz_03', phase: 5, stats: { spellPower: 5, hasteRating: 4 } },
  { name: 'Unstable Topaz', id: 32638, color: GemColor.Orange, iconName: 'inv_jewelcrafting_nobletopaz_02', phase: 2, stats: { spellPower: 5, intellect: 4 } },
  { name: 'Veiled Pyrestone', id: 32221, color: GemColor.Orange, iconName: 'inv_jewelcrafting_pyrestone_02', phase: 3, stats: { spellPower: 6, hitRating: 5 } },
  { name: 'Potent Pyrestone', id: 32218, color: GemColor.Orange, iconName: 'inv_jewelcrafting_pyrestone_02', phase: 3, stats: { spellPower: 6, critRating: 5 } },
  { name: 'Veiled Noble Topaz', id: 31867, color: GemColor.Orange, iconName: 'inv_jewelcrafting_nobletopaz_03', phase: 2, stats: { spellPower: 5, hitRating: 4 } },
  { name: 'Veiled Flame Spessarite', id: 31866, color: GemColor.Orange, iconName: 'inv_misc_gem_flamespessarite_02', phase: 2, stats: { spellPower: 4, hitRating: 3 } },
  { name: 'Potent Fire Opal', id: 30588, color: GemColor.Orange, iconName: 'inv_jewelcrafting_nobletopaz_03', phase: 1, stats: { spellPower: 6, critRating: 4 } },
  { name: 'Shining Fire Opal', id: 30564, color: GemColor.Orange, iconName: 'inv_jewelcrafting_nobletopaz_03', phase: 1, stats: { spellPower: 6, hitRating: 5 } },
  { name: 'Mysterious Fire Opal', id: 30573, color: GemColor.Orange, iconName: 'inv_jewelcrafting_nobletopaz_03', phase: 1, stats: { spellPower: 6, spellPenetration: 5 } },
  { name: 'Infused Fire Opal', id: 30551, color: GemColor.Orange, iconName: 'inv_jewelcrafting_nobletopaz_03', phase: 1, stats: { spellPower: 6, intellect: 4 } },
  { name: 'Potent Ornate Topaz', id: 28123, color: GemColor.Orange, iconName: 'inv_misc_gem_opal_02', phase: 1, stats: { spellPower: 6, critRating: 5 } },
  { name: 'Potent Noble Topaz', id: 24059, color: GemColor.Orange, iconName: 'inv_jewelcrafting_nobletopaz_03', phase: 2, stats: { critRating: 4, spellPower: 5 } },
  { name: 'Potent Flame Spessarite', id: 23101, color: GemColor.Orange, iconName: 'inv_misc_gem_flamespessarite_02', phase: 1, stats: { spellPower: 4, critRating: 3 } },

  // Yellow
  { name: 'Quick Lionseye', id: 35761, color: GemColor.Yellow, iconName: 'inv_jewelcrafting_lionseye_02', phase: 5, stats: { hasteRating: 10 } },
  { name: 'Quick Dawnstone', id: 35315, color: GemColor.Yellow, iconName: 'inv_jewelcrafting_dawnstone_03', phase: 5, stats: { hasteRating: 8 } },
  { name: 'Great Bladestone', id: 33141, color: GemColor.Yellow, iconName: 'inv_jewelcrafting_lionseye_02', phase: 3, stats: { hitRating: 12 } },
  { name: 'Blood of Amber', id: 33140, color: GemColor.Yellow, iconName: 'inv_jewelcrafting_lionseye_02', phase: 3, stats: { critRating: 12 } },
  { name: 'Great Lionseye', id: 32210, color: GemColor.Yellow, iconName: 'inv_jewelcrafting_lionseye_02', phase: 3, stats: { hitRating: 10 } },
  { name: 'Gleaming Lionseye', id: 32207, color: GemColor.Yellow, iconName: 'inv_jewelcrafting_lionseye_02', phase: 3, stats: { critRating: 10 } },
  { name: 'Brilliant Lionseye', id: 32204, color: GemColor.Yellow, iconName: 'inv_jewelcrafting_lionseye_02', phase: 3, stats: { intellect: 10 } },
  { name: 'Great Dawnstone', id: 31861, color: GemColor.Yellow, iconName: 'inv_jewelcrafting_dawnstone_03', phase: 2, stats: { hitRating: 8 } },
  { name: 'Great Golden Draenite', id: 31860, color: GemColor.Yellow, iconName: 'inv_misc_gem_goldendraenite_02', phase: 2, stats: { hitRating: 6 } },
  { name: 'Gleaming Dawnstone', id: 24050, color: GemColor.Yellow, iconName: 'inv_jewelcrafting_dawnstone_03', phase: 2, stats: { critRating: 8 } },
  { name: 'Brilliant Dawnstone', id: 24047, color: GemColor.Yellow, iconName: 'inv_jewelcrafting_dawnstone_03', phase: 2, stats: { intellect: 8 } },
  { name: 'Gleaming Golden Draenite', id: 23114, color: GemColor.Yellow, iconName: 'inv_misc_gem_goldendraenite_02', phase: 1, stats: { critRating: 6 } },
  { name: 'Brilliant Golden Draenite', id: 23113, color: GemColor.Yellow, iconName: 'inv_misc_gem_goldendraenite_02', phase: 1, stats: { intellect: 6 } },

  // Purple
  { name: 'Glowing Shadowsong Amethyst', id: 32215, color: GemColor.Purple, iconName: 'inv_jewelcrafting_shadowsongamethyst_02', phase: 3, stats: { stamina: 7, spellPower: 6 } },
  { name: 'Infused Amethyst', id: 31116, color: GemColor.Purple, iconName: 'inv_jewelcrafting_nightseye_03', phase: 1, stats: { stamina: 6, spellPower: 6 } },
  { name: 'Fluorescent Tanzanite', id: 30600, color: GemColor.Purple, iconName: 'inv_jewelcrafting_nightseye_03', phase: 1, stats: { spellPower: 6, spirit: 4 } },
  { name: 'Glowing Tanzanite', id: 30555, color: GemColor.Purple, iconName: 'inv_jewelcrafting_nightseye_03', phase: 1, stats: { stamina: 6, spellPower: 6 } },
  { name: 'Glowing Nightseye', id: 24056, color: GemColor.Purple, iconName: 'inv_jewelcrafting_nightseye_03', phase: 2, stats: { stamina: 6, spellPower: 5 } },
  { name: 'Glowing Shadow Draenite', id: 23108, color: GemColor.Purple, iconName: 'inv_misc_gem_ebondraenite_02', phase: 1, stats: { stamina: 4, spellPower: 4 } },

  // Blue
  { name: 'Stormy Empyrean Sapphire', id: 32203, color: GemColor.Blue, iconName: 'inv_jewelcrafting_empyreansapphire_02', phase: 3, stats: { spellPenetration: 13 } },
  { name: 'Stormy Star of Elune', id: 24039, color: GemColor.Blue, iconName: 'inv_jewelcrafting_starofelune_03', phase: 2, stats: { spellPenetration: 10 } },
  { name: 'Stormy Azure Moonstone', id: 23120, color: GemColor.Blue, iconName: 'inv_misc_gem_azuredraenite_02', phase: 1, stats: { spellPenetration: 8 } },

  // Green
  { name: 'Forceful Seaspray Emerald', id: 35759, color: GemColor.Green, iconName: 'inv_jewelcrafting_seasprayemerald_02', phase: 5, stats: { stamina: 7, hasteRating: 5 } },
  { name: 'Forceful Talasite', id: 35318, color: GemColor.Green, iconName: 'inv_jewelcrafting_talasite_03', phase: 5, stats: { stamina: 6, hasteRating: 4 } },
  { name: 'Unstable Talasite', id: 32639, color: GemColor.Green, iconName: 'inv_jewelcrafting_talasite_01', phase: 2, stats: { stamina: 4, critRating: 4 } },
  { name: 'Dazzling Seaspray Emerald', id: 32225, color: GemColor.Green, iconName: 'inv_jewelcrafting_seasprayemerald_02', phase: 3, stats: { intellect: 5, mp5: 2 } },
  { name: 'Radiant Seaspray Emerald', id: 32224, color: GemColor.Green, iconName: 'inv_jewelcrafting_seasprayemerald_02', phase: 3, stats: { critRating: 5, spellPenetration: 6 } },
  { name: 'Radiant Chrysoprase', id: 30608, color: GemColor.Green, iconName: 'inv_jewelcrafting_talasite_03', phase: 1, stats: { critRating: 5, spellPenetration: 5 } },
  { name: 'Lambent Chrysoprase', id: 30606, color: GemColor.Green, iconName: 'inv_jewelcrafting_talasite_03', phase: 1, stats: { hitRating: 5, mp5: 2 } },
  { name: 'Vivid Chrysoprase', id: 30605, color: GemColor.Green, iconName: 'inv_jewelcrafting_talasite_03', phase: 1, stats: { stamina: 6, hitRating: 5 } },
  { name: 'Rune Covered Chrysoprase', id: 30560, color: GemColor.Green, iconName: 'inv_jewelcrafting_talasite_03', phase: 1, stats: { critRating: 5, mp5: 2 } },
  { name: 'Polished Chrysoprase', id: 30548, color: GemColor.Green, iconName: 'inv_jewelcrafting_talasite_03', phase: 1, stats: { stamina: 6, critRating: 5 } },
  { name: 'Notched Deep Peridot', id: 27785, color: GemColor.Green, iconName: 'inv_misc_gem_deepperidot_03', phase: 1, stats: { stamina: 3, critRating: 4 } },
  { name: 'Radiant Talasite', id: 24066, color: GemColor.Green, iconName: 'inv_jewelcrafting_talasite_03', phase: 2, stats: { critRating: 4, spellPenetration: 5 } },
  { name: 'Radiant Deep Peridot', id: 23103, color: GemColor.Green, iconName: 'inv_misc_gem_deepperidot_02', phase: 1, stats: { critRating: 3, spellPenetration: 4 } },
]