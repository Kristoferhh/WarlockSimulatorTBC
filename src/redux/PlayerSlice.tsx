import { createSlice, PayloadAction } from '@reduxjs/toolkit'
import { getRemainingTalentPoints } from '../Common'
import { Spells } from '../data/Spells'
import {
  RotationGroup,
  Spell,
  PlayerState,
  InitialPlayerStats,
  InitialSelectedItemsAndEnchants,
  InitialSettings,
  Profile,
  InitialSelectedGems,
  TalentName,
  Setting,
  StatsCollection,
  InitialSetCounts,
  SetsStruct,
  AurasStruct,
  ItemAndEnchantStruct,
  SelectedGemsStruct,
  TalentStore,
  RotationStruct,
  Settings,
  rotationGroups,
} from '../Types'

const initialPlayerState: PlayerState = {
  talents: JSON.parse(localStorage.getItem('talents') || '{}'),
  talentPointsRemaining: getRemainingTalentPoints(
    JSON.parse(localStorage.getItem('talents') || '{}')
  ),
  selectedItems: JSON.parse(
    localStorage.getItem('selectedItems') ||
      JSON.stringify(InitialSelectedItemsAndEnchants)
  ),
  selectedEnchants: JSON.parse(
    localStorage.getItem('selectedEnchants') ||
      JSON.stringify(InitialSelectedItemsAndEnchants)
  ),
  selectedGems: JSON.parse(
    localStorage.getItem('selectedGems') || JSON.stringify(InitialSelectedGems)
  ),
  auras: JSON.parse(localStorage.getItem('auras') || '{}'),
  rotation: JSON.parse(
    localStorage.getItem('rotation') ||
      JSON.stringify({ filler: { shadowBolt: true } })
  ),
  stats: {
    base: InitialPlayerStats,
    auras: InitialPlayerStats,
    items: InitialPlayerStats,
    gems: InitialPlayerStats,
    enchants: InitialPlayerStats,
  },
  settings: JSON.parse(
    localStorage.getItem('settings') || JSON.stringify(InitialSettings)
  ),
  profiles: JSON.parse(localStorage.getItem('profiles') || '{}'),
  sets: InitialSetCounts,
}

export const PlayerSlice = createSlice({
  name: 'player',
  initialState: initialPlayerState,
  reducers: {
    setTalentPointValue: (
      state,
      talent: PayloadAction<{ name: TalentName; points: number }>
    ) => {
      state.talents[talent.payload.name] = talent.payload.points
      state.talentPointsRemaining = getRemainingTalentPoints(state.talents)
      localStorage.setItem('talents', JSON.stringify(state.talents))
    },
    setSelectedItems: (state, action: PayloadAction<ItemAndEnchantStruct>) => {
      state.selectedItems = action.payload
      localStorage.setItem('selectedItems', JSON.stringify(state.selectedItems))
    },
    setSelectedEnchants: (
      state,
      action: PayloadAction<ItemAndEnchantStruct>
    ) => {
      state.selectedEnchants = action.payload
      localStorage.setItem(
        'selectedEnchants',
        JSON.stringify(state.selectedEnchants)
      )
    },
    setSelectedAuras: (state, action: PayloadAction<AurasStruct>) => {
      state.auras = action.payload
      localStorage.setItem('auras', JSON.stringify(state.auras))
    },
    toggleRotationSpellSelection: (state, action: PayloadAction<Spell>) => {
      const rotationGroup = rotationGroups.find(
        e => e.header === action.payload.group
      )
      if (!rotationGroup) {
        return
      }
      if (state.rotation[rotationGroup.varName] == null) {
        state.rotation[rotationGroup.varName] = {}
      }
      const spellState =
        state.rotation[rotationGroup.varName][action.payload.varName]
      const isSpellDisabled = spellState == null || spellState === false
      // If a filler/curse is being enabled then disable all other curses/fillers
      if (isSpellDisabled) {
        if (
          [RotationGroup.Filler, RotationGroup.Curse].includes(
            rotationGroup.header
          )
        ) {
          Spells.filter(e => e.group === action.payload.group).forEach(e => {
            state.rotation[rotationGroup.varName][e.varName] = false
          })
        }
      }
      state.rotation[rotationGroup.varName][action.payload.varName] =
        spellState === null
          ? true
          : !state.rotation[rotationGroup.varName][action.payload.varName]
      localStorage.setItem('rotation', JSON.stringify(state.rotation))
    },
    setBaseStats: (state, action: PayloadAction<StatsCollection>) => {
      state.stats.base = action.payload
    },
    setAurasStats: (state, action: PayloadAction<StatsCollection>) => {
      state.stats.auras = action.payload
    },
    setItemsStats: (state, action: PayloadAction<StatsCollection>) => {
      state.stats.items = action.payload
    },
    setGemsStats: (state, action: PayloadAction<StatsCollection>) => {
      state.stats.gems = action.payload
    },
    setEnchantsStats: (state, action: PayloadAction<StatsCollection>) => {
      state.stats.enchants = action.payload
    },
    setItemSetCounts: (state, action: PayloadAction<SetsStruct>) => {
      state.sets = action.payload
    },
    modifySettingValue: (
      state,
      action: PayloadAction<{ setting: Setting; value: string }>
    ) => {
      state.settings[action.payload.setting] = action.payload.value
      localStorage.setItem('settings', JSON.stringify(state.settings))
    },
    setProfile: (
      state,
      action: PayloadAction<{ profile: Profile; name: string }>
    ) => {
      state.profiles[action.payload.name] = action.payload.profile
      localStorage.setItem('profiles', JSON.stringify(state.profiles))
    },
    setSelectedGems: (state, action: PayloadAction<SelectedGemsStruct>) => {
      state.selectedGems = action.payload
      localStorage.setItem('selectedGems', JSON.stringify(state.selectedGems))
    },
    deleteProfile: (state, action: PayloadAction<string>) => {
      if (state.profiles[action.payload]) {
        delete state.profiles[action.payload]
        localStorage.setItem('profiles', JSON.stringify(state.profiles))
        localStorage.removeItem('selectedProfile')
      }
    },
    renameProfile: (
      state,
      action: PayloadAction<{ oldName: string; newName: string }>
    ) => {
      if (state.profiles[action.payload.oldName]) {
        // Create a copy of the old profile with the new name and delete the old profile.
        state.profiles[action.payload.newName] =
          state.profiles[action.payload.oldName]
        delete state.profiles[action.payload.oldName]
        localStorage.setItem('profiles', JSON.stringify(state.profiles))
        localStorage.setItem('selectedProfile', action.payload.newName)
      }
    },
    setTalentsState: (state, action: PayloadAction<TalentStore>) => {
      state.talents = action.payload
      localStorage.setItem('talents', JSON.stringify(action.payload))
    },
    setRotationState: (state, action: PayloadAction<RotationStruct>) => {
      state.rotation = action.payload
      localStorage.setItem('rotation', JSON.stringify(action.payload))
    },
    setSettingsState: (state, action: PayloadAction<Settings>) => {
      state.settings = action.payload
      localStorage.setItem('settings', JSON.stringify(action.payload))
    },
  },
})

export const {
  setSettingsState,
  setRotationState,
  setTalentsState,
  setSelectedGems,
  setSelectedEnchants,
  setSelectedItems,
  setSelectedAuras,
  setItemSetCounts,
  setAurasStats,
  setBaseStats,
  setEnchantsStats,
  setGemsStats,
  setItemsStats,
  renameProfile,
  deleteProfile,
  setTalentPointValue,
  toggleRotationSpellSelection,
  modifySettingValue,
  setProfile,
} = PlayerSlice.actions
export default PlayerSlice.reducer
