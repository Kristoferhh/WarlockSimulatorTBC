import { useState } from "react";
import { useDispatch, useSelector } from "react-redux"
import { RootState } from "../redux/Store"
import { setImportExportWindowVisibility } from "../redux/UiSlice";

export default function ImportExport() {
  const playerState = useSelector((state: RootState) => state.player);
  const windowIsVisible = useSelector((state: RootState) => state.ui.importExportWindowVisible);
  const dispatch = useDispatch();
  const [contentString, setContentString] = useState('');

  function exportProfile() {
    setContentString(JSON.stringify({
      auras: playerState.auras,
      selectedGems: playerState.selectedGems,
      selectedItems: playerState.selectedItems,
      talents: playerState.talents,
      rotation: playerState.rotation,
      selectedEnchants: playerState.selectedEnchants,
      settings: playerState.settings
    }));
  }

  function importProfile() {
    const data = JSON.parse(contentString);

    if (data.auras)             localStorage.auras = JSON.stringify(data.auras);
    if (data.selectedGems)      localStorage.selectedGems = JSON.stringify(data.selectedGems);
    if (data.selectedItems)     localStorage.selectedItems = JSON.stringify(data.selectedItems);
    if (data.talents)           localStorage.talents = JSON.stringify(data.talents);
    if (data.rotation)          localStorage.rotation = JSON.stringify(data.rotation);
    if (data.selectedEnchants)  localStorage.selectedEnchants = JSON.stringify(data.selectedEnchants);
    if (data.settings)          localStorage.settings = JSON.stringify(data.settings);
    window.location.reload();
  }

  return(
    <div id="import-export-window" className="close-button-target" style={{ display: windowIsVisible ? '' : 'none' }}>
      <textarea onFocus={(e) => e.target.select()} value={contentString} onChange={(e) => setContentString(e.target.value)}></textarea>
      <br/>
      <button id="import-button" onClick={(e) => importProfile()}>Import</button>
      <button id="export-button" onClick={(e) => exportProfile()}>Export</button>
      <button id="export-close-button" onClick={(e) => { dispatch(setImportExportWindowVisibility(false)); setContentString(''); }}>Close</button>
    </div>
  )
}