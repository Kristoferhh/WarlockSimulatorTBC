import { createSlice, PayloadAction } from "@reduxjs/toolkit";
import { getRemainingTalentPoints } from "../Common";
import { Auras } from "../data/Auras";
import { Aura, RotationGroup, Spell, PlayerState, InitialPlayerStats, InitialSelectedItemsAndEnchants, InitialSettings, Stat, ItemSlot, Profile, InitialSelectedGems, ItemSlotKey, TalentName, Setting, ItemSet } from "../Types";

const initialPlayerState : PlayerState = {
  talents: JSON.parse(localStorage.getItem('talents') || '{}'),
  talentPointsRemaining: getRemainingTalentPoints(JSON.parse(localStorage.getItem('talents') || '{}')),
  selectedItems: JSON.parse(localStorage.getItem('selectedItems') || JSON.stringify(InitialSelectedItemsAndEnchants)),
  selectedEnchants: JSON.parse(localStorage.getItem('selectedEnchants') || JSON.stringify(InitialSelectedItemsAndEnchants)),
  selectedGems: JSON.parse(localStorage.getItem('selectedGems') || JSON.stringify(InitialSelectedGems)),
  auras: JSON.parse(localStorage.getItem('auras') || '{}'),
  rotation: JSON.parse(localStorage.getItem('rotation') || '{}'),
  stats: InitialPlayerStats,
  settings: JSON.parse(localStorage.getItem('settings') || JSON.stringify(InitialSettings)),
  profiles: JSON.parse(localStorage.getItem('profiles') || '{}'),
  sets: {'529': 0, '552': 0, '553': 0, '559': 0, '568': 0, '592': 0, '615': 0, '644': 0, '645': 0, '646': 0, '647': 0, '658': 0, '667': 0, '670': 0, '702': 0, '704': 0, '734': 0, '735': 0},
}

export const PlayerSlice = createSlice({
  name: 'player',
  initialState: initialPlayerState,
  reducers: {
    setTalentPointValue: (state, talent: PayloadAction<{name: TalentName, points: number}>) => {
      state.talents[talent.payload.name] = talent.payload.points;
      state.talentPointsRemaining = getRemainingTalentPoints(state.talents);
      localStorage.setItem('talents', JSON.stringify(state.talents));
    },
    setItemInItemSlot: (state, item: PayloadAction<{id: number, itemSlot: ItemSlot}>) => {
      if (state.selectedItems[item.payload.itemSlot] === item.payload.id) {
        state.selectedItems[item.payload.itemSlot] = 0;
      } else {
        state.selectedItems[item.payload.itemSlot] = item.payload.id;        
      }

      localStorage.setItem('selectedItems', JSON.stringify(state.selectedItems));
    },
    setEnchantInItemSlot: (state, item: PayloadAction<{id: number, itemSlot: ItemSlot}>) => {
      if (state.selectedEnchants[item.payload.itemSlot] === item.payload.id) {
        state.selectedEnchants[item.payload.itemSlot] = 0;
      } else {
        state.selectedEnchants[item.payload.itemSlot] = item.payload.id;
      }

      localStorage.setItem('selectedEnchants', JSON.stringify(state.selectedEnchants));
    },
    toggleAuraSelection: (state, action: PayloadAction<Aura>) => {
      const isAuraDisabled = state.auras[action.payload.varName] == null || state.auras[action.payload.varName] === false;

      // If the aura is being toggled on and it's a unique buff like potion/food buff, then disable all other auras with that unique property as true.
      if (isAuraDisabled) {
        if (action.payload.potion)         Auras.filter((e) => e.potion).forEach((e)         => state.auras[e.varName] = false);
        if (action.payload.foodBuff)       Auras.filter((e) => e.foodBuff).forEach((e)       => state.auras[e.varName] = false);
        if (action.payload.weaponOil)      Auras.filter((e) => e.weaponOil).forEach((e)      => state.auras[e.varName] = false);
        if (action.payload.battleElixir)   Auras.filter((e) => e.battleElixir).forEach((e)   => state.auras[e.varName] = false);
        if (action.payload.guardianElixir) Auras.filter((e) => e.guardianElixir).forEach((e) => state.auras[e.varName] = false);
        if (action.payload.alcohol)        Auras.filter((e) => e.alcohol).forEach((e)        => state.auras[e.varName] = false);
        if (action.payload.demonicRune)    Auras.filter((e) => e.demonicRune).forEach((e)    => state.auras[e.varName] = false);
        if (action.payload.drums)          Auras.filter((e) => e.drums).forEach((e)          => state.auras[e.varName] = false);
      }

      // Toggle the aura's bool property/initialize to true.
      state.auras[action.payload.varName] = state.auras[action.payload.varName] == null ? true : !state.auras[action.payload.varName];
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
    },
    modifyPlayerStat: (state, action: PayloadAction<{stat: Stat, value: number, action: 'add'|'remove'}>) => {
      if (action.payload.action === 'add') {
        if (action.payload.stat.includes('Modifier')) {
          state.stats[action.payload.stat] *= action.payload.value;
        } else {
          state.stats[action.payload.stat] += action.payload.value;
        }
      } else if (action.payload.action === 'remove') {
        if (action.payload.stat.includes('Modifier')) {
          state.stats[action.payload.stat] /= action.payload.value;
        } else {
          state.stats[action.payload.stat] -= action.payload.value;
        }
      }
    },
    modifySettingValue: (state, action: PayloadAction<{setting: Setting, value: string}>) => {
      state.settings[action.payload.setting] = action.payload.value;
      localStorage.setItem('settings', JSON.stringify(state.settings));
    },
    setProfile: (state, action: PayloadAction<{profile: Profile, name: string}>) => {
      state.profiles[action.payload.name] = action.payload.profile;
      localStorage.setItem('profiles', JSON.stringify(state.profiles));
    },
    loadProfile: (state, action: PayloadAction<Profile>) => {
      localStorage.setItem('auras', JSON.stringify(action.payload.auras));
      localStorage.setItem('rotation', JSON.stringify(action.payload.rotation));
      localStorage.setItem('selectedItems', JSON.stringify(action.payload.items));
      localStorage.setItem('selectedGems', JSON.stringify(action.payload.gems));
      localStorage.setItem('selectedEnchants', JSON.stringify(action.payload.enchants));
      localStorage.setItem('settings', JSON.stringify(action.payload.simSettings));
      localStorage.setItem('talents', JSON.stringify(action.payload.talents));
      window.location.reload();
    },
    setItemSocketsValue: (state, action: PayloadAction<{itemId: string, itemSlot: ItemSlotKey, value: [string, number][]}>) => {
      state.selectedGems[action.payload.itemSlot][action.payload.itemId] = action.payload.value;
      localStorage.setItem('selectedGems', JSON.stringify(state.selectedGems));
    },
    setItemSetCount: (state, action: PayloadAction<{setId: ItemSet, count: number}>) => {
      state.sets[action.payload.setId] = action.payload.count;
    },
    deleteProfile: (state, action: PayloadAction<string>) => {
      if (state.profiles[action.payload]) {
        delete state.profiles[action.payload];
        localStorage.setItem('profiles', JSON.stringify(state.profiles));
        localStorage.removeItem('selectedProfile');
      }
    },
    renameProfile: (state, action: PayloadAction<{oldName: string, newName: string}>) => {
      if (state.profiles[action.payload.oldName]) {
        // Create a copy of the old profile with the new name and delete the old profile.
        state.profiles[action.payload.newName] = state.profiles[action.payload.oldName];
        delete state.profiles[action.payload.oldName];
        localStorage.setItem('profiles', JSON.stringify(state.profiles));
        localStorage.setItem('selectedProfile', action.payload.newName);
      }
    }
  }
});

export const { renameProfile, deleteProfile, setItemSetCount, loadProfile, setItemSocketsValue, setTalentPointValue, setItemInItemSlot, setEnchantInItemSlot, toggleAuraSelection, toggleRotationSpellSelection, modifyPlayerStat, modifySettingValue, setProfile } = PlayerSlice.actions;
export default PlayerSlice.reducer;