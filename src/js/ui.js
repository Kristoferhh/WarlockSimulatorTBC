// RAID BUFFS
for(let buff of Object.keys(auras.buffs)) {
	let b = auras.buffs[buff];
	let lowerBuffName = b.name.toLowerCase().split(' ').join('-');
	let raidBuffUl = $("#buff-list");
	localStorage[buff] = localStorage[buff] || false;

	raidBuffUl.append($("<li data-aura-type='buffs' data-checked='" + localStorage[buff] + "' name='" + buff + "' id='" + lowerBuffName + "' class='aura'><a href='https://tbc.wowhead.com/spell=" + b.id + "'><img alt='" + b.name + "' src='img/" + b.iconName + ".jpg'></a></li>"));

	// If the user already had the buff selected from a previous session then add the stats from it.
	if (localStorage[buff] === 'true') {
		modifyStatsFromAura(auras.buffs[buff], false);
	}
}

// DEBUFFS
for (let buff of Object.keys(auras.debuffs)) {
	let b = auras.debuffs[buff];
	let lowerBuffName = b.name.toLowerCase().split(' ').join('-');
	let debuffUl = $("#debuff-list");
	localStorage[buff] = localStorage[buff] || false;

	debuffUl.append($("<li data-aura-type='debuffs' data-checked='" + localStorage[buff] + "' name='" + buff + "' id='" + lowerBuffName + "' class='debuff aura'><a href='https://tbc.wowhead.com/spell=" + b.id + "'><img alt='" + b.name + "' src='img/" + b.iconName + ".jpg'></a></li>"));

	// Add stats from already enabled debuffs
	if (localStorage[buff] === 'true') {
		modifyStatsFromAura(auras.debuffs[buff], false);
	}
}

// CONSUMABLES
for (let consumable of Object.keys(auras.consumables)) {
	let c = auras.consumables[consumable];
	let lowerConsumableName = c.name.toLowerCase().split(' ').join('-');
	let consumableUl = $("#consumable-list");
	localStorage[consumable] = localStorage[consumable] || false;

	consumableUl.append($("<li data-aura-type='consumables' data-checked='" + localStorage[consumable] + "' name='" + consumable + "' id='" + lowerConsumableName + "' class='" + (c.stats ? "stats " : "") + (c.potion ? "potion " : "") + (c.battleElixir ? "battle-elixir " : "") + (c.guardianElixir ? "guardian-elixir " : "") + (c.weaponOil ? "weapon-oil " : "") + (c.foodBuff ? "food-buff " : "") + "consumable aura'><a href='https://tbc.wowhead.com/item=" + c.id + "'><img alt='" + c.name + "' src='img/" + c.iconName + ".jpg'></a></li>"));

	// Add stats from already enabled consumables
	if (localStorage[consumable] === 'true') {
		modifyStatsFromAura(auras.consumables[consumable], false);
	}
}

// When a buff/debuff/consumable is clicked
$(".aura").click(function() {
	let auraType = $(this).attr('data-aura-type');
	let auraName = $(this).attr('name');
	modifyStatsFromAura(auras[auraType][auraName], $(this).attr('data-checked') === 'true');

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

// User clicks on an item slot in the selection above the item table
$("#item-slot-selection-list li").click(function() {
	loadItemsBySlot($(this).attr('data-slot'));
});

// User clicks on an item in the item table
$("#item-selection-table tbody").on('click', 'tr', function() {
	let itemSlot = $(this).attr('data-slot');
	let itemName = $(this).attr('data-name');

	// Toggle the item's data-selected boolean.
	let equipped = $(this).attr('data-selected') == 'true';
	$(this).attr('data-selected', !equipped);

	// Check if the user already has an item equipped in this slot and unequip it if so
	if (localStorage['equipped' + itemSlot] !== null && localStorage['equipped' + itemSlot] !== itemName) {
		// Set the old item's data-selected value to false and remove the item's stats from the player.
		$('[data-name="' + localStorage['equipped' + itemSlot] +'"]').attr('data-selected', false);
		modifyStatsFromItem(localStorage['equipped' + itemSlot], itemSlot);
	}

	// Add or remove the stats from the item depending on whether the player has the item equipped or not.
	modifyStatsFromItem(itemName,$(this).attr('data-slot'));

	// Return false so that the user doesn't get redirected to wowhead's website when he clicks the item.
	return false;

	//todo implement code to automatically unequip 2h when equipping 1h/offhand and vice versa.
});

// User changes races in the simulation settings
$("#race-dropdown-list").change(function() {
	let oldRace = $(this).data("currentRace");
	let newRace = $(this).val();
	$(this).data("currentRace", newRace);

	// Remove the previous race's stats
	// If the old race was Gnome then reduce the intellect modifier by 5%
	if (oldRace == "gnome") {
		characterStats.intellectModifier /= 1.05;
	}
	for (let stat in raceStats[oldRace]) {
		if (characterStats.hasOwnProperty(stat)) {
			characterStats[stat] -= raceStats[oldRace][stat];
		}
	}

	// Add the new race's stats
	// If the new race is Gnome then increase intellect by 5%
	if (newRace == "gnome") {
		characterStats.intellectModifier *= 1.05;
	}
	for (let stat in raceStats[newRace]) {
		if (characterStats.hasOwnProperty(stat)) {
			characterStats[stat] += raceStats[newRace][stat];
		}
	}

	$("#race").text($("#race-dropdown-list").children("option:selected").text());
	refreshCharacterStats();
});

// Loads items into the item table
function loadItemsBySlot(itemSlot) {
	// Removes all current item rows
	$(".item-row").remove(); 
	let tableBody = $("#item-selection-table tbody");

	for (let item of Object.keys(items[itemSlot])) {
		let i = items[itemSlot][item];

		//todo change 'i.spellPower + i.onUseSpellPower' to instead be the average spell power gain from the on-use effect		
		tableBody.append("<tr data-slot='" + itemSlot + "' data-name='" + item + "' data-selected='" + (localStorage['equipped' + itemSlot] == item || localStorage['equipped' + itemSlot + '1'] == item || localStorage['equipped' + itemSlot + '2'] == item || 'false') + "' class='item-row' data-wowhead-id='" + i.id + "'><td><a href='https://tbc.wowhead.com/item=" + i.id + "'>" + i.name + "</a></td><td>" + i.source + "</td><td>" + (i.stamina || '') + "</td><td>" + (i.intellect || '') + "</td><td>" + (Math.round(i.spellPower + (i.onUseSpellPower * i.duration / i.cooldown)) || i.spellPower || Math.round(i.onUseSpellPower * i.duration / i.cooldown) || '') + "</td><td>" + (i.shadowPower || '') + "</td><td>" + (i.firePower || '') + "</td><td>" + (i.critRating || '') + "</td><td>" + (i.hitRating || '') + "</td><td>" + (i.mp5 || '') + "</td><td>" + (i.spellPen || '') + "</td><td>" + (localStorage[item + "Dps"] || '') + "</td></tr>")
	}
}

// Equips or unequips an item. If the player has the item equipped then unequip it, else equip it.
// loadingUnequippedItems is set to true when the user is loading the website and it needs to add the stats from the user's equipped items from previous sessions.
function modifyStatsFromItem(itemName, itemSlot, loadingEquippedItems = false) {
	let itemObj = items[itemSlot][itemName];
	let equipped = localStorage['equipped' + itemSlot] == itemName || false;

	// Loop through the stats on the item and add them to/remove them from the character's stats.
	for (let stat in itemObj) {
		// Check if the item property is a character stat such as stamina/spell power.
		if (characterStats.hasOwnProperty(stat)) {
			if (equipped && !loadingEquippedItems) {
				characterStats[stat] -= itemObj[stat];
				localStorage.removeItem('equipped' + itemSlot);
			} else {
				characterStats[stat] += itemObj[stat];
				localStorage['equipped' + itemSlot] = itemName;
			}
		}
	}

	refreshCharacterStats();
}

function modifyStatsFromAura(auraObject, checked) {
	for (let stat in auraObject) {
		if (characterStats.hasOwnProperty(stat)) {
			// Check if the buff is a modifier to know whether to add/subtract or multiply/divide the stat
			if (stat.toLowerCase().search("modifier") !== -1) {
				if (checked) {
					characterStats[stat] /= auraObject[stat];
				} else {
					characterStats[stat] *= auraObject[stat];
				}
			} else {
				if (checked) {
					characterStats[stat] -= auraObject[stat];
				} else {
					characterStats[stat] += auraObject[stat];
				}
			}
		}
	}

	refreshCharacterStats();
}