// User clicks on the "Save New Profile" button
$('#save-new-profile-button').click(function () {
  // Remove apostrophes from the name to avoid it breaking the json object
  const profileName = $("input[name='profileName']").val().replaceAll("'", "")
  if (profileName.length <= 0) {
    alert('Missing profile name')
  } else if (profiles[profileName]) {
    alert('The profile "' + profileName + '" already exists')
  } else {
    saveProfile(profileName)
    localStorage.selectedProfile = profileName
    $("input[name='profileName']").val('')
    updateProfileSelection(profileName)
    drawProfileButtons()
  }
})

// User clicks on the "Save" profile button
$(document).on('click', '#save-profile-button', function () {
  saveProfile(localStorage.selectedProfile)
})

// User clicks on the "Delete" profile button
$('#delete-profile-button').click(function () {
  if (confirm('Are you sure you want to delete "' + localStorage.selectedProfile + "'?")) {
    delete profiles[localStorage.selectedProfile]
    localStorage.removeItem('selectedProfile')
    localStorage.profiles = JSON.stringify(profiles)
    drawProfileButtons()
    $('#save-profile-button').hide()
    $('#delete-profile-button').hide()
    $('#rename-profile-button').hide()
    if ($('.saved-profile').length == 0) {
      $('#saved-profiles').hide()
    }
  }
})

// User clicks on the "Rename" profile button
$('#rename-profile-button').click(function () {
  const newName = prompt('Enter the new name for profile "' + localStorage.selectedProfile + "'").replaceAll("'", "")
  if (newName !== null && newName.length > 0) {
    // Create a copy of the profile with the new name
    profiles[newName] = profiles[localStorage.selectedProfile]
    // Delete the old profile
    delete profiles[localStorage.selectedProfile]
    // Update localStorage
    localStorage.selectedProfile = newName
    localStorage.profiles = JSON.stringify(profiles)
    drawProfileButtons()
  }
})

// User clicks on one of their saved profiles
$(document).on('click', '.saved-profile', function () {
  const profileName = $(this).attr('data-name')
  updateProfileSelection(profileName)
  // Show the buttons to save, delete, and rename the profile
  $('#update-profile-div').show()
  // Load settings from the profile
  auras = profiles[profileName].auras
  localStorage.auras = JSON.stringify(auras)
  rotation = profiles[profileName].rotation
  localStorage.rotation = JSON.stringify(rotation)
  settings = profiles[profileName].simSettings
  localStorage.settings = JSON.stringify(settings)
  talents = profiles[profileName].talents
  localStorage.talents = JSON.stringify(talents)
  selectedItems = profiles[profileName].items
  localStorage.selectedItems = JSON.stringify(selectedItems)
  selectedGems = profiles[profileName].gems;
  localStorage.selectedGems = JSON.stringify(selectedGems);
  selectedEnchants = profiles[profileName].enchants
  localStorage.selectedEnchants = JSON.stringify(selectedEnchants)
  location.reload()
})

function drawProfileButtons () {
  $('.saved-profile').remove()
  for (const profile in profiles) {
    $('#saved-profiles ul').append("<li class='saved-profile' data-checked='" + (localStorage.selectedProfile == profile || false) + "' data-name='" + profile + "'>" + profile + '</li>')
  }
  // Show the profile fieldset if there are any profiles
  if ($('.saved-profile').length > 0) {
    $('#saved-profiles').show()
  }
}

function saveProfile (profileName) {
  profiles[profileName] = JSON.parse(JSON.stringify({
    auras: auras,
    rotation: rotation,
    simSettings: settings,
    talents: talents,
    items: selectedItems,
    gems: selectedGems,
    enchants: selectedEnchants
  }))
  localStorage.profiles = JSON.stringify(profiles)
}

// Called when the user creates a new profile or clicks on an existing one. It updates the 'selected' attribute for the profiles and displays the profile update buttons
function updateProfileSelection (profileName) {
  // De-select the previous profile
  if (localStorage.selectedProfile) {
    $(".saved-profile[data-name='" + localStorage.selectedProfile + "']").attr('data-checked', false)
  }
  localStorage.selectedProfile = profileName
  $(".saved-profile[data-name='" + profileName + "']").attr('data-selected', true)
  $('#update-profile-div').show()
}