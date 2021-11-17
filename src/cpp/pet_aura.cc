#include "pet_aura.h"

#include "common.h"
#include "player.h"

PetAura::PetAura(std::shared_ptr<Pet> pet) : pet(pet) {
  duration = 0;
  duration_remaining = 0;
  active = false;
}

void PetAura::Tick(double t) {
  duration_remaining -= t;

  if (active && duration_remaining <= 0) {
    Fade();
  }
}

void PetAura::Apply() {
  active = true;
  duration_remaining = duration;

  if (pet->player.ShouldWriteToCombatLog()) {
    std::string msg = pet->name + " gains " + name;

    if (pet->auras->demonic_frenzy != NULL && name == pet->auras->demonic_frenzy->name) {
      msg.append(" (" + std::to_string(stacks) + ") - Current AP: " + DoubleToString(round(pet->GetAttackPower())) +
                 ")");
    }

    pet->player.CombatLog(msg);
  }
}

void PetAura::Fade() {
  active = false;
  duration_remaining = 0;

  if (pet->player.ShouldWriteToCombatLog()) {
    pet->player.CombatLog(name + " faded");
  }
}

DemonicFrenzy::DemonicFrenzy(std::shared_ptr<Pet> pet) : PetAura(pet) {
  name = "Demonic Frenzy";
  duration = 10;
  max_stacks = 10;
  stacks = 0;
}

void DemonicFrenzy::Apply() {
  if (stacks < max_stacks) {
    stacks++;
  }

  PetAura::Apply();
}

void DemonicFrenzy::Fade() {
  stacks = 0;
  PetAura::Fade();
}

BlackBook::BlackBook(std::shared_ptr<Pet> pet) : PetAura(pet) {
  name = "Black Book";
  duration = 30;
}

void BlackBook::Apply(bool announce_in_combat_log) {
  PetAura::Apply();

  if (announce_in_combat_log && pet->player.ShouldWriteToCombatLog()) {
    pet->player.CombatLog(pet->name + " Spell Power + 200 (" + std::to_string(pet->stats->spell_power) + " -> " +
                          std::to_string(pet->stats->spell_power + 200) + ")");
    pet->player.CombatLog(pet->name + " Attack Power + 325 (" + std::to_string(pet->stats->attack_power) + " -> " +
                          std::to_string(pet->stats->attack_power + 325) + ")");
  }
  pet->buff_stats->spell_power += 200;
  pet->buff_stats->attack_power += 325;
  pet->CalculateStatsFromPlayer(announce_in_combat_log);
}

void BlackBook::Fade() {
  PetAura::Fade();

  if (pet->player.ShouldWriteToCombatLog()) {
    pet->player.CombatLog(pet->name + " Spell Power - 200 (" + std::to_string(pet->stats->spell_power) + " -> " +
                          std::to_string(pet->stats->spell_power - 200) + ")");
    pet->player.CombatLog(pet->name + " Attack Power - 325 (" + std::to_string(pet->stats->attack_power) + " -> " +
                          std::to_string(pet->stats->attack_power - 325) + ")");
  }
  pet->buff_stats->spell_power -= 200;
  pet->buff_stats->attack_power -= 325;
  pet->CalculateStatsFromPlayer();
}

BattleSquawk::BattleSquawk(std::shared_ptr<Pet> pet) : PetAura(pet) {
  name = "Battle Squawk";
  duration = 300;
  haste_percent = std::pow(1.05, pet->player.settings.battle_squawk_amount);
}

void BattleSquawk::Apply() {
  PetAura::Apply();
  const double kCurrentHastePercent = pet->GetHastePercent();
  if (pet->player.ShouldWriteToCombatLog()) {
    pet->player.CombatLog(pet->name + " Melee Haste % * " + DoubleToString(haste_percent * 100 - 100, 2) + " (" +
                          DoubleToString(kCurrentHastePercent * 100 - 100, 2) + "% -> " +
                          DoubleToString((kCurrentHastePercent * haste_percent) * 100 - 100, 2) + "%)");
  }
  pet->stats->melee_haste_percent = pet->stats->melee_haste_percent * haste_percent;
}

void BattleSquawk::Fade() {
  PetAura::Fade();
  const double kCurrentHastePercent = pet->GetHastePercent();
  if (pet->player.ShouldWriteToCombatLog()) {
    pet->player.CombatLog(pet->name + " Melee Haste % / " + DoubleToString(haste_percent * 100 - 100, 2) + " (" +
                          DoubleToString(kCurrentHastePercent * 100 - 100, 2) + "% -> " +
                          DoubleToString((kCurrentHastePercent / haste_percent) * 100 - 100, 2) + "%)");
  }
  pet->stats->melee_haste_percent = pet->stats->melee_haste_percent / haste_percent;
}