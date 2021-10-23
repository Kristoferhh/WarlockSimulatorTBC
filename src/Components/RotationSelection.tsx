import { useDispatch, useSelector } from "react-redux"
import { Rotation } from "../Data/Rotation"
import { toggleRotationSpellSelection } from "../PlayerSlice";
import { RootState } from "../Store"
import { RotationGroup, Spell } from "../Types";

export default function RotationSelection() {
  const rotationStore = useSelector((state: RootState) => state.player.rotation);
  const dispatch = useDispatch();

  function spellClickHandler(rotationGroup: RotationGroup, spell: Spell) {
    // If the user is enabling a filler or curse then disable all other fillers/curses
    if (rotationStore[rotationGroup.varName] && rotationStore[rotationGroup.varName][spell.varName] === false && ['filler', 'curse'].includes(rotationGroup.varName)) {
      rotationGroup.spells.forEach((e) => dispatch(toggleRotationSpellSelection({
        rotationGroup: rotationGroup.varName,
        spellName: e.varName,
        forceFalse: true
      })));
    }
    dispatch(toggleRotationSpellSelection({rotationGroup: rotationGroup.varName, spellName: spell.varName}));
  }

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
                    onClick={(e) => { spellClickHandler(rotationGroup, spell); e.preventDefault(); }}>
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