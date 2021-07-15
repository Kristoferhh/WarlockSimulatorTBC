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

// "Fill Item Sockets" button clicked
$('#gem-options-button').click(function () {
  $('#gem-options-window').toggle()

  if ($('#gem-options-window').is(':visible')) {
    const selectedSocketColor = $('#gem-options-window-socket-selection input[type="radio"]:checked').val()
    refreshGemOptionsGems(selectedSocketColor)
  }
})

// When one of the socket selection radio buttons gets changed in the "Fill Item Sockets"
$('#gem-options-window-socket-selection').change(function () {
  const selectedSocketColor = $('#gem-options-window-socket-selection input[type="radio"]:checked').val()
  refreshGemOptionsGems(selectedSocketColor)
})

// A gem in the "Fill Item Sockets" options gets clicked
$(document).on('click', '.gem-options-gem', function () {
  $('.gem-options-gem').attr('data-checked', 'false')
  $(this).attr('data-checked', 'true')
  return false
})

$('#gem-options-apply-button').click(function () {
  alert($('.gem-options-gem').attr('data-checked').length)
})

// Adds gems to the "Fill Item Sockets" window
function refreshGemOptionsGems (socketColor) {
  // Remove all currently listed gems
  $('.gem-options-gem').remove()

  for (const socket in gems) {
    // If the socket is a meta gem socket then only show meta gems, otherwise show all non-meta gems
    if ((socketColor === 'meta' && socket === 'meta') || (socketColor !== 'meta' && socket !== 'meta')) {
      for (const gem in gems[socket]) {
        const g = gems[socket][gem]
        if (!gemPreferences.hidden.includes(Number(gem))) {
          $('#gem-options-gem-list').append('<div class="gem-options-gem" data-gem-id="' + gem +'"><img src="img/' + g.iconName + '.jpg"><a href="https://tbc.wowhead.com/item=' + gem +'">' + g.name + '</a></div>')
        }
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
            // Check whether the stat is a modifier or not. If it's a modifier then multiply/divide rather than add/substract
            if (property.toLowerCase().search('modifier') !== -1) { 
              characterStats[property] *= gems[color][gemId][property]
            } else {
              characterStats[property] += gems[color][gemId][property]
            }
          } else if (action == 'remove') {
            if (property.toLowerCase().search('modifier') !== -1) { 
              characterStats[property] /= gems[color][gemId][property]
            } else {
              characterStats[property] -= gems[color][gemId][property]
            }
          }
        }
      }
      return
    }
  }
}