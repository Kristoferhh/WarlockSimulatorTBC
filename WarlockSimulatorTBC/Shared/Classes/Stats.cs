using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public static class Stats
	{
		public static CharacterStats playerStats = new CharacterStats();
		// Stat convertions
		public static double hitRatingPerPercent = 12.6;
		public static double critRatingPerPercent = 22.08;
		public static double hasteRatingPerPercent = 15.77;
		public static int manaPerInt = 15;
		public static int healthPerStamina = 10;
		public static double critPercentPerInt = 1 / 81.95;
		public static double baseCritChance = 1.701;
	}

	public class CharacterStats
	{
		public double health { get; set; } = 0;
		public double maxMana { get; set; } = 0;
		public double currentMana { get; set; } = 0;
		public double stamina { get; set; } = 0;
		public double intellect { get; set; } = 0;
		public double spirit { get; set; } = 0;
		public double spellPower { get; set; } = 0;
		public double shadowPower { get; set; } = 0;
		public double firePower { get; set; } = 0;
		public int spellCritRating { get; set; } = 0;
		public int spellHitRating { get; set; } = 0;
		public int spellHasteRating { get; set; } = 0;
		public int mp5 { get; set; } = 0;
		public int spellPenetration { get; set; } = 0;
		public double damageModifier { get; set; } = 1;
		public double shadowModifier { get; set; } = 1;
		public double fireModifier { get; set; } = 1;
		public double staminaModifier { get; set; } = 1;
		public double intellectModifier { get; set; } = 1;
		public double spiritModifier { get; set; } = 1;
		// Pet stats
		public int meleeCritRating { get; set; } = 0;
		public int meleeHitRating { get; set; } = 0;
	}
}
