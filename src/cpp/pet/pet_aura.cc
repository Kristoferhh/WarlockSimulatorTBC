#include "pet_aura.h"

#include "../common.h"
#include "../player/player.h"

PetAura::PetAura(std::shared_ptr<Pet> pet)
    : pet(pet), duration(0), duration_remaining(0), active(false), stacks(0), max_stacks(0) {}

void PetAura::Setup() { pet->player.pet_aura_list.push_back(this); }

void PetAura::Tick(double t) {
  if (!active) {
    pet->player.ThrowError("Error: Ticking " + name + " when it isn't active");
  }

  duration_remaining -= t;

  if (duration_remaining <= 0) {
    Fade();
  }
}

void PetAura::Apply() {
  duration_remaining = duration;
  if (stacks < max_stacks) {
    stacks++;
  }

  if (pet->player.ShouldWriteToCombatLog()) {
    std::string msg = pet->name + " " + (active && stacks == max_stacks ? "refreshed" : "gains") + " " + name;

    if (name == SpellName::kDemonicFrenzy) {
      msg.append(" (" + std::to_string(stacks) + ") - Current AP: " + DoubleToString(round(pet->GetAttackPower())) +
                 ")");
    }

    pet->player.CombatLog(msg);
  }

  for (auto& stat : stats) {
    stat.AddStat();
  }

  active = true;
  pet->CalculateStatsFromPlayer();
}

void PetAura::Fade() {
  if (!active) {
    pet->player.ThrowError("Error: Trying to fade " + name + " when it isn't active");
  }

  active = false;
  duration_remaining = 0;
  stacks = 0;

  if (pet->player.ShouldWriteToCombatLog()) {
    pet->player.CombatLog(name + " faded");
  }

  for (auto& stat : stats) {
    stat.RemoveStat();
  }

  pet->CalculateStatsFromPlayer();
}

DemonicFrenzy::DemonicFrenzy(std::shared_ptr<Pet> pet) : PetAura(pet) {
  name = SpellName::kDemonicFrenzy;
  duration = 10;
  max_stacks = 10;
}

BlackBook::BlackBook(std::shared_ptr<Pet> pet) : PetAura(pet) {
  name = SpellName::kBlackBook;
  duration = 30;
  stats = std::vector<Stat>{SpellPower(pet->player, pet->buff_stats.spell_power, EntityType::kPet, 200),
                            AttackPower(pet->player, pet->buff_stats.attack_power, EntityType::kPet, 325)};
}

BattleSquawk::BattleSquawk(std::shared_ptr<Pet> pet) : PetAura(pet) {
  name = SpellName::kBattleSquawk;
  duration = 300;
  stats = std::vector<Stat>{MeleeHastePercent(pet->player, pet->stats.melee_haste_percent, EntityType::kPet,
                                              std::pow(1.05, pet->player.settings.battle_squawk_amount))};
}