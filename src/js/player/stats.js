// Initializing normal stats
var stamina = intellect = spirit = spellPower = shadowPower = firePower = critRating = critPercent = hitRating = mp5 = spellPen = 0;
// Initializing percentage modifiers
var damageModifier = shadowModifier = fireModifier = staminaModifier = intellectModifier = spiritModifier = 1;
// Initializing resistances
var shadowResist = fireResist = frostResist = natureResist = arcaneResist = 0;

// Refreshes the player's values in the sidebar when changing e.g. gear, consumables, or buffs.
function refreshCharacterStats() {
	$("#character-stamina-val").text(stamina);
	$("#character-intellect-val").text(intellect);
	$("#character-spell-power-val").text(spellPower);
	$("#character-shadow-power-val").text(shadowPower + " (" + (shadowPower + spellPower) + ")");
	$("#character-fire-power-val").text(firePower + " (" + (firePower + spellPower) + ")");
	$("#character-crit-val").text(critRating + " (0%)");
	$("#character-hit-val").text(hitRating + " (0%)");
	$("#character-damage-modifier-val").text((damageModifier * 100) + "%");
	$("#character-mp5-val").text(mp5);
	$("#character-spell-pen-val").text(spellPen);
}