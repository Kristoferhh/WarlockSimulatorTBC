#ifndef WARLOCK_SIMULATOR_TBC_PET
#define WARLOCK_SIMULATOR_TBC_PET

struct Player;
#include <iostream>

#include "enums.h"
#include "pet_auras.h"
#include "pet_spells.h"
#include "pet_stats.h"

struct Pet : std::enable_shared_from_this<Pet> {
  std::shared_ptr<Player> player;
  std::unique_ptr<PetSpells> spells;
  std::unique_ptr<PetAuras> auras;
  std::unique_ptr<PetStats> base_stats;
  std::unique_ptr<PetStats> buff_stats;  // Certain stats from buffs need to be separated from the main
                                         // stat so we can re-calculate the pet's stats in
                                         // calculateStatsFromPlayer().
  std::unique_ptr<PetStats> debuff_stats;
  std::unique_ptr<PetStats> stats;
  PetName pet;
  PetMode mode;
  PetType pet_type;
  std::string name;
  double enemy_dodge_chance;
  double cast_time_remaining;
  double five_second_rule_timer_remaining;
  double spirit_tick_timer_remaining;
  double crit_multiplier;
  double glancing_blow_multiplier;
  double glancing_blow_chance;
  double crit_suppression;
  double armor_multiplier;
  double dmg;
  double base_melee_speed;

  Pet(std::shared_ptr<Player> player);
  void Initialize();
  void CalculateStatsFromAuras();
  void CalculateStatsFromPlayer(bool announce_in_combat_log = true);
  void Setup();
  void Cast();
  void Reset();
  void Tick(double t);
  int GetAttackPower();
  double GetMeleeCritChance();
  double GetMeleeHitChance();
  double GetSpellCritChance();
  double GetSpellHitChance();
  bool IsCrit(AttackType type);
  bool IsHit(AttackType type);
};

struct Imp : public Pet {
  Imp(std::shared_ptr<Player> player);
};

struct Succubus : public Pet {
  Succubus(std::shared_ptr<Player> player);
};

struct Felguard : public Pet {
  Felguard(std::shared_ptr<Player> player);
};

#endif