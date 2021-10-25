import { Items } from '../data/Items';
import { Enchant, Item, ItemSlot, ItemSlotKey, Phase, SocketColor, Stat, SubSlotValue } from '../Types';
import { useState } from 'react';
import { Enchants } from '../data/Enchants';
import { useDispatch, useSelector } from 'react-redux';
import { RootState } from '../redux/Store';
import { modifyPlayerStat, setEnchantInItemSlot, setItemInItemSlot, setItemSocketsValue } from '../redux/PlayerSlice';
import { itemMeetsSocketRequirements, ItemSlotKeyToItemSlot, ItemSlotToItemSlotKey } from '../Common';
import { togglePhase, setGemSelectionTable } from '../redux/UiSlice';
import Profiles from './Profiles';
import { Sockets } from '../data/Sockets';
import { Gems } from '../data/Gems';

const itemSlotInformation: {name: string, itemSlot: ItemSlotKey, subSlot: SubSlotValue}[] = [
  { name: 'Main Hand', itemSlot: ItemSlotKey.Mainhand, subSlot: '' },
  { name: 'Off Hand', itemSlot: ItemSlotKey.Offhand, subSlot: '' },
  { name: 'Two Hand', itemSlot: ItemSlotKey.Twohand, subSlot: '' },
  { name: 'Head', itemSlot: ItemSlotKey.Head, subSlot: '' },
  { name: 'Neck', itemSlot: ItemSlotKey.Neck, subSlot: '' },
  { name: 'Shoulders', itemSlot: ItemSlotKey.Shoulders, subSlot: '' },
  { name: 'Back', itemSlot: ItemSlotKey.Back, subSlot: '' },
  { name: 'Chest', itemSlot: ItemSlotKey.Chest, subSlot: '' },
  { name: 'Bracer', itemSlot: ItemSlotKey.Bracer, subSlot: '' },
  { name: 'Gloves', itemSlot: ItemSlotKey.Gloves, subSlot: '' },
  { name: 'Belt', itemSlot: ItemSlotKey.Belt, subSlot: '' },
  { name: 'Legs', itemSlot: ItemSlotKey.Legs, subSlot: '' },
  { name: 'Boots', itemSlot: ItemSlotKey.Boots, subSlot: '' },
  { name: 'Ring 1', itemSlot: ItemSlotKey.Ring, subSlot: '1' },
  { name: 'Ring 2', itemSlot: ItemSlotKey.Ring, subSlot: '2' },
  { name: 'Trinket 1', itemSlot: ItemSlotKey.Trinket, subSlot: '1' },
  { name: 'Trinket 2', itemSlot: ItemSlotKey.Trinket, subSlot: '2' },
  { name: 'Wand', itemSlot: ItemSlotKey.Wand, subSlot: ''}
];

const phases: {title: string, phase: Phase}[] = [
  { title: 'Classic', phase: 0 },
  { title: 'P1', phase: 1 },
  { title: 'P2', phase: 2 },
  { title: 'P3', phase: 3 },
  { title: 'P4', phase: 4 },
  { title: 'P5', phase: 5 },
]

export default function ItemSelection() {
  const [itemSlot, setItemSlot] = useState<ItemSlotKey>(JSON.parse(localStorage.getItem('selectedItemSlot') || JSON.stringify('mainhand')));
  const [itemSubSlot, setItemSubSlot] = useState<SubSlotValue>(JSON.parse(localStorage.getItem('selectedItemSubSlot') || JSON.stringify('1')));
  const playerStore = useSelector((state: RootState) => state.player);
  const uiStore = useSelector((state: RootState) => state.ui);
  const dispatch = useDispatch();

  function itemClickHandler(item: Item, slot: ItemSlot) {
    // Remove stats from equipped item
    if (playerStore.selectedItems[slot] !== 0) {
      const itemObj = Items.find(i => i.id === playerStore.selectedItems[slot]);

      for (const [prop, value] of Object.entries(itemObj!!)) {
        if (playerStore.stats.hasOwnProperty(prop)) {
          dispatch(modifyPlayerStat({
            stat: prop as Stat,
            value: value,
            action: 'remove'
          }));
        }
      }

      // Remove stats from socket bonus if it's active
      if (itemMeetsSocketRequirements({itemId: itemObj!!.id, selectedGems: playerStore.selectedGems})) {
        for (const [stat, value] of Object.entries(itemObj!!.socketBonus!!)) {
          dispatch(modifyPlayerStat({
            stat: stat as Stat,
            value: value,
            action: 'remove'
          }));
        }
      }
    }

    // If the item that was clicked on is the currently equipped item then don't add the stats since it's being unequipped
    if (playerStore.selectedItems[slot] !== item.id) {
      // Add stats from new item
      for (const [prop, value] of Object.entries(item)) {
        if (playerStore.stats.hasOwnProperty(prop)) {
          dispatch(modifyPlayerStat({
            stat: prop as Stat,
            value: value,
            action: 'add'
          }));
        }
      }

      // Add stats from the socket bonus
      if (itemMeetsSocketRequirements({itemId: item.id, selectedGems: playerStore.selectedGems})) {
        for (const [stat, value] of Object.entries(item.socketBonus!!)) {
          dispatch(modifyPlayerStat({
            stat: stat as Stat,
            value: value,
            action: 'add'
          }));
        }
      }
    }

    dispatch(setItemInItemSlot({
      id: item.id,
      itemSlot: slot,
    }));
  }

  function enchantClickHandler(enchant: Enchant, slot: ItemSlot) {
    // Remove stats from selected enchant
    if (playerStore.selectedEnchants[slot] !== 0) {
      const enchantObj = Enchants.find(i => i.id === playerStore.selectedEnchants[slot]);

      for (const [prop, value] of Object.entries(enchantObj!!)) {
        if (playerStore.stats.hasOwnProperty(prop)) {
          dispatch(modifyPlayerStat({
            stat: prop as Stat,
            value: value,
            action: 'remove'
          }));
        }
      }
    }

    // If the enchant that was clicked on is the currently selected enchant then don't add the stats since it's being removoed
    if (playerStore.selectedEnchants[slot] !== enchant.id) {
      // Add stats from new enchant
      for (const [prop, value] of Object.entries(enchant)) {
        if (playerStore.stats.hasOwnProperty(prop)) {
          dispatch(modifyPlayerStat({
            stat: prop as Stat,
            value: value,
            action: 'add'
          }));
        }
      }
    }

    dispatch(setEnchantInItemSlot({
      id: enchant.id,
      itemSlot: slot,
    }));
  } 

  function itemSlotClickHandler(slot: ItemSlotKey, subSlot: SubSlotValue) {
    setItemSlot(slot);
    setItemSubSlot(subSlot);
    localStorage.setItem('selectedItemSlot', JSON.stringify(slot));
    localStorage.setItem('selectedItemSubSlot', JSON.stringify(subSlot));
  }

  function getEnchantLookupKey(): ItemSlotKey {
    return itemSlot === ItemSlotKey.Twohand ? ItemSlotKey.Mainhand : itemSlot;
  }

  function itemSocketClickHandler(itemId: string, socketNumber: number, socketColor: SocketColor) {
    dispatch(setGemSelectionTable({
      visible: true,
      itemId: itemId,
      itemSlot: itemSlot,
      socketNumber: socketNumber,
      socketColor: socketColor,
      itemSubSlot: itemSubSlot
    }));
  }

  function removeGemFromSocket(itemId: string, socketNumber: number) {
    if (playerStore.selectedGems[itemSlot][itemId]) {
      let currentItemSocketsValue = JSON.parse(JSON.stringify(playerStore.selectedGems[itemSlot][itemId]));

      // If the gem is in an equipped item then remove the gem's stats.
      if (parseInt(itemId) === playerStore.selectedItems[ItemSlotKeyToItemSlot(false, itemSlot, itemSubSlot)]) {
        const gem = Gems.find(e => e.id === currentItemSocketsValue[socketNumber][1]);

        if (gem && gem.stats) {
          for (const [stat, value] of Object.entries(gem.stats)) {
            dispatch(modifyPlayerStat({
              stat: stat as Stat,
              value: value,
              action: 'remove'
            }));
          }
        }

        // If the socket bonus is active then remove the stats since it won't be active anymore after removing the gem
        if (itemMeetsSocketRequirements({itemId: parseInt(itemId), selectedGems: playerStore.selectedGems})) {
          for (const [stat, value] of Object.entries(Items.find(e => e.id === parseInt(itemId))!!.socketBonus!!)) {
            dispatch(modifyPlayerStat({
              stat: stat as Stat,
              value: value,
              action: 'remove'
            }));
          }
        }
      }

      if (currentItemSocketsValue[socketNumber][1] !== 0) {
        currentItemSocketsValue[socketNumber] = ['', 0];
        dispatch(setItemSocketsValue({
          itemId: itemId,
          itemSlot: itemSlot,
          value: currentItemSocketsValue
        }));
      }
    }
  }

  return(
    <div id="item-selection-container">
      <div id="profiles-and-sources">
        <Profiles />

        <fieldset id="source-filters">
          <legend>Sources</legend>
          <ul>
            {
              phases.map((phase, i) =>
                <li key={i} data-checked={uiStore.sources.phases[phase.phase] === true} className='phase-btn' onClick={() => dispatch(togglePhase(phase.phase))}>{phase.title}</li>
              )
            }
          </ul>
        </fieldset>
      </div>

      <ul id="item-slot-selection-list">
        {
          itemSlotInformation.map((slot, i) =>
            <li
              key={i}
              onClick={() => itemSlotClickHandler(slot.itemSlot, slot.subSlot)}
              data-selected={itemSlot === slot.itemSlot && (!slot.subSlot || itemSubSlot === slot.subSlot)}>
              {slot.name}
            </li>
          )
        }
      </ul>
      <button id='hide-show-items-btn'>Hide / Show Items</button>
      <button id='gem-options-button'>Fill Item Sockets</button>
      <button id='show-equipped-items'>Show Equipped Items</button>
      <div id='gem-options-window'>
        <div id='gem-options-window-replacement-options'>
          <input type="radio" name='gem-replacement-option' value='emptySockets' defaultChecked />
          <label htmlFor='emptySockets'>Fill empty sockets</label>
          <input type="radio" name='gem-replacement-option' value='allSockets' />
          <label htmlFor='allSockets'>Fill all sockets (replaces equipped gems)</label>
        </div>
        <div id='gem-options-window-item-slot'>
          <input type='radio' name='item-slot' value='currentSlot' defaultChecked />
          <label htmlFor='currentSlot'>Current item slot</label>
          <input type='radio' name='item-slot' value='allItems' />
          <label htmlFor='allItems'>All item slots</label>
        </div>
        <div id='gem-options-window-socket-selection'>
          <input type="radio" name="socket-selection" value='meta' />
          <label htmlFor='metaSockets'>Meta Sockets</label>
          <input type="radio" name="socket-selection" value='red' defaultChecked />
          <label htmlFor='redSockets'>Red Sockets</label>
          <input type="radio" name="socket-selection" value='blue' />
          <label htmlFor='blueSockets'>Blue Sockets</label>
          <input type="radio" name="socket-selection" value='yellow' />
          <label htmlFor='yellowSockets'>Yellow Sockets</label>
        </div>
        <div id='gem-options-gem-list'></div>
        <button id='gem-options-apply-button'>Apply</button>
      </div>
      <table id="item-selection-table" data-type="mainhand" className="tablesorter" data-sortlist='[[12,1]]'>
        <colgroup id="item-selection-colgroup">
          <col id="hide-item-col" style={{width: '2%'}} />
          <col style={{width: '20%'}} />
          <col style={{width: '4%'}} />
          <col style={{width: '13%'}} />
          <col style={{width: '3%'}} />
          <col style={{width: '3%'}} />
          <col style={{width: '6%'}} />
          <col style={{width: '6.5%'}} />
          <col style={{width: '5%'}} />
          <col style={{width: '3%'}} />
          <col style={{width: '3%'}} />
          <col style={{width: '3%'}} />
          <col style={{width: '8%'}} />
        </colgroup>
        <thead>
          <tr id="item-selection-header">
            <th id="header-hide-item"></th>
            <th id="header-name">Name</th>
            <th id="header-gems"></th>
            <th id="header-source">Source</th>
            <th id="header-stamina">Stam</th>
            <th id="header-intellect">Int</th>
            <th id="header-spell-power">Spell Power</th>
            <th id="header-shadow-power">Shadow Power</th>
            <th id="header-fire-power">Fire Power</th>
            <th id="header-crit">Crit</th>
            <th id="header-hit">Hit</th>
            <th id="header-haste">Haste</th>
            <th id="header-dps">DPS</th>
          </tr>
        </thead>
        <tbody aria-live='polite'>
        {
          Items.filter((e) => e.itemSlot === itemSlot).map((item, i) =>
            <tr
              key={i}
              className="item-row"
              data-selected={playerStore.selectedItems[ItemSlotKeyToItemSlot(false, itemSlot, itemSubSlot)] === item.id}
              onClick={() => itemClickHandler(item, ItemSlotKeyToItemSlot(false, itemSlot, itemSubSlot))}
              style={{display: uiStore.sources.phases[item.phase] === false ? 'none' : ''}}>
              <td className="hide-item-btn">❌</td>
              <td><a href={'https://tbc.wowhead.com/item=' + (item.displayId || item.id)} onClick={(e) => e.preventDefault()}>{item.name}</a></td>
              <td>
                <div>
                  {
                    item.sockets?.map((socket, j) =>
                      <a
                        target='_blank'
                        rel='noreferrer'
                        href={playerStore.selectedGems[itemSlot][item.id] && playerStore.selectedGems[itemSlot][item.id][j][1] !== 0 ? `https://tbc.wowhead.com/item=${playerStore.selectedGems[itemSlot][item.id][j][1]}` : ''}
                        key={j}
                        onClick={(e) => { itemSocketClickHandler(item.id.toString(), j, socket); e.preventDefault(); e.stopPropagation(); }}
                        onContextMenu={(e) => { removeGemFromSocket(item.id.toString(), j); e.preventDefault(); }}
                      >
                        <img
                          width={16}
                          height={16}
                          data-color={socket}
                          src={playerStore.selectedGems[itemSlot][item.id] && ![null, 0].includes(playerStore.selectedGems[itemSlot][item.id][j][1]) ? `${process.env.PUBLIC_URL}/img/${Gems.find(e => e.id === playerStore.selectedGems[itemSlot][item.id][j][1])?.iconName}.jpg` : `${process.env.PUBLIC_URL}/img/${Sockets.find(s => s.color === socket)!!.iconName}.jpg`}
                          alt={socket + ' socket'}
                        />
                      </a>
                    )
                  }
                </div>
              </td>
              <td>{item.source}</td>
              <td>{item.stamina}</td>
              <td>{item.intellect}</td>
              <td>{item.spellPower}</td>
              <td>{item.shadowPower}</td>
              <td>{item.firePower}</td>
              <td>{item.critRating}</td>
              <td>{item.hitRating}</td>
              <td>{item.hasteRating}</td>
              <td></td>
            </tr>
          )
        }
        </tbody>
      </table>
      {
        Enchants.filter((e) => e.itemSlot === getEnchantLookupKey()).length > 0 &&
          <table id="enchant-selection-table" data-type="mainhand">
          <colgroup id="item-selection-colgroup">
            <col style={{width: '20%'}} />
            <col style={{width: '9%'}} />
            <col style={{width: '10%'}} />
            <col style={{width: '9%'}} />
            <col style={{width: '9%'}} />
            <col style={{width: '9%'}} />
            <col style={{width: '6%'}} />
            <col style={{width: '6%'}} />
            <col style={{width: '6%'}} />
            <col style={{width: '6%'}} />
            <col style={{width: '10%'}} />
          </colgroup>
          <thead>
            <tr id="item-selection-header">
              <th id="header-enchant-name">Enchant</th>
              <th id="header-enchant-spell-power">Spell Power</th>
              <th id="header-enchant-shadow-power">Shadow Power</th>
              <th id="header-enchant-fire-power">Fire Power</th>
              <th id="header-enchant-hit-rating">Hit Rating</th>
              <th id="header-enchant-crit-rating">Crit Rating</th>
              <th id="header-enchant-stamina">Stamina</th>
              <th id="header-enchant-intellect">Intellect</th>
              <th id="header-enchant-mp5">MP5</th>
              <th id="header-enchant-spell-penetration">Spell Pen</th>
              <th id="header-enchant-dps">DPS</th>
            </tr>
          </thead>
          <tbody aria-live='polite'>
            {
              Enchants.filter((e) => e.itemSlot === getEnchantLookupKey()) != null &&
                Enchants.filter((e) => e.itemSlot === getEnchantLookupKey())!!.map((enchant, i) =>
                  <tr
                    key={i}
                    className="enchant-row"
                    data-selected={playerStore.selectedEnchants[ItemSlotKeyToItemSlot(true, itemSlot, itemSubSlot)] === enchant.id}
                    onClick={() => enchantClickHandler(enchant, ItemSlotKeyToItemSlot(true, itemSlot, itemSubSlot))}>
                    <td><a href={'https://tbc.wowhead.com/spell=' + enchant.id} onClick={(e) => e.preventDefault()}>{enchant.name}</a></td>
                    <td>{enchant.spellPower}</td>
                    <td>{enchant.shadowPower}</td>
                    <td>{enchant.firePower}</td>
                    <td>{enchant.hitRating}</td>
                    <td>{enchant.critRating}</td>
                    <td>{enchant.stamina}</td>
                    <td>{enchant.intellect}</td>
                    <td>{enchant.mp5}</td>
                    <td>{enchant.spellPenetration}</td>
                    <td></td>
                  </tr>
                )
            }
          </tbody>
        </table>
      }
    </div>
  )
}