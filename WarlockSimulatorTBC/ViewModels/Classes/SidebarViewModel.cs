using Microsoft.Toolkit.Mvvm.ComponentModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using WarlockSimulatorTBC.Shared;
using Blazored.LocalStorage;
using BlazorWorker.Core;
using WarlockSimulatorTBC.Shared.Classes;
using WarlockSimulatorTBC.Shared.Classes.UI;
using System.Text.Json;
using BlazorWorker.BackgroundServiceFactory;
using WarlockSimulatorTBC.ViewModels.Interfaces;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using Microsoft.AspNetCore.Components;
using Microsoft.JSInterop;

namespace WarlockSimulatorTBC.ViewModels.Classes
{
    public class SidebarViewModel : BaseViewModel, ISidebarViewModel
    {
        private ILocalStorageService _localStorage;
        private IWorkerFactory _workerFactory;
        private IJSRuntime _jsRuntime;

        public SidebarViewModel(ILocalStorageService localStorage, IWorkerFactory workerFactory, IJSRuntime jsRuntime)
        {
            _localStorage = localStorage;
            _workerFactory = workerFactory;
            _jsRuntime = jsRuntime;
        }


        private bool _simIsActive = false;
        // Saves the ID of the item that was equipped when the user started a sim. Without this, the sidebar will stop updating if the user selects another item while the sim is still running
        private int? _selectedItemIdWhenStartingSim;
        // Tracks information about multi-item sims. Key is the id of the item equipped in the sim and the value is the sim's progress %
        private Dictionary<int?, int> _multiItemSimInformation = new();
        // Tracks information about the recorded DPS results of a normal sim's threads. The key is the thread's startingIteration and the value is a List<> of the thread's recorded DPS values.
        private Dictionary<int, List<double>> _multiThreadSimDpsResults = new();
        // Tracks information about the simulation progress of a normal sim's threads. The key is the thread's startingIteration and the value is the thread's progress %
        private Dictionary<int, int> _multiThreadSimProgress = new();
        private DateTime? normalSimStart = null;
        private EventCallback RefreshItemTable;

        private string _medianDps;
        public string MedianDps
        {
            get => _medianDps;
            set => SetProperty(ref _medianDps, value);
        }

        private string _minDps;
        public string MinDps
        {
            get => _minDps;
            set => SetProperty(ref _minDps, value);
        }

        private string _maxDps;
        public string MaxDps
        {
            get => _maxDps;
            set => SetProperty(ref _maxDps, value);
        }

        private string _simulationDuration;
        public string SimulationDuration
        {
            get => _simulationDuration;
            set => SetProperty(ref _simulationDuration, value);
        }

        private string _simulateButtonText = "Simulate";
        public string SimulateButtonText
        {
            get => _simulateButtonText;
            set => SetProperty(ref _simulateButtonText, value);
        }

        private string _multiSimButtonText = "Simulate All Items";
        public string MultiSimButtonText
        {
            get => _multiSimButtonText;
            set => SetProperty(ref _multiSimButtonText, value);
        }

        private int _simProgress = 0;
        public int SimProgress
        {
            get => _simProgress;
            set => SetProperty(ref _simProgress, value);
        }

        private int _multiSimProgress = 0;
        public int MultiSimProgress
        {
            get => _multiSimProgress;
            set => SetProperty(ref _multiSimProgress, value);
        }

        private double _health = 0;
        public double Health
        {
            get => _health;
            set => SetProperty(ref _health, value);
        }

        private double _mana = 0;
        public double Mana
        {
            get => _mana;
            set => SetProperty(ref _mana, value);
        }

        private double _stamina = 0;
        public double Stamina
        {
            get => _stamina;
            set => SetProperty(ref _stamina, value);
        }

        private double _intellect = 0;
        public double Intellect
        {
            get => _intellect;
            set => SetProperty(ref _intellect, value);
        }

        private double _spirit = 0;
        public double Spirit
        {
            get => _spirit;
            set => SetProperty(ref _spirit, value);
        }

        private double _spellPower = 0;
        public double SpellPower
        {
            get => _spellPower;
            set => SetProperty(ref _spellPower, value);
        }

        private double _shadowPower = 0;
        public double ShadowPower
        {
            get => _shadowPower;
            set => SetProperty(ref _shadowPower, value);
        }

        private double _firePower = 0;
        public double FirePower
        {
            get => _firePower;
            set => SetProperty(ref _firePower, value);
        }

        private double _spellCritRating = 0;
        public double SpellCritRating
        {
            get => _spellCritRating;
            set => SetProperty(ref _spellCritRating, value);
        }

        private double _spellHitRating = 0;
        public double SpellHitRating
        {
            get => _spellHitRating;
            set => SetProperty(ref _spellHitRating, value);
        }

        private double _spellHasteRating = 0;
        public double SpellHasteRating
        {
            get => _spellHasteRating;
            set => SetProperty(ref _spellHasteRating, value);
        }

        private double _mp5 = 0;
        public double MP5
        {
            get => _mp5;
            set => SetProperty(ref _mp5, value);
        }

        private double _shadowModifier = 0;
        public double ShadowModifier
        {
            get => _shadowModifier;
            set => SetProperty(ref _shadowModifier, value);
        }

        private double _fireModifier = 0;
        public double FireModifier
        {
            get => _fireModifier;
            set => SetProperty(ref _fireModifier, value);
        }

        private double _critPercent = 0;
        public double CritPercent
        {
            get => _critPercent;
            set => SetProperty(ref _critPercent, value);
        }

        private double _hitPercent = 0;
        public double HitPercent
        {
            get => _hitPercent;
            set => SetProperty(ref _hitPercent, value);
        }


        public async Task SimulateDps(string simulationType)
        {
            if (_simIsActive)
            {
                return;
            }

            // Create the player and simulation settings and serialize them into json strings
            PlayerSettings playerSettings = Player.GetSettings();
            SimulationSettings simSettings = Simulation.GetSettings();
            int? currentlyEquippedItemId = Items.SelectedItems[Items.SelectedItemSlot + Items.SelectedItemSubSlot];
            int[] randomSeeds = new int[simSettings.iterations];
            _selectedItemIdWhenStartingSim = currentlyEquippedItemId;
            // Can't use the maximum amount of web workers cause the website will just slow down too much.
            // Maybe implement an option in the UI for the user to select how many threads they want to use.
            int maxWebWorkers = Math.Min(4, await _jsRuntime.InvokeAsync<int>("getMaxWebWorkers"));
            int webWorkersInUse = 0;
            int iterationAmount = simSettings.iterations;

            // Creates an array of random seeds for the simulations to use. This is so that each iteration will use the same Random() object across all the simulations.
            for (int i = 0; i < simSettings.iterations; i++)
            {
                randomSeeds[i] = Guid.NewGuid().GetHashCode();
            }

            // Loop through all the items of the currently selected item slot
            foreach (var item in Items.itemSlots[Items.SelectedItemSlot].items)
            {
                // Start a new simulation if it's a multi-item sim or if the item is the currently equipped item or if there is no item equipped in this item slot.
                if (simulationType == SimulationType.AllItems || item.Key == currentlyEquippedItemId || currentlyEquippedItemId == null)
                {
                    // Create a new clone of the playerSettings object since we're modifying the stats
                    PlayerSettings newPlayerSettings = playerSettings.Clone();

                    // Remove the stats of the currently equipped item and add the stats of the new item if it's a multi-item sim and if it's not our currently equipped item
                    if (simulationType == SimulationType.AllItems && item.Key != currentlyEquippedItemId)
                    {
                        if (currentlyEquippedItemId != null)
                        {
                            newPlayerSettings.stats.ModifyStatsFromItem(Items.SelectedItemSlot, (int)currentlyEquippedItemId, "remove");
                        }
                        newPlayerSettings.stats.ModifyStatsFromItem(Items.SelectedItemSlot, item.Key, "add");
                    }

                    string playerString = JsonSerializer.Serialize(newPlayerSettings);
                    string simString = JsonSerializer.Serialize(simSettings);
                    int? itemId = simulationType == SimulationType.AllItems ? item.Key : currentlyEquippedItemId;

                    // If it's a normal sim then split the simulation up into multiple threads to speed it up.
                    if (simulationType == SimulationType.Normal)
                    {
                        int iterationsPerWebWorker = (int)Math.Floor((double)iterationAmount / (maxWebWorkers - webWorkersInUse));
                        int simulationsStarted = 0;

                        while (webWorkersInUse++ < maxWebWorkers)
                        {
                            int startingIteration = iterationsPerWebWorker * simulationsStarted;

                            StartSimulation(simString, playerString, simulationType, itemId, randomSeeds, startingIteration, iterationsPerWebWorker);
                            _multiThreadSimDpsResults.Add(startingIteration, new List<double>());
                            _multiThreadSimProgress.Add(startingIteration, 0);
                            simulationsStarted++;
                        }
                    }
                    // If it's a multi-item sim then just start a normal sim for each item since we'll be using all available threads to run multiple items at once.
                    else if (simulationType == SimulationType.AllItems)
                    {
                        StartSimulation(simString, playerString, simulationType, itemId, randomSeeds, 0, simSettings.iterations);
                    }

                    // If there is no item equipped in this slot and it's a normal sim then break out of the loop since the current if-statement will always be true for each item.
                    if (currentlyEquippedItemId == null && simulationType == SimulationType.Normal)
                    {
                        break;
                    }
                }
            }
        }

        // The startingIteration and iterationAmount parameters are just for multi-threading normal sims. For multi-item sims, startingIteration is just set to 0 and iterationAmount is the total iteration amount.
        private async void StartSimulation(string simulationSettings, string playerSettings, string simulationType, int? itemId, int[] randomSeeds, int startingIteration, int iterationAmount)
        {
            var webWorker = await _workerFactory.CreateAsync();
            webWorker.IncomingMessage += OnWorkerMessage;

            var service = await webWorker.CreateBackgroundServiceAsync<Simulation>(
                options => options
                    .AddConventionalAssemblyOfService()
                    .AddAssemblies("System.Text.Json.dll", "System.Text.Encodings.Web.dll")
            );
            await service.RunAsync(s => s.Constructor(simulationSettings, playerSettings, simulationType, itemId, randomSeeds, startingIteration, iterationAmount));
            _simIsActive = true;
            if (simulationType == SimulationType.AllItems)
            {
                _multiItemSimInformation.Add((int)itemId, 0);
            }
            else if (simulationType == SimulationType.Normal)
            {
                if (normalSimStart == null)
                {
                    normalSimStart = DateTime.UtcNow;
                }
            }
            await service.RunAsync(s => s.Start());
        }

        public async void OnWorkerMessage(object sender, string message)
        {
            string[] messages = message.Split("-;");

            if (messages.Length >= 2)
            {
                try
                {
                    // First index is the type of message and the second index is the message itself
                    SimulationUpdate msg = JsonSerializer.Deserialize<SimulationUpdate>(messages[1].Trim());
                    string msgMedianDps;

                    if (msg.SimulationType == SimulationType.Normal)
                    {
                        // Update the dps list and sim progress for this thread
                        _multiThreadSimDpsResults[msg.StartingIteration] = msg.DamageResults;
                        _multiThreadSimProgress[msg.StartingIteration] = msg.SimulationProgress;

                        // Find the median dps
                        // Create a new List<> and for each thread, loop through the dps results and get the median and add it to the list.
                        List<double> medianDpsResults = new();
                        foreach (var sim in _multiThreadSimDpsResults)
                        {
                            medianDpsResults.Add(msg.DamageResults.OrderBy(x => x).Skip(msg.DamageResults.Count() / 2).First());
                        }

                        // After all the threads' median dps has been found, find the median value of that list to get the median dps across all threads.
                        msgMedianDps = Math.Round(medianDpsResults.OrderBy(x => x).Skip(medianDpsResults.Count() / 2).First(), 2).ToString();
                    }
                    // If it's not a normal sim then just get the median dps fo the DamageResults list from the message.
                    else
                    {
                        msgMedianDps = Math.Round(msg.DamageResults.OrderBy(x => x).Skip(msg.DamageResults.Count() / 2).First(), 2).ToString();
                    }

                    if (msg.ItemId == null || msg.ItemId == _selectedItemIdWhenStartingSim)
                    {
                        MedianDps = msgMedianDps;
                        MinDps = Math.Round(msg.MinDps, 2).ToString();
                        MaxDps = Math.Round(msg.MaxDps, 2).ToString();
                    }

                    if (message.StartsWith(MessageType.SimulationEnd))
                    {
                        _selectedItemIdWhenStartingSim = null;
                        _simIsActive = false;
                    }

                    if (msg.SimulationType == SimulationType.Normal)
                    {
                        // Find the average thread's sim progress and use that for the button text.
                        int averageSimProgress = (int)_multiThreadSimProgress.Average(x => x.Value);
                        if (averageSimProgress < 100)
                        {
                            SimProgress = averageSimProgress;
                            SimulateButtonText = SimProgress + "%";
                        }
                        else
                        {
                            SimProgress = 0;
                            SimulateButtonText = "Simulate";
                            _simIsActive = false;
                            // Clear the multi-thread lists at the end of a normal simulation.
                            _multiThreadSimDpsResults.Clear();
                            _multiThreadSimProgress.Clear();
                        }

                        // Since each thread will send a SimulationEnd update when they finish, we need to only run this code-block when all threads have finished running.
                        if (message.StartsWith(MessageType.SimulationEnd) && averageSimProgress >= 100)
                        {
                            SimulationDuration = Math.Round(DateTime.UtcNow.Subtract((DateTime)normalSimStart).TotalSeconds, 2).ToString();
                            normalSimStart = null;
                            await _localStorage.SetItemAsync("medianDps", MedianDps);
                            await _localStorage.SetItemAsync("minDps", MinDps);
                            await _localStorage.SetItemAsync("maxDps", MaxDps);
                            await _localStorage.SetItemAsync("simulationDuration", SimulationDuration);
                        }
                    }
                    else if (msg.SimulationType == SimulationType.AllItems && Items.savedItemDps[Items.SelectedItemSlot + Items.SelectedItemSubSlot].ContainsKey(msg.ItemId.ToString()))
                    {
                        _multiItemSimInformation[msg.ItemId] = msg.SimulationProgress;
                        MultiSimProgress = (int)Math.Ceiling(_multiItemSimInformation.Values.Average());
                        Items.savedItemDps[Items.SelectedItemSlot + Items.SelectedItemSubSlot][msg.ItemId.ToString()] = Convert.ToDouble(msgMedianDps);

                        if (MultiSimProgress < 100)
                        {
                            MultiSimButtonText = MultiSimProgress + "%";
                        }
                        else
                        {
                            MultiSimProgress = 0;
                            MultiSimButtonText = "Simulate All Items";
                            _multiItemSimInformation.Clear();
                        }

                        if (message.StartsWith(MessageType.SimulationEnd))
                        {
                            await _localStorage.SetItemAsync("savedItemDps", Items.savedItemDps);
                        }

                        await RefreshItemTable.InvokeAsync();
                    }
                }
                catch
                {
                    Console.WriteLine("Failed to read msg: " + message);
                }
            }
        }

        public void RefreshStats()
        {
            ShadowModifier = Stats.playerStats.shadowModifier;
            FireModifier = Stats.playerStats.fireModifier;
            double staminaModifier = Stats.playerStats.staminaModifier;

            // Crit
            SpellCritRating = Stats.playerStats.spellCritRating;
            if (AuraGroups.SelectedAuras.Contains("powerOfTheGuardianMage")) SpellCritRating += 28;
            CritPercent = SpellCritRating / Stats.critRatingPerPercent + Stats.baseCritChance + (Stats.playerStats.intellect * Stats.playerStats.intellectModifier * Stats.critPercentPerInt);
            if (TalentTree.CurrentTalents.ContainsKey("devastation")) CritPercent += TalentTree.CurrentTalents["devastation"];
            if (TalentTree.CurrentTalents.ContainsKey("backlash")) CritPercent += TalentTree.CurrentTalents["backlash"];
            if (TalentTree.CurrentTalents.ContainsKey("demonicTactics")) CritPercent += TalentTree.CurrentTalents["demonicTactics"];
            if (AuraGroups.SelectedAuras.Contains("totemOfWrath")) CritPercent += 3;
            if (AuraGroups.SelectedAuras.Contains("chainOfTheTwilightOwl")) CritPercent += 2;
            if (AuraGroups.SelectedAuras.Contains("moonkinAura")) CritPercent += 5;
            if (AuraGroups.SelectedAuras.Contains("judgementOfTheCrusader")) CritPercent += 3;

            // Hit
            SpellHitRating = Stats.playerStats.spellHitRating;
            HitPercent = SpellHitRating / Stats.hitRatingPerPercent;
            if (AuraGroups.SelectedAuras.Contains("inspiringPresence")) HitPercent++;
            if (AuraGroups.SelectedAuras.Contains("totemOfWrath")) HitPercent += 3;

            // Spell Power
            SpellPower = Stats.playerStats.spellPower;
            if (AuraGroups.SelectedAuras.Contains("felArmor") && TalentTree.CurrentTalents.ContainsKey("demonicAegis")) SpellPower += 10 * TalentTree.CurrentTalents["demonicAegis"];
            if (AuraGroups.SelectedAuras.Contains("powerOfTheGuardianWarlock")) SpellPower += 33;

            // Shadow/Fire % Modifiers
            if (TalentTree.CurrentTalents.ContainsKey("shadowMastery")) ShadowModifier *= (1 + (TalentTree.CurrentTalents["shadowMastery"] * 2.0) / 100);
            // add master demonologist
            if (TalentTree.CurrentTalents.ContainsKey("soulLink")) ShadowModifier *= (1 + (TalentTree.CurrentTalents["soulLink"] * 5.0) / 100);
            if (TalentTree.CurrentTalents.ContainsKey("emberstorm")) FireModifier *= (1 + (TalentTree.CurrentTalents["emberstorm"] * 2.0) / 100);

            // Stamina Modifiers
            if (TalentTree.CurrentTalents.ContainsKey("demonicEmbrace")) staminaModifier *= (1 + (TalentTree.CurrentTalents["demonicEmbrace"] * 3.0) / 100);
            Stamina = Stats.playerStats.stamina * staminaModifier;

            Health = Stats.playerStats.health + Stamina * Stats.healthPerStamina;
            Intellect = Stats.playerStats.intellect * Stats.playerStats.intellectModifier;
            Spirit = Stats.playerStats.spirit * Stats.playerStats.spiritModifier;
            ShadowPower = Stats.playerStats.shadowPower;
            FirePower = Stats.playerStats.firePower;
            SpellHasteRating = Stats.playerStats.spellHasteRating;
            MP5 = Stats.playerStats.mp5;

            // Mana
            Stats.playerStats.maxMana = Stats.playerStats.intellect * Stats.playerStats.intellectModifier * Stats.manaPerInt;
            Mana = Stats.playerStats.maxMana;
        }


        public async Task InitializeViewModel(EventCallback refreshItemTable)
        {
            if (await _localStorage.ContainKeyAsync("medianDps"))
            {
                _medianDps = await _localStorage.GetItemAsync<string>("medianDps");
            }
            if (await _localStorage.ContainKeyAsync("minDps"))
            {
                _minDps = await _localStorage.GetItemAsync<string>("minDps");
            }
            if (await _localStorage.ContainKeyAsync("maxDps"))
            {
                _maxDps = await _localStorage.GetItemAsync<string>("maxDps");
            }
            if (await _localStorage.ContainKeyAsync("simulationDuration"))
            {
                _simulationDuration = await _localStorage.GetItemAsync<string>("simulationDuration");
            }
            RefreshItemTable = refreshItemTable;
        }
    }
}