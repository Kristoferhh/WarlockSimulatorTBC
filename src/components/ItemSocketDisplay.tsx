import { nanoid } from "@reduxjs/toolkit";
import { useSelector } from "react-redux";
import { getBaseWowheadUrl } from "../Common";
import { Gems } from "../data/Gems";
import { Sockets } from "../data/Sockets";
import i18n from "../i18n/config";
import { RootState } from "../redux/Store";
import { Item, ItemSlotKey, SocketColor } from "../Types";


interface Props {
  item: Item,
  itemSlot: ItemSlotKey,
  itemSocketClickHandler?: (itemId: string, socketNumber: number, socketColor: SocketColor) => void,
  removeGemFromSocket?: (itemId: string, socketNumber: number) => void
}

export default function ItemSocketDisplay(props: Props) {
  const playerState = useSelector((state: RootState) => state.player);

  return (
    <div>
      {
        props.item.sockets?.map((socket, j) =>
          <a
            target='_blank'
            rel='noreferrer'
            href={playerState.selectedGems[props.itemSlot] && playerState.selectedGems[props.itemSlot][props.item.id] && playerState.selectedGems[props.itemSlot][props.item.id][j] && playerState.selectedGems[props.itemSlot][props.item.id][j][1] !== 0 ? `${getBaseWowheadUrl(i18n.language)}/item=${playerState.selectedGems[props.itemSlot][props.item.id][j][1]}` : ''}
            key={nanoid()}
            onClick={(e) => { props.itemSocketClickHandler && props.itemSocketClickHandler(props.item.id.toString(), j, socket); e.preventDefault(); e.stopPropagation(); }}
            onContextMenu={(e) => { props.removeGemFromSocket && props.removeGemFromSocket(props.item.id.toString(), j); e.preventDefault(); }}
          >
            <img
              width={16}
              height={16}
              data-color={socket}
              src={playerState.selectedGems[props.itemSlot] && playerState.selectedGems[props.itemSlot][props.item.id] && playerState.selectedGems[props.itemSlot][props.item.id][j] && ![null, 0].includes(playerState.selectedGems[props.itemSlot][props.item.id][j][1]) ? `${process.env.PUBLIC_URL}/img/${Gems.find(e => e.id === playerState.selectedGems[props.itemSlot][props.item.id][j][1])?.iconName}.jpg` : `${process.env.PUBLIC_URL}/img/${Sockets.find(s => s.color === socket)!.iconName}.jpg`}
              alt={socket + ' socket'}
            />
          </a>
        )
      }
    </div>
  )
}