import { useDispatch, useSelector } from "react-redux"
import { Rotation } from "../Data/Rotation"
import { toggleRotationSpellSelection } from "../PlayerSlice";
import { RootState } from "../Store"

export default function RotationSelection() {
  const rotationStore = useSelector((state: RootState) => state.player.rotation);
  const dispatch = useDispatch();

  return(
    <section id="rotation-section">
      <h3>Rotation</h3>
      <ul id="rotation-list">
        {
          Rotation.map((rotationGroup, i) =>
            <div key={i}>
              <h4>{rotationGroup.header}</h4>
              {
                rotationGroup.spells.map((spell, j) =>
                  <li
                    key={j}
                    data-checked={(rotationStore[rotationGroup.varName] && rotationStore[rotationGroup.varName][spell.varName] === true) || false}
                    onClick={(e) => { dispatch(toggleRotationSpellSelection({rotationGroup: rotationGroup, spell: spell})); e.preventDefault(); }}>
                    <a href={`https://tbc.wowhead.com/spell=${spell.id}`}>
                      <img src={`${process.env.PUBLIC_URL}/img/${spell.iconName}.jpg`} alt={spell.name} />
                    </a>
                  </li>
                )
              }
            </div>
          )
        }
      </ul>
    </section>
  )
}