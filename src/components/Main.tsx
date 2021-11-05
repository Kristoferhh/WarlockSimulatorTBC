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
import { useDispatch } from 'react-redux';
import { setGemSelectionTable } from '../redux/UiSlice';
import { InitialGemSelectionTableValue } from '../Types';
import ProfilesAndSources from './ProfilesAndSources';
import LanguageSelection from './LanguageSelection';
import packageJson from '../../package.json';

export default function Main() {
  const dispatch = useDispatch();

  return (
    <div id="main" onClick={(e) => dispatch(setGemSelectionTable(InitialGemSelectionTableValue))}>
      <div id='header'>
        <LanguageSelection />
        <p style={{marginRight: 'auto'}}>Please report bugs in the #tbc-sim-report channel on the Warlock TBC discord. <a target='_blank' rel='noreferrer' href='https://discord.gg/5MX6j7nk7s'>Click here to join</a>.</p>
        <p id='sim-version-number'>v{packageJson.version}</p>
      </div>
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
        <ProfilesAndSources />
        <ItemSelection />
      </div>  
    </div>
  )
}