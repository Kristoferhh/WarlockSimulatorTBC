#include "pet_aura.h"

#include "../common.h"
#include "../player/player.h"

PetAura::PetAura(Pet& pet) : pet(pet), duration(0), duration_remaining(0), active(false), stacks(0), max_stacks(0) {}

void PetAura::Setup() {
  if (pet.recording_combat_log_breakdown && pet.combat_log_breakdown.count(name) == 0) {
    pet.combat_log_breakdown.insert({name, std::make_unique<CombatLogBreakdown>(name)});
  }

  pet.player->pet_aura_list.push_back(this);
}

void PetAura::Tick(double t) {
  if (!active) {
    pet.player->ThrowError("Error: Ticking " + name + " when it isn't active");
  }

  duration_remaining -= t;

  if (duration_remaining <= 0) {
    Fade();
  }
}

void PetAura::Apply() {
  if (stacks < max_stacks) {
    stacks++;
  }

  if (pet.ShouldWriteToCombatLog()) {
    std::string msg = pet.name + " " + (active && stacks == max_stacks ? "refreshed" : "gains") + " " + name;

    if (name == SpellName::kDemonicFrenzy) {
      msg.append(" (" + std::to_string(stacks) + ") - Current AP: " + DoubleToString(round(pet.GetAttackPower())) +
                 ")");
    }

    pet.player->CombatLog(msg);
  }

  if (!active) {
    if (pet.recording_combat_log_breakdown) {
      pet.combat_log_breakdown.at(name)->applied_at = pet.simulation->fight_time;
    }

    for (auto& stat : stats) {
      stat.AddStat();
    }
  }

  active = true;
  pet.CalculateStatsFromPlayer();
  duration_remaining = duration;
  if (pet.recording_combat_log_breakdown) {
    pet.combat_log_breakdown.at(name)->count++;
  }
}

void PetAura::Fade() {
  if (!active) {
    pet.player->ThrowError("Error: Trying to fade " + name + " when it isn't active");
  }

  active = false;
  duration_remaining = 0;
  stacks = 0;

  if (pet.ShouldWriteToCombatLog()) {
    pet.player->CombatLog(name + " faded");
  }

  if (pet.recording_combat_log_breakdown) {
    pet.combat_log_breakdown.at(name)->uptime +=
        pet.simulation->fight_time - pet.combat_log_breakdown.at(name)->applied_at;
  }

  for (auto& stat : stats) {
    stat.RemoveStat();
  }

  pet.CalculateStatsFromPlayer();
}

DemonicFrenzy::DemonicFrenzy(Pet& pet) : PetAura(pet) {
  name = SpellName::kDemonicFrenzy;
  duration = 10;
  max_stacks = 10;
  Setup();
}

BlackBook::BlackBook(Pet& pet) : PetAura(pet) {
  name = SpellName::kBlackBook;
  duration = 30;
  stats.push_back(SpellPower(pet, pet.buff_stats.spell_power, 200));
  stats.push_back(AttackPower(pet, 325));
  Setup();
}

BattleSquawk::BattleSquawk(Pet& pet) : PetAura(pet) {
  name = SpellName::kBattleSquawk;
  duration = 300;
  stats.push_back(MeleeHastePercent(pet, std::pow(1.05, pet.player->settings.battle_squawk_amount)));
  Setup();
}