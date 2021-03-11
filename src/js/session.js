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

			lastRow.append($("<td><div data-points='" + (localStorage[lowerTalentName] || 0) + "' class='talent-icon' data-tree='" + tree + "' id='" + talent + "'><a href='https://tbc.wowhead.com/spell=" + t.rankIDs[Math.max(0,localStorage[lowerTalentName]-1)] + "'><img src='img/" + t.iconName + ".jpg' alt='" + t.name + "'><span id='" + lowerTalentName + "-point-amount' class='talent-point-amount'>" + localStorage[lowerTalentName] + "</span></a></div></td>"));
			
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

refreshCharacterStats();
loadItemsBySlot(localStorage['selectedItemSlot'] || "mainhand", (localStorage['selectedItemSubSlot'] || ""));
$("#race-dropdown-list").data("currentRace", currentRace); // Store the currently selected race to know the user's previous race when changing in the dropdown list.
$("#race").text($("#race-dropdown-list").children("option:selected").text()); // Set the player's race at the top of the sidebar (just a visual description)