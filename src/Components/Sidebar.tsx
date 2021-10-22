

export default function Sidebar() {


  return (
    <div id="sidebar">
      <section id="character-section">
        <p id="character-race"><span id="race"></span> Warlock</p>
        <p id="character-level">Level <span>70</span></p>

        <ul className="character-stats">
          <li>
            <p className="character-stat">Health</p>
            <p className="character-stat-val" id="character-health-val"></p>
          </li>
          <li>
            <p className="character-stat">Mana</p>
            <p className="character-stat-val" id="character-mana-val"></p>
          </li>
          <li>
            <p className="character-stat">Stamina</p>
            <p className="character-stat-val" id="character-stamina-val"></p>
          </li>
          <li>
            <p className="character-stat">Intellect</p>
            <p className="character-stat-val" id="character-intellect-val"></p>
          </li>
          <li>
            <p className="character-stat">Spirit</p>
            <p className="character-stat-val" id="character-spirit-val"></p>
          </li>
          <li>
            <p className="character-stat">Spell Power</p>
            <p className="character-stat-val" id="character-spell-power-val"></p>
          </li>
          <li>
            <p className="character-stat">Shadow Power</p>
            <p className="character-stat-val" id="character-shadow-power-val"></p>
          </li>
          <li>
            <p className="character-stat">Fire Power</p>
            <p className="character-stat-val" id="character-fire-power-val"></p>
          </li>
          <li>
            <p className="character-stat">Crit Rating</p>
            <p className="character-stat-val" id="character-crit-val"></p>
          </li>
          <li>
            <p className="character-stat">Hit Rating</p>
            <p className="character-stat-val" id="character-hit-val"></p>
          </li>
          <li>
            <p className="character-stat">Haste Rating</p>
            <p className="character-stat-val" id="character-haste-val"></p>
          </li>
          <li>
            <p className="character-stat">Shadow Dmg %</p>
            <p className="character-stat-val" id="character-shadow-damage-modifier-val"></p>
          </li>
          <li>
            <p className="character-stat">Fire Dmg %</p>
            <p className="character-stat-val" id="character-fire-damage-modifier-val"></p>
          </li>
          <li>
            <p className="character-stat">MP5</p>
            <p className="character-stat-val" id="character-mp5-val"></p>
          </li>
          <li>
            <p className="character-stat">Enemy Armor</p>
            <p className="character-stat-val" id="enemy-armor-val"></p>
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