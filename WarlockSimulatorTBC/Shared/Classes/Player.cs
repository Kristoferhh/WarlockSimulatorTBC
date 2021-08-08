using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using WarlockSimulatorTBC.Shared.Interfaces;
using WarlockSimulatorTBC.Shared.Classes.UI;
using System.Runtime.Serialization;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public class Player : IEntity
	{
		public CharacterStats stats;
		int level = 70;
		double gcdValue = 1.5;
		double minimumGcdValue = 1.0;
		double critMultiplier = 1.5;
		double totalManaRegenerated = 0;
		public decimal castTimeRemaining = decimal.Zero;
		public decimal gcdRemaining = decimal.Zero;
		public decimal mp5Timer = Convert.ToDecimal(5);
		public string shattrathFaction;
		bool exaltedWithShattrathFaction;
		List<string> combatLog = new List<string>();
		public Dictionary<string, Spell> spells = new Dictionary<string, Spell>();
		public Dictionary<string, int> talents;
		// The amount to increase spell cast times by.
		// This will not have any actual effect on the dps result because of how small the value is, but it will make things a lot more realistic
		// because cast times, dot ticks, and such will fall out of sync with each other (which is what happens when a real player is playing)
		// and it will make it easier to, for example, pre-cast Immolate to reapply it right after it expires.
		// If Immolate has 1.5 seconds remaining, the sim won't start casting immolate because it would refresh before it expires
		// so Immolate would need to have ~1.49999s left of its duration for it to start casting.
		// But with this delay, Immolate's cast time would be ~1.500001 which would allow it to reapply Immolate when it has 1.5 seconds left
		// This should solve that problem if I'm thinking through this correctly.
		public decimal spellDelay = Convert.ToDecimal(0.0001);

		// Simulation variables
		public int totalFightDuration = 0;
		public int iteration;
		public double iterationDamage;
		public decimal currentFightTime;

		public static PlayerSettings GetSettings()
		{
			return new PlayerSettings
			{
				shattrathFaction = "aldor",
				exaltedWithShattrathFaction = true,
				stats = Stats.playerStats,
				talents = TalentTree.CurrentTalents
			};
		}

		public Player(PlayerSettings settings)
		{
			shattrathFaction = settings.shattrathFaction;
			exaltedWithShattrathFaction = settings.exaltedWithShattrathFaction;
			stats = settings.stats;
			talents = settings.talents;
		}

		public void Initialize()
		{
			foreach (var talent in talents)
			{
				Console.WriteLine(talent.Key + " - " + talent.Value);
			}
			Console.WriteLine("Talent amount: " + talents.Count);
			spells.Add("lifeTap", new LifeTap(this));
			spells.Add("shadowBolt", new ShadowBolt(this));
		}

		public void Reset()
		{
			gcdRemaining = 0;
			castTimeRemaining = 0;
			stats.currentMana = stats.maxMana;
			mp5Timer = Convert.ToDecimal(5);

			if (spells.ContainsKey("shadowBolt")) spells["shadowBolt"].Reset();
		}

		public decimal GetGcdValue()
		{
			return (decimal)gcdValue;
		}

		public bool IsCrit()
		{
			return false;
		}

		public bool IsHit()
		{
			return false;
		}

		public void CombatLog(string msg)
		{
			if (iteration == 2)
			{
				Console.WriteLine(currentFightTime + ": " + msg);
			}
		}

		/*public Player(SerializationInfo info, StreamingContext context)
		{
			shattrathFaction = (string)info.GetValue("shattrathFaction", typeof(string));
			exaltedWithShattrathFaction = (bool)info.GetValue("exaltedWithShattrathFaction", typeof(bool));
			stats = (CharacterStats)info.GetValue("stats", typeof(CharacterStats));
			talents = (Dictionary<string, int>)info.GetValue("talents", typeof(Dictionary<string, int>));
		}

		public void GetObjectData(SerializationInfo info, StreamingContext context)
		{
			info.AddValue("shattrathFaction", shattrathFaction);
			info.AddValue("exaltedWithShattrathFaction", exaltedWithShattrathFaction);
			info.AddValue("stats", stats);
			info.AddValue("talents", talents);
		}*/
	}
}
