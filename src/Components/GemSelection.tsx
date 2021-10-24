import { useDispatch, useSelector } from "react-redux"
import { GemColors } from "../Data/Gems";
import { Items } from "../Data/Items";
import { setItemSocketsValue } from "../Redux/PlayerSlice";
import { RootState } from "../Redux/Store"
import { favoriteGem, hideGem, setGemSelectionTable } from "../Redux/UiSlice";
import { GemColor, InitialGemSelectionTableValue, SocketColor } from "../Types";

export default function GemSelection() {
  const uiState = useSelector((state: RootState) => state.ui);
  const selectedGemsState = useSelector((state: RootState) => state.player.selectedGems);
  const dispatch = useDispatch();

  function gemClickHandler(gemId: number, gemColor: GemColor) {
    let currentItemSocketArray = selectedGemsState[uiState.gemSelectionTable.itemSlot][uiState.gemSelectionTable.itemId];
    
    // If the item doesn't have a socket array yet then initialize it to an array of ['', 0] sub-arrays.
    // The first element is the gem color (not socket color) and the second element is the gem id.
    if (currentItemSocketArray == null) {
      const itemSocketAmount = Items[uiState.gemSelectionTable.itemSlot].find(i => i.id === parseInt(uiState.gemSelectionTable.itemId))?.sockets?.length;
      currentItemSocketArray = Array(itemSocketAmount).fill(['', 0]);
    } else {
      currentItemSocketArray = JSON.parse(JSON.stringify(currentItemSocketArray));
    }

    currentItemSocketArray[uiState.gemSelectionTable.socketNumber] = [gemColor, gemId];
    
    dispatch(setItemSocketsValue({
      itemId: uiState.gemSelectionTable.itemId,
      itemSlot: uiState.gemSelectionTable.itemSlot,
      value: currentItemSocketArray
    }));
    dispatch(setGemSelectionTable(InitialGemSelectionTableValue));
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
                  <td className='gem-info gem-name' onClick={(e) => { gemClickHandler(gem.id, gemColorArray.color); e.preventDefault(); }}>
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