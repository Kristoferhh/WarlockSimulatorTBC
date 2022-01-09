import { SocketColor, GemColor } from "../Types";


export const Sockets: { color: SocketColor, iconName: string, validColors: GemColor[] }[] = [
  {
    color: SocketColor.Meta,
    iconName: 'UI-EmptySocket-Meta',
    validColors: [GemColor.Meta]
  },
  {
    color: SocketColor.Red,
    iconName: 'UI-EmptySocket-Red',
    validColors: [GemColor.Red, GemColor.Orange, GemColor.Purple, GemColor.Void]
  },
  {
    color: SocketColor.Yellow,
    iconName: 'UI-EmptySocket-Yellow',
    validColors: [GemColor.Yellow, GemColor.Orange, GemColor.Green, GemColor.Void]
  },
  {
    color: SocketColor.Blue,
    iconName: 'UI-EmptySocket-Blue',
    validColors: [GemColor.Blue, GemColor.Green, GemColor.Purple, GemColor.Void]
  }
]