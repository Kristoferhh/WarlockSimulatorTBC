#pragma once

struct Player;
#include <iostream>
#include "auraStats.h"

struct Aura
{
    std::shared_ptr<Player> player;
    std::unique_ptr<AuraStats> stats;
    std::string name;
    int duration;
    double durationRemaining;
    int procChance;
    bool active;
    bool hasDuration;
    bool groupWide; // true if it's an aura that applies to everyone in the group (will apply to pets as well then)
    // dots
    int tickTimerTotal;
    double tickTimerRemaining;
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

    Aura(std::shared_ptr<Player> player);
    void setup();
    virtual void tick(double time);
    virtual void apply();
    virtual void fade();
    virtual void decrementStacks(); // ISB
};

struct ImprovedShadowBoltAura : public Aura
{
    double modifier;

    ImprovedShadowBoltAura(std::shared_ptr<Player> player);
    void apply();
    void fade();
    void decrementStacks();
};

struct CurseOfTheElementsAura : public Aura
{
    CurseOfTheElementsAura(std::shared_ptr<Player> player);
};

struct CurseOfRecklessnessAura : public Aura
{
    CurseOfRecklessnessAura(std::shared_ptr<Player> player);
};

struct ShadowTranceAura : public Aura
{
    ShadowTranceAura(std::shared_ptr<Player> player);
};

struct FlameshadowAura : public Aura
{
    FlameshadowAura(std::shared_ptr<Player> player);
};

struct ShadowflameAura : public Aura
{
    ShadowflameAura(std::shared_ptr<Player> player);
};

struct SpellstrikeAura : public Aura
{
    SpellstrikeAura(std::shared_ptr<Player> player);
};

struct PowerInfusionAura : public Aura
{
    PowerInfusionAura(std::shared_ptr<Player> player);
};

struct EyeOfMagtheridonAura : public Aura
{
    EyeOfMagtheridonAura(std::shared_ptr<Player> player);
};

struct SextantOfUnstableCurrentsAura : public Aura
{
    SextantOfUnstableCurrentsAura(std::shared_ptr<Player> player);
};

struct QuagmirransEyeAura : public Aura
{
    QuagmirransEyeAura(std::shared_ptr<Player> player);
};

struct ShiffarsNexusHornAura : public Aura
{
    ShiffarsNexusHornAura(std::shared_ptr<Player> player);
};

struct ManaEtched4SetAura : public Aura
{
    ManaEtched4SetAura(std::shared_ptr<Player> player);
};

struct DestructionPotionAura : public Aura
{
    DestructionPotionAura(std::shared_ptr<Player> player);
    void apply();
    void fade();
};

struct FlameCapAura : public Aura
{
    FlameCapAura(std::shared_ptr<Player> player);
};

struct BloodFuryAura : public Aura
{
    BloodFuryAura(std::shared_ptr<Player> player);
};

struct BloodlustAura : public Aura
{
    BloodlustAura(std::shared_ptr<Player> player);
};

struct DrumsOfBattleAura : public Aura
{
    DrumsOfBattleAura(std::shared_ptr<Player> player);
};

struct DrumsOfWarAura : public Aura
{
    DrumsOfWarAura(std::shared_ptr<Player> player);
};

struct DrumsOfRestorationAura : public Aura
{
    int manaGain;

    DrumsOfRestorationAura(std::shared_ptr<Player> player);
    void apply();
    void tick(double time);
};

struct AshtongueTalismanOfShadowsAura : public Aura
{
    AshtongueTalismanOfShadowsAura(std::shared_ptr<Player> player);
};

struct DarkmoonCardCrusadeAura : public Aura
{
    int stacks;
    int maxStacks;
    int spellPowerPerStack;

    DarkmoonCardCrusadeAura(std::shared_ptr<Player> player);
    void apply();
    void fade();
};

struct TheLightningCapacitorAura : public Aura
{
    TheLightningCapacitorAura(std::shared_ptr<Player> player);
    void apply();
    void fade();
};

struct BandOfTheEternalSageAura : public Aura
{
    BandOfTheEternalSageAura(std::shared_ptr<Player> player);
};

struct BladeOfWizardryAura : public Aura
{
    BladeOfWizardryAura(std::shared_ptr<Player> player);
};

struct ShatteredSunPendantOfAcumenAura : public Aura
{
    ShatteredSunPendantOfAcumenAura(std::shared_ptr<Player> player);
};

struct RobeOfTheElderScribesAura : public Aura
{
    RobeOfTheElderScribesAura(std::shared_ptr<Player> player);
};

struct MysticalSkyfireDiamondAura : public Aura
{
    MysticalSkyfireDiamondAura(std::shared_ptr<Player> player);
};

struct AmplifyCurseAura : public Aura
{
    AmplifyCurseAura(std::shared_ptr<Player> player);
};

struct WrathOfCenariusAura : public Aura
{
    WrathOfCenariusAura(std::shared_ptr<Player> player);
};

struct InnervateAura : public Aura
{
    InnervateAura(Player* player);
};

struct ChippedPowerCoreAura : public Aura
{
    ChippedPowerCoreAura(Player* player);
};

struct CrackedPowerCoreAura : public Aura
{
    CrackedPowerCoreAura(Player* player);
};

struct ManaTideTotemAura : public Aura
{
    ManaTideTotemAura(Player* player);
    void tick(double time);
    void apply();
};