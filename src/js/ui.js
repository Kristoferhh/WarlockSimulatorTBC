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

	consumableUl.append($("<li data-checked='" + localStorage[consumable] + "' name='" + consumable + "' id='" + lowerConsumableName + "' class='" + (c.stats ? "stats" : "") + " consumable aura'><a href='https://classic.wowhead.com/item=" + c.id + "'><img alt='" + c.name + "' src='img/" + c.iconName + ".jpg'></a></li>"));
}

// When a buff/debuff/consumable is clicked
$(".aura").click(function() {
	let checkedVal = $(this).attr('data-checked') == 'true';
	$(this).attr('data-checked', !checkedVal);
	localStorage[$(this).attr("name")] = true;

	return false;
});