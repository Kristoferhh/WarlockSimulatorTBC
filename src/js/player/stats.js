// Convertion calculations
const hitRatingPerPercent = 12.6
const critRatingPerPercent = 22.08
const hasteRatingPerPercent = 15.77 // confirm, maybe 15.7?
const manaPerInt = 15
const healthPerStamina = 10
const critPerInt = 1 / 81.95 // Crit % per point of intellect
const baseCritChancePercent = 1.701

const characterStats = {
  // Normal stats
  health: 3310,
  mana: 2335,
  stamina: 0,
  intellect: 0,
  spirit: 0,
  spellPower: 0,
  shadowPower: 0,
  firePower: 0,
  critRating: 0,
  hitRating: 0,
  hasteRating: 0,
  critPercent: 0,
  mp5: 0,
  spellPen: 0,
  // Percentage modifiers
  damageModifier: 1,
  shadowModifier: 1,
  fireModifier: 1,
  natureModifier: 1,
  arcaneModifier: 1,
  frostModifier: 1,
  staminaModifier: 1,
  intellectModifier: 1,
  spiritModifier: 1,
  // Resistances
  shadowResist: 0,
  fireResist: 0,
  frostResist: 0,
  natureResist: 0,
  arcaneResist: 0
}

const raceStats = {
  gnome: {
    stamina: 75,
    intellect: 136,
    spirit: 131,
    arcaneResist: 10,
    intellectModifier: 1.05
  },
  human: {
    stamina: 76,
    intellect: 129,
    spirit: 132,
    spiritModifier: 1.1
  },
  orc: {
    stamina: 78,
    intellect: 126,
    spirit: 134,
    petDamageModifier: 1.05
  },
  undead: {
    stamina: 77,
    intellect: 131,
    spirit: 144,
    shadowResist: 10
  },
  bloodElf: {
    stamina: 74,
    intellect: 133,
    spirit: 130,
    fireResist: 5,
    frostResist: 5,
    shadowResist: 5,
    arcaneResist: 5,
    natureResist: 5
  }
}

// Refreshes the player's values in the sidebar when changing e.g. gear, consumables, or buffs.
function refreshCharacterStats () {
  const spiritModifier = characterStats.spiritModifier * (1 - (0.01 * talents.demonicEmbrace))
  const staminaModifier = characterStats.staminaModifier * (1 + (0.03 * talents.demonicEmbrace))
  let stamina = characterStats.stamina

  // Crit
  let critRating = characterStats.critRating
  if (auras.powerOfTheGuardianMage) critRating += 28 * $("select[name='mageAtieshAmount']").val()
  let critChance = Math.round((critRating / critRatingPerPercent + baseCritChancePercent) * 100) / 100 + talents.devastation + talents.backlash + talents.demonicTactics + ((characterStats.intellect * characterStats.intellectModifier) * critPerInt)
  if (auras.moonkinAura) critChance += 5
  if (auras.judgementOfTheCrusader) critChance += 3
  if (auras.totemOfWrath) critChance += 3 * $("select[name='totemOfWrathAmount']").val()
  critChance = critChance.toFixed(2)

  // Hit
  let hitRating = JSON.parse(JSON.stringify(characterStats.hitRating))
  // Mana-Etched Regalia 2-set
  if (localStorage.getItem('setBonuses') && JSON.parse(localStorage.setBonuses)['658'] >= 2) hitRating += 35
  let hitChance = Math.round((hitRating / hitRatingPerPercent) * 100) / 100
  if (auras.inspiringPresence) hitChance += 1
  if (auras.totemOfWrath) hitChance += 3 * $("select[name='totemOfWrathAmount']").val()
  hitChance = hitChance.toFixed(2)

  // Shadow/Fire damage % modifiers
  let shadowModifier = characterStats.shadowModifier
  let fireModifier = characterStats.fireModifier
  // Add stats from pets/pet-related talents
  if (talents.demonicSacrifice === 1 && $("select[name='sacrificePet']").val() === 'yes') {
    const pet = $("select[name='petChoice']").val()
    if (pet == PetName.IMP) {
      fireModifier *= 1.15
    } else if (pet == PetName.SUCCUBUS) {
      shadowModifier *= 1.15
    } else if (pet == PetName.FELGUARD) {
      shadowModifier *= 1.1
    }
  } else {
    // Blood Pact
    if ($("select[name='petChoice']").val() === PetName.IMP) {
      stamina += 70 * (1 + 0.1 * talents.improvedImp)
    }
    // Master Demonologist
    if (talents.masterDemonologist > 0) {
      if ($("select[name='petChoice']").val() == PetName.SUCCUBUS) {
        shadowModifier *= 1.1
        fireModifier *= 1.1
      } else if ($("select[name='petChoice']").val() == PetName.FELGUARD) {
        shadowModifier *= 1.05
        fireModifier *= 1.05
      }
    }
    // Soul Link
    if (talents.soulLink > 0) {
      shadowModifier *= 1.05
      fireModifier *= 1.05
    }
  }
  if (auras.curseOfTheElements) {
    shadowModifier *= 1.1 + (0.01 * $("select[name='improvedCurseOfTheElements']").val())
    fireModifier *= 1.1 + (0.01 * $("select[name='improvedCurseOfTheElements']").val())
  }
  if (talents.emberstorm > 0) {
    fireModifier *= 1 + (0.02 * talents.emberstorm)
  }

  // Spell Power
  let spellPower = JSON.parse(JSON.stringify(characterStats.spellPower))
  if (localStorage.getItem('setBonuses') && JSON.parse(localStorage.setBonuses)['667'] == 2) spellPower += 15 // The Twin Stars 2-set bonus (15 spellpower)
  if (auras.felArmor) spellPower += 100 * (1 + 0.1 * talents.demonicAegis)
  if (auras.prayerOfSpirit) spellPower += (characterStats.spirit * spiritModifier * (0.1 * $("select[name='improvedDivineSpirit']").val()))
  if (auras.powerOfTheGuardianWarlock) spellPower += 33 * $("select[name='warlockAtieshAmount']").val()
  spellPower = Math.round(spellPower)

  // MP5
  let mp5 = characterStats.mp5
  // Add mp5 from Vampiric Touch
  if (auras.vampiricTouch) {
    mp5 += settings.shadowPriestDps * 0.05
  }

  // Enemy armor
  let enemyArmor = $("input[name='enemyArmor']").val()
  if (auras.faerieFire) enemyArmor -= 610
  if ((auras.sunderArmor && auras.exposeArmor && settings.improvedExposeArmor == 2) || (auras.exposeArmor && !auras.sunderArmor)) enemyArmor -= 2050 * (1 + 0.25 * settings.improvedExposeArmor)
  else if (auras.sunderArmor) enemyArmor -= 520 * 5
  if (auras.curseOfRecklessness) enemyArmor -= 800

  $('#character-health-val').text(Math.round((characterStats.health + (stamina * staminaModifier) * healthPerStamina) * (1 + (0.01 * talents.felStamina))))
  $('#character-mana-val').text(Math.round((characterStats.mana + (characterStats.intellect * characterStats.intellectModifier) * manaPerInt) * (1 + (0.01 * talents.felIntellect))))
  $('#character-stamina-val').text(Math.round(stamina * staminaModifier))
  $('#character-intellect-val').text(Math.round(characterStats.intellect * characterStats.intellectModifier))
  $('#character-spell-power-val').text(spellPower)
  $('#character-shadow-power-val').text(characterStats.shadowPower + ' (' + (characterStats.shadowPower + spellPower) + ')')
  $('#character-fire-power-val').text(characterStats.firePower + ' (' + (characterStats.firePower + spellPower) + ')')
  $('#character-crit-val').text(Math.round(critRating) + ' (' + critChance + '%)')
  $('#character-hit-val').text(Math.round(hitRating) + ' (' + hitChance + '%)')
  $('#character-haste-val').text(Math.round(characterStats.hasteRating) + ' (' + (Math.round((characterStats.hasteRating / hasteRatingPerPercent) * 100) / 100) + '%)')
  $('#character-shadow-damage-modifier-val').text(Math.round((shadowModifier * (1 + (0.02 * talents.shadowMastery))) * 100) + '%')
  $('#character-fire-damage-modifier-val').text(Math.round(fireModifier * 100) + '%')
  $('#character-mp5-val').text(Math.round(mp5))
  $('#enemy-armor-val').text(Math.max(0, enemyArmor))
}
