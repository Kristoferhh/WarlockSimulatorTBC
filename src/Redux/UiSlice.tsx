import { createSlice, PayloadAction } from "@reduxjs/toolkit";
import { ItemSlotKey, Phase, UiState } from "../Types";

const initialUiState : UiState = {
  sources: JSON.parse(localStorage.getItem('sources') || JSON.stringify({ phases: { 0: true, 1: true, 2: true, 3: true, 4: true, 5: true, } })),
  gemSelectionTable: {visible: false, socketNumber: 0, itemSlot: ItemSlotKey.Mainhand},
  gemPreferences: JSON.parse(localStorage.getItem('gemPreferences') || JSON.stringify({hidden: [], favorites: []})),
}

export const UiSlice = createSlice({
  name: 'ui',
  initialState: initialUiState,
  reducers: {
    togglePhase: (state, action: PayloadAction<Phase>) => {
      state.sources.phases[action.payload] = !state.sources.phases[action.payload];
      localStorage.setItem('sources', JSON.stringify(state.sources));
    },
    setGemSelectionTableVisibility: (state, action: PayloadAction<boolean>) => {
      state.gemSelectionTable.visible = action.payload;
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
    }
  }
});

export const { togglePhase, setGemSelectionTableVisibility, favoriteGem, hideGem } = UiSlice.actions;
export default UiSlice.reducer;