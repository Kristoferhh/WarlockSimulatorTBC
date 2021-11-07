import { useState } from "react";
import { useDispatch, useSelector } from "react-redux"
import { getAurasStats, getBaseStats, getEnchantsStats, getGemsStats, getItemSetCounts, getItemsStats } from "../Common";
import { setAurasStats, setBaseStats, setEnchantsStats, setGemsStats, setItemSetCounts, setItemsStats, setRotationState, setSelectedAuras, setSelectedEnchants, setSelectedGems, setSelectedItems, setSettingsState, setTalentsState } from "../redux/PlayerSlice";
import { RootState } from "../redux/Store"
import { setImportExportWindowVisibility } from "../redux/UiSlice";
import { Race } from "../Types";

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
    try {
      const data = JSON.parse(contentString);

      if (data.auras) {
        dispatch(setSelectedAuras(data.auras));
        dispatch(setAurasStats(getAurasStats(data.auras)));
      }
      if (data.selectedItems) {
        dispatch(setSelectedItems(data.selectedItems));
        dispatch(setItemsStats(getItemsStats(data.selectedItems)));
        dispatch(setItemSetCounts(getItemSetCounts(data.selectedItems)));
      }
      if (data.selectedEnchants) {
        dispatch(setSelectedEnchants(data.selectedEnchants));
        dispatch(setEnchantsStats(getEnchantsStats(data.selectedItems ? data.selectedItems : playerState.selectedItems, data.selectedEnchants)));
      }
      if (data.selectedGems) {
        dispatch(setSelectedGems(data.selectedGems));
        dispatch(setGemsStats(getGemsStats(data.selectedItems ? data.selectedItems : playerState.selectedItems, data.selectedGems)));
      }
      if (data.talents) {
        dispatch(setTalentsState(data.talents));
      }
      if (data.rotation) {
        dispatch(setRotationState(data.rotation));
      }
      if (data.settings) {
        dispatch(setSettingsState(data.settings));
        dispatch(setBaseStats(getBaseStats(data.settings.race as Race)));
      }
      
      dispatch(setImportExportWindowVisibility(false));
    } catch (error) {
      alert(`Error importing profile: ${error}`);
    }
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