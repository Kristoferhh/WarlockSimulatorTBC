import { useSelector } from "react-redux"
import { Races } from "../data/Races";
import { RootState } from "../redux/Store"
import { StatConstant } from "../Types";


export default function Sidebar() {
  const playerStore = useSelector((state: RootState) => state.player);

  return (
    <div id="sidebar">
      <section id="character-section">
        <p id="character-race"><span id="race">{Races.find(e => e.varName === playerStore.settings.race)?.name}</span> Warlock</p>
        <p id="character-level">Level <span>70</span></p>

        <ul className="character-stats">
          <li>
            <p className="character-stat">Health</p>
            <p className="character-stat-val" id="character-health-val">{Math.round((playerStore.stats.health + (playerStore.stats.stamina * playerStore.stats.staminaModifier) * StatConstant.healthPerStamina) * (1 + (0.01 * playerStore.talents['felStamina'])))}</p>
          </li>
          <li>
            <p className="character-stat">Mana</p>
            <p className="character-stat-val" id="character-mana-val">{Math.round((playerStore.stats.mana + (playerStore.stats.intellect * playerStore.stats.intellectModifier) * StatConstant.manaPerIntellect) * (1 + (0.01 * playerStore.talents['felIntellect'])))}</p>
          </li>
          <li>
            <p className="character-stat">Stamina</p>
            <p className="character-stat-val" id="character-stamina-val">{Math.round(playerStore.stats.stamina * playerStore.stats.staminaModifier)}</p>
          </li>
          <li>
            <p className="character-stat">Intellect</p>
            <p className="character-stat-val" id="character-intellect-val">{Math.round(playerStore.stats.intellect  * playerStore.stats.intellectModifier)}</p>
          </li>
          <li>
            <p className="character-stat">Spirit</p>
            <p className="character-stat-val" id="character-spirit-val">{Math.round(playerStore.stats.spirit * playerStore.stats.spiritModifier)}</p>
          </li>
          <li>
            <p className="character-stat">Spell Power</p>
            <p className="character-stat-val" id="character-spell-power-val">{playerStore.stats.spellPower}</p>
          </li>
          <li>
            <p className="character-stat">Shadow Power</p>
            <p className="character-stat-val" id="character-shadow-power-val">{playerStore.stats.shadowPower} ({playerStore.stats.spellPower + playerStore.stats.shadowPower})</p>
          </li>
          <li>
            <p className="character-stat">Fire Power</p>
            <p className="character-stat-val" id="character-fire-power-val">{playerStore.stats.firePower} ({playerStore.stats.spellPower + playerStore.stats.firePower})</p>
          </li>
          <li>
            <p className="character-stat">Crit Rating</p>
            <p className="character-stat-val" id="character-crit-val">{Math.round(playerStore.stats.critRating)} ({(Math.round((playerStore.stats.critRating / StatConstant.critRatingPerPercent + StatConstant.baseCritChancePercent) * 100) / 100 + (playerStore.talents['devastation'] || 0)).toFixed(2)}%)</p>
          </li>
          <li>
            <p className="character-stat">Hit Rating</p>
            <p className="character-stat-val" id="character-hit-val">{playerStore.stats.hitRating} ({(Math.round((playerStore.stats.hitRating / StatConstant.hitRatingPerPercent) * 100) / 100).toFixed(2)}%)</p>
          </li>
          <li>
            <p className="character-stat">Haste Rating</p>
            <p className="character-stat-val" id="character-haste-val">{playerStore.stats.hasteRating} ({(Math.round((playerStore.stats.hasteRating / StatConstant.hasteRatingPerPercent) * 100) / 100).toFixed(2)}%)</p>
          </li>
          <li>
            <p className="character-stat">Shadow Dmg %</p>
            <p className="character-stat-val" id="character-shadow-damage-modifier-val">{Math.round(playerStore.stats.shadowModifier * (1 + (0.02 * playerStore.talents['shadowMastery'])) * 100)}%</p>
          </li>
          <li>
            <p className="character-stat">Fire Dmg %</p>
            <p className="character-stat-val" id="character-fire-damage-modifier-val">{Math.round(playerStore.stats.fireModifier * 100)}%</p>
          </li>
          <li>
            <p className="character-stat">MP5</p>
            <p className="character-stat-val" id="character-mp5-val">{playerStore.stats.mp5}</p>
          </li>
          <li>
            <p className="character-stat">Enemy Armor</p>
            <p className="character-stat-val" id="enemy-armor-val">{playerStore.stats.enemyArmor}</p>
          </li>
          {/*<li>
            <p className="character-stat">Spell Penetration</p>
            <p className="character-stat-val" id="character-spell-pen-val"></p>
          </li>*/}
        </ul>

        <ul id="sidebar-sets">
          <li><h3>Set Bonuses</h3></li>
          <li className="sidebar-set-bonus">Plagueheart Raiment (4)</li>
        </ul>

        <div id="sidebar-simulation-selection">
          <div id="sim-result-dps-div">
            <p><span id="avg-dps"></span><span> DPS</span> <span id="dps-stdev"></span></p>
            <p>Min: <span id="min-dps"></span> Max: <span id="max-dps"></span></p>
          </div>
          <div className='btn' id='sim-dps'>Simulate</div>
          <div className='btn' id='sim-all-items'>Simulate All Items</div>
          <div className='btn' id='sim-stat-weights'>Stat Weights</div>
          <div className='btn' id='show-combat-log'>Combat Log</div>
          <div className='btn' id='show-histogram'>Histogram</div>
          <p id="sim-length-result"></p>
          <p id="guybrush-note">This sim was heavily inspired by <a rel="noreferrer" target="_blank" href='https://guybrushgit.github.io/WarriorSim/'>Guybrush's 1.13 Warrior Simulator</a></p>
          <p id="paypal-note"><a href="https://www.paypal.com/paypalme/kristoferhh" rel="noreferrer" target="_blank">Support me via PayPal</a></p>
        </div>
      </section>
    </div>
  )
}