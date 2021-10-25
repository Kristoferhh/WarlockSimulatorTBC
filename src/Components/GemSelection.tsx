import { useDispatch, useSelector } from "react-redux"
import { itemMeetsSocketRequirements, ItemSlotKeyToItemSlot } from "../Common";
import { GemColors } from "../Data/Gems";
import { Items } from "../Data/Items";
import { modifyPlayerStat, setItemSocketsValue } from "../Redux/PlayerSlice";
import { RootState } from "../Redux/Store"
import { favoriteGem, hideGem, setGemSelectionTable } from "../Redux/UiSlice";
import { Gem, GemColor, InitialGemSelectionTableValue, SocketColor, Stat } from "../Types";

export default function GemSelection() {
  const uiState = useSelector((state: RootState) => state.ui);
  const selectedGemsState = useSelector((state: RootState) => state.player.selectedGems);
  const selectedItemsState = useSelector((state: RootState) => state.player.selectedItems);
  const dispatch = useDispatch();

  function gemClickHandler(gem: Gem) {
    const itemIsEquipped = parseInt(uiState.gemSelectionTable.itemId) === selectedItemsState[ItemSlotKeyToItemSlot(false, uiState.gemSelectionTable.itemSlot, uiState.gemSelectionTable.itemSubSlot)];
    let currentItemSocketArray = selectedGemsState[uiState.gemSelectionTable.itemSlot][uiState.gemSelectionTable.itemId];
    
    // If the item doesn't have a socket array yet then initialize it to an array of ['', 0] sub-arrays.
    // The first element is the socket color (not gem color) and the second element is the gem id.
    if (currentItemSocketArray == null) {
      const itemSocketAmount = Items[uiState.gemSelectionTable.itemSlot].find(i => i.id === parseInt(uiState.gemSelectionTable.itemId))?.sockets?.length;
      currentItemSocketArray = Array(itemSocketAmount).fill(['', 0]);
    } else {
      currentItemSocketArray = JSON.parse(JSON.stringify(currentItemSocketArray));

      // Return if the clicked gem is the same as the already equipped gem
      if (currentItemSocketArray[uiState.gemSelectionTable.socketNumber][1] === gem.id) {
        return;
      }
    }

    // If a gem is already equipped then remove the stats from it and the socket bonus
    if (itemIsEquipped && ![null, 0].includes(currentItemSocketArray[uiState.gemSelectionTable.socketNumber][1])) {
      GemColors.forEach((gemColor) => {
        const g = gemColor.gems.find(e => e.id === currentItemSocketArray[uiState.gemSelectionTable.socketNumber][1]);

        if (g) {
          for (const itemArr of Object.values(Items)) {
            const item = itemArr.find(e => e.id === parseInt(uiState.gemSelectionTable.itemId));

            if (item) {
              for (const [stat, value] of Object.entries(item.socketBonus!!!)) {
                dispatch(modifyPlayerStat({
                  stat: stat as Stat,
                  value: value,
                  action: 'remove'
                }));
              }
            }
          }

          if (g.stats) {
            for (const [stat, value] of Object.entries(g.stats)) {
              dispatch(modifyPlayerStat({
                stat: stat as Stat,
                value: value,
                action: 'remove'
              }));
            }
          }
        }
      })
    }

    currentItemSocketArray[uiState.gemSelectionTable.socketNumber] = [uiState.gemSelectionTable.socketColor, gem.id];
    // Add stats from the gem if the item it's in is equipped
    if (gem.stats && itemIsEquipped) {
      for (const [stat, value] of Object.entries(gem.stats)) {
        dispatch(modifyPlayerStat({
          stat: stat as Stat,
          value: value,
          action: 'add'
        }));
      }
    }
    
    dispatch(setItemSocketsValue({
      itemId: uiState.gemSelectionTable.itemId,
      itemSlot: uiState.gemSelectionTable.itemSlot,
      value: currentItemSocketArray
    }));

    // Add socket bonus stats if the item meets the socket requirements
    if (itemIsEquipped && itemMeetsSocketRequirements({itemId: parseInt(uiState.gemSelectionTable.itemId), socketArray: currentItemSocketArray})) {
      for (const [stat, value] of Object.entries(Items[uiState.gemSelectionTable.itemSlot].find(e => e.id === parseInt(uiState.gemSelectionTable.itemId))!!.socketBonus!!)) {
        dispatch(modifyPlayerStat({
          stat: stat as Stat,
          value: value,
          action: 'add'
        }));
      }
    }
  }

  return(
    <table id="gem-selection-table" cellSpacing={0} data-color='none' style={{display: uiState.gemSelectionTable.visible ? '' : 'none'}} onClick={(e) => e.stopPropagation()}>
      <tbody>
        {
          GemColors.map((gemColorArray, i) =>
            // Only show the gems if they're meta gems and the socket is a meta socket or if they're not meta gems and the socket is not a meta socket.
            ((uiState.gemSelectionTable.socketColor === SocketColor.Meta && gemColorArray.color === GemColor.Meta) || (uiState.gemSelectionTable.socketColor !== SocketColor.Meta && gemColorArray.color !== GemColor.Meta)) && 
              gemColorArray.gems.map((gem, j) =>
                <tr key={j} className='gem-row' data-hidden={false}>
                  <td
                    className='gem-info gem-favorite-star'
                    title={uiState.gemPreferences.favorites.includes(gem.id) ? 'Remove gem from favorites' : 'Add gem to favorites'}
                    data-favorited={uiState.gemPreferences.favorites.includes(gem.id)}
                    onClick={(e) => dispatch(favoriteGem(gem.id))}
                  >★</td>
                  <td className='gem-info gem-name' onClick={(e) => { gemClickHandler(gem); dispatch(setGemSelectionTable(InitialGemSelectionTableValue)); e.preventDefault(); }}>
                    <img src={`${process.env.PUBLIC_URL}/img/${gem.iconName}.jpg`} alt={gem.name + ' icon'} width={20} height={20} />
                    <a href={`https://tbc.wowhead.com/item=${gem.id}`}>{gem.name}</a>
                  </td>
                  <td
                    className='gem-info gem-hide'
                    title={uiState.gemPreferences.hidden.includes(gem.id) ? 'Show Gem' : 'Hide Gem'}
                    data-hidden={uiState.gemPreferences.hidden.includes(gem.id)}
                    onClick={(e) => dispatch(hideGem(gem.id))}
                  >❌</td>
                </tr>
              )
          )
        }
      </tbody>
    </table>
  )
}