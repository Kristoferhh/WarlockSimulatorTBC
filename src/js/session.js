// Add the stats from all the items the user has equipped from previous sessions
for (let itemSlot in items) {
	//todo fix this mess
	if (localStorage['equipped' + itemSlot] || localStorage['equipped' + itemSlot + '1'] || localStorage['equipped' + itemSlot + '2']) {
		if (localStorage['equipped' + itemSlot]) {
			modifyStatsFromItem(localStorage['equipped' + itemSlot], itemSlot, "", true);
		} else {
			if (localStorage['equipped' + itemSlot + '1']) modifyStatsFromItem(localStorage['equipped' + itemSlot + '1'], itemSlot, '1', true);
			if (localStorage['equipped' + itemSlot + '2']) modifyStatsFromItem(localStorage['equipped' + itemSlot + '2'], itemSlot, '2', true);
		}
	}
}

// Add stats from the player's selected race
let currentRace = $("#race-dropdown-list").val();
if (currentRace == "gnome") {
	characterStats.intellectModifier *= 1.05;
}
for (let stat in raceStats[currentRace]) {
	if (characterStats.hasOwnProperty(stat)) {
		characterStats[stat] += raceStats[currentRace][stat];
	}
}

refreshCharacterStats();
loadItemsBySlot(localStorage['selectedItemSlot'] || "mainhand", (localStorage['selectedItemSubSlot'] || ""));
$("#race-dropdown-list").data("currentRace", currentRace); // Store the currently selected race to know the user's previous race when changing in the dropdown list.
$("#race").text($("#race-dropdown-list").children("option:selected").text()); // Set the player's race at the top of the sidebar (just a visual description)