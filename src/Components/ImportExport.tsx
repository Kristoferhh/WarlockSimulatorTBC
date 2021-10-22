
export default function ImportExport() {
  return(
    <div id="import-export-window" className="close-button-target" style={{display: 'none'}}>
      <textarea></textarea>
      <br/>
      <button id="import-button">Import</button>
      <button id="export-button">Export</button>
      <button id="export-close-button">Close</button>
    </div>
  )
}