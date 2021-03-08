var hitRatingPerPercent = 12.6;
var critRatingPerPercent = 22.1;
var hasteRatingPerPercent = 15.7;
var characterStats = {
	// Initializing normal stats
	"stamina": 0,
	"intellect": 0,
	"spirit": 0,
	"spellPower": 0,
	"shadowPower": 0,
	"firePower": 0,
	"critRating": 0,
	"hitRating": 0,
	"critPercent": 0,
	"mp5": 0,
	"spellPen": 0,
	// Initializing percentage modifiers
	"damageModifier": 1,
	"shadowModifier": 1,
	"fireModifier": 1,
	"staminaModifier": 1,
	"intellectModifier": 1,
	"spiritModifier": 1,
	// Initializing resistances
	"shadowResist": 0,
	"fireResist": 0,
	"frostResist": 0,
	"natureResist": 0,
	"arcaneResist": 0
}

// Refreshes the player's values in the sidebar when changing e.g. gear, consumables, or buffs.
function refreshCharacterStats() {
	$("#character-stamina-val").text(Math.round(characterStats.stamina * characterStats.staminaModifier));
	$("#character-intellect-val").text(Math.round(characterStats.intellect * characterStats.intellectModifier));
	$("#character-spell-power-val").text(characterStats.spellPower);
	$("#character-shadow-power-val").text(characterStats.shadowPower + " (" + (characterStats.shadowPower + characterStats.spellPower) + ")");
	$("#character-fire-power-val").text(characterStats.firePower + " (" + (characterStats.firePower + characterStats.spellPower) + ")");
	$("#character-crit-val").text(characterStats.critRating + " (" + (Math.round((characterStats.critRating/critRatingPerPercent) * 100) / 100) + "%)");
	$("#character-hit-val").text(characterStats.hitRating + " (" + (Math.round((characterStats.hitRating/hitRatingPerPercent) * 100) / 100) + "%)");
	$("#character-shadow-damage-modifier-val").text((characterStats.shadowModifier * 100) + "%");
	$("#character-fire-damage-modifier-val").text((characterStats.fireModifier * 100) + "%");
	$("#character-mp5-val").text(characterStats.mp5);
	$("#character-spell-pen-val").text(characterStats.spellPen);
}