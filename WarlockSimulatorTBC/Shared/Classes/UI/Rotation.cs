using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes.UI
{
	public class RotationGroup
	{
		public string heading;
		public Dictionary<string, RotationSpell> spells;
	}

	public class RotationSpell
	{
		public int id;
		public string name;
		public string iconName;
		public bool curse = false;
		public bool filler = false;
		public bool selected = false;
	}

	public static class RotationGroups
	{
		public static readonly Dictionary<string, RotationGroup> groups = new Dictionary<string, RotationGroup>
		{
			{
				"dot", new RotationGroup
				{
					heading = "Dots",
					spells = new Dictionary<string, RotationSpell>
					{
						{
							"immolate", new RotationSpell
							{
								id = 27215,
								name = "Immolate",
								iconName = "spell_fire_immolation.jpg",
							}
						},
						{
							"corruption", new RotationSpell
							{
								id = 27216,
								name = "Corruption",
								iconName = "spell_shadow_abominationexplosion.jpg",
							}
						}
						,
						{
							"siphonLife", new RotationSpell
							{
								id = 30911,
								name = "Siphon Life",
								iconName = "spell_shadow_requiem.jpg",
							}
						}
						,
						{
							"unstableAffliction", new RotationSpell
							{
								id = 30405,
								name = "Unstable Affliction",
								iconName = "spell_shadow_unstableaffliction_3.jpg",
							}
						}
					}
				}
			},
			{
				"filler", new RotationGroup
				{
					heading = "Filler",
					spells = new Dictionary<string, RotationSpell>
					{
						{
							"searingPain", new RotationSpell
							{
								id = 30459,
								name = "Searing Pain",
								iconName = "spell_fire_soulburn.jpg",
								filler = true

							}
						},
						{
							"shadowBolt", new RotationSpell
							{
								id = 27209,
								name = "Shadow Bolt",
								iconName = "spell_shadow_shadowbolt.jpg",
								filler = true

							}
						},
						{
							"incinerate", new RotationSpell
							{
								id = 32231,
								name = "Incinerate",
								iconName = "spell_fire_burnout.jpg",
								filler = true
							}
						}
					}
				}
			},
			{
				"curse", new RotationGroup
				{
					heading = "Curse",
					spells = new Dictionary<string, RotationSpell>
					{
						{
							"curseOfRecklessness", new RotationSpell
							{
								id = 27226,
								name = "Curse of Recklessness",
								iconName = "spell_shadow_unholystrength.jpg",
								curse = true
							}
						},
						{
							"curseOfTheElements", new RotationSpell
							{
								id = 27228,
								name = "Curse of the Elements",
								iconName = "spell_shadow_chilltouch.jpg",
								curse = true
							}
						},
						{
							"curseOfDoom", new RotationSpell
							{
								id = 30910,
								name = "Curse of Doom",
								iconName = "spell_shadow_auraofdarkness.jpg",
								curse = true
							}
						},
						{
							"curseOfAgony", new RotationSpell
							{
								id = 27218,
								name = "Curse of Agony",
								iconName = "spell_shadow_curseofsargeras.jpg",
								curse = true
							}
						}
					}
				}
			},
			{
				"finisher", new RotationGroup
				{
					heading = "Finisher",
					spells = new Dictionary<string, RotationSpell>
					{
						{
							"deathCoil", new RotationSpell
							{
								id = 27223,
								name = "Death Coil",
								iconName = "spell_shadow_deathcoil.jpg",
							}
						},
						{
							"shadowburn", new RotationSpell
							{
								id = 30546,
								name = "Shadowburn",
								iconName = "spell_shadow_scourgebuild.jpg",
							}
						},
						{
							"conflagrate", new RotationSpell
							{
								id = 30912,
								name = "Conflagrate",
								iconName = "spell_fire_fireball.jpg",
							}
						}
					}
				}
			},
			{
				"other", new RotationGroup
				{
					heading = "Other",
					spells = new Dictionary<string, RotationSpell>
					{
						{
							"shadowfury", new RotationSpell
							{
								id = 30414,
								name = "Shadowfury",
								iconName = "spell_shadow_shadowfury.jpg",
							}
						},
						{
							"amplifyCurse", new RotationSpell
							{
								id = 18288,
								name = "Amplify Curse",
								iconName = "spell_shadow_contagion.jpg",
							}
						},
						{
							"darkPact", new RotationSpell
							{
								id = 27265,
								name = "Dark Pact",
								iconName = "spell_shadow_darkritual.jpg",
							}
						}
					}
				}
			}
		};
	}
}