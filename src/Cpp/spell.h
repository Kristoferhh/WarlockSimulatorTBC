#pragma once

struct Player;

#include <string>
#include <cmath>
#include "enums.h"
#include "damageOverTime.h"

struct Spell : std::enable_shared_from_this<Spell>
{
    Player* player;
    std::shared_ptr<Aura> auraEffect;
    std::shared_ptr<DamageOverTime> dotEffect;
    SpellSchool school;
    SpellType type;
    int minDmg;
    int maxDmg;
    int minMana;
    int maxMana;
    int procChance;
    double manaGain;
    double castTime;
    double manaCost;
    double coefficient;
    double manaReturn;
    double cooldown;
    double modifier;
    double bonusCrit;
    double cooldownRemaining;
    double dmg;
    double avgManaValue;
    int bonusDamageFromImmolateMin;
    int bonusDamageFromImmolateMax;
    double bonusDamageFromImmolate;
    bool isDot;
    bool doesDamage;
    bool canCrit;
    bool isNonWarlockAbility;
    bool casting;
    bool isItem;
    bool isAura;
    bool onGcd;
    bool isProc;
    bool isFinisher;
    std::string name;
    std::string breakdownTable;

    Spell(Player* player, std::shared_ptr<Aura> aura = nullptr, std::shared_ptr<DamageOverTime> dot = nullptr);

    void reset();
    void setup();
    bool canCast();
    bool hasEnoughMana();
    virtual bool ready();
    virtual double getCastTime();
    virtual void startCast(double predictedDamage = 0);
    virtual void cast();
    virtual double getModifier();
    virtual void damage(bool isCrit = false);
    std::vector<double> getConstantDamage(bool noRng = true);
    double getCritMultiplier(double critMultiplier);
    double predictDamage();
    void tick(double t);
    void onCritProcs();
    void onDamageProcs();
    void onHitProcs();
};

struct LifeTap : public Spell
{
    LifeTap(Player* player);
    int manaGain();
    void cast();
};

struct ShadowBolt : public Spell
{
    ShadowBolt(Player* player);
    void startCast(double predictedDamage);
    double calculateCastTime();
};

struct Incinerate : public Spell
{
    Incinerate(Player* player);
};

struct SearingPain : public Spell
{
    SearingPain(Player* player);
};

struct SoulFire : public Spell
{
    SoulFire(Player* player);
};

struct Shadowburn : public Spell
{
    Shadowburn(Player* player);
};

struct DeathCoil : public Spell
{
    DeathCoil(Player* player);
};

struct Shadowfury : public Spell
{
    Shadowfury(Player* player);
};

struct SeedOfCorruption : public Spell
{
    int dmgCap;
    SeedOfCorruption(Player* player);
    void damage(bool isCrit = false);
    double getModifier();
};

struct DarkPact : public Spell
{
    DarkPact(Player* player);
    void cast();
    bool ready();
    double manaGain();
};

struct Corruption : public Spell
{
    Corruption(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct UnstableAffliction : public Spell
{
    UnstableAffliction(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct SiphonLife : public Spell
{
    SiphonLife(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct Immolate : public Spell
{
    Immolate(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
    double getModifier();
};

struct CurseOfAgony : public Spell
{
    CurseOfAgony(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct CurseOfTheElements : public Spell
{
    CurseOfTheElements(Player* player, std::shared_ptr<Aura> aura);
};

struct CurseOfRecklessness : public Spell
{
    CurseOfRecklessness(Player* player, std::shared_ptr<Aura> aura);
};

struct CurseOfDoom : public Spell
{
    CurseOfDoom(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct Conflagrate : public Spell
{
    Conflagrate(Player* player);
    void startCast();
};

struct DestructionPotion : public Spell
{
    DestructionPotion(Player* player, std::shared_ptr<Aura> aura);
};

struct SuperManaPotion : public Spell
{
    SuperManaPotion(Player* player);
    void cast();
};

struct DemonicRune : public Spell
{
    DemonicRune(Player* player);
    void cast();
};

struct FlameCap : public Spell
{
    FlameCap(Player* player, std::shared_ptr<Aura> aura);
};

struct BloodFury : public Spell
{
    BloodFury(Player* player, std::shared_ptr<Aura> aura);
};

struct Bloodlust : public Spell
{
    Bloodlust(Player* player, std::shared_ptr<Aura> aura);
};

struct DrumsOfBattle : public Spell
{
    DrumsOfBattle(Player* player, std::shared_ptr<Aura> aura);
    bool ready();
};

struct DrumsOfWar : public Spell
{
    DrumsOfWar(Player* player, std::shared_ptr<Aura> aura);
    bool ready();
};

struct DrumsOfRestoration : public Spell
{
    DrumsOfRestoration(Player* player, std::shared_ptr<Aura> aura);
    bool ready();
};

struct TimbalsFocusingCrystal : public Spell
{
    TimbalsFocusingCrystal(Player* player);
};

struct MarkOfDefiance : public Spell
{
    MarkOfDefiance(Player* player);
    void cast();
};

struct TheLightningCapacitor : public Spell
{
    TheLightningCapacitor(Player* player, std::shared_ptr<Aura> aura);
    void startCast(double predictedDamage = 0);
};

struct BladeOfWizardry : public Spell
{
    BladeOfWizardry(Player* player, std::shared_ptr<Aura> aura);
};

struct ShatteredSunPendantOfAcumen : public Spell
{
    ShatteredSunPendantOfAcumen(Player* player, std::shared_ptr<Aura> aura);
};

struct RobeOfTheElderScribes : public Spell
{
    RobeOfTheElderScribes(Player* player, std::shared_ptr<Aura> aura);
};

struct QuagmirransEye : public Spell
{
    QuagmirransEye(Player* player, std::shared_ptr<Aura> aura);
};

struct ShiffarsNexusHorn : public Spell
{
    ShiffarsNexusHorn(Player* player, std::shared_ptr<Aura> aura);
};

struct SextantOfUnstableCurrents : public Spell
{
    SextantOfUnstableCurrents(Player* player, std::shared_ptr<Aura> aura);
};

struct BandOfTheEternalSage : public Spell
{
    BandOfTheEternalSage(Player* player, std::shared_ptr<Aura> aura);
};

struct MysticalSkyfireDiamond : public Spell
{
    MysticalSkyfireDiamond(Player* player, std::shared_ptr<Aura> aura);
};

struct InsightfulEarthstormDiamond : public Spell
{
    InsightfulEarthstormDiamond(Player* player);
    void cast();
};

struct AmplifyCurse : public Spell
{
    AmplifyCurse(Player* player, std::shared_ptr<Aura> aura);
};

struct PowerInfusion : public Spell
{
    PowerInfusion(Player* player, std::shared_ptr<Aura> aura);
};

struct Innervate : public Spell
{
    Innervate(Player* player, std::shared_ptr<Aura> aura);
};