// Maximum amount of web workers that should be run concurrently
const maxWorkers = navigator.hardwareConcurrency || 8
// Object with all the gems the user has equipped on any item, including items that are not equipped. Each key is the item's name and the value is an array with the ID of the gems equipped in that item.
const selectedGems = localStorage.selectedGems ? JSON.parse(localStorage.selectedGems) : {}
// Key: Item slot. Value: Equipped item's ID
let selectedItems = localStorage.selectedItems ? JSON.parse(localStorage.selectedItems) : {}
// Key: Talent's name. Value: Talent's point amount
let talents = localStorage.talents ? JSON.parse(localStorage.talents) : {}
// Key: Aura's name. Value: Boolean
let auras = localStorage.auras ? JSON.parse(localStorage.auras) : {}
let rotation = localStorage.rotation ? JSON.parse(localStorage.rotation) : {}
let selectedEnchants = localStorage.selectedEnchants ? JSON.parse(localStorage.selectedEnchants) : {}
// Key: Item ID. Value: Item's saved DPS from previous simulations.
const savedItemDps = localStorage.savedItemDps ? JSON.parse(localStorage.savedItemDps) : {}
let settings = localStorage.settings ? JSON.parse(localStorage.settings) : {}
const sources = localStorage.sources ? JSON.parse(localStorage.sources) : { phase: { 1: true } }
const profiles = localStorage.profiles ? JSON.parse(localStorage.profiles) : {}
const gemPreferences = localStorage.gemPreferences ? JSON.parse(localStorage.gemPreferences) : { hidden: [], favorites: [] }

// Buffs, debuffs, consumables, and pet buffs
for (const auraType in _auras) {
  const lowerAuraType = auraType.toLowerCase().split(' ').join('-')
  $('#buffs-and-debuffs-section').append("<h3 id='" + auraType + "-heading'>" + _auras[auraType].heading + "</h3><ul id='" + lowerAuraType + "-list'></ul>")
  for (const aura in _auras[auraType].auras) {
    const a = _auras[auraType].auras[aura]
    $('#' + lowerAuraType + '-list').append("<li data-aura-type='" + auraType + "' data-checked='" + (auras[aura] || false) + "' name='" + aura + "' id='" + aura + "' class='" + (a.alcohol ? 'alcohol ' : '') + (a.stats ? 'stats ' : '') + (a.potion ? 'potion ' : '') + (a.battleElixir ? 'battle-elixir ' : '') + (a.guardianElixir ? 'guardian-elixir ' : '') + (a.weaponOil ? 'weapon-oil ' : '') + (a.foodBuff ? 'food-buff ' : '') + (a.demonicRune ? 'demonic-rune ' : '') + (a.petOnly ? 'petBuff ' : '') + (a.forPet ? 'petDebuff ' : '') + auraType + " aura'><a href='https://tbc.wowhead.com/" + _auras[auraType].type + '=' + a.id + "'><img alt='" + a.name + "' src='img/" + a.iconName + ".jpg'></a></li>")
  }
}

// Spell Selection
for (const type in _spellSelection) {
  const rotationList = $('#rotation-list')
  let str = '<div><h4>' + _spellSelection[type].header + '</h4>'
  for (const spell in _spellSelection[type].spells) {
    rotation[type] = rotation[type] || {}
    str += "<li data-type='" + type + "' data-name='" + spell + "' class='rotation-" + type + "' data-checked='" + (rotation[type][spell] || false) + "' id='" + type + '-' + spell + "'><a href=https://tbc.wowhead.com/spell=" + _spellSelection[type].spells[spell].id + "><img src='img/" + _spellSelection[type].spells[spell].iconName + ".jpg' alt='" + _spellSelection[type].spells[spell].name + "'></a></li>"
  }
  str += '</div>'
  rotationList.append(str)
}

// Talent trees
for (const tree in _talents) {
  if (_talents.hasOwnProperty(tree)) {
    $('#talents-section').append($("<div class='talent-tree-div'><table data-name='" + tree + "' background='img/talent_tree_background_" + tree + ".jpg' id='talent-table-" + tree + "' class='talent-tree-table'></table><div class='talent-tree-name'><h3 style='display: inline-block;' data-name='" + tree + "'>" + tree.charAt(0).toUpperCase() + tree.slice(1) + "</h3><span class='clear-talent-tree'>&#10060;</span></div></div>"))
    $('#talent-table-' + tree).append($('<tbody></tbody>'))
    $('#talent-table-' + tree).data('points', 0)
    $('#talent-table-' + tree + ' tbody').append($("<tr class='" + tree + "-tree-row'></tr>"))
    let lastRow = $('#talent-table-' + tree + ' tbody tr:last')
    let currentCol = 1

    for (const talent in _talents[tree]) {
      const t = _talents[tree][talent]
      talents[talent] = talents[talent] || 0
      talentPointsRemaining -= talents[talent]
      $('#talent-table-' + tree).data('points', $('#talent-table-' + tree).data('points') + talents[talent])

      // Check if the current talent should be in the next row below and create a new row if true
      if (t.row > $('.' + tree + '-tree-row').length) {
        $('#talent-table-' + tree + ' tbody').append($("<tr class='" + tree + "-tree-row'></tr>"))
        lastRow = $('#talent-table-' + tree + ' tbody tr:last')
        currentCol = 1
      }

      // Create empty cells between talents if skipping a number (e.g. going from column 1 straight to column 4)
      while (currentCol < t.column) {
        lastRow.append($('<td></td>'))
        currentCol++
      }

      lastRow.append($("<td><div data-maxpoints='" + t.rankIDs.length + "' data-points='" + talents[talent] + "' class='talent-icon' data-tree='" + tree + "' id='" + talent + "'><a href='https://tbc.wowhead.com/spell=" + t.rankIDs[Math.max(0, talents[talent] - 1)] + "'><img src='img/" + t.iconName + ".jpg' alt='" + t.name + "'><span id='" + talent + "-point-amount' class='talent-point-amount'>" + talents[talent] + '</span></a></div></td>'))

      // Check if the text displaying the talent point amount should be hidden or colored (for maxed out talents)
      const pointAmount = $('#' + talent + '-point-amount')
      if (pointAmount.text() <= 0) {
        pointAmount.hide()
      } else if (pointAmount.text() == t.rankIDs.length) {
        pointAmount.css('color', '#ffcd45')
      } else {
        pointAmount.css('color', '#7FFF00')
      }
      currentCol++
    }

    updateTalentTreeNames()
  }
}

// Add in the pre-requisite talent arrows (todo: find a better way to do this)
/* $("#talent-table-affliction").append("<div data-row='3' data-column='3' class='talent-arrow'><img width='15' height='60' src='img/talent_arrow_down.jpg'></div>"); // Amplify Curse to Curse of Exhaustion
$("#talent-table-affliction").append("<div data-row='5' data-column='2' class='talent-arrow'><img width='15' height='10' src='img/talent_arrow_down.jpg'></div>"); // Siphon Life to Shadow Mastery
$("#talent-table-affliction").append("<div data-row='7' data-column='2' class='talent-arrow'><img width='15' height='60' src='img/talent_arrow_down.jpg'></div>"); // Contagion to Unstable Agony
$("#talent-table-demonology").append("<div data-row='3' data-column='2' class='talent-arrow'><img width='15' height='10' src='img/talent_arrow_down.jpg'></div>"); */ // Fel Domination to Master Summoner

// When a buff/debuff/consumable is clicked
$('.aura').click(function () {
  const auraType = $(this).attr('data-aura-type')
  const auraName = $(this).attr('name')
  const checkedVal = $(this).attr('data-checked') === 'true'
  $(this).attr('data-checked', !checkedVal)
  auras[$(this).attr('name')] = !checkedVal

  if (auraName == 'faerieFire' || auraName == 'vampiricTouch' || auraName == 'exposeArmor' || auraName == 'exposeWeakness' || auraName == 'totemOfWrath' || auraName == 'curseOfTheElements' || auraName == 'prayerOfSpirit' || auraName == 'powerOfTheGuardianWarlock' || auraName == 'powerOfTheGuardianMage' || auraName == 'drumsOfBattle' || auraName == 'drumsOfWar' || auraName == 'drumsOfRestoration' || auraName == 'bloodlust') {
    updateSimulationSettingsVisibility()
  }
  if (!$(this).hasClass('petBuff')) {
    modifyStatsFromAura(_auras[auraType].auras[auraName], checkedVal)
    refreshCharacterStats()
  }
  localStorage.auras = JSON.stringify(auras)
  return false
})

// Array of consumables whose clicks we want to track.
const consumableTypesToTrack = ['.weapon-oil', '.battle-elixir', '.guardian-elixir', '.food-buff', '.potion', '.demonic-rune', '.alcohol']
// When a consumable is clicked, uncheck all other types of that consumable since we can only have one at a time (e.g. disable all other weapon oils if a weapon oil is clicked).
$(consumableTypesToTrack.join(',')).click(function (event) {
  const clickedConsumableName = $(this).attr('name')
  const consumableTypes = []

  // Loop through the consumable types we're tracking and check if the consumable that got clicked has any of those consumables as a class.
  for (let i = 0; i < consumableTypesToTrack.length; i++) {
    if ($(this).hasClass(consumableTypesToTrack[i].substring(1))) {
      consumableTypes.push(consumableTypesToTrack[i])
    }
  }

  // Loop through the consumable classes we found in the previous loop and uncheck all childs of those classes aside from the consumable that just got clicked
  $(consumableTypes.join(',')).each(function () {
    const consumableName = $(this).attr('name')

    if (consumableName !== clickedConsumableName) {
      if ($(this).attr('data-checked') === 'true') {
        $(this).attr('data-checked', false)
        auras[consumableName] = false

        for (const stat in _auras.consumables.auras[consumableName]) {
          if (characterStats.hasOwnProperty(stat)) {
            characterStats[stat] -= _auras.consumables.auras[consumableName][stat]
          }
        }
      }
    }
  })

  localStorage.auras = JSON.stringify(auras)
  refreshCharacterStats()
})

// User clicks on one of the preset item set buttons above the item slot selection menu
$('.preset-item-set').click(function () {
  const name = $(this).data('name')
  if (presetItemSets[name]) {
    for (itemSlot in presetItemSets[name]) {
      if (selectedItems[itemSlot] !== presetItemSets[name][itemSlot]) {
        for (item in items[itemSlot]) {
          if (items[itemSlot][item].id == presetItemSets[name][itemSlot]) {
            modifyStatsFromItem(items[itemSlot][item], 'add')
          } else if (items[itemSlot][item].id == selectedItems[itemSlot]) {
            modifyStatsFromItem(items[itemSlot][item], 'remove')
          }
        }
        if (itemSlot == (localStorage.selectedItemSlot + localStorage.selectedItemSubSlot)) {
          $(".item-row[data-wowhead-id='" + selectedItems[itemSlot] + "']").attr('data-selected', 'false')
          $(".item-row[data-wowhead-id='" + presetItemSets[name][itemSlot] + "']").attr('data-selected', 'true')
        }
        selectedItems[itemSlot] = presetItemSets[name][itemSlot]
      }
    }
    selectedItems = presetItemSets[name]
    localStorage.selectedItems = JSON.stringify(selectedItems)
    refreshCharacterStats()
    updateSetBonuses()
  }
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

// User clicks on an item slot in the selection above the item table
$('#item-slot-selection-list li').click(function () {
  loadItemsBySlot($(this).attr('data-slot'), $(this).attr('data-subslot') || null)
})

$('#show-combat-log').click(function () {
  $('#combat-log').toggle()
})

// When the user clicks anywhere on the webpage
$(document).on('click', function (e) {
  // Hide the gem selection table if the user clicks outside of it.
  if (e.target.id !== 'gem-selection-table' && !e.target.className.split(' ').includes('gem-info')) {
    $('#gem-selection-table').css('visibility', 'hidden')
  }
})

// User clicks on the "Save New Profile" button
$('#save-profile-button').click(function () {
  const profileName = $("input[name='profileName']").val()
  if (profileName.length <= 0) {
    alert('Missing profile name')
  } else if (profiles[profileName]) {
    alert('The profile "' + profileName + '" already exists')
  } else {
    saveProfile(profileName)
    localStorage.selectedProfile = profileName
    $("input[name='profileName']").val('')
    updateProfileSelection(profileName)
    drawProfileButtons()
  }
})

// User clicks on the "Save" profile button
$(document).on('click', '#save-profile-button', function () {
  saveProfile(localStorage.selectedProfile)
})

// User clicks on the "Delete" profile button
$('#delete-profile-button').click(function () {
  if (confirm('Are you sure you want to delete "' + localStorage.selectedProfile + "'?")) {
    delete profiles[localStorage.selectedProfile]
    localStorage.removeItem('selectedProfile')
    localStorage.profiles = JSON.stringify(profiles)
    drawProfileButtons()
    $('#update-profile-div').hide()
    if ($('.saved-profile').length == 0) {
      $('#saved-profiles').hide()
    }
  }
})

// User clicks on the "Rename" profile button
$('#rename-profile-button').click(function () {
  const newName = prompt('Enter the new name for profile "' + localStorage.selectedProfile + "'")
  if (newName !== null && newName.length > 0) {
    // Create a copy of the profile with the new name
    profiles[newName] = profiles[localStorage.selectedProfile]
    // Delete the old profile
    delete profiles[localStorage.selectedProfile]
    // Update localStorage
    localStorage.selectedProfile = newName
    localStorage.profiles = JSON.stringify(profiles)
    drawProfileButtons()
  }
})

// User clicks on one of their saved profiles
$(document).on('click', '.saved-profile', function () {
  const profileName = $(this).attr('data-name')
  updateProfileSelection(profileName)
  // Show the buttons to save, delete, and rename the profile
  $('#update-profile-div').show()
  // Load settings from the profile
  auras = profiles[profileName].auras
  localStorage.auras = JSON.stringify(auras)
  rotation = profiles[profileName].rotation
  localStorage.rotation = JSON.stringify(rotation)
  settings = profiles[profileName].simSettings
  localStorage.settings = JSON.stringify(settings)
  talents = profiles[profileName].talents
  localStorage.talents = JSON.stringify(talents)
  selectedItems = profiles[profileName].items
  localStorage.selectedItems = JSON.stringify(selectedItems)
  // selectedGems = profiles[profileName].gems;
  // localStorage.selectedGems = JSON.stringify(selectedGems);
  selectedEnchants = profiles[profileName].enchants
  localStorage.selectedEnchants = JSON.stringify(selectedEnchants)
  location.reload()
})

// User clicks on a star next to a gem to favorite it
$(document).on('click', '.gem-favorite-star', function () {
  const favorited = $(this).attr('data-favorited') == 'true'
  const gemId = parseInt($(this).closest('tr').attr('data-id'))
  const favoritesArrayIndex = gemPreferences.favorites.indexOf(gemId)
  // Toggle the favorited data value
  $(this).attr('data-favorited', !favorited)

  // Remove or add the gem to the favorite gem array
  if (favorited && favoritesArrayIndex > -1) {
    gemPreferences.favorites.splice(favoritesArrayIndex, 1)
  } else if (!favorited && favoritesArrayIndex < 0) {
    gemPreferences.favorites.push(gemId)
  }
  localStorage.gemPreferences = JSON.stringify(gemPreferences)
})

// User clicks on the X next to a gem to hide it
$(document).on('click', '.gem-hide', function () {
  const hidden = $(this).attr('data-hidden') == 'true'
  const gemId = parseInt($(this).closest('tr').attr('data-id'))
  const hiddenArrayIndex = gemPreferences.hidden.indexOf(gemId)
  $(this).attr('data-hidden', !hidden)

  if (hidden && hiddenArrayIndex > -1) {
    gemPreferences.hidden.splice(hiddenArrayIndex, 1)
    $(this).closest('tr').attr('data-hidden', 'false')
    if (gemPreferences.hidden.length == 0) {
      $('#show-hidden-gems-button').closest('tr').hide()
      $('#show-hidden-gems-button').attr('data-enabled', 'false')
    }
  } else if (!hidden && hiddenArrayIndex < 0) {
    gemPreferences.hidden.push(gemId)
    $(this).closest('tr').attr('data-hidden', 'true')
    if ($('#show-hidden-gems-button').attr('data-enabled') == 'false') {
      $(this).closest('tr').hide()
    }
    if (gemPreferences.hidden.length > 0) {
      $('#show-hidden-gems-button').closest('tr').show()
    }
  }
  localStorage.gemPreferences = JSON.stringify(gemPreferences)
})

// User clicks on the "Toggle Hidden Gems" button in the gem selection table
$(document).on('click', '#show-hidden-gems-button', function (e) {
  const enabled = $(this).attr('data-enabled') == 'true'
  $(this).attr('data-enabled', !enabled)

  if (enabled) {
    $(".gem-row[data-hidden='true']").hide()
  } else {
    $(".gem-row[data-hidden='true']").show()
  }

  e.stopPropagation()
})

// User clicks on a gem in the gem selection table
$('#gem-selection-table').on('click', '.gem-name', function () {
  const itemId = $('#gem-selection-table').data('itemId')
  const itemSlot = $('tr[data-wowhead-id="' + itemId + '"]').data('slot')
  const gemColor = $(this).closest('tr').data('color')
  let gemIconName = href = null
  const gemId = $(this).closest('tr').data('id')
  const socket = $('tr[data-wowhead-id="' + itemId + '"]').find('.gem').eq($('#gem-selection-table').data('socketSlot'))
  const socketColor = $(this).closest('#gem-selection-table').data('color')
  const socketSlot = $('#gem-selection-table').data('socketSlot')
  selectedGems[itemSlot] = selectedGems[itemSlot] || {}

  if (!selectedGems[itemSlot][itemId]) {
    // The amount of sockets in the item
    const socketAmount = $('tr[data-wowhead-id="' + itemId + '"]').find('.gem').last().data('order') + 1
    if (socketAmount > 0) {
      selectedGems[itemSlot][itemId] = Array(socketAmount).fill(null)
    }
  }

  // Check whether the user chose a gem or the option to remove the current gem
  if (gemId == '0') {
    gemIconName = socketInfo[gemColor].iconName + '.jpg'
    href = ''
  } else {
    gemIconName = gems[gemColor][gemId].iconName + '.jpg'
    href = 'https://tbc.wowhead.com/item=' + gemId
  }

  // Check if the socket that was changed was on an equipped item
  if (socket.closest('tr').data('wowhead-id') == selectedItems[itemSlot]) {
    // Remove stats from old gem if equipped
    if (selectedGems[itemSlot][itemId][socketSlot]) {
      modifyStatsFromGem(selectedGems[itemSlot][itemId][socketSlot][1], 'remove')
      if ($(".item-row[data-wowhead-id='" + itemId + "']").attr('data-socket-bonus-active') == 'true') {
        modifyStatsFromItemSocketBonus(itemId, 'remove')
      }
    }
    // Add stats from new gem
    if (gemId) {
      modifyStatsFromGem(gemId, 'add')
    }
  }

  if ($(".item-row[data-wowhead-id='" + itemId + "']").attr('data-socket-bonus-active') == 'true') {
    $(".item-row[data-wowhead-id='" + itemId + "']").attr('data-socket-bonus-active', 'false')
  }

  if (gemId == 0) {
    selectedGems[itemSlot][itemId][socketSlot] = null
  } else {
    selectedGems[itemSlot][itemId][socketSlot] = [socketColor, gemId]
  }
  if (itemMeetsSocketRequirements(itemId)) {
    // Only add the socket bonus if the player has the item equipped
    if (socket.closest('tr').data('wowhead-id') == selectedItems[itemSlot]) {
      modifyStatsFromItemSocketBonus(itemId, 'add')
    }
    $(".item-row[data-wowhead-id='" + itemId + "']").attr('data-socket-bonus-active', 'true')
  }
  socket.attr('src', 'img/' + gemIconName)
  socket.closest('a').attr('href', href)
  localStorage.selectedGems = JSON.stringify(selectedGems)
  $('#gem-selection-table').css('visibility', 'hidden')
  refreshCharacterStats()
  return false
})

// Remove gem from item socket if user right clicks on the socket
$('#item-selection-table tbody').on('contextmenu', '.gem', function (event) {
  // Check whether there is a gem in the socket or not
  if ($(this).closest('a').attr('href') !== '') {
    const socketColor = $(this).data('color')
    const itemSlot = $(this).closest('tr').data('slot')
    const itemId = $(this).closest('tr').data('wowhead-id')
    const socketOrder = $(this).data('order')

    if ($(".item-row[data-wowhead-id='" + itemId + "']").attr('data-socket-bonus-active') == 'true') {
      if (selectedItems[itemSlot] == itemId) {
        modifyStatsFromItemSocketBonus(itemId, 'remove')
      }
      $(".item-row[data-wowhead-id='" + itemId + "']").attr('data-socket-bonus-active', 'false')
    }

    if (selectedItems[itemSlot] == itemId) {
      modifyStatsFromGem(selectedGems[itemSlot][itemId][socketOrder][1], 'remove')
    }
    $(this).attr('src', 'img/' + socketInfo[socketColor].iconName + '.jpg')
    $(this).closest('a').attr('href', '')
    selectedGems[itemSlot][itemId][socketOrder] = null
    localStorage.selectedGems = JSON.stringify(selectedGems)
    refreshCharacterStats()
  }

  return false
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
            modifyStatsFromItemSocketBonus(itemId, 'remove')
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
  return false
})

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

$('.preset-talent-button').click(function () {
  talents = JSON.parse(JSON.stringify(presetTalents[$(this).data('name')]))
  talentPointsRemaining = totalTalentPoints

  $('.talent-tree-table').each(function () {
    $(this).data('points', 0)
  })
  $('.talent-icon').each(function () {
    talents[$(this).attr('id')] = talents[$(this).attr('id')] || 0
    const pointAmount = talents[$(this).attr('id')]
    const talentTableObj = $(this).closest('table')

    $(this).attr('data-points', pointAmount)
    talentTableObj.data('points', talentTableObj.data('points') + pointAmount)
    talentPointsRemaining -= pointAmount || 0
    updateTalentInformation($(this))
    updateTalentTreeNames()
  })

  // Disable spells that aren't available anymore with the new talents
  if (talents.siphonLife == 0) {
    rotation.dot.siphonLife = false
    $('#dot-siphonLife').attr('data-checked', 'false')
  }
  if (talents.unstableAffliction == 0) {
    rotation.dot.unstableAffliction = false
    $('#dot-unstableAffliction').attr('data-checked', 'false')
  }
  if (talents.shadowburn == 0) {
    rotation.finisher.shadowburn = false
    $('#finisher-shadowburn').attr('data-checked', 'false')
  }

  localStorage.rotation = JSON.stringify(rotation)
  localStorage.talents = JSON.stringify(talents)
  refreshCharacterStats()
  updateSimulationSettingsVisibility()
})

// Disable the context menu from appearing when the user right clicks a talent
$('.talent-icon').bind('contextmenu', function (event) {
  return false
})

// Prevents the user from being redirected to the talent's wowhead page when clicking it.
$('.talent-icon').click(function () {
  return false
})

// Fires when the user left or right clicks a talent
$('.talent-icon').mousedown(function (event) {
  // Check if the click was a left or right click
  if ((event.which === 1 && talentPointsRemaining > 0) || event.which === 3) {
    const icon = $(this)
    const talent = _talents[icon.attr('data-tree')][icon.attr('id')] // get the talent's object
    const talentName = $(this).attr('id')
    const talentTree = $('#talent-table-' + $(this).data('tree'))

    // left click
    if (event.which === 1) {
      // compare the amount of points in the talent vs the amount of ranks before incrementing
      if (Number(icon.attr('data-points')) < talent.rankIDs.length && talentTree.data('points') >= (talent.row - 1) * 5) {
        // If the talent has another talent that needs to be selected before this one is selectable, then check if the required talent is selected.
        if (talent.requirement && talents[talent.requirement.name] < talent.requirement.points) {
          return
        }

        icon.attr('data-points', Number(icon.attr('data-points')) + 1)
        talents[talentName] = Number(talents[talentName]) + 1
        talentPointsRemaining--
        talentTree.data('points', talentTree.data('points') + 1)
      }
      // right click
    } else if (event.which === 3) {
      // only decrement if the point amount is above 0
      if (icon.attr('data-points') > 0) {
        const talentTreeName = $(this).data('tree')
        // Check if the talent is locked due to a dependency (such as not being able to remove Amplify Curse because Curse of Exhaustion is selected)
        for (const t in _talents[talentTreeName]) {
          if (_talents[talentTreeName][t].requirement && _talents[talentTreeName][t].requirement.name == talentName && talents[t] > 0) {
            return
          }
        }

        icon.attr('data-points', Number(icon.attr('data-points')) - 1)
        talents[talentName] = Number(talents[talentName]) - 1
        talentPointsRemaining++
        talentTree.data('points', talentTree.data('points') - 1)
        icon.children('a').children('span').css('color', '#7FFF00')
      }
    }

    updateTalentTreeNames()
    if (talent.name == 'Conflagrate' || talent.name == 'Master Demonologist' || talent.name == 'Demonic Sacrifice' || talent.name == 'Summon Felguard' || talent.name == 'Dark Pact') {
      updateSimulationSettingsVisibility()
    }
    if (talent.name == 'Emberstorm' || talent.name == 'Improved Imp' || talent.name == 'Demonic Aegis' || talent.name == 'Demonic Embrace' || talent.name == 'Devastation' || talent.name == 'Backlash' || talent.name == 'Fel Stamina' || talent.name == 'Fel Intellect' || talent.name == 'Master Demonologist' || talent.name == 'Soul Link' || talent.name == 'Demonic Tactics' || talent.name == 'Shadow Mastery') {
      refreshCharacterStats()
    }
    updateTalentInformation(icon)
  }

  localStorage.talents = JSON.stringify(talents)
  return false
})

// User clicks on the red X next to a talent tree's name to clear it
$('.clear-talent-tree').click(function () {
  clearTalentTree($(this).closest('div').find('h3').data('name'))
})

// Listens to any clicks on the "rotation" spells for dots, filler, curse, and finisher.
$('#rotation-list div li').click(function () {
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

$('#sim-settings select, #sim-settings input').change(function () {
  settings[$(this).attr('name')] = $(this).val()
  localStorage.settings = JSON.stringify(settings)
  refreshCharacterStats()
  updateSimulationSettingsVisibility()
})

// to-do: don't allow people to start multiple simulations
$('#sim-dps').click(function () {
  simDPS([$(".item-row[data-selected='true']").data('wowhead-id')])
  return false
})

$('#sim-all-items').click(function () {
  const arr = []
  $('.item-row').each(function (i) {
    arr.push($(this).data('wowhead-id'))
  })
  $('.breakdown-section').hide()
  simDPS(arr)
  return false
})

$("#sim-stat-weights").click(function() {
  simStatWeights()
})

$('.btn').hover(function () {
  $(this).find('a').css('color', '#1a1a1a')
})

$('.btn').mouseout(function () {
  $(this).find('a').css('color', 'white')
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

// Loads items into the item table
function loadItemsBySlot (itemSlot, subSlot) {
  // Set old item slot's selected value to false
  $("#item-slot-selection-list li[data-selected='true']").attr('data-selected', 'false')
  // Set the new item slot's seleected value to true
  let newItemSlotSelector = "#item-slot-selection-list li[data-slot='" + itemSlot + "']"
  // If the item has a subslot then add a subslot selector to the query
  if (subSlot !== null) {
    newItemSlotSelector += "[data-subslot='" + subSlot + "']"
  } else {
    subSlot = ''
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

    if (!sources.phase || !sources.phase[i.phase]) {
      continue
    }

    // If an item is unique and it is already equipped in the other slot then skip it
    if (i.unique && (itemSlot == 'ring' || itemSlot == 'trinket') && subSlot !== null) {
      const otherSlot = subSlot == '1' ? '2' : '1'
      if (selectedItems[itemSlot + otherSlot] == i.id) continue
    }

    // Add the item's gem sockets
    const sockets = []
    let counter = 0
    for (const socket in socketInfo) {
      if (i.hasOwnProperty(socket)) {
        for (j = 0; j < i[socket]; j++) {
          let gemIcon = socketInfo[socket].iconName
          let gemHref = ''

          if (selectedGems[itemSlot] && selectedGems[itemSlot][i.id]) {
            for (const color in gems) {
              if (selectedGems[itemSlot][i.id][counter]) {
                const gemId = selectedGems[itemSlot][i.id][counter][1]
                if (gems[color][gemId]) {
                  gemIcon = gems[color][gemId].iconName
                  gemHref = 'https://tbc.wowhead.com/item=' + gemId
                }
              }
            }
          }
          sockets.push("<a href='" + gemHref + "'><img width='16' height='16' class='gem' data-color='" + socket + "' data-order='" + counter + "' src='img/" + gemIcon + ".jpg'></a>")
          counter++
        }
      }
    }

    tableBody.append("<tr data-subslot='" + localStorage.selectedItemSubSlot + "' data-socket-bonus-active='false' data-slot='" + itemSlot + "' data-name='" + item + "' data-selected='" + (selectedItems[itemSlot + localStorage.selectedItemSubSlot] == i.id || 'false') + "' class='item-row' data-wowhead-id='" + i.id + "'><td><a href='https://tbc.wowhead.com/item=" + i.id + "'>" + i.name + '</a></td><td><div>' + sockets.join('') + '</div></td><td>' + i.source + '</td><td>' + (i.stamina || '') + '</td><td>' + (i.intellect || '') + '</td><td>' + (i.spellPower || '') + '</td><td>' + (i.shadowPower || '') + '</td><td>' + (i.firePower || '') + '</td><td>' + (i.critRating || '') + '</td><td>' + (i.hitRating || '') + '</td><td>' + (i.hasteRating || '') + "</td><td class='item-dps'>" + (savedItemDps[itemSlot + subSlot][i.id] || '') + '</td></tr>').trigger('update')
    if (itemMeetsSocketRequirements(i.id)) {
      $(".item-row[data-wowhead-id='" + i.id + "']").attr('data-socket-bonus-active', 'true')
    }
  }

  loadEnchantsBySlot(itemSlot, subSlot)
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

      tableBody.append("<tr data-slot='" + itemSlot + "' data-subslot='" + (subSlot || '') + "' data-name='" + enchant + "' data-selected='" + (selectedEnchants[itemSlot + (subSlot || '')] == e.id || 'false') + "' class='enchant-row' data-wowhead-id='" + e.id + "'><td><a href='https://tbc.wowhead.com/spell=" + e.id + "'>" + e.name + '</a></td><td>' + (e.spellPower || '') + '</td><td>' + (e.shadowPower || '') + '</td><td>' + (e.firePower || '') + '</td><td>' + (e.stamina || '') + '</td><td>' + (e.intellect || '') + '</td><td>' + (e.mp5 || '') + '</td><td>' + (((e.natureResistance || 0) + (e.allResistance || 0)) || '') + '</td><td>' + (((e.shadowResistance || 0) + (e.allResistance || 0)) || '') + '</td><td>' + (((e.fireResistance || 0) + (e.allResistance || 0)) || '') + '</td><td>' + ((e.threatReduction * 100) || '') + '</td><td>' + (e.threatIncrease * 100 || '') + '</td><td>' + (localStorage[enchant + 'Dps'] || '') + '</td></tr>')
    }

    $('#enchant-selection-table').show()
  } else {
    $('#enchant-selection-table').hide()
  }

  refreshCharacterStats()
}

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

function modifyStatsFromGem (gemId, action) {
  for (const color in gems) {
    if (gems[color][gemId]) {
      for (const property in gems[color][gemId]) {
        if (characterStats.hasOwnProperty(property)) {
          if (action == 'add') {
            characterStats[property] += gems[color][gemId][property]
          } else if (action == 'remove') {
            characterStats[property] -= gems[color][gemId][property]
          }
        }
      }
      return
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

function modifyStatsFromAura (auraObject, checked) {
  for (const stat in auraObject) {
    if (characterStats.hasOwnProperty(stat)) {
      // Check if the buff is a modifier to know whether to add/subtract or multiply/divide the stat
      if (stat.toLowerCase().search('modifier') !== -1) {
        if (checked) {
          characterStats[stat] /= auraObject[stat]
        } else {
          characterStats[stat] *= auraObject[stat]
        }
      } else {
        if (checked) {
          characterStats[stat] -= auraObject[stat]
        } else {
          characterStats[stat] += auraObject[stat]
        }
      }
    }
  }
}

function simDPS (items) {
  const item = $("#item-slot-selection-list li[data-selected='true']")
  const itemSlot = item.attr('data-slot')
  const itemSubSlot = item.attr('data-subslot') || ''
  const itemAmount = items.length
  let simulationsRunning = 0
  let simulationsFinished = 0
  const multiSimInfo = []
  const simulations = []
  let simIndex = 0
  console.log(items)

  if (items.length > 1) {
    $(".item-dps").text('')
  }

  for (let i = 0; i < items.length; i++) {
    multiSimInfo.push([items[i], 0])

    simulations.push(new SimWorker(
      (simulationEnd) => {
        simulationsFinished++
        // DPS information on the sidebar
        if (itemAmount === 1) {
          localStorage.avgDps = simulationEnd.avgDps
          localStorage.minDps = simulationEnd.minDps
          localStorage.maxDps = simulationEnd.maxDps
          localStorage.simulationDuration = simulationEnd.length
          $('#avg-dps').text(simulationEnd.avgDps)
          $('#min-dps').text(simulationEnd.minDps)
          $('#max-dps').text(simulationEnd.maxDps)
          $('#sim-length-result').text(simulationEnd.length + 's')
          $('#sim-dps').text('Simulate')

          // Populate the combat log
          $('#combat-log p').remove()
          for (const entry in simulationEnd.combatlog) {
            $('#combat-log').append('<p>' + simulationEnd.combatlog[entry] + '</p>')
          }
        } else if (simulationsFinished == itemAmount) {
          $('#sim-all-items').text('Simulate All Items')
        }
        savedItemDps[itemSlot + itemSubSlot] = savedItemDps[itemSlot + itemSubSlot] || {}
        savedItemDps[itemSlot + itemSubSlot][simulationEnd.itemId] = simulationEnd.avgDps
        localStorage.savedItemDps = JSON.stringify(savedItemDps)

        if (simulationsFinished === itemAmount) {
          // Remove the background coloring (progress bar)
          $('.btn').css('background', '')

          if (itemAmount === 1) {
            // Setup the damage breakdown table (showing avg damage, avg cast etc. for each spell)
            $('.spell-damage-information').remove()
            for (const spell in simulationEnd.damageBreakdown) {
              const s = simulationEnd.damageBreakdown[spell]
              const percentDamage = (~~((s.damage / simulationEnd.totalDamage) * 10000) / 100).toFixed(2)
              if (s.damage > 0 || s.casts > 0) $('#damage-breakdown-table tbody').append("<tr class='spell-damage-information'><td>" + s.name + "</td><td><meter value='" + percentDamage + "' min='0' max='100'></meter> " + percentDamage + "%</td><td class='number'>" + Math.ceil(s.casts / simulationEnd.iterations) + "</td><td class='number'>" + ~~(s.damage / s.casts) + (s.dotDamage ? ('(' + ~~(s.dotDamage / s.casts) + ')') : '') + "</td><td class='number'>" + ((~~(((s.crits / s.casts) * 100) * 100)) / 100).toFixed(2) + "</td><td class='number'>" + (~~(((s.misses / s.casts) * 100) * 100) / 100).toFixed(2) + "</td><td class='number'>" + (~~(((s.dodges / s.casts) * 100) * 100) / 100).toFixed(2) + "</td><td class='number'>" + (Math.round((s.damage / simulationEnd.totalDuration) * 100) / 100 || 0) + '</td></tr>').trigger('update')
            }
            // Setup the aura breakdown table
            for (const aura in simulationEnd.auraBreakdown) {
              const a = simulationEnd.auraBreakdown[aura]
              const percentUptime = (~~((a.uptime / simulationEnd.totalDuration) * 10000) / 100).toFixed(2)
              if (a.count > 0) {
                $("#aura-breakdown-table tbody").append("<tr class='spell-damage-information'><td>" + a.name + "</td><td>" + Math.ceil(a.count / simulationEnd.iterations) + "</td><td><meter value='" + percentUptime + "' min='0' max='100'></meter> " + percentUptime + "%</td></tr>")
              }
            }
            // Setup the mana gain breakdown table
            for (const element in simulationEnd.manaGainBreakdown) {
              const e = simulationEnd.manaGainBreakdown[element]
              const percentOfGain = (~~((e.manaGain / simulationEnd.totalManaRegenerated) * 10000) / 100).toFixed(2)
              if (e.casts > 0) {
                $("#mana-gain-breakdown-table tbody").append("<tr class='spell-damage-information'><td>" + e.name +"</td><td><meter value='" + percentOfGain + "' min='0' max='100'></meter> " + percentOfGain + "</td><td>" + Math.ceil(e.casts / simulationEnd.iterations) + "</td><td>" + ~~(e.manaGain / e.casts) + "</td<</tr>")
              }
            }
            $(".breakdown-section").css("display", "inline-block")
          }
        }

        // Start a new simulation that's waiting in the queue if there are any remaining
        if (simulationsRunning - simulationsFinished < maxWorkers && simIndex < simulations.length) {
          simulations[simIndex++].start()
        }
      },
      (simulationUpdate) => {
        if (itemAmount === 1) {
          $('#avg-dps').text(simulationUpdate.avgDps)
          // Uses the sim button as a progress bar by coloring it based on how many iterations are done with
          $('#sim-dps').css('background', 'linear-gradient(to right, #9482C9 ' + simulationUpdate.percent + '%, transparent ' + simulationUpdate.percent + '%)')
          $('#sim-dps').text(Math.round(simulationUpdate.percent) + '%')
        } else {
          // multiSimInfo tracks the % progress of each simulation and the average simulation progress % is used for the multi-item simulation progress bar
          let totalProgress = 0
          for (let i = 0; i < multiSimInfo.length; i++) {
            if (multiSimInfo[i][0] == simulationUpdate.itemId) {
              multiSimInfo[i][1] = simulationUpdate.percent
            }
            totalProgress += multiSimInfo[i][1]
          }

          const averageProgress = ~~(totalProgress / simulations.length)
          $('#sim-all-items').css('background', 'linear-gradient(to right, #9482C9 ' + averageProgress + '%, transparent ' + averageProgress + '%)')
          $('#sim-all-items').text(averageProgress + '%')
        }
        // Set the DPS value on the item in the item selection list
        $(".item-row[data-wowhead-id='" + simulationUpdate.itemId + "']").find('.item-dps').text(simulationUpdate.avgDps)
        $('#item-selection-table').trigger('update')
      },
      {
        player: Player.getSettings(),
        simulation: Simulation.getSettings(),
        itemSlot: itemSlot,
        itemSubSlot: itemSubSlot,
        itemId: items[i],
        itemAmount: itemAmount
      }
    ))
  }

  // Start as many simulations as 'maxWorkers' says can be run simultaneously.
  while (simulationsRunning < maxWorkers && simIndex < simulations.length) {
    simulations[simIndex++].start()
    simulationsRunning++
  }
}

function updateStatWeight(stat, value) {
  $("#stat-weight-" + stat).text(Math.max(0,value))
}

function simStatWeights() {
  const stats = {
    normal: 0,
    intellect: 100,
    spellPower: 100,
    shadowPower: 100,
    firePower: 100,
    critRating: 100,
    hasteRating: 100,
    mp5: 100
  }
  // Filthy way of getting the current hit % from the sidebar hit % value.
  // The idea of these hit rating calculations is to put the player at hit cap to get the value of hit rather than just giving a static value like with other stats since they don't have a cap.
  const missingHitChance = 16 - $("#character-hit-val").text().split("(")[1].split("%")[0]
  const missingHitRating = Math.floor(missingHitChance * hitRatingPerPercent)
  stats.hitRating = Math.max(missingHitRating,1)
  let sims = []
  let simInfo = []
  let normalSimAvgDps = 0
  let simsFinished = 0

  const simStart = performance.now()
  sims.push(new SimWorker(
    (simulationEnd) => {
      simsFinished++
      normalSimAvgDps = simulationEnd.avgDps
      $('#avg-dps').text(simulationEnd.avgDps)
    },
    (simulationUpdate) => {
      normalSimAvgDps = simulationUpdate.avgDps
      $('#avg-dps').text(simulationUpdate.avgDps)
    },
    {
      player: Player.getSettings(),
      simulation: Simulation.getSettings(),
    }
  ).start())

  for (const stat in stats) {
    sims.push(new SimWorker(
      (simulationEnd) => {
        // Round to 3 decimals
        const statValue = Math.round((((simulationEnd.avgDps - normalSimAvgDps) / stats[simulationEnd.customStat.stat]) * 1000)) / 1000
        updateStatWeight(simulationEnd.customStat.stat, statValue)
        simsFinished++

        if (simsFinished == sims.length) {
          let pawnString = '( Pawn: v1: "' + (localStorage.selectedProfile || "Warlock") + '": Class=Warlock, Spec=Affliction, Intellect=' + $("#stat-weight-intellect").text()
          + ', SpellCritRating=' + $("#stat-weight-critRating").text()
          + ', SpellHitRating=' + $("#stat-weight-hitRating").text()
          + ', FireSpellDamage=' + $("#stat-weight-firePower").text()
          + ', ShadowSpellDamage=' + $("#stat-weight-shadowPower").text()
          + ', SpellDamage=' + $("#stat-weight-spellPower").text()
          + ', Mp5=' + $("#stat-weight-mp5").text()
          + ', SpellHasteRating=' + $("#stat-weight-hasteRating").text() + ')'
          $("#pawn-import-string p").text(pawnString)
          $("#sim-length-result").text((performance.now() - simStart) / 1000 + "s")
          $("#sim-stat-weights").text("Stat Weights")
          $('.btn').css('background', '')
        }
      },
      (simulationUpdate) => {
        const statValue = Math.round((((simulationUpdate.avgDps - normalSimAvgDps) / stats[simulationUpdate.customStat.stat]) * 1000)) / 1000
        updateStatWeight(simulationUpdate.customStat.stat, statValue)

        let smallestValue = 100
        for (let i = 0; i < simInfo.length; i++) {
          if (simInfo[i][0] == simulationUpdate.customStat.stat) {
            simInfo[i][1] = simulationUpdate.percent
          }
          if (simInfo[i][1] < smallestValue) smallestValue = simInfo[i][1]
        }

          $('#sim-stat-weights').css('background', 'linear-gradient(to right, #9482C9 ' + smallestValue + '%, transparent ' + smallestValue + '%)')
          $('#sim-stat-weights').text(smallestValue + '%')
      },
      {
        player: Player.getSettings(),
        simulation: Simulation.getSettings(),
        customStat: stat,
        customStatValue: stats[stat]
      }
    ).start())
    simInfo.push([stat,0])
  }

  $("#stat-weights-section").show()
}

function updateTalentTreeNames () {
  $('.talent-tree-table').each(function () {
    let talentTreeName = $(this).data('name')
    talentTreeName = talentTreeName.charAt(0).toUpperCase() + talentTreeName.slice(1)
    if ($(this).data('points') > 0) {
      talentTreeName += ' (' + $(this).data('points') + ')'
    }
    $(".talent-tree-name h3[data-name='" + $(this).data('name') + "']").text(talentTreeName)
  })
}

function updateTalentInformation (talentUiObj) {
  // Update the point amount on the talent icon
  talentUiObj.children('a').children('.talent-point-amount').text(talentUiObj.attr('data-points'))
  // if the point amount is 0 then we hide the amount
  if (talentUiObj.children('a').children('.talent-point-amount').text() > 0) {
    talentUiObj.children('a').children('.talent-point-amount').show()
  } else {
    talentUiObj.children('a').children('.talent-point-amount').hide()
  }
  talentUiObj.children('a').attr('href', 'https://tbc.wowhead.com/spell=' + _talents[talentUiObj.data('tree')][talentUiObj.attr('id')].rankIDs[Math.max(0, talents[talentUiObj.attr('id')] - 1)])
  // $WowheadPower.refreshLinks();

  // todo: move these JS css changes to the css file
  if (Number(talentUiObj.attr('data-points')) == Number(talentUiObj.attr('data-maxpoints'))) {
    talentUiObj.children('a').children('span').css('color', '#ffcd45')
  } else {
    talentUiObj.children('a').children('span').css('color', '#7FFF00')
  }
}

function clearTalentTree (talentTreeName) {
  if ($('#talent-table-' + talentTreeName).data('points') > 0) {
    for (const talent in _talents[talentTreeName]) {
      talentPointsRemaining += talents[talent]
      talents[talent] = 0
      $('#' + talent).attr('data-points', 0)
      $('#talent-table-' + talentTreeName).data('points', 0)
      updateTalentInformation($('#' + talent))
    }
    updateTalentTreeNames()
    localStorage.talents = JSON.stringify(talents)
  }

  refreshCharacterStats()
  updateSimulationSettingsVisibility()
}

function updateSetBonuses () {
  $('.sidebar-set-bonus').remove()
  const setBonusCounter = {}

  for (let itemSlot in selectedItems) {
    const itemId = selectedItems[itemSlot]
    if (itemId) {
      if (itemSlot == 'ring1' || itemSlot == 'ring2' || itemSlot == 'trinket1' || itemSlot == 'trinket2') {
        itemSlot = itemSlot.substring(0, itemSlot.length - 1)
      }
      for (const item in items[itemSlot]) {
        if (items[itemSlot][item].id === itemId) {
          const setID = items[itemSlot][item].setId
          if (setID) {
            setBonusCounter[setID] = setBonusCounter[setID] + 1 || 1
            break
          }
        }
      }
    }
  }

  for (set in setBonusCounter) {
    // Check if the item's set has actually been implemented
    if (sets[set]) {
      for (let i = sets[set].bonuses.length - 1; i >= 0; i--) {
        if (sets[set].bonuses[i] <= setBonusCounter[set]) {
          $('#sidebar-sets').append("<li class='sidebar-set-bonus'><a href='https://tbc.wowhead.com/item-set=" + set + "'>" + sets[set].name + ' (' + setBonusCounter[set] + ')</a></li>')
          break
        }
      }
    }
  }

  localStorage.setBonuses = JSON.stringify(setBonusCounter)
}

function drawProfileButtons () {
  $('.saved-profile').remove()
  for (const profile in profiles) {
    $('#saved-profiles ul').append("<li class='saved-profile' data-checked='" + (localStorage.selectedProfile == profile || false) + "' data-name='" + profile + "'>" + profile + '</li>')
  }
  // Show the profile fieldset if there are any profiles
  if ($('.saved-profile').length > 0) {
    $('#saved-profiles').show()
  }
}

function saveProfile (profileName) {
  profiles[profileName] = {
    auras: auras,
    rotation: rotation,
    simSettings: settings,
    talents: talents,
    items: selectedItems,
    gems: selectedGems, // unsure if gems should be saved but it could be useful in case the different profiles want to use different gems
    enchants: selectedEnchants
  }
  localStorage.profiles = JSON.stringify(profiles)
}

// Called when the user creates a new profile or clicks on an existing one. It updates the 'selected' attribute for the profiles and displays the profile update buttons
function updateProfileSelection (profileName) {
  // De-select the previous profile
  if (localStorage.selectedProfile) {
    $(".saved-profile[data-name='" + localStorage.selectedProfile + "']").attr('data-checked', false)
  }
  localStorage.selectedProfile = profileName
  $(".saved-profile[data-name='" + profileName + "']").attr('data-selected', true)
  $('#update-profile-div').show()
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

function updateSimulationSettingsVisibility () {
  if (talents.demonicSacrifice === 0) {
    $("#sacrificePet").hide()
  } else {
    $("#sacrificePet").show()
  }

  if ($("#sacrificePet").is(":visible") && $("#sacrificePet").children("select").val() == "yes") {
    $("#petMode").hide()
  } else {
    $("#petMode").show()
  }

  if ($('#sacrificePet').children('select').val() == 'no' || !$("#sacrificePet").is(":visible")) {
    $('#petBuffs-heading').show()
    $('.petBuffs').show()
    if ($('#petMode').children('select').val() == PetMode.AGGRESSIVE) {
      if ($("#petChoice").children("select").val() != PetName.IMP) {
        $('#enemyArmor').show()
        $('#enemy-armor-val').closest('li').show()
      } else {
        $('#enemyArmor').hide()
        $('#enemy-armor-val').closest('li').hide()
      }
      $('.petDebuff').show()
    } else {
      $('#enemyArmor').hide()
      $('#enemy-armor-val').closest('li').hide()
      $('.petDebuff').hide()
    }
  } else {
    $('#petBuffs-heading').hide()
    $('.petBuffs').hide()
    $('#enemyArmor').hide()
    $('#enemy-armor-val').closest('li').hide()
    $('.petDebuff').hide()
  }

  if ($("#petChoice").children("select").val() == PetName.SUCCUBUS && $("#petMode").children("select").val() == PetMode.AGGRESSIVE) {
    $("#lashOfPainUsage").show()
  } else {
    $("#lashOfPainUsage").hide()
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

  if (talents.conflagrate > 0) {
    $('#conflagrateUse').show()
  } else {
    $('#conflagrateUse').hide()
  }

  if (auras.powerOfTheGuardianMage) {
    $('#mageAtieshAmount').show()
  } else {
    $('#mageAtieshAmount').hide()
  }

  if (auras.powerOfTheGuardianWarlock) {
    $('#warlockAtieshAmount').show()
  } else {
    $('#warlockAtieshAmount').hide()
  }

  if (auras.bloodlust) {
    $('#bloodlustAmount').show()
  } else {
    $('#bloodlustAmount').hide()
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
}
