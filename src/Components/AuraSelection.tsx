import { useDispatch, useSelector } from "react-redux";
import { Auras } from "../data/Auras";
import { RootState } from "../redux/Store";
import { modifyPlayerStat, toggleAuraSelection } from '../redux/PlayerSlice';
import { Aura, AuraGroupKey, Stat } from "../Types";
import { AuraGroups } from "../data/AuraGroups";

export default function AuraSelection() {
  const playerStore = useSelector((state: RootState) => state.player);
  const dispatch = useDispatch();

  function auraClickHandler(aura: Aura) {
    dispatch(toggleAuraSelection(aura));

    if (aura.stats) {
      for (const [stat, value] of Object.entries(aura.stats)) {
        dispatch(modifyPlayerStat({
          stat: stat as Stat,
          value: value,
          action: playerStore.auras[aura.varName] === true ? 'remove' : 'add'
        }));
      }
    }
  }

  return(
    <section id="buffs-and-debuffs-section">
      {
        AuraGroups.map((auraGroup, i) =>
          <div
            key={i}
            style={{display: auraGroup.heading !== AuraGroupKey.PetBuffs || playerStore.talents.demonicSacrifice === 0 || playerStore.settings.sacrificePet === 'no' ? '' : 'none'}}>
            <h3 id='buffs-heading'>{auraGroup.heading}</h3>
            <ul>
              {
                Auras.filter((e) => e.group === auraGroup.heading).map((aura, j) =>
                  <li
                    key={j}
                    id={aura.varName}
                    className='buffs aura'
                    data-checked={playerStore.auras[aura.varName] === true}
                    onClick={(e) => { auraClickHandler(aura); e.preventDefault() }}
                    style={{display: (!aura.forPet || (playerStore.settings.petMode === '1' && (!playerStore.talents.demonicSacrifice || playerStore.settings.sacrificePet === 'no'))) ? '' : 'none'}}>
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