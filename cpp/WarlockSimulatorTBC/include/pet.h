#pragma once
#include "entity.h"

enum class EmbindConstant;
struct Player;

struct Pet final : Entity, std::enable_shared_from_this<Pet> {
  const double kBaseMeleeSpeed = 2;
  PetName pet_name = PetName::kNoName;
  PetType pet_type = PetType::kNoPetType;
  double glancing_blow_multiplier;
  double glancing_blow_chance;
  double enemy_damage_reduction_from_armor = 0;

  Pet(Player& player_param, EmbindConstant kSelectedPet);
  void Initialize(Simulation* simulation_ptr) override;
  void CalculateStatsFromAuras();
  void Setup();
  void Reset() override;
  void Tick(double kTime) override;
  double GetAttackPower() const;
  double GetHastePercent() override;
  double GetSpellCritChance(SpellType spell_type = SpellType::kNoSpellType) override;
  double GetStamina() override;
  double GetIntellect() override;
  double GetAgility() const;
  double GetStrength() const;
  double GetPlayerSpellPower() const;
  double GetSpellPower(bool dealing_damage, SpellSchool spell_school) override;
  double GetDebuffAttackPower() const;
  double CalculateMaxMana();
  double GetDamageModifier(Spell& spell, bool kIsDot) override;
};
