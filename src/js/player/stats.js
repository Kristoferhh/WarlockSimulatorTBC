// Conversion calculations
var hitRatingPerPercent = 12.6;
var critRatingPerPercent = 22.08; // maybe 22.1
var hasteRatingPerPercent = 15.77; // maybe 15.7
var manaPerInt = 15; // todo figure out the correct value
var healthPerStamina = 10;
var critPerInt = 1 / 81.95;

var characterStats = {
	// Normal stats
	"health": 3310,
	"mana": 2335,
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
	// Percentage modifiers
	"damageModifier": 1,
	"shadowModifier": 1,
	"fireModifier": 1,
	"staminaModifier": 1,
	"intellectModifier": 1,
	"spiritModifier": 1,
	// Resistances
	"shadowResist": 0,
	"fireResist": 0,
	"frostResist": 0,
	"natureResist": 0,
	"arcaneResist": 0
}

var raceStats = {
	"gnome": {
		"stamina": 75,
		"intellect": 136,
		"spirit": 131,
		"arcaneResist": 10
	},
	"human": {
		"stamina": 76,
		"intellect": 129,
		"spiritModifier": 1.1
	},
	"orc": {
		"stamina": 78,
		"intellect": 126,
		"petDamageModifier": 1.05
	},
	"undead": {
		"stamina": 77,
		"intellect": 131,
		"shadowResist": 10
	},
	"bloodElf": {
		"stamina": 74,
		"intellect": 133,
		"fireResist": 5,
		"frostResist": 5,
		"shadowResist": 5,
		"arcaneResist": 5,
		"natureResist": 5
	}
}

// Refreshes the player's values in the sidebar when changing e.g. gear, consumables, or buffs.
function refreshCharacterStats() {
	$("#character-stamina-val").text(Math.round(characterStats.stamina * characterStats.staminaModifier));
	$("#character-intellect-val").text(Math.round(characterStats.intellect * characterStats.intellectModifier));
	$("#character-spell-power-val").text(characterStats.spellPower);
	$("#character-shadow-power-val").text(characterStats.shadowPower + " (" + (characterStats.shadowPower + characterStats.spellPower) + ")");
	$("#character-fire-power-val").text(characterStats.firePower + " (" + (characterStats.firePower + characterStats.spellPower) + ")");
	$("#character-crit-val").text(Math.round(characterStats.critRating) + " (" + (Math.round((characterStats.critRating/critRatingPerPercent) * 100) / 100) + "%)");
	$("#character-hit-val").text(Math.round(characterStats.hitRating) + " (" + (Math.round((characterStats.hitRating/hitRatingPerPercent) * 100) / 100) + "%)");
	$("#character-shadow-damage-modifier-val").text(Math.round(characterStats.shadowModifier * 100) + "%");
	$("#character-fire-damage-modifier-val").text(Math.round(characterStats.fireModifier * 100) + "%");
	$("#character-mp5-val").text(characterStats.mp5);
	$("#character-spell-pen-val").text(characterStats.spellPen);
}