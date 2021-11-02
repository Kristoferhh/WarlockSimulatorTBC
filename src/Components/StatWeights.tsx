import { nanoid } from "nanoid";
import { useDispatch, useSelector } from "react-redux"
import { RootState } from "../redux/Store"
import { setStatWeightVisibility } from "../redux/UiSlice";
import { Stat } from "../Types";

const statDisplay: {displayName: string, stat: Stat}[] = [
  { displayName: 'Stamina', stat: Stat.stamina },
  { displayName: 'Intellect', stat: Stat.intellect },
  { displayName: 'Spirit', stat: Stat.spirit },
  { displayName: 'Spell Power', stat: Stat.spellPower },
  { displayName: 'Shadow Power', stat: Stat.shadowPower },
  { displayName: 'Fire Power', stat: Stat.firePower },
  { displayName: 'Hit Rating', stat: Stat.hitRating },
  { displayName: 'Crit Rating', stat: Stat.critRating },
  { displayName: 'Haste Rating', stat: Stat.hasteRating },
  { displayName: 'Mp5', stat: Stat.mp5 },
]

export default function StatWeights() {
  const statWeightState = useSelector((state: RootState) => state.ui.statWeights);
  const dispatch = useDispatch();

  return(
    <section id="stat-weights-section" style={{display: statWeightState.visible ? '' : 'none'}}>
      <div>
        <h2 style={{display: 'inline-block'}}>Stat Weights</h2>
        <a href='#' className='close' id='stat-weight-close-button' onClick={(e) => { dispatch(setStatWeightVisibility(false)); e.preventDefault() }}></a>
      </div>
      <ul className="character-stats" id="stat-weights">
        {
          statDisplay.map(stat =>
            <li key={nanoid()}>
              <p className='character-stat'>{stat.displayName}</p>
              <p id={`stat-weight-${stat.stat}`} className='character-stat-val'>0</p>
            </li>
          )
        }
        <li id="pawn-import-string">
          <p></p>
        </li>
      </ul>
    </section>
  )
}