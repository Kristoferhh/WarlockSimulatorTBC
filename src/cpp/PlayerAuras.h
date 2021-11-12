#pragma once

#include "aura.h"
#include "damageOverTime.h"

struct PlayerAuras
{
    std::shared_ptr<DamageOverTime> Corruption;
    std::shared_ptr<DamageOverTime> UnstableAffliction;
    std::shared_ptr<DamageOverTime> SiphonLife;
    std::shared_ptr<DamageOverTime> Immolate;
    std::shared_ptr<DamageOverTime> CurseOfAgony;
    std::shared_ptr<DamageOverTime> CurseOfDoom;
    std::shared_ptr<Aura> ImprovedShadowBolt;
    std::shared_ptr<Aura> CurseOfTheElements;
    std::shared_ptr<Aura> CurseOfRecklessness;
    std::shared_ptr<Aura> ShadowTrance;
    std::shared_ptr<Aura> AmplifyCurse;
    std::shared_ptr<Aura> PowerInfusion;
    std::shared_ptr<Aura> Innervate;
    std::shared_ptr<Aura> BloodFury;
    std::shared_ptr<Aura> DestructionPotion;
    std::shared_ptr<Aura> FlameCap;
    std::shared_ptr<Aura> Bloodlust;
    std::shared_ptr<Aura> DrumsOfBattle;
    std::shared_ptr<Aura> DrumsOfWar;
    std::shared_ptr<Aura> DrumsOfRestoration;
    std::shared_ptr<Aura> BandOfTheEternalSage;
    std::shared_ptr<Aura> WrathOfCenarius;
    std::shared_ptr<Aura> BladeOfWizardry;
    std::shared_ptr<Aura> ShatteredSunPendantOfAcumen;
    std::shared_ptr<Aura> RobeOfTheElderScribes;
    std::shared_ptr<Aura> MysticalSkyfireDiamond;
    std::shared_ptr<Aura> EyeOfMagtheridon;
    std::shared_ptr<Aura> SextantOfUnstableCurrents;
    std::shared_ptr<Aura> QuagmirransEye;
    std::shared_ptr<Aura> ShiffarsNexusHorn;
    std::shared_ptr<Aura> AshtongueTalismanOfShadows;
    std::shared_ptr<Aura> DarkmoonCardCrusade;
    std::shared_ptr<Aura> TheLightningCapacitor;
    std::shared_ptr<Aura> Flameshadow; // Shadowpower
    std::shared_ptr<Aura> Shadowflame; // Firepower
    std::shared_ptr<Aura> Spellstrike;
    std::shared_ptr<Aura> ManaEtched4Set;
    std::shared_ptr<Aura> ChippedPowerCore;
    std::shared_ptr<Aura> CrackedPowerCore;
    std::shared_ptr<Aura> ManaTideTotem;
};