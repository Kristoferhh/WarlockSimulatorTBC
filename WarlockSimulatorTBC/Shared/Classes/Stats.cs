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

	[Serializable()]
	[KnownType(typeof(CharacterStats))]
	public class CharacterStats : ISerializable
	{
		public double health = 0;
		public double maxMana = 0;
		public double currentMana = 0;
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

		public CharacterStats() { }

		public CharacterStats(SerializationInfo info, StreamingContext context)
		{
			health = (double)info.GetValue("health", typeof(double));
			maxMana = (double)info.GetValue("maxMana", typeof(double));
			currentMana = (double)info.GetValue("currentMana", typeof(double));
			stamina = (double)info.GetValue("stamina", typeof(double));
			intellect = (double)info.GetValue("intellect", typeof(double));
			spirit = (double)info.GetValue("spirit", typeof(double));
			spellPower = (double)info.GetValue("spellPower", typeof(double));
			shadowPower = (double)info.GetValue("shadowPower", typeof(double));
			firePower = (double)info.GetValue("firePower", typeof(double));
			spellCritRating = (int)info.GetValue("spellCritRating", typeof(int));
			spellHitRating = (int)info.GetValue("spellHitRating", typeof(int));
			spellHasteRating = (int)info.GetValue("spellHasteRating", typeof(int));
			mp5 = (int)info.GetValue("mp5", typeof(int));
			spellPenetration = (int)info.GetValue("spellPenetration", typeof(int));
			damageModifier = (double)info.GetValue("damageModifier", typeof(double));
			shadowModifier = (double)info.GetValue("shadowModifier", typeof(double));
			fireModifier = (double)info.GetValue("fireModifier", typeof(double));
			staminaModifier = (double)info.GetValue("staminaModifier", typeof(double));
			intellectModifier = (double)info.GetValue("intellectModifier", typeof(double));
			spiritModifier = (double)info.GetValue("spiritModifier", typeof(double));
			meleeCritRating = (int)info.GetValue("meleeCritRating", typeof(int));
			meleeHitRating = (int)info.GetValue("meleeHitRating", typeof(int));
		}

		public void GetObjectData(SerializationInfo info, StreamingContext context)
		{
			info.AddValue("health", health);
			info.AddValue("maxMana", maxMana);
			info.AddValue("currentMana", currentMana);
			info.AddValue("stamina", stamina);
			info.AddValue("intellect", intellect);
			info.AddValue("spirit", spirit);
			info.AddValue("spellPower", spellPower);
			info.AddValue("shadowPower", shadowPower);
			info.AddValue("firePower", firePower);
			info.AddValue("spellCritRating", spellCritRating);
			info.AddValue("spellHitRating", spellHitRating);
			info.AddValue("spellHasteRating", spellHasteRating);
			info.AddValue("mp5", mp5);
			info.AddValue("spellPenetration", spellPenetration);
			info.AddValue("damageModifier", damageModifier);
			info.AddValue("shadowModifier", shadowModifier);
			info.AddValue("fireModifier", fireModifier);
			info.AddValue("staminaModifier", staminaModifier);
			info.AddValue("intellectModifier", intellectModifier);
			info.AddValue("spiritModifier", spiritModifier);
			info.AddValue("meleeCritRating", meleeCritRating);
			info.AddValue("meleeHitRating", meleeHitRating);
		}
	}
}
