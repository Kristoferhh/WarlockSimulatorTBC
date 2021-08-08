using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	[DataContract]
	[KnownType(typeof(SimulationSettings))]
	public class SimulationSettings// : ISerializable
	{
		[DataMember] public int iterations { get; set; }
		[DataMember] public int minTime { get; set; }
		[DataMember] public int maxTime { get; set; }

		/*public SimulationSettings()
		{
			Console.WriteLine("SimulationSettings()");
		}

		public SimulationSettings(SerializationInfo info, StreamingContext context)
		{
			Console.WriteLine("SimulationSettings(info, context)");
			iterations = (int)info.GetValue(nameof(iterations), typeof(int));
			minTime = (int)info.GetValue(nameof(minTime), typeof(int));
			maxTime = (int)info.GetValue(nameof(maxTime), typeof(int));
			Console.WriteLine("SimulationSettings finished");
		}

		public void GetObjectData(SerializationInfo info, StreamingContext context)
		{
			Console.WriteLine("Attempting to get object data");
			info.AddValue(nameof(iterations), iterations);
			info.AddValue(nameof(minTime), minTime);
			info.AddValue(nameof(maxTime), maxTime);
			Console.WriteLine("Got object data");
		}*/
	}
}
