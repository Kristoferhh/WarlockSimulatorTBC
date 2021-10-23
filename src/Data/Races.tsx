import { Race, Stat } from "../Types";

export const Races: {name: string, varName: Race, stats: {[key in Stat]?: number}}[] = [
  {
    name: 'Gnome',
    varName: Race.Gnome,
    stats: {
      stamina: 75,
      intellect: 136,
      spirit: 139,
      arcaneResist: 10,
      intellectModifier: 1.05
    }
  },
  {
    name: 'Human',
    varName: Race.Human,
    stats: {
      stamina: 76,
      intellect: 133,
      spirit: 139,
      spiritModifier: 1.1
    }
  },
  {
    name: 'Orc',
    varName: Race.Orc,
    stats: {
      stamina: 78,
      intellect: 130,
      spirit: 142,
      petDamageModifier: 1.05
    }
  },
  {
    name: 'Undead',
    varName: Race.Undead,
    stats: {
      stamina: 77,
      intellect: 131,
      spirit: 144,
      shadowResist: 10
    }
  },
  {
    name: 'Blood Elf',
    varName: Race.BloodElf,
    stats: {
      stamina: 74,
      intellect: 137,
      spirit: 138,
      fireResist: 5,
      frostResist: 5,
      shadowResist: 5,
      arcaneResist: 5,
      natureResist: 5
    }
  }
]