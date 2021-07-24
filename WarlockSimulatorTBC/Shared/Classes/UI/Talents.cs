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
			}
		};
	}

	public class Talents
	{
	}
}
