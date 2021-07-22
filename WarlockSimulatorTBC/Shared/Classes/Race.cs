using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public class RaceStats
	{
		public int stamina;
		public int intellect;
		public int spirit;
		public double intellectModifier;
		public double spiritModifier;
		public double petDamageModifier;
		public int fireResist;
		public int frostResist;
		public int shadowResist;
		public int arcaneResist;
		public int natureResist;
	}

	public static class Race
	{
		const string GNOME = "gnome";
		const string HUMAN = "human";
		const string ORC = "orc";
		const string UNDEAD = "undead";
		const string BLOODELF = "bloodElf";

		public static RaceStats GetStats(string race)
		{
			switch (race)
			{
				case GNOME:
					return new RaceStats
					{
						stamina = 75,
						intellect = 136,
						spirit = 139,
						arcaneResist = 10,
						intellectModifier = 1.05
					};
				case HUMAN:
					return new RaceStats
					{
						stamina = 76,
						intellect = 129,
						spirit = 132,
						spiritModifier = 1.1
					};
				case ORC:
					return new RaceStats
					{
						stamina = 78,
						intellect = 126,
						spirit = 134,
						petDamageModifier = 1.05
					};
				case UNDEAD:
					return new RaceStats
					{
						stamina = 77,
						intellect = 131,
						spirit = 144,
						shadowResist = 10
					};
				case BLOODELF:
					return new RaceStats
					{
						stamina = 74,
						intellect = 133,
						spirit = 130,
						fireResist = 5,
						frostResist = 5,
						shadowResist = 5,
						arcaneResist = 5,
						natureResist = 5
					};
				default:
					return new RaceStats
					{
						stamina = 0,
						intellect = 0,
						spirit = 0,
						intellectModifier = 1,
						spiritModifier = 1,
						petDamageModifier = 1,
						fireResist = 0,
						frostResist = 0,
						shadowResist = 0,
						arcaneResist = 0,
						natureResist = 0
					};
			}
		}
	}
}
