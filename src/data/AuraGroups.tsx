import { AuraGroup, AuraGroupKey } from "../Types";

export const AuraGroups: AuraGroup[] = [
  {
    heading: AuraGroupKey.Buffs,
    type: 'spell',
    varName: 'buffs'
  },
  {
    heading: AuraGroupKey.Debuffs,
    type: 'spell',
    varName: 'debuffs',
  },
  {
    heading: AuraGroupKey.Consumables,
    type: 'item',
    varName: 'consumables',
  },
  {
    heading: AuraGroupKey.PetBuffs,
    type: 'spell',
    varName: 'petBuffs'
  }
]