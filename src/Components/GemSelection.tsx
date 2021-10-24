import { useDispatch, useSelector } from "react-redux"
import { GemColors } from "../Data/Gems";
import { RootState } from "../Redux/Store"
import { favoriteGem, hideGem, setGemSelectionTableVisibility } from "../Redux/UiSlice";
import { Gem } from "../Types";

export default function GemSelection() {
  const uiState = useSelector((state: RootState) => state.ui);
  const selectedGemsState = useSelector((state: RootState) => state.player.selectedGems);
  const dispatch = useDispatch();

  function gemClickHandler(gem: Gem) {

    dispatch(setGemSelectionTableVisibility(false));
  }

  return(
    <table id="gem-selection-table" cellSpacing={0} data-color='none' style={{display: uiState.gemSelectionTable.visible ? '' : 'none'}} onClick={(e) => e.stopPropagation()}>
      {
        GemColors.map((gemColorArray, i) =>
          gemColorArray.gems.map((gem, j) =>
            <tr key={j} className='gem-row' data-hidden={false}>
              <td
                className='gem-info gem-favorite-star'
                title={uiState.gemPreferences.favorites.includes(gem.id) ? 'Remove gem from favorites' : 'Add gem to favorites'}
                data-favorited={uiState.gemPreferences.favorites.includes(gem.id)}
                onClick={(e) => dispatch(favoriteGem(gem.id))}
              >★</td>
              <td className='gem-info gem-name' onClick={(e) => { gemClickHandler(gem); e.preventDefault(); }}>
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
    </table>
  )
}