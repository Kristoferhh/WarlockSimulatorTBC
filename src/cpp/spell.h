#ifndef WARLOCK_SIMULATOR_TBC_SPELL
#define WARLOCK_SIMULATOR_TBC_SPELL

struct Player;
#include <string>
#include <cmath>
#include "enums.h"
#include "damage_over_time.h"

struct Spell : std::enable_shared_from_this<Spell>
{
    std::shared_ptr<Player> player;
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
    // For Chipped Power Core
    bool usableOncePerFight;
    bool hasNotBeenCastThisFight;

    Spell(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura = nullptr, std::shared_ptr<DamageOverTime> dot = nullptr);

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
    LifeTap(std::shared_ptr<Player> player);
    int manaGain();
    void cast();
};

struct ShadowBolt : public Spell
{
    ShadowBolt(std::shared_ptr<Player> player);
    void startCast(double predictedDamage);
    double calculateCastTime();
};

struct Incinerate : public Spell
{
    Incinerate(std::shared_ptr<Player> player);
};

struct SearingPain : public Spell
{
    SearingPain(std::shared_ptr<Player> player);
};

struct SoulFire : public Spell
{
    SoulFire(std::shared_ptr<Player> player);
};

struct Shadowburn : public Spell
{
    Shadowburn(std::shared_ptr<Player> player);
};

struct DeathCoil : public Spell
{
    DeathCoil(std::shared_ptr<Player> player);
};

struct Shadowfury : public Spell
{
    Shadowfury(std::shared_ptr<Player> player);
};

struct SeedOfCorruption : public Spell
{
    int dmgCap;
    SeedOfCorruption(std::shared_ptr<Player> player);
    void damage(bool isCrit = false);
    double getModifier();
};

struct DarkPact : public Spell
{
    DarkPact(std::shared_ptr<Player> player);
    void cast();
    bool ready();
    double manaGain();
};

struct Corruption : public Spell
{
    Corruption(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct UnstableAffliction : public Spell
{
    UnstableAffliction(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct SiphonLife : public Spell
{
    SiphonLife(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct Immolate : public Spell
{
    Immolate(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
    double getModifier();
};

struct CurseOfAgony : public Spell
{
    CurseOfAgony(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct CurseOfTheElements : public Spell
{
    CurseOfTheElements(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct CurseOfRecklessness : public Spell
{
    CurseOfRecklessness(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct CurseOfDoom : public Spell
{
    CurseOfDoom(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot);
};

struct Conflagrate : public Spell
{
    Conflagrate(std::shared_ptr<Player> player);
    void startCast();
};

struct DestructionPotion : public Spell
{
    DestructionPotion(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct SuperManaPotion : public Spell
{
    SuperManaPotion(std::shared_ptr<Player> player);
    void cast();
};

struct DemonicRune : public Spell
{
    DemonicRune(std::shared_ptr<Player> player);
    void cast();
};

struct FlameCap : public Spell
{
    FlameCap(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
    void cast();
};

struct BloodFury : public Spell
{
    BloodFury(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct Bloodlust : public Spell
{
    Bloodlust(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct DrumsOfBattle : public Spell
{
    DrumsOfBattle(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
    bool ready();
};

struct DrumsOfWar : public Spell
{
    DrumsOfWar(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
    bool ready();
};

struct DrumsOfRestoration : public Spell
{
    DrumsOfRestoration(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
    bool ready();
};

struct TimbalsFocusingCrystal : public Spell
{
    TimbalsFocusingCrystal(std::shared_ptr<Player> player);
};

struct MarkOfDefiance : public Spell
{
    MarkOfDefiance(std::shared_ptr<Player> player);
    void cast();
};

struct TheLightningCapacitor : public Spell
{
    TheLightningCapacitor(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
    void startCast(double predictedDamage = 0);
};

struct BladeOfWizardry : public Spell
{
    BladeOfWizardry(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct ShatteredSunPendantOfAcumen : public Spell
{
    ShatteredSunPendantOfAcumen(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct RobeOfTheElderScribes : public Spell
{
    RobeOfTheElderScribes(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct QuagmirransEye : public Spell
{
    QuagmirransEye(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct ShiffarsNexusHorn : public Spell
{
    ShiffarsNexusHorn(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct SextantOfUnstableCurrents : public Spell
{
    SextantOfUnstableCurrents(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct BandOfTheEternalSage : public Spell
{
    BandOfTheEternalSage(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct MysticalSkyfireDiamond : public Spell
{
    MysticalSkyfireDiamond(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct InsightfulEarthstormDiamond : public Spell
{
    InsightfulEarthstormDiamond(std::shared_ptr<Player> player);
    void cast();
};

struct AmplifyCurse : public Spell
{
    AmplifyCurse(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct PowerInfusion : public Spell
{
    PowerInfusion(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct Innervate : public Spell
{
    Innervate(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

struct ChippedPowerCore : public Spell
{
    ChippedPowerCore(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
    void cast();
};

struct CrackedPowerCore : public Spell
{
    CrackedPowerCore(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
    void cast();
};

struct ManaTideTotem : public Spell
{
    ManaTideTotem(std::shared_ptr<Player> player, std::shared_ptr<Aura> aura);
};

#endif