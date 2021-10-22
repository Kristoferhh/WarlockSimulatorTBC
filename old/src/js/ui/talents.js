// User clicks on one of the preset talent buttons
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

  localStorage.rotation = JSON.stringify(rotation)
  localStorage.talents = JSON.stringify(talents)
  refreshCharacterStats()
  updateSimulationSettingsVisibility()
})

// Disable the context menu from appearing when the user right clicks a talent
$(document).on('contextmenu', '.talent-icon', function () {
  return false
})

// Prevents the user from being redirected to the talent's wowhead page when clicking it.
$(document).on('click', '.talent-icon', function () {
  return false
})

// Fires when the user left or right clicks a talent
$(document).on('mousedown', '.talent-icon', function () {
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
$(document).on('click', '.clear-talent-tree', function () {
  clearTalentTree($(this).closest('div').find('h3').data('name'))
})

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