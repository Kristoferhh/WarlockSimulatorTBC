import { createSlice, PayloadAction } from "@reduxjs/toolkit";
import { Aura, AuraGroup, ItemSlot, RotationGroup, SubSlotValue, Spell } from "./Types";

export interface TalentStore {
  [key: string]: number
}

type ItemAndEnchantStruct = {[key: string]: number}

export interface PlayerState {
  talents: TalentStore,
  talentPointsRemaining: number,
  selectedItems: ItemAndEnchantStruct,
  selectedEnchants: ItemAndEnchantStruct,
  auras: {[key: string]: boolean},
  rotation: {[key: string]: {[key: string]: boolean}}
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
  auras: JSON.parse(localStorage.getItem('auras') || '{}'),
  rotation: JSON.parse(localStorage.getItem('rotation') || '{}'),
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
    toggleAuraSelection: (state, action: PayloadAction<{auraGroup: AuraGroup, aura: Aura}>) => {
      const isAuraDisabled = state.auras[action.payload.aura.varName] == null || state.auras[action.payload.aura.varName] === false;

      // If the aura is being toggled on and it's a unique buff like potion/food buff, then disable all other auras with that unique property as true.
      if (isAuraDisabled) {
        if (action.payload.aura.potion)         action.payload.auraGroup.auras.filter((e) => e.potion).forEach((e)          => state.auras[e.varName] = false);
        if (action.payload.aura.foodBuff)       action.payload.auraGroup.auras.filter((e) => e.foodBuff).forEach((e)        => state.auras[e.varName] = false);
        if (action.payload.aura.weaponOil)      action.payload.auraGroup.auras.filter((e) => e.weaponOil).forEach((e)       => state.auras[e.varName] = false);
        if (action.payload.aura.battleElixir)   action.payload.auraGroup.auras.filter((e) => e.battleElixir).forEach((e)    => state.auras[e.varName] = false);
        if (action.payload.aura.guardianElixir) action.payload.auraGroup.auras.filter((e) => e.guardianElixir).forEach((e)  => state.auras[e.varName] = false);
        if (action.payload.aura.alcohol)        action.payload.auraGroup.auras.filter((e) => e.alcohol).forEach((e)         => state.auras[e.varName] = false);
        if (action.payload.aura.demonicRune)    action.payload.auraGroup.auras.filter((e) => e.demonicRune).forEach((e)     => state.auras[e.varName] = false);
        if (action.payload.aura.drums)          action.payload.auraGroup.auras.filter((e) => e.drums).forEach((e)           => state.auras[e.varName] = false);
      }

      // Toggle the aura's bool property/initialize to true.
      state.auras[action.payload.aura.varName] = state.auras[action.payload.aura.varName] == null ? true : !state.auras[action.payload.aura.varName];
      localStorage.setItem('auras', JSON.stringify(state.auras));
    },
    toggleRotationSpellSelection: (state, action: PayloadAction<{rotationGroup: RotationGroup, spell: Spell}>) => {
      if (state.rotation[action.payload.rotationGroup.varName] == null) {
        state.rotation[action.payload.rotationGroup.varName] = {};
      }
      
      const isSpellDisabled = state.rotation[action.payload.rotationGroup.varName][action.payload.spell.varName] == null || state.rotation[action.payload.rotationGroup.varName][action.payload.spell.varName] === false;
      // If a filler/curse is being enabled then disable all other curses/fillers
      if (isSpellDisabled) {
        if (['filler', 'curse'].includes(action.payload.rotationGroup.varName)) {
          action.payload.rotationGroup.spells.forEach((e) => state.rotation[action.payload.rotationGroup.varName][e.varName] = false);
        }
      }
      if (state.rotation[action.payload.rotationGroup.varName][action.payload.spell.varName] == null) {
        state.rotation[action.payload.rotationGroup.varName][action.payload.spell.varName] = true;
      } else {
        state.rotation[action.payload.rotationGroup.varName][action.payload.spell.varName] = !state.rotation[action.payload.rotationGroup.varName][action.payload.spell.varName];
      }

      localStorage.setItem('rotation', JSON.stringify(state.rotation));
    }
  }
});

export const { setTalentPointValue, setItemInItemSlot, setEnchantInItemSlot, toggleAuraSelection, toggleRotationSpellSelection }  = PlayerSlice.actions;
export default PlayerSlice.reducer;