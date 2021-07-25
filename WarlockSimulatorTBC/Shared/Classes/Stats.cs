using System;
using System.Collections.Generic;
using System.Linq;
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
		public double health = 0;
		public double mana = 0;
		public double stamina = 0;
		public double intellect = 0;
		public double spirit = 0;
		public double spellPower = 0;
		public double shadowPower = 0;
		public double firePower = 0;
		public int spellCritRating = 0;
		public int spellHitRating = 0;
		public int spellHasteRating = 0;
		public int mp5 = 0;
		public int spellPenetration = 0;
		public double damageModifier = 1;
		public double shadowModifier = 1;
		public double fireModifier = 1;
		public double staminaModifier = 1;
		public double intellectModifier = 1;
		public double spiritModifier = 1;
		// Pet stats
		public int meleeCritRating = 0;
		public int meleeHitRating = 0;
	}
}
