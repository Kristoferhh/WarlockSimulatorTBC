using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	[KnownType(typeof(PlayerSettings))]
	[KnownType(typeof(Dictionary<string, int>))]
	//[Serializable()]
	[DataContract]
	public class PlayerSettings// : ISerializable
	{
		[DataMember]
		public string shattrathFaction { get; set; }
		[DataMember]
		public bool exaltedWithShattrathFaction { get; set; }
		[DataMember]
		public CharacterStats stats { get; set; }
		[DataMember]
		public Dictionary<string, int> talents { get; set; }

		/*public PlayerSettings() { }

		public void GetObjectData(SerializationInfo info, StreamingContext context)
		{
			info.AddValue("shattrathFaction", shattrathFaction);
			info.AddValue("exaltedWithShattrathFaction", exaltedWithShattrathFaction);
			info.AddValue("stats", stats);
			info.AddValue("talents", talents);
		}

		public PlayerSettings(SerializationInfo info, StreamingContext context)
		{
			shattrathFaction = (string)info.GetValue("shattrathFaction", typeof(string));
			exaltedWithShattrathFaction = (bool)info.GetValue("exaltedWithShattrathFaction", typeof(bool));
			stats = (CharacterStats)info.GetValue("stats", typeof(CharacterStats));
			talents = (Dictionary<string, int>)info.GetValue("talents", typeof(Dictionary<string, int>));
		}*/
	}
}
