import { ItemSlot } from "../Types";

export const PresetItemSets: {
  name: string;
  items: {
    [key in ItemSlot]?: {
      itemId: number;
      gemIds?: number[];
      enchantId?: number;
    };
  };
}[] = [
  {
    name: "Shadow Destruction Pre-BiS",
    items: {
      head: { itemId: 28193, gemIds: [24030, 34220] },
      neck: { itemId: 28134 },
      shoulders: { itemId: 21869, gemIds: [31867, 24056], enchantId: 35406 },
      back: { itemId: 23050, enchantId: 25084 },
      chest: { itemId: 21871, gemIds: [31867, 24056], enchantId: 27960 },
      bracer: { itemId: 24250, gemIds: [31867], enchantId: 27917 },
      gloves: { itemId: 27465, gemIds: [24030, 24030], enchantId: 33997 },
      belt: { itemId: 30038, gemIds: [24056, 31867] },
      legs: { itemId: 24262, gemIds: [31867, 24030, 24030], enchantId: 31372 },
      boots: { itemId: 21870, gemIds: [31867, 24056], enchantId: 34008 },
      ring1: { itemId: 28227, enchantId: 27924 },
      ring2: { itemId: 21709, enchantId: 27924 },
      trinket1: { itemId: 29370 },
      trinket2: { itemId: 27683 },
      mainhand: { itemId: 29155, enchantId: 27982 },
      offhand: { itemId: 29273 },
      wand: { itemId: 22821 },
    },
  },
];
