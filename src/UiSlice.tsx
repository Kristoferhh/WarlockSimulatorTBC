import { createSlice, PayloadAction } from "@reduxjs/toolkit";
import { Phase, UiState } from "./Types";

const initialUiState : UiState = {
  sources: JSON.parse(localStorage.getItem('sources') || JSON.stringify({ phases: { 0: true, 1: true, 2: true, 3: true, 4: true, 5: true, } })),
}

export const UiSlice = createSlice({
  name: 'ui',
  initialState: initialUiState,
  reducers: {
    togglePhase: (state, action: PayloadAction<Phase>) => {
      state.sources.phases[action.payload] = !state.sources.phases[action.payload];
      localStorage.setItem('sources', JSON.stringify(state.sources));
    }
  }
});

export const { togglePhase } = UiSlice.actions;
export default UiSlice.reducer;