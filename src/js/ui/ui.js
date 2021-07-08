// Maximum amount of web workers that should be run concurrently
const maxWorkers = navigator.hardwareConcurrency || 8
// Object with all the gems the user has equipped on any item, including items that are not equipped. Each key is the item's name and the value is an array with the ID of the gems equipped in that item.
var selectedGems = localStorage.selectedGems ? JSON.parse(localStorage.selectedGems) : {}
// Key: Item slot. Value: Equipped item's ID
var selectedItems = localStorage.selectedItems ? JSON.parse(localStorage.selectedItems) : {}
// Key: Talent's name. Value: Talent's point amount
var talents = localStorage.talents ? JSON.parse(localStorage.talents) : {}
// Key: Aura's name. Value: Boolean
var auras = localStorage.auras ? JSON.parse(localStorage.auras) : {}
var rotation = localStorage.rotation ? JSON.parse(localStorage.rotation) : {}
var selectedEnchants = localStorage.selectedEnchants ? JSON.parse(localStorage.selectedEnchants) : {}
// Key: Item ID. Value: Item's saved DPS from previous simulations.
const savedItemDps = localStorage.savedItemDps ? JSON.parse(localStorage.savedItemDps) : {}
var settings = localStorage.settings ? JSON.parse(localStorage.settings) : {}
const sources = localStorage.sources ? JSON.parse(localStorage.sources) : { phase: { 0: true, 1: true } }
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

// When the user clicks anywhere on the webpage
$(document).on('click', function (e) {
  // Hide the gem selection table if the user clicks outside of it.
  if (e.target.id !== 'gem-selection-table' && !e.target.className.split(' ').includes('gem-info')) {
    $('#gem-selection-table').css('visibility', 'hidden')
  }
})

// User clicks on the X on a section to hide it
$(document).on('click', '.close, #export-close-button', function () {
  $(this).closest('.close-button-target').css('display', 'none')
  return false
})

// These are the buttons on the sidebar
$('.btn').hover(function () {
  $(this).find('a').css('color', '#1a1a1a')
})

$('.btn').mouseout(function () {
  $(this).find('a').css('color', 'white')
})

// "Import/Export" button in the Profile Options fieldset
$('#import-export-button').click(function () {
  $('#import-export-window textarea').val('')
  $('#import-export-window').show()
})

// Importing settings
$('#import-button').click(function () {
  const data = JSON.parse($('#import-export-window textarea').val())
  if (data.auras) localStorage.auras = JSON.stringify(data.auras)
  if (data.selectedGems) localStorage.selectedGems = JSON.stringify(data.selectedGems)
  if (data.selectedItems) localStorage.selectedItems = JSON.stringify(data.selectedItems)
  if (data.talents) localStorage.talents = JSON.stringify(data.talents)
  if (data.rotation) localStorage.rotation = JSON.stringify(data.rotation)
  if (data.selectedEnchants) localStorage.selectedEnchants = JSON.stringify(data.selectedEnchants)
  if (data.settings) localStorage.settings = JSON.stringify(data.settings)
  location.reload()
})

// Exporting settings
$('#export-button').click(function () {
  const data =
  {
    auras: auras,
    selectedGems: selectedGems,
    selectedItems: selectedItems,
    talents: talents,
    rotation: rotation,
    selectedEnchants: selectedEnchants,
    settings: settings
  }
  $('#import-export-window textarea').val(JSON.stringify(data)).select()
})

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