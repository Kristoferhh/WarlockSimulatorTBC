using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes.UI
{
	public class Aura
	{
		public int id;
		public int spellPower = 0;
		public int shadowPower = 0;
		public int firePower = 0;
		public int spellPenetration = 0;
		public int spellCritRating = 0;
		public int intellect = 0;
		public int spirit = 0;
		public int mp5 = 0;
		public int stamina = 0;
		public double shadowModifier = 1;
		public double fireModifier = 1;
		public string name = null;
		public string iconName = null;
		public bool battleElixir = false;
		public bool guardianElixir = false;
		public bool potion = false;
		public bool weaponOil = false;
		public bool demonicRune = false;
		public bool foodBuff = false;
		public bool alcohol = false;
		public bool petOnly = false;
		public bool forPet = false;
	}

	public class AuraGroup
	{
		public string heading;
		// Either 'spell' or 'item' depending on what the wowhead url says that we need to direct to (e.g. "wowhead.com/spell=12543/"
		public string type;
		public Dictionary<string, Aura> auras;
	}

	public static class AuraGroups
	{
		public static readonly Dictionary<string, AuraGroup> groups = new Dictionary<string, AuraGroup>
		{
			{
				"buffs", new AuraGroup
				{
					heading = "Buffs",
					type = "spell",
					auras = new Dictionary<string, Aura>
					{
						{
							"felArmor", new Aura
							{
								id = 28189,
								name = "Fel Armor",
								iconName = "spell_shadow_felarmour.jpg",
								spellPower = 100
							}
						}
					}
				}
			},
			{
				"debuffs", new AuraGroup
				{
					heading = "Debuffs",
					type = "spell",
					auras = new Dictionary<string, Aura>
					{
						{
							"curseOfTheElements", new Aura
							{
								id = 27228,
								name = "Curse of the Elements",
								iconName = "spell_shadow_chilltouch.jpg",
								spellPenetration = 88,
								shadowModifier = 1.1,
								fireModifier = 1.1

							}
						}
					}
				}
			},
			{
				"consumables", new AuraGroup
				{
					heading = "Consumables",
					type = "item",
					auras = new Dictionary<string, Aura>
					{
						{
							"flaskOfPureDeath", new Aura
							{
								id = 22866,
								name = "Flask of Pure Death",
								iconName = "inv_potion_115.jpg",
								shadowPower = 80,
								firePower = 80,
								battleElixir = true,
								guardianElixir = true
							}
						},
						{
							"elixirOfMajorShadowPower", new Aura
							{
								id = 22835,
								name = "Elixir of Major Shadow Power",
								iconName = "inv_potion_145.jpg",
								shadowPower = 55,
								battleElixir = true
							}
						},
						{
							"elixirOfMajorFirePower", new Aura
							{
								id = 22833,
								name = "Elixir of Major Fire Power",
								iconName = "inv_potion_146.jpg",
								firePower = 55,
								battleElixir = true
							}
						},
						{
							"greaterArcaneElixir", new Aura
							{
								id = 13454,
								name = "Greater Arcane Elixir",
								iconName = "inv_potion_25.jpg",
								spellPower = 35,
								battleElixir = true
							}
						},
						{
							"adeptsElixir", new Aura
							{
								id = 28103,
								name = "Adept's Elixir",
								iconName = "inv_potion_96.jpg",
								spellPower = 24,
								spellCritRating = 24,
								battleElixir = true
							}
						},
						{
							"elixirOfDraenicWisdom", new Aura
							{
								id = 32067,
								name = "Elixir of Draenic Wisdom",
								iconName = "inv_potion_155.jpg",
								intellect = 30,
								spirit = 30,
								guardianElixir = true
							}
						},
						{
							"elixirOfMajorMageblood", new Aura
							{
								id = 22840,
								name = "Elixir of Major Mageblood",
								iconName = "inv_potion_151.jpg",
								mp5 = 16,
								guardianElixir = true
							}
						},
						{
							"superManaPotion", new Aura
							{
								id = 22832,
								name = "Super Mana Potion",
								iconName = "inv_potion_137.jpg",
								potion = true
							}
						},
						{
							"destructionPotion", new Aura
							{
								id = 22839,
								name = "Destruction Potion",
								iconName = "inv_potion_107.jpg",
								potion = true
							}
						},
						{
							"brilliantWizardOil", new Aura
							{
								id = 20749,
								name = "Brilliant Wizard oil",
								iconName = "inv_potion_105.jpg",
								spellPower = 36,
								spellCritRating = 14,
								weaponOil = true
							}
						},
						{
							"superiorWizardOil", new Aura
							{
								id = 22522,
								name = "Superior Wizard Oil",
								iconName = "inv_potion_141.jpg",
								spellPower = 42,
								weaponOil = true
							}
						},
						{
							"blessedWizardOil", new Aura
							{
								id = 23123,
								name = "Blessed Wizard Oil",
								iconName = "inv_potion_26.jpg",
								spellPower = 60,
								weaponOil = true
							}
						},
						{
							"demonicRune", new Aura
							{
								id = 12662,
								name = "Demonic Rune",
								iconName = "inv_misc_rune_04.jpg",
								demonicRune = true
							}
						},
						{
							"flameCap", new Aura
							{
								id = 22788,
								name = "Flame Cap",
								iconName = "inv_misc_herb_flamecap.jpg",
								demonicRune = true
							}
						},
						{
							"rumseyRumBlackLabel", new Aura
							{
								id = 21151,
								name = "Rumsey Rum Black Label",
								iconName = "inv_drink_04.jpg",
								stamina = 15,
								alcohol = true
							}
						},
						{
							"kreegsStoutBeatdown", new Aura
							{
								id = 18284,
								name = "Kreeg's Sout Beatdown",
								iconName = "inv_drink_05.jpg",
								intellect = -5,
								spirit = 25,
								alcohol = true
							}
						},
						{
							"blackenedBasilisk", new Aura
							{
								id = 27657,
								name = "Blackened Basilisk",
								iconName = "inv_misc_food_86_basilisk.jpg",
								spellPower = 23,
								spirit = 20,
								foodBuff = true
							}
						},
						{
							"skullfishSoup", new Aura
							{
								id = 33825,
								name = "Skullfish Soup",
								iconName = "inv_misc_food_63.jpg",
								spellCritRating = 20,
								spirit = 20,
								foodBuff = true
							}
						},
						{
							"veryBerryCream", new Aura
							{
								id = 22238,
								name = "Very Berry Cream",
								iconName = "inv_valentineschocolate02.jpg",
								spellPower = 23
							}
						},
					}
				}
			}
		};
	}
}
