import { useDispatch, useSelector } from "react-redux";
import { Auras } from "../data/Auras";
import { RootState } from "../redux/Store";
import { Aura, AuraGroup, AurasStruct } from "../Types";
import { AuraGroups } from "../data/AuraGroups";
import { setAurasStats, setSelectedAuras } from "../redux/PlayerSlice";
import { nanoid } from "nanoid";
import { getAurasStats, getBaseWowheadUrl } from "../Common";
import { useTranslation } from "react-i18next";
import i18n from "../i18n/config";

function disableAurasWithUniqueProperties(aura: Aura, auraObj: AurasStruct): void {
  if (aura.potion)         Auras.filter((e) => e.potion).forEach((e)         => auraObj[e.varName] = false);
  if (aura.foodBuff)       Auras.filter((e) => e.foodBuff).forEach((e)       => auraObj[e.varName] = false);
  if (aura.weaponOil)      Auras.filter((e) => e.weaponOil).forEach((e)      => auraObj[e.varName] = false);
  if (aura.battleElixir)   Auras.filter((e) => e.battleElixir).forEach((e)   => auraObj[e.varName] = false);
  if (aura.guardianElixir) Auras.filter((e) => e.guardianElixir).forEach((e) => auraObj[e.varName] = false);
  if (aura.alcohol)        Auras.filter((e) => e.alcohol).forEach((e)        => auraObj[e.varName] = false);
  if (aura.demonicRune)    Auras.filter((e) => e.demonicRune).forEach((e)    => auraObj[e.varName] = false);
  if (aura.drums)          Auras.filter((e) => e.drums).forEach((e)          => auraObj[e.varName] = false);
}

export default function AuraSelection() {
  const playerState = useSelector((state: RootState) => state.player);
  const dispatch = useDispatch();
  const { t } = useTranslation();

  function auraClickHandler(aura: Aura): void {
    let newAuras = JSON.parse(JSON.stringify(playerState.auras));
    const isAuraDisabled = newAuras[aura.varName] == null || newAuras[aura.varName] === false;

    // If the aura is being toggled on and it's a unique buff like potion/food buff, then disable all other auras with that unique property as true.
    if (isAuraDisabled) { disableAurasWithUniqueProperties(aura, newAuras); }
    // Toggle the aura's bool property/initialize to true.
    newAuras[aura.varName] = newAuras[aura.varName] == null ? true : !newAuras[aura.varName];

    dispatch(setSelectedAuras(newAuras));
    dispatch(setAurasStats(getAurasStats(newAuras)));
  }

  return(
    <section id="buffs-and-debuffs-section">
      {
        AuraGroups.map(auraGroup =>
          <div
            key={nanoid()}
            style={{display: auraGroup.heading !== AuraGroup.PetBuffs || playerState.talents.demonicSacrifice === 0 || playerState.settings.sacrificePet === 'no' ? '' : 'none'}}>
            <h3 id='buffs-heading'>{t(auraGroup.heading)}</h3>
            <ul>
              {
                Auras.filter((e) => e.group === auraGroup.heading).map(aura =>
                  <li
                    key={nanoid()}
                    id={aura.varName}
                    className='buffs aura'
                    data-checked={playerState.auras[aura.varName] === true}
                    onClick={(e) => { auraClickHandler(aura); e.preventDefault() }}
                    style={{display: (!aura.forPet || (playerState.settings.petMode === '1' && (!playerState.talents.demonicSacrifice || playerState.settings.sacrificePet === 'no'))) ? '' : 'none'}}>
                    <a href={`${getBaseWowheadUrl(i18n.language)}/${auraGroup.type}=${aura.id}`}>
                      <img src={`${process.env.PUBLIC_URL}/img/${aura.iconName}.jpg`} alt={t(aura.name)} />
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