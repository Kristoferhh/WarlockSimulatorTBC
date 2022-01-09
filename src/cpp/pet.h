#pragma once

struct Player;
#include <iostream>
#include <map>

#include "character_stats.h"
#include "entity.h"
#include "enums.h"
#include "simulation.h"

struct Pet : public Entity, std::enable_shared_from_this<Pet> {
  const double kBaseMeleeSpeed = 2;
  PetName pet_name;
  PetType pet_type;
  double glancing_blow_multiplier;
  double glancing_blow_chance;
  double crit_suppression;
  double enemy_damage_reduction_from_armor;

  Pet(Player& player, EmbindConstant selected_pet);
  void Initialize(Simulation* simulation);
  void CalculateStatsFromAuras();
  void Setup();
  void Reset();
  void Tick(double t);
  double GetAttackPower();
  double GetHastePercent();
  double GetSpellCritChance(SpellType spell_type = SpellType::kNoSpellType);
  double GetStamina();
  double GetIntellect();
  double GetAgility();
  double GetStrength();
  double GetPlayerSpellPower();
  double GetSpellPower(bool dealing_damage, SpellSchool spell_school);
  double GetDebuffAttackPower();
  double CalculateMaxMana();
  bool isMeleeCrit();
  bool IsHit(AttackType type);
};