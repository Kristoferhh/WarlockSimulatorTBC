using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using WarlockSimulatorTBC.Shared.Classes.UI;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public static class ExtensionMethods
	{
		//todo: optimize
		public static void ModifyStatsFromItem(this CharacterStats stats, string itemSlot, int itemId, string action)
		{
			if (action == "add")
			{
				stats.stamina += Items.itemSlots[itemSlot].items[itemId].stamina;
				stats.intellect += Items.itemSlots[itemSlot].items[itemId].intellect;
				stats.spirit += Items.itemSlots[itemSlot].items[itemId].spirit;
				stats.spellPower += Items.itemSlots[itemSlot].items[itemId].spellPower;
				stats.shadowPower += Items.itemSlots[itemSlot].items[itemId].shadowPower;
				stats.firePower += Items.itemSlots[itemSlot].items[itemId].firePower;
				stats.spellCritRating += Items.itemSlots[itemSlot].items[itemId].spellCritRating;
				stats.spellHitRating += Items.itemSlots[itemSlot].items[itemId].spellHitRating;
				stats.spellHasteRating += Items.itemSlots[itemSlot].items[itemId].spellHasteRating;
				stats.mp5 += Items.itemSlots[itemSlot].items[itemId].mp5;
				stats.spellPenetration += Items.itemSlots[itemSlot].items[itemId].spellPenetration;
			}
			else if (action == "remove")
			{
				stats.stamina -= Items.itemSlots[itemSlot].items[itemId].stamina;
				stats.intellect -= Items.itemSlots[itemSlot].items[itemId].intellect;
				stats.spirit -= Items.itemSlots[itemSlot].items[itemId].spirit;
				stats.spellPower -= Items.itemSlots[itemSlot].items[itemId].spellPower;
				stats.shadowPower -= Items.itemSlots[itemSlot].items[itemId].shadowPower;
				stats.firePower -= Items.itemSlots[itemSlot].items[itemId].firePower;
				stats.spellCritRating -= Items.itemSlots[itemSlot].items[itemId].spellCritRating;
				stats.spellHitRating -= Items.itemSlots[itemSlot].items[itemId].spellHitRating;
				stats.spellHasteRating -= Items.itemSlots[itemSlot].items[itemId].spellHasteRating;
				stats.mp5 -= Items.itemSlots[itemSlot].items[itemId].mp5;
				stats.spellPenetration -= Items.itemSlots[itemSlot].items[itemId].spellPenetration;
			}
		}

		public static T Clone<T>(this T source)
		{
			var serialized = JsonConvert.SerializeObject(source);
			return JsonConvert.DeserializeObject<T>(serialized);
		}
	}
}
