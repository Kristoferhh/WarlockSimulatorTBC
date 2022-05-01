import { Talents, TalentTreeStruct } from '../data/Talents'
import { RootState } from '../redux/Store'
import { useSelector, useDispatch } from 'react-redux'
import { setTalentPointValue } from '../redux/PlayerSlice'
import { PresetTalents } from '../data/PresetTalents'
import { Talent, TalentName } from '../Types'
import { nanoid } from 'nanoid'
import { getAllocatedTalentsPointsInTree, getBaseWowheadUrl } from '../Common'
import { useTranslation } from 'react-i18next'
import i18n from '../i18n/config'

export default function TalentTrees() {
  const talentState = useSelector((state: RootState) => state.player.talents)
  const talentPointsRemaining = useSelector(
    (state: RootState) => state.player.talentPointsRemaining,
  )
  const dispatch = useDispatch()
  const { t } = useTranslation()

  function applyTalentTemplate(talentTemplate: {
    [key in TalentName]?: number
  }) {
    for (const talentKey in TalentName) {
      dispatch(
        setTalentPointValue({
          name: talentKey as TalentName,
          points: 0,
        }),
      )
    }
    for (const [talentKey, points] of Object.entries(talentTemplate)) {
      dispatch(
        setTalentPointValue({
          name: talentKey as TalentName,
          points: points,
        }),
      )
    }
  }

  function talentClickHandler(mouseButton: number, talent: Talent) {
    const currentTalentPointsValue = talentState[talent.varName!!] || 0

    // 0 is left click and 2 is right click
    if (
      mouseButton === 0 &&
      currentTalentPointsValue < talent.rankIDs!!.length &&
      talentPointsRemaining > 0
    ) {
      dispatch(
        setTalentPointValue({
          name: talent.varName!!,
          points: currentTalentPointsValue + 1,
        }),
      )
    } else if (mouseButton === 2 && currentTalentPointsValue > 0) {
      dispatch(
        setTalentPointValue({
          name: talent.varName!!,
          points: currentTalentPointsValue - 1,
        }),
      )
    }
  }

  function clearTalentTree(talentTree: TalentTreeStruct) {
    for (const row in talentTree.rows) {
      for (const talentKey in talentTree.rows[row]) {
        dispatch(
          setTalentPointValue({
            name: talentTree.rows[row][talentKey].varName!!,
            points: 0,
          }),
        )
      }
    }
  }

  return (
    <div id='talents-container'>
      <div id='preset-talent-buttons'>
        {PresetTalents.map(talentTemplate => (
          <button
            className='btn btn-primary btn-sm'
            type='button'
            key={nanoid()}
            onClick={() => applyTalentTemplate(talentTemplate.talents)}
          >
            {t(talentTemplate.name)}
          </button>
        ))}
      </div>
      <section id='talents-section'>
        {Talents.map(talentTree => (
          <div key={talentTree.name} className='talent-tree-div'>
            <img
              src={`${process.env.PUBLIC_URL}/img/talent_tree_background_${talentTree.name}.jpg`}
              alt={talentTree.name}
              style={{ position: 'absolute', height: '430px', width: '206px' }}
            />
            <table
              id={'talent-table-' + talentTree.name}
              className='talent-tree-table'
            >
              <tbody>
                {talentTree.rows.map(row => (
                  <tr key={nanoid()}>
                    {row.map(talent => (
                      <td key={nanoid()}>
                        {talent.rankIDs && (
                          <div
                            id={talent.varName}
                            className='talent-icon'
                            data-points={talentState[talent.varName!] || 0}
                            data-maxpoints={talent.rankIDs.length}
                            onClick={e =>
                              talentClickHandler(e.nativeEvent.button, talent)
                            }
                            onContextMenu={e => {
                              talentClickHandler(e.nativeEvent.button, talent)
                              e.preventDefault()
                            }}
                          >
                            <a
                              href={`${getBaseWowheadUrl(
                                i18n.language,
                              )}/spell=${
                                talent.rankIDs[
                                  Math.max(
                                    talentState[talent.varName!] - 1,
                                    0,
                                  ) || 0
                                ]
                              }`}
                              onClick={e => e.preventDefault()}
                            >
                              <img
                                src={`${process.env.PUBLIC_URL}/img/${talent.iconName}.jpg`}
                                alt={t(talent.name!)}
                              />
                              <span
                                id={talent.varName! + '-point-amount'}
                                className={
                                  'talent-point-amount' +
                                  (talentState[talent.varName!] &&
                                  talentState[talent.varName!] ===
                                    talent.rankIDs.length
                                    ? ' maxed-talent'
                                    : '') +
                                  (talentState[talent.varName!] &&
                                  talentState[talent.varName!] > 0 &&
                                  talentState[talent.varName!] <
                                    talent.rankIDs.length
                                    ? ' half-full-talent'
                                    : '') +
                                  (talentState[talent.varName!] == null ||
                                  talentState[talent.varName!] === 0
                                    ? ' empty-talent'
                                    : '')
                                }
                              >
                                {talentState[talent.varName!] || 0}
                              </span>
                            </a>
                          </div>
                        )}
                      </td>
                    ))}
                  </tr>
                ))}
              </tbody>
            </table>
            <div className='talent-tree-name'>
              <h3 style={{ display: 'inline-block' }}>
                {t(talentTree.name) +
                  ' ' +
                  (getAllocatedTalentsPointsInTree(talentState, talentTree) > 0
                    ? '(' +
                      getAllocatedTalentsPointsInTree(talentState, talentTree) +
                      ')'
                    : '')}
              </h3>
              <span
                className='clear-talent-tree'
                onClick={() => clearTalentTree(talentTree)}
              >
                ❌
              </span>
            </div>
          </div>
        ))}
      </section>
    </div>
  )
}
