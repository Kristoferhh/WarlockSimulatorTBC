using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public static class MessageType
	{
		public static string SimulationUpdate = "SimulationUpdate";
		public static string SimulationEnd = "SimulationEnd";
	}

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
