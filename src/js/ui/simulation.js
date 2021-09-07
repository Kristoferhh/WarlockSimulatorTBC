// to-do: don't allow people to start multiple simulations
// Starts a normal simulation
$('#sim-dps').click(function () {
  simDPS([$(".item-row[data-selected='true']").data('wowhead-id')])
  return false
})

// Starts a multi-item simulation
$('#sim-all-items').click(function () {
  const arr = []
  $('.item-row').each(function (i) {
    arr.push($(this).data('wowhead-id'))
  })
  $('.breakdown-section').hide()
  simDPS(arr)
  return false
})

// Starts a stat weight simulation
$('#sim-stat-weights').click(function () {
  simStatWeights()
})

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

  if (items.length > 1) {
    $('.item-dps').text('')
  }

  for (let i = 0; i < items.length; i++) {
    multiSimInfo.push([items[i], 0])

    simulations.push(new SimWorker(
      (simulationEnd) => {
        let minDps = simulationEnd.minDps
        let maxDps = simulationEnd.maxDps
        simulationsFinished++

        // DPS information on the sidebar
        if (itemAmount === 1) {
          localStorage.medianDps = simulationEnd.medianDps
          localStorage.minDps = minDps
          localStorage.maxDps = maxDps
          localStorage.simulationDuration = simulationEnd.length
          $('#avg-dps').text(simulationEnd.medianDps)
          $('#min-dps').text(minDps)
          $('#max-dps').text(maxDps)
          $('#sim-length-result').text(Math.round(simulationEnd.length * 10000) / 10000 + 's')
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
        savedItemDps[itemSlot + itemSubSlot][simulationEnd.itemId] = simulationEnd.medianDps
        localStorage.savedItemDps = JSON.stringify(savedItemDps)

        if (simulationsFinished === itemAmount) {
          // Remove the background coloring (progress bar)
          $('.btn').css('background', '')

          if (itemAmount === 1) {
            if ($('#automatically-open-sim-details').children('select').val() === 'yes') {
              // Setup the damage breakdown table (showing avg damage, avg cast etc. for each spell)
              $('.spell-damage-information').remove()
              let hasPet = simulationEnd.damageBreakdown.melee && simulationEnd.damageBreakdown.melee.casts > 0

              for (const spell in simulationEnd.damageBreakdown) {
                const s = simulationEnd.damageBreakdown[spell]
                const percentDamage = (~~((s.damage / simulationEnd.totalDamage) * 10000) / 100).toFixed(2)
                if (s.damage > 0 || s.casts > 0) {
                  var tableRow = "<tr class='spell-damage-information'><td>" + s.name + "</td><td><meter value='" + percentDamage + "' min='0' max='100'></meter> " + percentDamage + "%</td><td class='number'>" + Math.ceil(s.casts / simulationEnd.iterations) + "</td><td class='number'>" + ~~(s.damage / s.casts) + (s.dotDamage ? ('(' + ~~(s.dotDamage / s.casts) + ')') : '') + "</td><td class='number'>" + ((~~(((s.crits / s.casts) * 100) * 100)) / 100).toFixed(2) + "</td><td class='number'>" + (~~(((s.misses / s.casts) * 100) * 100) / 100).toFixed(2) + "</td>"
                  // Only add the dodge and glancing cells if the player has a pet and it's not an imp
                  if (hasPet) {
                    tableRow += "<td class='number'>" + (~~(((s.dodges / s.casts) * 100) * 100) / 100).toFixed(2) + "</td><td class='number'>" + (~~(((s.glancingBlows / s.casts) * 100) * 100) / 100).toFixed(2) + "</td>"
                  }
                  tableRow += "<td class='number'>" + (Math.round((s.damage / simulationEnd.totalDuration) * 100) / 100 || 0) + '</td></tr>'
                  $('#damage-breakdown-table tbody').append(tableRow)
                }
              }
              // Setup the aura breakdown table
              for (const aura in simulationEnd.auraBreakdown) {
                const a = simulationEnd.auraBreakdown[aura]
                const percentUptime = (~~((a.uptime / simulationEnd.totalDuration) * 10000) / 100).toFixed(2)
                if (a.count > 0) {
                  $('#aura-breakdown-table tbody').append("<tr class='spell-damage-information'><td>" + a.name + '</td><td>' + Math.ceil(a.count / simulationEnd.iterations) + "</td><td><meter value='" + percentUptime + "' min='0' max='100'></meter> " + percentUptime + '%</td></tr>')
                }
              }
              // Setup the mana gain breakdown table
              for (const element in simulationEnd.manaGainBreakdown) {
                const e = simulationEnd.manaGainBreakdown[element]
                const percentOfGain = (~~((e.manaGain / simulationEnd.totalManaRegenerated) * 10000) / 100).toFixed(2)
                if (e.casts > 0) {
                  $('#mana-gain-breakdown-table tbody').append("<tr class='spell-damage-information'><td>" + e.name + "</td><td><meter value='" + percentOfGain + "' min='0' max='100'></meter> " + percentOfGain + '</td><td>' + Math.ceil(e.casts / simulationEnd.iterations) + '</td><td>' + ~~(e.manaGain / e.casts) + '</td<</tr>')
                }
              }
              $('.breakdown-section').css('display', 'inline-block')
              // Hide the Glancing and Dodge columns if not using a melee pet
              if (hasPet) {
                $('#damage-breakdown-dodge').show()
                $('#damage-breakdown-glancing').show()
              } else {
                $('#damage-breakdown-dodge').hide()
                $('#damage-breakdown-glancing').hide()
              }

              $(".breakdown-table").trigger('update')
            }
          }
        }

        // Start a new simulation that's waiting in the queue if there are any remaining
        if (simulationsRunning - simulationsFinished < maxWorkers && simIndex < simulations.length) {
          simulations[simIndex++].start()
        }
      },
      (simulationUpdate) => {
        if (itemAmount === 1) {
          $('#avg-dps').text(simulationUpdate.medianDps)
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
        $(".item-row[data-wowhead-id='" + simulationUpdate.itemId + "']").find('.item-dps').text(simulationUpdate.medianDps)
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

function updateStatWeight (stat, value) {
  // If the value is smaller than 0.01 then just display 0 since the 0.01 value is likely just because it's not using the same Random seed for all simulations
  $('#stat-weight-' + stat).text(Math.max(0, value > 0.01 ? value : 0))
}

function simStatWeights () {
  const statAmount = 100
  const stats = {
    normal: 0,
    intellect: statAmount,
    spellPower: statAmount,
    shadowPower: statAmount,
    firePower: statAmount,
    critRating: statAmount,
    hasteRating: statAmount,
    mp5: statAmount
  }
  // Only check for the hit rating weight if the player isn't hit capped (this gets the player's hit % from the sidebar since we don't have any variable with hit % included)
  let hitPercent = Number($('#character-hit-val').text().split('(')[1].split('%')[0])
  let baseHitChance = getBaseHitChance($('#character-level span').text(), $('#target-level').val())
  let playerHitChance = hitPercent + baseHitChance
  
  if (playerHitChance >= 99) {
    updateStatWeight('hitRating', 0)
  } else {
    stats.hitRating = statAmount
    // If the player is too close to hit cap then we instead subtract hit rating from them instead of adding more,
    // since otherwise the extra hit would be wasted and it'd give the wrong result.
    if (playerHitChance >= 99 - (statAmount / hitRatingPerPercent)) {
      stats.hitRating *= -1
    }
  }

  const sims = []
  const simInfo = []
  let normalSimMedianDps = 0
  let simsFinished = 0

  const simStart = performance.now()
  sims.push(new SimWorker(
    (simulationEnd) => {
      simsFinished++
      normalSimMedianDps = simulationEnd.medianDps
      $('#avg-dps').text(normalSimMedianDps)
    },
    (simulationUpdate) => {
      normalSimMedianDps = simulationUpdate.medianDps
      $('#avg-dps').text(normalSimMedianDps)
    },
    {
      player: Player.getSettings(),
      simulation: Simulation.getSettings()
    }
  ).start())

  for (const stat in stats) {
    sims.push(new SimWorker(
      (simulationEnd) => {
        // Round to 3 decimals
        const statValue = Math.round((((simulationEnd.medianDps - normalSimMedianDps) / stats[simulationEnd.customStat.stat]) * 1000)) / 1000
        updateStatWeight(simulationEnd.customStat.stat, statValue)
        simsFinished++

        if (simsFinished == sims.length) {
          const pawnString = '( Pawn: v1: "' + (localStorage.selectedProfile || 'Warlock') + '": Class=Warlock, Spec=Affliction, Intellect=' + $('#stat-weight-intellect').text() +
          ', SpellCritRating=' + $('#stat-weight-critRating').text() +
          ', SpellHitRating=' + $('#stat-weight-hitRating').text() +
          ', FireSpellDamage=' + $('#stat-weight-firePower').text() +
          ', ShadowSpellDamage=' + $('#stat-weight-shadowPower').text() +
          ', SpellDamage=' + $('#stat-weight-spellPower').text() +
          ', Mp5=' + $('#stat-weight-mp5').text() +
          ', SpellHasteRating=' + $('#stat-weight-hasteRating').text() + ')'
          $('#pawn-import-string p').text(pawnString)
          $('#sim-length-result').text(Math.round(performance.now() - simStart) / 1000 + 's')
          $('#sim-stat-weights').text('Stat Weights')
          $('.btn').css('background', '')
        }
      },
      (simulationUpdate) => {
        const statValue = Math.round((((simulationUpdate.medianDps - normalSimMedianDps) / stats[simulationUpdate.customStat.stat]) * 1000)) / 1000
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
    simInfo.push([stat, 0])
  }

  $('#stat-weights-section').show()
}