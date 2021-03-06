// RAID BUFFS
for(let buff of Object.keys(raidBuffs)) {
	let b = raidBuffs[buff];
	let lowerBuffName = b.name.toLowerCase().split(' ').join('-');
	let raidBuffUl = $("#raid-buff-list");
	localStorage[buff] = localStorage[buff] || false; // some problem with this

	raidBuffUl.append($("<li data-checked='" + localStorage[buff] + "' name='" + buff + "' id='" + lowerBuffName + "' class='aura'><a href='https://classic.wowhead.com/spell=" + b.id + "'><img alt='" + b.name + "' src='img/" + b.iconName + ".jpg'></a></li>"));
}

// DEBUFFS
for (let buff of Object.keys(debuffs)) {
	let b = debuffs[buff];
	let lowerBuffName = b.name.toLowerCase().split(' ').join('-');
	let debuffUl = $("#debuff-list");
	localStorage[buff] = localStorage[buff] || false;

	debuffUl.append($("<li data-checked='" + localStorage[buff] + "' name='" + buff + "' id='" + lowerBuffName + "' class='debuff aura'><a href='https://classic.wowhead.com/spell=" + b.id + "'><img alt='" + b.name + "' src='img/" + b.iconName + ".jpg'></a></li>"));
}

// CONSUMABLES
for (let consumable of Object.keys(consumables)) {
	let c = consumables[consumable];
	let lowerConsumableName = c.name.toLowerCase().split(' ').join('-');
	let consumableUl = $("#consumable-list");
	localStorage[consumable] = localStorage[consumable] || false;

	consumableUl.append($("<li data-checked='" + localStorage[consumable] + "' name='" + consumable + "' id='" + lowerConsumableName + "' class='" + (c.stats ? "stats " : "") + (c.potion ? "potion " : "") + (c.battleElixir ? "battle-elixir " : "") + (c.guardianElixir ? "guardian-elixir " : "") + (c.weaponOil ? "weapon-oil " : "") + (c.foodBuff ? "food-buff " : "") + "consumable aura'><a href='https://classic.wowhead.com/item=" + c.id + "'><img alt='" + c.name + "' src='img/" + c.iconName + ".jpg'></a></li>"));
}

// When a buff/debuff/consumable is clicked
$(".aura").click(function() {
	let checkedVal = $(this).attr('data-checked') == 'true';
	$(this).attr('data-checked', !checkedVal);
	localStorage[$(this).attr("name")] = !checkedVal;

	return false;
});

// Array of consumables whose clicks we want to track.
let consumableTypesToTrack = ['.weapon-oil', '.battle-elixir', '.guardian-elixir', '.food-buff', '.potion'];
// When a consumable is clicked, uncheck all other types of that consumable since we can only have one at a time (e.g. disable all other weapon oils if a weapon oil is clicked).
$(consumableTypesToTrack.join(',')).click(function(event) {
	let clickedConsumableName = $(this).attr("name");
	let consumableTypes = [];

	// Loop through the consumable types we're tracking and check if the consumable that got clicked has any of those consumables as a class.
	for (let i = 0; i < consumableTypesToTrack.length; i++) {
		if ($(this).hasClass(consumableTypesToTrack[i].substring(1))) {
			consumableTypes.push(consumableTypesToTrack[i]);
		}
	}

	// Loop through the consumable classes we found in the previous loop and uncheck all childs of those classes aside from the consumable that just got clicked
	$(consumableTypes.join(',')).each(function() {
		if ($(this).attr('name') !== clickedConsumableName) {
			$(this).attr('data-checked', false);
			localStorage[$(this).attr('name')] = false;
		}
	});

	refreshCharacterStats();
});

refreshCharacterStats();