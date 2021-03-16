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
	let checkedVal = $(this).attr('data-checked') === 'true';
	$(this).attr('data-checked', !checkedVal);
	localStorage[$(this).attr("name")] = !checkedVal;

	modifyStatsFromAura(auras[auraType][auraName], checkedVal);

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
				localStorage[consumableName] = false;

				for (let stat in auras.consumables[consumableName]) {
					if (characterStats.hasOwnProperty(stat)) {
						characterStats[stat] -= auras.consumables[consumableName][stat];
					}
				}
			}
		}
	});

	refreshCharacterStats();
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
	$("#gem-selection-table").css('visibility', 'hidden');

	return false;
});

// User clicks on one of the item's gem sockets
$("#item-selection-table tbody").on('click', '.gem', function(event) {
	// Check if the socket color that was clicked is a different color, otherwise there's no reason to delete and insert new rows.
	if ($("#gem-selection-table").data('color') !== $(this).data('color')) {
		$(".gem-row").remove();

		let socketColor = $(this).attr('data-color');
		for (let gemType in socketInfo[socketColor].gems) {
			let gemColor = socketInfo[socketColor].gems[gemType];

			for (let gem in gems[gemColor]) {
				let g = gems[gemColor][gem];
				$("#gem-selection-table").append("<tr class='gem-row'><td><img width='20' height='20' src='img/" + g.iconName + ".jpg'></td><td><a href='https://tbc.wowhead.com/item=" + g.id + "'>" + g.name + "</a></td></tr>");
			}
		}
	}

	$("#gem-selection-table").css('top', event.pageY - ($("#gem-selection-table").height()) / 2);
	$("#gem-selection-table").css('left', event.pageX + 50);
	$("#gem-selection-table").css('visibility', 'visible');
	$("#gem-selection-table").data('color', $(this).data('color'));
	$("#gem-selection-table").focus();

	// Stop the click from being registered by the .item-row listener as well.
	event.stopPropagation();
});

// User clicks on an item in the item table
$("#item-selection-table tbody").on('click', 'tr', function() {
	let itemSlot = $(this).attr('data-slot');
	let itemName = $(this).attr('data-name');
	let subSlot = $(this).data('subSlot') || ""; // Workaround for having two selections for rings and trinkets but only one selection for the other slots.

	// Toggle the item's data-selected boolean.
	let equipped = $(this).attr('data-selected') == 'true';
	$(this).attr('data-selected', !equipped);

	// Check if the user already has an item equipped in this slot and unequip it if so
	if (localStorage['equipped' + itemSlot + subSlot] !== null && localStorage['equipped' + itemSlot + subSlot] !== itemName) {
		// Set the old item's data-selected value to false and remove the item's stats from the player.
		$('[data-name="' + localStorage['equipped' + itemSlot + subSlot] +'"]').attr('data-selected', false);
		modifyStatsFromItem(localStorage['equipped' + itemSlot + subSlot], itemSlot, subSlot);
	}

	// Add or remove the stats from the item depending on whether the player has the item equipped or not.
	modifyStatsFromItem(itemName,itemSlot,subSlot);

	// If the user is equipping a main hand or offhand then equip their twohander if they have one equipped and vice versa
	if (itemSlot == "mainhand" || itemSlot == "offhand") {
		if (localStorage['equippedtwohand']) {
			modifyStatsFromItem(localStorage['equippedtwohand'], 'twohand');
			localStorage.removeItem('equippedtwohand');
		}
	} else {
		if (localStorage['equippedmainhand']) {
			modifyStatsFromItem(localStorage['equippedmainhand'], 'mainhand');
			localStorage.removeItem('equippedmainhand');
		}
		if (localStorage['equippedoffhand']) {
			modifyStatsFromItem(localStorage['equippedoffhand'], 'offhand');
			localStorage.removeItem('equippedoffhand');
		}
	}

	// Return false so that the user doesn't get redirected to wowhead's website when he clicks the item.
	return false;

	//todo implement code to automatically unequip 2h when equipping 1h/offhand and vice versa.
});

// User clicks on an enchant
$("#enchant-selection-table tbody").on('click', 'tr', function(event) {
	let itemSlot = $(this).attr('data-slot');
	let enchantName = $(this).attr('data-name');

	// Toggle the enchant's data-selected boolean.
	let equipped = $(this).attr('data-selected') == 'true';
	$(this).attr('data-selected', !equipped);

	// Check if the user already has an enchant equipped in this slot and unequip it if so
	if (localStorage['equipped' + itemSlot + "Enchant"] !== null && localStorage['equipped' + itemSlot + "Enchant"] !== enchantName) {
		// Set the old enchant's data-selected value to false and remove the enchants's stats from the player.
		$('[data-name="' + localStorage['equipped' + itemSlot + "Enchant"] +'"]').attr('data-selected', false);
		modifyStatsFromEnchant(localStorage['equipped' + itemSlot + "Enchant"], itemSlot);
	}

	modifyStatsFromEnchant(enchantName,itemSlot);

	return false;
});

// Add the talent trees
for (let tree in talents) {
	if (talents.hasOwnProperty(tree)) {
		$("#talents-section").append($("<div class='talent-tree-div'><table background='img/talent_tree_background_" + tree + ".jpg' id='talent-table-" + tree + "' class='talent-tree-table'></table><h3 class='talent-tree-name'>" + tree.charAt(0).toUpperCase() + tree.slice(1) + "</h3></div>"));
		$("#talent-table-" + tree).append($("<tbody></tbody>"));
		$("#talent-table-" + tree + " tbody").append($("<tr class='" + tree + "-tree-row'></tr>"));
		let lastRow = $("#talent-table-" + tree + " tbody tr:last");
		let currentCol = 1;

		for (let talent in talents[tree]) {
			let t = talents[tree][talent];
			let lowerTalentName = t.name.toLowerCase().split(' ').join('-');
			localStorage[lowerTalentName] = localStorage[lowerTalentName] || 0;

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

			lastRow.append($("<td><div data-points='" + localStorage[lowerTalentName] + "' class='talent-icon' data-tree='" + tree + "' id='" + talent + "'><a href='https://classic.wowhead.com/spell=" + t.rankIDs[Math.max(0,localStorage[lowerTalentName]-1)] + "'><img src='img/" + t.iconName + ".jpg' alt='" + t.name + "'><span id='" + lowerTalentName + "-point-amount' class='talent-point-amount'>" + localStorage[lowerTalentName] + "</span></a></div></td>"));
			
			// Check if the text displaying the talent point amount should be hidden or colored (for maxed out talents)
			let pointAmount = $("#" + lowerTalentName + "-point-amount")
			if (pointAmount.text() <= 0) {
				pointAmount.hide();
			} else if (pointAmount.text() == t.rankIDs.length) {
				pointAmount.css("color","#ffcd45");
			} else {
				pointAmount.css("color","#7FFF00")
			}
			currentCol++;
		}
	}
}

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
		let talent = talents[icon.attr('data-tree')][icon.attr('id')]; // get the talent's object
		let lowerTalentName = talent.name.toLowerCase().split(' ').join('-');

		// left click
		if (event.which === 1) {
			// compare the amount of points in the talent vs the amount of ranks before incrementing
			if (Number(icon.attr('data-points')) < talent.rankIDs.length) {
				icon.attr('data-points', Number(icon.attr('data-points')) + 1);
				localStorage[lowerTalentName] = Number(localStorage[lowerTalentName]) + 1;

				// todo: move these JS css changes to the css file
				if (Number(icon.attr('data-points')) == talent.rankIDs.length) {
					icon.children('a').children('span').css('color', "#ffcd45");	
				} else {
					icon.children('a').children('span').css('color', "#7FFF00");
				}
			}
		// right click
		} else if (event.which === 3) {
			// only decrement if the point amount is above 0
			if (icon.attr('data-points') > 0) {
				icon.attr('data-points', Number(icon.attr('data-points'))-1);
				localStorage[lowerTalentName] = Number(localStorage[lowerTalentName]) - 1;
				icon.children('a').children('span').css('color', "#7FFF00");
			}
		}

		// if these talents are changed then the character stats in the sidebar need to be updated
		if (talent.name == "Demonic Embrace" || talent.name == "Devastation" || talent.name == "Backlash" || talent.name == "Fel Stamina" || talent.name == "Fel Intellect" || talent.name == "Master Demonologist" || talent.name == "Soul Link" || talent.name == "Demonic Tactics" || talent.name == "Shadow Mastery") {
			refreshCharacterStats();
		}

		// update the point amount on the talent icon
		icon.children('a').children('.talent-point-amount').text(icon.attr('data-points'));

		// if the point amount is 0 then we hide the amount, otherwise we show it
		if (icon.children('a').children('.talent-point-amount').text() > 0) {
			icon.children('a').children('.talent-point-amount').show();
		} else {
			icon.children('a').children('.talent-point-amount').hide();
		}

		icon.children('a').attr('href', 'https://classic.wowhead.com/spell=' + talent.rankIDs[Math.max(0,localStorage[lowerTalentName]-1)]);
		//$WowheadPower.refreshLinks();
	}

	return false;
});

// Listens to any clicks on the "rotation" spells for dots, filler, curse, and finisher.
$("#rotation-list div li").click(function() {
	let clickedSpell = $(this).attr('id');

	if ($(this).hasClass("rotation-filler")) {
		$(".rotation-filler").each(function() {
			$(this).attr('data-checked', false);
			localStorage[$(this).attr('id')] = false;
		});
	} else if ($(this).hasClass("rotation-curse")) {
		$(".rotation-curse").each(function() {
			if ($(this).attr('id') !== clickedSpell) {
				$(this).attr('data-checked', false);
				localStorage[$(this).attr('id')] = false;
			}
		});
	}

	let checkedVal = $(this).attr('data-checked') === 'true';
	$(this).attr('data-checked', !checkedVal);
	localStorage[$(this).attr('id')] = !checkedVal;

	return false;
});

// to-do: don't allow people to start multiple simulations
$(".btn").click(function() {
	//$("#dps-result-div").css('visibility', 'visible');
	simDPS();
});

$(".btn").hover(function() {
	$("#sim-dps").css('color', '#1a1a1a');
});

$(".btn").mouseout(function() {
	$("#sim-dps").css('color', 'white');
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
function loadItemsBySlot(itemSlot, subSlot = "") {
	localStorage['selectedItemSlot'] = itemSlot;
	localStorage['selectedItemSubSlot'] = (subSlot || "");
	// Removes all current item rows
	$(".item-row").remove(); 
	let tableBody = $("#item-selection-table tbody");

	for (let item of Object.keys(items[itemSlot])) {
		let i = items[itemSlot][item];

		// Add the item's gem sockets
		let sockets = [];
		for (let socket in socketInfo) {
			if (i.hasOwnProperty(socket)) {
				for(j = 0; j < i[socket]; j++) {
					sockets.push("<img class='gem' data-color='" + socket + "' src='img/" + socketInfo[socket].iconName + "'>");
				}
			}
		}

		tableBody.append("<tr data-slot='" + itemSlot + "' data-name='" + item + "' data-selected='" + (localStorage['equipped' + itemSlot + (subSlot || "")] == item || 'false') + "' class='item-row' data-wowhead-id='" + i.id + "'><td><a href='https://tbc.wowhead.com/item=" + i.id + "'>" + i.name + "</a></td><td><div>" + sockets.join('') + "</div></td><td>" + i.source + "</td><td>" + (i.stamina || '') + "</td><td>" + (i.intellect || '') + "</td><td>" + (Math.round(i.spellPower + (i.onUseSpellPower * i.duration / i.cooldown)) || i.spellPower || Math.round(i.onUseSpellPower * i.duration / i.cooldown) || '') + "</td><td>" + (i.shadowPower || '') + "</td><td>" + (i.firePower || '') + "</td><td>" + (i.critRating || '') + "</td><td>" + (i.hitRating || '') + "</td><td>" + (Math.round(i.hasteRating + (i.onUseHasteRating * i.duration / i.cooldown)) || i.hasteRating || Math.round(i.onUseHasteRating * i.duration / i.cooldown) || '') + "</td><td>" + (localStorage[item + "Dps"] || '') + "</td></tr>")
		$("[data-name='" + item +"']").data('subSlot', subSlot);
	}

	loadEnchantsBySlot(itemSlot);
}

function loadEnchantsBySlot(itemSlot) {
	if (itemSlot == "mainhand" || itemSlot == "twohand") {
		itemSlot = "weapon";
	}

	if (enchants[itemSlot]) {
		$(".enchant-row").remove(); // Removes all item enchant rows
		let tableBody = $("#enchant-selection-table tbody");

		for (let enchant of Object.keys(enchants[itemSlot])) {
			let e = enchants[itemSlot][enchant];

			tableBody.append("<tr data-slot='" + itemSlot + "' data-name='" + enchant + "' data-selected='" + (localStorage['equipped' + itemSlot + "Enchant"] == enchant || 'false') + "' class='enchant-row' data-wowhead-id='" + e.id + "'><td><a href='https://tbc.wowhead.com/spell=" + e.id + "'>" + e.name + "</a></td><td>" + (e.spellPower || '') + "</td><td>" + (e.shadowPower || '') + "</td><td>" + (e.firePower || '') + "</td><td>" + (e.stamina || '') + "</td><td>" + (e.intellect || '') + "</td><td>" + (e.mp5 || '') + "</td><td>" + (((e.natureResistance || 0) + (e.allResistance || 0)) || '') + "</td><td>" + (((e.shadowResistance || 0) + (e.allResistance || 0)) || '') + "</td><td>" + (((e.fireResistance || 0) + (e.allResistance || 0)) || '') + "</td><td>" + ((e.threatReduction * 100) || '') + "</td><td>" + (e.threatIncrease * 100 || '') + "</td><td>" + (localStorage[enchant + "Dps"] || '') + "</td></tr>")
		}

		$("#enchant-selection-table").show();
	} else {
		$("#enchant-selection-table").hide();
	}

	refreshCharacterStats();
}

// Equips or unequips an item. If the player has the item equipped then unequip it, else equip it.
// loadingUnequippedItems is set to true when the user is loading the website and it needs to add the stats from the user's equipped items from previous sessions.
function modifyStatsFromItem(itemName, itemSlot, subSlot = "", loadingEquippedItems = false) {
	let itemObj = items[itemSlot][itemName];
	let equipped = localStorage['equipped' + itemSlot + subSlot] == itemName || false;

	// If the user has the item equipped and is not loading the stats from equipped items when loading the website
	if (equipped && !loadingEquippedItems) {
		// Loop through the stats on the item and add them to/remove them from the character's stats.
		for (let stat in itemObj) {
			// Check if the item property is a character stat such as stamina/spell power.
			if (characterStats.hasOwnProperty(stat)) {
				characterStats[stat] -= itemObj[stat];
			}
		}

		localStorage.removeItem('equipped' + itemSlot + subSlot);
	} else {
		for (let stat in itemObj) {
			if (characterStats.hasOwnProperty(stat)) {
				characterStats[stat] += itemObj[stat];
			}
		}

		localStorage['equipped' + itemSlot + subSlot] = itemName;
	}

	refreshCharacterStats();
}

function modifyStatsFromEnchant(enchantName, itemSlot, loadingEquippedEnchants = false) {
	let enchantObj = enchants[itemSlot][enchantName];
	let equipped = localStorage['equipped' + itemSlot + 'Enchant'] == enchantName || false;

	if (equipped && !loadingEquippedEnchants) {
		for (let stat in enchantObj) {
			if (characterStats.hasOwnProperty(stat)) {
				characterStats[stat] -= enchantObj[stat];
			}
		}

		localStorage.removeItem("equipped" + itemSlot + "Enchant");
	} else {
		for (let stat in enchantObj) {
			if (characterStats.hasOwnProperty(stat)) {
				characterStats[stat] += enchantObj[stat];
			}
		}

		localStorage['equipped' + itemSlot + 'Enchant'] = enchantName;
	}

	if (!loadingEquippedEnchants) {
		refreshCharacterStats();
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

	refreshCharacterStats();
}

function simDPS() {
	var player = new Player();
	var simulation = new Simulation(player);

	simulation.start();
}