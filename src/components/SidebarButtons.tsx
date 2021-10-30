import { useDispatch, useSelector } from "react-redux"
import { ItemSlotKeyToItemSlot } from "../Common";
import { Items } from "../data/Items";
import { RootState } from "../redux/Store"
import { setMaxDps, setMedianDps, setMinDps, setSimulationDuration, setSimulationProgressPercent } from "../redux/UiSlice";
import { SimWorker } from "../SimWorker.js";
import { Item, Stat, WorkerParams } from "../Types";

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

  function getWorkerParams(itemId: number, itemAmount: number): WorkerParams {
    let params = {
      playerSettings: {
        auras: playerState.auras,
        items: playerState.selectedItems,
        enchants: playerState.selectedEnchants,
        gems: playerState.selectedGems,
        talents: playerState.talents,
        rotation: playerState.rotation,
        stats: playerState.stats,
        sets: playerState.sets,
        simSettings: playerState.settings,
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

    //todo remove/add stats here

    return params;
  }

  function simulate(itemsToSim: Item[]) {
    const maxWorkers = window.navigator.hardwareConcurrency || 8; // Maximum amount of web workers that can be run concurrently.
    const simulations: SimWorker[] = [];
    const equippedItemId = playerState.selectedItems[ItemSlotKeyToItemSlot(false, uiState.selectedItemSlot, uiState.selectedItemSubSlot)];
    let simulationsFinished = 0;
    let simulationsRunning = 0;
    let simIndex = 0;

    itemsToSim.forEach(item => {
      simulations.push(new SimWorker(
        (dpsUpdate: () => void) => {

        },
        (combatLogVector: () => void) => {

        },
        (errorCallback: () => void) => {

        },
        (combatLogUpdate: () => void) => {

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
          if (itemsToSim.length === 1 || params.itemId === equippedItemId) {
            dispatch(setMedianDps(medianDps.toString()));
            dispatch(setMinDps(minDps.toString()));
            dispatch(setMaxDps(maxDps.toString()));
          }

          // All simulations finished
          if (simulationsFinished === itemsToSim.length) {
            const totalSimDuration = (performance.now() - startTime) / 1000;
            dispatch(setSimulationDuration((Math.round(totalSimDuration * 10000) / 10000).toString()));
            dispatch(setSimulationProgressPercent(0));
          }

          // Start a new simulation that's waiting in the queue if there are any remaining
          if (simulationsRunning - simulationsFinished < maxWorkers && simIndex < simulations.length) {
            simulations[simIndex++].start();
            simulationsRunning++;
          }
        },
        (params: SimulationUpdate) => {
          let medianDps = Math.round(params.medianDps * 100) / 100;

          if (itemsToSim.length === 1 || params.itemId ===  equippedItemId) {
            dispatch(setMedianDps(medianDps.toString()));
          }

          if (itemsToSim.length === 1) {
            // Update the simulation's progress %
            dispatch(setSimulationProgressPercent(Math.ceil((params.iteration / params.iterationAmount) * 100)));
          }
        },
        getWorkerParams(item.id, itemsToSim.length)
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
        onClick={() => simulate(Items.filter(item => item.id === playerState.selectedItems[ItemSlotKeyToItemSlot(false, uiState.selectedItemSlot, uiState.selectedItemSubSlot)]))}
        style={{background: uiState.simulationProgressPercent > 0 ? `linear-gradient(to right, #9482C9 ${uiState.simulationProgressPercent}%, transparent ${uiState.simulationProgressPercent}%)` : ''}}
      >{uiState.simulationProgressPercent > 0 ? `${uiState.simulationProgressPercent}%` : 'Simulate'}</div>
      <div
        className='btn'
        onClick={() => simulate(Items.filter(item => item.itemSlot === uiState.selectedItemSlot))}
      >Simulate All Items</div>
      <div className='btn'>Stat Weights</div>
      <div className='btn'>Combat Log</div>
      <div className='btn'>Histogram</div>
    </>
  )
}