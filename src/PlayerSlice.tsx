import { createSlice, PayloadAction } from "@reduxjs/toolkit";
import { ItemSlot, SubSlotValue } from "./Types";

export interface TalentStore {
  [key: string]: number
}

type ItemAndEnchantStruct = {[key: string]: number}

export interface PlayerState {
  talents: TalentStore,
  talentPointsRemaining: number,
  selectedItems: ItemAndEnchantStruct,
  selectedEnchants: ItemAndEnchantStruct,
}

const initialSelectedItemsAndEnchants: ItemAndEnchantStruct = {
  head: 0,
  neck: 0,
  shoulders: 0,
  back: 0,
  chest: 0,
  bracer: 0,
  gloves: 0,
  belt: 0,
  legs: 0,
  boots: 0,
  ring1: 0,
  ring2: 0,
  trinket1: 0,
  trinket2: 0,
  mainhand: 0,
  offhand: 0,
  twohand: 0,
  wand: 0
}

const initialPlayerState : PlayerState = {
  talents: JSON.parse(localStorage.getItem('talents') || '{}'),
  talentPointsRemaining: getRemainingTalentPoints(JSON.parse(localStorage.getItem('talents') || '{}')),
  selectedItems: JSON.parse(localStorage.getItem('selectedItems') || JSON.stringify(initialSelectedItemsAndEnchants)),
  selectedEnchants: JSON.parse(localStorage.getItem('selectedEnchants') || JSON.stringify(initialSelectedItemsAndEnchants)),
}

function getRemainingTalentPoints(talents: TalentStore) {
  return 61 - Object.values<number>(talents).reduce((a, b) => a + b); // 61 available talent points at lvl 70
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
    setItemInItemSlot: (state, item: PayloadAction<{id: number, itemSlot: ItemSlot, subSlot: SubSlotValue}>) => {
      state.selectedItems[item.payload.itemSlot + item.payload.subSlot] = item.payload.id;
      localStorage.setItem('selectedItems', JSON.stringify(state.selectedItems));
    },
    setEnchantInItemSlot: (state, item: PayloadAction<{id: number, itemSlot: ItemSlot, subSlot: SubSlotValue}>) => {
      state.selectedEnchants[item.payload.itemSlot + item.payload.subSlot] = item.payload.id;
      localStorage.setItem('selectedEnchants', JSON.stringify(state.selectedEnchants));
    },
  }
});

export const { setTalentPointValue, setItemInItemSlot, setEnchantInItemSlot }  = PlayerSlice.actions;
export default PlayerSlice.reducer;