import AuraSelection from './AuraSelection';
import SettingsDisplay from './SettingsDisplay';
import RotationSelection from './RotationSelection';
import TalentTrees from './TalentTrees';
import StatWeights from './StatWeights';
import BreakdownTables from './BreakdownTables';
import EquippedItemsDisplay from './EquippedItemsDisplay';
import CombatLog from './CombatLog';
import DpsHistogram from './DpsHistogram';
import ImportExport from './ImportExport';
import GemSelection from './GemSelection';
import ItemSelection from './ItemSelection';
import { useDispatch, useSelector } from 'react-redux';
import { RootState } from '../Redux/Store';
import { setGemSelectionTableVisibility } from '../Redux/UiSlice';

export default function Main() {
  const uiStore = useSelector((state: RootState) => state.ui);
  const dispatch = useDispatch();

  return (
    <div id="main" onClick={(e) => dispatch(setGemSelectionTableVisibility(false))}>
      <p id="pet-wip-note">Please report bugs in the #tbc-sim-report channel on the Warlock TBC discord. <a target='_blank' rel='noreferrer' href='https://discord.gg/5MX6j7nk7s'>Click here to join</a>.</p>
      <div>
        <AuraSelection />
        <RotationSelection />
        <SettingsDisplay />
        <TalentTrees />
        <StatWeights />
        <BreakdownTables />
      </div>
      <div>
        <EquippedItemsDisplay />
        <CombatLog />
        <DpsHistogram />
        <ImportExport />
        <GemSelection />
        <ItemSelection />
      </div>  
    </div>
  )
}