import { nanoid } from "@reduxjs/toolkit";
import { useState } from "react";
import { useDispatch, useSelector } from "react-redux";
import { deleteProfile, loadProfile, renameProfile, setProfile } from "../redux/PlayerSlice";
import { RootState } from "../redux/Store";
import { setImportExportWindowVisibility, setSelectedProfile, togglePhase } from "../redux/UiSlice";
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

  function callSetProfile(newProfile: boolean) {
    const name = newProfile ? profileName.replaceAll("'", "") : selectedProfileState;

    dispatch(setProfile({
      name: name,
      profile: {
        auras: playerStore.auras,
        items: playerStore.selectedItems,
        enchants: playerStore.selectedEnchants,
        gems: playerStore.selectedGems,
        talents: playerStore.talents,
        rotation: playerStore.rotation,
        simSettings: playerStore.settings,
      }
    }));

    if (newProfile) {
      dispatch(setSelectedProfile(name));
      setProfileName('');
    }
  }

  function profileClickHandler(params: [string, Profile]) {
    dispatch(setSelectedProfile(params[0]));
    dispatch(loadProfile(params[1]));
  }

  function deleteProfileHandler() {
    if (window.confirm(`Are you sure you want to delete profile '${selectedProfileState}'?`)) {
      dispatch(deleteProfile(selectedProfileState));
    }
  }

  function renameProfileHandler() {
    const newName = prompt(`Enter the new name for profile '${selectedProfileState}'`)?.replaceAll("'", "");

    if (newName != null && newName.length > 0 && newName !== selectedProfileState) {
      dispatch(renameProfile({
        oldName: selectedProfileState,
        newName: newName
      }));
      dispatch(setSelectedProfile(newName));
    }
  }

  return (
    <div id="profiles-and-sources">
      <fieldset id="profile-fieldset">
        <legend>Profile Options</legend>
        <input placeholder='E.g. "P3 Shadow BiS"' type="text" value={profileName} onChange={(e) => setProfileName(e.target.value)} name="profileName" />
        <button
          id="save-new-profile-button"
          onClick={(e) => callSetProfile(true)}
          disabled={profileName.replaceAll("'", "").length === 0}
        >Save New Profile</button>
        <div id="update-profile-div">
          <button
            style={{ display: playerStore.profiles[selectedProfileState] == null ? 'none' : '' }}
            id="save-profile-button"
            onClick={(e) => callSetProfile(false)}
          >Save</button>
          <button
            style={{ display: playerStore.profiles[selectedProfileState] == null ? 'none' : '' }}
            id="delete-profile-button"
            onClick={(e) => deleteProfileHandler()}
          >Delete</button>
          <button
            style={{ display: playerStore.profiles[selectedProfileState] == null ? 'none' : '' }}
            id="rename-profile-button"
            onClick={(e) => renameProfileHandler()}
          >Rename</button>
          <button id="import-export-button" onClick={(e) => dispatch(setImportExportWindowVisibility(true))}>Import/Export</button>
        </div>
      </fieldset>
      <fieldset id="saved-profiles" style={{ display: Object.keys(playerStore.profiles).length === 0 ? 'none' : '' }}>
        <legend>Saved Profiles</legend>
        <ul>
          {
            Object.entries(playerStore.profiles).map(profile =>
              <li
                key={nanoid()}
                className='saved-profile'
                data-checked={selectedProfileState === profile[0]}
                onClick={(e) => profileClickHandler(profile)}
              >
                {profile[0]}
              </li>
            )
          }
        </ul>
      </fieldset>

      <fieldset id="source-filters">
        <legend>Sources</legend>
        <ul>
          {
            phases.map(phase =>
              <li
                key={phase.phase}
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