import { Items } from '../Data/Items';
import { ItemSlot, SubSlotValue } from '../Types';
import { useState } from 'react';
import { Enchants } from '../Data/Enchants';
import { useDispatch, useSelector } from 'react-redux';
import { RootState } from '../Store';
import { setEnchantInItemSlot, setItemInItemSlot } from '../PlayerSlice';

const itemSlotInformation: {name: string, itemSlot: ItemSlot, subSlot: SubSlotValue}[] = [
  { name: 'Main Hand', itemSlot: ItemSlot.Mainhand, subSlot: '' },
  { name: 'Off Hand', itemSlot: ItemSlot.Offhand, subSlot: '' },
  { name: 'Two Hand', itemSlot: ItemSlot.Twohand, subSlot: '' },
  { name: 'Head', itemSlot: ItemSlot.Head, subSlot: '' },
  { name: 'Neck', itemSlot: ItemSlot.Neck, subSlot: '' },
  { name: 'Shoulders', itemSlot: ItemSlot.Shoulders, subSlot: '' },
  { name: 'Back', itemSlot: ItemSlot.Back, subSlot: '' },
  { name: 'Chest', itemSlot: ItemSlot.Chest, subSlot: '' },
  { name: 'Bracer', itemSlot: ItemSlot.Bracer, subSlot: '' },
  { name: 'Gloves', itemSlot: ItemSlot.Gloves, subSlot: '' },
  { name: 'Belt', itemSlot: ItemSlot.Belt, subSlot: '' },
  { name: 'Legs', itemSlot: ItemSlot.Legs, subSlot: '' },
  { name: 'Boots', itemSlot: ItemSlot.Boots, subSlot: '' },
  { name: 'Ring 1', itemSlot: ItemSlot.Ring, subSlot: '1' },
  { name: 'Ring 2', itemSlot: ItemSlot.Ring, subSlot: '2' },
  { name: 'Trinket 1', itemSlot: ItemSlot.Trinket, subSlot: '1' },
  { name: 'Trinket 2', itemSlot: ItemSlot.Trinket, subSlot: '2' },
  { name: 'Wand', itemSlot: ItemSlot.Wand, subSlot: ''}
];

export default function ItemSelection() {
  const [itemSlot, setItemSlot] = useState<ItemSlot>(JSON.parse(localStorage.getItem('selectedItemSlot') || JSON.stringify('mainhand')));
  const [itemSubSlot, setItemSubSlot] = useState<SubSlotValue>(JSON.parse(localStorage.getItem('selectedItemSubSlot') || JSON.stringify('1')));
  const itemStore = useSelector((state: RootState) => state.player.selectedItems);
  const enchantStore = useSelector((state: RootState) => state.player.selectedEnchants);
  const dispatch = useDispatch();

  function itemClickHandler(id: number, slot: ItemSlot, subSlot: SubSlotValue) {
    dispatch(setItemInItemSlot({
      id: id,
      itemSlot: slot,
      subSlot: subSlot
    }));
  }

  function enchantClickHandler(id: number, slot: ItemSlot, subSlot: SubSlotValue) {
    dispatch(setEnchantInItemSlot({
      id: id,
      itemSlot: slot,
      subSlot: subSlot
    }));
  } 

  function itemSlotClickHandler(slot: ItemSlot, subSlot: SubSlotValue) {
    setItemSlot(slot);
    setItemSubSlot(subSlot);
    localStorage.setItem('selectedItemSlot', JSON.stringify(slot));
    localStorage.setItem('selectedItemSubSlot', JSON.stringify(subSlot));
  }

  function getEnchantLookupKey(): ItemSlot {
    return itemSlot === ItemSlot.Twohand ? ItemSlot.Mainhand : itemSlot;
  }

  return(
    <div id="item-selection-container">
      <div id="profiles-and-sources">
        <fieldset id="profile-fieldset">
          <legend>Profile Options</legend>
          <input placeholder='E.g. "P3 Shadow BiS"' type="text" name="profileName" />
          <button id="save-new-profile-button">Save New Profile</button>
          <div id="update-profile-div">
            <button style={{display: 'none'}} id="save-profile-button">Save</button>
            <button style={{display: 'none'}} id="delete-profile-button">Delete</button>
            <button style={{display: 'none'}} id="rename-profile-button">Rename</button>
            <button id="import-export-button">Import/Export</button>
          </div>
        </fieldset>

        <fieldset id="saved-profiles" style={{display: 'none'}}>
          <legend>Saved Profiles</legend>
          <ul></ul>
        </fieldset>

        <fieldset id="source-filters">
          <legend>Sources</legend>
          <ul>
            <li data-checked='false' data-source='phase' data-value='0'>Classic</li>
            <li data-checked='false' data-source='phase' data-value='1'>P1</li>
            <li data-checked='false' data-source='phase' data-value='2'>P2</li>
            <li data-checked='false' data-source='phase' data-value='3'>P3</li>
            <li data-checked='false' data-source='phase' data-value='4'>P4</li>
            <li data-checked='false' data-source='phase' data-value='5'>P5</li>
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
          <col style={{width: '13'}} />
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
          Items[itemSlot].map((item, i) =>
            <tr
              key={i}
              className="item-row"
              data-selected={itemStore[itemSlot + itemSubSlot] === item.id}
              onClick={() => itemClickHandler(item.id, itemSlot, itemSubSlot)}>
              <td className="hide-item-btn">❌</td>
              <td><a href={'https://tbc.wowhead.com/item=' + (item.displayId || item.id)} onClick={(e) => e.preventDefault()}>{item.name}</a></td>
              <td></td>
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
        Enchants[getEnchantLookupKey()] != null &&
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
              Enchants[getEnchantLookupKey()] != null &&
                Enchants[getEnchantLookupKey()]!!.map((enchant, i) =>
                  <tr
                    key={i}
                    className="enchant-row"
                    data-selected={enchantStore[getEnchantLookupKey() + itemSubSlot] === enchant.id}
                    onClick={() => enchantClickHandler(enchant.id, getEnchantLookupKey(), itemSubSlot)}>
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