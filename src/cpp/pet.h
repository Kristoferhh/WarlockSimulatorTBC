#ifndef WARLOCK_SIMULATOR_TBC_PET
#define WARLOCK_SIMULATOR_TBC_PET

struct Player;
#include "enums.h"
#include "pet_stats.h"
#include "pet_spells.h"
#include "pet_auras.h"
#include <iostream>

struct Pet : std::enable_shared_from_this<Pet>
{
    std::shared_ptr<Player> player;
    std::unique_ptr<PetSpells> spells;
    std::unique_ptr<PetAuras> auras;
    std::unique_ptr<PetStats> baseStats;
    std::unique_ptr<PetStats> buffStats; // Certain stats from buffs need to be separated from the main stat so we can re-calculate the pet's stats in calculateStatsFromPlayer().
    std::unique_ptr<PetStats> debuffStats;
    std::unique_ptr<PetStats> stats;
    PetName pet;
    PetMode mode;
    PetType petType;
    std::string name;
    double enemyDodgeChance;
    double castTimeRemaining;
    double fiveSecondRuleTimerRemaining;
    double spiritTickTimerRemaining;
    double critMultiplier;
    double glancingBlowMultiplier;
    double glancingBlowChance;
    double critSuppression;
    double armorMultiplier;
    double dmg;
    double baseMeleeSpeed;

    Pet(std::shared_ptr<Player> player);
    void initialize();
    void calculateStatsFromAuras();
    void calculateStatsFromPlayer(bool announceInCombatLog = true);
    void setup();
    void cast();
    void reset();
    void tick(double t);
    int getAttackPower();
    double getMeleeCritChance();
    double getMeleeHitChance();
    double getSpellCritChance();
    double getSpellHitChance();
    bool isCrit(AttackType type);
    bool isHit(AttackType type);
};

struct Imp : public Pet
{
    Imp(std::shared_ptr<Player> player);
};

struct Succubus : public Pet
{
    Succubus(std::shared_ptr<Player> player);
};

struct Felguard : public Pet
{
    Felguard(std::shared_ptr<Player> player);
};

#endif