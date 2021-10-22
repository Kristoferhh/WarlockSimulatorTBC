

export default function EquippedItemsDisplay() {
  return(
    <div id="currently-equipped-items-container" className="close-button-target">
      <div id="currently-equipped-items">
        <div>
          <a href='#' className='close' id='currently-equipped-items-close-button'></a>
        </div>
        <table>
          <colgroup>
            <col style={{width: '13%'}} />
            <col style={{width: '45%'}} />
            <col style={{width: '10%'}} />
            <col style={{width: '32%'}} />
          </colgroup>
          <thead>
            <th>Slot</th>
            <th>Name</th>
            <th></th>
            <th>Enchant</th>
          </thead>
          <tbody></tbody>
        </table>
      </div>
    </div>
  )
}