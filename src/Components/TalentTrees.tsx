import { Talents } from "../Data/Talents"
import { Talent } from "../Types"

export default function TalentTrees() {

  
  function TalentClickHandler(talent: Talent) {
    
  }

  return(
    <div id="talents-container">
      <div id="preset-talent-buttons">
        <button className="preset-talent-button" data-name="afflictionUa">Affli/UA</button>
        <button className="preset-talent-button" data-name="afflictionRuin">Affli/Ruin</button>
        <button className="preset-talent-button" data-name="destruction">Destruction</button>
        <button className="preset-talent-button" data-name='destroNightfall'>Destro/Nightfall</button>
        <button className="preset-talent-button" data-name="demonologyFelguard">Demonology/Felguard</button>
        <button className="preset-talent-button" data-name="demonologyRuin">Demonology/Ruin</button>
      </div>
      <section id="talents-section">
        {
          Talents.map((talentTree, i) =>
            <div key={i} className="talent-tree-div">
              <table id={'talent-table-' + talentTree.name} className='talent-tree-table'>
                <tbody>
                  <img src={`${process.env.PUBLIC_URL}/img/talent_tree_background_${talentTree.name}.jpg`} alt={talentTree.name} style={{position: 'absolute', height: '430px', width: '206px'}} />
                  {
                    talentTree.rows.map((row, j) =>
                      <tr key={j}>
                        {
                          row.map((talent, k) =>
                            <td key={k}>
                              {
                                talent.rankIDs &&
                                  <div id={talent.varName} className='talent-icon' data-points={0} data-maxpoints={talent.rankIDs.length} onClick={() => TalentClickHandler(talent)}>
                                    <a href={'https://tbc.wowhead.com/spell=' + talent.rankIDs[0]} onClick={(e) => e.preventDefault()}>
                                      <img src={`${process.env.PUBLIC_URL}/img/${talent.iconName}.jpg`} alt={talent.name} />
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
              <div>
                <h3 className='talent-tree-name' style={{display: 'inline-block'}}>{talentTree.name}</h3>
                <span className='clear-talent-tree'>❌</span>
              </div>
            </div>
          )
        }
      </section>
    </div>
  )
}