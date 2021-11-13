import { nanoid } from "nanoid";
import { useDispatch, useSelector } from "react-redux"
import { getAllocatedTalentsPointsInTree } from "../Common";
import { Talents } from "../data/Talents";
import { RootState } from "../redux/Store"
import { setStatWeightVisibility } from "../redux/UiSlice";
import { Stat, StatWeightStats, TalentTree } from "../Types";

const statDisplay: {displayName: string, stat: [keyof StatWeightStats]}[] = [
  { displayName: 'Stamina', stat: [Stat.stamina] },
  { displayName: 'Intellect', stat: [Stat.intellect] },
  { displayName: 'Spirit', stat: [Stat.spirit] },
  { displayName: 'Spell Power', stat: [Stat.spellPower] },
  { displayName: 'Shadow Power', stat: [Stat.shadowPower] },
  { displayName: 'Fire Power', stat: [Stat.firePower] },
  { displayName: 'Hit Rating', stat: [Stat.hitRating] },
  { displayName: 'Crit Rating', stat: [Stat.critRating] },
  { displayName: 'Haste Rating', stat: [Stat.hasteRating] },
  { displayName: 'Mp5', stat: [Stat.mp5] },
]

export default function StatWeights() {
  const playerState = useSelector((state: RootState) => state.player);
  const uiState = useSelector((state: RootState) => state.ui);
  const dispatch = useDispatch();

  return(
    <section id="stat-weights-section" style={{display: uiState.statWeights.visible ? '' : 'none'}}>
      <div>
        <h2 style={{display: 'inline-block'}}>Stat Weights</h2>
        <p
          className='close'
          id='stat-weight-close-button'
          onClick={(e) => {
            dispatch(setStatWeightVisibility(false));
            e.preventDefault();
          }}
        ></p>
      </div>
      <ul className="character-stats" id="stat-weights">
        {
          statDisplay.map(stat =>
            <li key={nanoid()}>
              <p className='character-stat'>{stat.displayName}</p>
              <p className='character-stat-val'>{uiState.statWeights.statValues[stat.stat as unknown as Stat]}</p>
            </li>
          )
        }
        <button
          className='btn btn-primary btn-sm'
          style={{ marginTop: '15px', display: uiState.simulationInProgress ? 'none' : '' }}
          onClick={() => {
            let talentTreePoints: {name: TalentTree, points: number}[] = [
              { name: TalentTree.Affliction, points: getAllocatedTalentsPointsInTree(playerState.talents, Talents.find(e => e.name === TalentTree.Affliction)!) },
              { name: TalentTree.Demonology, points: getAllocatedTalentsPointsInTree(playerState.talents, Talents.find(e => e.name === TalentTree.Demonology)!) },
              { name: TalentTree.Destruction, points: getAllocatedTalentsPointsInTree(playerState.talents, Talents.find(e => e.name === TalentTree.Destruction)!) }
            ];

            // Gets the name of the talent tree with the most amount of points allocated by the player.
            const playerSpec = talentTreePoints.find(e => e.points === Math.max.apply(Math, talentTreePoints.map(a => a.points)))?.name;
            navigator.clipboard.writeText(
              `( Pawn: v1: "${uiState.selectedProfile || 'Warlock'}": Class=Warlock, Spec=${playerSpec}` +
              `, Stamina=${uiState.statWeights.statValues.stamina}` +
              `, Intellect=${uiState.statWeights.statValues.intellect}` +
              `, Spirit=${uiState.statWeights.statValues.spirit}` +
              `, SpellCritRating=${uiState.statWeights.statValues.critRating}` +
              `, SpellHitRating=${uiState.statWeights.statValues.hitRating}` +
              `, FireSpellDamage=${uiState.statWeights.statValues.firePower}` +
              `, ShadowSpellDamage=${uiState.statWeights.statValues.shadowPower}` +
              `, SpellDamage=${uiState.statWeights.statValues.spellPower}` +
              `, Mp5=${uiState.statWeights.statValues.mp5}` +
              `, SpellHasteRating=${uiState.statWeights.statValues.hasteRating})`
            );
          }}
        >Copy Pawn Import String</button>
      </ul>
    </section>
  )
}