

export default function StatWeights() {
  return(
    <section className="close-button-target" id="stat-weights-section" style={{display: 'none'}}>
      <div>
        <h2 style={{display: 'inline-block'}}>Stat Weights</h2>
        <a href='#' className='close' id='stat-weight-close-button'></a>
      </div>
      <ul className="character-stats" id="stat-weights">
        <li>
          <p className="character-stat">Stamina</p>
          <p id="stat-weight-stamina" className="character-stat-val">0</p>
        </li>
        <li>
          <p className="character-stat">Intellect</p>
          <p id="stat-weight-intellect" className="character-stat-val">0</p>
        </li>
        <li>
          <p className="character-stat">Spirit</p>
          <p id="stat-weight-spirit" className="character-stat-val">0</p>
        </li>
        <li>
          <p className="character-stat">Spell Power</p>
          <p id="stat-weight-spellPower" className="character-stat-val">0</p>
        </li>
        <li>
          <p className="character-stat">Shadow Power</p>
          <p id="stat-weight-shadowPower" className="character-stat-val">0</p>
        </li>
        <li>
          <p className="character-stat">Fire Power</p>
          <p id="stat-weight-firePower" className="character-stat-val">0</p>
        </li>
        <li>
          <p className="character-stat">Hit Rating</p>
          <p id="stat-weight-hitRating" className="character-stat-val">0</p>
        </li>
        <li>
          <p className="character-stat">Crit Rating</p>
          <p id="stat-weight-critRating" className="character-stat-val">0</p>
        </li>
        <li>
          <p className="character-stat">Haste Rating</p>
          <p id="stat-weight-hasteRating" className="character-stat-val">0</p>
        </li>
        <li>
          <p className="character-stat">Mp5</p>
          <p id="stat-weight-mp5" className="character-stat-val">0</p>
        </li>
        <li id="pawn-import-string">
          <p></p>
        </li>
      </ul>
    </section>
  )
}