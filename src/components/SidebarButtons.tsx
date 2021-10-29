import { useDispatch, useSelector } from "react-redux"
import { ItemSlotKeyToItemSlot } from "../Common";
import { Items } from "../data/Items";
import { RootState } from "../redux/Store"
import { setMedianDps, setSimulationProgressPercent } from "../redux/UiSlice";
import { SimWorker } from "../SimWorker";
import { Item, Stat, WorkerParams } from "../Types";

interface SimulationUpdate {
  medianDps: number,
  iteration: number,
  iterationAmount: number,
  itemId: number,
  customStat: Stat
}

export function SidebarButtons() {
  const playerState = useSelector((state: RootState) => state.player);
  const uiState = useSelector((state: RootState) => state.ui);
  const dispatch = useDispatch();

  function getWorkerParams(itemId: number, itemAmount: number): WorkerParams {
    return {
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
  }

  function simulate(itemsToSim: Item[]) {
    const simWorkers = [];
    const equippedItemId = playerState.selectedItems[ItemSlotKeyToItemSlot(false, uiState.selectedItemSlot, uiState.selectedItemSubSlot)];

    itemsToSim.forEach(item => {
      simWorkers.push(new SimWorker(
        (dpsUpdate) => {

        },
        (combatLogVector) => {

        },
        (errorCallback) => {

        },
        (combatLogUpdate) => {

        },
        (combatLogBreakdown) => {

        },
        (simulationEnd) => {

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
  }

  return(
    <>
      <div
        className='btn'
        onClick={() => simulate(Items.filter(item => item.id === playerState.selectedItems[ItemSlotKeyToItemSlot(false, uiState.selectedItemSlot, uiState.selectedItemSubSlot)]))}
      >Simulate</div>
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