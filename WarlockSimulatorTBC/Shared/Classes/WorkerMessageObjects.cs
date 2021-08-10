using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public static class SimulationType
	{
		public static string Normal = "normal";
		public static string AllItems = "allItems";
		public static string StatWeights = "statWeights";
	}

	public static class MessageType
	{
		public static string SimulationUpdate = "SimulationUpdate";
		public static string SimulationEnd = "SimulationEnd";
		public static string MuliItemSimulationUpdate = "MultiItemSimulationUpdate";
		public static string MultiItemSimulationEnd = "MultiItemSimulationEnd";
	}

	public class SimulationUpdate
	{
		public string simulationType { get; set; }
		public uint simulationProgress { get; set; }
		public int? itemId { get; set; }
		public double minDps { get; set; }
		public double maxDps { get; set; }
		public double totalDamage { get; set; }
		public double totalFightDuration { get; set; }
		public double simulationLength { get; set; }
	}
}
