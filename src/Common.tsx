import { Gems } from "./data/Gems";
import { Items } from "./data/Items";
import { Sockets } from "./data/Sockets";
import { GemColor, ItemSlot, ItemSlotKey, SelectedGemsStruct, SettingsStruct, TalentStore } from "./Types";

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
  if (socketArray === undefined && params.selectedGems !== undefined) {
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
      const gemColor = Gems.find(e => e.id === currentGemId)!!.color;
      const item = Items.find(e => e.id === params.itemId)!!;
      if (!Sockets.find(e => e.color === item.sockets!![parseInt(key)])!!.validColors.includes(gemColor)) {
        return false;
      }
    }
  
    return true;
  }

  return false;
}

export function getRemainingTalentPoints(talents: TalentStore) {
  return 61 - Object.values<number>(talents).reduce((a, b) => a + b, 0); // 61 available talent points at lvl 70
}

export function shouldDisplayPetSetting(talents: TalentStore, settings: SettingsStruct, requiresAggressivePet: boolean) {
  return (!talents['demonicSacrifice'] || settings['sacrificePet'] === 'no') && (!requiresAggressivePet || settings['petMode'] === '1');
}