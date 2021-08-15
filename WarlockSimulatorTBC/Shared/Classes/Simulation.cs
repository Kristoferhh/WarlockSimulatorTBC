using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using WarlockSimulatorTBC.Shared.Classes;
using System.Diagnostics;
using System.Runtime.Serialization;
using System.Text.Json;
using BlazorWorker.WorkerCore;

namespace WarlockSimulatorTBC.Shared
{
	public class Simulation
	{
		private readonly IWorkerMessageService _messageService;
		private Stopwatch _timer;
		private double simUpdateNum;
		private int[] randomSeeds;
        private int startingIteration;
        private int iterationAmount;
		public Player player;
		public int iterations;
		public int minTime;
		public int maxTime;
		public int? itemId;
		public string simulationType;

		private Stopwatch _passTimeTimer = new Stopwatch();

		public static SimulationSettings GetSettings()
		{ 
			return new SimulationSettings
			{
				iterations = int.Parse(SimulationSettings.settings["iterations"]),
				minTime = int.Parse(SimulationSettings.settings["min-fight-length"]),
				maxTime = int.Parse(SimulationSettings.settings["max-fight-length"])
            };
		}

		public Simulation(IWorkerMessageService messageService)
		{
			_messageService = messageService;
		}

		public void Constructor(string simulationSettings, string playerSettings, string simulationType, int? itemId, int[] randomSeeds, int startingIteration, int iterationAmount)
		{
			player = new Player(JsonSerializer.Deserialize<PlayerSettings>(playerSettings));
			SimulationSettings simSettings = JsonSerializer.Deserialize<SimulationSettings>(simulationSettings);
			iterations = simSettings.iterations;
			minTime = simSettings.minTime;
			maxTime = simSettings.maxTime;
			simUpdateNum = Math.Floor(iterations / 100.0);
			this.simulationType = simulationType;
			this.itemId = itemId;
			this.randomSeeds = randomSeeds;
            this.startingIteration = startingIteration;
            this.iterationAmount = iterationAmount;
		}

		private double PassTime()
		{
			_passTimeTimer.Start();
			double time = player.castTimeRemaining;
			if (time == 0)
			{
				time = player.gcdRemaining;
			}

			// Tick the spells and auras
			player.CombatLog("Forwarding by: " + time + " seconds");
			player.currentFightTime += time;
			player.castTimeRemaining -= time;
			player.gcdRemaining -= time;

			if (player.spells.ContainsKey("shadowBolt") && player.spells["shadowBolt"].casting) player.spells["shadowBolt"].Tick(time);

			player.mp5Timer -= time;
			if (player.mp5Timer <= 0)
			{
				player.mp5Timer = 5;
				player.stats.currentMana += player.stats.mp5;
			}

			_passTimeTimer.Stop();
			return time;
		}

		public void Start()
		{
            List<double> damageResults = new List<double>();
			double minDps = 999999;
			double maxDps = 0;
			player.Initialize();
			_timer = new Stopwatch();
			_timer.Start();

			for (player.iteration = startingIteration; player.iteration < (startingIteration + iterationAmount); player.iteration++)
			{
                // Create a new Random object with a pre-determined (but random) seed from the randomSeeds array.
                player.rand = new Random(randomSeeds[player.iteration]);
                player.Reset();
				player.iterationDamage = 0;
				player.currentFightTime = 0;
				int fightLength = player.rand.Next(minTime, maxTime);

				while (player.currentFightTime < fightLength)
				{
					// Player
					if (player.castTimeRemaining <= 0)
					{
						// Spells on the GCD
						if (player.gcdRemaining <= 0)
						{
							double timeRemaining = fightLength - player.currentFightTime;

							if (player.spells["shadowBolt"].Ready())
							{
								player.CombatLog("Choosing Shadow Bolt");
								player.spells["shadowBolt"].StartCast();
							}
							else if (player.spells["lifeTap"].Ready())
							{
								player.CombatLog("Choosing Life Tap");
								player.spells["lifeTap"].StartCast();
							}
						}
					}

					if (PassTime() == 0)
					{
						throw new Exception("The simulation got stuck in an endless loop. If you'd like to help with fixing this bug then please export your current settings and send it to Kristofer#8003 on Discord.");
					}
				}

                double iterationDps = player.iterationDamage / fightLength;
                damageResults.Add(iterationDps);
				maxDps = Math.Max(maxDps, iterationDps);
				minDps = Math.Min(minDps, iterationDps);

				// Send a simulation update for every 1% of progress
				if (player.iteration % simUpdateNum == 0)
				{
					SendSimulationUpdate(MessageType.SimulationUpdate, minDps, maxDps, damageResults);
				}
			}

			_timer.Stop();
			//Console.WriteLine("PassTime(): " + _passTimeTimer.Elapsed);
			SendSimulationUpdate(MessageType.SimulationEnd, minDps, maxDps, damageResults);
		}

		private void SendSimulationUpdate(string msgType, double minDps, double maxDps, List<double> damageResults)
		{
			SimulationUpdate msg = new SimulationUpdate
			{
				SimulationType = simulationType,
				SimulationProgress = (int)Math.Ceiling(((double)player.iteration - startingIteration) / iterationAmount * 100),
				MinDps = minDps,
				MaxDps = maxDps,
                DamageResults = damageResults,
				TotalFightDuration = player.totalFightDuration,
				SimulationLength = _timer.Elapsed.TotalSeconds,
				ItemId = itemId,
                StartingIteration = startingIteration
			};
			_messageService.PostMessageAsync(msgType + "-;" + JsonSerializer.Serialize(msg));
		}
	}
}
