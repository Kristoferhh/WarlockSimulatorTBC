#ifndef WARLOCK_SIMULATOR_TBC_PET
#define WARLOCK_SIMULATOR_TBC_PET

struct Player;
#include <iostream>
#include <map>

#include "../character_stats.h"
#include "../enums.h"
#include "pet_auras.h"
#include "pet_spells.h"

struct Pet : public Entity, std::enable_shared_from_this<Pet> {
  PetSpells spells;
  PetAuras auras;
  CharacterStats base_stats;
  CharacterStats buff_stats;  // Certain stats from buffs need to be separated from the main
                              // stat so we can re-calculate the pet's stats in
                              // calculateStatsFromPlayer().
  CharacterStats debuff_stats;
  PetName pet_name;
  PetType pet_type;
  double enemy_dodge_chance;
  double glancing_blow_multiplier;
  double glancing_blow_chance;
  double armor_multiplier;
  double dmg;
  double base_melee_speed;

  Pet(Player* player);
  void Initialize();
  void CalculateStatsFromAuras();
  void CalculateStatsFromPlayer(bool announce_in_combat_log = true);
  void Setup();
  void Cast();
  void Reset();
  void EndAuras();
  void Tick(double t);
  int GetAttackPower();
  double GetMeleeCritChance();
  double GetHastePercent();
  double GetSpellCritChance();
  int GetStamina();
  int GetIntellect();
  int GetSpirit();
  int GetAgility();
  int GetStrength();
  int GetPlayerSpellPower();
  bool IsCrit(AttackType type);
  bool IsHit(AttackType type);
};

struct Imp : public Pet {
  Imp(Player* player);
};

struct Succubus : public Pet {
  Succubus(Player* player);
};

struct Felguard : public Pet {
  Felguard(Player* player);
};

#endif