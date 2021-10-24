import { useState } from "react";
import { useDispatch, useSelector } from "react-redux";
import { setProfile } from "../Redux/PlayerSlice";
import { RootState } from "../Redux/Store";


export default function Profiles() {
  const playerStore = useSelector((state: RootState) => state.player);
  const dispatch = useDispatch();
  const [profileName, setProfileName] = useState('');

  function callSetProfile() {
    /*dispatch(setProfile({
      name: profileName,
      profile: {
        auras: playerStore.auras,
        selectedItems: playerStore.selectedItems,
        selectedEnchants: playerStore.selectedEnchants,
        talents: playerStore.talents,
        rotation: playerStore.rotation,
        settings: playerStore.settings
      }
    }))*/
  }

  return (
    <div>
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
      <fieldset id="saved-profiles" style={{display: 'none'}}>
        <legend>Saved Profiles</legend>
        <ul></ul>
      </fieldset>
    </div>
  );
}