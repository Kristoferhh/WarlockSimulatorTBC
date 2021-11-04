import { Auras } from "./data/Auras";
import { Enchants } from "./data/Enchants";
import { Gems } from "./data/Gems";
import { Items } from "./data/Items";
import { Races } from "./data/Races";
import { Sockets } from "./data/Sockets";
import { TalentTreeStruct } from "./data/Talents";
import { AurasStruct, GemColor, InitialPlayerStats, InitialSetCounts, Item, ItemAndEnchantStruct, ItemSet, ItemSlot, ItemSlotKey, Languages, PlayerState, Race, SelectedGemsStruct, SetsStruct, Settings, SocketColor, SourcesStruct, Stat, StatConstant, StatsCollection, SubSlotValue, TalentStore } from "./Types";

export function ItemSlotKeyToItemSlot(forEnchants: boolean, itemSlot: ItemSlotKey, itemSubSlot?: string): ItemSlot {
  switch(itemSlot) {
    case ItemSlotKey.Head: return ItemSlot.head;
    case ItemSlotKey.Neck: return ItemSlot.neck;
    case ItemSlotKey.Shoulders: return ItemSlot.shoulders;
    case ItemSlotKey.Back: return ItemSlot.back;
    case ItemSlotKey.Chest: return ItemSlot.chest;
    case ItemSlotKey.Bracer: return ItemSlot.bracer;
    case ItemSlotKey.Gloves: return ItemSlot.gloves;
    case ItemSlotKey.Belt: return ItemSlot.belt;
    case ItemSlotKey.Legs: return ItemSlot.legs;
    case ItemSlotKey.Boots: return ItemSlot.boots;
    case ItemSlotKey.Ring: return itemSubSlot === '1' ? ItemSlot.ring1 : ItemSlot.ring2;
    case ItemSlotKey.Trinket: return itemSubSlot === '1' ? ItemSlot.trinket1 : ItemSlot.trinket2;
    case ItemSlotKey.Mainhand: return ItemSlot.mainhand;
    case ItemSlotKey.Offhand: return ItemSlot.offhand;
    case ItemSlotKey.Twohand: return forEnchants ? ItemSlot.mainhand : ItemSlot.twohand;
    case ItemSlotKey.Wand: return ItemSlot.wand;
  }
}

export function ItemSlotToItemSlotKey(forEnchants: boolean, itemSlot: ItemSlot): ItemSlotKey {
  switch(itemSlot) {
    case ItemSlot.head: return ItemSlotKey.Head;
    case ItemSlot.neck: return ItemSlotKey.Neck;
    case ItemSlot.shoulders: return ItemSlotKey.Shoulders;
    case ItemSlot.back: return ItemSlotKey.Back;
    case ItemSlot.chest: return ItemSlotKey.Chest;
    case ItemSlot.bracer: return ItemSlotKey.Bracer;
    case ItemSlot.gloves: return ItemSlotKey.Gloves;
    case ItemSlot.belt: return ItemSlotKey.Belt;
    case ItemSlot.legs: return ItemSlotKey.Legs;
    case ItemSlot.boots: return ItemSlotKey.Boots;
    case ItemSlot.ring1: return ItemSlotKey.Ring;
    case ItemSlot.ring2: return ItemSlotKey.Ring;
    case ItemSlot.trinket1: return ItemSlotKey.Trinket;
    case ItemSlot.trinket2: return ItemSlotKey.Trinket;
    case ItemSlot.mainhand: return ItemSlotKey.Mainhand;
    case ItemSlot.offhand: return ItemSlotKey.Offhand;
    case ItemSlot.twohand: return forEnchants ? ItemSlotKey.Mainhand : ItemSlotKey.Twohand;
    case ItemSlot.wand: return ItemSlotKey.Wand;
  }
}

export function itemMeetsSocketRequirements(params: { itemId: number, selectedGems?: SelectedGemsStruct, socketArray?: [string, number][] }): boolean {
  let socketArray = params.socketArray;

  // If the socketArray parameter is undefined then find the array using the selectedGems parameter instead
  if (socketArray === undefined && params.selectedGems === undefined && params.selectedGems) {
    for (const itemSlotKey of Object.keys(params.selectedGems)) {
      const itemSlot = ItemSlotToItemSlotKey(false, itemSlotKey as ItemSlot);
      const itemGemArrays = params.selectedGems[itemSlot];
  
      if (itemGemArrays && itemGemArrays[params.itemId]) {
        socketArray = itemGemArrays[params.itemId];
        break;
      }
    }
  }

  if (socketArray) {
    // Loop through the gems in the item and if any of gems don't match the socket's color or if a gem isn't equipped then return false.
    for (const [key, value] of Object.entries(socketArray)) {
      const currentGemId = value[1];
  
      if (currentGemId === 0) {
        return false;
      }
  
      // Find the item object to get access to the item's socket array to get the socket color
      const gem = Gems.find(e => e.id === currentGemId);
      if (gem) {
        const gemColor = gem.color;
        const item = Items.find(e => e.id === params.itemId);
        // Check if the array of valid gem colors for this socket color does not include the equipped gem color.
        if (item && item.sockets && !Sockets.find(e => e.color === item.sockets![parseInt(key)])?.validColors.includes(gemColor)) {
          return false;
        }
      }
    }
  
    return true;
  }

  return false;
}

export function getRemainingTalentPoints(talents: TalentStore): number {
  return 61 - Object.values<number>(talents).reduce((a, b) => a + b, 0); // 61 available talent points at lvl 70
}

export function isPetActive(talents: TalentStore, settings: Settings, requiresAggressivePet: boolean): boolean {
  return (talents.demonicSacrifice === 0 || settings.sacrificePet === 'no') && (!requiresAggressivePet || settings.petMode === '1');
}

export function canGemColorBeInsertedIntoSocketColor(socketColor: SocketColor, gemColor: GemColor): boolean {
  return (socketColor === SocketColor.Meta && gemColor === GemColor.Meta) || (socketColor !== SocketColor.Meta && gemColor !== GemColor.Meta);
}

/**
 * Returns an array of items meeting the criteria to be displayed in the item selection table.
 * The item needs to be of the specified item slot, the item's phase needs to be selected, it needs to not be hidden unless the player is showing hidden items
 * and the item needs to not be unique unless it is not equipped in the other item slot (only applicable to rings and trinkets).
 */
export function getItemTableItems(itemSlot: ItemSlotKey, itemSubSlot: SubSlotValue, selectedItems: ItemAndEnchantStruct, sources: SourcesStruct, hiddenItems: number[], hidingItems: boolean): Item[] {
  return Items.filter((e) => e.itemSlot === itemSlot && sources.phase[e.phase] === true && (!hiddenItems.includes(e.id) || hidingItems) && (!e.unique || (selectedItems[ItemSlotKeyToItemSlot(false, itemSlot, itemSubSlot === '1' ? '2' : '1')] !== e.id)));
}

export function getStdev (array: number[]) {
  if (!array || array.length === 0) { return 0 }
  const n = array.length
  const mean = array.reduce((a, b) => a + b) / n
  return Math.sqrt(array.map(x => Math.pow(x - mean, 2)).reduce((a, b) => a + b) / n)
}

export function average(nums?: number[]) {
  if (nums === undefined || nums.length === 0) { return 0; }
  return nums.reduce((a, b) => a + b) / nums.length;
}

export function getBaseStats(race: Race, statsObj?: StatsCollection): StatsCollection {
  let stats = JSON.parse(JSON.stringify(InitialPlayerStats));

  const raceObj = Races.find(e => e.varName === race);
  if (raceObj && raceObj.stats) {
    Object.entries(raceObj.stats).forEach(stat => {
      addOrMultiplyStat(statsObj || stats, stat[0] as Stat, stat[1]);
    });
  }

  return statsObj || stats;
}

export function getAurasStats(auras: AurasStruct, statsObj?: StatsCollection): StatsCollection {
  let stats: StatsCollection = JSON.parse(JSON.stringify(InitialPlayerStats));

  Object.entries(auras).forEach(aura => {
    // Check if the aura is selected
    if (aura[1] === true) {
      const auraObj = Auras.find(e => e.varName === aura[0]);
      if (auraObj && auraObj.stats) {
        Object.entries(auraObj.stats).forEach(auraStat => {
          addOrMultiplyStat(statsObj || stats, auraStat[0] as Stat, auraStat[1]);
        })
      }
    }
  });

  return statsObj || stats;
}

export function getItemsStats(items: ItemAndEnchantStruct, statsObj?: StatsCollection): StatsCollection {
  let stats: StatsCollection = JSON.parse(JSON.stringify(InitialPlayerStats));

  Object.values(items).forEach(itemId => {
    const itemObj = Items.find(e => e.id === itemId);
    if (itemObj) {
      Object.entries(itemObj).forEach(item => {
        if (item[0] in Stat) {
          addOrMultiplyStat(statsObj || stats, item[0] as Stat, item[1]);
        }
      })
    }
  });

  return statsObj || stats;
}

export function getGemsStats(items: ItemAndEnchantStruct, gems: SelectedGemsStruct, statsObj?: StatsCollection): StatsCollection {
  let stats: StatsCollection = JSON.parse(JSON.stringify(InitialPlayerStats));

  Object.entries(items).forEach(item => {
    const itemSlotGems = gems[ItemSlotToItemSlotKey(false, item[0] as ItemSlot)];
    if (itemSlotGems) {
      const itemGemArrays = itemSlotGems[item[1]];
      if (itemGemArrays) {
        itemGemArrays.map(e => e[1]).forEach(gemId => {
          const gem = Gems.find(e => e.id === gemId);
          if (gem && gem.stats) {
            Object.entries(gem.stats).forEach(gemStat => {
              addOrMultiplyStat(statsObj || stats, gemStat[0] as Stat, gemStat[1]);
            })
          }
        });

        if (itemMeetsSocketRequirements({ itemId: item[1], socketArray: itemGemArrays })) {
          const itemObj = Items.find(e => e.id === item[1]);
          if (itemObj && itemObj.socketBonus) {
            Object.entries(itemObj.socketBonus).forEach(stat => {
              addOrMultiplyStat(statsObj || stats, stat[0] as Stat, stat[1]);
            })
          }
        }
      }
    }
  });

  return statsObj || stats;
}

export function getEnchantsStats(items: ItemAndEnchantStruct, enchants: ItemAndEnchantStruct, statsObj?: StatsCollection): StatsCollection {
  let stats: StatsCollection = JSON.parse(JSON.stringify(InitialPlayerStats));

  Object.entries(enchants).forEach(enchant => {
    // Only add the enchant's stats if the user has an item equipped in that slot as well.
    if (Items.find(e => e.id === items[enchant[0] as ItemSlot]) || (enchant[0] === ItemSlot.mainhand && Items.find(e => e.id === items[ItemSlot.twohand]))) {
      const enchantObj = Enchants.find(e => e.id === enchant[1]);
      if (enchantObj) {
        Object.entries(enchantObj).forEach(prop => {
          if (prop[0] in Stat) {
            addOrMultiplyStat(statsObj || stats, prop[0] as Stat, prop[1]);
          }
        })
      }
    }
  });

  return statsObj || stats;
}

export function getItemSetCounts(items: ItemAndEnchantStruct): SetsStruct {
  let sets = JSON.parse(JSON.stringify(InitialSetCounts));

  Object.values(items).forEach(itemId => {
    const itemObj = Items.find(e => e.id === itemId);
    if (itemObj && itemObj.setId) {
      sets[itemObj.setId]++;
    }
  });

  return sets;
}

export function addOrMultiplyStat(statsCollection: StatsCollection, stat: Stat, value: number): void {
  if (stat.includes('Modifier')) {
    statsCollection[stat]! *= value;
  } else {
    statsCollection[stat]! += value;
  }
}

export function calculatePlayerStats(playerState: PlayerState): StatsCollection {
  let mainStatsObj: StatsCollection = JSON.parse(JSON.stringify(InitialPlayerStats));

  getBaseStats(playerState.settings.race as Race, mainStatsObj);
  getAurasStats(playerState.auras, mainStatsObj);
  getItemsStats(playerState.selectedItems, mainStatsObj);
  getGemsStats(playerState.selectedItems, playerState.selectedGems, mainStatsObj);
  getEnchantsStats(playerState.selectedItems, playerState.selectedEnchants, mainStatsObj);

  return mainStatsObj;
}

export function getPlayerHitRating(playerState: PlayerState): number {
  let hitRating = Object.values(playerState.stats).map(obj => obj.hitRating || 0).reduce((a, b) => a + b);
  if (playerState.sets[ItemSet.ManaEtchedRegalia] >= 2) hitRating += 35;
  return hitRating;
}

export function getPlayerHitPercent(playerState: PlayerState, hitRating?: number): number {
  let hitPercent = (hitRating || getPlayerHitRating(playerState)) / StatConstant.hitRatingPerPercent;
  if (playerState.auras.inspiringPresence) hitPercent++;
  if (playerState.auras.totemOfWrath) hitPercent += 3 * parseInt(playerState.settings.totemOfWrathAmount);
  return hitPercent;
}

export function getAllocatedTalentsPointsInTree(talentState: TalentStore, tree: TalentTreeStruct): number {
  let points = 0;

  for (const row in tree.rows) {
    for (const talent in tree.rows[row]) {
      const talentKey = tree.rows[row][talent].varName;

      if (talentKey != null) {
        points += talentState[talentKey];
      }
    }
  }

  return points;
}

export function getBaseWowheadUrl(language: string): string {
  return `https://${Languages.find(e => e.iso === language)!.wowheadPrefix}tbc.wowhead.com`;
}