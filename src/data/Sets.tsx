import { ItemSet, Quality } from "../Types";

export const Sets: {
  name: string;
  setId: ItemSet;
  quality: Quality;
  bonuses: number[];
}[] = [
  // Season 3 Arena
  {
    name: "Vengeful Gladiator's Felshroud",
    setId: ItemSet.VengefulGladiatorsFelshroud,
    quality: Quality.Epic,
    bonuses: [2, 4],
  },
  // Season 3 Arena
  {
    name: "Vengeful Gladiator's Dreadgear",
    setId: ItemSet.VengefulGladiatorsDreadgear,
    quality: Quality.Epic,
    bonuses: [2, 4],
  },
  // Season 2 Arena
  {
    name: "Merciless Gladiator's Felshroud",
    setId: ItemSet.MercilessGladiatorsFelshroud,
    quality: Quality.Epic,
    bonuses: [2, 4],
  },
  // Season 2 Arena
  {
    name: "Merciless Gladiator's Dreadgear",
    setId: ItemSet.MercilessGladiatorsDreadgear,
    quality: Quality.Epic,
    bonuses: [2, 4],
  },
  // T3
  {
    name: "Plagueheart Raiment",
    setId: ItemSet.T3,
    quality: Quality.Epic,
    bonuses: [2, 4, 6, 8],
  },
  // Spellfire (Tailoring)
  {
    name: "Wrath of Spellfire",
    setId: ItemSet.Spellfire,
    quality: Quality.Epic,
    bonuses: [3],
  },
  // Frozen Shadoweave (Tailoring)
  {
    name: "Shadow's Embrace",
    setId: ItemSet.FrozenShadoweave,
    quality: Quality.Epic,
    bonuses: [3],
  },
  // Spellstrike (Tailoring)
  {
    name: "Spellstrike Infusion",
    setId: ItemSet.Spellstrike,
    quality: Quality.Epic,
    bonuses: [2],
  },
  // Dungeon Set
  {
    name: "Oblivion Raiment",
    setId: ItemSet.Oblivion,
    quality: Quality.Rare,
    bonuses: [2, 4],
  },
  // T4
  {
    name: "Voidheart Raiment",
    setId: ItemSet.T4,
    quality: Quality.Epic,
    bonuses: [2, 4],
  },
  // T5
  {
    name: "Corruptor Raiment",
    setId: ItemSet.T5,
    quality: Quality.Epic,
    bonuses: [2, 4],
  },
  // Dungeon Set
  {
    name: "Mana-Etched Regalia",
    setId: ItemSet.ManaEtchedRegalia,
    quality: Quality.Rare,
    bonuses: [2, 4],
  },
  // Dungeon Set
  {
    name: "Incanter's Regalia",
    setId: ItemSet.IncantersRegalia,
    quality: Quality.Rare,
    bonuses: [2, 4],
  },
  // The Twin Stars
  {
    name: "The Twin Stars",
    setId: ItemSet.TwinStars,
    quality: Quality.Epic,
    bonuses: [2],
  },
  // T6
  {
    name: "Malefic Raiment",
    setId: ItemSet.T6,
    quality: Quality.Epic,
    bonuses: [2, 4],
  },
  // Season 1 Arena
  {
    name: "Gladiator's Felshroud",
    setId: ItemSet.GladiatorsFelshroud,
    quality: Quality.Epic,
    bonuses: [2, 4],
  },
  // Season 1 Arena
  {
    name: "Gladiator's Dreadgear",
    setId: ItemSet.GladiatorsDreadgear,
    quality: Quality.Epic,
    bonuses: [2, 4],
  },
  // LvL 70 Rare PvP Set
  {
    name: "High Warlord's Dreadgear",
    setId: ItemSet.HighWarlordsDreadgear,
    quality: Quality.Rare,
    bonuses: [2, 4],
  },
];
