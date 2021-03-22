// Convertion calculations
const hitRatingPerPercent = 12.6;
const critRatingPerPercent = 22.08; // maybe 22.1
const hasteRatingPerPercent = 15.77; // maybe 15.7
const manaPerInt = 15;
const healthPerStamina = 10;
const critPerInt = 1 / 81.95;
const baseCritChancePercent = 1.701;

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
	"hasteRating": 0,
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
	let staminaModifier = characterStats.staminaModifier * (1 + (0.03 * talents.demonicEmbrace));
	let stamina = characterStats.stamina;
	// Blood Pact
	if ($("#petChoice select").children("option:selected").val() === "imp" && (talents.demonicSacrifice === 0 || (talents.masterDemonologist > 0 && $("#sacrificePet").children("option:selected").val() === "no"))) {
		stamina += 70 * (1 + 0.1 * talents.improvedImp);
	}
	let critRating = characterStats.critRating + ((characterStats.intellect * characterStats.intellectModifier) * critPerInt);
	let critChance = Math.round((critRating / critRatingPerPercent + baseCritChancePercent) * 100) / 100 + talents.devastation + talents.backlash + talents.demonicTactics;
	if (auras.moonkinAura) {
		critChance += 5;
	}
	if (auras.judgementOfTheCrusader) {
		critChance += 3;
	}
	critChance = critChance.toFixed(2);

	let hitChance = Math.round((characterStats.hitRating / hitRatingPerPercent) * 100) / 100;
	if (auras.inspiringPresence) {
		hitChance += 1;
	}
	hitChance = hitChance.toFixed(2);

	let shadowModifier = characterStats.shadowModifier;
	let fireModifier = characterStats.fireModifier;
	if (talents.demonicSacrifice === 1 && (talents.masterDemonologist === 0 || $("#sacrificePet").children('option:selected').val() === 'yes')) {
		if (rotation.filler.shadowBolt) {
			shadowModifier *= 1.15;
		} else if (rotation.filler.incinerate || rotation.filler.searingPain) {
			fireModifier *= 1.15;
		}
	}

	let spellPower = characterStats.spellPower;
	if (auras.felArmor) {
		spellPower += 100 * (1 + 0.1 * talents.demonicAegis);
	}

	$("#character-health-val").text(Math.round((characterStats.health + (stamina * staminaModifier) * healthPerStamina) * (1 + (0.01 * talents.felStamina))));
	$("#character-mana-val").text(Math.round((characterStats.mana + (characterStats.intellect * characterStats.intellectModifier) * manaPerInt) * (1 + (0.01 * talents.felIntellect))));
	$("#character-stamina-val").text(Math.round(stamina * staminaModifier));
	$("#character-intellect-val").text(Math.round(characterStats.intellect * characterStats.intellectModifier));
	$("#character-spell-power-val").text(spellPower);
	$("#character-shadow-power-val").text(characterStats.shadowPower + " (" + (characterStats.shadowPower + spellPower) + ")");
	$("#character-fire-power-val").text(characterStats.firePower + " (" + (characterStats.firePower + spellPower) + ")");
	$("#character-crit-val").text(Math.round(critRating) + " (" + critChance + "%)");
	$("#character-hit-val").text(Math.round(characterStats.hitRating) + " (" + hitChance + "%)");
	$("#character-haste-val").text(Math.round(characterStats.hasteRating) + " (" + (Math.round((characterStats.hasteRating/hasteRatingPerPercent) * 100) / 100) + "%)");
	$("#character-shadow-damage-modifier-val").text(Math.round((shadowModifier * (1 + (0.02 * talents.shadowMastery))) * 100) + "%");
	$("#character-fire-damage-modifier-val").text(Math.round(fireModifier * 100) + "%");
	$("#character-mp5-val").text(characterStats.mp5);
}