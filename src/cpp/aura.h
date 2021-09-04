#pragma once

struct Player;
#include <iostream>
#include "auraStats.h"

struct Aura
{
    Player* player;
    AuraStats* stats;
    std::string name;
    std::string varName;
    int duration;
    int durationRemaining;
    int hiddenCooldown;
    int procChance;
    // dots
    int tickTimerTotal;
    int tickTimerRemaining;
    int ticksRemaining;
    int ticksTotal;
    int minimumDuration;
    // The Lightning Capacitor & ISB
    int stacks;
    int maxStacks;
    // ISB
    double modifier;
    double uptimeSoFar;
    // Bloodlust
    double hasteModifier;
    bool active;
    bool hasDuration;
    bool groupWide; // true if it's an aura that applies to everyone in the group (will apply to pets as well then)

    Aura(Player* player);
    void setup();
    void tick(int time);
    virtual void apply();
    virtual void fade(bool endOfIteration = false);
    void decrementStacks(); // ISB
};

struct ImprovedShadowBoltAura : public Aura
{
    double modifier;

    ImprovedShadowBoltAura(Player* player);
    void apply();
    void fade(bool endOfIteration = false);
    void decrementStacks();
};

struct CurseOfTheElementsAura : public Aura
{
    CurseOfTheElementsAura(Player* player);
};

struct CurseOfRecklessnessAura : public Aura
{
    CurseOfRecklessnessAura(Player* player);
};

struct ShadowTranceAura : public Aura
{
    ShadowTranceAura(Player* player);
};

struct FlameshadowAura : public Aura
{
    FlameshadowAura(Player* player);
};

struct ShadowflameAura : public Aura
{
    ShadowflameAura(Player* player);
};

struct SpellstrikeAura : public Aura
{
    SpellstrikeAura(Player* player);
};

struct PowerInfusionAura : public Aura
{
    PowerInfusionAura(Player* player);
};

struct EyeOfMagtheridonAura : public Aura
{
    EyeOfMagtheridonAura(Player* player);
};

struct SextantOfUnstableCurrentsAura : public Aura
{
    SextantOfUnstableCurrentsAura(Player* player);
};

struct QuagmirransEyeAura : public Aura
{
    QuagmirransEyeAura(Player* player);
};

struct ShiffarsNexusHornAura : public Aura
{
    ShiffarsNexusHornAura(Player* player);
};

struct ManaEtched4SetAura : public Aura
{
    ManaEtched4SetAura(Player* player);
};

struct DestructionPotionAura : public Aura
{
    DestructionPotionAura(Player* player);
    void apply();
    void fade(bool endOfIteration = false);
};

struct FlameCapAura : public Aura
{
    FlameCapAura(Player* player);
};

struct BloodFuryAura : public Aura
{
    BloodFuryAura(Player* player);
};

struct BloodlustAura : public Aura
{
    BloodlustAura(Player* player);
};

struct DrumsOfBattleAura : public Aura
{
    DrumsOfBattleAura(Player* player);
};

struct DrumsOfWarAura : public Aura
{
    DrumsOfWarAura(Player* player);
};

struct DrumsOfRestorationAura : public Aura
{
    int manaGain;

    DrumsOfRestorationAura(Player* player);
    void apply();
    void tick(int time);
};

struct AshtongueTalismanOfShadowsAura : public Aura
{
    AshtongueTalismanOfShadowsAura(Player* player);
};

struct DarkmoonCardCrusadeAura : public Aura
{
    int stacks;
    int maxStacks;
    int spellPowerPerStack;

    DarkmoonCardCrusadeAura(Player* player);
    void apply();
    void fade(bool endOfIteration = false);
};

struct TheLightningCapacitorAura : public Aura
{
    TheLightningCapacitorAura(Player* player);
    void apply();
    void fade(bool endOfIteration = false);
};

struct BandOfTheEternalSageAura : public Aura
{
    BandOfTheEternalSageAura(Player* player);
};

struct BladeOfWizardryAura : public Aura
{
    BladeOfWizardryAura(Player* player);
};

struct ShatteredSunPendantOfAcumenAura : public Aura
{
    ShatteredSunPendantOfAcumenAura(Player* player);
};

struct RobeOfTheElderScribesAura : public Aura
{
    RobeOfTheElderScribesAura(Player* player);
};

struct MysticalSkyfireDiamondAura : public Aura
{
    MysticalSkyfireDiamondAura(Player* player);
};

struct AmplifyCurseAura : public Aura
{
    AmplifyCurseAura(Player* player);
};

struct WrathOfCenariusAura : public Aura
{
    WrathOfCenariusAura(Player* player);
};

struct InnervateAura : public Aura
{
    InnervateAura(Player* player);
};