using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	[KnownType("SimulationSettings")]
	[DataContract]
	public class SimulationSettings
	{
		public int iterations;
		public int minTime;
		public int maxTime;
	}
}
