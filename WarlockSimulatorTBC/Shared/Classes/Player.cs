using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using WarlockSimulatorTBC.Shared.Interfaces;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public class Player : IEntity
	{
		int level = 70;
		double gcdValue = 1.5;
		double minimumGcdValue = 1.0;
		double critMultiplier = 1.5;
		double totalManaRegenerated = 0;
		public decimal castTimeRemaining = decimal.Zero;
		public decimal gcdRemaining = decimal.Zero;
		decimal mp5Timer = Convert.ToDecimal(5);
		string shattrathFaction = null;
		bool exaltedWithShattrathFaction = false;
		List<string> combatLog = new List<string>();

		public Player(Settings settings)
		{
			shattrathFaction = settings.shattrathFaction;
			exaltedWithShattrathFaction = settings.exaltedWithShattrathFaction;
			// The amount to increase spell cast times by.
			// This will not have any actual effect on the dps result because of how small the value is, but it will make things a lot more realistic
			// because cast times, dot ticks, and such will fall out of sync with each other (which is what happens when a real player is playing)
			// and it will make it easier to, for example, pre-cast Immolate to reapply it right after it expires.
			// If Immolate has 1.5 seconds remaining, the sim won't start casting immolate because it would refresh before it expires
			// so Immolate would need to have ~1.49999s left of its duration for it to start casting.
			// But with this delay, Immolate's cast time would be ~1.500001 which would allow it to reapply Immolate when it has 1.5 seconds left
			// This should solve that problem if I'm thinking through this correctly.
			decimal spellDelay = Convert.ToDecimal(0.0001);
		}

		public void Initialize()
		{

		}

		public void Reset()
		{

		}

		public void Tick(decimal time)
		{

		}

		public bool IsCrit()
		{
			return false;
		}

		public bool IsHit()
		{
			return false;
		}
	}
}
