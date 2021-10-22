

export default function BreakdownTables() {
  return(
    <div>
      <section className="breakdown-section" id="damage-breakdown-section">
        <table className="breakdown-table tablesorter" id="damage-breakdown-table" data-sortlist='[[1,1]]'>
          <thead>
            <tr>
              <th>Name</th>
              <th>Damage</th>
              <th>Casts</th>
              <th>Avg Cast</th>
              <th>Crit %</th>
              <th>Miss %</th>
              <th id='damage-breakdown-dodge'>Dodge %</th>
              <th id='damage-breakdown-glancing'>Glancing %</th>
              <th>Dps</th>
            </tr>
          </thead>
          <tbody></tbody>
        </table>
      </section>
      <section className="breakdown-section" id="mana-gain-breakdown-section">
        <table className="breakdown-table tablesorter" id="mana-gain-breakdown-table" data-sortlist='[[1,1]]'>
          <thead>
            <tr>
              <th>Name</th>
              <th>% Of Total Gain</th>
              <th>Casts</th>
              <th>Avg Cast</th>
            </tr>
          </thead>
          <tbody></tbody>
        </table>
      </section>
      <section className="breakdown-section" id="aura-breakdown-section">
        <table className="breakdown-table tablesorter" id="aura-breakdown-table" data-sortlist='[[2,1]]'>
          <thead>
            <tr>
              <th>Name</th>
              <th>Count</th>
              <th>Uptime</th>
            </tr>
          </thead>
          <tbody></tbody>
        </table>
      </section>
    </div>
  )
}