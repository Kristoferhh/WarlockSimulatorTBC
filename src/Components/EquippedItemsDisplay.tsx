import { useDispatch, useSelector } from "react-redux";
import { ItemSlotToItemSlotKey } from "../Common";
import { Enchants } from "../data/Enchants";
import { Items } from "../data/Items";
import { RootState } from "../redux/Store";
import { setEquippedItemsWindowVisibility } from "../redux/UiSlice";
import { Enchant, Item, ItemSlot } from "../Types";
import ItemSocketDisplay from "./ItemSocketDisplay";


export default function EquippedItemsDisplay() {
  const uiState = useSelector((state: RootState) => state.ui);
  const playerState = useSelector((state: RootState) => state.player);
  const dispatch = useDispatch();

  function getEnchantInItemSlot(itemSlot: ItemSlot): Enchant | undefined {
    let slot = itemSlot;

    if (slot === ItemSlot.twohand) {
      slot = ItemSlot.mainhand;
    }

    return Enchants.find(e => e.id === playerState.selectedEnchants[slot]);
  }

  function getItemInItemSlot(itemSlot: ItemSlot): Item | undefined {
    return Items.find(e => e.id === playerState.selectedItems[itemSlot]);
  }

  return(
    <div id="currently-equipped-items-container" style={{display: uiState.equippedItemsWindowVisible ? '' : 'none'}}>
      <div id="currently-equipped-items">
        <div onClick={(e) => dispatch(setEquippedItemsWindowVisibility(false))}>
          <a href='#' className='close' id='currently-equipped-items-close-button'></a>
        </div>
        <table>
          <colgroup>
            <col style={{width: '13%'}} />
            <col style={{width: '45%'}} />
            <col style={{width: '10%'}} />
            <col style={{width: '32%'}} />
          </colgroup>
          <thead>
            <tr>
              <th>Slot</th>
              <th>Name</th>
              <th></th>
              <th>Enchant</th>
            </tr>
          </thead>
          <tbody>
            {
              Object.values(ItemSlot).map((slot, i) =>
                (![ItemSlot.mainhand, ItemSlot.offhand, ItemSlot.twohand].includes(slot) || (([ItemSlot.mainhand, ItemSlot.offhand].includes(slot) && (!playerState.selectedItems[ItemSlot.twohand] || playerState.selectedItems[ItemSlot.twohand] === 0)) || (slot === ItemSlot.twohand && (!playerState.selectedItems[ItemSlot.mainhand] || playerState.selectedItems[ItemSlot.mainhand] === 0) && (!playerState.selectedItems[ItemSlot.offhand] || playerState.selectedItems[ItemSlot.offhand] === 0)))) &&
                  <tr key={i} className='equipped-item-row'>
                    <td>{slot}</td>
                    <td className={'equipped-item-name ' + (playerState.selectedItems[slot] != null ? getItemInItemSlot(slot)?.quality : '')}>
                      {
                        getItemInItemSlot(slot) &&
                          <>
                            <a
                              href={'https://tbc.wowhead.com/item=' + (getItemInItemSlot(slot)!.displayId || getItemInItemSlot(slot)!.id)}
                              onClick={(e) => e.preventDefault()}
                            ></a>
                            {getItemInItemSlot(slot)?.name}
                          </>
                      }
                    </td>
                    <td>
                      {
                        (playerState.selectedItems[slot] && getItemInItemSlot(slot) !== undefined) ?
                          <ItemSocketDisplay
                            item={getItemInItemSlot(slot)!}
                            itemSlot={ItemSlotToItemSlotKey(false, slot)} />
                        : ''
                      }
                    </td>
                    <td className={'equipped-item-enchant-name ' + (getEnchantInItemSlot(slot) !== undefined ? getEnchantInItemSlot(slot)?.quality : '')}>
                      {
                        getEnchantInItemSlot(slot) !== undefined &&
                        <>
                          <a
                            href={'https://tbc.wowhead.com/spell=' + (getEnchantInItemSlot(slot)!.id)}
                            onClick={(e) => e.preventDefault()}
                          ></a>
                          {getEnchantInItemSlot(slot)!.name}
                        </>
                      }
                    </td>
                  </tr>
              )
            }
          </tbody>
        </table>
      </div>
    </div>
  )
}