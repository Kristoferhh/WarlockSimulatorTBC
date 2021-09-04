#include "simulation.h"
#include <vector>
#include <stdlib.h>
#include "common.h"
#include "bindings.h"
#include "spell.h"
#include <algorithm>
#include <chrono>

Simulation::Simulation(Player* _player, SimulationSettings* simulationSettings) : player(_player), settings(simulationSettings) {}

void Simulation::passTime()
{
    double time = player->castTimeRemaining;
    if (time == 0 || (player->gcdRemaining > 0 && player->gcdRemaining < time)) time = player->gcdRemaining;

    // Find the lowest time until the next action needs to be taken
    // Spells
    if (player->spells->LifeTap->cooldownRemaining > 0 && player->spells->LifeTap->cooldownRemaining < time) time = player->spells->LifeTap->cooldownRemaining;
    if (player->spells->SeedOfCorruption->cooldownRemaining > 0 && player->spells->SeedOfCorruption->cooldownRemaining < time) time = player->spells->SeedOfCorruption->cooldownRemaining;
    if (player->spells->ShadowBolt->cooldownRemaining > 0 && player->spells->ShadowBolt->cooldownRemaining < time) time = player->spells->ShadowBolt->cooldownRemaining;
    if (player->spells->Incinerate->cooldownRemaining > 0 && player->spells->Incinerate->cooldownRemaining < time) time = player->spells->Incinerate->cooldownRemaining;
    if (player->spells->SearingPain->cooldownRemaining > 0 && player->spells->SearingPain->cooldownRemaining < time) time = player->spells->SearingPain->cooldownRemaining;
    if (player->spells->Corruption->cooldownRemaining > 0 && player->spells->Corruption->cooldownRemaining < time) time = player->spells->Corruption->cooldownRemaining;
    if (player->spells->UnstableAffliction->cooldownRemaining > 0 && player->spells->UnstableAffliction->cooldownRemaining < time) time = player->spells->UnstableAffliction->cooldownRemaining;
    if (player->spells->SiphonLife->cooldownRemaining > 0 && player->spells->SiphonLife->cooldownRemaining < time) time = player->spells->SiphonLife->cooldownRemaining;
    if (player->spells->Immolate->cooldownRemaining > 0 && player->spells->Immolate->cooldownRemaining < time) time = player->spells->Immolate->cooldownRemaining;
    if (player->spells->CurseOfAgony->cooldownRemaining > 0 && player->spells->CurseOfAgony->cooldownRemaining < time) time = player->spells->CurseOfAgony->cooldownRemaining;
    if (player->spells->CurseOfTheElements->cooldownRemaining > 0 && player->spells->CurseOfTheElements->cooldownRemaining < time) time = player->spells->CurseOfTheElements->cooldownRemaining;
    if (player->spells->CurseOfRecklessness->cooldownRemaining > 0 && player->spells->CurseOfRecklessness->cooldownRemaining < time) time = player->spells->CurseOfRecklessness->cooldownRemaining;
    if (player->spells->CurseOfDoom->cooldownRemaining > 0 && player->spells->CurseOfDoom->cooldownRemaining < time) time = player->spells->CurseOfDoom->cooldownRemaining;
    if (player->spells->Conflagrate->cooldownRemaining > 0 && player->spells->Conflagrate->cooldownRemaining < time) time = player->spells->Conflagrate->cooldownRemaining;
    if (player->spells->Shadowburn->cooldownRemaining > 0 && player->spells->Shadowburn->cooldownRemaining < time) time = player->spells->Shadowburn->cooldownRemaining;
    if (player->spells->DeathCoil->cooldownRemaining > 0 && player->spells->DeathCoil->cooldownRemaining < time) time = player->spells->DeathCoil->cooldownRemaining;
    if (player->spells->Shadowfury->cooldownRemaining > 0 && player->spells->Shadowfury->cooldownRemaining < time) time = player->spells->Shadowfury->cooldownRemaining;
    if (player->spells->AmplifyCurse->cooldownRemaining > 0 && player->spells->AmplifyCurse->cooldownRemaining < time) time = player->spells->AmplifyCurse->cooldownRemaining;
    if (player->spells->DarkPact->cooldownRemaining > 0 && player->spells->DarkPact->cooldownRemaining < time) time = player->spells->DarkPact->cooldownRemaining;
    if (player->spells->DestructionPotion->cooldownRemaining > 0 && player->spells->DestructionPotion->cooldownRemaining < time) time = player->spells->DestructionPotion->cooldownRemaining;
    if (player->spells->SuperManaPotion->cooldownRemaining > 0 && player->spells->SuperManaPotion->cooldownRemaining < time) time = player->spells->SuperManaPotion->cooldownRemaining;
    if (player->spells->DemonicRune->cooldownRemaining > 0 && player->spells->DemonicRune->cooldownRemaining < time) time = player->spells->DemonicRune->cooldownRemaining;
    if (player->spells->FlameCap->cooldownRemaining > 0 && player->spells->FlameCap->cooldownRemaining < time) time = player->spells->FlameCap->cooldownRemaining;
    if (player->spells->BloodFury->cooldownRemaining > 0 && player->spells->BloodFury->cooldownRemaining < time) time = player->spells->BloodFury->cooldownRemaining;
    if (player->spells->DrumsOfBattle->cooldownRemaining > 0 && player->spells->DrumsOfBattle->cooldownRemaining < time) time = player->spells->DrumsOfBattle->cooldownRemaining;
    if (player->spells->DrumsOfWar->cooldownRemaining > 0 && player->spells->DrumsOfWar->cooldownRemaining < time) time = player->spells->DrumsOfWar->cooldownRemaining;
    if (player->spells->DrumsOfRestoration->cooldownRemaining > 0 && player->spells->DrumsOfRestoration->cooldownRemaining < time) time = player->spells->DrumsOfRestoration->cooldownRemaining;
    if (player->spells->BladeOfWizardry->cooldownRemaining > 0 && player->spells->BladeOfWizardry->cooldownRemaining < time) time = player->spells->BladeOfWizardry->cooldownRemaining;
    if (player->spells->ShatteredSunPendantOfAcumen->cooldownRemaining > 0 && player->spells->ShatteredSunPendantOfAcumen->cooldownRemaining < time) time = player->spells->ShatteredSunPendantOfAcumen->cooldownRemaining;
    if (player->spells->RobeOfTheElderScribes->cooldownRemaining > 0 && player->spells->RobeOfTheElderScribes->cooldownRemaining < time) time = player->spells->RobeOfTheElderScribes->cooldownRemaining;
    if (player->spells->MysticalSkyfireDiamond->cooldownRemaining > 0 && player->spells->MysticalSkyfireDiamond->cooldownRemaining < time) time = player->spells->MysticalSkyfireDiamond->cooldownRemaining;
    if (player->spells->InsightfulEarthstormDiamond->cooldownRemaining > 0 && player->spells->InsightfulEarthstormDiamond->cooldownRemaining < time) time = player->spells->InsightfulEarthstormDiamond->cooldownRemaining;
    if (player->spells->TimbalsFocusingCrystal->cooldownRemaining > 0 && player->spells->TimbalsFocusingCrystal->cooldownRemaining < time) time = player->spells->TimbalsFocusingCrystal->cooldownRemaining;
    if (player->spells->MarkOfDefiance->cooldownRemaining > 0 && player->spells->MarkOfDefiance->cooldownRemaining < time) time = player->spells->MarkOfDefiance->cooldownRemaining;
    if (player->spells->TheLightningCapacitor->cooldownRemaining > 0 && player->spells->TheLightningCapacitor->cooldownRemaining < time) time = player->spells->TheLightningCapacitor->cooldownRemaining;
    if (player->spells->QuagmirransEye->cooldownRemaining > 0 && player->spells->QuagmirransEye->cooldownRemaining < time) time = player->spells->QuagmirransEye->cooldownRemaining;
    if (player->spells->ShiffarsNexusHorn->cooldownRemaining > 0 && player->spells->ShiffarsNexusHorn->cooldownRemaining < time) time = player->spells->ShiffarsNexusHorn->cooldownRemaining;
    if (player->spells->SextantOfUnstableCurrents->cooldownRemaining > 0 && player->spells->SextantOfUnstableCurrents->cooldownRemaining < time) time = player->spells->SextantOfUnstableCurrents->cooldownRemaining;
    if (player->spells->BandOfTheEternalSage->cooldownRemaining > 0 && player->spells->BandOfTheEternalSage->cooldownRemaining < time) time = player->spells->BandOfTheEternalSage->cooldownRemaining;
    for (std::vector<Spell*>::iterator it = player->spells->PowerInfusion.begin(); it != player->spells->PowerInfusion.end(); it++)
    {
        if ((*it)->cooldownRemaining > 0 && (*it)->cooldownRemaining < time) time = (*it)->cooldownRemaining;
    }
    for (std::vector<Spell*>::iterator it = player->spells->Bloodlust.begin(); it != player->spells->Bloodlust.end(); it++)
    {
        if ((*it)->cooldownRemaining > 0 && (*it)->cooldownRemaining < time) time = (*it)->cooldownRemaining;
    }
    for (std::vector<Spell*>::iterator it = player->spells->Innervate.begin(); it != player->spells->Innervate.end(); it++)
    {
        if ((*it)->cooldownRemaining > 0 && (*it)->cooldownRemaining < time) time = (*it)->cooldownRemaining;
    }

    // Auras & Dots
    if (player->auras->Corruption->active && player->auras->Corruption->tickTimerRemaining < time) time = player->auras->Corruption->tickTimerRemaining;
    if (player->auras->UnstableAffliction->active && player->auras->UnstableAffliction->tickTimerRemaining < time) time = player->auras->UnstableAffliction->tickTimerRemaining;
    if (player->auras->SiphonLife->active && player->auras->SiphonLife->tickTimerRemaining < time) time = player->auras->SiphonLife->tickTimerRemaining;
    if (player->auras->Immolate->active && player->auras->Immolate->tickTimerRemaining < time) time = player->auras->Immolate->tickTimerRemaining;
    if (player->auras->CurseOfAgony->active && player->auras->CurseOfAgony->tickTimerRemaining < time) time = player->auras->CurseOfAgony->tickTimerRemaining;
    if (player->auras->CurseOfDoom->active && player->auras->CurseOfDoom->tickTimerRemaining < time) time = player->auras->CurseOfDoom->tickTimerRemaining;
    if (player->auras->ImprovedShadowBolt->hasDuration && player->auras->ImprovedShadowBolt->active && player->auras->ImprovedShadowBolt->durationRemaining < time) time = player->auras->ImprovedShadowBolt->durationRemaining;
    if (player->auras->CurseOfTheElements->hasDuration && player->auras->CurseOfTheElements->active && player->auras->CurseOfTheElements->durationRemaining < time) time = player->auras->CurseOfTheElements->durationRemaining;
    if (player->auras->CurseOfRecklessness->hasDuration && player->auras->CurseOfRecklessness->active && player->auras->CurseOfRecklessness->durationRemaining < time) time = player->auras->CurseOfRecklessness->durationRemaining;
    if (player->auras->ShadowTrance->hasDuration && player->auras->ShadowTrance->active && player->auras->ShadowTrance->durationRemaining < time) time = player->auras->ShadowTrance->durationRemaining;
    if (player->auras->AmplifyCurse->hasDuration && player->auras->AmplifyCurse->active && player->auras->AmplifyCurse->durationRemaining < time) time = player->auras->AmplifyCurse->durationRemaining;
    if (player->auras->PowerInfusion->hasDuration && player->auras->PowerInfusion->active && player->auras->PowerInfusion->durationRemaining < time) time = player->auras->PowerInfusion->durationRemaining;
    if (player->auras->Innervate->hasDuration && player->auras->Innervate->active && player->auras->Innervate->durationRemaining < time) time = player->auras->Innervate->durationRemaining;
    if (player->auras->BloodFury->hasDuration && player->auras->BloodFury->active && player->auras->BloodFury->durationRemaining < time) time = player->auras->BloodFury->durationRemaining;
    if (player->auras->DestructionPotion->hasDuration && player->auras->DestructionPotion->active && player->auras->DestructionPotion->durationRemaining < time) time = player->auras->DestructionPotion->durationRemaining;
    if (player->auras->FlameCap->hasDuration && player->auras->FlameCap->active && player->auras->FlameCap->durationRemaining < time) time = player->auras->FlameCap->durationRemaining;
    if (player->auras->Bloodlust->hasDuration && player->auras->Bloodlust->active && player->auras->Bloodlust->durationRemaining < time) time = player->auras->Bloodlust->durationRemaining;
    if (player->auras->DrumsOfBattle->hasDuration && player->auras->DrumsOfBattle->active && player->auras->DrumsOfBattle->durationRemaining < time) time = player->auras->DrumsOfBattle->durationRemaining;
    if (player->auras->DrumsOfWar->hasDuration && player->auras->DrumsOfWar->active && player->auras->DrumsOfWar->durationRemaining < time) time = player->auras->DrumsOfWar->durationRemaining;
    if (player->auras->DrumsOfRestoration->hasDuration && player->auras->DrumsOfRestoration->active && player->auras->DrumsOfRestoration->durationRemaining < time) time = player->auras->DrumsOfRestoration->durationRemaining;
    if (player->auras->BandOfTheEternalSage->hasDuration && player->auras->BandOfTheEternalSage->active && player->auras->BandOfTheEternalSage->durationRemaining < time) time = player->auras->BandOfTheEternalSage->durationRemaining;
    if (player->auras->WrathOfCenarius->hasDuration && player->auras->WrathOfCenarius->active && player->auras->WrathOfCenarius->durationRemaining < time) time = player->auras->WrathOfCenarius->durationRemaining;
    if (player->auras->BladeOfWizardry->hasDuration && player->auras->BladeOfWizardry->active && player->auras->BladeOfWizardry->durationRemaining < time) time = player->auras->BladeOfWizardry->durationRemaining;
    if (player->auras->ShatteredSunPendantOfAcumen->hasDuration && player->auras->ShatteredSunPendantOfAcumen->active && player->auras->ShatteredSunPendantOfAcumen->durationRemaining < time) time = player->auras->ShatteredSunPendantOfAcumen->durationRemaining;
    if (player->auras->RobeOfTheElderScribes->hasDuration && player->auras->RobeOfTheElderScribes->active && player->auras->RobeOfTheElderScribes->durationRemaining < time) time = player->auras->RobeOfTheElderScribes->durationRemaining;
    if (player->auras->MysticalSkyfireDiamond->hasDuration && player->auras->MysticalSkyfireDiamond->active && player->auras->MysticalSkyfireDiamond->durationRemaining < time) time = player->auras->MysticalSkyfireDiamond->durationRemaining;
    if (player->auras->EyeOfMagtheridon->hasDuration && player->auras->EyeOfMagtheridon->active && player->auras->EyeOfMagtheridon->durationRemaining < time) time = player->auras->EyeOfMagtheridon->durationRemaining;
    if (player->auras->SextantOfUnstableCurrents->hasDuration && player->auras->SextantOfUnstableCurrents->active && player->auras->SextantOfUnstableCurrents->durationRemaining < time) time = player->auras->SextantOfUnstableCurrents->durationRemaining;
    if (player->auras->QuagmirransEye->hasDuration && player->auras->QuagmirransEye->active && player->auras->QuagmirransEye->durationRemaining < time) time = player->auras->QuagmirransEye->durationRemaining;
    if (player->auras->ShiffarsNexusHorn->hasDuration && player->auras->ShiffarsNexusHorn->active && player->auras->ShiffarsNexusHorn->durationRemaining < time) time = player->auras->ShiffarsNexusHorn->durationRemaining;
    if (player->auras->AshtongueTalismanOfShadows->hasDuration && player->auras->AshtongueTalismanOfShadows->active && player->auras->AshtongueTalismanOfShadows->durationRemaining < time) time = player->auras->AshtongueTalismanOfShadows->durationRemaining;
    if (player->auras->DarkmoonCardCrusade->hasDuration && player->auras->DarkmoonCardCrusade->active && player->auras->DarkmoonCardCrusade->durationRemaining < time) time = player->auras->DarkmoonCardCrusade->durationRemaining;
    if (player->auras->TheLightningCapacitor->hasDuration && player->auras->TheLightningCapacitor->active && player->auras->TheLightningCapacitor->durationRemaining < time) time = player->auras->TheLightningCapacitor->durationRemaining;
    if (player->auras->Flameshadow->hasDuration && player->auras->Flameshadow->active && player->auras->Flameshadow->durationRemaining < time) time = player->auras->Flameshadow->durationRemaining; // Shadowpower
    if (player->auras->Shadowflame->hasDuration && player->auras->Shadowflame->active && player->auras->Shadowflame->durationRemaining < time) time = player->auras->Shadowflame->durationRemaining; // Firepower
    if (player->auras->Spellstrike->hasDuration && player->auras->Spellstrike->active && player->auras->Spellstrike->durationRemaining < time) time = player->auras->Spellstrike->durationRemaining;
    if (player->auras->ManaEtched4Set->hasDuration && player->auras->ManaEtched4Set->active && player->auras->ManaEtched4Set->durationRemaining < time) time = player->auras->ManaEtched4Set->durationRemaining;

    // MP5
    if (player->mp5Timer < time)
    {
        time = player->mp5Timer;
    }

    // Trinkets
    for (int i = 0; i < player->trinkets.size(); i++)
    {
        if (player->trinkets[i]->active && player->trinkets[i]->durationRemaining < time)
        {
            time = player->trinkets[i]->durationRemaining;
        }
        if (player->trinkets[i]->cooldownRemaining > 0 && player->trinkets[i]->cooldownRemaining < time)
        {
            time = player->trinkets[i]->cooldownRemaining;
        }
    }

    // Pass time
    // This needs to be the first modified value since the time in combat needs to be updated before spells start dealing damage/auras expiring etc. for the combat logging.
    player->fightTime += time;
    player->castTimeRemaining -= time;

    // Auras need to tick before Spells because otherwise you'll, for example, finish casting Corruption and then immediately afterwards, in the same millisecond, immediately tick down the aura
    // This was also causing buffs like e.g. the t4 4pc buffs to expire sooner than they should.
    // Auras
    if (player->auras->Corruption->active && player->auras->Corruption->tickTimerRemaining > 0) player->auras->Corruption->tick(time);
    if (player->auras->UnstableAffliction->active && player->auras->UnstableAffliction->tickTimerRemaining > 0) player->auras->UnstableAffliction->tick(time);
    if (player->auras->SiphonLife->active && player->auras->SiphonLife->tickTimerRemaining > 0) player->auras->SiphonLife->tick(time);
    if (player->auras->Immolate->active && player->auras->Immolate->tickTimerRemaining > 0) player->auras->Immolate->tick(time);
    if (player->auras->CurseOfAgony->active && player->auras->CurseOfAgony->tickTimerRemaining > 0) player->auras->CurseOfAgony->tick(time);
    if (player->auras->CurseOfDoom->active && player->auras->CurseOfDoom->tickTimerRemaining > 0) player->auras->CurseOfDoom->tick(time);
    if (player->auras->ImprovedShadowBolt->active && player->auras->ImprovedShadowBolt->hasDuration) player->auras->ImprovedShadowBolt->tick(time);
    if (player->auras->CurseOfTheElements->active && player->auras->CurseOfTheElements->hasDuration) player->auras->CurseOfTheElements->tick(time);
    if (player->auras->CurseOfRecklessness->active && player->auras->CurseOfRecklessness->hasDuration) player->auras->CurseOfRecklessness->tick(time);
    if (player->auras->ShadowTrance->active && player->auras->ShadowTrance->hasDuration) player->auras->ShadowTrance->tick(time);
    if (player->auras->AmplifyCurse->active && player->auras->AmplifyCurse->hasDuration) player->auras->AmplifyCurse->tick(time);
    if (player->auras->PowerInfusion->active && player->auras->PowerInfusion->hasDuration) player->auras->PowerInfusion->tick(time);
    if (player->auras->Innervate->active && player->auras->Innervate->hasDuration) player->auras->Innervate->tick(time);
    if (player->auras->BloodFury->active && player->auras->BloodFury->hasDuration) player->auras->BloodFury->tick(time);
    if (player->auras->DestructionPotion->active && player->auras->DestructionPotion->hasDuration) player->auras->DestructionPotion->tick(time);
    if (player->auras->FlameCap->active && player->auras->FlameCap->hasDuration) player->auras->FlameCap->tick(time);
    if (player->auras->Bloodlust->active && player->auras->Bloodlust->hasDuration) player->auras->Bloodlust->tick(time);
    if (player->auras->DrumsOfBattle->active && player->auras->DrumsOfBattle->hasDuration) player->auras->DrumsOfBattle->tick(time);
    if (player->auras->DrumsOfWar->active && player->auras->DrumsOfWar->hasDuration) player->auras->DrumsOfWar->tick(time);
    if (player->auras->DrumsOfRestoration->active && player->auras->DrumsOfRestoration->hasDuration) player->auras->DrumsOfRestoration->tick(time);
    if (player->auras->BandOfTheEternalSage->active && player->auras->BandOfTheEternalSage->hasDuration) player->auras->BandOfTheEternalSage->tick(time);
    if (player->auras->WrathOfCenarius->active && player->auras->WrathOfCenarius->hasDuration) player->auras->WrathOfCenarius->tick(time);
    if (player->auras->BladeOfWizardry->active && player->auras->BladeOfWizardry->hasDuration) player->auras->BladeOfWizardry->tick(time);
    if (player->auras->ShatteredSunPendantOfAcumen->active && player->auras->ShatteredSunPendantOfAcumen->hasDuration) player->auras->ShatteredSunPendantOfAcumen->tick(time);
    if (player->auras->RobeOfTheElderScribes->active && player->auras->RobeOfTheElderScribes->hasDuration) player->auras->RobeOfTheElderScribes->tick(time);
    if (player->auras->MysticalSkyfireDiamond->active && player->auras->MysticalSkyfireDiamond->hasDuration) player->auras->MysticalSkyfireDiamond->tick(time);
    if (player->auras->EyeOfMagtheridon->active && player->auras->EyeOfMagtheridon->hasDuration) player->auras->EyeOfMagtheridon->tick(time);
    if (player->auras->SextantOfUnstableCurrents->active && player->auras->SextantOfUnstableCurrents->hasDuration) player->auras->SextantOfUnstableCurrents->tick(time);
    if (player->auras->QuagmirransEye->active && player->auras->QuagmirransEye->hasDuration) player->auras->QuagmirransEye->tick(time);
    if (player->auras->ShiffarsNexusHorn->active && player->auras->ShiffarsNexusHorn->hasDuration) player->auras->ShiffarsNexusHorn->tick(time);
    if (player->auras->AshtongueTalismanOfShadows->active && player->auras->AshtongueTalismanOfShadows->hasDuration) player->auras->AshtongueTalismanOfShadows->tick(time);
    if (player->auras->DarkmoonCardCrusade->active && player->auras->DarkmoonCardCrusade->hasDuration) player->auras->DarkmoonCardCrusade->tick(time);
    if (player->auras->TheLightningCapacitor->active && player->auras->TheLightningCapacitor->hasDuration) player->auras->TheLightningCapacitor->tick(time);
    if (player->auras->Flameshadow->active && player->auras->Flameshadow->hasDuration) player->auras->Flameshadow->tick(time);
    if (player->auras->Shadowflame->active && player->auras->Shadowflame->hasDuration) player->auras->Shadowflame->tick(time);
    if (player->auras->Spellstrike->active && player->auras->Spellstrike->hasDuration) player->auras->Spellstrike->tick(time);
    if (player->auras->ManaEtched4Set->active && player->auras->ManaEtched4Set->hasDuration) player->auras->ManaEtched4Set->tick(time);

    // Spells
    if (player->spells->LifeTap->cooldownRemaining > 0 || player->spells->LifeTap->casting) player->spells->LifeTap->tick(time);
    if (player->spells->SeedOfCorruption->cooldownRemaining > 0 || player->spells->SeedOfCorruption->casting) player->spells->SeedOfCorruption->tick(time);
    if (player->spells->ShadowBolt->cooldownRemaining > 0 || player->spells->ShadowBolt->casting) player->spells->ShadowBolt->tick(time);
    if (player->spells->Incinerate->cooldownRemaining > 0 || player->spells->Incinerate->casting) player->spells->Incinerate->tick(time);
    if (player->spells->SearingPain->cooldownRemaining > 0 || player->spells->SearingPain->casting) player->spells->SearingPain->tick(time);
    if (player->spells->Corruption->cooldownRemaining > 0 || player->spells->Corruption->casting) player->spells->Corruption->tick(time);
    if (player->spells->UnstableAffliction->cooldownRemaining > 0 || player->spells->UnstableAffliction->casting) player->spells->UnstableAffliction->tick(time);
    if (player->spells->SiphonLife->cooldownRemaining > 0 || player->spells->SiphonLife->casting) player->spells->SiphonLife->tick(time);
    if (player->spells->Immolate->cooldownRemaining > 0 || player->spells->Immolate->casting) player->spells->Immolate->tick(time);
    if (player->spells->CurseOfAgony->cooldownRemaining > 0 || player->spells->CurseOfAgony->casting) player->spells->CurseOfAgony->tick(time);
    if (player->spells->CurseOfTheElements->cooldownRemaining > 0 || player->spells->CurseOfTheElements->casting) player->spells->CurseOfTheElements->tick(time);
    if (player->spells->CurseOfRecklessness->cooldownRemaining > 0 || player->spells->CurseOfRecklessness->casting) player->spells->CurseOfRecklessness->tick(time);
    if (player->spells->CurseOfDoom->cooldownRemaining > 0 || player->spells->CurseOfDoom->casting) player->spells->CurseOfDoom->tick(time);
    if (player->spells->Conflagrate->cooldownRemaining > 0 || player->spells->Conflagrate->casting) player->spells->Conflagrate->tick(time);
    if (player->spells->Shadowburn->cooldownRemaining > 0 || player->spells->Shadowburn->casting) player->spells->Shadowburn->tick(time);
    if (player->spells->DeathCoil->cooldownRemaining > 0 || player->spells->DeathCoil->casting) player->spells->DeathCoil->tick(time);
    if (player->spells->Shadowfury->cooldownRemaining > 0 || player->spells->Shadowfury->casting) player->spells->Shadowfury->tick(time);
    if (player->spells->AmplifyCurse->cooldownRemaining > 0 || player->spells->AmplifyCurse->casting) player->spells->AmplifyCurse->tick(time);
    if (player->spells->DarkPact->cooldownRemaining > 0 || player->spells->DarkPact->casting) player->spells->DarkPact->tick(time);
    if (player->spells->DestructionPotion->cooldownRemaining > 0 || player->spells->DestructionPotion->casting) player->spells->DestructionPotion->tick(time);
    if (player->spells->SuperManaPotion->cooldownRemaining > 0 || player->spells->SuperManaPotion->casting) player->spells->SuperManaPotion->tick(time);
    if (player->spells->DemonicRune->cooldownRemaining > 0 || player->spells->DemonicRune->casting) player->spells->DemonicRune->tick(time);
    if (player->spells->FlameCap->cooldownRemaining > 0 || player->spells->FlameCap->casting) player->spells->FlameCap->tick(time);
    if (player->spells->BloodFury->cooldownRemaining > 0 || player->spells->BloodFury->casting) player->spells->BloodFury->tick(time);
    if (player->spells->DrumsOfBattle->cooldownRemaining > 0 || player->spells->DrumsOfBattle->casting) player->spells->DrumsOfBattle->tick(time);
    if (player->spells->DrumsOfWar->cooldownRemaining > 0 || player->spells->DrumsOfWar->casting) player->spells->DrumsOfWar->tick(time);
    if (player->spells->DrumsOfRestoration->cooldownRemaining > 0 || player->spells->DrumsOfRestoration->casting) player->spells->DrumsOfRestoration->tick(time);
    if (player->spells->BladeOfWizardry->cooldownRemaining > 0 || player->spells->BladeOfWizardry->casting) player->spells->BladeOfWizardry->tick(time);
    if (player->spells->ShatteredSunPendantOfAcumen->cooldownRemaining > 0 || player->spells->ShatteredSunPendantOfAcumen->casting) player->spells->ShatteredSunPendantOfAcumen->tick(time);
    if (player->spells->RobeOfTheElderScribes->cooldownRemaining > 0 || player->spells->RobeOfTheElderScribes->casting) player->spells->RobeOfTheElderScribes->tick(time);
    if (player->spells->MysticalSkyfireDiamond->cooldownRemaining > 0 || player->spells->MysticalSkyfireDiamond->casting) player->spells->MysticalSkyfireDiamond->tick(time);
    if (player->spells->InsightfulEarthstormDiamond->cooldownRemaining > 0 || player->spells->InsightfulEarthstormDiamond->casting) player->spells->InsightfulEarthstormDiamond->tick(time);
    if (player->spells->TimbalsFocusingCrystal->cooldownRemaining > 0 || player->spells->TimbalsFocusingCrystal->casting) player->spells->TimbalsFocusingCrystal->tick(time);
    if (player->spells->MarkOfDefiance->cooldownRemaining > 0 || player->spells->MarkOfDefiance->casting) player->spells->MarkOfDefiance->tick(time);
    if (player->spells->TheLightningCapacitor->cooldownRemaining > 0 || player->spells->TheLightningCapacitor->casting) player->spells->TheLightningCapacitor->tick(time);
    if (player->spells->QuagmirransEye->cooldownRemaining > 0 || player->spells->QuagmirransEye->casting) player->spells->QuagmirransEye->tick(time);
    if (player->spells->ShiffarsNexusHorn->cooldownRemaining > 0 || player->spells->ShiffarsNexusHorn->casting) player->spells->ShiffarsNexusHorn->tick(time);
    if (player->spells->SextantOfUnstableCurrents->cooldownRemaining > 0 || player->spells->SextantOfUnstableCurrents->casting) player->spells->SextantOfUnstableCurrents->tick(time);
    if (player->spells->BandOfTheEternalSage->cooldownRemaining > 0 || player->spells->BandOfTheEternalSage->casting) player->spells->BandOfTheEternalSage->tick(time);
    for (std::vector<Spell*>::iterator it = player->spells->PowerInfusion.begin(); it != player->spells->PowerInfusion.end(); it++)
    {
        if ((*it)->cooldownRemaining > 0 || (*it)->casting) (*it)->tick(time);
    }
    for (std::vector<Spell*>::iterator it = player->spells->Bloodlust.begin(); it != player->spells->Bloodlust.end(); it++)
    {
        if ((*it)->cooldownRemaining > 0 || (*it)->casting) (*it)->tick(time);
    }
    for (std::vector<Spell*>::iterator it = player->spells->Innervate.begin(); it != player->spells->Innervate.end(); it++)
    {
        if ((*it)->cooldownRemaining > 0 || (*it)->casting) (*it)->tick(time);
    }

    // Trinkets
    for (int i = 0; i < player->trinkets.size(); i++)
    {
        player->trinkets[i]->tick(time);
    }

    player->gcdRemaining -= time;
    player->mp5Timer -= time;
    player->fiveSecondRuleTimer -= time;
    if (player->mp5Timer <= 0)
    {
        player->mp5Timer = 5;
        if (player->stats->mp5 > 0 || player->fiveSecondRuleTimer <= 0 || (!player->spells->Innervate.empty() && player->auras->Innervate->active))
        {
            bool innervateExists = !player->spells->Innervate.empty();
            bool innervateActive = innervateExists && player->auras->Innervate->active;
            int currentPlayerMana = player->stats->mana;

            // MP5
            if (player->stats->mp5)
            {
                player->stats->mana = std::min(player->stats->maxMana, currentPlayerMana + player->stats->mp5);
            }
            // Spirit mana regen
            if (player->fiveSecondRuleTimer <= 0 || innervateActive)
            {
                // Formula from https://wowwiki-archive.fandom.com/wiki/Spirit?oldid=1572910
                int mp5FromSpirit = 5 * (0.001 + std::sqrt(player->stats->intellect * player->stats->intellectModifier) * (player->stats->spirit * player->stats->spiritModifier) * 0.009327);
                if (innervateActive)
                {
                    mp5FromSpirit *= 4;
                }
                player->stats->mana = std::min(player->stats->maxMana, currentPlayerMana + mp5FromSpirit);
            }

            int manaGained = player->stats->mana - currentPlayerMana;
            player->totalManaRegenerated += manaGained;
            
            if (player->shouldWriteToCombatLog())
            {
                std::string msg = "Player gains " + std::to_string(round(manaGained)) + " mana from MP5 (" + std::to_string(currentPlayerMana) + " -> " + std::to_string(player->stats->mana) + ")";
                player->combatLog(msg);
            }
        }
    }
}

void Simulation::start()
{
    double totalDamage = 0;
    std::vector<double> dpsVector;
    player->totalDuration = 0;
    player->initialize();
    double minDps = 99999;
    double maxDps = 0;
    auto startTime = std::chrono::high_resolution_clock::now();

    for (player->iteration = 0; player->iteration < this->settings->iterations; player->iteration++)
    {
        player->reset();
        const int fightLength = random(this->settings->minTime, this->settings->maxTime);
        player->iterationDamage = 0;
        player->fightTime = 0;
        //player->combatLog("Fight length: " + std::to_string(fightLength) + " seconds");

        while (player->fightTime < fightLength)
        {
            // Use Drums
            if (player->spells->DrumsOfBattle != NULL && !player->auras->DrumsOfBattle->active && player->spells->DrumsOfBattle->ready())
            {
                player->spells->DrumsOfBattle->startCast();
            }
            else if (player->spells->DrumsOfWar != NULL && !player->auras->DrumsOfWar->active && player->spells->DrumsOfWar->ready())
            {
                player->spells->DrumsOfWar->startCast();
            }
            else if (player->spells->DrumsOfRestoration != NULL && !player->auras->DrumsOfRestoration->active && player->spells->DrumsOfRestoration->ready())
            {
                player->spells->DrumsOfRestoration->startCast();
            }

            // Player
            if (player->castTimeRemaining <= 0)
            {
                // Spells not on the GCD
                // Demonic Rune
                if ((player->fightTime > 5 || player->stats->mp5 == 0) && player->spells->DemonicRune != NULL && (player->stats->maxMana - player->stats->mana) > player->spells->DemonicRune->avgManaValue && player->spells->DemonicRune->ready())
                {
                    player->spells->DemonicRune->startCast();
                }
                // Super Mana Potion
                if ((player->fightTime > 5 || player->stats->mp5 == 0) && player->spells->SuperManaPotion != NULL && (player->stats->maxMana - player->stats->mana) > player->spells->SuperManaPotion->avgManaValue && player->spells->SuperManaPotion->ready())
                {
                    player->spells->SuperManaPotion->startCast();
                }

                // Spells on the GCD
                if (player->gcdRemaining <= 0)
                {
                    if (player->settings->isSingleTarget)
                    {
                        double timeRemaining = fightLength - player->fightTime;

                        // Map of spells with their predicted damage as the value. This is used by the sim to determine what the best spell to cast is.
                        std::map<Spell*, double> predictedDamageOfSpells;

                        // If the sim is choosing the rotation for the user then predict the damage of the three filler spells if they're available (maybe just skip Searing Pain to save time, there's no way it will ever be the best spell to cast)
                        if (player->settings->simChoosingRotation)
                        {
                            predictedDamageOfSpells.insert(std::make_pair(player->spells->ShadowBolt, player->spells->ShadowBolt->predictDamage()));
                            predictedDamageOfSpells.insert(std::make_pair(player->spells->Incinerate, player->spells->Incinerate->predictDamage()));
                            predictedDamageOfSpells.insert(std::make_pair(player->spells->SearingPain, player->spells->SearingPain->predictDamage()));
                        }

                        //todo finishers

                        // Cast selected curse if it's either CoR or CoE and it's not up
                        if (player->curse != "" && (player->curse == "curseOfRecklessness" || player->curse == "curseOfTheElements") && !player->getCurseAura()->active && player->getCurseSpell()->ready())
                        {
                            player->getCurseSpell()->startCast();
                        }
                        else
                        {
                            player->useCooldowns();
                            // Cast Curse of Doom if there's more than 60 seconds remaining
                            if (timeRemaining > 60 && player->curse == "curseOfDoom" && !player->auras->CurseOfDoom->active && player->spells->CurseOfDoom->canCast())
                            {
                                // If the sim is choosing the rotation for the player then predict the damage of the spell and put it in the map
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair(player->spells->CurseOfDoom, player->spells->CurseOfDoom->predictDamage()));
                                }
                                // Else if the player is choosing the rotation themselves then just cast the highest priority spell that needs to be cast
                                else if (player->spells->CurseOfDoom->hasEnoughMana())
                                {
                                    if (player->spells->AmplifyCurse != NULL && player->spells->AmplifyCurse->ready())
                                    {
                                        player->spells->AmplifyCurse->startCast();
                                    }
                                    player->spells->CurseOfDoom->startCast();
                                }
                            }
                            // Cast Curse of Agony if CoA is the selected curse or if Curse of Doom is the selected curse and there's less than 60 seconds remaining of the fight
                            if (player->curse != "" && player->auras->CurseOfAgony != NULL && !player->auras->CurseOfAgony->active && timeRemaining > player->auras->CurseOfAgony->minimumDuration && ((player->curse == "curseOfDoom" && !player->auras->CurseOfDoom->active && (player->spells->CurseOfDoom->cooldownRemaining > player->auras->CurseOfAgony->minimumDuration || timeRemaining < 60)) || (player->curse == "curseOfAgony" && player->spells->CurseOfAgony->canCast())))
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair(player->spells->CurseOfAgony, player->spells->CurseOfAgony->predictDamage()));
                                }
                                else if (player->spells->CurseOfAgony->hasEnoughMana())
                                {
                                    if (player->spells->AmplifyCurse != NULL && player->spells->AmplifyCurse->ready())
                                    {
                                        player->spells->AmplifyCurse->startCast();
                                    }
                                    player->spells->CurseOfAgony->startCast();
                                }
                            }
                            // Cast Corruption if Corruption isn't up or if it will expire before the cast finishes (if no instant Corruption)
                            if (player->spells->Corruption != NULL && (!player->auras->Corruption->active || (player->auras->Corruption->ticksRemaining == 1 && player->auras->Corruption->tickTimerRemaining < player->spells->Corruption->getCastTime())) && player->spells->Corruption->canCast() && (timeRemaining - player->spells->Corruption->getCastTime()) >= player->auras->Corruption->minimumDuration)
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair(player->spells->Corruption, player->spells->Corruption->predictDamage()));
                                }
                                else if (player->spells->Corruption->hasEnoughMana())
                                {
                                    player->spells->Corruption->startCast();
                                }
                            }
                            // Cast Shadow Bolt if Shadow Trance (Nightfall) is active and Corruption is active as well to avoid potentially wasting another Nightfall proc
                            if (player->spells->ShadowBolt != NULL && player->auras->ShadowTrance != NULL && player->auras->ShadowTrance->active && player->auras->Corruption->active && player->spells->ShadowBolt->canCast())
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    // todo: check if the spell is already in the array before adding it
                                    predictedDamageOfSpells.insert(std::make_pair(player->spells->ShadowBolt, player->spells->ShadowBolt->predictDamage()));
                                }
                                else if (player->spells->ShadowBolt->hasEnoughMana())
                                {
                                    player->spells->ShadowBolt->startCast();
                                }
                            }
                            // Cast Unstable Affliction if it's not up or if it's about to expire
                            if (player->spells->UnstableAffliction != NULL && player->spells->UnstableAffliction->canCast() && (!player->auras->UnstableAffliction->active || (player->auras->UnstableAffliction->ticksRemaining == 1 && player->auras->UnstableAffliction->tickTimerRemaining < player->spells->UnstableAffliction->getCastTime())) && (timeRemaining - player->spells->UnstableAffliction->getCastTime()) >= player->auras->UnstableAffliction->minimumDuration)
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair(player->spells->UnstableAffliction, player->spells->UnstableAffliction->predictDamage()));
                                }
                                else if (player->spells->UnstableAffliction->hasEnoughMana())
                                {
                                    player->spells->UnstableAffliction->startCast();
                                }
                            }
                            // Cast Siphon Life if it's not up (todo: add option to only cast it while ISB is active if not using custom ISB uptime %)
                            if (player->spells->SiphonLife != NULL && !player->auras->SiphonLife->active && player->spells->SiphonLife->canCast() && timeRemaining >= player->auras->SiphonLife->minimumDuration)
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair(player->spells->SiphonLife, player->spells->SiphonLife->predictDamage()));
                                }
                                else if (player->spells->SiphonLife->hasEnoughMana())
                                {
                                    player->spells->SiphonLife->startCast();
                                }
                            }
                            // Cast Immolate if it's not up or about to expire
                            if (player->spells->Immolate != NULL && player->spells->Immolate->canCast() && (!player->auras->Immolate->active || (player->auras->Immolate->ticksRemaining == 1 && player->auras->Immolate->tickTimerRemaining < player->spells->Immolate->getCastTime())) && (timeRemaining - player->spells->Immolate->getCastTime()) >= player->auras->Immolate->minimumDuration)
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair(player->spells->Immolate, player->spells->Immolate->predictDamage()));
                                }
                                else if (player->spells->Immolate->hasEnoughMana())
                                {
                                    player->spells->Immolate->startCast();
                                }
                            }
                            // Cast Shadow Bolt if Shadow Trance (Nightfall) is active
                            if (player->spells->ShadowBolt != NULL && player->auras->ShadowTrance != NULL && player->auras->ShadowTrance->active && player->spells->ShadowBolt->canCast())
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair(player->spells->ShadowBolt, player->spells->ShadowBolt->predictDamage()));
                                }
                                else if (player->spells->ShadowBolt->hasEnoughMana())
                                {
                                    player->spells->ShadowBolt->startCast();
                                }
                            }
                            // Cast Shadowfury
                            if (player->spells->Shadowfury != NULL && player->spells->Shadowfury->canCast())
                            {
                                if (player->settings->simChoosingRotation)
                                {
                                    predictedDamageOfSpells.insert(std::make_pair(player->spells->Shadowfury, player->spells->Shadowfury->predictDamage()));
                                }
                                else if (player->spells->Shadowfury->hasEnoughMana())
                                {
                                    player->spells->Shadowfury->startCast();
                                }
                            }
                            // Cast filler spell if sim is not choosing the rotation for the user
                            if (!player->settings->simChoosingRotation && player->getFiller()->ready())
                            {
                                player->getFiller()->startCast();
                            }
                            // Cast Dark Pact/Life Tap if nothing else is possible and the sim is not choosing the rotation for the user
                            if (!player->settings->simChoosingRotation && player->spells->DarkPact != NULL && player->spells->DarkPact->ready())
                            {
                                player->spells->DarkPact->startCast();
                            }
                            if (!player->settings->simChoosingRotation && player->spells->LifeTap->ready())
                            {
                                player->spells->LifeTap->startCast();
                            }
                        }

                        // If the sim is choosing the rotation for the player then check now which spell would be the best to cast
                        if (player->settings->simChoosingRotation)
                        {
                            Spell* maxDamageSpell;
                            double maxDamageSpellValue = 0;

                            for (std::map<Spell*, double>::iterator it = predictedDamageOfSpells.begin(); it != predictedDamageOfSpells.end(); it++)
                            {
                                if (it->second > maxDamageSpellValue)
                                {
                                    maxDamageSpell = it->first;
                                    maxDamageSpellValue = it->second;
                                }
                            }

                            // If a max damage spell was not found or if the max damage spell isn't ready (no mana), then cast Life Tap
                            if (maxDamageSpell != NULL && maxDamageSpell->hasEnoughMana())
                            {
                                maxDamageSpell->startCast();
                            }
                            else
                            {
                                player->castLifeTapOrDarkPact();
                            }
                        }
                    }
                    // AoE (currently just does Seed of Corruption by default)
                    else
                    {
                        if (player->spells->SeedOfCorruption->ready())
                        {
                            player->useCooldowns();
                            player->spells->SeedOfCorruption->startCast();
                        }
                        else
                        {
                            player->castLifeTapOrDarkPact();
                        }
                    }
                }
            }

            passTime();
        }

        player->totalDuration += fightLength;
        totalDamage += player->iterationDamage;
        double dps = player->iterationDamage / fightLength;
        if (dps > maxDps)
        {
            maxDps = dps;
        }
        if (dps < minDps)
        {
            minDps = dps;
        }
        dpsVector.push_back(dps);

        //todo remove hard-coding
        if (player->iteration % 100 == 0)
        {
            simulationUpdate(player->iteration, this->settings->iterations, median(dpsVector), player->settings->itemId);
        }

        // End all active auras & dots
        if (player->auras->Corruption != NULL) player->auras->Corruption->fade(true);
        if (player->auras->UnstableAffliction != NULL) player->auras->UnstableAffliction->fade(true);
        if (player->auras->SiphonLife != NULL) player->auras->SiphonLife->fade(true);
        if (player->auras->Immolate != NULL) player->auras->Immolate->fade(true);
        if (player->auras->CurseOfAgony != NULL) player->auras->CurseOfAgony->fade(true);
        if (player->auras->CurseOfDoom != NULL) player->auras->CurseOfDoom->fade(true);
        if (player->auras->ImprovedShadowBolt != NULL) player->auras->ImprovedShadowBolt->fade(true);
        if (player->auras->CurseOfTheElements != NULL) player->auras->CurseOfTheElements->fade(true);
        if (player->auras->CurseOfRecklessness != NULL) player->auras->CurseOfRecklessness->fade(true);
        if (player->auras->ShadowTrance != NULL) player->auras->ShadowTrance->fade(true);
        if (player->auras->AmplifyCurse != NULL) player->auras->AmplifyCurse->fade(true);
        if (player->auras->PowerInfusion != NULL) player->auras->PowerInfusion->fade(true);
        if (player->auras->Innervate != NULL) player->auras->Innervate->fade(true);
        if (player->auras->BloodFury != NULL) player->auras->BloodFury->fade(true);
        if (player->auras->DestructionPotion != NULL) player->auras->DestructionPotion->fade(true);
        if (player->auras->FlameCap != NULL) player->auras->FlameCap->fade(true);
        if (player->auras->Bloodlust != NULL) player->auras->Bloodlust->fade(true);
        if (player->auras->DrumsOfBattle != NULL) player->auras->DrumsOfBattle->fade(true);
        if (player->auras->DrumsOfWar != NULL) player->auras->DrumsOfWar->fade(true);
        if (player->auras->DrumsOfRestoration != NULL) player->auras->DrumsOfRestoration->fade(true);
        if (player->auras->BandOfTheEternalSage != NULL) player->auras->BandOfTheEternalSage->fade(true);
        if (player->auras->WrathOfCenarius != NULL) player->auras->WrathOfCenarius->fade(true);
        if (player->auras->BladeOfWizardry != NULL) player->auras->BladeOfWizardry->fade(true);
        if (player->auras->ShatteredSunPendantOfAcumen != NULL) player->auras->ShatteredSunPendantOfAcumen->fade(true);
        if (player->auras->RobeOfTheElderScribes != NULL) player->auras->RobeOfTheElderScribes->fade(true);
        if (player->auras->MysticalSkyfireDiamond != NULL) player->auras->MysticalSkyfireDiamond->fade(true);
        if (player->auras->EyeOfMagtheridon != NULL) player->auras->EyeOfMagtheridon->fade(true);
        if (player->auras->SextantOfUnstableCurrents != NULL) player->auras->SextantOfUnstableCurrents->fade(true);
        if (player->auras->QuagmirransEye != NULL) player->auras->QuagmirransEye->fade(true);
        if (player->auras->ShiffarsNexusHorn != NULL) player->auras->ShiffarsNexusHorn->fade(true);
        if (player->auras->AshtongueTalismanOfShadows != NULL) player->auras->AshtongueTalismanOfShadows->fade(true);
        if (player->auras->DarkmoonCardCrusade != NULL) player->auras->DarkmoonCardCrusade->fade(true);
        if (player->auras->TheLightningCapacitor != NULL) player->auras->TheLightningCapacitor->fade(true);
        if (player->auras->Flameshadow != NULL) player->auras->Flameshadow->fade(true);
        if (player->auras->Shadowflame != NULL) player->auras->Shadowflame->fade(true);
        if (player->auras->Spellstrike != NULL) player->auras->Spellstrike->fade(true);
        if (player->auras->ManaEtched4Set != NULL) player->auras->ManaEtched4Set->fade(true);
    }

    auto finishTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = finishTime - startTime;
    // Send the contents of the combat log to the web worker
    /*for (const auto& value: player->combatLogEntries)
    {
        combatLogUpdate(value.c_str());
    }*/
    simulationEnd(median(dpsVector), minDps, maxDps, elapsedTime, player->settings->itemId);

    // Free spells and auras
    delete player->spells;
    delete player->auras;
}