using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public class SimulationSettings
	{
        // The settings need to be initialized here to their default values. Otherwise there will be an error on startup because the binds won't find the keys in the empty dictionary.
        public static Dictionary<string, string> settings = new Dictionary<string, string>()
        {
            //todo: fix the inconsistent key names (imported from before Blazor when using Javascript)
            { "race", "gnome" },
            { "rotationOption", "simChooses" },
            { "iterations", "1000" },
            { "min-fight-length", "150" },
            { "max-fight-length", "210" },
            { "target-level", "73" },
            { "target-shadow-resistance", "0" },
            { "target-fire-resistance", "0" },
            { "automatically-open-sim-details", "no" },
            { "randomizeValues", "no" },
            { "infinitePlayerMana", "no" },
            { "infinitePetMana", "no" },
            { "petChoice", "0" },
            { "sacrificePet", "no" },
            { "petMode", "0" },
            { "prepopBlackBook", "no" },
            { "shattrathFaction", "Aldor" },
            { "shattrathFactionReputation", "no" },
            { "lashOfPainUsage", "noISB" },
            { "enemyArmor", "7700" },
            { "improvedCurseOfTheElements", "no" },
            { "bloodlustAmount", "1" },
            { "improvedDivineSpirit", "no" },
            { "mageAtieshAmount", "1" },
            { "warlockAtieshAmount", "1" },
            { "totemOfWrathAmount", "1" },
            { "shadowPriestDps", "1000" },
            { "improvedFaerieFire", "no" },
            { "improvedExposeArmor", "no" },
            { "survivalHunterAgility", "1000" },
            { "exposeWeaknessUptime", "90" },
            { "customIsbUptime", "yes" },
            { "customIsbUptimeValue", "70" }
        };

        public int iterations { get; set; }
		public int minTime { get; set; }
		public int maxTime { get; set; }
	}
}
