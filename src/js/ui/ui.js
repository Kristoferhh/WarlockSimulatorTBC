// Maximum amount of web workers that should be run concurrently
const maxWorkers =  navigator.hardwareConcurrency || 8;
// Object with all the gems the user has equipped on any item, including items that are not equipped. Each key is the item's name and the value is an array with the ID of the gems equipped in that item.
var selectedGems = localStorage['selectedGems'] ? JSON.parse(localStorage['selectedGems']) : {};
// Key: Item slot. Value: Equipped item's ID
var selectedItems = localStorage['selectedItems'] ? JSON.parse(localStorage['selectedItems']) : {};
// Key: Talent's name. Value: Talent's point amount
var talents = localStorage['talents'] ? JSON.parse(localStorage['talents']) : {};
// Key: Aura's name. Value: Boolean
var auras = localStorage['auras'] ? JSON.parse(localStorage['auras']) : {};
var rotation = localStorage['rotation'] ? JSON.parse(localStorage['rotation']) : {};
var selectedEnchants = localStorage['selectedEnchants'] ? JSON.parse(localStorage['selectedEnchants']) : {};
// Key: Item ID. Value: Item's saved DPS from previous simulations.
var savedItemDPS = localStorage['savedItemDPS'] ? JSON.parse(localStorage['savedItemDPS']) : {};
var settings = localStorage.settings ? JSON.parse(localStorage.settings) : {};

// RAID BUFFS
for(let buff of Object.keys(_auras.buffs)) {
	let b = _auras.buffs[buff];
	let lowerBuffName = b.name.toLowerCase().split(' ').join('-');
	let raidBuffUl = $("#buff-list");
	auras[buff] = auras[buff] || false;

	raidBuffUl.append($("<li data-aura-type='buffs' data-checked='" + auras[buff] + "' name='" + buff + "' id='" + lowerBuffName + "' class='aura'><a href='https://tbc.wowhead.com/spell=" + b.id + "'><img alt='" + b.name + "' src='img/" + b.iconName + ".jpg'></a></li>"));

	// If the user already had the buff selected from a previous session then add the stats from it.
	if (auras[buff] === true) {
		modifyStatsFromAura(_auras.buffs[buff], false);
	}
}

// DEBUFFS
for (let buff of Object.keys(_auras.debuffs)) {
	let b = _auras.debuffs[buff];
	let lowerBuffName = b.name.toLowerCase().split(' ').join('-');
	let debuffUl = $("#debuff-list");
	auras[buff] = auras[buff] || false;

	debuffUl.append($("<li data-aura-type='debuffs' data-checked='" + auras[buff] + "' name='" + buff + "' id='" + lowerBuffName + "' class='debuff aura'><a href='https://tbc.wowhead.com/spell=" + b.id + "'><img alt='" + b.name + "' src='img/" + b.iconName + ".jpg'></a></li>"));

	// Add stats from already enabled debuffs
	if (auras[buff] === true) {
		modifyStatsFromAura(_auras.debuffs[buff], false);
	}
}

// CONSUMABLES
for (let consumable of Object.keys(_auras.consumables)) {
	let c = _auras.consumables[consumable];
	let lowerConsumableName = c.name.toLowerCase().split(' ').join('-');
	let consumableUl = $("#consumable-list");
	auras[consumable] = auras[consumable] || false;

	consumableUl.append($("<li data-aura-type='consumables' data-checked='" + auras[consumable] + "' name='" + consumable + "' id='" + lowerConsumableName + "' class='" + (c.stats ? "stats " : "") + (c.potion ? "potion " : "") + (c.battleElixir ? "battle-elixir " : "") + (c.guardianElixir ? "guardian-elixir " : "") + (c.weaponOil ? "weapon-oil " : "") + (c.foodBuff ? "food-buff " : "") + "consumable aura'><a href='https://tbc.wowhead.com/item=" + c.id + "'><img alt='" + c.name + "' src='img/" + c.iconName + ".jpg'></a></li>"));

	// Add stats from already enabled consumables
	if (auras[consumable] === true) {
		modifyStatsFromAura(_auras.consumables[consumable], false);
	}
}

// Spell Selection
for (let type in _spellSelection) {
	let rotationList = $("#rotation-list");
	let str = "<div><h4>" + _spellSelection[type].header + "</h4>";
	for (let spell in _spellSelection[type].spells) {
		rotation[type] = rotation[type] || {};
		str += "<li data-type='" + type + "' data-name='" + spell + "' class='rotation-" + type + "' data-checked='" + (rotation[type][spell] || false) + "' id='" + type + "-" + spell + "'><a href=https://tbc.wowhead.com/spell=" + _spellSelection[type].spells[spell].id + "><img src='img/" + _spellSelection[type].spells[spell].iconName + ".jpg' alt='" + _spellSelection[type].spells[spell].name + "'></a></li>";
	}
	str += "</div>";
	rotationList.append(str);
}

// Add the talent trees
for (let tree in _talents) {
	if (_talents.hasOwnProperty(tree)) {
		$("#talents-section").append($("<div class='talent-tree-div'><table data-name='" + tree + "' background='img/talent_tree_background_" + tree + ".jpg' id='talent-table-" + tree + "' class='talent-tree-table'></table><div class='talent-tree-name'><h3 style='display: inline-block;' data-name='" + tree + "'>" + tree.charAt(0).toUpperCase() + tree.slice(1) + "</h3><span class='clear-talent-tree'>&#10060;</span></div></div>"));
		$("#talent-table-" + tree).append($("<tbody></tbody>"));
		$("#talent-table-" + tree).data('points', 0);
		$("#talent-table-" + tree + " tbody").append($("<tr class='" + tree + "-tree-row'></tr>"));
		let lastRow = $("#talent-table-" + tree + " tbody tr:last");
		let currentCol = 1;

		for (let talent in _talents[tree]) {
			let t = _talents[tree][talent];
			talents[talent] = talents[talent] || 0;
			talentPointsRemaining -= talents[talent];
			$("#talent-table-" + tree).data('points', $("#talent-table-" + tree).data('points') + talents[talent]);

			// Check if the current talent should be in the next row below and create a new row if true
			if (t.row > $("." + tree + "-tree-row").length) {
				$("#talent-table-" + tree + " tbody").append($("<tr class='" + tree + "-tree-row'></tr>"));
				lastRow = $("#talent-table-" + tree + " tbody tr:last");
				currentCol = 1;
			}

			// Create empty cells between talents if skipping a number (e.g. going from column 1 straight to column 4)
			while (currentCol < t.column) {
				lastRow.append($("<td></td>"));
				currentCol++;
			}

			lastRow.append($("<td><div data-maxpoints='" + t.rankIDs.length + "' data-points='" + talents[talent] + "' class='talent-icon' data-tree='" + tree + "' id='" + talent + "'><a href='https://classic.wowhead.com/spell=" + t.rankIDs[Math.max(0,talents[talent]-1)] + "'><img src='img/" + t.iconName + ".jpg' alt='" + t.name + "'><span id='" + talent + "-point-amount' class='talent-point-amount'>" + talents[talent] + "</span></a></div></td>"));
			
			// Check if the text displaying the talent point amount should be hidden or colored (for maxed out talents)
			let pointAmount = $("#" + talent + "-point-amount")
			if (pointAmount.text() <= 0) {
				pointAmount.hide();
			} else if (pointAmount.text() == t.rankIDs.length) {
				pointAmount.css("color","#ffcd45");
			} else {
				pointAmount.css("color","#7FFF00")
			}
			currentCol++;
		}

		updateTalentTreeNames();
	}
}

// Add in the pre-requisite talent arrows (todo: find a better way to do this)
/*$("#talent-table-affliction").append("<div data-row='3' data-column='3' class='talent-arrow'><img width='15' height='60' src='img/talent_arrow_down.jpg'></div>"); // Amplify Curse to Curse of Exhaustion
$("#talent-table-affliction").append("<div data-row='5' data-column='2' class='talent-arrow'><img width='15' height='10' src='img/talent_arrow_down.jpg'></div>"); // Siphon Life to Shadow Mastery
$("#talent-table-affliction").append("<div data-row='7' data-column='2' class='talent-arrow'><img width='15' height='60' src='img/talent_arrow_down.jpg'></div>"); // Contagion to Unstable Agony
$("#talent-table-demonology").append("<div data-row='3' data-column='2' class='talent-arrow'><img width='15' height='10' src='img/talent_arrow_down.jpg'></div>");*/ // Fel Domination to Master Summoner

// When a buff/debuff/consumable is clicked
$(".aura").click(function() {
	let auraType = $(this).attr('data-aura-type');
	let auraName = $(this).attr('name');
	let checkedVal = $(this).attr('data-checked') === 'true';
	$(this).attr('data-checked', !checkedVal);
	auras[$(this).attr('name')] = !checkedVal;

	if (auraName == "totemOfWrath" || auraName == "curseOfTheElements" || auraName == "prayerOfSpirit" || auraName == "powerOfTheGuardianWarlock" || auraName == "powerOfTheGuardianMage" || auraName == "drumsOfBattle" || auraName == "drumsOfWar" || auraName == "drumsOfRestoration" || auraName == "bloodlust") {
		updateSimulationSettingsVisibility();
	}
	modifyStatsFromAura(_auras[auraType][auraName], checkedVal);
	localStorage.auras = JSON.stringify(auras);
	refreshCharacterStats();
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
		let consumableName = $(this).attr('name');

		if (consumableName !== clickedConsumableName) {
			if ($(this).attr('data-checked') === 'true') {
				$(this).attr('data-checked', false);
				auras[consumableName] = false;

				for (let stat in _auras.consumables[consumableName]) {
					if (characterStats.hasOwnProperty(stat)) {
						characterStats[stat] -= _auras.consumables[consumableName][stat];
					}
				}
			}
		}
	});

	localStorage.auras = JSON.stringify(auras);
	refreshCharacterStats();
});

// User clicks on one of the preset item set buttons above the item slot selection menu
$(".preset-item-set").click(function() {
	let name = $(this).data('name');
	if (presetItemSets[name]) {
		for (itemSlot in presetItemSets[name]) {
			if (selectedItems[itemSlot] !== presetItemSets[name][itemSlot]) {
				for (item in items[itemSlot]) {
					if (items[itemSlot][item].id == presetItemSets[name][itemSlot]) {
						modifyStatsFromItem(items[itemSlot][item], 'add');
					} else if (items[itemSlot][item].id == selectedItems[itemSlot]) {
						modifyStatsFromItem(items[itemSlot][item], 'remove');
					}
				}
				if (itemSlot == (localStorage['selectedItemSlot'] + localStorage['selectedItemSubSlot'])) {
					$(".item-row[data-wowhead-id='" + selectedItems[itemSlot] + "']").attr('data-selected', 'false');
					$(".item-row[data-wowhead-id='" + presetItemSets[name][itemSlot] + "']").attr('data-selected', 'true');
				}
				selectedItems[itemSlot] = presetItemSets[name][itemSlot];
			}
		}
		selectedItems = presetItemSets[name];
		localStorage.selectedItems = JSON.stringify(selectedItems);
		refreshCharacterStats();
		updateSetBonuses();
	}
});

// User clicks on an item slot in the selection above the item table
$("#item-slot-selection-list li").click(function() {
	loadItemsBySlot($(this).attr('data-slot'), $(this).attr('data-subslot') || null);
});

// When the user clicks anywhere on the webpage
$(document).on('click', function(e) {
	// Hide the gem selection table if the user clicks outside of it.
	if (e.target.id !== "gem-selection-table") {
		$("#gem-selection-table").css('visibility', 'hidden');
	}
});

// User clicks on a gem row in the gem selection table
$("#gem-selection-table").on('click', 'tr', function() {
	let itemID = $("#gem-selection-table").data('itemID');
	let itemSlot = $('tr[data-wowhead-id="' + itemID + '"]').data('slot');
	let gemName = $(this).data('name');
	let gemColor = $(this).data('color');
	let gemIconName = href = null;
	let gemID = null;
	let socket = $('tr[data-wowhead-id="' + itemID + '"]').find('.gem').eq($("#gem-selection-table").data('socketSlot'));
	let socketSlot = $("#gem-selection-table").data('socketSlot');
	selectedGems[itemSlot] = selectedGems[itemSlot] || {};

	if (!selectedGems[itemSlot][itemID]) {
		let socketAmount = $('tr[data-wowhead-id="' + itemID + '"]').find('.gem').last().data('order') + 1; // The amount of sockets in the item

		selectedGems[itemSlot][itemID] = Array(socketAmount).fill(null);
	}

	// Check whether the user chose a gem or the option to remove the current gem
	if (gemName === "none") {
		gemIconName = socketInfo[gemColor].iconName + ".jpg";
		href = '';
	} else {
		gemIconName = gems[gemColor][gemName].iconName + ".jpg";
		gemID = gems[gemColor][gemName].id;
		href = 'https://tbc.wowhead.com/item=' + gems[gemColor][gemName].id
	}

	// Check if the socket that was changed was on an equipped item
	if (socket.closest('tr').data('wowhead-id') == selectedItems[itemSlot]) {
		// Remove stats from old gem if equipped
		if (selectedGems[itemSlot][itemID][socketSlot]) {
			for (let color in gems) {
				for (let gem in gems[color]) {
					if (gems[color][gem].id == selectedGems[itemSlot][itemID][socketSlot]) {
						modifyStatsFromGem(gems[color][gem].id, 'remove');
						break
					} 
				}
			}
		}
		modifyStatsFromGem(gemID, 'add'); // Add stats from new gem
	}

	socket.attr('src', 'img/' + gemIconName);
	socket.closest('a').attr('href', href);
	selectedGems[itemSlot][itemID][socketSlot] = gemID;
	localStorage.selectedGems = JSON.stringify(selectedGems);
	$("#gem-selection-table").css('visibility', 'hidden');
	return false;
});

// Tracks right clicks on item sockets
$("#item-selection-table tbody").on('contextmenu', '.gem', function(event) {
	// Check whether there is a gem in the socket or not
	if ($(this).closest('a').attr('href') !== "") {
		let socketColor = $(this).data('color');
		let itemSlot = $(this).closest('tr').data('slot');
		let itemID = $(this).closest('tr').data('wowhead-id');
		let socketOrder = $(this).data('order');
	
		$(this).attr('src', 'img/' + socketInfo[socketColor].iconName + '.jpg');
		$(this).closest('a').attr('href', '');
		modifyStatsFromGem(selectedGems[itemSlot][itemID][socketOrder], 'remove');
		selectedGems[itemSlot][itemID][socketOrder] = null;
		localStorage.selectedGems = JSON.stringify(selectedGems);
	}

	return false;
});

// User left-clicks on one of the item's gem sockets
$("#item-selection-table tbody").on('click', '.gem', function(event) {
	// Check if the socket color that was clicked is a different color, otherwise there's no reason to delete and insert new rows.
	if ($("#gem-selection-table").data('color') !== $(this).data('color')) {
		let socketColor = $(this).attr('data-color');
		$(".gem-row").remove();
		$("#gem-selection-table").append('<tr data-color="' + socketColor + '" data-name="none" class="gem-row"><td></td><td>None</td></tr>');

		for (let color in gems) {
			for (let gem in gems[color]) {
				// Show all gems for normal slots (except for Meta gems) and only show Meta gems for Meta gem slots
				if ((socketColor === "meta" && color == "meta") || (socketColor !== "meta" && color !== "meta")) {
					let g = gems[color][gem];
					$("#gem-selection-table").append("<tr data-color='" + color + "' data-name='" + gem + "' class='gem-row'><td><img width='20' height='20' src='img/" + g.iconName + ".jpg'></td><td><a href='https://tbc.wowhead.com/item=" + g.id + "'>" + g.name + "</a></td></tr>");
				}
			}
		}
	}

	$("#gem-selection-table").css('top', event.pageY - ($("#gem-selection-table").height()) / 2);
	$("#gem-selection-table").css('left', event.pageX + 50);
	$("#gem-selection-table").css('visibility', 'visible');
	$("#gem-selection-table").data('color', $(this).data('color'));
	$("#gem-selection-table").data('itemID', $(this).closest('tr').data('wowhead-id'));
	$("#gem-selection-table").data('socketSlot', $(this).data('order'));

	// Stop the click from being registered by the .item-row listener as well.
	event.stopPropagation();
	return false;
});

// User clicks on an item in the item table
$("#item-selection-table tbody").on('click', 'tr', function() {
	let itemSlot = $(this).attr('data-slot');
	let itemName = $(this).attr('data-name');
	let itemID = $(this).closest('tr').data('wowhead-id');
	let subSlot = localStorage['selectedItemSubSlot'] || $(this).data('subslot') || ""; // Workaround for having two selections for rings and trinkets but only one selection for the other slots.

	// Toggle the item's data-selected boolean.
	let equipped = $(this).attr('data-selected') == 'true' && selectedItems[itemSlot + subSlot] && selectedItems[itemSlot + subSlot] == itemID;
	$(this).attr('data-selected', !equipped);

	// Check if the user already has an item equipped in this slot and unequip it if so
	if (selectedItems[itemSlot + subSlot]) {
		// Set the old item's data-selected value to false and remove the item's stats from the player.
		$('[data-wowhead-id="' + selectedItems[itemSlot + subSlot] +'"]').attr('data-selected', false);
		itemLoop:
		for (let slot in items) {
			for (let item in items[slot]) {
				if (items[slot][item].id == selectedItems[itemSlot + subSlot]) {
					// Remove the stats from the item
					modifyStatsFromItem(items[slot][item], 'remove');

					// Remove stats from gems equipped in the item
					if (selectedGems[slot] && selectedGems[slot][selectedItems[itemSlot + subSlot]]) {
						for (gemID of selectedGems[slot][selectedItems[itemSlot + subSlot]]) {
							if (gemID !== null) {
								modifyStatsFromGem(gemID, 'remove');
							}
						}
					}

					selectedItems[itemSlot + subSlot] = null;
					break itemLoop;
				}
			}
		}
	}

	// Add the stats from the item
	if (!equipped) {
		modifyStatsFromItem(items[itemSlot][itemName], 'add');
		selectedItems[itemSlot + subSlot] = items[itemSlot][itemName].id;

		// Add stats from the item's equipped gems
		if (selectedGems[itemSlot + subSlot] && selectedGems[itemSlot + subSlot][items[itemSlot][itemName].id]) {
			for (gemID of selectedGems[itemSlot + subSlot][items[itemSlot][itemName].id]) {
				if (gemID !== null) {
					modifyStatsFromGem(gemID, 'add');
				}
			}
		}
	}

	// If the user is equipping a main hand or offhand then unequip their twohander if they have one equipped and vice versa
	if (itemSlot == "mainhand" || itemSlot == "offhand") {
		if (selectedItems['twohand'] !== null) {
			itemSlotLoop:
			for (let slot in items) {
				for (let item in items[slot]) {
					if (items[slot][item].id == selectedItems['twohand']) {
						modifyStatsFromItem(items[slot][item], 'remove');
						selectedItems['twohand'] = null;
						break itemSlotLoop;
					}
				}
			}
		}
	} else if (itemSlot == "twohand") {
		if (selectedItems['mainhand'] !== null) {
			itemSlotLoop:
			for (let slot in items) {
				for (let item in items[slot]) {
					if (items[slot][item].id == selectedItems['mainhand']) {
						modifyStatsFromItem(items[slot][item], 'remove');
						selectedItems['mainhand'] = null;
						break itemSlotLoop;
					}
				}
			}
		}
		if (selectedItems['offhand'] !== null) {
			itemSlotLoop:
			for (let slot in items) {
				for (let item in items[slot]) {
					if (items[slot][item].id == selectedItems['offhand']) {
						modifyStatsFromItem(items[slot][item], 'remove');
						selectedItems['offhand'] = null;
						break itemSlotLoop;
					}
				}
			}
		}
	}

	updateSetBonuses();
	refreshCharacterStats();
	localStorage.selectedItems = JSON.stringify(selectedItems);
	return false;
});

// User clicks on an enchant
$("#enchant-selection-table tbody").on('click', 'tr', function(event) {
	let itemSlot = $(this).attr('data-slot');
	let subSlot = $(this).attr('data-subslot') || "";
	let enchantID = $(this).data('wowhead-id');

	// Toggle the enchant's data-selected boolean.
	let equipped = $(this).attr('data-selected') == 'true' && selectedEnchants[itemSlot + subSlot] && selectedEnchants[itemSlot + subSlot] == enchantID;
	$(this).attr('data-selected', !equipped);

	// Check if the user already has an enchant equipped in this slot and unequip it if so
	if (selectedEnchants[itemSlot + subSlot]) {
		$("#enchant-selection-table tr[data-wowhead-id='" + selectedEnchants[itemSlot + subSlot] + "']").attr('data-selected', 'false');
		modifyStatsFromEnchant(selectedEnchants[itemSlot + subSlot], 'remove');
		selectedEnchants[itemSlot + subSlot] = null;
	}

	if (!equipped) {
		modifyStatsFromEnchant(enchantID, 'add');
		selectedEnchants[itemSlot + subSlot] = enchantID;
	}
	localStorage.selectedEnchants = JSON.stringify(selectedEnchants);
	refreshCharacterStats();
	return false;
});

$(".preset-talent-button").click(function() {
	talents = presetTalents[$(this).data('name')];
	talentPointsRemaining = totalTalentPoints;

	$(".talent-tree-table").each(function() {
		$(this).data('points', 0);
	});
	$(".talent-icon").each(function() {
		talents[$(this).attr('id')] = talents[$(this).attr('id')] || 0;
		let pointAmount = talents[$(this).attr('id')];
		let talentTableObj = $(this).closest('table');

		$(this).attr('data-points', pointAmount);
		talentTableObj.data('points', talentTableObj.data('points') + pointAmount);
		talentPointsRemaining -= pointAmount || 0;
		updateTalentInformation($(this));
		updateTalentTreeNames();
	});

	localStorage.talents = JSON.stringify(talents);
	refreshCharacterStats();
	updateSimulationSettingsVisibility();
});

// Disable the context menu from appearing when the user right clicks a talent
$(".talent-icon").bind("contextmenu", function(event) {
	return false;
});

// Prevents the user from being redirected to the talent's wowhead page when clicking it.
$(".talent-icon").click(function() {
	return false;
});

// Fires when the user left or right clicks a talent
$(".talent-icon").mousedown(function(event) {
	// Check if the click was a left or right click
	if ((event.which === 1 && talentPointsRemaining > 0) || event.which === 3) {
		let icon = $(this);
		let talent = _talents[icon.attr('data-tree')][icon.attr('id')]; // get the talent's object
		let talentName = $(this).attr('id');
		let talentTree = $("#talent-table-" + $(this).data('tree'));

		// left click
		if (event.which === 1) {
			// compare the amount of points in the talent vs the amount of ranks before incrementing
			if (Number(icon.attr('data-points')) < talent.rankIDs.length && talentTree.data('points') >= (talent.row - 1) * 5) {
				// If the talent has another talent that needs to be selected before this one is selectable, then check if the required talent is selected.
				if (talent.requirement && talents[talent.requirement.name] < talent.requirement.points) {
					return;
				}

				icon.attr('data-points', Number(icon.attr('data-points')) + 1);
				talents[talentName] = Number(talents[talentName]) + 1;
				talentPointsRemaining--;
				talentTree.data('points', talentTree.data('points') + 1);
			}
		// right click
		} else if (event.which === 3) {
			// only decrement if the point amount is above 0
			if (icon.attr('data-points') > 0) {
				let talentTreeName = $(this).data('tree');
				// Check if the talent is locked due to a dependency (such as not being able to remove Amplify Curse because Curse of Exhaustion is selected)
				for (let t in _talents[talentTreeName]) {
					if (_talents[talentTreeName][t].requirement && _talents[talentTreeName][t].requirement.name == talentName && talents[t] > 0) {
						return;
					}
				}

				icon.attr('data-points', Number(icon.attr('data-points'))-1);
				talents[talentName] = Number(talents[talentName]) - 1;
				talentPointsRemaining++;
				talentTree.data('points', talentTree.data('points') - 1);
				icon.children('a').children('span').css('color', "#7FFF00");
			}
		}

		updateTalentTreeNames();
		if (talent.name == "Conflagrate" || talent.name == "Master Demonologist" || talent.name == "Demonic Sacrifice" || talent.name == "Summon Felguard" || talent.name == "Dark Pact") {
			updateSimulationSettingsVisibility();
		}
		if (talent.name == "Emberstorm" || talent.name == "Improved Imp" || talent.name == "Demonic Aegis" || talent.name == "Demonic Embrace" || talent.name == "Devastation" || talent.name == "Backlash" || talent.name == "Fel Stamina" || talent.name == "Fel Intellect" || talent.name == "Master Demonologist" || talent.name == "Soul Link" || talent.name == "Demonic Tactics" || talent.name == "Shadow Mastery") {
			refreshCharacterStats();
		}
		updateTalentInformation(icon);
	}

	localStorage.talents = JSON.stringify(talents);
	return false;
});

// User clicks on the red X next to a talent tree's name to clear it
$(".clear-talent-tree").click(function() {
	clearTalentTree($(this).closest('div').find('h3').data('name'));
});

// Listens to any clicks on the "rotation" spells for dots, filler, curse, and finisher.
$("#rotation-list div li").click(function() {
	let clickedSpell = $(this).data('name');
	let refreshStats = false;

	if ($(this).hasClass("rotation-filler")) {
		$(".rotation-filler").each(function() {
			$(this).attr('data-checked', false);
			rotation[$(this).data('type')][$(this).data('name')] = false;
		});

		if ($("#demonicSacrifice").data('points') == 1) {
			refreshStats = true;
		}
	} else if ($(this).hasClass("rotation-curse")) {
		$(".rotation-curse").each(function() {
			if ($(this).data('name') !== clickedSpell) {
				$(this).attr('data-checked', false);
				rotation[$(this).data('type')][$(this).data('name')] = false;
			}
		});
	}

	let checkedVal = $(this).attr('data-checked') === 'true';
	$(this).attr('data-checked', !checkedVal);
	rotation[$(this).data('type')][$(this).data('name')] = !checkedVal;
	localStorage.rotation = JSON.stringify(rotation);
	if (refreshStats) refreshCharacterStats();
	return false;
});

$("#sim-settings select, #sim-settings input").change(function() {
	settings[$(this).attr('id')] = $(this).val();
	localStorage.settings = JSON.stringify(settings);
	refreshCharacterStats();
});

// to-do: don't allow people to start multiple simulations
$("#sim-dps").click(function() {
	simDPS([$(".item-row[data-selected='true']").data('wowhead-id')]);
	return false;
});

$("#sim-all-items").click(function() {
	let arr = [];
	$(".item-row").each(function(i) {
		arr.push($(this).data('wowhead-id'));
	});
	$("#damage-breakdown-section").hide();
	simDPS(arr);
	return false;
});

$(".btn").hover(function() {
	$(this).find('a').css('color', '#1a1a1a');
});

$(".btn").mouseout(function() {
	$(this).find('a').css('color', 'white');
});

// User changes races in the simulation settings
$("#race-dropdown-list").change(function() {
	let oldRace = $(this).data("currentRace");
	let newRace = $(this).val();
	$(this).data("currentRace", newRace);

	// Remove the previous race's stats
	for (let stat in raceStats[oldRace]) {
		if (characterStats.hasOwnProperty(stat)) {
			// Check if the buff is a modifier to know whether to add/subtract or multiply/divide the stat
			if (stat.toLowerCase().search("modifier") !== -1) {
				characterStats[stat] /= raceStats[oldRace][stat];
			} else {
				characterStats[stat] -= raceStats[oldRace][stat];
			}
		}
	}

	// Add the new race's stats
	for (let stat in raceStats[newRace]) {
		if (characterStats.hasOwnProperty(stat)) {
			// Check if the buff is a modifier to know whether to add/subtract or multiply/divide the stat
			if (stat.toLowerCase().search("modifier") !== -1) {
				characterStats[stat] *= raceStats[newRace][stat];
			} else {
				characterStats[stat] += raceStats[newRace][stat];
			}
		}
	}

	$("#race").text($("#race-dropdown-list").children("option:selected").text());
	refreshCharacterStats();
});

// Loads items into the item table
function loadItemsBySlot(itemSlot, subSlot) {
	// Set old item slot's selected value to false
	$("#item-slot-selection-list li[data-selected='true']").attr('data-selected', 'false');
	// Set the new item slot's seleected value to true
	let newItemSlotSelector = "#item-slot-selection-list li[data-slot='" + itemSlot + "']";
	// If the item has a subslot then add a subslot selector to the query
	if (subSlot !== null) newItemSlotSelector += "[data-subslot='" + subSlot + "']";
	$(newItemSlotSelector).attr('data-selected', 'true');
	localStorage['selectedItemSlot'] = itemSlot;
	localStorage['selectedItemSubSlot'] = (subSlot || "");
	savedItemDPS[itemSlot + subSlot] = savedItemDPS[itemSlot + subSlot] || {};

	// Removes all current item rows
	$(".item-row").remove(); 
	let tableBody = $("#item-selection-table tbody");

	for (let item of Object.keys(items[itemSlot])) {
		let i = items[itemSlot][item];

		// If an item is unique and it is already equipped in the other slot then skip it
		if (i.unique && (itemSlot == "ring" || itemSlot == "trinket") && subSlot !== null) {
			let otherSlot = subSlot == '1' ? '2' : '1';
			if (selectedItems[itemSlot + otherSlot] == i.id) continue;
		}

		// Add the item's gem sockets
		let sockets = [];
		let counter = 0;
		for (let socket in socketInfo) {
			if (i.hasOwnProperty(socket)) {
				for(j = 0; j < i[socket]; j++) {
					let gemIcon = socketInfo[socket].iconName;
					let gemHref = '';

					if (selectedGems[itemSlot] && selectedGems[itemSlot][i.id]) {
						for (let color in gems) {
							for (let gem in gems[color]) {
								if (gems[color][gem].id == selectedGems[itemSlot][i.id][counter]) {
									gemIcon = gems[color][gem].iconName;
									gemHref = 'https://tbc.wowhead.com/item=' + gems[color][gem].id;
								}
							}
						}
					}
					sockets.push("<a href='" + gemHref + "'><img width='16' height='16' class='gem' data-color='" + socket + "' data-order='" + counter + "' src='img/" + gemIcon + ".jpg'></a>");
					counter++;
				}
			}

		}

		tableBody.append("<tr data-subslot='" + localStorage['selectedItemSubSlot'] + "' data-slot='" + itemSlot + "' data-name='" + item + "' data-selected='" + (selectedItems[itemSlot + localStorage['selectedItemSubSlot']] == i.id || 'false') + "' class='item-row' data-wowhead-id='" + i.id + "'><td><a href='https://tbc.wowhead.com/item=" + i.id + "'>" + i.name + "</a></td><td><div>" + sockets.join('') + "</div></td><td>" + i.source + "</td><td>" + (i.stamina || '') + "</td><td>" + (i.intellect || '') + "</td><td>" + (Math.round(i.spellPower + (i.onUseSpellPower * i.duration / i.cooldown)) || i.spellPower || Math.round(i.onUseSpellPower * i.duration / i.cooldown) || '') + "</td><td>" + (i.shadowPower || '') + "</td><td>" + (i.firePower || '') + "</td><td>" + (i.critRating || '') + "</td><td>" + (i.hitRating || '') + "</td><td>" + (Math.round(i.hasteRating + (i.onUseHasteRating * i.duration / i.cooldown)) || i.hasteRating || Math.round(i.onUseHasteRating * i.duration / i.cooldown) || '') + "</td><td class='item-dps'>" + (savedItemDPS[itemSlot + subSlot][i.id] || '') + "</td></tr>")
	}

	sortItemTable();
	loadEnchantsBySlot(itemSlot, subSlot);
}

function loadEnchantsBySlot(itemSlot, subSlot = null) {
	if (itemSlot == "mainhand" || itemSlot == "twohand") {
		itemSlot = "weapon";
	}

	if (enchants[itemSlot]) {
		$(".enchant-row").remove();
		let tableBody = $("#enchant-selection-table tbody");

		for (let enchant of Object.keys(enchants[itemSlot])) {
			let e = enchants[itemSlot][enchant];

			tableBody.append("<tr data-slot='" + itemSlot + "' data-subslot='" + (subSlot || "") + "' data-name='" + enchant + "' data-selected='" + (selectedEnchants[itemSlot + (subSlot || "")] == e.id || 'false') + "' class='enchant-row' data-wowhead-id='" + e.id + "'><td><a href='https://tbc.wowhead.com/spell=" + e.id + "'>" + e.name + "</a></td><td>" + (e.spellPower || '') + "</td><td>" + (e.shadowPower || '') + "</td><td>" + (e.firePower || '') + "</td><td>" + (e.stamina || '') + "</td><td>" + (e.intellect || '') + "</td><td>" + (e.mp5 || '') + "</td><td>" + (((e.natureResistance || 0) + (e.allResistance || 0)) || '') + "</td><td>" + (((e.shadowResistance || 0) + (e.allResistance || 0)) || '') + "</td><td>" + (((e.fireResistance || 0) + (e.allResistance || 0)) || '') + "</td><td>" + ((e.threatReduction * 100) || '') + "</td><td>" + (e.threatIncrease * 100 || '') + "</td><td>" + (localStorage[enchant + "Dps"] || '') + "</td></tr>")
		}

		$("#enchant-selection-table").show();
	} else {
		$("#enchant-selection-table").hide();
	}

	refreshCharacterStats();
}

// Adds or removes an item's stats from the player
function modifyStatsFromItem(itemObj, action) {
	// If the user has the item equipped and is not loading the stats from equipped items when loading the website
	if (action == 'remove') {
		// Loop through the stats on the item and add them to/remove them from the character's stats.
		for (let stat in itemObj) {
			// Check if the item property is a character stat such as stamina/spell power.
			if (characterStats.hasOwnProperty(stat)) {
				characterStats[stat] -= itemObj[stat];
			}
		}
	} else if (action == 'add') {
		for (let stat in itemObj) {
			if (characterStats.hasOwnProperty(stat)) {
				characterStats[stat] += itemObj[stat];
			}
		}
	}
}

// Adds or removes an enchant's stats from the player
function modifyStatsFromEnchant(enchantID, action) {
	for (let itemSlot in enchants) {
		for (let enchant in enchants[itemSlot]) {
			if (enchants[itemSlot][enchant].id == enchantID) {
				let enchantObj = enchants[itemSlot][enchant];
				
				if (action == 'remove') {
					for (let stat in enchantObj) {
						if (characterStats.hasOwnProperty(stat)) {
							characterStats[stat] -= enchantObj[stat];
						}
					}
				} else if (action == 'add') {
					for (let stat in enchantObj) {
						if (characterStats.hasOwnProperty(stat)) {
							characterStats[stat] += enchantObj[stat];
						}
					}
				}
				return;
			}
		}
	}
}

function modifyStatsFromGem(gemID, action) {
	for (let color in gems) {
		for (let gem in gems[color]) {
			if (gems[color][gem].id == gemID) {
				for (let property in gems[color][gem]) {
					if (characterStats.hasOwnProperty(property)) {
						if (action == 'add') {
							characterStats[property] += gems[color][gem][property];
						} else if (action == 'remove') {
							characterStats[property] -= gems[color][gem][property];
						}
					}
				}
				return;
			}
		}
	}
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
}

function simDPS(items) {
	let item = $("#item-slot-selection-list li[data-selected='true']");
	let itemSlot = item.attr('data-slot');
	let itemSubSlot = item.attr('data-subslot') || '';
	let itemAmount = items.length;
	let simulationsRunning = 0;
	let simulationsFinished = 0;
	let multiSimInfo = [];
	let simulations = [];
	let simIndex = 0;

	for (let i = 0; i < items.length; i++) {
		multiSimInfo.push([items[i],0]);

		simulations.push(new SimWorker(
			(simulationEnd) => {
				simulationsFinished++;
				// DPS information on the sidebar
				let avgDps = Math.round((simulationEnd.totalDamage / simulationEnd.totalDuration) * 100) / 100
				if (itemAmount === 1) {
					localStorage['avgDps'] = avgDps;
					localStorage['minDps'] = simulationEnd.minDps;
					localStorage['maxDps'] = simulationEnd.maxDps;
					localStorage['simulationDuration'] = simulationEnd.length;
					$("#avg-dps").text(avgDps);
					$("#min-dps").text(simulationEnd.minDps);
					$("#max-dps").text(simulationEnd.maxDps);
					$("#sim-length-result").text(simulationEnd.length + "s");
					$("#sim-dps a").text("Simulate");
				} else if (simulationsFinished == itemAmount) {
					$("#sim-all-items a").text("Simulate All Items");
				}
				savedItemDPS[itemSlot + itemSubSlot] = savedItemDPS[itemSlot + itemSubSlot] || {};
				savedItemDPS[itemSlot + itemSubSlot][simulationEnd.itemID] = avgDps;
				localStorage.savedItemDPS = JSON.stringify(savedItemDPS);

				if (simulationsFinished === itemAmount) {
					// Remove the background coloring (progress bar)
					$(".btn").css('background', 'none');

					if (itemAmount === 1) {
						// Setup the damage breakdown table (showing avg damage, avg cast etc. for each spell)
						$(".spell-damage-information").remove();
						for (let spell of Object.keys(simulationEnd.damageBreakdown)) {
							let s = simulationEnd.damageBreakdown[spell];
							let percentDamage = (~~(((s.damage / simulationEnd.totalDamage) * 100) * 100) / 100).toFixed(2);
							if (s.damage > 0 || s.casts > 0) $("#damage-breakdown-table tbody").append("<tr class='spell-damage-information'><td>" + s.name + "</td><td><meter value='" + percentDamage + "' min='0' max='100'></meter> " + percentDamage + "%</td><td class='number'>" + Math.ceil(s.casts / simulationEnd.iterations) + "</td><td class='number'>" + ~~(s.damage / s.casts) + (s.dotDamage ? ("(" + ~~(s.dotDamage / s.casts) + ")") : "") + "</td><td class='number'>" + ((~~(((s.crits / s.casts) * 100) * 100)) / 100).toFixed(2) + "</td><td class='number'>" + (~~(((s.misses / s.casts) * 100) * 100) / 100).toFixed(2) + "</td><td class='number'>" + Math.round((s.damage / simulationEnd.totalDuration) * 100) / 100 + "</td></tr>");
						}
						$("#damage-breakdown-section").css('display', 'inline-block');
						$("#damage-breakdown-table").tablesorter();
					}
				}
	
				// Start a new simulation that's waiting in the queue if there are any remaining
				if (simulationsRunning - simulationsFinished < maxWorkers && simIndex < simulations.length) {
					simulations[simIndex++].start();
				}
			},
			(simulationUpdate) => {
				if (itemAmount === 1) {
					$("#avg-dps").text(simulationUpdate.averageDamage)
					// Uses the sim button as a progress bar by coloring it based on how many iterations are done with
					$("#sim-dps").css('background', 'linear-gradient(to right, #9482C9 ' + simulationUpdate.percent + '%, transparent ' + simulationUpdate.percent + '%)');
					$("#sim-dps a").text(Math.round(simulationUpdate.percent) + "%");
				} else {
					// multiSimInfo tracks the % progress of each simulation and the least progressed simulation's % is used for the multi simulation button/progress bar
					let smallestValue = 100;
					for(let i = 0; i < multiSimInfo.length; i++) {
						if (multiSimInfo[i][0] == simulationUpdate.itemID) {
							multiSimInfo[i][1] = simulationUpdate.percent;
						}
						if (multiSimInfo[i][1] < smallestValue) smallestValue = multiSimInfo[i][1];
					}

					$("#sim-all-items").css('background', 'linear-gradient(to right, #9482C9 ' + smallestValue + '%, transparent ' + smallestValue + '%)');
					$("#sim-all-items a").text(smallestValue + "%");
				}
				// Set the DPS value on the item in the item selection list
				$(".item-row[data-wowhead-id='" + simulationUpdate.itemID + "']").find('.item-dps').text(simulationUpdate.averageDamage);
			},
			{
				"player": Player.getSettings(),
				"simulation": Simulation.getSettings(),
				"itemSlot": itemSlot,
				"itemSubSlot": itemSubSlot,
				"itemID": items[i],
				"itemAmount": itemAmount
			}
		));
	}

	// Start as many simulations as 'maxWorkers' says can be run simultaneously.
	while (simulationsRunning < maxWorkers && simIndex < simulations.length) {
		simulations[simIndex++].start();
		simulationsRunning++;
	}
}

function updateTalentTreeNames() {
	$(".talent-tree-table").each(function() {
		let talentTreeName = $(this).data('name');
		talentTreeName = talentTreeName.charAt(0).toUpperCase() + talentTreeName.slice(1);
		if ($(this).data('points') > 0) {
			talentTreeName += " (" + $(this).data('points') + ")";
		}
		$(".talent-tree-name h3[data-name='" + $(this).data('name') + "']").text(talentTreeName);
	});
}

function updateTalentInformation(talentUiObj) {
	// Update the point amount on the talent icon
	talentUiObj.children('a').children('.talent-point-amount').text(talentUiObj.attr('data-points'));
	// if the point amount is 0 then we hide the amount
	if (talentUiObj.children('a').children('.talent-point-amount').text() > 0) {
		talentUiObj.children('a').children('.talent-point-amount').show();
	} else {
		talentUiObj.children('a').children('.talent-point-amount').hide();
	}
	talentUiObj.children('a').attr('href', 'https://classic.wowhead.com/spell=' + _talents[talentUiObj.data('tree')][talentUiObj.attr('id')].rankIDs[Math.max(0,talents[talentUiObj.attr('id')]-1)]);
	//$WowheadPower.refreshLinks();

	// todo: move these JS css changes to the css file
	if (Number(talentUiObj.attr('data-points')) == Number(talentUiObj.attr('data-maxpoints'))) {
		talentUiObj.children('a').children('span').css('color', "#ffcd45");	
	} else {
		talentUiObj.children('a').children('span').css('color', "#7FFF00");
	}
}

function clearTalentTree(talentTreeName) {
	if ($("#talent-table-" + talentTreeName).data('points') > 0) {
		for (let talent in _talents[talentTreeName]) {
			talentPointsRemaining += talents[talent];
			talents[talent] = 0;
			$("#" + talent).attr('data-points', 0);
			$("#talent-table-" + talentTreeName).data('points', 0);
			updateTalentInformation($("#" + talent));
		}
		updateTalentTreeNames();
		localStorage.talents = JSON.stringify(talents);
	}

	refreshCharacterStats();
	updateSimulationSettingsVisibility();
}

function sortItemTable() {
	$("#item-selection-table").tablesorter();
}

function updateSetBonuses() {
	$(".sidebar-set-bonus").remove();
	let setBonusCounter = {};

	for (let itemSlot in selectedItems) {
		let itemID = selectedItems[itemSlot];
		if (itemID) {
			if (itemSlot == "ring1" || itemSlot == "ring2" || itemSlot == "trinket1" || itemSlot == "trinket2") {
				itemSlot = itemSlot.substring(0,itemSlot.length-1);
			}
			for (let item in items[itemSlot]) {
				if (items[itemSlot][item].id === itemID) {
					let setID = items[itemSlot][item].setID;
					if (setID) {
						setBonusCounter[setID] = setBonusCounter[setID] + 1 || 1;
						break;
					}
				}
			}
		}
	}

	for (set in setBonusCounter) {
		// Check if the item's set has actually been implemented
		if (sets[set]) {
			for (let i = sets[set].bonuses.length-1; i >= 0; i--) {
				if (sets[set].bonuses[i] <= setBonusCounter[set]) {
					$("#sidebar-sets").append("<li class='sidebar-set-bonus'><a href='https://tbc.wowhead.com/item-set=" + set + "'>" + sets[set].name + " (" + sets[set].bonuses[i] + ")</a></li>")
					break;
				}
			}
		}
	}

	localStorage.setBonuses = JSON.stringify(setBonusCounter);
}

function updateSimulationSettingsVisibility() {
	if (talents.demonicSacrifice === 1 && (talents.masterDemonologist > 0 || talents.darkPact > 0)) {
		$("#sacrificePet").closest('li').show();
	} else {
		$("#sacrificePet").closest('li').hide();
	}

	if (talents.demonicSacrifice === 0 || talents.masterDemonologist > 0 || talents.darkPact > 0) {
		$("#petChoice").closest('li').show();
	} else {
		$("#petChoice").closest('li').hide();
	}

	if (talents.summonFelguard === 0) {
		$("#petChoice option[value='felguard']").hide();
	} else {
		$("#petChoice option[value='felguard']").show();
	}

	if (auras.curseOfTheElements) {
		$("#improvedCurseOfTheElements").closest('li').show();
	} else {
		$("#improvedCurseOfTheElements").closest('li').hide();
	}

	if (auras.prayerOfSpirit) {
		$("#improvedDivineSpirit").closest('li').show();
	} else {
		$("#improvedDivineSpirit").closest('li').hide();
	}

	if (talents.conflagrate > 0) {
		$("#conflagrateUse").closest('li').show();
	} else {
		$("#conflagrateUse").closest('li').hide();
	}

	if (auras.powerOfTheGuardianMage) {
		$("#mageAtieshAmount").closest('li').show();
	} else {
		$("#mageAtieshAmount").closest('li').hide();
	}

	if (auras.powerOfTheGuardianWarlock) {
		$("#warlockAtieshAmount").closest('li').show();
	} else {
		$("#warlockAtieshAmount").closest('li').hide();
	}

	if (auras.drumsOfBattle) {
		$("#drumsOfBattleAmount").closest('li').show();
	} else {
		$("#drumsOfBattleAmount").closest('li').hide();
	}

	if (auras.drumsOfWar) {
		$("#drumsOfWarAmount").closest('li').show();
	} else {
		$("#drumsOfWarAmount").closest('li').hide();
	}

	if (auras.drumsOfRestoration) {
		$("#drumsOfRestorationAmount").closest('li').show();
	} else {
		$("#drumsOfRestorationAmount").closest('li').hide();
	}

	if (auras.bloodlust) {
		$("#bloodlustAmount").closest('li').show();
	} else {
		$("#bloodlustAmount").closest('li').hide();
	}

	if (auras.totemOfWrath) {
		$("#totemOfWrathAmount").closest('li').show();
	} else {
		$("#totemOfWrathAmount").closest('li').hide();
	}
}