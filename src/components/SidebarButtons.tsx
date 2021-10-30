import { useDispatch, useSelector } from "react-redux"
import { getItemTableItems, itemMeetsSocketRequirements, ItemSlotKeyToItemSlot } from "../Common";
import { Gems } from "../data/Gems";
import { Items } from "../data/Items";
import { RootState } from "../redux/Store"
import { setCombatLogData, setCombatLogVisibility, setMaxDps, setMedianDps, setMinDps, setSavedItemDps, setSimulationDuration, setSimulationProgressPercent } from "../redux/UiSlice";
import { SimWorker } from "../SimWorker.js";
import { GemColor, ItemSlot, Stat, WorkerParams } from "../Types";

interface SimulationUpdate {
  medianDps: number,
  iteration: number,
  iterationAmount: number,
  itemId: number,
  customStat: Stat
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

export function SidebarButtons() {
  const playerState = useSelector((state: RootState) => state.player);
  const uiState = useSelector((state: RootState) => state.ui);
  const dispatch = useDispatch();
  let combatLogEntries: string[] = [];

  function getWorkerParams(itemId: number, itemAmount: number): WorkerParams {
    let params = {
      playerSettings: {
        auras: playerState.auras,
        items: JSON.parse(JSON.stringify(playerState.selectedItems)),
        enchants: playerState.selectedEnchants,
        gems: playerState.selectedGems,
        talents: playerState.talents,
        rotation: playerState.rotation,
        stats: JSON.parse(JSON.stringify(playerState.stats)),
        sets: JSON.parse(JSON.stringify(playerState.sets)),
        simSettings: playerState.settings,
        metaGemId: 0,
      },
      simulationSettings: {
        iterations: parseInt(playerState.settings.iterations),
        minTime: parseInt(playerState.settings["min-fight-length"]),
        maxTime: parseInt(playerState.settings['max-fight-length'])
      },
      itemId: itemId,
      itemAmount: itemAmount,
      itemSubSlot: uiState.selectedItemSubSlot
    }
    let equippedMetaGemId = -1;
    let customItemMetaGemId = -1;

    // Get the equipped meta gem id
    if (params.playerSettings.items.head !== 0 && params.playerSettings.gems.head[params.playerSettings.items.head]) {
      params.playerSettings.gems.head[params.playerSettings.items.head].forEach(gemArray => {
        const gem = Gems.find(e => e.id === gemArray[1]);
        if (gem && gem.color === GemColor.Meta) {
          equippedMetaGemId = gem.id;
        }
      });
    }

    // Check if the parameter item is not the equipped item. If so then unequip the equipped item.
    const equippedItemId = params.playerSettings.items[ItemSlotKeyToItemSlot(false, uiState.selectedItemSlot, uiState.selectedItemSubSlot)];
    const equippedItem = Items.find(e => e.id === equippedItemId);
    if (equippedItem && equippedItemId !== itemId) {
      // Remove item stats
      for (const [stat, value] of Object.entries(equippedItem)) {
        if (params.playerSettings.stats.hasOwnProperty(stat)) {
          params.playerSettings.stats[stat as Stat] -= value;
        }
      }

      // Remove stats from socket bonus if it's active
      if (itemMeetsSocketRequirements({itemId: equippedItem.id, selectedGems: params.playerSettings.gems}) && equippedItem.socketBonus) {
        for (const [stat, value] of Object.entries(equippedItem.socketBonus)) {
          params.playerSettings.stats[stat] -= value;
        }
      }

      // Lower the item set counter by 1 if the item is part of a set
      if (equippedItem.setId && params.playerSettings.sets[equippedItem.setId] && params.playerSettings.sets[equippedItem.setId] > 0) {
        params.playerSettings.sets[equippedItem.setId] -= 1;
      }
    }

    // Equip the new item if it's not the same as the equipped item.
    const customItem = Items.find(e => e.id === itemId);
    if (customItem && itemId !== equippedItemId) {
      // Add stats from new item
      for (const [stat, value] of Object.entries(itemId)) {
        if (params.playerSettings.stats.hasOwnProperty(stat)) {
          params.playerSettings.stats[stat] -= value;
        }
      }

      // Socket bonus
      if (itemMeetsSocketRequirements({itemId: itemId, selectedGems: params.playerSettings.gems})) {
        for (const [stat, value] of Object.entries(customItem.socketBonus!)) {
          params.playerSettings.stats[stat] -= value;
        }
      }

      // Increment set id counter
      if (customItem.setId) {
        params.playerSettings.sets[customItem.setId] += 1;
      }
    }

    return params;
  }

  function simulate(itemIdsToSim: number[]) {
    const maxWorkers = window.navigator.hardwareConcurrency || 8; // Maximum amount of web workers that can be run concurrently.
    const simulations: SimWorker[] = [];
    const itemSlot: ItemSlot = ItemSlotKeyToItemSlot(false, uiState.selectedItemSlot, uiState.selectedItemSubSlot);
    const equippedItemId = playerState.selectedItems[itemSlot];
    let simulationsFinished = 0;
    let simulationsRunning = 0;
    let simIndex = 0;
    combatLogEntries = [];

    itemIdsToSim.forEach(itemId => {
      simulations.push(new SimWorker(
        (dpsUpdate: () => void) => {

        },
        (combatLogVector: () => void) => {

        },
        (errorCallback: () => void) => {

        },
        (combatLogUpdate: {combatLogEntry: string}) => {
          combatLogEntries.push(combatLogUpdate.combatLogEntry);
        },
        (combatLogBreakdown: () => void) => {

        },
        (params: SimulationEnd) => {
          console.log(params);
          const minDps = Math.round(params.minDps * 100) / 100;
          const maxDps = Math.round(params.maxDps * 100) / 100;
          const medianDps = Math.round(params.medianDps * 100) / 100;
          simulationsFinished++;

          // Callback for the currently equipped item
          if (itemIdsToSim.length === 1 || params.itemId === equippedItemId) {
            dispatch(setMedianDps(medianDps.toString()));
            dispatch(setMinDps(minDps.toString()));
            dispatch(setMaxDps(maxDps.toString()));
          }

          // All simulations finished
          if (simulationsFinished === itemIdsToSim.length) {
            const totalSimDuration = (performance.now() - startTime) / 1000;
            dispatch(setSimulationDuration((Math.round(totalSimDuration * 10000) / 10000).toString()));
            dispatch(setSimulationProgressPercent(0));
            dispatch(setCombatLogData(combatLogEntries));
          }

          // Start a new simulation that's waiting in the queue if there are any remaining
          if (simulationsRunning - simulationsFinished < maxWorkers && simIndex < simulations.length) {
            simulations[simIndex++].start();
            simulationsRunning++;
          }

          dispatch(setSavedItemDps({ itemSlot: itemSlot, itemId: params.itemId, dps: medianDps, saveLocalStorage: true }));
        },
        (params: SimulationUpdate) => {
          let medianDps = Math.round(params.medianDps * 100) / 100;

          if (itemIdsToSim.length === 1 || params.itemId ===  equippedItemId) {
            dispatch(setMedianDps(medianDps.toString()));
          }

          if (itemIdsToSim.length === 1) {
            // Update the simulation's progress %
            dispatch(setSimulationProgressPercent(Math.ceil((params.iteration / params.iterationAmount) * 100)));
          }

          dispatch(setSavedItemDps({ itemSlot: itemSlot, itemId: params.itemId, dps: medianDps, saveLocalStorage: false }));
        },
        getWorkerParams(itemId, itemIdsToSim.length)
      ));
    });

    const startTime = performance.now();
    while (simulationsRunning < maxWorkers && simIndex < simulations.length) {
      simulations[simIndex++].start();
      simulationsRunning++;
    }
  }

  return(
    <>
      <div
        className='btn'
        onClick={() => simulate([Items.find(e => e.id === playerState.selectedItems[ItemSlotKeyToItemSlot(false, uiState.selectedItemSlot, uiState.selectedItemSubSlot)])?.id || 0])}
        style={{background: uiState.simulationProgressPercent > 0 ? `linear-gradient(to right, #9482C9 ${uiState.simulationProgressPercent}%, transparent ${uiState.simulationProgressPercent}%)` : ''}}
      >{uiState.simulationProgressPercent > 0 ? `${uiState.simulationProgressPercent}%` : 'Simulate'}</div>
      <div
        className='btn'
        onClick={() => simulate(getItemTableItems(uiState.selectedItemSlot, uiState.selectedItemSubSlot, playerState.selectedItems, uiState.sources, uiState.hiddenItems, false).map(item => item.id))}
      >Simulate All Items</div>
      <div className='btn'>Stat Weights</div>
      <div className='btn' onClick={() => dispatch(setCombatLogVisibility(!uiState.combatLog.visible))}>Combat Log</div>
      <div className='btn'>Histogram</div>
    </>
  )
}