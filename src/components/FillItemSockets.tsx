import { useState } from "react";
import { useDispatch, useSelector } from "react-redux"
import { shouldDisplayGemOfSocketColor } from "../Common";
import { Gems } from "../data/Gems";
import { Items } from "../data/Items";
import { setItemSocketsValue } from "../redux/PlayerSlice";
import { RootState } from "../redux/Store"
import { setFillItemSocketsWindowVisibility, setSelectedItemSlot } from "../redux/UiSlice";
import { Item, SocketColor } from "../Types";


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
        <input type="radio" name='gem-replacement-option' value='emptySockets' onChange={(e) => setReplacingExistingGems(false)} checked={replacingExistingGems === false} />
        <label htmlFor='emptySockets'>Fill empty sockets</label>
        <input type="radio" name='gem-replacement-option' value='allSockets' onChange={(e) => setReplacingExistingGems(true)} checked={replacingExistingGems === true} />
        <label htmlFor='allSockets'>Fill all sockets (replaces equipped gems)</label>
      </div>
      <div id='gem-options-window-item-slot'>
        <input type='radio' name='item-slot' value='currentSlot' onChange={(e) => setItemSlotToFill('currentSlot')} checked={itemSlotToFill === 'currentSlot'} />
        <label htmlFor='currentSlot'>Current item slot</label>
        <input type='radio' name='item-slot' value='allSlots' onChange={(e) => setItemSlotToFill('allSlots')} checked={itemSlotToFill === 'allSlots'} />
        <label htmlFor='allSlots'>All item slots</label>
      </div>
      <div id='gem-options-window-socket-selection'>
        <input type="radio" name="socket-selection" value='meta' onChange={(e) => socketColorClickHandler(SocketColor.Meta)} checked={socketColor === SocketColor.Meta} />
        <label>Meta Sockets</label>
        <input type="radio" name="socket-selection" value='red' onChange={(e) => socketColorClickHandler(SocketColor.Red)} checked={socketColor === SocketColor.Red} />
        <label>Red Sockets</label>
        <input type="radio" name="socket-selection" value='blue' onChange={(e) => socketColorClickHandler(SocketColor.Blue)} checked={socketColor === SocketColor.Blue} />
        <label>Blue Sockets</label>
        <input type="radio" name="socket-selection" value='yellow' onChange={(e) => socketColorClickHandler(SocketColor.Yellow)} checked={socketColor === SocketColor.Yellow} />
        <label>Yellow Sockets</label>
      </div>
      <div id='gem-options-gem-list'>
        <div id='gem-options-gem-list'>
          {
            Gems.map((gem, i) =>
              shouldDisplayGemOfSocketColor(socketColor, gem.color) &&
                <div
                  className='gem-options-gem'
                  key={i}
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