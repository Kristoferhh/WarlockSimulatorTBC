using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes.UI
{
	public static class ItemSource
	{
		public static string KARAZHAN = "Karazhan";
		public static string TAILORING = "Tailoring";
		public static string NAXXRAMAS = "Naxxramas";
	}

	public static class ItemSlots
	{
		public static string HEAD = "head";
		public static string NECK = "neck";
		public static string SHOULDERS = "shoulders";
		public static string BACK = "back";
		public static string CHEST = "chest";
		public static string BRACERS = "bracers";
		public static string GLOVES = "gloves";
		public static string BELT = "belt";
		public static string LEGS = "legs";
		public static string BOOTS = "boots";
		public static string RING = "ring";
		public static string TRINKET = "trinket";
		public static string MAINHAND = "mainHand";
		public static string OFFHAND = "offHand";
		public static string TWOHAND = "twoHand";
		public static string WAND = "wand";
	}

	public class SocketBonus
	{
		public int spellPower;
		public int spellHitRating;
		public int spellCritRating;
		public int resilienceRating;
		public int stamina;
		public int spirit;
		public int mp5;
	}

	public class Item
	{
		public int id = 0;
		public int phase = 0;
		public int setId = 0;
		public int stamina = 0;
		public int intellect = 0;
		public int meta = 0;
		public int red = 0;
		public int blue = 0;
		public int yellow = 0;
		public SocketBonus socketBonus;
		public int spellPower = 0;
		public int shadowPower = 0;
		public int firePower = 0;
		public int spellCritRating = 0;
		public int spellHitRating = 0;
		public int spellHasteRating = 0;
		public int resilienceRating = 0;
		public string name = null;
		public string source = null;
	}

	public class ItemSlot
	{
		public Dictionary<string, Item> items;
	}

	public static class Items
	{
		public static void LoadItemsBySlot(string itemSlot)
		{
			Console.WriteLine("Loading item slot " + itemSlot);
		}

		public static readonly Dictionary<string, ItemSlot> itemSlots = new Dictionary<string, ItemSlot>
		{
			{
				"head", new ItemSlot
				{
					items = new Dictionary<string, Item>
					{
						{
							"voidheartCrown", new Item
							{
								name = "Voidheart Crown",
								id = 28963,
								meta = 1,
								red = 1,
								socketBonus = new SocketBonus
								{
									spellHitRating = 4
								},
								stamina = 39,
								intellect = 36,
								spellPower = 40,
								spellCritRating = 19,
								setId = 645,
								source = ItemSource.KARAZHAN
							}
						},
						{
							"spellstrikeHood", new Item
							{
								name = "Spellstrike Hood",
								id = 24266,
								red = 1,
								blue = 1,
								yellow = 1,
								socketBonus = new SocketBonus
								{
									stamina = 6
								},
								stamina = 16,
								intellect = 12,
								spellPower = 46,
								spellCritRating = 24,
								spellHitRating = 16,
								setId = 559,
								source = ItemSource.TAILORING
							}
						}
					}
				}
			}
		};
	}
}
