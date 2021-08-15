using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public static class SimulationType
	{
		public const string Normal = "normal";
		public const string AllItems = "allItems";
		public const string StatWeights = "statWeights";
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
		public string SimulationType { get; set; }
		public int SimulationProgress { get; set; }
		public int? ItemId { get; set; }
		public double MinDps { get; set; }
		public double MaxDps { get; set; }
        public List<double> DamageResults { get; set; }
		public double TotalFightDuration { get; set; }
		public double SimulationLength { get; set; }
        public int StartingIteration { get; set; }
	}
}
