
export default function Talents() {
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
      <section id="talents-section"></section>
    </div>
  )
}