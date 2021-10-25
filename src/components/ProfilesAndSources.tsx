import { useState } from "react";
import { useDispatch, useSelector } from "react-redux";
import { setProfile } from "../redux/PlayerSlice";
import { RootState } from "../redux/Store";
import { setSelectedProfile, togglePhase } from "../redux/UiSlice";
import { Phase, Profile } from "../Types";

const phases: {title: string, phase: Phase}[] = [
  { title: 'Classic', phase: 0 },
  { title: 'P1', phase: 1 },
  { title: 'P2', phase: 2 },
  { title: 'P3', phase: 3 },
  { title: 'P4', phase: 4 },
  { title: 'P5', phase: 5 },
]

export default function ProfilesAndSources() {
  const playerStore = useSelector((state: RootState) => state.player);
  const selectedProfileState = useSelector((state: RootState) => state.ui.selectedProfile);
  const sourcesState = useSelector((state: RootState) => state.ui.sources);
  const dispatch = useDispatch();
  const [profileName, setProfileName] = useState('');

  function callSetProfile() {
    dispatch(setProfile({
      name: profileName,
      profile: {
        auras: playerStore.auras,
        selectedItems: playerStore.selectedItems,
        selectedEnchants: playerStore.selectedEnchants,
        selectedGems: playerStore.selectedGems,
        talents: playerStore.talents,
        rotation: playerStore.rotation,
        settings: playerStore.settings
      }
    }));
  }

  function profileClickHandler(params: [string, Profile]) {
    dispatch(setSelectedProfile(params[0]));
  }

  return (
    <div id="profiles-and-sources">
      <fieldset id="profile-fieldset">
        <legend>Profile Options</legend>
        <input placeholder='E.g. "P3 Shadow BiS"' type="text" onChange={(e) => setProfileName(e.target.value)} name="profileName" />
        <button id="save-new-profile-button" onClick={(e) => callSetProfile()} disabled={profileName.length === 0}>Save New Profile</button>
        <div id="update-profile-div">
          <button style={{display: 'none'}} id="save-profile-button">Save</button>
          <button style={{display: 'none'}} id="delete-profile-button">Delete</button>
          <button style={{display: 'none'}} id="rename-profile-button">Rename</button>
          <button id="import-export-button">Import/Export</button>
        </div>
      </fieldset>
      <fieldset id="saved-profiles" style={{display: Object.keys(playerStore.profiles).length === 0 ? 'none' : ''}}>
        <legend>Saved Profiles</legend>
        <ul>
          {
            Object.entries(playerStore.profiles).map((profile, i) =>
              <li key={i} className='saved-profile' data-checked={selectedProfileState === profile[0]} onClick={(e) => profileClickHandler(profile)}>{profile[0]}</li>
            )
          }
        </ul>
      </fieldset>

      <fieldset id="source-filters">
        <legend>Sources</legend>
        <ul>
          {
            phases.map((phase, i) =>
              <li
                key={i}
                data-checked={sourcesState.phase[phase.phase] === true}
                className='phase-btn'
                onClick={() => dispatch(togglePhase(phase.phase))}
              >
                {phase.title}
              </li>
            )
          }
        </ul>
      </fieldset>
    </div>
  );
}