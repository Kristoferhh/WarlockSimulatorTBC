import { useDispatch, useSelector } from "react-redux";
import { Auras } from "../Data/Auras";
import { RootState } from "../Store";
import { modifyPlayerStat, toggleAuraSelection } from '../PlayerSlice';
import { Aura, AuraGroup, Stat } from "../Types";

export default function AuraSelection() {
  const auraStore = useSelector((state: RootState) => state.player.auras);
  const dispatch = useDispatch();

  function auraClickHandler(auraGroup: AuraGroup, aura: Aura) {
    dispatch(toggleAuraSelection({auraGroup: auraGroup, aura: aura}));

    if (aura.stats) {
      for (const [stat, value] of Object.entries(aura.stats)) {
        dispatch(modifyPlayerStat({
          stat: stat as Stat,
          value: value,
          action: auraStore[aura.varName] === true ? 'remove' : 'add'
        }));
      }
    }
  }

  return(
    <section id="buffs-and-debuffs-section">
      {
        Auras.map((auraGroup, i) =>
          <div key={i}>
            <h3 id='buffs-heading'>{auraGroup.heading}</h3>
            <ul>
              {
                auraGroup.auras.map((aura, j) =>
                  <li
                    key={j}
                    id={aura.varName}
                    className='buffs aura'
                    data-checked={auraStore[aura.varName] === true}
                    onClick={(e) => { auraClickHandler(auraGroup, aura); e.preventDefault() }}>
                    <a href={`https://tbc.wowhead.com/${auraGroup.type}=${aura.id}`}>
                      <img src={`${process.env.PUBLIC_URL}/img/${aura.iconName}.jpg`} alt={aura.name} />
                    </a>
                  </li>
                )
              }
            </ul>
          </div>
        )
      }
    </section>
  )
}