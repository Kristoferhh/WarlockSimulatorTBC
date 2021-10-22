import Auras from './Auras';
import Settings from './Settings';
import Rotation from './Rotation';
import TalentTrees from './TalentTrees';
import StatWeights from './StatWeights';
import BreakdownTables from './BreakdownTables';
import EquippedItemsDisplay from './EquippedItemsDisplay';
import CombatLog from './CombatLog';
import DpsHistogram from './DpsHistogram';
import ImportExport from './ImportExport';
import GemSelection from './GemSelection';
import ItemSelection from './ItemSelection';

export default function Main() {
  return (
    <div id="main">
      <p id="pet-wip-note">Please report bugs in the #tbc-sim-report channel on the Warlock TBC discord.</p>
      <div>
        <Auras />
        <Rotation />
        <Settings />
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