using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	[KnownType("GetSettings")]
	[DataContract]
	public class PlayerSettings
	{
		[DataMember]
		public string shattrathFaction { get; set; }
		[DataMember]
		public bool exaltedWithShattrathFaction { get; set; }
		[DataMember]
		public CharacterStats stats { get; set; }
		[DataMember]
		public Dictionary<string, int> talents { get; set; }

	}
}
