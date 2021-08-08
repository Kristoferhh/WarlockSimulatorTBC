﻿using System;
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
		private Random rand = new Random();
		private double simUpdateNum;
		public Player player;
		public int iterations;
		public int minTime;
		public int maxTime;

		public static SimulationSettings GetSettings()
		{ 
			return new SimulationSettings
			{
				iterations = 1000,
				minTime = 150,
				maxTime = 210
			};
		}

		public Simulation(IWorkerMessageService messageService)
		{
			_messageService = messageService;
		}

		public void Constructor(string simulationSettings, string playerSettings)
		{
			player = new Player(JsonSerializer.Deserialize<PlayerSettings>(playerSettings));
			SimulationSettings simSettings = JsonSerializer.Deserialize<SimulationSettings>(simulationSettings);
			iterations = simSettings.iterations;
			minTime = simSettings.minTime;
			maxTime = simSettings.maxTime;
			simUpdateNum = Math.Floor(iterations / 100.0);
		}

		private decimal PassTime()
		{
			decimal time = player.castTimeRemaining;
			if (time == 0)
			{
				time = player.gcdRemaining;
			}

			// Tick the spells and auras
			player.CombatLog("Forwarding by: " + time + " seconds");
			player.currentFightTime += time;
			player.castTimeRemaining = Math.Max(0, player.castTimeRemaining - time);
			player.gcdRemaining = Math.Max(0, player.gcdRemaining - time);

			if (player.spells.ContainsKey("shadowBolt") && player.spells["shadowBolt"].casting) player.spells["shadowBolt"].Tick(time);

			player.mp5Timer = Math.Max(0, player.mp5Timer - time);
			if (player.mp5Timer <= 0)
			{
				player.mp5Timer = 5;
				player.stats.currentMana += player.stats.mp5;
			}

			return time;
		}

		public void Start()
		{
			double totalDamage = 0;
			double minDps = 999999;
			double maxDps = 0;
			player.Initialize();
			_timer = new Stopwatch();
			_timer.Start();

			for (player.iteration = 1; player.iteration < iterations; player.iteration++)
			{
				if (player.iteration == 1) _messageService.PostMessageAsync("{SimulationUpdate}(yep)");
				player.Reset();
				player.iterationDamage = 0;
				player.currentFightTime = 0;
				int fightLength = rand.Next(minTime, maxTime);

				while (player.currentFightTime < fightLength)
				{
					// Player
					if (player.castTimeRemaining <= 0)
					{
						// Spells on the GCD
						if (player.gcdRemaining <= 0)
						{
							decimal timeRemaining = fightLength - player.currentFightTime;

							if (player.spells["shadowBolt"].Ready())
							{
								player.CombatLog("Casting Shadow Bolt");
								player.spells["shadowBolt"].StartCast();
							}
							else if (player.spells["lifeTap"].Ready())
							{
								player.CombatLog("Casting Life Tap");
								player.spells["lifeTap"].StartCast();
							}
						}
					}

					if (PassTime() == 0)
					{
						throw new Exception("The simulation got stuck in an endless loop. If you'd like to help with fixing this bug then please export your current settings and send it to Kristofer#8003 on Discord.");
					}
				}

				player.totalFightDuration += fightLength;
				totalDamage += player.iterationDamage;

				double iterationDps = player.iterationDamage / fightLength;
				maxDps = Math.Max(maxDps, iterationDps);
				minDps = Math.Min(minDps, iterationDps);

				// Send a simulation update for every 1% of progress
				if (player.iteration % simUpdateNum == 0)
				{
					SendSimulationUpdate("SimulationUpdate", minDps, maxDps, totalDamage);
				}
			}

			_timer.Stop();
			SendSimulationUpdate("SimulationEnd", minDps, maxDps, totalDamage);
		}

		private void SendSimulationUpdate(string msgType, double minDps, double maxDps, double totalDamage)
		{
			SimulationUpdate msg = new SimulationUpdate
			{
				simulationProgress = (uint)Math.Ceiling((double)player.iteration / iterations * 100),
				minDps = minDps,
				maxDps = maxDps,
				totalDamage = totalDamage,
				totalFightDuration = player.totalFightDuration,
				simulationLength = _timer.Elapsed.TotalSeconds
			};
			_messageService.PostMessageAsync(msgType + "-;" + JsonSerializer.Serialize(msg));
		}
	}
}
