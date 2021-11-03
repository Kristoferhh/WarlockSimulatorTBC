import { useState } from "react";
import { useDispatch, useSelector } from "react-redux"
import { average, calculatePlayerStats, getAllocatedTalentsPointsInTree, getItemSetCounts, getItemTableItems, getPlayerHitPercent, getStdev, ItemSlotKeyToItemSlot } from "../Common";
import { Gems } from "../data/Gems";
import { Items } from "../data/Items";
import { Talents } from "../data/Talents";
import { RootState } from "../redux/Store"
import { clearSavedItemSlotDps, setCombatLogBreakdownValue, setCombatLogData, setCombatLogVisibility, setHistogramData, setHistogramVisibility, setSavedItemDps, setStatWeightVisibility } from "../redux/UiSlice";
import { SimWorker } from "../SimWorker.js";
import { CombatLogBreakdownData, GemColor, ItemAndEnchantStruct, ItemSlot, PlayerState, SelectedGemsStruct, SimulationType, Stat, StatConstant, TalentTree, WorkerParams } from "../Types";

interface SimulationUpdate {
  medianDps: number,
  iteration: number,
  iterationAmount: number,
  itemId: number,
  customStat: string
}

interface SimulationEnd {
  customStat: string,
  itemId: number,
  iterationAmount: number,
  totalDuration: number,
  maxDps:  number,
  minDps: number,
  medianDps: number
}

interface IGetWorkerParams {
  itemId: number,
  equippedItemId: number,
  simulationType: SimulationType,
  customStat?: {
    stat: string,
    value: number
  }
}

interface ISimulationProgressPercent {
  itemId: number,
  progressPercent: number,
  customStat?: string
}

const statAmount = 100;
const statWeightValues: {[key: string]: number} = {
  'normal': 0,
  [Stat.stamina]: statAmount,
  [Stat.intellect]: statAmount,
  [Stat.spirit]: statAmount,
  [Stat.spellPower]: statAmount,
  [Stat.shadowPower]: statAmount,
  [Stat.firePower]: statAmount,
  [Stat.hitRating]: statAmount,
  [Stat.critRating]: statAmount,
  [Stat.hasteRating]: statAmount,
  [Stat.mp5]: statAmount,
}

function getEquippedMetaGemId(items: ItemAndEnchantStruct, gems: SelectedGemsStruct): number {
  if ([null, 0].includes(items.head) || !gems.head || !gems.head[items.head]) { return 0; }

  for (const gemArray of Object.values(gems.head[items.head])) {
    if (Gems.find(e => e.id === gemArray[1])?.color === GemColor.Meta) {
      return gemArray[1];
    }
  }

  return 0;
}

export function SimulationButtons() {
  const playerState = useSelector((state: RootState) => state.player);
  const uiState = useSelector((state: RootState) => state.ui);
  const dispatch = useDispatch();
  const [medianDps, setMedianDps] = useState(localStorage.getItem('medianDps') || '');
  const [minDps, setMinDps] = useState(localStorage.getItem('minDps') || '');
  const [maxDps, setMaxDps] = useState(localStorage.getItem('maxDps') || '');
  const [simulationDuration, setSimulationDuration] = useState(localStorage.getItem('simulationDuration') || '');
  const [simulationProgressPercent, setSimulationProgressPercent] = useState(0);
  const [simulationInProgress, setSimulationInProgress] = useState(false);
  const [dpsStdev, setDpsStdev] = useState('');
  const [simulationType, setSimulationType] = useState(SimulationType.Normal);
  let combatLogEntries: string[] = [];

  function getWorkerParams(params: IGetWorkerParams): WorkerParams {
    let customPlayerState: PlayerState = JSON.parse(JSON.stringify(playerState));
    let iterationAmount = parseInt(customPlayerState.settings.iterations);

    if (params.simulationType === SimulationType.StatWeights) {
      // Set minimum iteration amount to 10,000 for stat weight sims
      if (iterationAmount < 10000) {
        iterationAmount = 10000;
      }
      // Double the iteration amount for stat weight sims if it's not the 'normal' sim with no added stats.
      if (params.customStat?.stat !== 'normal') {
        iterationAmount *= 2;
      }
    }

    if (params.simulationType !== SimulationType.StatWeights) {
      customPlayerState.selectedItems[ItemSlotKeyToItemSlot(false, uiState.selectedItemSlot, uiState.selectedItemSubSlot)] = params.itemId;
    }

    let playerStats = calculatePlayerStats(customPlayerState);
    
    if (params.simulationType === SimulationType.StatWeights) {
      if (params.customStat?.stat && params.customStat.stat !== 'normal') {
        const hitPercent = getPlayerHitPercent(customPlayerState);
        let statValue = params.customStat.value;
        // If the user isn't hitcapped but adding the extra hit rating would overcap them, then instead remove hit rating instead of adding it so it doesn't get wasted.
        if (params.customStat.stat === Stat.hitRating && hitPercent < 16 && hitPercent + statWeightValues[Stat.hitRating] / StatConstant.hitRatingPerPercent > 16) {
          statValue *= -1;
        }
        playerStats[params.customStat.stat as Stat]! += statValue;
      }
    }

    return {
      playerSettings: {
        auras: customPlayerState.auras,
        items: customPlayerState.selectedItems,
        enchants: customPlayerState.selectedEnchants,
        gems: customPlayerState.selectedGems,
        talents: customPlayerState.talents,
        rotation: customPlayerState.rotation,
        stats: playerStats,
        sets: getItemSetCounts(customPlayerState.selectedItems),
        simSettings: customPlayerState.settings,
        metaGemId: getEquippedMetaGemId(customPlayerState.selectedItems, customPlayerState.selectedGems),
      },
      simulationSettings: {
        // Double the iteration amount if it's a stat weight sim and it's not the 'normal' sim with no added stats.
        // Also set a minimum amount of iterations to 10,000 for stat weight simulations.
        iterations: iterationAmount,
        minTime: parseInt(customPlayerState.settings["min-fight-length"]),
        maxTime: parseInt(customPlayerState.settings['max-fight-length'])
      },
      itemId: params.itemId,
      simulationType: params.simulationType,
      itemSubSlot: uiState.selectedItemSubSlot,
      customStat: params.customStat?.stat ||  'normal',
      equippedItemSimulation: params.itemId === params.equippedItemId || (params.itemId === 0 && params.equippedItemId == null),
    }
  }

  function simulate(simulationParams: { type: SimulationType, itemIdsToSim?: number[] }) {
    if (simulationInProgress) { return; }
    const maxWorkers = window.navigator.hardwareConcurrency || 8; // Maximum amount of web workers that can be run concurrently.
    const simulations: SimWorker[] = [];
    const itemSlot: ItemSlot = ItemSlotKeyToItemSlot(false, uiState.selectedItemSlot, uiState.selectedItemSubSlot);
    const equippedItemId = playerState.selectedItems[itemSlot];
    let simulationsFinished = 0;
    let simulationsRunning = 0;
    let simIndex = 0;
    let dpsArray: number[] = [];
    let dpsCount: {[key: string]: number} = {};
    let combatLogBreakdownArr: CombatLogBreakdownData[] = [];
    let totalManaRegenerated = 0;
    let totalDamageDone = 0;
    let spellDamageDict: {[key: string]: number} = {};
    let spellManaGainDict: {[key: string]: number} = {};
    // Used to keep track of the progress % of sims for the progress bar.
    let simulationProgressPercentages: ISimulationProgressPercent[] = [];
    let simWorkerParameters: IGetWorkerParams[] = [];
    combatLogEntries = [];
    setSimulationInProgress(true);
    setSimulationType(simulationParams.type);
    if (simulationParams.type === SimulationType.AllItems) {
      dispatch(clearSavedItemSlotDps(itemSlot));
    } else if (simulationParams.type === SimulationType.StatWeights) {
      dispatch(setStatWeightVisibility(true));
    }

    if (simulationParams.type === SimulationType.StatWeights) {
      Object.entries(statWeightValues).forEach(statWeight => {
        simWorkerParameters.push({
          itemId: equippedItemId,
          equippedItemId: equippedItemId,
          simulationType: simulationParams.type,
          customStat: { stat: statWeight[0], value: statWeight[1] }
        });
      });
    } else if (simulationParams.itemIdsToSim) {
      simulationParams.itemIdsToSim.forEach(itemId => {
        simWorkerParameters.push({
          itemId: itemId,
          equippedItemId: equippedItemId,
          simulationType: simulationParams.type,
        });
      });
    }

    try {
      simWorkerParameters.forEach(simWorkerParameter => {
        simulationProgressPercentages.push({itemId: simWorkerParameter.itemId, progressPercent: 0, customStat: simWorkerParameter.customStat?.stat});
        simulations.push(new SimWorker(
          (dpsUpdate: {dps: number}) => {
            dpsArray.push(dpsUpdate.dps);
            const dps: string = Math.round(dpsUpdate.dps).toString();
            dpsCount[dps] = Math.round(dpsCount[dps]) + 1 || 1;
          },
          (combatLogVector: { name: string, damage: number, manaGain: number }) => {
            spellDamageDict[combatLogVector.name] = spellDamageDict[combatLogVector.name] + combatLogVector.damage || combatLogVector.damage;
            spellManaGainDict[combatLogVector.name] = spellManaGainDict[combatLogVector.name] + combatLogVector.manaGain || combatLogVector.manaGain;
            totalManaRegenerated += combatLogVector.manaGain;
            totalDamageDone += combatLogVector.damage;
          },
          (errorCallback: {errorMsg: string}) => {
            populateCombatLog();
            errorCallbackHandler(errorCallback);
          },
          (combatLogUpdate: {combatLogEntry: string}) => {
            combatLogEntries.push(combatLogUpdate.combatLogEntry);
          },
          (combatLogBreakdown: CombatLogBreakdownData) => {
            combatLogBreakdownArr.push(combatLogBreakdown);
          },
          (params: SimulationEnd) => {
            const newMedianDps = params.medianDps;
            simulationsFinished++;
            findSimulationProgressPercentObject({ simulationProgressPercentages: simulationProgressPercentages, simType: simulationParams.type, itemId: params.itemId, stat: params.customStat  })
              .progressPercent = 100;
  
            // Callback for the currently equipped item
            if (simulationParams.type === SimulationType.Normal || (simulationParams.type === SimulationType.AllItems && params.itemId === equippedItemId) || (simulationParams.type === SimulationType.StatWeights && params.customStat === 'normal')) {
              const newMinDps = Math.round(params.minDps * 100) / 100;
              const newMaxDps = Math.round(params.maxDps * 100) / 100;
              setNewMedianDps(newMedianDps.toString(), true);
              setNewMinDps(newMinDps.toString(), true);
              setNewMaxDps(newMaxDps.toString(), true);
            }

            if (simulationParams.type === SimulationType.StatWeights) {
              updateStatWeightValue(params.customStat, newMedianDps);
            }
  
            // All simulations finished
            if (simulationsFinished === simWorkerParameters.length) {
              setSimulationInProgress(false);
              const totalSimDuration = (performance.now() - startTime) / 1000;
              setNewSimulationDuration((Math.round(totalSimDuration * 10000) / 10000).toString(), true);
              setSimulationProgressPercent(0);

              // Either normal sim or multi-item sim
              if ([SimulationType.Normal, SimulationType.AllItems].includes(simulationParams.type)) {
                populateCombatLog();
                setSavedItemDpsValue(itemSlot, params.itemId, newMedianDps, true);
              }

              if (simulationParams.type === SimulationType.Normal) {
                setDpsStdev(Math.round(getStdev(dpsArray)).toString());
                dispatch(setHistogramData(dpsCount));

                if (playerState.settings["automatically-open-sim-details"] === 'yes') {
                  dispatch(setCombatLogBreakdownValue({
                    totalDamageDone: totalDamageDone,
                    totalManaGained: totalManaRegenerated,
                    totalSimulationFightLength: params.totalDuration,
                    totalIterationAmount: params.iterationAmount,
                    spellDamageDict: spellDamageDict,
                    spellManaGainDict: spellManaGainDict,
                    data: combatLogBreakdownArr,
                  }));
                  $('.breakdown-table').trigger('update');
                }
              } else if (simulationParams.type === SimulationType.StatWeights) {
                let talentTreePoints: {name: TalentTree, points: number}[] = [
                  { name: TalentTree.Affliction, points: getAllocatedTalentsPointsInTree(playerState.talents, Talents.find(e => e.name === TalentTree.Affliction)!) },
                  { name: TalentTree.Demonology, points: getAllocatedTalentsPointsInTree(playerState.talents, Talents.find(e => e.name === TalentTree.Demonology)!) },
                  { name: TalentTree.Destruction, points: getAllocatedTalentsPointsInTree(playerState.talents, Talents.find(e => e.name === TalentTree.Destruction)!) }
                ];
                // Gets the name of the talent tree with the most amount of points allocated by the player.
                const playerSpec = talentTreePoints.find(e => e.points === Math.max.apply(Math, talentTreePoints.map(a => a.points)))?.name;
  
                document.getElementById('pawn-import-string')!.innerHTML = `( Pawn: v1: "${uiState.selectedProfile || 'Warlock'}": Class=Warlock, Spec=${playerSpec}` +
                ', Stamina=' + $('#stat-weight-stamina').text() +
                ', Intellect=' + $('#stat-weight-intellect').text() +
                ', Spirit=' + $('#stat-weight-spirit').text() +
                ', SpellCritRating=' + $('#stat-weight-critRating').text() +
                ', SpellHitRating=' + $('#stat-weight-hitRating').text() +
                ', FireSpellDamage=' + $('#stat-weight-firePower').text() +
                ', ShadowSpellDamage=' + $('#stat-weight-shadowPower').text() +
                ', SpellDamage=' + $('#stat-weight-spellPower').text() +
                ', Mp5=' + $('#stat-weight-mp5').text() +
                ', SpellHasteRating=' + $('#stat-weight-hasteRating').text() + ')';
              }
            }
            // Multi-item sim
            else if (simulationParams.type === SimulationType.AllItems) {
              // Start a new simulation that's waiting in the queue if there are any remaining
              if (simulationsRunning - simulationsFinished < maxWorkers && simIndex < simulations.length) {
                simulations[simIndex++].start();
                simulationsRunning++;
              }
            }
          },
          (params: SimulationUpdate) => {
            let newMedianDps = params.medianDps;
            const simProgressPercent = Math.ceil((params.iteration / params.iterationAmount) * 100);
            findSimulationProgressPercentObject({ simulationProgressPercentages: simulationProgressPercentages, simType: simulationParams.type, itemId: params.itemId, stat: params.customStat  })
              .progressPercent = simProgressPercent;
            setSimulationProgressPercent(Math.round(average(simulationProgressPercentages.map(e => e.progressPercent))));
            // Only update the item table dps value for every 10% of progress because otherwise the simulation slows down too much.
            if (simulationParams.type === SimulationType.Normal || (simulationParams.type === SimulationType.AllItems && simProgressPercent % 10 === 0)) {
              const domElement = document.getElementById(params.itemId.toString());
              if (domElement) {
                domElement.innerHTML = (Math.round(newMedianDps * 100) / 100).toString();
                $('#item-selection-table').trigger('update');
              }
            }
            if (simulationParams.type === SimulationType.Normal || (simulationParams.type === SimulationType.AllItems && params.itemId ===  equippedItemId) || (simulationParams.type === SimulationType.StatWeights && params.customStat === 'normal')) {
              setNewMedianDps(newMedianDps.toString(), false);
            } else if (simulationParams.type === SimulationType.StatWeights) {
              updateStatWeightValue(params.customStat, params.medianDps);
            }
          },
          getWorkerParams({
            itemId: simWorkerParameter.itemId,
            equippedItemId: simWorkerParameter.equippedItemId,
            simulationType: simWorkerParameter.simulationType,
            customStat: simWorkerParameter.customStat,
          })
        ));
      });
  
      const startTime = performance.now();
      while ((simulationsRunning < maxWorkers || simulationParams.type === SimulationType.StatWeights) && simIndex < simulations.length) {
        simulations[simIndex++].start();
        simulationsRunning++;
      }
    } catch(error) {
      setSimulationInProgress(false);
      throw new Error("Error when trying to run simulation. " + error);
    }
  }

  function updateStatWeightValue(stat: string, value: number): void {
    const element = document.getElementById(`stat-weight-${stat}`);
    if (element) {
      let dpsDifference = Math.abs(Math.round(((value - Number(medianDps)) / statWeightValues[stat]) * 1000) / 1000);
      if (dpsDifference < 0.05) {
        dpsDifference = 0;
      }
      element.innerHTML = dpsDifference.toFixed(3).toString();
    }
  }

  function findSimulationProgressPercentObject(params: { simulationProgressPercentages: ISimulationProgressPercent[], simType: SimulationType, itemId: number, stat: string }): ISimulationProgressPercent {
    return params.simulationProgressPercentages
      .find(e => (e.itemId === params.itemId && params.simType !== SimulationType.StatWeights) || (e.customStat === params.stat))!
  }

  function setSavedItemDpsValue(itemSlot: ItemSlot, itemId: number, newMedianDps: number, saveToLocalStorage: boolean): void {
    dispatch(setSavedItemDps({ itemSlot: itemSlot, itemId: itemId, dps: newMedianDps, saveLocalStorage: saveToLocalStorage }));
    $('#item-selection-table').trigger('update');
  }

  function populateCombatLog(): void {
    dispatch(setCombatLogData(combatLogEntries));
  }

  function errorCallbackHandler(errorCallback: {errorMsg: string}): void {
    alert("Error: " + errorCallback.errorMsg + "\nPost in #tbc-sim-report on the TBC Warlock Discord or contact Kristofer#8003 on Discord.");
  }

  function setNewMedianDps(newMedianDps: string, savingLocalStorage: boolean) {
    setMedianDps(newMedianDps);
    if (savingLocalStorage) {
      localStorage.setItem('medianDps', newMedianDps);
    }
  }

  function setNewMinDps(newMinDps: string, savingLocalStorage: boolean) {
    setMinDps(newMinDps);
    if (savingLocalStorage) {
      localStorage.setItem('minDps', newMinDps);
    }
  }

  function setNewMaxDps(newMaxDps: string, savingLocalStorage: boolean) {
    setMaxDps(newMaxDps);
    if (savingLocalStorage) {
      localStorage.setItem('maxDps', newMaxDps);
    }
  }

  function setNewSimulationDuration(newSimulationDuration: string, savingLocalStorage: boolean) {
    setSimulationDuration(newSimulationDuration);
    if (savingLocalStorage) {
      localStorage.setItem('simulationDuration', newSimulationDuration);
    }
  }

  return(
    <>
      {
        medianDps.length > 0 &&
          <div id="sim-result-dps-div">
            <p><span id="median-dps">{Math.round(Number(medianDps) * 100) / 100}</span><span> DPS</span> <span id="dps-stdev">{dpsStdev.length > 0 ? '±' + dpsStdev : ''}</span></p>
            {
              maxDps.length > 0 && minDps.length > 0 &&
                <p>Min: <span id="min-dps">{minDps}</span> Max: <span id="max-dps">{maxDps}</span></p>
            }
          </div>
      }
      <div
        className='btn'
        onClick={() => simulate({ itemIdsToSim: [Items.find(e => e.id === playerState.selectedItems[ItemSlotKeyToItemSlot(false, uiState.selectedItemSlot, uiState.selectedItemSubSlot)])?.id || 0], type: SimulationType.Normal })}
        style={{background: simulationInProgress && simulationType === SimulationType.Normal ? `linear-gradient(to right, #9482C9 ${simulationProgressPercent}%, transparent ${simulationProgressPercent}%)` : ''}}
      >{simulationInProgress && simulationType === SimulationType.Normal ? `${simulationProgressPercent}%` : 'Simulate'}</div>
      <div
        className='btn'
        onClick={() => simulate({ itemIdsToSim: getItemTableItems(uiState.selectedItemSlot, uiState.selectedItemSubSlot, playerState.selectedItems, uiState.sources, uiState.hiddenItems, false).map(item => item.id), type: SimulationType.AllItems })}
        style={{background: simulationInProgress && simulationType === SimulationType.AllItems ? `linear-gradient(to right, #9482C9 ${simulationProgressPercent}%, transparent ${simulationProgressPercent}%)` : ''}}
      >{simulationInProgress && simulationType === SimulationType.AllItems ? `${simulationProgressPercent}%` : 'Simulate All Items'}</div>
      <div
        className='btn'
        onClick={() => simulate({ type: SimulationType.StatWeights })}
        style={{background: simulationInProgress && simulationType === SimulationType.StatWeights ? `linear-gradient(to right, #9482C9 ${simulationProgressPercent}%, transparent ${simulationProgressPercent}%)` : ''}}
      >{simulationInProgress && simulationType === SimulationType.StatWeights ? `${simulationProgressPercent}%` : 'Stat Weights'}</div>
      {
        uiState.combatLog.data.length > 0 &&
          <div className='btn' onClick={() => dispatch(setCombatLogVisibility(!uiState.combatLog.visible))}>Combat Log</div>
      }
      {
        uiState.histogram.data !== undefined &&
          <div className='btn' onClick={() => dispatch(setHistogramVisibility(!uiState.histogram.visible))}>Histogram</div>
      }
      <p id="sim-length-result">{simulationDuration.length > 0 ? simulationDuration + 's' : ''}</p>
    </>
  )
}