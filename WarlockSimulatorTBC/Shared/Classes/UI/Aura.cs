using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Blazored.LocalStorage;

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
		public int arcaneResist = 0;
		public int shadowResist = 0;
		public int fireResist = 0;
		public int frostResist = 0;
		public int natureResist = 0;
		public double shadowModifier = 1;
		public double fireModifier = 1;
		public double staminaModifier = 1;
		public double intellectModifier = 1;
		public double spiritModifier = 1;
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
		public bool drum = false;
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
		public static List<string> SelectedAuras = new List<string>();
		
		//todo: optimize
		public static void ModifyStatsFromAura(string auraGroup, string auraName, string action)
		{
			if (action == "add")
			{
				Console.WriteLine(Stats.playerStats.firePower);
				Console.WriteLine("adding " + groups[auraGroup].auras[auraName].firePower);
				Stats.playerStats.spellPower += groups[auraGroup].auras[auraName].spellPower;
				Stats.playerStats.shadowPower += groups[auraGroup].auras[auraName].shadowPower;
				Stats.playerStats.firePower += groups[auraGroup].auras[auraName].firePower;
				Stats.playerStats.spellPenetration += groups[auraGroup].auras[auraName].spellPenetration;
				Stats.playerStats.spellCritRating += groups[auraGroup].auras[auraName].spellCritRating;
				Stats.playerStats.intellect += groups[auraGroup].auras[auraName].intellect;
				Stats.playerStats.spirit += groups[auraGroup].auras[auraName].spirit;
				Stats.playerStats.mp5 += groups[auraGroup].auras[auraName].mp5;
				Stats.playerStats.stamina += groups[auraGroup].auras[auraName].stamina;
				Stats.playerStats.shadowModifier *= groups[auraGroup].auras[auraName].shadowModifier;
				Stats.playerStats.fireModifier *= groups[auraGroup].auras[auraName].fireModifier;
				Stats.playerStats.staminaModifier *= groups[auraGroup].auras[auraName].staminaModifier;
				Stats.playerStats.intellectModifier *= groups[auraGroup].auras[auraName].intellectModifier;
				Stats.playerStats.spiritModifier *= groups[auraGroup].auras[auraName].spiritModifier;
			}
			else if (action == "remove")
			{
				Console.WriteLine(Stats.playerStats.firePower);
				Console.WriteLine("removing: " + groups[auraGroup].auras[auraName].firePower);
				Stats.playerStats.spellPower -= groups[auraGroup].auras[auraName].spellPower;
				Stats.playerStats.shadowPower -= groups[auraGroup].auras[auraName].shadowPower;
				Stats.playerStats.firePower -= groups[auraGroup].auras[auraName].firePower;
				Stats.playerStats.spellPenetration -= groups[auraGroup].auras[auraName].spellPenetration;
				Stats.playerStats.spellCritRating -= groups[auraGroup].auras[auraName].spellCritRating;
				Stats.playerStats.intellect -= groups[auraGroup].auras[auraName].intellect;
				Stats.playerStats.spirit -= groups[auraGroup].auras[auraName].spirit;
				Stats.playerStats.mp5 -= groups[auraGroup].auras[auraName].mp5;
				Stats.playerStats.stamina -= groups[auraGroup].auras[auraName].stamina;
				Stats.playerStats.shadowModifier /= groups[auraGroup].auras[auraName].shadowModifier;
				Stats.playerStats.fireModifier /= groups[auraGroup].auras[auraName].fireModifier;
				Stats.playerStats.staminaModifier /= groups[auraGroup].auras[auraName].staminaModifier;
				Stats.playerStats.intellectModifier /= groups[auraGroup].auras[auraName].intellectModifier;
				Stats.playerStats.spiritModifier /= groups[auraGroup].auras[auraName].spiritModifier;
			}
		}

		public static Dictionary<string, AuraGroup> groups = new Dictionary<string, AuraGroup>
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
						},
						{
							"blessingOfKings", new Aura
							{
								id = 25898,
								name = "Blessing of Kings",
								staminaModifier = 1.1,
								intellectModifier = 1.1,
								spiritModifier = 1.1,
								iconName = "spell_magic_greaterblessingofkings.jpg"
							}
						},
						{
							"blessingOfWisdom", new Aura
							{
								id = 27143,
								name = "Blessing of Wisdom",
								mp5 = 41,
								iconName = "spell_holy_greaterblessingofwisdom.jpg"
							}
						},
						{
							"judgementOfWisdom", new Aura
							{
								id = 20354,
								name = "Judgement of Wisdom",
								iconName = "spell_holy_righteousnessaura.jpg"
							}
						},
						{
							"manaSpringTotem", new Aura
							{
								id = 10497,
								name = "Mana Spring Totem",
								iconName = "spell_nature_manaregentotem.jpg",
								mp5 = 50
							}
						},
						{
							"wrathOfAirTotem", new Aura
							{
								id = 3738,
								name = "Wrath of Air Totem",
								iconName = "spell_nature_slowingtotem.jpg",
								spellPower = 101
							}
						},
						{
							"totemOfWrath", new Aura
							{
								id = 30706,
								name = "Totem of Wrath",
								iconName = "spell_fire_totemofwrath.jpg"
							}
						},
						{
							"markOfTheWild", new Aura
							{
								id = 26990,
								name = "Mark of the Wild",
								iconName = "spell_nature_regeneration.jpg",
								stamina = 14,
								intellect = 14,
								spirit = 14,
								arcaneResist = 25,
								frostResist = 25,
								fireResist = 25,
								natureResist = 25,
								shadowResist = 25,
							}
						},
						{
							"arcaneIntellect", new Aura
							{
								id = 27127,
								name = "Arcane Intellect",
								intellect = 40,
								iconName = "spell_holy_arcaneintellect.jpg"
							}
						},
						{
							"prayerOfFortitude", new Aura
							{
								id = 25392,
								name = "Prayer of Fortitude",
								iconName = "spell_holy_prayeroffortitude.jpg",
								stamina = 79
							}
						},
						{
							"prayerOfSpirit", new Aura
							{
								id = 32999,
								name = "Prayer of Spirit",
								iconName = "spell_holy_prayerofspirit.jpg",
								spirit = 50
							}
						},
						{
							"bloodPact", new Aura
							{
								id = 27268,
								name = "Blood Pact",
								iconName = "spell_shadow_bloodboil.jpg",
								stamina = 70
							}
						},
						{
							"inspiringPresence", new Aura
							{
								id = 28878,
								name = "Inspiring Presence",
								iconName = "inv_staff_23.jpg"
							}
						},
						{
							"moonkinAura", new Aura
							{
								id = 24907,
								name = "Moonkin Aura",
								iconName = "spell_nature_moonglow.jpg"
							}
						},
						{
							"powerInfusion", new Aura
							{
								id = 10060,
								name = "Power Infusion",
								iconName = "spell_holy_powerinfusion.jpg"
							}
						},
						{
							"powerOfTheGuardianWarlock", new Aura
							{
								id = 28143,
								name = "Power of the Guardian (Warlock)",
								iconName = "spell_nature_moonglow.jpg"
							}
						},
						{
							"powerOfTheGuardianMage", new Aura
							{
								id = 28142,
								name = "Power of the Guardian (Mage)",
								iconName = "spell_nature_moonglow.jpg"
							}
						},
						{
							"eyeOfTheNight", new Aura
							{
								id = 31033,
								name = "Eye of the Night",
								spellPower = 34,
								iconName = "inv_jewelry_necklace_28.jpg"
							}
						},
						{
							"chainOfTheTwilightOwl", new Aura
							{
								id = 31035,
								name = "Chain of the Twilight Owl",
								iconName = "inv_jewelry_necklace_ahnqiraj_02.jpg"
							}
						},
						{
							"jadePendantOfBlasting", new Aura
							{
								id = 25607,
								name = "Jade Pendant of Blasting",
								iconName = "inv_jewelry_necklace_01.jpg",
								spellPower = 15
							}
						},
						{
							"idolOfTheRavenGoddess", new Aura
							{
								id = 39926,
								name = "Idol of the Raven Goddess",
								iconName = "inv-mount_raven_54.jpg",
								spellCritRating = 20
							}
						},
						{
							"drumsOfBattle", new Aura
							{
								id = 35476,
								name = "Drums of Battle",
								iconName = "inv_misc_drum_02.jpg",
								drum = true
							}
						},
						{
							"drumsOfWar", new Aura
							{
								id = 35475,
								name = "Drums of War",
								iconName = "inv_misc_drum_03.jpg",
								drum = true
							}
						},
						{
							"drumsOfRestoration", new Aura
							{
								id = 35478,
								name = "Drums of Restoration",
								iconName = "inv_misc_drum_07.jpg",
								drum = true
							}
						},
						{
							"bloodlust", new Aura
							{
								id = 2825,
								name = "Bloodlust",
								iconName = "spell_nature_bloodlust.jpg"
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
						},
						{
							"shadowWeaving", new Aura
							{
								id = 15334,
								name = "Shadow Weaving",
								iconName = "spell_shadow_blackplague.jpg",
								shadowModifier = 1.1
							}
						},
						{
							"improvedScorch", new Aura
							{
								id = 12873,
								name = "Improved Scorch",
								iconName = "spell_fire_soulburn.jpg",
								fireModifier = 1.15
							}
						},
						{
							"misery", new Aura
							{
								id = 33195,
								name = "Misery",
								shadowModifier = 1.05,
								fireModifier = 1.05,
								iconName = "Spell_shadow_misery.jpg"
							}
						},
						{
							"judgementOfTheCrusader", new Aura
							{
								id = 20337,
								name = "Judgement of the Crusader",
								iconName = "spell_holy_holysmite.jpg"
							}
						},
						{
							"vampiricTouch", new Aura
							{
								id = 34914,
								name = "Vampiric Touch",
								iconName = "spell_holy_stoicism.jpg"
							}
						},
						{
							"faerieFire", new Aura
							{
								id = 26993,
								name = "Faerie Fire",
								forPet = true,
								iconName = "spell_nature_faeriefire.jpg"
							}
						},
						{
							"sunderArmor", new Aura
							{
								id = 25225,
								name = "Sunder Armor",
								forPet = true,
								iconName = "ability_warrior_sunder.jpg"
							}
						},
						{
							"exposeArmor", new Aura
							{
								id = 26866,
								name = "Expose Armor",
								forPet = true,
								iconName = "ability_warrior_riposte.jpg"
							}
						},
						{
							"curseOfRecklessness", new Aura
							{
								id = 27226,
								name = "Curse of Recklessness",
								forPet = true,
								iconName = "spell_shadow_unholystrength.jpg"
							}
						},
						{
							"bloodFrenzy", new Aura
							{
								id = 29859,
								name = "Blood Frenzy",
								forPet = true,
								iconName = "ability_warrior_bloodfrenzy.jpg"
							}
						},
						{
							"exposeWeakness", new Aura
							{
								id = 34503,
								iconName = "ability_rogue_findweakness.jpg",
								forPet = true,
								name = "Expose Weakness"
							}
						},
						{
							"annihilator", new Aura
							{
								id = 16928,
								name = "Annihilator",
								forPet = true,
								iconName = "inv_axe_12.jpg"
							}
						},
						{
							"improvedHuntersMark", new Aura
							{
								id = 19425,
								name = "Improved Hunter's Mark",
								forPet = true,
								iconName = "ability_hunter_snipershot.jpg"
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
