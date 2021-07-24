using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes.UI
{
	public class TalentRequirement
	{
		public string name = null;
		public int pointsRequired = 0;
	}

	public class Talent
	{
		public int[] rankIds = null;
		public int column = 0;
		public string name = null;
		public string iconName = null;
		public TalentRequirement requirement = null;
	}

	public static class TalentTree
	{
		public static readonly Dictionary<string, Dictionary<int, Dictionary<string, Talent>>> trees = new Dictionary<string, Dictionary<int, Dictionary<string, Talent>>>
		{
			{
				"affliction", new Dictionary<int, Dictionary<string, Talent>>
				{
					{
						1, new Dictionary<string, Talent>
						{
							{
								"suppression", new Talent
								{
									name = "Suppression",
									rankIds = new int[] { 18174, 18175, 18176, 18177, 18178 },
									iconName = "spell_shadow_unsummonbuilding.jpg",
									column = 2
								}
							},
							{
								"improvedCorruption", new Talent
								{
									name = "Improved Corruption",
									rankIds = new int[] { 17810, 17811, 17812, 17813, 17814 },
									iconName = "spell_shadow_abominationexplosion.jpg",
									column = 3
								}
							}
						}
					},
					{
						2, new Dictionary<string, Talent>
						{
							{
								"improvedCurseOfWeakness", new Talent
								{
									name = "Improved Curse of Weakness",
									rankIds = new int[] { 18179, 18180 },
									iconName = "spell_shadow_curseofmannoroth.jpg",
									column = 1
								}
							},
							{
								"improvedDrainSoul", new Talent
								{
									name = "Improved Drain Soul",
									rankIds = new int[] { 18213, 18372 },
									iconName = "spell_shadow_haunting.jpg",
									column = 2
								}
							},
							{
								"improvedLifeTap", new Talent
								{
									name = "Improved Life Tap",
									rankIds = new int[] { 18182, 18183 },
									iconName = "spell_shadow_burningspirit.jpg",
									column = 3
								}
							},
							{
								"soulSiphon", new Talent
								{
									name = "Soul Siphon",
									rankIds = new int[] { 17804, 17805 },
									iconName = "spell_shadow_lifedrain02.jpg",
									column = 4
								}
							}
						}
					},
					{
						3, new Dictionary<string, Talent>
						{
							{
								"improvedCurseOfAgony", new Talent
								{
									name = "Improved Curse of Agony",
									rankIds = new int[] { 18827, 18829 },
									iconName = "spell_shadow_curseofsargeras.jpg",
									column = 1
								}
							},
							{
								"felConcentration", new Talent
								{
									name = "Fel Concentration",
									rankIds = new int[] { 17783, 17784, 17785, 17786, 17787 },
									iconName = "spell_shadow_fingerofdeath.jpg",
									column = 2
								}
							},
							{
								"amplifyCurse", new Talent
								{
									name = "Amplify Curse",
									rankIds = new int[] { 18288 },
									iconName = "spell_shadow_contagion.jpg",
									column = 3
								}
							}
						}
					},
					{
						4, new Dictionary<string, Talent>
						{
							{
								"grimReach", new Talent
								{
									name = "Grim Reach",
									rankIds = new int[] { 18218, 18219 },
									iconName = "spell_shadow_callofbone.jpg",
									column = 1
								}
							},
							{
								"nightfall", new Talent
								{
									name = "Nightfall",
									rankIds = new int[] { 18094, 18095 },
									iconName = "spell_shadow_twilight.jpg",
									column = 2
								}
							},
							{
								"empoweredCorruption", new Talent
								{
									name = "Empowered Corruption",
									rankIds = new int[] { 32381, 32382, 32383 },
									iconName = "spell_shadow_abominationexplosion.jpg",
									column = 4
								}
							},
						}
					},
					{
						5, new Dictionary<string, Talent>
						{
							{
								"shadowEmbrace", new Talent
								{
									name = "Shadow Embrace",
									rankIds = new int[] { 32385, 32387, 32392, 32393, 32394 },
									iconName = "spell_shadow_shadowembrace.jpg",
									column = 1
								}
							},
							{
								"siphonLife", new Talent
								{
									name = "Siphon Life",
									rankIds = new int[] { 18265 },
									iconName = "spell_shadow_requiem.jpg",
									column = 2
								}
							},
							{
								"curseOfExhaustion", new Talent
								{
									name = "Curse of Exhaustion",
									rankIds = new int[] { 18223 },
									iconName = "spell_shadow_grimward.jpg",
									column = 3,
									requirement = new TalentRequirement
									{
										name = "amplifyCurse",
										pointsRequired = 1
									}
								}
							}
						}
					},
					{
						6, new Dictionary<string, Talent>
						{
							{
								"shadowMastery", new Talent
								{
									name = "Shadow Mastery",
									rankIds = new int[] { 18271, 18272, 18273, 18274, 18275 },
									iconName = "spell_shadow_shadetruesight.jpg",
									column = 2,
									requirement = new TalentRequirement
									{
										name = "siphonLife",
										pointsRequired = 1
									}
								}
							}
						}
					},
					{
						7, new Dictionary<string, Talent>
						{
							{
								"contagion", new Talent
								{
									name = "Contagion",
									rankIds = new int[] { 30060, 30061, 30062, 30063, 30064 },
									iconName = "Spell_shadow_painfulafflictions.jpg",
									column = 2
								}
							},
							{
								"darkPact", new Talent
								{
									name = "Dark Pact",
									rankIds = new int[] { 18220 },
									iconName = "spell_shadow_darkritual.jpg",
									column = 3
								}
							}
						}
					},
					{
						8, new Dictionary<string, Talent>
						{
							{
								"improvedHowlOfTerror", new Talent
								{
									name = "Improved Howl of Terror",
									rankIds = new int[] { 30054, 30057 },
									iconName = "spell_shadow_deathscream.jpg",
									column = 1
								}
							},
							{
								"malediction", new Talent
								{
									name = "Malediction",
									rankIds = new int[] { 32477, 32483, 32484 },
									iconName = "spell_shadow_curseofachimonde.jpg",
									column = 3
								}
							}
						}
					},
					{
						9, new Dictionary<string, Talent>
						{
							{
								"unstableAffliction", new Talent
								{
									name = "Unstable Affliction",
									rankIds = new int[] { 30108 },
									iconName = "spell_shadow_unstableaffliction_3.jpg",
									column = 2,
									requirement = new TalentRequirement
									{
										name = "contagion",
										pointsRequired = 5
									}
								}
							}
						}
					}
				}
			},
			{
				"demonology", new Dictionary<int, Dictionary<string, Talent>>
				{
					{
						1, new Dictionary<string, Talent>
						{
							{
								"improvedHealthstone", new Talent
								{
									name = "Improved Healthstone",
									rankIds = new int[] { 18692, 18693 },
									iconName = "inv_stone_04.jpg",
									column = 1
								}
							},
							{
								"improvedImp", new Talent
								{
									name = "Improved Imp",
									rankIds = new int[] { 18694, 18695, 18696 },
									iconName = "spell_shadow_summonimp.jpg",
									column = 2
								}
							},
							{
								"demonicEmbrace", new Talent
								{
									name = "Demonic Embrace",
									rankIds = new int[] { 18697, 18698, 18699, 18700, 18701 },
									iconName = "spell_shadow_metamorphosis.jpg",
									column = 3
								}
							}
						}
					},
					{
						2, new Dictionary<string, Talent>
						{
							{
								"improvedHealthFunnel", new Talent
								{
									name = "Improved Health Funnel",
									rankIds = new int[] { 18703, 18704 },
									iconName = "spell_shadow_lifedrain.jpg",
									column = 1
								}
							},
							{
								"improvedVoidwalker", new Talent
								{
									name = "Improved Voidwalker",
									rankIds = new int[] { 18705, 18706, 18707 },
									iconName = "spell_shadow_summonvoidwalker.jpg",
									column = 2
								}
							},
							{
								"felIntellect", new Talent
								{
									name = "Fel Intellect",
									rankIds = new int[] { 18731, 18743, 18744 },
									iconName = "spell_holy_magicalsentry.jpg",
									column = 3
								}
							}
						}
					},
					{
						3, new Dictionary<string, Talent>
						{
							{
								"improvedSuccubus", new Talent
								{
									name = "Improved Succubus",
									rankIds = new int[] { 18754, 18755, 18756 },
									iconName = "spell_shadow_summonsuccubus.jpg",
									column = 1
								}
							},
							{
								"felDomination", new Talent
								{
									name = "Fel Domination",
									rankIds = new int[] { 18708 },
									iconName = "spell_nature_removecurse.jpg",
									column = 2
								}
							},
							{
								"felStamina", new Talent
								{
									name = "Fel Stamina",
									rankIds = new int[] { 18748, 18749, 18750 },
									iconName = "spell_shadow_antishadow.jpg",
									column = 3
								}
							},
							{
								"demonicAegis", new Talent
								{
									name = "Demonic Aegis",
									rankIds = new int[] { 30143, 30144, 30145 },
									iconName = "spell_shadow_ragingscream.jpg",
									column = 4
								}
							}
						}
					},
					{
						4, new Dictionary<string, Talent>
						{
							{
								"masterSummoner", new Talent
								{
									name = "Master Summoner",
									rankIds = new int[] { 18709, 18710 },
									iconName = "spell_shadow_impphaseshift.jpg",
									column = 2,
									requirement = new TalentRequirement
									{
										name = "felDomination",
										pointsRequired = 1
									}
								}
							},
							{
								"unholyPower", new Talent
								{
									name = "Unholy Power",
									rankIds = new int[] { 18769, 18770, 18771, 18772, 18773 },
									iconName = "spell_shadow_shadowworddominate.jpg",
									column = 3
								}
							}
						}
					},
					{
						5, new Dictionary<string, Talent>
						{
							{
								"improvedEnslaveDemon", new Talent
								{
									name = "Improved Enslave Demon",
									rankIds = new int[] { 18821, 18822, 18823, 18824, 18825 },
									iconName = "spell_shadow_enslavedemon.jpg",
									column = 1
								}
							},
							{
								"demonicSacrifice", new Talent
								{
									name = "Demonic Sacrifice",
									rankIds = new int[] { 18788 },
									iconName = "spell_shadow_psychicscream.jpg",
									column = 2
								}
							},
							{
								"improvedFirestone", new Talent
								{
									name = "Improved Firestone",
									rankIds = new int[] { 18767, 18768 },
									iconName = "inv_ammo_firetar.jpg",
									column = 4
								}
							}
						}
					},
					{
						6, new Dictionary<string, Talent>
						{
							{
								"manaFeed", new Talent
								{
									name = "Mana Feed",
									rankIds = new int[] { 30326, 30327, 30328 },
									iconName = "spell_shadow_manafeed.jpg",
									column = 1
								}
							},
							{
								"masterDemonologist", new Talent
								{
									name = "Master Demonologist",
									rankIds = new int[] { 23785, 23822, 23823, 23824, 23825 },
									iconName = "spell_shadow_shadowpact.jpg",
									column = 3,
									requirement = new TalentRequirement
									{
										name = "unholyPower",
										pointsRequired = 5
									}
								}
							}
						}
					},
					{
						7, new Dictionary<string, Talent>
						{
							{
								"demonicResilience", new Talent
								{
									name = "Demonic Resilience",
									rankIds = new int[] { 30319, 30320, 30321 },
									iconName = "spell_shadow_demonicfortitude.jpg",
									column = 1
								}
							},
							{
								"soulLink", new Talent
								{
									name = "Soul Link",
									rankIds = new int[] { 19028 },
									iconName = "spell_shadow_gathershadows.jpg",
									column = 2,
									requirement = new TalentRequirement
									{
										name = "demonicSacrifice",
										pointsRequired = 1
									}
								}
							},
							{
								"demonicKnowledge", new Talent
								{
									name = "Demonic Knowledge",
									rankIds = new int[] { 35691, 35692, 35693 },
									iconName = "Spell_shadow_improvedvampiricembrace.jpg",
									column = 3
								}
							}
						}
					},
					{
						8, new Dictionary<string, Talent>
						{
							{
								"demonicTactics", new Talent
								{
									name = "Demonic Tactics",
									rankIds = new int[] { 30242, 30245, 30246, 30247, 30248 },
									iconName = "Spell_shadow_demonictactics.jpg",
									column = 2
								}
							}
						}
					},
					{
						9, new Dictionary<string, Talent>
						{
							{
								"summonFelguard", new Talent
								{
									name = "Summon Felguard",
									rankIds = new int[] { 30146 },
									iconName = "spell_shadow_summonfelguard.jpg",
									column = 2
								}
							}
						}
					}
				}
			},
			{
				"destruction", new Dictionary<int, Dictionary<string, Talent>>
				{
					{
						1, new Dictionary<string, Talent>
						{
							{
								"improvedShadowBolt", new Talent
								{
									name = "Improved Shadow Bolt",
									rankIds = new int[] { 17793, 17796, 17801, 17802, 17803 },
									iconName = "spell_shadow_shadowbolt.jpg",
									column = 2
								}
							},
							{
								"cataclysm", new Talent
								{
									name = "Cataclysm",
									rankIds = new int[] { 17778, 17779, 17780, 17781, 17782 },
									iconName = "spell_fire_windsofwoe.jpg",
									column = 3
								}
							}
						}
					},
					{
						2, new Dictionary<string, Talent>
						{
							{
								"bane", new Talent
								{
									name = "Bane",
									rankIds = new int[] { 17788, 17789, 17790, 17791, 17792 },
									iconName = "spell_shadow_deathpact.jpg",
									column = 2
								}
							},
							{
								"aftermath", new Talent
								{
									name = "Aftermath",
									rankIds = new int[] { 18119, 18120, 18121, 18122, 18123 },
									iconName = "spell_fire_fire.jpg",
									column = 3
								}
							}
						}
					},
					{
						3, new Dictionary<string, Talent>
						{
							{
								"improvedFirebolt", new Talent
								{
									name = "Improved Firebolt",
									rankIds = new int[] { 18126, 18127 },
									iconName = "spell_fire_firebolt.jpg",
									column = 1
								}
							},
							{
								"improvedLashOfPain", new Talent
								{
									name = "Improved Lash of Pain",
									rankIds = new int[] { 18128, 18129 },
									iconName = "spell_shadow_curse.jpg",
									column = 2
								}
							},
							{
								"devastation", new Talent
								{
									name = "Devastation",
									rankIds = new int[] { 18130, 18131, 18132, 18133, 18134 },
									iconName = "spell_fire_flameshock.jpg",
									column = 3
								}
							},
							{
								"shadowburn", new Talent
								{
									name = "Shadowburn",
									rankIds = new int[] { 17877 },
									iconName = "spell_shadow_scourgebuild.jpg",
									column = 4
								}
							}
						}
					},
					{
						4, new Dictionary<string, Talent>
						{
							{
								"intensity", new Talent
								{
									name = "Intensity",
									rankIds = new int[] { 18135, 18136 },
									iconName = "spell_fire_lavaspawn.jpg",
									column = 1
								}
							},
							{
								"destructiveReach", new Talent
								{
									name = "Destructive Reach",
									rankIds = new int[] { 17917, 17918 },
									iconName = "spell_shadow_corpseexplode.jpg",
									column = 2
								}
							},
							{
								"improvedSearingPain", new Talent
								{
									name = "Improved Searing Pain",
									rankIds = new int[] { 17927, 17929, 17930 },
									iconName = "spell_fire_soulburn.jpg",
									column = 4
								}
							}
						}
					},
					{
						5, new Dictionary<string, Talent>
						{
							{
								"pyroclasm", new Talent
								{
									name = "Pyroclasm",
									rankIds = new int[] { 18096, 18073 },
									iconName = "spell_fire_volcano.jpg",
									column = 1,
									requirement = new TalentRequirement
									{
										name = "intensity",
										pointsRequired = 2
									}
								}
							},
							{
								"improvedImmolate", new Talent
								{
									name = "Improved Immolate",
									rankIds = new int[] { 17815, 17833, 17834, 17835, 17836 },
									iconName = "spell_fire_immolation.jpg",
									column = 2
								}
							},
							{
								"ruin", new Talent
								{
									name = "Ruin",
									rankIds = new int[] { 17959 },
									iconName = "spell_shadow_shadowwordpain.jpg",
									column = 3,
									requirement = new TalentRequirement
									{
										name = "devastation",
										pointsRequired = 5
									}
								}
							}
						}
					},
					{
						6, new Dictionary<string, Talent>
						{
							{
								"netherProtection", new Talent
								{
									name = "Nether Protection",
									rankIds = new int[] { 30299, 30301, 30302 },
									iconName = "spell_shadow_netherprotection.jpg",
									column = 1
								}
							},
							{
								"emberstorm", new Talent
								{
									name = "Emberstorm",
									rankIds = new int[] { 17954, 17955, 17956, 17957, 17958 },
									iconName = "spell_fire_selfdestruct.jpg",
									column = 3
								}
							}
						}
					},
					{
						7, new Dictionary<string, Talent>
						{
							{
								"backlash", new Talent
								{
									name = "Backlash",
									rankIds = new int[] { 34935, 34938, 34939 },
									iconName = "spell_fire_playingwithfire.jpg",
									column = 1
								}
							},
							{
								"conflagrate", new Talent
								{
									name = "Conflagrate",
									rankIds = new int[] { 17962 },
									iconName = "spell_fire_fireball.jpg",
									column = 2,
									requirement = new TalentRequirement
									{
										name = "improvedImmolate",
										pointsRequired = 5
									}
								}
							},
							{
								"soulLeech", new Talent
								{
									name = "Soul Leech",
									rankIds = new int[] { 30293, 30295, 30296 },
									iconName = "spell_shadow_soulleech_3.jpg",
									column = 3
								}
							}
						}
					},
					{
						8, new Dictionary<string, Talent>
						{
							{
								"shadowAndFlame", new Talent
								{
									name = "Shadow and Flame",
									rankIds = new int[] { 30288, 30289, 30290, 30291, 30292 },
									iconName = "spell_shadow_shadowandflame.jpg",
									column = 2
								}
							}
						}
					},
					{
						9, new Dictionary<string, Talent>
						{
							{
								"shadowfury", new Talent
								{
									name = "Shadowfury",
									rankIds = new int[] { 30283 },
									iconName = "spell_shadow_shadowfury.jpg",
									column = 2,
									requirement = new TalentRequirement
									{
										name = "shadowAndFlame",
										pointsRequired = 5
									}
								}
							}
						}
					}
				}
			}
		};
	}

	public class Talents
	{
	}
}
