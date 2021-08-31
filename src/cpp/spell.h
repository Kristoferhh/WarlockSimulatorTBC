#pragma once

struct Player; // dirty fix for circular reference when we were including "player.h" before

#include <string>
#include <cmath>

enum SpellSchool {SHADOW, FIRE};
enum SpellType {AFFLICTION, DESTRUCTION};

struct Spell
{
    Player* player;
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
    bool isDot;
    bool doesDamage;
    bool canCrit;
    bool isNonWarlockAbility;
    bool casting;
    bool isItem;
    bool isAura;
    bool onGcd;
    bool isProc;
    std::string varName; // Same as 'name' except it's written in camelCase
    std::string name;
    std::string breakdownTable;

    Spell(Player* player);

    void reset();
    void setup();
    bool canCast();
    bool hasEnoughMana();
    virtual bool ready();
    virtual double getCastTime();
    virtual void startCast(double predictedDamage = 0);
    virtual void cast();
    double getModifier();
    double getConstantDamage(bool noRng);
    double getCritMultiplier(double critMultiplier);
    double predictDamage();
    void damage(bool isCrit);
    void tick(int t);
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
    int bonusDamageFromImmolateMin;
    int bonusDamageFromImmolateMax;
    double bonusDamageFromImmolate;
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
    double damage();
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
    Corruption(Player* player);
};

struct UnstableAffliction : public Spell
{
    UnstableAffliction(Player* player);
};

struct SiphonLife : public Spell
{
    SiphonLife(Player* player);
};

struct Immolate : public Spell
{
    Immolate(Player* player);
    double getModifier();
};

struct CurseOfAgony : public Spell
{
    CurseOfAgony(Player* player);
};

struct CurseOfTheElements : public Spell
{
    CurseOfTheElements(Player* player);
};

struct CurseOfRecklessness : public Spell
{
    CurseOfRecklessness(Player* player);
};

struct CurseOfDoom : public Spell
{
    CurseOfDoom(Player* player);
};

struct Conflagrate : public Spell
{
    Conflagrate(Player* player);
    void startCast();
};

struct DestructionPotion : public Spell
{
    DestructionPotion(Player* player);
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
    FlameCap(Player* player);
};

struct BloodFury : public Spell
{
    BloodFury(Player* player);
};

struct Bloodlust : public Spell
{
    Bloodlust(Player* player);
};

struct DrumsOfBattle : public Spell
{
    DrumsOfBattle(Player* player);
    bool ready();
};

struct DrumsOfWar : public Spell
{
    DrumsOfWar(Player* player);
    bool ready();
};

struct DrumsOfRestoration : public Spell
{
    DrumsOfRestoration(Player* player);
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
    TheLightningCapacitor(Player* player);
    void startCast();
};

struct BladeOfWizardry : public Spell
{
    BladeOfWizardry(Player* player);
};

struct ShatteredSunPendantOfAcumen : public Spell
{
    ShatteredSunPendantOfAcumen(Player* player);
};

struct RobeOfTheElderScribes : public Spell
{
    RobeOfTheElderScribes(Player* player);
};

struct QuagmirransEye : public Spell
{
    QuagmirransEye(Player* player);
};

struct ShiffarsNexusHorn : public Spell
{
    ShiffarsNexusHorn(Player* player);
};

struct SextantOfUnstableCurrents : public Spell
{
    SextantOfUnstableCurrents(Player* player);
};

struct BandOfTheEternalSage : public Spell
{
    BandOfTheEternalSage(Player* player);
};

struct MysticalSkyfireDiamond : public Spell
{
    MysticalSkyfireDiamond(Player* player);
};

struct InsightfulEarthstormDiamond : public Spell
{
    InsightfulEarthstormDiamond(Player* player);
    void cast();
};

struct AmplifyCurse : public Spell
{
    AmplifyCurse(Player* player);
};

struct PowerInfusion : public Spell
{
    PowerInfusion(Player* player);
};

struct Innervate : public Spell
{
    Innervate(Player* player);
};