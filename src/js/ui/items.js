// User clicks on an item in the item table
$('#item-selection-table tbody').on('click', 'tr', function () {
  const itemSlot = $(this).attr('data-slot')
  const itemName = $(this).attr('data-name')
  const itemId = $(this).closest('tr').data('wowhead-id')
  const subSlot = localStorage.selectedItemSubSlot || $(this).data('subslot') || '' // Workaround for having two selections for rings and trinkets but only one selection for the other slots.

  // Toggle the item's data-selected boolean.
  const equipped = $(this).attr('data-selected') == 'true' && selectedItems[itemSlot + subSlot] && selectedItems[itemSlot + subSlot] == itemId
  $(this).attr('data-selected', !equipped)

  // Check if the user already has an item equipped in this slot and unequip it if so
  if (selectedItems[itemSlot + subSlot]) {
    // Set the old item's data-selected value to false and remove the item's stats from the player.
    $('[data-wowhead-id="' + selectedItems[itemSlot + subSlot] + '"]').attr('data-selected', false)
    itemLoop:
    for (const slot in items) {
      for (const item in items[slot]) {
        if (items[slot][item].id == selectedItems[itemSlot + subSlot]) {
          // Remove the stats from the item
          modifyStatsFromItem(items[slot][item], 'remove')
          // Remove socket bonus if active
          if ($(".item-row[data-wowhead-id='" + items[slot][item].id + "']").attr('data-socket-bonus-active') == 'true') {
            modifyStatsFromItemSocketBonus(items[slot][item].id, 'remove')
          }

          // Remove stats from gems equipped in the item
          if (selectedGems[slot] && selectedGems[slot][selectedItems[itemSlot + subSlot]]) {
            for (gemArray of selectedGems[slot][selectedItems[itemSlot + subSlot]]) {
              if (gemArray) {
                modifyStatsFromGem(gemArray[1], 'remove')
              }
            }
          }

          selectedItems[itemSlot + subSlot] = null
          break itemLoop
        }
      }
    }
  }

  // Add the stats from the item
  if (!equipped) {
    modifyStatsFromItem(items[itemSlot][itemName], 'add')
    selectedItems[itemSlot + subSlot] = items[itemSlot][itemName].id
    if ($(this).attr('data-socket-bonus-active') == 'true') {
      modifyStatsFromItemSocketBonus(itemId, 'add')
    }

    // Add stats from the item's equipped gems
    if (selectedGems[itemSlot + subSlot] && selectedGems[itemSlot + subSlot][items[itemSlot][itemName].id]) {
      for (gemArray of selectedGems[itemSlot + subSlot][items[itemSlot][itemName].id]) {
        if (gemArray) {
          modifyStatsFromGem(gemArray[1], 'add')
        }
      }
    }
  }

  // If the user is equipping a main hand or offhand then unequip their twohander if they have one equipped and vice versa
  if (itemSlot == 'mainhand' || itemSlot == 'offhand') {
    if (selectedItems.twohand !== null) {
      itemSlotLoop:
      for (const slot in items) {
        for (const item in items[slot]) {
          if (items[slot][item].id == selectedItems.twohand) {
            modifyStatsFromItem(items[slot][item], 'remove')
            selectedItems.twohand = null
            break itemSlotLoop
          }
        }
      }
    }
  } else if (itemSlot == 'twohand') {
    if (selectedItems.mainhand !== null) {
      itemSlotLoop:
      for (const slot in items) {
        for (const item in items[slot]) {
          if (items[slot][item].id == selectedItems.mainhand) {
            modifyStatsFromItem(items[slot][item], 'remove')
            selectedItems.mainhand = null
            break itemSlotLoop
          }
        }
      }
    }
    if (selectedItems.offhand !== null) {
      itemSlotLoop:
      for (const slot in items) {
        for (const item in items[slot]) {
          if (items[slot][item].id == selectedItems.offhand) {
            modifyStatsFromItem(items[slot][item], 'remove')
            selectedItems.offhand = null
            break itemSlotLoop
          }
        }
      }
    }
  }

  updateSetBonuses()
  refreshCharacterStats()
  localStorage.selectedItems = JSON.stringify(selectedItems)
  if (itemId == 34678) {
    updateSimulationSettingsVisibility()
  }
  return false
})

// User clicks on an item slot in the selection above the item table
$('#item-slot-selection-list li').click(function () {
  loadItemsBySlot($(this).attr('data-slot'), $(this).attr('data-subslot'))
})

$('#show-combat-log').click(function () {
  $('#combat-log').toggle()
})

$('#show-histogram').click(function() {
  $('#dps-histogram').toggle()
})

// User left-clicks on one of the item's gem sockets
$('#item-selection-table tbody').on('click', '.gem', function (event) {
  const socketColor = $(this).attr('data-color')
  $('#gem-selection-table').empty()

  for (const color in gems) {
    for (const gem in gems[color]) {
      // Show all gems for normal slots (except for Meta gems) and only show Meta gems for Meta gem slots
      if ((socketColor === 'meta' && color == 'meta') || (socketColor !== 'meta' && color !== 'meta')) {
        const g = gems[color][gem]
        const gemRowInfo = "<tr data-hidden='" + (gemPreferences.hidden.indexOf(parseInt(gem)) > -1) + "' data-color='" + color + "' data-id='" + gem + "' class='gem-row'><td title='" + (gemPreferences.favorites.indexOf(parseInt(gem)) > -1 ? 'Remove gem from favorites' : 'Favorite gem') + "' data-favorited='" + (gemPreferences.favorites.indexOf(parseInt(gem)) > -1) + "' class='gem-favorite-star gem-info'>★</td><td class='gem-name gem-info'><img width='20' height='20' src='img/" + g.iconName + ".jpg'><a href='https://tbc.wowhead.com/item=" + gem + "'>" + g.name + "</a></td><td title='" + (gemPreferences.hidden.indexOf(parseInt(gem)) > -1 ? 'Restore ' : 'Hide ') + " Gem' data-hidden='" + (gemPreferences.hidden.indexOf(parseInt(gem)) > -1) + "' class='gem-hide gem-info'>❌</td></tr>"
        if (gemPreferences.favorites.indexOf(parseInt(gem)) > -1) {
          $('#gem-selection-table').prepend(gemRowInfo)
        } else {
          $('#gem-selection-table').append(gemRowInfo)
        }
        if (gemPreferences.hidden.indexOf(parseInt(gem)) > -1) {
          $('.gem-row[data-id="' + gem + '"]').hide()
        }
      }
    }
  }
  $('#gem-selection-table').prepend('<tr data-color="' + socketColor + '" data-id="0" class="gem-row"><td class="gem-info"></td><td class="gem-name gem-info">Remove Gem From Socket</td></tr>')
  $('#gem-selection-table').prepend('<tr><td></td><td data-enabled="false" id="show-hidden-gems-button">Toggle Hidden Gems</td></tr>')
  if (gemPreferences.hidden.length == 0) {
    $('#show-hidden-gems-button').closest('tr').hide()
  }

  $('#gem-selection-table').css('top', event.pageY - $('#gem-selection-table').height() / 5)
  $('#gem-selection-table').css('left', event.pageX + 50)
  $('#gem-selection-table').css('visibility', 'visible')
  $('#gem-selection-table').data('color', $(this).data('color'))
  $('#gem-selection-table').data('itemId', $(this).closest('tr').data('wowhead-id'))
  $('#gem-selection-table').data('socketSlot', $(this).data('order'))

  // Stop the click from being registered by the .item-row listener as well.
  event.stopPropagation()
  return false
})

// User clicks on one of the item source buttons
$('#source-filters ul li').click(function () {
  // Toggle the source's checked value
  const checked = $(this).attr('data-checked') == 'true'
  const source = $(this).attr('data-source')

  $(this).attr('data-checked', !checked)
  sources[source] = sources[source] || {}
  sources[source][$(this).attr('data-value')] = !checked
  localStorage.sources = JSON.stringify(sources)
  // Reload the item list
  loadItemsBySlot(localStorage.selectedItemSlot || 'mainhand', (localStorage.selectedItemSubSlot || ''))
})

// User clicks on the "Hide / Show Items" button
$('#hide-show-items-btn').click(function () {
  $('#hide-item-col').toggle()
  $('#header-hide-item').toggle()
  $('.hide-item-btn').toggle()
  $('.hidden-item').toggle()
  changingItemVisibility = !changingItemVisibility
  loadItemsBySlot(localStorage.selectedItemSlot, localStorage.selectedItemSubSlot)
})

// User clicks on the red x next to an item row to hide (or un-hide) it
$('#item-selection-table tbody').on('click', '.hide-item-btn', function (event) {
  const itemId = Number($(this).closest('tr').attr('data-wowhead-id'))

  if (hiddenItems.includes(itemId)) {
    hiddenItems.splice(hiddenItems.indexOf(itemId), 1)
    $(this).attr('data-hidden', 'false')
  } else {
    hiddenItems.push(itemId)
    $(this).attr('data-hidden', 'true')
  }

  localStorage.hiddenItems = JSON.stringify(hiddenItems)
  event.stopPropagation()
  loadItemsBySlot(localStorage.selectedItemSlot, localStorage.selectedItemSubSlot)
})

// User clicks on the "Show Equipped Items" button
$('#show-equipped-items').click(function () {
  // Toggle the visibility of the table
  $('#currently-equipped-items-container').toggle()

  // Populate the table with the equipped items
  if ($('#currently-equipped-items').is(':visible')) {
    // Clear the table since it might have old data
    $('#currently-equipped-items tbody tr').remove()

    // Loop through the equipped items and insert a new row for each slot
    for (const itemSlot in selectedItems) {
      let itemSlotWithoutSubSlot = ['1','2'].includes(itemSlot.charAt(itemSlot.length - 1)) ? itemSlot.slice(0, itemSlot.length - 1) : itemSlot
      if (selectedItems[itemSlot]) {
        let itemId = selectedItems[itemSlot]
        let item = null
        // Find the item because I didn't use the item id as the key for some reason
        for (const i in items[itemSlotWithoutSubSlot]) {
          if (items[itemSlotWithoutSubSlot][i].id == itemId) {
            item = items[itemSlotWithoutSubSlot][i]
          }
        }
        enchantSlot = (['mainhand','twohand'].includes(itemSlot) ? 'weapon' : itemSlot)
        let enchantId = selectedEnchants[enchantSlot]
        let enchant = null
        // Find the enchant
        for (const e in enchants[enchantSlot]) {
          if (enchants[enchantSlot][e].id == enchantId) {
            enchant = enchants[enchantSlot][e]
          }
        }
        // Capitalize the item slot
        let slot = itemSlot.charAt(0).toUpperCase() + itemSlot.slice(1)
        // E.g. 'Ring1' -> 'Ring 1'
        if (['1','2'].includes(slot.charAt(slot.length - 1))) {
          slot = slot.slice(0, slot.length - 1) + ' ' + slot.charAt(slot.length - 1)
        }

        $('#currently-equipped-items tbody').append('<tr><td>' + slot + '</td><td><a target="_blank" href=' + (item && item.id ? 'https://tbc.wowhead.com/item=' + item.id : '') + '>' + (item && item.name ? item.name : '') + '</a></td><td>' + getGemsInItemAsHTML(itemSlotWithoutSubSlot, item) + '</td><td><a target="_blank" href="' + (enchant && enchant.id ? 'https://tbc.wowhead.com/spell=' + enchant.id : '') + '">' + (enchant && enchant.name ? enchant.name : '') + '</a></td></tr>')
      }
    }
  }
})

// Adds or removes an item's stats from the player
function modifyStatsFromItem (itemObj, action) {
  // If the user has the item equipped and is not loading the stats from equipped items when loading the website
  if (action == 'remove') {
    // Loop through the stats on the item and add them to/remove them from the character's stats.
    for (const stat in itemObj) {
      // Check if the item property is a character stat such as stamina/spell power.
      if (characterStats.hasOwnProperty(stat)) {
        characterStats[stat] -= itemObj[stat]
      }
    }
  } else if (action == 'add') {
    for (const stat in itemObj) {
      if (characterStats.hasOwnProperty(stat)) {
        characterStats[stat] += itemObj[stat]
      }
    }
  }
}

function modifyStatsFromItemSocketBonus (itemId, action) {
  for (const itemSlot in items) {
    for (const item in items[itemSlot]) {
      if (items[itemSlot][item].id == itemId) {
        for (const stat in items[itemSlot][item].socketBonus) {
          if (characterStats.hasOwnProperty(stat)) {
            if (action == 'add') {
              characterStats[stat] += items[itemSlot][item].socketBonus[stat]
            } else if (action == 'remove') {
              characterStats[stat] -= items[itemSlot][item].socketBonus[stat]
            }
          }
        }
      }
    }
  }
}

function getGemsInItemAsHTML(itemSlot, item) {
  let sockets = []
  let counter = 0
  for (const socket in socketInfo) {
    if (item.hasOwnProperty(socket)) {
      for (j = 0; j < item[socket]; j++) {
        let gemIcon = socketInfo[socket].iconName
        let gemHref = ''

        if (selectedGems[itemSlot] && selectedGems[itemSlot][item.id]) {
          for (const color in gems) {
            if (selectedGems[itemSlot][item.id][counter]) {
              const gemId = selectedGems[itemSlot][item.id][counter][1]
              if (gems[color][gemId]) {
                gemIcon = gems[color][gemId].iconName
                gemHref = 'https://tbc.wowhead.com/item=' + gemId
              }
            }
          }
        }
        sockets.push("<a target='_blank' href='" + gemHref + "'><img width='16' height='16' class='gem' data-color='" + socket + "' data-order='" + counter + "' src='img/" + gemIcon + ".jpg'></a>")
        counter++
      }
    }
  }

  return sockets.join('')
}

// Loads items into the item table
function loadItemsBySlot (itemSlot = 'mainhand', subSlot = '') {
  // Set old item slot's selected value to false
  $("#item-slot-selection-list li[data-selected='true']").attr('data-selected', 'false')
  // Set the new item slot's seleected value to true
  let newItemSlotSelector = "#item-slot-selection-list li[data-slot='" + itemSlot + "']"
  // If the item has a subslot then add a subslot selector to the query
  if (subSlot !== '') {
    newItemSlotSelector += "[data-subslot='" + subSlot + "']"
  }
  $(newItemSlotSelector).attr('data-selected', 'true')
  localStorage.selectedItemSlot = itemSlot
  localStorage.selectedItemSubSlot = (subSlot || '')
  savedItemDps[itemSlot + subSlot] = savedItemDps[itemSlot + subSlot] || {}

  // Removes all current item rows
  $('.item-row').remove()
  const tableBody = $('#item-selection-table tbody')

  for (const item of Object.keys(items[itemSlot])) {
    const i = items[itemSlot][item]
    const itemIsHidden = hiddenItems.includes(i.displayId) || hiddenItems.includes(i.id)

    if (!sources.phase || !sources.phase[i.phase] || (itemIsHidden && !changingItemVisibility)) {
      continue
    }

    // If an item is unique and it is already equipped in the other slot then skip it
    if (i.unique && (itemSlot == 'ring' || itemSlot == 'trinket') && subSlot !== null) {
      const otherSlot = subSlot == '1' ? '2' : '1'
      if (selectedItems[itemSlot + otherSlot] == i.id) {
        continue 
      }
    }

    tableBody.append("<tr data-hidden='" + itemIsHidden + "' data-subslot='" + localStorage.selectedItemSubSlot + "' data-socket-bonus-active='false' data-slot='" + itemSlot + "' data-name='" + item + "' data-selected='" + (selectedItems[itemSlot + localStorage.selectedItemSubSlot] == i.id || 'false') + "' class='item-row' data-wowhead-id='" + i.id + "'><td title='" + (itemIsHidden ? 'Show Item' : 'Hide Item') + "' class='hide-item-btn' data-hidden='" + (itemIsHidden ? 'true' : 'false') + "'>❌</td><td><a href='https://tbc.wowhead.com/item=" + (i.displayId || i.id) + "'>" + i.name + '</a></td><td><div>' + getGemsInItemAsHTML(itemSlot, i) + '</div></td><td>' + i.source + '</td><td>' + (i.stamina || '') + '</td><td>' + (i.intellect || '') + '</td><td>' + (i.spellPower || '') + '</td><td>' + (i.shadowPower || '') + '</td><td>' + (i.firePower || '') + '</td><td>' + (i.critRating || '') + '</td><td>' + (i.hitRating || '') + '</td><td>' + (i.hasteRating || '') + "</td><td class='item-dps'>" + (savedItemDps[itemSlot + subSlot][i.id] || '') + '</td></tr>').trigger('update')
    if (itemMeetsSocketRequirements(i.id)) {
      $(".item-row[data-wowhead-id='" + i.id + "']").attr('data-socket-bonus-active', 'true')
    }
  }

  if (changingItemVisibility) {
    $('#hide-item-col').show()
    $('#header-hide-item').show()
    $('.hide-item-btn').show()
    $('.hidden-item').show()
  }

  loadEnchantsBySlot(itemSlot, subSlot)
}

// Returns true/false depending on if the gems in the item match the sockets' colors
function itemMeetsSocketRequirements (itemId) {
  // Look through the item slots and see if it includes the item we're looking for
  for (const itemSlot in selectedGems) {
    if (selectedGems[itemSlot][itemId]) {
      // Loop through the sockets in the item and return false if any of them contain a gem that doesn't fit that socket's color
      for (const socket in selectedGems[itemSlot][itemId]) {
        // Loop through the gem colors that fit into this socket (e.g. blue, green, and purple if it's a blue socket) and return false if the gem isn't found under any of those colors
        let gemFound = false
        const equippedGemArray = selectedGems[itemSlot][itemId][parseInt(socket)]
        if (equippedGemArray && socketInfo[equippedGemArray[0]]) {
          // Array index 0 is the item socket's color and index 1 is the equipped gem's ID
          for (const gemColor in socketInfo[equippedGemArray[0]].gems) {
            if (gems[socketInfo[equippedGemArray[0]].gems[gemColor]][equippedGemArray[1]]) {
              gemFound = true
              break
            }
          }
        }
        if (!gemFound) {
          return false
        }
      }
      return true
    }
  }
  return false
}