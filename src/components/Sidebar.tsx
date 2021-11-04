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

export default function Sidebar() {
  const playerState = useSelector((state: RootState) => state.player);
  const { t } = useTranslation();

  return (
    <div id="sidebar">
      <section id="character-section">
        <p id="character-race"><span id="race">{t(Races.find(e => e.varName === playerState.settings.race)!.name)}</span> {t('Warlock')}</p>
        <p id="character-level">{t('Level')} <span>70</span></p>
        <StatsDisplay />
        <ul id="sidebar-sets">
          <li><h3>Set Bonuses</h3></li>
          {
            Object.entries(playerState.sets)
              // Show the set only if at least one bonus is active from it
              .filter(set => Sets.find(e => e.setId === set[0]) != null && set[1] >= Sets.find(e => e.setId === set[0])!.bonuses[0])
              .map(set =>
                <li key={nanoid()} className="sidebar-set-bonus">
                  <a
                    target='_blank'
                    rel='noreferrer'
                    href={`${getBaseWowheadUrl(i18n.language)}/item-set=${Sets.find(e => e.setId === set[0])?.setId}`}
                    className={Sets.find(e => e.setId === set[0])?.quality}
                  >{Sets.find(e => e.setId === set[0])?.name} ({set[1]})</a>
                </li>
            )
          }
        </ul>

        <div id="sidebar-simulation-selection">
          <SimulationButtons />
          <p id="guybrush-note">This sim was heavily inspired by <a rel="noreferrer" target="_blank" href='https://guybrushgit.github.io/WarriorSim/'>Guybrush's 1.13 Warrior Simulator</a></p>
          <p id="paypal-note"><a href="https://www.paypal.com/paypalme/kristoferhh" rel="noreferrer" target="_blank">Support me via PayPal</a></p>
        </div>
      </section>
    </div>
  )
}