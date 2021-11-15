import { nanoid } from "nanoid";
import { useTranslation } from "react-i18next";
import { useSelector } from "react-redux"
import { getBaseWowheadUrl } from "../Common";
import { AuraGroups } from "../data/AuraGroups";
import { Auras } from "../data/Auras";
import { Items } from "../data/Items";
import { Spells } from "../data/Spells";
import i18n from "../i18n/config";
import { RootState } from "../redux/Store"

function formatPercentage(num: number) {
  return (Math.round(num * 10000) / 100).toFixed(2);
}

function findSpellByName(name: string): { iconName: string, id: number, wowheadType: 'spell' | 'item' } | undefined {
  const spellObj = Spells.find(e => e.name === name);
  if (spellObj) {
    return { iconName: spellObj.iconName, id: spellObj.id, wowheadType: 'spell' }
  }

  const auraObj = Auras.find(e => e.name === name);
  if (auraObj) {
    return { iconName: auraObj.iconName, id: auraObj.id, wowheadType: AuraGroups.find(e => e.heading === auraObj.group)!.type }
  }

  const itemObj = Items.find(e => e.name === name);
  if (itemObj) {
    return { iconName: itemObj.iconName, id: itemObj.id, wowheadType: 'item' }
  }
}

export default function BreakdownTables() {
  const breakdownObj = useSelector((state: RootState) => state.ui.combatLogBreakdown);
  const { t } = useTranslation();

  return (
    <div id='breakdown-tables-container' style={{ display: breakdownObj.data.length === 0 ? 'none' : '' }}>
      <section className="breakdown-section" id="damage-breakdown-section" style={{ display: 'inline-block' }}>
        <table className="breakdown-table tablesorter" id="damage-breakdown-table" data-sortlist='[[1,1]]'>
          <thead>
            <tr>
              <th>{t('Name')}</th>
              <th>{t('Damage')}</th>
              <th>{t('Casts')}</th>
              <th>{t('Avg Cast')}</th>
              <th>{t('Crit %')}</th>
              <th>{t('Miss %')}</th>
              <th style={{ display: breakdownObj.spellDamageDict.Melee ? '' : 'none' }}>{t('Dodge %')}</th>
              <th style={{ display: breakdownObj.spellDamageDict.Melee ? '' : 'none' }}>{t('Glancing %')}</th>
              <th>{t('Dps')}</th>
            </tr>
          </thead>
          <tbody>
            {
              breakdownObj.data
                .filter(e => (breakdownObj.spellDamageDict[e.name] && breakdownObj.spellDamageDict[e.name] > 0))
                .map(spell => {
                  const spellObj = findSpellByName(spell.name);

                  return (
                    <tr key={nanoid()} className='spell-damage-information'>
                      <td>
                        {
                          spellObj?.iconName && spellObj.iconName.length > 0 &&
                          <img
                            className='breakdown-table-spell-icon'
                            src={`${process.env.PUBLIC_URL}/img/${spellObj?.iconName}.jpg`}
                          />
                        }
                        <a
                          className='breakdown-table-spell-name'
                          target='_blank'
                          rel='noreferrer'
                          href={spellObj && spellObj.id !== 0 ?
                            `${getBaseWowheadUrl(i18n.language)}/${spellObj.wowheadType}=${spellObj.id}` : ''}
                        >{t(spell.name)}</a>
                      </td>
                      <td>
                        <meter
                          value={breakdownObj.spellDamageDict[spell.name] ?
                            (breakdownObj.spellDamageDict[spell.name] / breakdownObj.totalDamageDone) * 100 : '0'}
                          min='0'
                          max='100'
                        ></meter> {breakdownObj.spellDamageDict[spell.name] ?
                          formatPercentage(breakdownObj.spellDamageDict[spell.name] / breakdownObj.totalDamageDone) : 0}%
                      </td>
                      <td className='number' title={spell.casts.toString()}>
                        {
                          spell.casts / breakdownObj.totalIterationAmount < 2 ?
                            Math.round((spell.casts / breakdownObj.totalIterationAmount) * 10) / 10 :
                            Math.round(spell.casts / breakdownObj.totalIterationAmount)
                        }
                      </td>
                      <td className='number'>
                        {
                          breakdownObj.spellDamageDict[spell.name] ?
                            Math.round(breakdownObj.spellDamageDict[spell.name] / spell.casts) : 0
                        }
                      </td>
                      <td className='number'>{formatPercentage(spell.crits / spell.casts)}</td>
                      <td className='number'>{formatPercentage(spell.misses / spell.casts)}</td>
                      {
                        breakdownObj.spellDamageDict.Melee &&
                        <>
                          <td className='number'>{formatPercentage(spell.dodges / spell.casts)}</td>
                          <td className='number'>{formatPercentage(spell.glancingBlows / spell.casts)}</td>
                        </>
                      }
                      <td className='number'>
                        {
                          breakdownObj.spellDamageDict[spell.name] ?
                            Math.round((breakdownObj.spellDamageDict[spell.name] / breakdownObj.totalSimulationFightLength) * 100) / 100 : 0
                        }
                      </td>
                    </tr>
                  )
                })
            }
          </tbody>
        </table>
      </section>
      <section className="breakdown-section" id="mana-gain-breakdown-section" style={{ display: 'inline-block' }}>
        <table className="breakdown-table tablesorter" id="mana-gain-breakdown-table" data-sortlist='[[1,1]]'>
          <thead>
            <tr>
              <th>{t('Name')}</th>
              <th>{t('% Of Total Gain')}</th>
              <th>{t('Casts')}</th>
              <th>{t('Avg Cast')}</th>
            </tr>
          </thead>
          <tbody>
            {
              breakdownObj.data
                .filter(e => breakdownObj.spellManaGainDict[e.name] && breakdownObj.spellManaGainDict[e.name] > 0)
                .map(spell => {
                  const spellObj = findSpellByName(spell.name);

                  return (
                    <tr key={nanoid()} className='spell-damage-information'>
                      <td>
                        {
                          spellObj?.iconName && spellObj.iconName.length > 0 &&
                          <img
                            className='breakdown-table-spell-icon'
                            src={`${process.env.PUBLIC_URL}/img/${spellObj?.iconName}.jpg`}
                          />
                        }
                        <a
                          className='breakdown-table-spell-name'
                          target='_blank'
                          rel='noreferrer'
                          href={spellObj && spellObj.id !== 0 ?
                            `${getBaseWowheadUrl(i18n.language)}/${spellObj.wowheadType}=${spellObj.id}` : ''}
                        >{t(spell.name)}</a>
                      </td>
                      <td>
                        <meter
                          value={breakdownObj.spellManaGainDict[spell.name] ?
                            (breakdownObj.spellManaGainDict[spell.name] / breakdownObj.totalManaGained) * 100 : '0'}
                          min='0'
                          max='100'
                        ></meter> {breakdownObj.spellManaGainDict[spell.name] ?
                          formatPercentage(breakdownObj.spellManaGainDict[spell.name] / breakdownObj.totalManaGained) : 0}%
                      </td>
                      <td className='number'>
                        {
                          (spell.casts / breakdownObj.totalIterationAmount) < 2 ?
                            Math.round((spell.casts / breakdownObj.totalIterationAmount) * 10) / 10 :
                            Math.round(spell.casts / breakdownObj.totalIterationAmount)
                        }
                      </td>
                      <td className='number'>
                        {
                          breakdownObj.spellManaGainDict[spell.name] ?
                            Math.round(breakdownObj.spellManaGainDict[spell.name] / spell.casts) : 0
                        }
                      </td>
                    </tr>
                  )
                })
            }
          </tbody>
        </table>
      </section>
      <section className="breakdown-section" id="aura-breakdown-section" style={{ display: 'inline-block' }}>
        <table className="breakdown-table tablesorter" id="aura-breakdown-table" data-sortlist='[[2,1]]'>
          <thead>
            <tr>
              <th>{t('Name')}</th>
              <th>{t('Count')}</th>
              <th>{t('Uptime')}</th>
            </tr>
          </thead>
          <tbody>
            {
              breakdownObj.data
                .filter(e => e.uptime > 0)
                .map(spell => {
                  const spellObj = findSpellByName(spell.name);

                  return (
                    <tr key={nanoid()} className='spell-damage-information'>
                      <td>
                        {
                          spellObj?.iconName && spellObj.iconName.length > 0 &&
                          <img
                            className='breakdown-table-spell-icon'
                            src={`${process.env.PUBLIC_URL}/img/${spellObj?.iconName}.jpg`}
                          />
                        }
                        <a
                          className='breakdown-table-spell-name'
                          target='_blank'
                          rel='noreferrer'
                          href={spellObj && spellObj.id !== 0 ?
                            `${getBaseWowheadUrl(i18n.language)}/${spellObj.wowheadType}=${spellObj.id}` : ''}
                        >{t(spell.name)}</a>
                      </td>
                      {/*If the aura's count is less than 2 then show 1 decimal place, otherwise just round the value*/}
                      <td className='number'>
                        {
                          (spell.count / breakdownObj.totalIterationAmount) < 2 ?
                            Math.round((spell.count / breakdownObj.totalIterationAmount) * 10) / 10 :
                            Math.round(spell.count / breakdownObj.totalIterationAmount)
                        }
                      </td>
                      <td>
                        <meter
                          value={(spell.uptime / breakdownObj.totalSimulationFightLength) * 100}
                          min='0'
                          max='100'
                        ></meter> {formatPercentage(spell.uptime / breakdownObj.totalSimulationFightLength)}%
                      </td>
                    </tr>
                  )
                })
            }
          </tbody>
        </table>
      </section>
    </div>
  )
}