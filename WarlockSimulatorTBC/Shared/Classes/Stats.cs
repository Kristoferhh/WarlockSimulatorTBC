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

	[DataContract]
	public class CharacterStats
	{
		[DataMember]
		public double health = 0;
		[DataMember]
		public double maxMana = 0;
		[DataMember]
		public double currentMana = 0;
		[DataMember]
		public double stamina = 0;
		[DataMember]
		public double intellect = 0;
		[DataMember]
		public double spirit = 0;
		[DataMember]
		public double spellPower = 0;
		[DataMember]
		public double shadowPower = 0;
		[DataMember]
		public double firePower = 0;
		[DataMember]
		public int spellCritRating = 0;
		[DataMember]
		public int spellHitRating = 0;
		[DataMember]
		public int spellHasteRating = 0;
		[DataMember]
		public int mp5 = 0;
		[DataMember]
		public int spellPenetration = 0;
		[DataMember]
		public double damageModifier = 1;
		[DataMember]
		public double shadowModifier = 1;
		[DataMember]
		public double fireModifier = 1;
		[DataMember]
		public double staminaModifier = 1;
		[DataMember]
		public double intellectModifier = 1;
		[DataMember]
		public double spiritModifier = 1;
		// Pet stats
		[DataMember]
		public int meleeCritRating = 0;
		[DataMember]
		public int meleeHitRating = 0;
	}
}
