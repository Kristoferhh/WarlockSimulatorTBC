import { createSlice, PayloadAction } from "@reduxjs/toolkit";

function getRemainingTalentPoints(talents: TalentStore) {
  return 61 - Object.values<number>(talents).reduce((a, b) => a + b); // 61 available talent points at lvl 70
}

export interface TalentStore {
  [key: string]: number
}

export interface PlayerState {
  talents: TalentStore,
  talentPointsRemaining: number,
}

const initialPlayerState : PlayerState = {
  talents: JSON.parse(localStorage.getItem('talents') || '{}'),
  talentPointsRemaining: getRemainingTalentPoints(JSON.parse(localStorage.getItem('talents') || '{}'))
}

export const PlayerSlice = createSlice({
  name: 'player',
  initialState: initialPlayerState,
  reducers: {
    setTalentPointValue: (state, talent: PayloadAction<{name: string, points: number}>) => {
      state.talents[talent.payload.name] = talent.payload.points;
      state.talentPointsRemaining = getRemainingTalentPoints(state.talents);
      localStorage.setItem('talents', JSON.stringify(state.talents));
    },
  }
});

export const { setTalentPointValue }  = PlayerSlice.actions;
export default PlayerSlice.reducer;