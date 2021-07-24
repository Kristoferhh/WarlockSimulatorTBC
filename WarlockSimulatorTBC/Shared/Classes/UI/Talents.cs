using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes.UI
{
	public class Talent
	{
		public int[] rankIds = null;
		public int row = 0;
		public int column = 0;
		public string name = null;
		public string iconName = null;
	}

	public static class TalentTree
	{
		public static readonly Dictionary<string, Dictionary<string, Talent>> trees = new Dictionary<string, Dictionary<string, Talent>>
		{
			{
				"affliction", new Dictionary<string, Talent>
				{
					{
						"suppression", new Talent
						{
							name = "Suppression",
							rankIds = new int[] { 18174, 18175, 18176, 18177, 18178 },
							iconName = "spell_shadow_unsummonbuilding.jpg",
							row = 1,
							column = 2
						}
					},
					{
						"improvedCorruption", new Talent
						{
							name = "Improved Corruption",
							rankIds = new int[] { 17810, 17811, 17812, 17813, 17814 },
							iconName = "spell_shadow_abominationexplosion.jpg",
							row = 1,
							column = 3
						}
					},
					{
						"improvedCurseOfWeakness", new Talent
						{
							name = "Improved Curse of Weakness",
							rankIds = new int[] { 18179, 18180 },
							iconName = "spell_shadow_curseofmannoroth.jpg",
							row = 2,
							column = 1
						}
					},
					{
						"improvedDrainSoul", new Talent
						{
							name = "Improved Drain Soul",
							rankIds = new int[] { 18213, 18372 },
							iconName = "spell_shadow_haunting.jpg",
							row = 2,
							column = 2
						}
					},
					{
						"improvedLifeTap", new Talent
						{
							name = "Improved Life Tap",
							rankIds = new int[] { 18182, 18183 },
							iconName = "spell_shadow_burningspirit.jpg",
							row = 2,
							column = 3
						}
					},
					{
						"soulSiphon", new Talent
						{
							name = "Soul Siphon",
							rankIds = new int[] { 17804, 17805 },
							iconName = "spell_shadow_lifedrain02.jpg",
							row = 2,
							column = 4
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
