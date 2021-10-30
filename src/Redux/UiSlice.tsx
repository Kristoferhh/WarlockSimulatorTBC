import { createSlice, PayloadAction } from "@reduxjs/toolkit";
import { GemSelectionTableStruct, InitialGemSelectionTableValue, ItemSlot, ItemSlotKey, Phase, SubSlotValue, UiState } from "../Types";

const initialUiState : UiState = {
  sources: JSON.parse(localStorage.getItem('sources') || JSON.stringify({ phase: { 0: true, 1: true, 2: true, 3: true, 4: true, 5: true, } })),
  gemSelectionTable: InitialGemSelectionTableValue,
  gemPreferences: JSON.parse(localStorage.getItem('gemPreferences') || JSON.stringify({hidden: [], favorites: []})),
  selectedProfile: localStorage.getItem('selectedProfile') || '',
  importExportWindowVisible: false,
  equippedItemsWindowVisible: false,
  fillItemSocketsWindowVisible: false,
  hiddenItems: JSON.parse(localStorage.getItem('hiddenItems') || JSON.stringify([])),
  selectedItemSlot: localStorage.getItem('selectedItemSlot') as ItemSlotKey || ItemSlotKey.Mainhand,
  selectedItemSubSlot: localStorage.getItem('selectedItemSubSlot') as SubSlotValue || '1',
  medianDps: localStorage.getItem('medianDps') || '',
  minDps: localStorage.getItem('minDps') || '',
  maxDps: localStorage.getItem('maxDps') || '',
  simulationProgressPercent: 0,
  simulationDuration: localStorage.getItem('simulationDuration') || '',
  savedItemDps: JSON.parse(localStorage.getItem('savedItemDps') || '{}'),
  combatLog: {visible: false, data: []},
}

export const UiSlice = createSlice({
  name: 'ui',
  initialState: initialUiState,
  reducers: {
    togglePhase: (state, action: PayloadAction<Phase>) => {
      state.sources.phase[action.payload] = !state.sources.phase[action.payload];
      localStorage.setItem('sources', JSON.stringify(state.sources));
    },
    setGemSelectionTable: (state, action: PayloadAction<GemSelectionTableStruct>) => {
      state.gemSelectionTable = action.payload;
    },
    favoriteGem: (state, action: PayloadAction<number>) => {
      if (state.gemPreferences.favorites.includes(action.payload)) {
        state.gemPreferences.favorites = state.gemPreferences.favorites.filter((e) => e !== action.payload);
      } else {
        state.gemPreferences.favorites.push(action.payload);
      }

      localStorage.setItem('gemPreferences', JSON.stringify(state.gemPreferences));
    },
    hideGem: (state, action: PayloadAction<number>) => {
      if (state.gemPreferences.hidden.includes(action.payload)) {
        state.gemPreferences.hidden = state.gemPreferences.hidden.filter((e) => e !== action.payload);
      } else {
        state.gemPreferences.hidden.push(action.payload);
      }

      localStorage.setItem('gemPreferences', JSON.stringify(state.gemPreferences));
    },
    setSelectedProfile: (state, action: PayloadAction<string>) => {
      state.selectedProfile = action.payload;
      localStorage.setItem('selectedProfile', action.payload);
    },
    setImportExportWindowVisibility: (state, action: PayloadAction<boolean>) => {
      state.importExportWindowVisible = action.payload;
    },
    setEquippedItemsWindowVisibility: (state, action: PayloadAction<boolean>) => {
      state.equippedItemsWindowVisible = action.payload;
    },
    setFillItemSocketsWindowVisibility: (state, action: PayloadAction<boolean>) => {
      state.fillItemSocketsWindowVisible = action.payload;
    },
    toggleHiddenItemId: (state, action: PayloadAction<number>) => {
      if (state.hiddenItems.includes(action.payload)) {
        state.hiddenItems = state.hiddenItems.filter((e) => e !== action.payload);
      } else {
        state.hiddenItems.push(action.payload);
      }

      localStorage.setItem('hiddenItems', JSON.stringify(state.hiddenItems));
    },
    setSelectedItemSlot: (state, action: PayloadAction<ItemSlotKey>) => {
      state.selectedItemSlot = action.payload;
      localStorage.setItem('selectedItemSlot', state.selectedItemSlot);
    },
    setSelectedItemSubSlot: (state, action: PayloadAction<SubSlotValue>) => {
      state.selectedItemSubSlot = action.payload;
      localStorage.setItem('selectedItemSubSlot', state.selectedItemSubSlot);
    },
    setMedianDps: (state, action: PayloadAction<string>) => {
      state.medianDps = action.payload;
      localStorage.setItem('medianDps', state.medianDps);
    },
    setMinDps: (state, action: PayloadAction<string>) => {
      state.minDps = action.payload;
      localStorage.setItem('minDps', state.minDps);
    },
    setMaxDps: (state, action: PayloadAction<string>) => {
      state.maxDps = action.payload;
      localStorage.setItem('maxDps', state.maxDps);
    },
    setSimulationProgressPercent: (state, action: PayloadAction<number>) => {
      state.simulationProgressPercent = action.payload;
    },
    setSimulationDuration: (state, action: PayloadAction<string>) => {
      state.simulationDuration = action.payload + 's';
      localStorage.setItem('simulationDuration', state.simulationDuration);
    },
    setSavedItemDps: (state, action: PayloadAction<{itemSlot: ItemSlot, itemId: number, dps: number, saveLocalStorage: boolean}>) => {
      if (!state.savedItemDps[action.payload.itemSlot])  {
        state.savedItemDps[action.payload.itemSlot] = {};
      }

      state.savedItemDps[action.payload.itemSlot][action.payload.itemId] = action.payload.dps;
      if (action.payload.saveLocalStorage) {
        localStorage.setItem('savedItemDps', JSON.stringify(state.savedItemDps));
      }
    },
    setCombatLogVisibility: (state, action: PayloadAction<boolean>) => {
      state.combatLog.visible = action.payload;
    },
    setCombatLogData: (state, action: PayloadAction<string[]>) => {
      state.combatLog.data = action.payload;
    }
  }
});

export const { setCombatLogData, setCombatLogVisibility, setSavedItemDps, setSimulationDuration, setMinDps, setMaxDps, setSimulationProgressPercent, setMedianDps, setSelectedItemSubSlot, setSelectedItemSlot, setFillItemSocketsWindowVisibility, setEquippedItemsWindowVisibility, toggleHiddenItemId, setImportExportWindowVisibility, setSelectedProfile, togglePhase, setGemSelectionTable, favoriteGem, hideGem } = UiSlice.actions;
export default UiSlice.reducer;