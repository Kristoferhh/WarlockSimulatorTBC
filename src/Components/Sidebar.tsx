import { useSelector } from "react-redux";
import { Races } from "../data/Races";
import { Sets } from "../data/Sets";
import { RootState } from "../redux/Store";
import { SidebarButtons } from "./SidebarButtons";
import StatsDisplay from "./StatsDisplay";

export default function Sidebar() {
  const playerState = useSelector((state: RootState) => state.player);

  return (
    <div id="sidebar">
      <section id="character-section">
        <p id="character-race"><span id="race">{Races.find(e => e.varName === playerState.settings.race)?.name}</span> Warlock</p>
        <p id="character-level">Level <span>70</span></p>

        <StatsDisplay />

        <ul id="sidebar-sets">
          <li><h3>Set Bonuses</h3></li>
          {
            Object.entries(playerState.sets).map((set, i) =>
              // Show the set only if at least one bonus is active from it
              Sets.find(e => e.setId === set[0]) != null && set[1] >= Sets.find(e => e.setId === set[0])!.bonuses[0] &&
                <li key={i} className="sidebar-set-bonus">
                  <a
                    target='_blank'
                    rel='noreferrer'
                    href={`https://tbc.wowhead.com/item-set=${Sets.find(e => e.setId === set[0])?.setId}`}
                    className={Sets.find(e => e.setId === set[0])?.quality}
                  >{Sets.find(e => e.setId === set[0])?.name} ({set[1]})</a>
                </li>
            )
          }
        </ul>

        <div id="sidebar-simulation-selection">
          <SidebarButtons />
          <p id="guybrush-note">This sim was heavily inspired by <a rel="noreferrer" target="_blank" href='https://guybrushgit.github.io/WarriorSim/'>Guybrush's 1.13 Warrior Simulator</a></p>
          <p id="paypal-note"><a href="https://www.paypal.com/paypalme/kristoferhh" rel="noreferrer" target="_blank">Support me via PayPal</a></p>
        </div>
      </section>
    </div>
  )
}