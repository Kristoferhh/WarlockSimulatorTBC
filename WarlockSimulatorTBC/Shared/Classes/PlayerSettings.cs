using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public class PlayerSettings
	{
		public string shattrathFaction { get; set; }
		public bool exaltedWithShattrathFaction { get; set; }
		public CharacterStats stats { get; set; }
		public Dictionary<string, int> talents { get; set; }
	}
}
