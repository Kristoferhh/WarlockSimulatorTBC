using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Interfaces
{
	interface IEntity
	{
		void Initialize();
		void Tick(decimal time);
		void Reset();
		bool IsCrit(); // add spell type parameter
		bool IsHit(); // add spell type parameter
	}
}
