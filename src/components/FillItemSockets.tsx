import { nanoid } from "@reduxjs/toolkit";
import { useState } from "react";
import { useDispatch, useSelector } from "react-redux"
import { canGemColorBeInsertedIntoSocketColor } from "../Common";
import { Gems } from "../data/Gems";
import { Items } from "../data/Items";
import { setItemSocketsValue } from "../redux/PlayerSlice";
import { RootState } from "../redux/Store"
import { setFillItemSocketsWindowVisibility } from "../redux/UiSlice";
import { Item, SocketColor } from "../Types";

const socketOptions: {name: string, color: SocketColor}[] = [
  { name: 'Meta Socket', color: SocketColor.Meta },
  { name: 'Red Socket', color: SocketColor.Red },
  { name: 'Yellow Socket', color: SocketColor.Yellow },
  { name: 'Blue Socket', color: SocketColor.Blue }
]

export function FillItemSockets() {
  const uiState = useSelector((state: RootState) => state.ui);
  const playerState = useSelector((state: RootState) => state.player);
  const dispatch = useDispatch();
  const [socketColor, setSocketColor] = useState(SocketColor.Red);
  const [selectedGemId, setSelectedGemId] = useState(0);
  const [itemSlotToFill, setItemSlotToFill] = useState<'currentSlot' | 'allSlots'>('currentSlot');
  const [replacingExistingGems, setReplacingExistingGems] = useState(false);

  function fillSockets(): void {
    if (selectedGemId === 0) {
      return;
    }

    Items.filter(item => (item.itemSlot === uiState.selectedItemSlot && itemSlotToFill === 'currentSlot') || itemSlotToFill === 'allSlots').forEach(item => {
      fillSocketsInItem(item);
    });

    dispatch(setFillItemSocketsWindowVisibility(false));
  }

  function fillSocketsInItem(item: Item): void {
    if (!item.sockets) {
      return;
    }

    // Create an empty gem array.
    // If the item already has a gem array then replace it with the existing one.
    let itemGemArray = Array(item.sockets.length).fill(['', 0]);
    if (playerState.selectedGems[item.itemSlot] && playerState.selectedGems[item.itemSlot][item.id]) {
      itemGemArray = JSON.parse(JSON.stringify(playerState.selectedGems[item.itemSlot][item.id]));
    }

    // Loop through the sockets in the item and insert the gem if the socket color matches the user's choice or if the user is inserting into all sockets
    // and if the socket is empty or if the user chose to replace existing gems as well
    for (let i = 0; i < item.sockets.length; i++) {
      if (item.sockets[i] === socketColor && ([null, 0].includes(itemGemArray[i][1]) || replacingExistingGems)) {
        itemGemArray[i] = [item.sockets[i], selectedGemId];
      }
    }

    dispatch(setItemSocketsValue({
      itemId: item.id.toString(),
      itemSlot: item.itemSlot,
      value: itemGemArray
    }));
  }

  function socketColorClickHandler(newColor: SocketColor) {
    if ((socketColor === SocketColor.Meta && newColor !== SocketColor.Meta) || (socketColor !== SocketColor.Meta && newColor === SocketColor.Meta)) {
      setSelectedGemId(0);
    }
    setSocketColor(newColor);
  }
  
  return (
    <div id='gem-options-window' style={{display: uiState.fillItemSocketsWindowVisible ? '' : 'none'}}>
      <div id='gem-options-window-replacement-options'>
        <label htmlFor='emptySockets'>Fill empty sockets</label>
        <input type="radio" name='gem-replacement-option' id='emptySockets' onChange={(e) => setReplacingExistingGems(false)} checked={replacingExistingGems === false} />
        <label htmlFor='allSockets'>Fill all sockets (replaces equipped gems)</label>
        <input type="radio" name='gem-replacement-option' id='allSockets' onChange={(e) => setReplacingExistingGems(true)} checked={replacingExistingGems === true} />
      </div>
      <div id='gem-options-window-item-slot'>
        <label htmlFor='currentSlot'>Current item slot</label>
        <input type='radio' name='item-slot' id='currentSlot' onChange={(e) => setItemSlotToFill('currentSlot')} checked={itemSlotToFill === 'currentSlot'} />
        <label htmlFor='allSlots'>All item slots</label>
        <input type='radio' name='item-slot' id='allSlots' onChange={(e) => setItemSlotToFill('allSlots')} checked={itemSlotToFill === 'allSlots'} />
      </div>
      <div id='gem-options-window-socket-selection'>
        {
          socketOptions.map(socket =>
            <label key={nanoid()}>{socket.name}
              <input type='radio' name='socket-selection' onChange={(e) => socketColorClickHandler(socket.color)} checked={socketColor === socket.color}></input>
            </label>          
          )
        }
      </div>
      <div id='gem-options-gem-list'>
        <div id='gem-options-gem-list'>
          {
            Gems.filter(e => canGemColorBeInsertedIntoSocketColor(socketColor, e.color)).map(gem =>
              <div
                className='gem-options-gem'
                key={gem.id}
                onClick={(e) => { setSelectedGemId(gem.id); e.preventDefault(); }}
                data-checked={selectedGemId === gem.id}
              >
                <img src={`${process.env.PUBLIC_URL}/img/${gem.iconName}.jpg`} alt={gem.name} />
                <a href={`https://tbc.wowhead.com/item=${gem.id}`}>{gem.name}</a>
              </div>
            )
          }
        </div>
      </div>
      <button id='gem-options-apply-button' onClick={(e) => fillSockets()} disabled={selectedGemId === 0}>Apply</button>
      <button onClick={(e) => dispatch(setFillItemSocketsWindowVisibility(false))}>Close</button>
    </div>
  )
}