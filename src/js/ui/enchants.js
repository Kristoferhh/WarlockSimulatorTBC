// User clicks on an enchant
$('#enchant-selection-table tbody').on('click', 'tr', function (event) {
  const itemSlot = $(this).attr('data-slot')
  const subSlot = $(this).attr('data-subslot') || ''
  const enchantID = $(this).data('wowhead-id')

  // Toggle the enchant's data-selected boolean.
  const equipped = $(this).attr('data-selected') == 'true' && selectedEnchants[itemSlot + subSlot] && selectedEnchants[itemSlot + subSlot] == enchantID
  $(this).attr('data-selected', !equipped)

  // Check if the user already has an enchant equipped in this slot and unequip it if so
  if (selectedEnchants[itemSlot + subSlot]) {
    $("#enchant-selection-table tr[data-wowhead-id='" + selectedEnchants[itemSlot + subSlot] + "']").attr('data-selected', 'false')
    modifyStatsFromEnchant(selectedEnchants[itemSlot + subSlot], 'remove')
    selectedEnchants[itemSlot + subSlot] = null
  }

  if (!equipped) {
    modifyStatsFromEnchant(enchantID, 'add')
    selectedEnchants[itemSlot + subSlot] = enchantID
  }
  localStorage.selectedEnchants = JSON.stringify(selectedEnchants)
  refreshCharacterStats()
  return false
})

// Adds or removes an enchant's stats from the player
function modifyStatsFromEnchant (enchantID, action) {
  for (const itemSlot in enchants) {
    for (const enchant in enchants[itemSlot]) {
      if (enchants[itemSlot][enchant].id == enchantID) {
        const enchantObj = enchants[itemSlot][enchant]

        if (action == 'remove') {
          for (const stat in enchantObj) {
            if (characterStats.hasOwnProperty(stat)) {
              characterStats[stat] -= enchantObj[stat]
            }
          }
        } else if (action == 'add') {
          for (const stat in enchantObj) {
            if (characterStats.hasOwnProperty(stat)) {
              characterStats[stat] += enchantObj[stat]
            }
          }
        }
        return
      }
    }
  }
}

function loadEnchantsBySlot (itemSlot, subSlot = null) {
  if (itemSlot == 'mainhand' || itemSlot == 'twohand') {
    itemSlot = 'weapon'
  }

  if (enchants[itemSlot]) {
    $('.enchant-row').remove()
    const tableBody = $('#enchant-selection-table tbody')

    for (const enchant of Object.keys(enchants[itemSlot])) {
      const e = enchants[itemSlot][enchant]

      tableBody.append("<tr data-slot='" + itemSlot + "' data-subslot='" + (subSlot || '') + "' data-name='" + enchant + "' data-selected='" + (selectedEnchants[itemSlot + (subSlot || '')] == e.id || 'false') + "' class='enchant-row' data-wowhead-id='" + e.id + "'><td><a href='https://tbc.wowhead.com/spell=" + e.id + "'>" + e.name + '</a></td><td>' + (e.spellPower || '') + '</td><td>' + (e.shadowPower || '') + '</td><td>' + (e.firePower || '') + '</td><td>' + (e.hitRating || '') + '</td><td>' + (e.critRating || '') + '</td><td>' + (e.stamina || '') + '</td><td>' + (e.intellect || '') + '</td><td>' + (e.mp5 || '') + '</td><td>' + (e.spellPen || '') + '</td><td>' + (localStorage[enchant + 'Dps'] || '') + '</td></tr>')
    }

    $('#enchant-selection-table').show()
  } else {
    $('#enchant-selection-table').hide()
  }

  refreshCharacterStats()
}