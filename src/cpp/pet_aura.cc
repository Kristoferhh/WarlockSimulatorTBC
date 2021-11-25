#include "pet_aura.h"

#include "common.h"
#include "player.h"

PetAura::PetAura(std::shared_ptr<Pet> pet)
    : pet(pet), duration(0), duration_remaining(0), active(false), stacks(0), max_stacks(0) {}

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

  if (pet->player.ShouldWriteToCombatLog()) {
    std::string msg = pet->name + " " + (active && stacks == max_stacks ? "refreshed" : "gains") + " " + name;

    if (pet->auras.demonic_frenzy != NULL && name == pet->auras.demonic_frenzy->name) {
      msg.append(" (" + std::to_string(stacks) + ") - Current AP: " + DoubleToString(round(pet->GetAttackPower())) +
                 ")");
    }

    pet->player.CombatLog(msg);
  }

  for (auto& stat : stats) {
    stat.AddStat();
  }

  if (stacks < max_stacks) {
    stacks++;
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
  name = "Demonic Frenzy";
  duration = 10;
  max_stacks = 10;
}

BlackBook::BlackBook(std::shared_ptr<Pet> pet) : PetAura(pet) {
  name = "Black Book";
  duration = 30;
  stats = std::vector<Stat>{SpellPower(pet->player, pet->buff_stats, EntityType::kPet, 200),
                            AttackPower(pet->player, pet->buff_stats, EntityType::kPet, 325)};
}

BattleSquawk::BattleSquawk(std::shared_ptr<Pet> pet) : PetAura(pet) {
  name = "Battle Squawk";
  duration = 300;
  stats = std::vector<Stat>{MeleeHastePercent(pet->player, pet->stats, EntityType::kPet,
                                              std::pow(1.05, pet->player.settings.battle_squawk_amount))};
}