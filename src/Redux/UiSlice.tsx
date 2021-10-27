import { createSlice, PayloadAction } from "@reduxjs/toolkit";
import { GemSelectionTableStruct, InitialGemSelectionTableValue, Phase, UiState } from "../Types";

const initialUiState : UiState = {
  sources: JSON.parse(localStorage.getItem('sources') || JSON.stringify({ phase: { 0: true, 1: true, 2: true, 3: true, 4: true, 5: true, } })),
  gemSelectionTable: InitialGemSelectionTableValue,
  gemPreferences: JSON.parse(localStorage.getItem('gemPreferences') || JSON.stringify({hidden: [], favorites: []})),
  selectedProfile: localStorage.getItem('selectedProfile') || '',
  importExportWindowVisible: false,
  hiddenItems: JSON.parse(localStorage.getItem('hiddenItems') || JSON.stringify([])),
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
    toggleHiddenItemId: (state, action: PayloadAction<number>) => {
      if (state.hiddenItems.includes(action.payload)) {
        state.hiddenItems = state.hiddenItems.filter((e) => e !== action.payload);
      } else {
        state.hiddenItems.push(action.payload);
      }

      localStorage.setItem('hiddenItems', JSON.stringify(state.hiddenItems));
    }
  }
});

export const { toggleHiddenItemId, setImportExportWindowVisibility, setSelectedProfile, togglePhase, setGemSelectionTable, favoriteGem, hideGem } = UiSlice.actions;
export default UiSlice.reducer;