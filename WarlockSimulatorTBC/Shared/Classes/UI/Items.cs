using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes.UI
{
	public static class ItemSource
	{
		public static string MAGTHERIDONS_LAIR = "Magtheridon's Lair";
		public static string GRUULS_LAIR = "Gruul's Lair";
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
		public Dictionary<int, Item> items;
	}

	public static class Items
	{
		public static string SelectedItemSlot = ItemSlots.HEAD;
		public static string SelectedItemSubSlot = "";
		public static Dictionary<string, int?> SelectedItems = new Dictionary<string, int?>
		{
			{ ItemSlots.HEAD, null },
			{ ItemSlots.NECK, null },
			{ ItemSlots.SHOULDERS, null },
			{ ItemSlots.BACK, null },
			{ ItemSlots.CHEST, null },
			{ ItemSlots.BRACERS, null },
			{ ItemSlots.GLOVES, null },
			{ ItemSlots.BELT, null },
			{ ItemSlots.LEGS, null },
			{ ItemSlots.BOOTS, null },
			{ ItemSlots.RING + "1", null },
			{ ItemSlots.RING + "2", null },
			{ ItemSlots.TRINKET + "1", null },
			{ ItemSlots.TRINKET + "2", null },
			{ ItemSlots.MAINHAND, null },
			{ ItemSlots.OFFHAND, null },
			{ ItemSlots.TWOHAND, null },
			{ ItemSlots.WAND, null }
		};

		public static readonly Dictionary<string, ItemSlot> itemSlots = new Dictionary<string, ItemSlot>
		{
			{
				ItemSlots.HEAD, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							28963, new Item
							{
								name = "Voidheart Crown",
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
							24266, new Item
							{
								name = "Spellstrike Hood",
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
			},
			{
				ItemSlots.NECK, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							28762, new Item
							{
								name = "Adornment of Stolen Souls",
								stamina = 18,
								intellect = 20,
								spellPower = 28,
								spellCritRating = 23,
								source = ItemSource.KARAZHAN,
								phase = 1
							}
						}
					}
				}
			},
			{
				ItemSlots.SHOULDERS, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							28967, new Item
							{
								name = "Voidheart Mantle",
								stamina = 26,
								intellect = 22,
								spellPower = 37,
								spellHitRating = 14,
								blue = 1,
								yellow = 1,
								source = ItemSource.GRUULS_LAIR,
								phase = 1,
								setId = 645
							}
						}
					}
				}
			},
			{
				ItemSlots.BACK, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							28766, new Item
							{
								name = "Ruby Drape of the Mysticant",
								stamina = 22,
								intellect = 21,
								spellPower = 30,
								spellHitRating = 18,
								source = ItemSource.KARAZHAN,
								phase = 1
							}
						}
					}
				}
			},
			{
				ItemSlots.CHEST, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							28964, new Item
							{
								name = "Voidheart Robe",
								stamina = 42,
								intellect = 33,
								spellPower = 42,
								spellHitRating = 13,
								red = 1,
								yellow = 2,
								source = ItemSource.MAGTHERIDONS_LAIR,
								phase = 1,
								setId = 645
							}
						}
					}
				}
			},
			{
				ItemSlots.BRACERS, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							24250, new Item
							{
								name = "Bracers of Havok",
								intellect = 12,
								spellPower = 30,
								yellow = 1,
								source = ItemSource.TAILORING,
								phase = 1
							}
						}
					}
				}
			},
			{
				ItemSlots.GLOVES, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							28968, new Item
							{
								name = "Voidheart Gloves",
								stamina = 34,
								intellect = 28,
								spellPower = 35,
								spellCritRating = 22,
								source = ItemSource.KARAZHAN,
								phase = 1,
								setId = 645
							}
						}
					}
				}
			},
			{
				ItemSlots.BELT, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							24256, new Item
							{
								name = "Girdle of Ruination",
								stamina = 18,
								intellect = 13,
								spellPower = 39,
								spellCritRating = 20,
								red = 1,
								yellow = 1,
								source = ItemSource.TAILORING,
								phase = 1
							}
						}
					}
				}
			},
			{
				ItemSlots.LEGS, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							24262, new Item
							{
								name = "Spellstrike Pants",
								stamina = 12,
								intellect = 8,
								spellPower = 46,
								spellCritRating = 26,
								spellHitRating = 22,
								red = 1,
								yellow = 1,
								blue = 1,
								source = ItemSource.TAILORING,
								phase = 1
							}
						}
					}
				}
			},
			{
				ItemSlots.BOOTS, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							21870, new Item
							{
								name = "Frozen Shadoweave Boots",
								stamina = 15,
								intellect = 9,
								shadowPower = 57,
								yellow = 1,
								blue = 1,
								source = ItemSource.TAILORING,
								phase = 1
							}
						}
					}
				}
			},
			{
				ItemSlots.RING, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							28793, new Item
							{
								name = "Band of Crimson Fury",
								stamina = 22,
								intellect = 22,
								spellPower = 28,
								spellHitRating = 16,
								source = ItemSource.MAGTHERIDONS_LAIR,
								phase = 1
							}
						},
						{
							28753, new Item
							{
								name = "Ring of Recurrence",
								stamina = 15,
								intellect = 15,
								spellPower = 32,
								spellCritRating = 19,
								source = ItemSource.KARAZHAN,
								phase = 1
							}
						}
					}
				}
			},
			{
				ItemSlots.TRINKET, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							29370, new Item
							{
								name = "Icon of the Silver Crescent",
								spellPower = 43,
								source = "41 Badge of Justice - G'eras",
								phase = 1
							}
						},
						{
							27683, new Item
							{
								name = "Quagmirran's Eye",
								spellPower = 37,
								source = "Heroic Slave Pens",
								phase = 1
							}
						}
					}
				}
			},
			{
				ItemSlots.MAINHAND, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							28802, new Item
							{
								name = "Bloodmaw Magus-Blade",
								stamina = 16,
								intellect = 15,
								spellPower = 203,
								spellCritRating = 25,
								source = ItemSource.GRUULS_LAIR,
								phase = 1
							}
						}
					}
				}
			},
			{
				ItemSlots.OFFHAND, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							29273, new Item
							{
								name = "Khadgar's Knapsack",
								spellPower = 49,
								source = "25 Badge of Justice - G'eras",
								phase = 1
							}
						}
					}
				}
			},
			{
				ItemSlots.TWOHAND, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							22630, new Item
							{
								name = "Atiesh, Greatstaff of the Guardian",
								stamina = 30,
								intellect = 29,
								spellPower = 183,
								spellCritRating = 28,
								source = ItemSource.NAXXRAMAS,
								phase = 0
							}
						}
					}
				}
			},
			{
				ItemSlots.WAND, new ItemSlot
				{
					items = new Dictionary<int, Item>
					{
						{
							28783, new Item
							{
								name = "Eredar Wand of Obliteration",
								stamina = 10,
								intellect = 11,
								spellPower = 16,
								spellCritRating = 14,
								source = ItemSource.MAGTHERIDONS_LAIR,
								phase = 1
							}
						}
					}
				}
			}
		};
	}
}
