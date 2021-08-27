// Load saved simulation settings
$('#sim-settings select, #sim-settings input').each(function () {
  if (settings[$(this).attr('name')]) {
    if ($(this).is(':checkbox')) {
      $(this).attr('checked', settings[$(this).attr('name')])
    } else if ($(this).is(':radio')) {
      $(this).attr('checked', $(this).val() == settings[$(this).attr('name')])
    } else {
      $(this).val(settings[$(this).attr('name')])
    }
  } else {
    // If the setting is not initialized in the settings object then set it to its default value
    settings[$(this).attr('name')] = $(this).val()
  }
})

// Load checked items sources
$('#source-filters ul li').each(function () {
  const s = $(this).attr('data-source')
  const val = $(this).attr('data-value')
  $(this).attr('data-checked', sources[s] && sources[s][val] || false)
})

// Add the stats from all the items the user has equipped from previous sessions
for (const itemSlot in selectedItems) {
  for (const slot in items) {
    for (const item in items[slot]) {
      if (items[slot][item].id == selectedItems[itemSlot]) {
        modifyStatsFromItem(items[slot][item], 'add')
        break
      }
    }
  }
}

// Same as above but for enchants
for (const itemSlot in selectedEnchants) {
  for (const slot in enchants) {
    for (const enchant in enchants[slot]) {
      if (enchants[slot][enchant].id == selectedEnchants[itemSlot]) {
        modifyStatsFromEnchant(enchants[slot][enchant].id, 'add')
        break
      }
    }
  }
}

// Add stats from gems in equipped items
for (itemSlot in selectedItems) {
  const itemId = selectedItems[itemSlot]
  if (selectedGems[itemSlot] && selectedGems[itemSlot][itemId]) {
    for (gem in selectedGems[itemSlot][itemId]) {
      const gemArray = selectedGems[itemSlot][itemId][gem]
      if (gemArray) {
        modifyStatsFromGem(gemArray[1], 'add')
      }
    }
    // Add stats from the socket bonus if the gem colors match the socket colors
    if (itemMeetsSocketRequirements(itemId)) {
      modifyStatsFromItemSocketBonus(itemId, 'add')
    }
  }
}

// Add stats from the player's selected race
const currentRace = $('#race-dropdown-list').val()
for (const stat in raceStats[currentRace]) {
  if (characterStats.hasOwnProperty(stat)) {
    // Check if the buff is a modifier to know whether to add/subtract or multiply/divide the stat
    if (stat.toLowerCase().search('modifier') !== -1) {
      characterStats[stat] *= raceStats[currentRace][stat]
    } else {
      characterStats[stat] += raceStats[currentRace][stat]
    }
  }
}

// Add stats from selected auras
for (const auraType in _auras) {
  for (const aura in _auras[auraType].auras) {
    if (auras[aura]) {
      modifyStatsFromAura(_auras[auraType].auras[aura], false)
    }
  }
}

// Use previous simulation's result on the sidebar
$('#avg-dps').text(localStorage.medianDps || '')
$('#min-dps').text(localStorage.minDps || '')
$('#max-dps').text(localStorage.maxDps || '')
if (localStorage.simulationDuration) {
  $('#sim-length-result').text(Math.round(localStorage.simulationDuration * 10000) / 10000 + 's')
}

// Add buttons for the saved settings selection
drawProfileButtons()
// Show the Save/Delete/Rename buttons if a profile is selected
if (localStorage.selectedProfile) {
  $('#update-profile-div button').show()
}

$('#race-dropdown-list').data('currentRace', currentRace) // Store the currently selected race to know the user's previous race when changing in the dropdown list.
$('#race').text($('#race-dropdown-list').children('option:selected').text()) // Set the player's race at the top of the sidebar (just a visual description)
// Initialize tablesorter on the item list and breakdown tables
$('#item-selection-table').tablesorter()
$('#damage-breakdown-table').tablesorter()
$('#aura-breakdown-table').tablesorter()
$('#mana-gain-breakdown-table').tablesorter()
loadItemsBySlot(localStorage.selectedItemSlot, localStorage.selectedItemSubSlot)
let newItemSelector = "#item-slot-selection-list li[data-slot='" + (localStorage.selectedItemSlot || 'mainhand') + "']"
if (localStorage.selectedItemSubSlot) newItemSelector += "[data-subslot='" + localStorage.selectedItemSubSlot + "']"
$(newItemSelector).attr('data-selected', 'true')
refreshCharacterStats()
updateSimulationSettingsVisibility()
updateSetBonuses()
