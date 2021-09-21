// Listens to any clicks on the "rotation" spells for dots, filler, curse, and finisher.
$(document).on('click', '#rotation-list div li', function () {
  const clickedSpell = $(this).data('name')
  let refreshStats = false

  if ($(this).hasClass('rotation-filler')) {
    $('.rotation-filler').each(function () {
      $(this).attr('data-checked', false)
      rotation[$(this).data('type')][$(this).data('name')] = false
    })

    if ($('#demonicSacrifice').data('points') == 1) {
      refreshStats = true
    }
  } else if ($(this).hasClass('rotation-curse')) {
    $('.rotation-curse').each(function () {
      if ($(this).data('name') !== clickedSpell) {
        $(this).attr('data-checked', false)
        rotation[$(this).data('type')][$(this).data('name')] = false
      }
    })
  }

  const checkedVal = $(this).attr('data-checked') === 'true'
  $(this).attr('data-checked', !checkedVal)
  rotation[$(this).data('type')][$(this).data('name')] = !checkedVal
  localStorage.rotation = JSON.stringify(rotation)
  if (refreshStats) refreshCharacterStats()
  return false
})

// Fired when the user changes any settings such as fight length, iteration amount, etc.
$('#sim-settings select, #sim-settings input').change(function () {
  if ($(this).is(':checkbox')) {
    settings[$(this).attr('name')] = $(this).is(':checked')
  } else {
    settings[$(this).attr('name')] = $(this).val()
  }
  localStorage.settings = JSON.stringify(settings)
  refreshCharacterStats()
  updateSimulationSettingsVisibility()
})

// User changes races in the simulation settings
$('#race-dropdown-list').change(function () {
  const oldRace = $(this).data('currentRace')
  const newRace = $(this).val()
  $(this).data('currentRace', newRace)

  // Remove the previous race's stats
  for (const stat in raceStats[oldRace]) {
    if (characterStats.hasOwnProperty(stat)) {
      // Check if the buff is a modifier to know whether to add/subtract or multiply/divide the stat
      if (stat.toLowerCase().search('modifier') !== -1) {
        characterStats[stat] /= raceStats[oldRace][stat]
      } else {
        characterStats[stat] -= raceStats[oldRace][stat]
      }
    }
  }

  // Add the new race's stats
  for (const stat in raceStats[newRace]) {
    if (characterStats.hasOwnProperty(stat)) {
      // Check if the buff is a modifier to know whether to add/subtract or multiply/divide the stat
      if (stat.toLowerCase().search('modifier') !== -1) {
        characterStats[stat] *= raceStats[newRace][stat]
      } else {
        characterStats[stat] += raceStats[newRace][stat]
      }
    }
  }

  $('#race').text($('#race-dropdown-list').children('option:selected').text())
  refreshCharacterStats()
})

function updateSimulationSettingsVisibility () {
  if ($('#sim-settings input[name="rotationOption"]:checked').val() == 'simChooses') {
    $('#rotation-list div').hide()
    $('#rotation-curse').show()
  } else {
    $('#rotation-list div').show()
  }

  if (talents.demonicSacrifice === 0) {
    $('#sacrificePet').hide()
  } else {
    $('#sacrificePet').show()
  }

  if ($('#sacrificePet').is(':visible') && $('#sacrificePet').children('select').val() == 'yes') {
    $('#petMode').hide()
  } else {
    $('#petMode').show()
  }

  if ($('#sacrificePet').children('select').val() == 'no' || !$('#sacrificePet').is(':visible')) {
    $('#petBuffs-heading').show()
    $('.petBuffs').show()
    if ($('#petMode').children('select').val() == PetMode.AGGRESSIVE) {
      $('#prepopBlackBook').show()
      if ($('#petChoice').children('select').val() != PetName.IMP) {
        $('#enemyArmor').show()
        $('#enemy-armor-val').closest('li').show()
      } else {
        $('#enemyArmor').hide()
        $('#enemy-armor-val').closest('li').hide()
      }
      $('.petDebuff').show()
    } else {
      $('#prepopBlackBook').hide()
      $('#enemyArmor').hide()
      $('#enemy-armor-val').closest('li').hide()
      $('.petDebuff').hide()
    }
  } else {
    $('#prepopBlackBook').hide()
    $('#petBuffs-heading').hide()
    $('.petBuffs').hide()
    $('#enemyArmor').hide()
    $('#enemy-armor-val').closest('li').hide()
    $('.petDebuff').hide()
  }

  if ($('#petChoice').children('select').val() == PetName.SUCCUBUS && $('#petMode').children('select').val() == PetMode.AGGRESSIVE && (talents.demonicSacrifice == 0 || $('#sacrificePet').children('select').val() === 'no')) {
    $('#lashOfPainUsage').show()
  } else {
    $('#lashOfPainUsage').hide()
  }

  if (talents.summonFelguard === 0) {
    $("#petChoice option[value='felguard']").hide()
  } else {
    $("#petChoice option[value='felguard']").show()
  }

  if (auras.curseOfTheElements) {
    $('#improvedCurseOfTheElements').show()
  } else {
    $('#improvedCurseOfTheElements').hide()
  }

  if (auras.prayerOfSpirit) {
    $('#improvedDivineSpirit').show()
  } else {
    $('#improvedDivineSpirit').hide()
  }

  /* if (talents.conflagrate > 0) {
    $('#conflagrateUse').show()
  } else {
    $('#conflagrateUse').hide()
  } */

  if (auras.powerOfTheGuardianMage) {
    $('#mageAtieshAmount').show()
  } else {
    $('#mageAtieshAmount').hide()
  }

  if (auras.ferociousInspiration) {
    $('#ferociousInspirationAmount').show()
  } else {
    $('#ferociousInspirationAmount').hide()
  }

  if (auras.powerOfTheGuardianWarlock) {
    $('#warlockAtieshAmount').show()
  } else {
    $('#warlockAtieshAmount').hide()
  }

  if (auras.powerInfusion) {
    $('#powerInfusionAmount').show()
  } else {
    $('#powerInfusionAmount').hide()
  }

  if (auras.bloodlust) {
    $('#bloodlustAmount').show()
  } else {
    $('#bloodlustAmount').hide()
  }

  if (auras.innervate) {
    $('#innervateAmount').show()
  } else {
    $('#innervateAmount').hide()
  }

  if (auras.totemOfWrath) {
    $('#totemOfWrathAmount').show()
  } else {
    $('#totemOfWrathAmount').hide()
  }

  if (auras.vampiricTouch) {
    $('#shadowPriestDps').show()
  } else {
    $('#shadowPriestDps').hide()
  }

  if (auras.bloodPact) {
    $('#improvedImp').show()
  } else {
    $('#improvedImp').hide()
  }

  if ($('#faerieFire').is(':visible') && auras.faerieFire) {
    $('#improvedFaerieFire').show()
  } else {
    $('#improvedFaerieFire').hide()
  }

  if ($('#exposeArmor').is(':visible') && auras.exposeArmor) {
    $('#improvedExposeArmor').show()
  } else {
    $('#improvedExposeArmor').hide()
  }

  if ($('#exposeWeakness').is(':visible') && auras.exposeWeakness) {
    $('#survivalHunterAgility').show()
    $('#exposeWeaknessUptime').show()
  } else {
    $('#survivalHunterAgility').hide()
    $('#exposeWeaknessUptime').hide()
  }

  if ($('#customIsbUptime').children('select').val() == 'yes') {
    $('#custom-isb-uptime-value').show()
  } else {
    $('#custom-isb-uptime-value').hide()
  }

  if ($('#fight-type').val() == 'singleTarget') {
    $('#enemy-amount').hide()
  } else {
    $('#enemy-amount').show()
  }

  if (selectedItems.neck == 34678) {
    $('#shattrathFaction').show()
    $('#shattrathFactionReputation').show()
  } else {
    $('#shattrathFaction').hide()
    $('#shattrathFactionReputation').hide()
  }

  if (auras.wrathOfAirTotem) {
    $('#improvedWrathOfAirTotem').show()
  } else {
    $('#improvedWrathOfAirTotem').hide()
  }
}
