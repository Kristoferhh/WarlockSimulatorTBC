import { nanoid } from "nanoid";
import { useTranslation } from "react-i18next";
import { useDispatch, useSelector } from "react-redux"
import { getBaseWowheadUrl } from "../Common";
import { Spells } from "../data/Spells";
import i18n from "../i18n/config";
import { toggleRotationSpellSelection } from "../redux/PlayerSlice";
import { RootState } from "../redux/Store"
import { RotationGroup, rotationGroups } from "../Types";

export default function RotationSelection() {
  const playerStore = useSelector((state: RootState) => state.player);
  const dispatch = useDispatch();
  const { t } = useTranslation();

  return (
    <section id="rotation-section">
      <h3>{t('Rotation')}</h3>
      <ul id="rotation-list">
        {
          rotationGroups.map(group =>
            <li
              key={nanoid()}
              style={{
                display: group.header !== RotationGroup.Curse &&
                  playerStore.settings.rotationOption === 'simChooses' ? 'none' : ''
              }}
            >
              <h4>{t(group.header)}</h4>
              {
                Spells
                  .filter(s => s.group === group.header)
                  .map(spell =>
                    <div
                      key={nanoid()}
                      data-checked={
                        (playerStore.rotation[group.varName] &&
                          playerStore.rotation[group.varName][spell.varName] === true) || false
                      }
                      onClick={(e) => {
                        dispatch(toggleRotationSpellSelection(spell));
                        e.preventDefault();
                      }}>
                      <a href={`${getBaseWowheadUrl(i18n.language)}/spell=${spell.id}`}>
                        <img
                          src={`${process.env.PUBLIC_URL}/img/${spell.iconName}.jpg`}
                          alt={t(spell.name)}
                        />
                      </a>
                    </div>
                  )
              }
            </li>
          )
        }
      </ul>
    </section>
  )
}