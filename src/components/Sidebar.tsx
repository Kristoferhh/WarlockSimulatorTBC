import { nanoid } from "nanoid";
import { useTranslation } from "react-i18next";
import { useSelector } from "react-redux";
import { getBaseWowheadUrl } from "../Common";
import { Races } from "../data/Races";
import { Sets } from "../data/Sets";
import i18n from "../i18n/config";
import { RootState } from "../redux/Store";
import { SimulationButtons } from "./SimulationButtons";
import StatsDisplay from "./StatsDisplay";

function setHasActiveBonus(set: [string, number]): boolean {
  const setObj = Sets.find(e => e.setId === set[0]);
  if (setObj) {
    return set[1] >= setObj.bonuses[0];
  }
  return false;
}

export default function Sidebar() {
  const playerState = useSelector((state: RootState) => state.player);
  const { t } = useTranslation();

  return (
    <div id="sidebar">
      <section id="character-section">
        <p id="character-race">
          <span id="race">
            {t(Races.find(e => e.varName === playerState.settings.race)!.name)}
          </span> {t('Warlock')}
        </p>
        <p id="character-level">{t('Level')} <span>70</span></p>
        <StatsDisplay />
        <ul id="sidebar-sets">
          {
            Object.entries(playerState.sets).find(set => setHasActiveBonus(set)) &&
            <li><h3>{t('Set Bonuses')}</h3></li>
          }
          {
            Object.entries(playerState.sets)
              .filter(set => setHasActiveBonus(set))
              .map(set =>
                <li key={nanoid()} className="sidebar-set-bonus">
                  <a
                    target='_blank'
                    rel='noreferrer'
                    href={
                      `${getBaseWowheadUrl(i18n.language)}/item-set=${Sets.find(e => e.setId === set[0])?.setId}`
                    }
                    className={Sets.find(e => e.setId === set[0])?.quality}
                  >{t(Sets.find(e => e.setId === set[0])!.name)} ({set[1]})</a>
                </li>
              )
          }
        </ul>

        <div id="sidebar-simulation-selection">
          <SimulationButtons />
          <p id="guybrush-note">
            This sim was heavily inspired by <a rel="noreferrer" target="_blank" href='https://guybrushgit.github.io/WarriorSim/'>Guybrush's 1.13 Warrior Simulator</a>
          </p>
          <p id="paypal-note">
            <a href="https://www.paypal.com/paypalme/kristoferhh" rel="noreferrer" target="_blank">
              Support me via PayPal
            </a>
          </p>
        </div>
      </section>
    </div>
  )
}