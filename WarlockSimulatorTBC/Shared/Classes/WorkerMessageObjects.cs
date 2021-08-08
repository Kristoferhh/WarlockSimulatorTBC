using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public class SimulationUpdate
	{
		public uint simulationProgress { get; set; }
		public double minDps { get; set; }
		public double maxDps { get; set; }
		public double totalDamage { get; set; }
		public double totalFightDuration { get; set; }
		public double simulationLength { get; set; }
	}
}
