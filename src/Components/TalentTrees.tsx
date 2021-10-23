import { Talents, TalentTreeStruct } from "../Data/Talents"
import { RootState } from "../Store";
import { useSelector, useDispatch } from "react-redux";
import { setTalentPointValue } from "../PlayerSlice";
import { PresetTalents } from "../Data/PresetTalents";
import { Talent, TalentNames } from "../Types";

export default function TalentTrees() {
  const talentStore = useSelector((state: RootState) => state.player.talents);
  const talentPointsRemaining = useSelector((state: RootState) => state.player.talentPointsRemaining);
  const dispatch = useDispatch();

  /**
   * Returns how many talent points are allocated in a given talent tree.
   * 
   * @param {TalentTreeStruct} tree
   * @returns number
   */
  function getAllocatedTalentsPointsInTree(tree: TalentTreeStruct): number {
    let points = 0;

    for (const row in tree.rows) {
      for (const talent in tree.rows[row]) {
        const talentKey = tree.rows[row][talent].varName;

        if (talentKey != null) {
          points += talentStore[talentKey];
        }
      }
    }

    return points;
  }

  function applyTalentTemplate(talentTemplate: {[key in keyof typeof TalentNames]?: number}) {
    for (const talentKey in TalentNames) {
      dispatch(setTalentPointValue({
        name: talentKey,
        points: 0
      }));
    }
    for (const [talentKey, points] of Object.entries(talentTemplate)) {
      dispatch(setTalentPointValue({
        name: talentKey,
        points: points
      }));
    }
  }

  function talentClickHandler(mouseButton: number, talent: Talent) {
    const currentTalentPointsValue = talentStore[talent.varName!!] || 0;

    // 0 is left click and 2 is right click
    if (mouseButton === 0 && currentTalentPointsValue < talent.rankIDs!!.length && talentPointsRemaining > 0) {
      dispatch(setTalentPointValue({
        name: talent.varName!!,
        points: currentTalentPointsValue + 1
      }));
    } else if (mouseButton === 2 && currentTalentPointsValue > 0) {
      dispatch(setTalentPointValue({
        name: talent.varName!!,
        points: currentTalentPointsValue - 1
      }));
    }
  }

  function clearTalentTree(talentTree: TalentTreeStruct) {
    for (const row in talentTree.rows) {
      for (const talentKey in talentTree.rows[row]) {
        dispatch(setTalentPointValue({
          name: talentTree.rows[row][talentKey].varName!!,
          points: 0
        }));
      }
    }
  }

  return(
    <div id="talents-container">
      <div id="preset-talent-buttons">
        {
          PresetTalents.map((talentTemplate, i) =>
            <button key={i} onClick={() => applyTalentTemplate(talentTemplate.talents)}>{talentTemplate.name}</button>
          )
        }
      </div>
      <section id="talents-section">
        {
          Talents.map((talentTree, i) =>
            <div key={i} className="talent-tree-div">
              <img src={`${process.env.PUBLIC_URL}/img/talent_tree_background_${talentTree.name}.jpg`} alt={talentTree.name} style={{position: 'absolute', height: '430px', width: '206px'}} />
              <table id={'talent-table-' + talentTree.name} className='talent-tree-table'>
                <tbody>
                  {
                    talentTree.rows.map((row, j) =>
                      <tr key={j}>
                        {
                          row.map((talent, k) =>
                            <td key={k}>
                              {
                                talent.rankIDs &&
                                  <div
                                    id={talent.varName}
                                    className='talent-icon'
                                    data-points={talentStore[talent.varName!!] || 0}
                                    data-maxpoints={talent.rankIDs.length}
                                    onClick={(e) => talentClickHandler(e.nativeEvent.button, talent)}
                                    onContextMenu={(e) => { talentClickHandler(e.nativeEvent.button, talent); e.preventDefault() }}>
                                    <a href={'https://tbc.wowhead.com/spell=' + talent.rankIDs[0]} onClick={(e) => e.preventDefault()}>
                                      <img src={`${process.env.PUBLIC_URL}/img/${talent.iconName}.jpg`} alt={talent.name} />
                                      <span
                                        id={talent.varName!! + '-point-amount'}
                                        className={'talent-point-amount' +
                                          (talentStore[talent.varName!!] && talentStore[talent.varName!!] === talent.rankIDs.length ? ' maxed-talent' : '') +
                                          (talentStore[talent.varName!!] && talentStore[talent.varName!!] > 0 && talentStore[talent.varName!!] < talent.rankIDs.length ? ' half-full-talent' : '') +
                                          (talentStore[talent.varName!!] == null || talentStore[talent.varName!!] === 0 ? ' empty-talent' : '')}>
                                        {talentStore[talent.varName!!] || 0}
                                      </span>
                                    </a>
                                  </div>
                              }
                            </td>
                          )
                        }
                      </tr>
                    )
                  }
                </tbody>
              </table>
              <div className='talent-tree-name'>
                <h3 style={{display: 'inline-block'}}>{talentTree.name + ' ' + (getAllocatedTalentsPointsInTree(talentTree) > 0 ? '(' + getAllocatedTalentsPointsInTree(talentTree) + ')' : '')}</h3>
                <span className='clear-talent-tree' onClick={() => clearTalentTree(talentTree)}>❌</span>
              </div>
            </div>
          )
        }
      </section>
    </div>
  )
}