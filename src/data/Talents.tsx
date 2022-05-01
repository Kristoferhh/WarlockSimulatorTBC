import { TalentTree, Talent, TalentName } from "../Types";

export interface TalentTreeStruct {
  name: TalentTree;
  rows: Talent[][];
}

export const Talents: TalentTreeStruct[] = [
  {
    name: TalentTree.Affliction,
    rows: [
      [
        {},
        {
          name: "Suppression",
          varName: TalentName.suppression,
          rankIDs: [18174, 18175, 18176, 18177, 18178],
          iconName: "spell_shadow_unsummonbuilding",
        },
        {
          name: "Improved Corruption",
          varName: TalentName.improvedCorruption,
          rankIDs: [17810, 17811, 17812, 17813, 17814],
          iconName: "spell_shadow_abominationexplosion",
        },
      ],
      [
        {
          name: "Improved Curse of Weakness",
          varName: TalentName.improvedCurseOfWeakness,
          rankIDs: [18179, 18180],
          iconName: "spell_shadow_curseofmannoroth",
        },
        {
          name: "Improved Drain Soul",
          varName: TalentName.improvedDrainSoul,
          rankIDs: [18213, 18372],
          iconName: "spell_shadow_haunting",
        },
        {
          name: "Improved Life Tap",
          varName: TalentName.improvedLifeTap,
          rankIDs: [18182, 18183],
          iconName: "spell_shadow_burningspirit",
        },
        {
          name: "Soul Siphon",
          varName: TalentName.soulSiphon,
          rankIDs: [17804, 17805],
          iconName: "spell_shadow_lifedrain02",
        },
      ],
      [
        {
          name: "Improved Curse of Agony",
          varName: TalentName.improvedCurseOfAgony,
          rankIDs: [18827, 18829],
          iconName: "spell_shadow_curseofsargeras",
        },
        {
          name: "Fel Concentration",
          varName: TalentName.felConcentration,
          rankIDs: [17783, 17784, 17785, 17786, 17787],
          iconName: "spell_shadow_fingerofdeath",
        },
        {
          name: "Amplify Curse",
          varName: TalentName.amplifyCurse,
          rankIDs: [18288],
          iconName: "spell_shadow_contagion",
        },
      ],
      [
        {
          name: "Grim Reach",
          varName: TalentName.grimReach,
          rankIDs: [18218, 18219],
          iconName: "spell_shadow_callofbone",
        },
        {
          name: "Nightfall",
          varName: TalentName.nightfall,
          rankIDs: [18094, 18095],
          iconName: "spell_shadow_twilight",
        },
        {},
        {
          name: "Empowered Corruption",
          varName: TalentName.empoweredCorruption,
          rankIDs: [32381, 32382, 32383],
          iconName: "spell_shadow_abominationexplosion",
        },
      ],
      [
        {
          name: "Shadow Embrace",
          varName: TalentName.shadowEmbrace,
          rankIDs: [32385, 32387, 32392, 32393, 32394],
          iconName: "spell_shadow_shadowembrace",
        },
        {
          name: "Siphon Life",
          varName: TalentName.siphonLife,
          rankIDs: [18265],
          iconName: "spell_shadow_requiem",
        },
        {
          name: "Curse of Exhaustion",
          varName: TalentName.curseOfExhaustion,
          rankIDs: [18223],
          iconName: "spell_shadow_grimward",
          requirement: {
            name: "amplifyCurse",
            points: 1,
          },
        },
      ],
      [
        {},
        {
          name: "Shadow Mastery",
          varName: TalentName.shadowMastery,
          rankIDs: [18271, 18272, 18273, 18274, 18275],
          iconName: "spell_shadow_shadetruesight",
          requirement: {
            name: "siphonLife",
            points: 1,
          },
        },
      ],
      [
        {},
        {
          name: "Contagion",
          varName: TalentName.contagion,
          rankIDs: [30060, 30061, 30062, 30063, 30064],
          iconName: "Spell_shadow_painfulafflictions",
        },
        {
          name: "Dark Pact",
          varName: TalentName.darkPact,
          rankIDs: [18220],
          iconName: "spell_shadow_darkritual",
        },
      ],
      [
        {
          name: "Improved Howl of Terror",
          varName: TalentName.improvedHowlOfTerror,
          rankIDs: [30054, 30057],
          iconName: "spell_shadow_deathscream",
        },
        {},
        {
          name: "Malediction",
          varName: TalentName.malediction,
          rankIDs: [32477, 32483, 32484],
          iconName: "spell_shadow_curseofachimonde",
        },
      ],
      [
        {},
        {
          name: "Unstable Affliction",
          varName: TalentName.unstableAffliction,
          rankIDs: [30108],
          iconName: "spell_shadow_unstableaffliction_3",
          requirement: {
            name: "contagion",
            points: 5,
          },
        },
      ],
    ],
  },
  {
    name: TalentTree.Demonology,
    rows: [
      [
        {
          name: "Improved Healthstone",
          varName: TalentName.improvedHealthstone,
          rankIDs: [18692, 18693],
          iconName: "inv_stone_04",
        },
        {
          name: "Improved Imp",
          varName: TalentName.improvedImp,
          rankIDs: [18694, 18695, 18696],
          iconName: "spell_shadow_summonimp",
        },
        {
          name: "Demonic Embrace",
          varName: TalentName.demonicEmbrace,
          rankIDs: [18697, 18698, 18699, 18700, 18701],
          iconName: "spell_shadow_metamorphosis",
        },
      ],
      [
        {
          name: "Improved Health Funnel",
          varName: TalentName.improvedHealthFunnel,
          rankIDs: [18703, 18704],
          iconName: "spell_shadow_lifedrain",
        },
        {
          name: "Improved Voidwalker",
          varName: TalentName.improvedVoidwalker,
          rankIDs: [18705, 18706, 18707],
          iconName: "spell_shadow_summonvoidwalker",
        },
        {
          name: "Fel Intellect",
          varName: TalentName.felIntellect,
          rankIDs: [18731, 18743, 18744],
          iconName: "spell_holy_magicalsentry",
        },
      ],
      [
        {
          name: "Improved Succubus",
          varName: TalentName.improvedSuccubus,
          rankIDs: [18754, 18755, 18756],
          iconName: "spell_shadow_summonsuccubus",
        },
        {
          name: "Fel Domination",
          varName: TalentName.felDomination,
          rankIDs: [18708],
          iconName: "spell_nature_removecurse",
        },
        {
          name: "Fel Stamina",
          varName: TalentName.felStamina,
          rankIDs: [18748, 18749, 18750],
          iconName: "spell_shadow_antishadow",
        },
        {
          name: "Demonic Aegis",
          varName: TalentName.demonicAegis,
          rankIDs: [30143, 30144, 30145],
          iconName: "spell_shadow_ragingscream",
        },
      ],
      [
        {},
        {
          name: "Master Summoner",
          varName: TalentName.masterSummoner,
          rankIDs: [18709, 18710],
          iconName: "spell_shadow_impphaseshift",
          requirement: {
            name: "felDomination",
            points: 1,
          },
        },
        {
          name: "Unholy Power",
          varName: TalentName.unholyPower,
          rankIDs: [18769, 18770, 18771, 18772, 18773],
          iconName: "spell_shadow_shadowworddominate",
        },
      ],
      [
        {
          name: "Improved Enslave Demon",
          varName: TalentName.improvedEnslaveDemon,
          rankIDs: [18821, 18822, 18823, 18824, 18825],
          iconName: "spell_shadow_enslavedemon",
        },
        {
          name: "Demonic Sacrifice",
          varName: TalentName.demonicSacrifice,
          rankIDs: [18788],
          iconName: "spell_shadow_psychicscream",
        },
        {},
        {
          name: "Improved Firestone",
          varName: TalentName.improvedFirestone,
          rankIDs: [18767, 18768],
          iconName: "inv_ammo_firetar",
        },
      ],
      [
        {
          name: "Mana Feed",
          varName: TalentName.manaFeed,
          rankIDs: [30326, 30327, 30328],
          iconName: "spell_shadow_manafeed",
        },
        {},
        {
          name: "Master Demonologist",
          varName: TalentName.masterDemonologist,
          rankIDs: [23785, 23822, 23823, 23824, 23825],
          iconName: "spell_shadow_shadowpact",
          requirement: {
            name: "unholyPower",
            points: 5,
          },
        },
      ],
      [
        {
          name: "Demonic Resilience",
          varName: TalentName.demonicResilience,
          rankIDs: [30319, 30320, 30321],
          iconName: "spell_shadow_demonicfortitude",
        },
        {
          name: "Soul Link",
          varName: TalentName.soulLink,
          rankIDs: [19028],
          iconName: "spell_shadow_gathershadows",
          requirement: {
            name: "demonicSacrifice",
            points: 1,
          },
        },
        {
          name: "Demonic Knowledge",
          varName: TalentName.demonicKnowledge,
          rankIDs: [35691, 35692, 35693],
          iconName: "Spell_shadow_improvedvampiricembrace",
        },
      ],
      [
        {},
        {
          name: "Demonic Tactics",
          varName: TalentName.demonicTactics,
          rankIDs: [30242, 30245, 30246, 30247, 30248],
          iconName: "Spell_shadow_demonictactics",
        },
      ],
      [
        {},
        {
          name: "Summon Felguard",
          varName: TalentName.summonFelguard,
          rankIDs: [30146],
          iconName: "spell_shadow_summonfelguard",
        },
      ],
    ],
  },
  {
    name: TalentTree.Destruction,
    rows: [
      [
        {},
        {
          name: "Improved Shadow Bolt",
          varName: TalentName.improvedShadowBolt,
          rankIDs: [17793, 17796, 17801, 17802, 17803],
          iconName: "spell_shadow_shadowbolt",
        },
        {
          name: "Cataclysm",
          varName: TalentName.cataclysm,
          rankIDs: [17778, 17779, 17780, 17781, 17782],
          iconName: "spell_fire_windsofwoe",
        },
      ],
      [
        {},
        {
          name: "Bane",
          varName: TalentName.bane,
          rankIDs: [17788, 17789, 17790, 17791, 17792],
          iconName: "spell_shadow_deathpact",
        },
        {
          name: "Aftermath",
          varName: TalentName.aftermath,
          rankIDs: [18119, 18120, 18121, 18122, 18123],
          iconName: "spell_fire_fire",
        },
      ],
      [
        {
          name: "Improved Firebolt",
          varName: TalentName.improvedFirebolt,
          rankIDs: [18126, 18127],
          iconName: "spell_fire_firebolt",
        },
        {
          name: "Improved Lash of Pain",
          varName: TalentName.improvedLashOfPain,
          rankIDs: [18128, 18129],
          iconName: "spell_shadow_curse",
        },
        {
          name: "Devastation",
          varName: TalentName.devastation,
          rankIDs: [18130, 18131, 18132, 18133, 18134],
          iconName: "spell_fire_flameshock",
        },
        {
          name: "Shadowburn",
          varName: TalentName.shadowburn,
          rankIDs: [17877],
          iconName: "spell_shadow_scourgebuild",
        },
      ],
      [
        {
          name: "Intensity",
          varName: TalentName.intensity,
          rankIDs: [18135, 18136],
          iconName: "spell_fire_lavaspawn",
        },
        {
          name: "Destructive Reach",
          varName: TalentName.destructiveReach,
          rankIDs: [17917, 17918],
          iconName: "spell_shadow_corpseexplode",
        },
        {},
        {
          name: "Improved Searing Pain",
          varName: TalentName.improvedSearingPain,
          rankIDs: [17927, 17929, 17930],
          iconName: "spell_fire_soulburn",
        },
      ],
      [
        {
          name: "Pyroclasm",
          varName: TalentName.pyroclasm,
          rankIDs: [18096, 18073],
          iconName: "spell_fire_volcano",
          requirement: {
            name: "intensity",
            points: 2,
          },
        },
        {
          name: "Improved Immolate",
          varName: TalentName.improvedImmolate,
          rankIDs: [17815, 17833, 17834, 17835, 17836],
          iconName: "spell_fire_immolation",
        },
        {
          name: "Ruin",
          varName: TalentName.ruin,
          rankIDs: [17959],
          iconName: "spell_shadow_shadowwordpain",
          requirement: {
            name: "devastation",
            points: 5,
          },
        },
      ],
      [
        {
          name: "Nether Protection",
          varName: TalentName.netherProtection,
          rankIDs: [30299, 30301, 30302],
          iconName: "spell_shadow_netherprotection",
        },
        {},
        {
          name: "Emberstorm",
          varName: TalentName.emberstorm,
          rankIDs: [17954, 17955, 17956, 17957, 17958],
          iconName: "spell_fire_selfdestruct",
        },
      ],
      [
        {
          name: "Backlash",
          varName: TalentName.backlash,
          rankIDs: [34935, 34938, 34939],
          iconName: "spell_fire_playingwithfire",
        },
        {
          name: "Conflagrate",
          varName: TalentName.conflagrate,
          rankIDs: [17962],
          iconName: "spell_fire_fireball",
          requirement: {
            name: "improvedImmolate",
            points: 5,
          },
        },
        {
          name: "Soul Leech",
          varName: TalentName.soulLeech,
          rankIDs: [30293, 30295, 30296],
          iconName: "spell_shadow_soulleech_3",
        },
      ],
      [
        {},
        {
          name: "Shadow and Flame",
          varName: TalentName.shadowAndFlame,
          rankIDs: [30288, 30289, 30290, 30291, 30292],
          iconName: "spell_shadow_shadowandflame",
        },
      ],
      [
        {},
        {
          name: "Shadowfury",
          varName: TalentName.shadowfury,
          rankIDs: [30283],
          iconName: "spell_shadow_shadowfury",
          requirement: {
            name: "shadowAndFlame",
            points: 5,
          },
        },
      ],
    ],
  },
];
