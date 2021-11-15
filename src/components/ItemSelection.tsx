import { Items } from '../data/Items';
import { Enchant, Item, ItemSlot, ItemSlotKey, SocketColor, SubSlotValue } from '../Types';
import { useState } from 'react';
import { Enchants } from '../data/Enchants';
import { useDispatch, useSelector } from 'react-redux';
import { RootState } from '../redux/Store';
import { setEnchantsStats, setGemsStats, setItemSetCounts, setItemsStats, setSelectedEnchants, setSelectedGems, setSelectedItems } from '../redux/PlayerSlice';
import { getBaseWowheadUrl, getEnchantsStats, getGemsStats, getItemSetCounts, getItemsStats, getItemTableItems, ItemSlotKeyToItemSlot } from '../Common';
import { setEquippedItemsWindowVisibility, setFillItemSocketsWindowVisibility, setGemSelectionTable, setSelectedItemSlot, setSelectedItemSubSlot, toggleHiddenItemId } from '../redux/UiSlice';
import ItemSocketDisplay from './ItemSocketDisplay';
import { FillItemSockets } from './FillItemSockets';
import { nanoid } from '@reduxjs/toolkit';
import { useTranslation } from 'react-i18next';
import i18n from '../i18n/config';

const itemSlotInformation: { name: string, itemSlot: ItemSlotKey, subSlot: SubSlotValue }[] = [
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
  { name: 'Wand', itemSlot: ItemSlotKey.Wand, subSlot: '' }
];

export default function ItemSelection() {
  const [hidingItems, setHidingItems] = useState(false);
  const playerStore = useSelector((state: RootState) => state.player);
  const uiStore = useSelector((state: RootState) => state.ui);
  const dispatch = useDispatch();
  const { t } = useTranslation();

  function changeEquippedItemId(itemSlot: ItemSlot, itemId: number) {
    if (playerStore.selectedItems[itemSlot] === itemId) { return; }
    let newSelectedItems = JSON.parse(JSON.stringify(playerStore.selectedItems));
    newSelectedItems[itemSlot] = itemId;

    // If we're not unequipping an item
    if (itemId !== 0) {
      // If equipping a two handed weapon then unequip main hand and offhand and vice versa
      if (itemSlot === ItemSlot.twohand) {
        newSelectedItems[ItemSlot.mainhand] = 0;
        newSelectedItems[ItemSlot.offhand] = 0;
      } else if ([ItemSlot.mainhand, ItemSlot.offhand].includes(itemSlot)) {
        newSelectedItems[ItemSlot.twohand] = 0;
      }
    }

    dispatch(setSelectedItems(newSelectedItems));
    dispatch(setItemsStats(getItemsStats(newSelectedItems)));
    dispatch(setGemsStats(getGemsStats(newSelectedItems, playerStore.selectedGems)));
    dispatch(setEnchantsStats(getEnchantsStats(newSelectedItems, playerStore.selectedEnchants)));
    dispatch(setItemSetCounts(getItemSetCounts(newSelectedItems)));
  }

  function itemClickHandler(item: Item, itemSlot: ItemSlot) {
    changeEquippedItemId(itemSlot, playerStore.selectedItems[itemSlot] === item.id ? 0 : item.id);
  }

  function enchantClickHandler(enchant: Enchant, itemSlot: ItemSlot) {
    let newSelectedEnchants = JSON.parse(JSON.stringify(playerStore.selectedEnchants));
    newSelectedEnchants[itemSlot] = newSelectedEnchants[itemSlot] === enchant.id ? 0 : enchant.id;
    dispatch(setSelectedEnchants(newSelectedEnchants));
    dispatch(setEnchantsStats(getEnchantsStats(playerStore.selectedItems, newSelectedEnchants)));
  }

  function itemSlotClickHandler(slot: ItemSlotKey, subSlot: SubSlotValue) {
    dispatch(setSelectedItemSlot(slot));
    dispatch(setSelectedItemSubSlot(subSlot));
  }

  function getEnchantLookupKey(itemSlot: ItemSlotKey): ItemSlotKey {
    return itemSlot === ItemSlotKey.Twohand ? ItemSlotKey.Mainhand : itemSlot;
  }

  function itemSocketClickHandler(itemId: string, socketNumber: number, socketColor: SocketColor) {
    dispatch(setGemSelectionTable({
      visible: true,
      itemId: itemId,
      itemSlot: uiStore.selectedItemSlot,
      socketNumber: socketNumber,
      socketColor: socketColor,
      itemSubSlot: uiStore.selectedItemSubSlot
    }));
  }

  function removeGemFromSocket(itemId: string, socketNumber: number) {
    let newSelectedGems = JSON.parse(JSON.stringify(playerStore.selectedGems));
    if (newSelectedGems[uiStore.selectedItemSlot][itemId]) {
      let currentItemSocketsValue = newSelectedGems[uiStore.selectedItemSlot][itemId];
      if (currentItemSocketsValue[socketNumber][1] !== 0) {
        currentItemSocketsValue[socketNumber] = ['', 0];
        dispatch(setSelectedGems(newSelectedGems));
        dispatch(setGemsStats(getGemsStats(playerStore.selectedItems, newSelectedGems)));
      }
    }
  }

  function shouldDisplayMissingEnchantWarning(itemSlot: ItemSlotKey, subSlot: SubSlotValue): boolean {
    const equippedEnchantId =
      playerStore.selectedEnchants[ItemSlotKeyToItemSlot(true, itemSlot, subSlot)];

    if (itemSlot === ItemSlotKey.Ring ||
      Enchants.find(e => e.itemSlot === getEnchantLookupKey(itemSlot)) == null) {
      return false;
    }

    // Checks if the user has an item equipped in the slot but no enchant.
    return (!equippedEnchantId || [null, 0].includes(equippedEnchantId)) &&
      ([0, null].includes(playerStore.selectedItems[ItemSlotKeyToItemSlot(false, itemSlot, subSlot)]) === false);
  }

  return (
    <div id="item-selection-container">
      <ul id="item-slot-selection-list">
        {
          itemSlotInformation.map(slot =>
            <li
              key={nanoid()}
            >
              <p
                style={{ display: 'inline-block' }}
                onClick={() => itemSlotClickHandler(slot.itemSlot, slot.subSlot)}
                data-selected={
                  uiStore.selectedItemSlot === slot.itemSlot &&
                  (!slot.subSlot || uiStore.selectedItemSubSlot === slot.subSlot)
                }
              >{t(slot.name)}</p>
              {
                shouldDisplayMissingEnchantWarning(slot.itemSlot, slot.subSlot) &&
                <i
                  title='Missing enchant!'
                  className="fas fa-exclamation-triangle"
                  style={{ marginLeft: '2px' }}
                ></i>
              }
            </li>
          )
        }
      </ul>
      <button
        className='btn btn-primary btn-sm'
        onClick={(e) => setHidingItems(!hidingItems)}
      >{t('Hide / Show Items')}</button>
      {' '}
      <button
        className='btn btn-primary btn-sm'
        onClick={(e) => dispatch(setFillItemSocketsWindowVisibility(!uiStore.fillItemSocketsWindowVisible))}
      >{t('Fill Item Sockets')}</button>
      {' '}
      <button
        className='btn btn-primary btn-sm'
        onClick={(e) => dispatch(setEquippedItemsWindowVisibility(!uiStore.equippedItemsWindowVisible))}
      >{t('Show Equipped Items')}</button>
      <FillItemSockets />
      <table id="item-selection-table" data-type="mainhand" className="tablesorter" data-sortlist='[[12,1]]'>
        {
          // If no items are found by the filter then don't display the item table headers
          Items
            .find((e) => e.itemSlot === uiStore.selectedItemSlot && uiStore.sources.phase[e.phase] === true) != null ?
            <>
              <colgroup id="item-selection-colgroup">
                <col style={{ width: '2%', display: hidingItems ? '' : 'none' }} />
                <col />
                <col style={{ width: '5%' }} />
                <col style={{ width: '20%' }} />
                <col style={{ width: '3%' }} />
                <col style={{ width: '3%' }} />
                <col style={{ width: '8%' }} />
                <col style={{ width: '4%' }} />
                <col style={{ width: '4%' }} />
                <col style={{ width: '3%' }} />
                <col style={{ width: '3%' }} />
                <col style={{ width: '3%' }} />
                <col style={{ width: '8%' }} />
              </colgroup>
              <thead>
                <tr id="item-selection-header">
                  <th style={{ display: hidingItems ? '' : 'none' }}></th>
                  <th id="header-name">{t('Name')}</th>
                  <th id="header-gems"></th>
                  <th id="header-source">{t('Source')}</th>
                  <th id="header-stamina">{t('Stam')}</th>
                  <th id="header-intellect">{t('Int')}</th>
                  <th id="header-spell-power">{t('Spell Power')}</th>
                  <th id="header-shadow-power">{t('Shadow')}</th>
                  <th id="header-fire-power">{t('Fire')}</th>
                  <th id="header-crit">{t('Crit')}</th>
                  <th id="header-hit">{t('Hit')}</th>
                  <th id="header-haste">{t('Haste')}</th>
                  <th id="header-dps">{t('DPS')}</th>
                </tr>
              </thead>
            </>
            :
            <tbody><tr><td><h3>No items found 😱 try selecting different item sources.</h3></td></tr></tbody>
        }
        <tbody aria-live='polite'>
          {
            getItemTableItems(uiStore.selectedItemSlot,
              uiStore.selectedItemSubSlot,
              playerStore.selectedItems,
              uiStore.sources,
              uiStore.hiddenItems,
              hidingItems,
              uiStore.savedItemDps)
              .map(item =>
                <tr
                  key={item.id}
                  className='item-row'
                  data-selected={
                    playerStore
                      .selectedItems[ItemSlotKeyToItemSlot(false, uiStore.selectedItemSlot, uiStore.selectedItemSubSlot)] === item.id
                  }
                  data-hidden={uiStore.hiddenItems.includes(item.id)}
                  onClick={() =>
                    itemClickHandler(item, ItemSlotKeyToItemSlot(false, uiStore.selectedItemSlot, uiStore.selectedItemSubSlot))}
                >
                  <td
                    className="hide-item-btn"
                    style={{ display: hidingItems ? 'table-cell' : 'none' }}
                    title={uiStore.hiddenItems.includes(item.id) ? 'Show Item' : 'Hide Item'}
                    onClick={(e) => { dispatch(toggleHiddenItemId(item.id)); e.stopPropagation(); }}
                  >❌</td>
                  <td className={item.quality + ' item-row-name'}>
                    <a
                      href={`${getBaseWowheadUrl(i18n.language)}/item=${(item.displayId || item.id)}`}
                      onClick={(e) => e.preventDefault()}
                      style={{ fontSize: '0px' }}
                    >.</a>
                    <img
                      src={`${process.env.PUBLIC_URL}/img/${item.iconName}.jpg`}
                      alt={t(item.name)}
                      className='item-icon'
                    />
                    {t(item.name)}
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
                  <td>{t(item.source)}</td>
                  <td>{item.stamina}</td>
                  <td>{item.intellect}</td>
                  <td>{item.spellPower}</td>
                  <td>{item.shadowPower}</td>
                  <td>{item.firePower}</td>
                  <td>{item.critRating}</td>
                  <td>{item.hitRating}</td>
                  <td>{item.hasteRating}</td>
                  <td id={item.id.toString()}>
                    {
                      uiStore.savedItemDps[ItemSlotKeyToItemSlot(false, uiStore.selectedItemSlot, uiStore.selectedItemSubSlot)] &&
                        uiStore.savedItemDps[ItemSlotKeyToItemSlot(false, uiStore.selectedItemSlot, uiStore.selectedItemSubSlot)][item.id] ?
                        (Math.round(uiStore.savedItemDps[ItemSlotKeyToItemSlot(false, uiStore.selectedItemSlot, uiStore.selectedItemSubSlot)][item.id] * 100) / 100).toString() : ''
                    }
                  </td>
                </tr>
              )
          }
        </tbody>
      </table>
      {
        Enchants.filter((e) => e.itemSlot === getEnchantLookupKey(uiStore.selectedItemSlot)).length > 0 &&
        <table id="enchant-selection-table" data-type="mainhand">
          <colgroup id="enchant-selection-colgroup">
            <col style={{ width: '20%' }} />
            <col style={{ width: '9%' }} />
            <col style={{ width: '10%' }} />
            <col style={{ width: '9%' }} />
            <col style={{ width: '9%' }} />
            <col style={{ width: '9%' }} />
            <col style={{ width: '6%' }} />
            <col style={{ width: '6%' }} />
            <col style={{ width: '6%' }} />
            <col style={{ width: '6%' }} />
            <col style={{ width: '10%' }} />
          </colgroup>
          <thead>
            <tr id="enchant-selection-header">
              <th id="header-enchant-name">{t('Enchant')}</th>
              <th id="header-enchant-spell-power">{t('Spell Power')}</th>
              <th id="header-enchant-shadow-power">{t('Shadow Power')}</th>
              <th id="header-enchant-fire-power">{t('Fire Power')}</th>
              <th id="header-enchant-hit-rating">{t('Hit Rating')}</th>
              <th id="header-enchant-crit-rating">{t('Crit Rating')}</th>
              <th id="header-enchant-stamina">{t('Stamina')}</th>
              <th id="header-enchant-intellect">{t('Intellect')}</th>
              <th id="header-enchant-mp5">{t('MP5')}</th>
              <th id="header-enchant-spell-penetration">{t('Spell Pen')}</th>
              <th id="header-enchant-dps">{t('DPS')}</th>
            </tr>
          </thead>
          <tbody aria-live='polite'>
            {
              Enchants.filter((e) => e.itemSlot === getEnchantLookupKey(uiStore.selectedItemSlot)).map(enchant =>
                <tr
                  key={enchant.id}
                  className="enchant-row"
                  data-selected={
                    playerStore
                      .selectedEnchants[ItemSlotKeyToItemSlot(true, uiStore.selectedItemSlot, uiStore.selectedItemSubSlot)] === enchant.id
                  }
                  onClick={() =>
                    enchantClickHandler(enchant, ItemSlotKeyToItemSlot(true, uiStore.selectedItemSlot, uiStore.selectedItemSubSlot))}
                >
                  <td className={enchant.quality + ' enchant-row-name'}>
                    <a
                      href={`${getBaseWowheadUrl(i18n.language)}/spell=${enchant.id}`}
                      onClick={(e) => e.preventDefault()}
                      style={{ fontSize: '0px' }}
                    >.</a>
                    {t(enchant.name)}
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