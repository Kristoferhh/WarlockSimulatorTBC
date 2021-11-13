#include "pet_aura.h"

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

  if (pet->player->ShouldWriteToCombatLog()) {
    std::string msg = pet->name + " gains " + name;

    if (pet->auras->demonic_frenzy != NULL) {
      msg.append(" (" + std::to_string(stacks) + ") - Current AP: " + std::to_string(round(pet->GetAttackPower())) +
                 ")");
    }

    pet->player->CombatLog(msg);
  }
}

void PetAura::Fade() {
  active = false;
  duration_remaining = 0;

  if (pet->player->ShouldWriteToCombatLog()) {
    std::string msg = name;

    if (pet->auras->demonic_frenzy != NULL) {
      msg.append(" (" + std::to_string(stacks) + ")");
    }

    pet->player->CombatLog(msg);
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

  if (announce_in_combat_log && pet->player->ShouldWriteToCombatLog()) {
    pet->player->CombatLog(pet->name + " Spell Power + 200 (" + std::to_string(pet->stats->spell_power) + " -> " +
                           std::to_string(pet->stats->spell_power + 200) + ")");
    pet->player->CombatLog(pet->name + " Attack Power + 325 (" + std::to_string(pet->stats->attack_power) + " -> " +
                           std::to_string(pet->stats->attack_power + 325) + ")");
  }
  pet->buff_stats->spell_power += 200;
  pet->buff_stats->attack_power += 325;
  pet->CalculateStatsFromPlayer(announce_in_combat_log);
}

void BlackBook::Fade() {
  PetAura::Fade();

  if (pet->player->ShouldWriteToCombatLog()) {
    pet->player->CombatLog(pet->name + " Spell Power - 200 (" + std::to_string(pet->stats->spell_power) + " -> " +
                           std::to_string(pet->stats->spell_power - 200) + ")");
    pet->player->CombatLog(pet->name + " Attack Power - 325 (" + std::to_string(pet->stats->attack_power) + " -> " +
                           std::to_string(pet->stats->attack_power - 325) + ")");
  }
  pet->buff_stats->spell_power -= 200;
  pet->buff_stats->attack_power -= 325;
  pet->CalculateStatsFromPlayer();
}