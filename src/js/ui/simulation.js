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
  const equippedItemId = $('.item-row[data-selected="true"]').data('wowhead-id')
  let simulationsRunning = 0
  let simulationsFinished = 0
  const multiSimInfo = []
  const simulations = []
  let simIndex = 0
  let combatLog = []
  let combatLogBreakdownArr = []
  let playerHasMeleePet = false
  let totalManaRegenerated = 0
  let totalDamageDone = 0
  let spellDamageDict = {}
  let spellManaGainDict = {}
  let startTime = performance.now()

  if (items.length > 1) {
    $('.item-dps').text('')
  }

  for (let i = 0; i < items.length; i++) {
    multiSimInfo.push([items[i], 0])

    simulations.push(new SimWorker(
      // Combat Log Vector callback
      (combatLogVector) => {
        spellDamageDict[combatLogVector.name] = spellDamageDict[combatLogVector.name] + combatLogVector.damage || combatLogVector.damage
        spellManaGainDict[combatLogVector.name] = spellManaGainDict[combatLogVector.name] + combatLogVector.manaGain || combatLogVector.manaGain
        totalManaRegenerated += combatLogVector.manaGain
        totalDamageDone += combatLogVector.damage
      },
      // Error callback
      (errorCallback) => {
        populateCombatLog(combatLog)
        errorCallbackHandler(errorCallback)
      },
      // Combat Log Update callback
      (combatLogUpdate) => {
        combatLog.push(combatLogUpdate.combatLogEntry);
      },
      // Combat Log Breakdown callback
      (combatLogBreakdown) => {
        combatLogBreakdownArr.push(combatLogBreakdown)

        if (combatLogBreakdown.name == "Melee") {
          playerHasMeleePet = true
        }
      },
      // Simulation End callback
      (simulationEnd) => {
        let minDps = Math.round(simulationEnd.minDps * 100) / 100
        let maxDps = Math.round(simulationEnd.maxDps * 100) / 100
        let medianDps = Math.round(simulationEnd.medianDps * 100) / 100
        simulationsFinished++
        
        if (simulationEnd.itemId == equippedItemId || itemAmount === 1) {
          localStorage.medianDps = medianDps
          localStorage.minDps = minDps
          localStorage.maxDps = maxDps
          $('#avg-dps').text(medianDps)
          $('#min-dps').text(minDps)
          $('#max-dps').text(maxDps)
        }
        // DPS information on the sidebar
        if (itemAmount === 1) {
          $('#sim-dps').text('Simulate')

          populateCombatLog(combatLog)
        } else if (simulationsFinished == itemAmount) {
          $('#sim-all-items').text('Simulate All Items')
        }
        savedItemDps[itemSlot + itemSubSlot] = savedItemDps[itemSlot + itemSubSlot] || {}
        savedItemDps[itemSlot + itemSubSlot][simulationEnd.itemId] = medianDps
        localStorage.savedItemDps = JSON.stringify(savedItemDps)

        if (simulationsFinished === itemAmount) {
          let totalSimDuration = (performance.now() - startTime) / 1000
          localStorage.simulationDuration = Math.round(totalSimDuration * 10000) / 10000
          $('#sim-length-result').text(localStorage.simulationDuration + 's')
          // Remove the background coloring (progress bar)
          $('.btn').css('background', '')

          if (itemAmount === 1 && $('#automatically-open-sim-details').children('select').val() === 'yes') {
            // Setup the damage breakdown table (showing avg damage, avg cast etc. for each spell)
            $('.spell-damage-information').remove()

            for (const combatLogBreakdown in combatLogBreakdownArr) {
              const s = combatLogBreakdownArr[combatLogBreakdown]

              // Damage
              if (s.casts > 0) {
                if (spellDamageDict[s.name] > 0 || s.misses > 0 || s.crits > 0 || s.glancingBlows > 0 || s.dodges > 0) {
                  const percentDamage = (~~((spellDamageDict[s.name] / totalDamageDone) * 10000) / 100).toFixed(2)
                  var tableRow = "<tr class='spell-damage-information'><td>" + s.name + "</td><td><meter value='" + percentDamage + "' min='0' max='100'></meter> " + percentDamage + "%</td><td class='number'>" + Math.ceil(s.casts / simulationEnd.iterationAmount) + "</td><td class='number'>" + ~~(spellDamageDict[s.name] / s.casts) + (s.dotDamage ? ('(' + ~~(s.dotDamage / s.casts) + ')') : '') + "</td><td class='number'>" + ((~~(((s.crits / s.casts) * 100) * 100)) / 100).toFixed(2) + "</td><td class='number'>" + (~~(((s.misses / s.casts) * 100) * 100) / 100).toFixed(2) + "</td>"

                  // Only add the dodge and glancing cells if the player has a melee pet
                  if (playerHasMeleePet) {
                    tableRow += "<td class='number'>" + (~~(((s.dodges / s.casts) * 100) * 100) / 100).toFixed(2) + "</td><td class='number'>" + (~~(((s.glancingBlows / s.casts) * 100) * 100) / 100).toFixed(2) + "</td>"
                  }
                  tableRow += "<td class='number'>" + (Math.round((spellDamageDict[s.name] / simulationEnd.totalDuration) * 100) / 100 || 0) + '</td></tr>'
                  $('#damage-breakdown-table tbody').append(tableRow)
                }

                // Mana Gain
                if (spellManaGainDict[s.name] > 0) {
                  const percentOfGain = (~~((spellManaGainDict[s.name] / totalManaRegenerated) * 10000) / 100).toFixed(2)
                  $('#mana-gain-breakdown-table tbody').append("<tr class='spell-damage-information'><td>" + s.name + "</td><td><meter value='" + percentOfGain + "' min='0' max='100'></meter> " + percentOfGain + '</td><td>' + Math.ceil(s.casts / simulationEnd.iterationAmount) + '</td><td>' + ~~(spellManaGainDict[s.name] / s.casts) + '</td<</tr>')
                }
              }

              // Aura
              if (s.count > 0) {
                const percentUptime = (~~((s.uptime / simulationEnd.totalDuration) * 10000) / 100).toFixed(2)
                $('#aura-breakdown-table tbody').append("<tr class='spell-damage-information'><td>" + s.name + '</td><td>' + Math.ceil(s.count / simulationEnd.iterationAmount) + "</td><td><meter value='" + percentUptime + "' min='0' max='100'></meter> " + percentUptime + '%</td></tr>')
              }

              $(".breakdown-table").trigger('update')
            }

            $('.breakdown-table tbody').trigger('update')
            $('.breakdown-section').css('display', 'inline-block')
            // Hide the Glancing and Dodge columns if not using a melee pet
            if (playerHasMeleePet) {
              $('#damage-breakdown-dodge').show()
              $('#damage-breakdown-glancing').show()
            } else {
              $('#damage-breakdown-dodge').hide()
              $('#damage-breakdown-glancing').hide()
            }
          }
        }

        updateItemRowDps(simulationEnd.itemId, medianDps)

        // Start a new simulation that's waiting in the queue if there are any remaining
        if (simulationsRunning - simulationsFinished < maxWorkers && simIndex < simulations.length) {
          simulations[simIndex++].start()
        }
      },
      // Simulation Update callback
      (simulationUpdate) => {
        let medianDps = Math.round(simulationUpdate.medianDps * 100) / 100

        if (simulationUpdate.itemId == equippedItemId || itemAmount == 1) {
          $('#avg-dps').text(medianDps)
        }
        if (itemAmount === 1) {
          let progressPercent = Math.ceil((simulationUpdate.iteration / simulationUpdate.iterationAmount) * 100)
          // Uses the sim button as a progress bar by coloring it based on how many iterations are done with
          $('#sim-dps').css('background', 'linear-gradient(to right, #9482C9 ' + progressPercent + '%, transparent ' + progressPercent + '%)')
          $('#sim-dps').text(Math.round(progressPercent) + '%')
        } else {
          // multiSimInfo tracks the % progress of each simulation and the average simulation progress % is used for the multi-item simulation progress bar
          let totalProgress = 0
          for (let i = 0; i < multiSimInfo.length; i++) {
            if (multiSimInfo[i][0] == simulationUpdate.itemId) {
              multiSimInfo[i][1] = Math.ceil((simulationUpdate.iteration / simulationUpdate.iterationAmount) * 100)
            }
            totalProgress += multiSimInfo[i][1]
          }

          const averageProgress = ~~(totalProgress / simulations.length)
          $('#sim-all-items').css('background', 'linear-gradient(to right, #9482C9 ' + averageProgress + '%, transparent ' + averageProgress + '%)')
          $('#sim-all-items').text(averageProgress + '%')
        }
        // Set the DPS value on the item in the item selection list
        updateItemRowDps(simulationUpdate.itemId, medianDps)
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

  // Normal sim with no added or removed stats
  sims.push(new SimWorker(
    // Combat Log Vector callback
    (combatLogVector) => {},
    // Error callback
    (errorCallback) => {
      errorCallbackHandler(errorCallback)
    },
    // Combat Log Update callback
    (combatLogUpdate) => {},
    // Combat Log Breakdown callback
    (combatLogBreakdown) => {},
    (simulationEnd) => {
      simsFinished++
      normalSimMedianDps = simulationEnd.medianDps
      $('#avg-dps').text(Math.round(normalSimMedianDps * 100) / 100)
    },
    (simulationUpdate) => {
      normalSimMedianDps = simulationUpdate.medianDps
      $('#avg-dps').text(Math.round(normalSimMedianDps * 100) / 100)
    },
    {
      player: Player.getSettings(),
      simulation: Simulation.getSettings()
    }
  ).start())

  for (const stat in stats) {
    sims.push(new SimWorker(
      // Combat Log Vector callback
      (combatLogVector) => {},
      // Error callback
      (errorCallback) => {
        errorCallbackHandler(errorCallback)
      },
      // Combat Log Update callback
      (combatLogUpdate) => {},
      // Combat Log Breakdown callback
      (combatLogBreakdown) => {},
      (simulationEnd) => {
        // Round to 3 decimals
        const statValue = Math.round((((simulationEnd.medianDps - normalSimMedianDps) / stats[simulationEnd.customStat]) * 1000)) / 1000
        updateStatWeight(simulationEnd.customStat, statValue)
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
        const statValue = Math.round((((simulationUpdate.medianDps - normalSimMedianDps) / stats[simulationUpdate.customStat]) * 1000)) / 1000
        updateStatWeight(simulationUpdate.customStat, statValue)

        let smallestValue = 100
        for (let i = 0; i < simInfo.length; i++) {
          if (simInfo[i][0] == simulationUpdate.customStat) {
            simInfo[i][1] = Math.ceil((simulationUpdate.iteration / simulationUpdate.iterationAmount) * 100)
          }
          if (simInfo[i][1] < smallestValue) {
            smallestValue = simInfo[i][1]
          }
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

function errorCallbackHandler(errorCallback) {
  alert("Error: " + errorCallback.errorMsg + "\nPost in #tbc-sim-report on the TBC Warlock Discord or contact Kristofer#8003 on Discord.")
}

function updateItemRowDps(itemId, medianDps) {
  $(".item-row[data-wowhead-id='" + itemId + "']").find('.item-dps').text(medianDps)
  $('#item-selection-table').trigger('update')
}

function populateCombatLog(combatLog) {
  $('#combat-log p').remove()
  for (const entry in combatLog) {
    $('#combat-log').append('<p>' + combatLog[entry] + '</p>')
  }
}