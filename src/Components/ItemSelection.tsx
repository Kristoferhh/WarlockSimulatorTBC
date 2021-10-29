import { Items } from '../data/Items';
import { Enchant, Item, ItemSlot, ItemSlotKey, SocketColor, Stat, SubSlotValue } from '../Types';
import { useState } from 'react';
import { Enchants } from '../data/Enchants';
import { useDispatch, useSelector } from 'react-redux';
import { RootState } from '../redux/Store';
import { modifyPlayerStat, setEnchantInItemSlot, setItemInItemSlot, setItemSetCount, setItemSocketsValue } from '../redux/PlayerSlice';
import { itemMeetsSocketRequirements, ItemSlotKeyToItemSlot } from '../Common';
import { setEquippedItemsWindowVisibility, setFillItemSocketsWindowVisibility, setGemSelectionTable, setSelectedItemSlot, toggleHiddenItemId } from '../redux/UiSlice';
import { Gems } from '../data/Gems';
import ItemSocketDisplay from './ItemSocketDisplay';
import { FillItemSockets } from './FillItemSockets';

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

export default function ItemSelection() {
  const [itemSubSlot, setItemSubSlot] = useState<SubSlotValue>(localStorage.getItem('selectedItemSubSlot') as SubSlotValue || '1');
  const [hidingItems, setHidingItems]  = useState(false);
  const playerStore = useSelector((state: RootState) => state.player);
  const uiStore = useSelector((state: RootState) => state.ui);
  const dispatch = useDispatch();

  function unequipItemSlot(slot: ItemSlot) {
    const itemObj = Items.find(i => i.id === playerStore.selectedItems[slot]);

    if (itemObj) {
      for (const [prop, value] of Object.entries(itemObj)) {
        if (playerStore.stats.hasOwnProperty(prop)) {
          dispatch(modifyPlayerStat({
            stat: prop as Stat,
            value: value,
            action: 'remove'
          }));
        }
      }

      // Remove stats from socket bonus if it's active
      if (itemMeetsSocketRequirements({itemId: itemObj!.id, selectedGems: playerStore.selectedGems})) {
        for (const [stat, value] of Object.entries(itemObj!.socketBonus!)) {
          dispatch(modifyPlayerStat({
            stat: stat as Stat,
            value: value,
            action: 'remove'
          }));
        }
      }

      // Lower the item set counter by 1 if the item is part of a set
      if (itemObj!.setId && playerStore.sets[itemObj!.setId] && playerStore.sets[itemObj!.setId] > 0) {
        dispatch(setItemSetCount({
          setId: itemObj!.setId,
          count: playerStore.sets[itemObj!.setId] - 1
        }));
      }
    }

    dispatch(setItemInItemSlot({
      id: 0,
      itemSlot: slot,
    }));
  }

  function equipItem(item: Item, slot: ItemSlot) {
    if (playerStore.selectedItems[slot] !== item.id) {
      for (const [prop, value] of Object.entries(item)) {
        if (playerStore.stats.hasOwnProperty(prop)) {
          dispatch(modifyPlayerStat({
            stat: prop as Stat,
            value: value,
            action: 'add'
          }));
        }
      }

      if (itemMeetsSocketRequirements({itemId: item.id, selectedGems: playerStore.selectedGems})) {
        for (const [stat, value] of Object.entries(item.socketBonus!)) {
          dispatch(modifyPlayerStat({
            stat: stat as Stat,
            value: value,
            action: 'add'
          }));
        }
      }

      if (item.setId) {
        dispatch(setItemSetCount({
          setId: item.setId,
          count: playerStore.sets[item.setId] == null ? 1 : playerStore.sets[item.setId] + 1
        }));
      }
    }

    dispatch(setItemInItemSlot({
      id: item.id,
      itemSlot: slot,
    }));

    // If equipping a two handed weapon then unequip main hand and offhand and vice versa
    if (slot === ItemSlot.twohand) {
      unequipItemSlot(ItemSlot.mainhand);
      unequipItemSlot(ItemSlot.offhand);
    } else if ([ItemSlot.mainhand, ItemSlot.offhand].includes(slot)) {
      unequipItemSlot(ItemSlot.twohand);
    }
  }

  function itemClickHandler(item: Item, slot: ItemSlot) {
    // Remove stats from equipped item
    if (playerStore.selectedItems[slot] !== 0) {
      unequipItemSlot(slot);
    }

    // Add stats from the item if it's not the currently equipped item
    if (playerStore.selectedItems[slot] !== item.id) {
      equipItem(item, slot);
    }
  }

  function enchantClickHandler(enchant: Enchant, slot: ItemSlot) {
    // Remove stats from selected enchant
    if (playerStore.selectedEnchants[slot] !== 0) {
      const enchantObj = Enchants.find(i => i.id === playerStore.selectedEnchants[slot]);

      if (enchantObj) {
        for (const [prop, value] of Object.entries(enchantObj)) {
          if (playerStore.stats.hasOwnProperty(prop)) {
            dispatch(modifyPlayerStat({
              stat: prop as Stat,
              value: value,
              action: 'remove'
            }));
          }
        }
      }

      dispatch(setEnchantInItemSlot({
        id: 0,
        itemSlot: slot,
      }));
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

      dispatch(setEnchantInItemSlot({
        id: enchant.id,
        itemSlot: slot,
      }));
    }
  }

  function itemSlotClickHandler(slot: ItemSlotKey, subSlot: SubSlotValue) {
    dispatch(setSelectedItemSlot(slot));
    setItemSubSlot(subSlot);
    localStorage.setItem('selectedItemSubSlot', subSlot);
  }

  function getEnchantLookupKey(): ItemSlotKey {
    return uiStore.selectedItemSlot === ItemSlotKey.Twohand ? ItemSlotKey.Mainhand : uiStore.selectedItemSlot;
  }

  function itemSocketClickHandler(itemId: string, socketNumber: number, socketColor: SocketColor) {
    dispatch(setGemSelectionTable({
      visible: true,
      itemId: itemId,
      itemSlot: uiStore.selectedItemSlot,
      socketNumber: socketNumber,
      socketColor: socketColor,
      itemSubSlot: itemSubSlot
    }));
  }

  function removeGemFromSocket(itemId: string, socketNumber: number) {
    if (playerStore.selectedGems[uiStore.selectedItemSlot][itemId]) {
      let currentItemSocketsValue = JSON.parse(JSON.stringify(playerStore.selectedGems[uiStore.selectedItemSlot][itemId]));

      // If the gem is in an equipped item then remove the gem's stats.
      if (parseInt(itemId) === playerStore.selectedItems[ItemSlotKeyToItemSlot(false, uiStore.selectedItemSlot, itemSubSlot)]) {
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
          for (const [stat, value] of Object.entries(Items.find(e => e.id === parseInt(itemId))!.socketBonus!)) {
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
          itemSlot: uiStore.selectedItemSlot,
          value: currentItemSocketsValue
        }));
      }
    }
  }

  return(
    <div id="item-selection-container">
      <ul id="item-slot-selection-list">
        {
          itemSlotInformation.map((slot, i) =>
            <li
              key={i}
              onClick={() => itemSlotClickHandler(slot.itemSlot, slot.subSlot)}
              data-selected={uiStore.selectedItemSlot === slot.itemSlot && (!slot.subSlot || itemSubSlot === slot.subSlot)}>
              {slot.name}
            </li>
          )
        }
      </ul>
      <button id='hide-show-items-btn' onClick={(e) => setHidingItems(!hidingItems)}>Hide / Show Items</button>
      <button id='gem-options-button' onClick={(e) => dispatch(setFillItemSocketsWindowVisibility(!uiStore.fillItemSocketsWindowVisible))}>Fill Item Sockets</button>
      <button id='show-equipped-items' onClick={(e) => dispatch(setEquippedItemsWindowVisibility(!uiStore.equippedItemsWindowVisible))}>Show Equipped Items</button>
      <FillItemSockets />
      <table id="item-selection-table" data-type="mainhand" className="tablesorter" data-sortlist='[[12,1]]'>
        {
          // If no items are found by the filter then don't display the item table headers
          Items.filter((e) => e.itemSlot === uiStore.selectedItemSlot && uiStore.sources.phase[e.phase] === true).length > 0 ?
            <>
              <colgroup id="item-selection-colgroup">
                <col style={{width: '2%', display: hidingItems ? '' : 'none'}} />
                <col />
                <col style={{width: '5%'}} />
                <col style={{width: '20%'}} />
                <col style={{width: '3%'}} />
                <col style={{width: '3%'}} />
                <col style={{width: '8%'}} />
                <col style={{width: '4%'}} />
                <col style={{width: '4%'}} />
                <col style={{width: '3%'}} />
                <col style={{width: '3%'}} />
                <col style={{width: '3%'}} />
                <col style={{width: '8%'}} />
              </colgroup>
              <thead>
                <tr id="item-selection-header">
                  <th style={{display: hidingItems ? '' : 'none'}}></th>
                  <th id="header-name">Name</th>
                  <th id="header-gems"></th>
                  <th id="header-source">Source</th>
                  <th id="header-stamina">Stam</th>
                  <th id="header-intellect">Int</th>
                  <th id="header-spell-power">Spell Power</th>
                  <th id="header-shadow-power">Shadow</th>
                  <th id="header-fire-power">Fire</th>
                  <th id="header-crit">Crit</th>
                  <th id="header-hit">Hit</th>
                  <th id="header-haste">Haste</th>
                  <th id="header-dps">DPS</th>
                </tr>
              </thead>
            </>
          :
            <tbody><tr><td><h3>No items found 😱 try selecting different item sources.</h3></td></tr></tbody>
        }
        <tbody aria-live='polite'>
        {
          Items.filter((e) => e.itemSlot === uiStore.selectedItemSlot && uiStore.sources.phase[e.phase] === true && (!uiStore.hiddenItems.includes(e.id) || hidingItems)).map((item, i) =>
            // Show the item if it's not unique or if it is unique but the other item slot (ring or trinket) isn't equipped with the item
            (!item.unique || (playerStore.selectedItems[ItemSlotKeyToItemSlot(false, uiStore.selectedItemSlot, itemSubSlot === '1' ? '2' : '1')] !== item.id)) &&
              <tr
                key={i}
                className="item-row"
                data-selected={playerStore.selectedItems[ItemSlotKeyToItemSlot(false, uiStore.selectedItemSlot, itemSubSlot)] === item.id}
                data-hidden={uiStore.hiddenItems.includes(item.id)}
                onClick={() => itemClickHandler(item, ItemSlotKeyToItemSlot(false, uiStore.selectedItemSlot, itemSubSlot))}
              >
                <td
                  className="hide-item-btn"
                  style={{display: hidingItems ? 'table-cell' : 'none'}}
                  title={uiStore.hiddenItems.includes(item.id) ? 'Show Item' : 'Hide Item'}
                  onClick={(e) => { dispatch(toggleHiddenItemId(item.id)); e.stopPropagation(); }}
                >❌</td>
                <td className={item.quality + ' item-row-name'}>
                  <a
                    href={'https://tbc.wowhead.com/item=' + (item.displayId || item.id)}
                    onClick={(e) => e.preventDefault()}
                  ></a>
                  {item.name}
                </td>
                <td>
                  {
                    <ItemSocketDisplay
                      item={item}
                      itemSlot={uiStore.selectedItemSlot}
                      itemSocketClickHandler={itemSocketClickHandler}
                      removeGemFromSocket={removeGemFromSocket} />
                  }
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
              Enchants.filter((e) => e.itemSlot === getEnchantLookupKey()).map((enchant, i) =>
                <tr
                  key={i}
                  className="enchant-row"
                  data-selected={playerStore.selectedEnchants[ItemSlotKeyToItemSlot(true, uiStore.selectedItemSlot, itemSubSlot)] === enchant.id}
                  onClick={() => enchantClickHandler(enchant, ItemSlotKeyToItemSlot(true, uiStore.selectedItemSlot, itemSubSlot))}>
                  <td className={enchant.quality + ' enchant-row-name'}>
                    <a
                      href={'https://tbc.wowhead.com/spell=' + enchant.id}
                      onClick={(e) => e.preventDefault()}
                    ></a>
                    {enchant.name}
                    </td>
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