using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public class SimulationSettings
	{
		public int iterations { get; set; }
		public int minTime { get; set; }
		public int maxTime { get; set; }
	}
}
