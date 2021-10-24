import { useDispatch, useSelector } from "react-redux"
import { modifySettingValue } from "../Redux/PlayerSlice";
import { RootState } from "../Redux/Store"

export default function SettingsDisplay() {
  const playerStore = useSelector((state: RootState) => state.player);
  const dispatch = useDispatch();

  function settingModifiedHandler(setting: string, value: any) {
    dispatch(modifySettingValue({setting: setting, value: value}));
  }

  return(
    <section id="sim-settings">
      <fieldset>
        <legend>Rotation Options</legend>
        <input onChange={(e) => settingModifiedHandler('rotationOption', 'simChooses')} type='radio' name='rotationOption' value='simChooses' checked={playerStore.settings['rotationOption'] === 'simChooses'} />
        <label htmlFor='simChooses'>Simulation chooses spells for me</label>
        <br/>
        <input onChange={(e) => settingModifiedHandler('rotationOption', 'userChooses')} type='radio' name='rotationOption' value='userChooses' checked={playerStore.settings['rotationOption'] === 'userChooses'} />
        <label htmlFor='userChooses'>Choose spells myself</label>
      </fieldset>
      <ul>
        <li>
          <span className="settings-left">Race</span>
          <select onChange={(e) => settingModifiedHandler('race', e.target.value)} name="race" id="race-dropdown-list" className="settings-right" value={playerStore.settings['race']}>
            <option value="gnome">Gnome</option>
            <option value="human">Human</option>
            <option value="orc">Orc</option>
            <option value="undead">Undead</option>
            <option value="bloodElf">Blood Elf</option>
          </select>
        </li>
        <li>
          <span className="settings-left">Iterations</span>
          <input id="iterations" onChange={(e) => settingModifiedHandler('iterations', e.target.value)} value={playerStore.settings['iterations']} step='1000' min="1000" type="number" name="iterations" className="settings-right" />
        </li>
        <li>
          <span className="settings-left">Min Fight Length</span>
          <input id="min-fight-length" onChange={(e) => settingModifiedHandler('min-fight-length', e.target.value)} value={playerStore.settings['min-fight-length']} type="number" name="min-fight-length" className="settings-right" />
        </li>
        <li>
          <span className="settings-left">Max Fight Length</span>
          <input id="max-fight-length" onChange={(e) => settingModifiedHandler('max-fight-length', e.target.value)} value={playerStore.settings['max-fight-length']} type="number" name="max-fight-length" className="settings-right" />
        </li>
        <li>
          <span className="settings-left">Target Level</span>
          <input id="target-level" onChange={(e) => settingModifiedHandler('target-level', e.target.value)} value={playerStore.settings['target-level']} type="number" name="target-level" className="settings-right" />
        </li>
        <li>
          <span className="settings-left">Target Shadow Resistance</span>
          <input id="target-shadow-resistance" onChange={(e) => settingModifiedHandler('target-shadow-resistance', e.target.value)} value={playerStore.settings['target-shadow-resistance']} type="number" name="target-shadow-resistance" className="settings-right" />
        </li>
        <li>
          <span className="settings-left">Target Fire Resistance</span>
          <input id="target-fire-resistance" onChange={(e) => settingModifiedHandler('target-fire-resistance', e.target.value)} value={playerStore.settings['target-fire-resistance']} type="number" name="target-fire-resistance" className="settings-right" />
        </li>
        <li>
          <span className="settings-left">Fight Type</span>
          <select onChange={(e) => settingModifiedHandler('fightType', e.target.value)} value={playerStore.settings['fightType']} name="fightType" id="fight-type" className="settings-right">
            <option value="singleTarget">Single Target</option>
            <option value="aoe">AoE (Seed of Corruption)</option>
          </select>
        </li>
        {
          playerStore.settings['fightType'] === 'aoe' &&
            <li id="enemy-amount" title="Including the target you're casting Seed of Corruption on">
              <span className="settings-left">Enemy Amount</span>
              <input name="enemyAmount" className="settings-right" onChange={(e) => settingModifiedHandler('enemyAmount', e.target.value)} value={playerStore.settings['enemyAmount']} step="1" min="1" type="number" />
            </li>
        }
        <li id='automatically-open-sim-details'>
          <label className="settings-left" htmlFor="automatically-open-sim-details">Show Damage & Aura Tables</label>
          <select className="settings-right" name="automatically-open-sim-details" onChange={(e) => settingModifiedHandler('automatically-open-sim-details', e.target.value)} value={playerStore.settings['automatically-open-sim-details']}>
            <option value="yes">Yes</option>
            <option value="no">No</option>
          </select>
        </li>
        <li id='randomizeValues' title="Chooses a random value between a minimum and a maximum value instead of taking the average of the two.">
          <label className="settings-left" htmlFor="randomizeValues">Randomize instead of averaging</label>
          <select className="settings-right" name="randomizeValues" onChange={(e) => settingModifiedHandler('randomizeValues', e.target.value)} value={playerStore.settings['randomizeValues']}>
            <option value="no">No</option>
            <option value="yes">Yes</option>
          </select>
        </li>
        <li id="infinitePlayerMana">
          <label className="settings-left" htmlFor="infinitePlayerMana">Infinite player mana?</label>
          <select className="settings-right" name="infinitePlayerMana" onChange={(e) => settingModifiedHandler('infinitePlayerMana', e.target.value)} value={playerStore.settings['infinitePlayerMana']}>
            <option value="no">No</option>
            <option value="yes">Yes</option>
          </select>
        </li>
        <li id="infinitePetMana">
          <label className="settings-left" htmlFor="infinitePetMana">Infinite pet mana?</label>
          <select className="settings-right" name="infinitePetMana" onChange={(e) => settingModifiedHandler('infinitePetMana', e.target.value)} value={playerStore.settings['infinitePetMana']}>
            <option value="no">No</option>
            <option value="yes">Yes</option>
          </select>
        </li>
        <li id='petChoice'>
          <label className="settings-left" htmlFor="petChoice">Pet</label>
          <select className="settings-right" name="petChoice" onChange={(e) => settingModifiedHandler('petChoice', e.target.value)} value={playerStore.settings['petChoice']}>
            <option value="0">Imp</option>
            <option value="2">Succubus</option>
            <option value="4">Felguard</option>
          </select>
        </li>
        <li id="sacrificePet">
          <label className="settings-left" htmlFor="sacrificePet">Sacrifice pet?</label>
          <select className="settings-right" name="sacrificePet" onChange={(e) => settingModifiedHandler('sacrificePet', e.target.value)} value={playerStore.settings['sacrificePet']}>
            <option value="no">No</option>
            <option value="yes">Yes</option>
          </select>
        </li>
        <li id="petMode">
          <label className="settings-left" htmlFor="petMode">Pet mode</label>
          <select className="settings-right" name="petMode" onChange={(e) => settingModifiedHandler('petMode', e.target.value)} value={playerStore.settings['petMode']}>
            <option value="0">Passive</option>
            <option value="1">Aggressive</option>
          </select>
        </li>
        {
          playerStore.settings['petMode'] === '1' && (!playerStore.talents['demonicSacrifice'] || playerStore.settings['sacrificePet'] === 'no') &&
            <li id="prepopBlackBook">
              <label className="settings-left" htmlFor="prepopBlackBook">Prepop Black Book?</label>
              <select className="settings-right" name="prepopBlackBook" onChange={(e) => settingModifiedHandler('prepopBlackBook', e.target.value)} value={playerStore.settings['prepopBlackBook']}>
                <option value="no">No</option>
                <option value="yes">Yes</option>
              </select>
            </li>
        }
        {
          // Shattered Sun Pendant of Acumen equipped
          playerStore.selectedItems['neck'] === 34678 &&
            <div>
              <li id="shattrathFaction">
                <label className="settings-left" htmlFor="shattrathFaction">Shattrath Faction</label>
                <select className="settings-right" name="shattrathFaction" onChange={(e) => settingModifiedHandler('shattrathFaction', e.target.value)} value={playerStore.settings['shattrathFaction']}>
                  <option value="Aldor">Aldor</option>
                  <option value="Scryers">Scryers</option>
                </select>
              </li>
              <li id="shattrathFactionReputation">
                <label className="settings-left" htmlFor="shattrathFactionReputation">Exalted with Shattrath Faction</label>
                <select className="settings-right" name="shattrathFactionReputation" onChange={(e) => settingModifiedHandler('shattrathFactionReputation', e.target.value)} value={playerStore.settings['shattrathFactionReputation']}>
                  <option value="no">No</option>
                  <option value="yes">Yes</option>
                </select>
              </li>
            </div>
        }
        {
          // Display if pet is succubus, pet is aggressive, and pet is not being sacrificed.
          playerStore.settings['petChoice'] === '2' && playerStore.settings['petMode'] === '1' && (!playerStore.talents['demonicSacrifice'] || playerStore.settings['sacrificePet'] === 'no') &&
            <li id="lashOfPainUsage">
              <label className='settings-left' htmlFor='lashOfPainUsage'>When to use Lash of Pain?</label>
              <select className='settings-right' name='lashOfPainUsage' onChange={(e) => settingModifiedHandler('lashOfPainUsage', e.target.value)} value={playerStore.settings['lashOfPainUsage']}>
                <option value='noISB'>When ISB is not up</option>
                <option value='onCooldown'>On Cooldown</option>
              </select>
            </li>
        }
        <li id="enemyArmor">
          <label className="settings-left" htmlFor="enemyArmor">Enemy Armor</label>
          <input className="settings-right" onChange={(e) => settingModifiedHandler('enemyArmor', e.target.value)} value={playerStore.settings['enemyArmor']} type="number" min='0' max='10000' name="enemyArmor" />
        </li>
        {
          playerStore.auras['curseOfTheElements'] === true &&
            <li id="improvedCurseOfTheElements">
              <label className="settings-left">Malediction?</label>
              <select className="settings-right" name="improvedCurseOfTheElements" onChange={(e) => settingModifiedHandler('improvedCurseOfTheElements', e.target.value)} value={playerStore.settings['improvedCurseOfTheElements']}>
                <option value='0'>No</option>
                <option value='1'>1/3</option>
                <option value='2'>2/3</option>
                <option value='3'>3/3</option>
              </select>
            </li>
        }
        {
          playerStore.auras['powerInfusion'] &&
            <li id="powerInfusionAmount">
              <label className="settings-left" htmlFor="powerInfusionAmount">Power Infusion amount</label>
              <select className="settings-right" name="powerInfusionAmount" onChange={(e) => settingModifiedHandler('powerInfusionAmount', e.target.value)} value={playerStore.settings['powerInfusionAmount']}>
                <option value="1">1</option>
                <option value="2">2</option>
                <option value="3">3</option>
                <option value="4">4</option>
                <option value="5">5</option>
                <option value="6">6</option>
                <option value="7">7</option>
                <option value="8">8</option>
                <option value="9">9</option>
                <option value="10">10</option>
                <option value="11">11</option>
                <option value="12">12</option>
              </select>
            </li>
        }
        {
          playerStore.auras['bloodlust'] &&
            <li id="bloodlustAmount">
              <label className="settings-left" htmlFor="bloodlustAmount">Bloodlust amount</label>
              <select className="settings-right" name="bloodlustAmount" onChange={(e) => settingModifiedHandler('bloodlustAmount', e.target.value)} value={playerStore.settings['bloodlustAmount']}>
                <option value="1">1</option>
                <option value="2">2</option>
                <option value="3">3</option>
                <option value="4">4</option>
                <option value="5">5</option>
                <option value="6">6</option>
                <option value="7">7</option>
                <option value="8">8</option>
                <option value="9">9</option>
                <option value="10">10</option>
                <option value="11">11</option>
                <option value="12">12</option>
                <option value="13">13</option>
                <option value="14">14</option>
                <option value="15">15</option>
              </select>
            </li>
        }
        {
          playerStore.auras['innervate'] &&
            <li id="innervateAmount">
              <label className="settings-left" htmlFor="innervateAmount">Innervate amount</label>
              <select className="settings-right" name="innervateAmount" onChange={(e) => settingModifiedHandler('innervateAmount', e.target.value)} value={playerStore.settings['innervateAmount']}>
                <option value="1">1</option>
                <option value="2">2</option>
                <option value="3">3</option>
                <option value="4">4</option>
                <option value="5">5</option>
                <option value="6">6</option>
                <option value="7">7</option>
                <option value="8">8</option>
                <option value="9">9</option>
                <option value="10">10</option>
                <option value="11">11</option>
                <option value="12">12</option>
                <option value="13">13</option>
                <option value="14">14</option>
                <option value="15">15</option>
                <option value="16">16</option>
                <option value="17">17</option>
                <option value="18">18</option>
              </select>
            </li>
        }
        {
          playerStore.auras['prayerOfSpirit'] &&
            <li id="improvedDivineSpirit">
              <label className="settings-left" htmlFor="improvedDivineSpirit">Improved Divine Spirit?</label>
              <select className="settings-right" name="improvedDivineSpirit" onChange={(e) => settingModifiedHandler('improvedDivineSpirit', e.target.value)} value={playerStore.settings['improvedDivineSpirit']}>
                <option value="0">No</option>
                <option value="1">1/2</option>
                <option value="2">2/2</option>
              </select>
            </li>
        }
        {
          playerStore.auras['powerOfTheGuardianMage'] &&
            <li id="mageAtieshAmount">
              <label className="settings-left" htmlFor="mageAtieshAmount">Mage Atiesh amount</label>
              <select className="settings-right" name="mageAtieshAmount" onChange={(e) => settingModifiedHandler('mageAtieshAmount', e.target.value)} value={playerStore.settings['mageAtieshAmount']}>
                <option value="1">1</option>
                <option value="2">2</option>
                <option value="3">3</option>
                <option value="4">4</option>
              </select>
            </li>
        }
        {
          playerStore.auras['powerOfTheGuardianWarlock'] &&
            <li id="warlockAtieshAmount">
              <label className="settings-left" htmlFor="warlockAtieshAmount">Warlock Atiesh amount</label>
              <select className="settings-right" name="warlockAtieshAmount" onChange={(e) => settingModifiedHandler('warlockAtieshAmount', e.target.value)} value={playerStore.settings['warlockAtieshAmount']}>
                <option disabled={true} value="0">{'>> Do not count your own Atiesh <<'}</option>
                <option value="1">1</option>
                <option value="2">2</option>
                <option value="3">3</option>
                <option value="4">4</option>
              </select>
            </li>
        }
        {
          playerStore.auras['totemOfWrath'] &&
            <li id="totemOfWrathAmount">
              <label className="settings-left" htmlFor="totemOfWrathAmount">Totem of Wrath amount</label>
              <select className="settings-right" name="totemOfWrathAmount" onChange={(e) => settingModifiedHandler('totemOfWrathAmount', e.target.value)} value={playerStore.settings['totemOfWrathAmount']}>
                <option value="1">1</option>
                <option value="2">2</option>
                <option value="3">3</option>
                <option value="4">4</option>
              </select>
            </li>
        }
        {
          playerStore.auras['ferociousInspiration'] &&
            <li id="ferociousInspirationAmount">
              <label className="settings-left" htmlFor="ferociousInspirationAmount">Ferocious Inspiration amount</label>
              <select className="settings-right" name="ferociousInspirationAmount" onChange={(e) => settingModifiedHandler('ferociousInspirationAmount', e.target.value)} value={playerStore.settings['ferociousInspirationAmount']}>
                <option value="1">1</option>
                <option value="2">2</option>
                <option value="3">3</option>
                <option value="4">4</option>
              </select>
            </li>
        }
        {
          playerStore.auras['wrathOfAirTotem'] &&
            <li id="improvedWrathOfAirTotem">
              <label className="settings-left" htmlFor="improvedWrathOfAirTotem">Elemental Shaman T4 2pc bonus?</label>
              <select className="settings-right" name="improvedWrathOfAirTotem" onChange={(e) => settingModifiedHandler('improvedWrathOfAirTotem', e.target.value)} value={playerStore.settings['improvedWrathOfAirTotem']}>
                <option value="no">No</option>
                <option value="yes">Yes</option>
              </select>
            </li>
        }
        {
          playerStore.auras['vampiricTouch'] &&
            <li id='shadowPriestDps'>
              <label className="settings-left" htmlFor="shadowPriestDps">Shadow Priest Dps</label>
              <input className='settings-right' onChange={(e) => settingModifiedHandler('shadowPriestDps', e.target.value)} value={playerStore.settings['shadowPriestDps']} type="number" min='0' name="shadowPriestDps" />
            </li>
        }
        {
          playerStore.auras['bloodPact'] &&
            <li id='improvedImpSetting'>
              <label className='settings-left' htmlFor="improvedImpSetting">Improved Imp?</label>
              <select className='settings-right' name='improvedImpSetting' onChange={(e) => settingModifiedHandler('improvedImpSetting', e.target.value)} value={playerStore.settings['improvedImpSetting']}>
                <option value='0'>No</option>
                <option value='1'>1/3</option>
                <option value='2'>2/3</option>
                <option value='3'>3/3</option>
              </select>
            </li>
        }
        {
          playerStore.auras['faerieFire'] &&
            <li id='improvedFaerieFire'>
              <label className='settings-left' htmlFor="improvedFaerieFire">Improved Faerie Fire?</label>
              <select className='settings-right' name='improvedFaerieFire' onChange={(e) => settingModifiedHandler('improvedFaerieFire', e.target.value)} value={playerStore.settings['improvedFaerieFire']}>
                <option value='no'>No</option>
                <option value='yes'>Yes</option>
              </select>
            </li>
        }
        {
          playerStore.auras['exposeArmor'] &&
            <li id='improvedExposeArmor'>
              <label className='settings-left' htmlFor="improvedExposeArmor">Improved Expose Armor?</label>
              <select className='settings-right' name='improvedExposeArmor' onChange={(e) => settingModifiedHandler('improvedExposeArmor', e.target.value)} value={playerStore.settings['improvedExposeArmor']}>
                <option value='0'>No</option>
                <option value='1'>1/2</option>
                <option value='2'>2/2</option>
              </select>
            </li>
        }
        {
          playerStore.auras['exposeWeakness'] &&
            <div>
              <li id='survivalHunterAgility'>
                <label className="settings-left" htmlFor="survivalHunterAgility">Survival Hunter Agility</label>
                <input className='settings-right' onChange={(e) => settingModifiedHandler('survivalHunterAgility', e.target.value)} value={playerStore.settings['survivalHunterAgility']} type="number" min='0' name="survivalHunterAgility" />
              </li>
              <li id='exposeWeaknessUptime'>
                <label className="settings-left" htmlFor="exposeWeaknessUptime">Expose Weakness Uptime %</label>
                <input className='settings-right' onChange={(e) => settingModifiedHandler('exposeWeaknessUptime', e.target.value)} value={playerStore.settings['exposeWeaknessUptime']} type="number" min='0' name="exposeWeaknessUptime" />
              </li>
            </div>
        }
        <li id="customIsbUptime">
          <label className="settings-left" htmlFor="customIsbUptime">Use custom ISB uptime %?</label>
          <select className="settings-right" name="customIsbUptime" onChange={(e) => settingModifiedHandler('customIsbUptime', e.target.value)} value={playerStore.settings['customIsbUptime']}>
            <option value="yes">Yes</option>
            <option value="no">No</option>
          </select>
        </li>
        {
          playerStore.settings['customIsbUptime'] === 'yes' &&
            <li id="custom-isb-uptime-value">
              <span className="settings-left">Custom ISB Uptime %</span>
              <input id="customIsbUptimeValue" onChange={(e) => settingModifiedHandler('customIsbUptimeValue', e.target.value)} value={playerStore.settings['customIsbUptimeValue']} type="number" name="customIsbUptimeValue" className="settings-right" />
            </li>
        }
      </ul>
    </section>
  )
}