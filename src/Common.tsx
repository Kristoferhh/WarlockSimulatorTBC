import { ItemSlot, ItemSlotKey } from "./Types";

export function ItemSlotKeyToItemSlot(itemSlot: ItemSlotKey, itemSubSlot?: string): ItemSlot {
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
    case ItemSlotKey.Twohand: return ItemSlot.twohand;
    case ItemSlotKey.Wand: return ItemSlot.wand;
  }
}

export function ItemSlotToItemSlotKey(itemSlot: ItemSlot): ItemSlotKey {
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
    case ItemSlot.twohand: return ItemSlotKey.Twohand;
    case ItemSlot.wand: return ItemSlotKey.Wand;
  }
}