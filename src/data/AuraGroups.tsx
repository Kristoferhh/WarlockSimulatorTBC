import { IAuraGroup, AuraGroup } from "../Types";

export const AuraGroups: IAuraGroup[] = [
  { heading: AuraGroup.Buffs, type: "spell", varName: "buffs" },
  { heading: AuraGroup.Debuffs, type: "spell", varName: "debuffs" },
  { heading: AuraGroup.Consumables, type: "item", varName: "consumables" },
  { heading: AuraGroup.PetBuffs, type: "spell", varName: "petBuffs" },
];
