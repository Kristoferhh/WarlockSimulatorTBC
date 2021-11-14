import { nanoid } from "@reduxjs/toolkit";
import { useState } from "react";
import { useTranslation } from "react-i18next";
import { useDispatch, useSelector } from "react-redux";
import { getAurasStats, getBaseStats, getEnchantsStats, getGemsStats, getItemSetCounts, getItemsStats } from "../Common";
import { deleteProfile, renameProfile, setAurasStats, setBaseStats, setEnchantsStats, setGemsStats, setItemSetCounts, setItemsStats, setProfile, setRotationState, setSelectedAuras, setSelectedEnchants, setSelectedGems, setSelectedItems, setSettingsState, setTalentsState } from "../redux/PlayerSlice";
import { RootState } from "../redux/Store";
import { setImportExportWindowVisibility, setSelectedProfile, togglePhase } from "../redux/UiSlice";
import { Phase, Profile, Race } from "../Types";

const phases: { title: string, phase: Phase }[] = [
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
  const { t } = useTranslation();

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
    dispatch(setSelectedAuras(params[1].auras));
    dispatch(setSelectedGems(params[1].gems));
    dispatch(setSelectedItems(params[1].items));
    dispatch(setTalentsState(params[1].talents));
    dispatch(setRotationState(params[1].rotation));
    dispatch(setSelectedEnchants(params[1].enchants));
    dispatch(setSettingsState(params[1].simSettings));
    // Recalculate the player's stats
    dispatch(setBaseStats(getBaseStats(params[1].simSettings.race as Race)));
    dispatch(setAurasStats(getAurasStats(params[1].auras)));
    dispatch(setItemsStats(getItemsStats(params[1].items)));
    dispatch(setGemsStats(getGemsStats(params[1].items, params[1].gems)));
    dispatch(setEnchantsStats(getEnchantsStats(params[1].items, params[1].enchants)));
    dispatch(setItemSetCounts(getItemSetCounts(params[1].items)));
  }

  function deleteProfileHandler() {
    if (window.confirm(`Are you sure you want to delete profile '${selectedProfileState}'?`)) {
      dispatch(deleteProfile(selectedProfileState));
    }
  }

  function renameProfileHandler() {
    const newName =
      prompt(`Enter the new name for profile '${selectedProfileState}'`)?.replaceAll("'", "");

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
        <legend>{t('Profile Options')}</legend>
        <input
          placeholder={t('E.g. "P3 Shadow BiS"')}
          type="text"
          value={profileName}
          onChange={(e) => setProfileName(e.target.value)}
          name="profileName"
        />
        <button
          className='btn btn-primary btn-sm'
          id="save-new-profile-button"
          onClick={(e) => callSetProfile(true)}
          disabled={profileName.replaceAll("'", "").length === 0}
        >{t('Save New Profile')}</button>
        <div id="update-profile-div">
          <button
            className='btn btn-primary btn-sm'
            style={{
              display: playerStore.profiles[selectedProfileState] == null ? 'none' : ''
            }}
            id="save-profile-button"
            onClick={(e) => callSetProfile(false)}
          >{t('Save')}</button>
          {' '}
          <button
            className='btn btn-primary btn-sm'
            style={{
              display: playerStore.profiles[selectedProfileState] == null ? 'none' : ''
            }}
            id="delete-profile-button"
            onClick={(e) => deleteProfileHandler()}
          >{t('Delete')}</button>
          {' '}
          <button
            className='btn btn-primary btn-sm'
            style={{
              display: playerStore.profiles[selectedProfileState] == null ? 'none' : ''
            }}
            id="rename-profile-button"
            onClick={(e) => renameProfileHandler()}
          >{t('Rename')}</button>
          {' '}
          <button
            id="import-export-button"
            className='btn btn-primary btn-sm'
            onClick={(e) => dispatch(setImportExportWindowVisibility(true))}
          >{t('Import/Export')}</button>
        </div>
      </fieldset>
      <fieldset
        id="saved-profiles"
        style={{
          display: Object.keys(playerStore.profiles).length === 0 ? 'none' : ''
        }}
      >
        <legend>{t('Saved Profiles')}</legend>
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
        <legend>{t('Sources')}</legend>
        <ul>
          {
            phases.map(phase =>
              <li
                key={phase.phase}
                data-checked={sourcesState.phase[phase.phase] === true}
                className='phase-btn'
                onClick={() => dispatch(togglePhase(phase.phase))}
              >
                {t(phase.title)}
              </li>
            )
          }
        </ul>
      </fieldset>
    </div>
  );
}