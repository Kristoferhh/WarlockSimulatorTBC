#include "aura.h"

#include <iostream>

#include "common.h"
#include "player.h"

Aura::Aura(std::shared_ptr<Player> player) : player(player) {
  duration_remaining = 0;
  has_duration = true;
  group_wide = false;
  modifier = 1;
  stacks = 0;
  ticks_remaining = 0;
  tick_timer_remaining = 0;
  active = false;
}

void Aura::Setup() {
  if (player->combat_log_breakdown.count(name) == 0) {
    player->combat_log_breakdown.insert(std::make_pair(name, new CombatLogBreakdown(name)));
  }
}

void Aura::Tick(double t) {
  if (has_duration) {
    duration_remaining -= t;
    if (duration_remaining <= 0) {
      Fade();
    }
  }
}

void Aura::Apply() {
  if (active && player->ShouldWriteToCombatLog()) {
    player->CombatLog(name + " refreshed");
  } else if (!active) {
    bool recalculatePetStats = false;
    player->combat_log_breakdown.at(name)->applied_at = player->fight_time;

    if (stats != NULL && stats->spell_power > 0) {
      if (player->ShouldWriteToCombatLog()) {
        int currentSpellPower = player->GetSpellPower();
        player->CombatLog("Spell Power + " + std::to_string(stats->spell_power) + " (" +
                          std::to_string(currentSpellPower) + " -> " +
                          std::to_string(currentSpellPower + stats->spell_power) + ")");
      }
      player->stats->spell_power += stats->spell_power;
      recalculatePetStats = true;
    }
    if (stats != NULL && stats->shadow_power > 0) {
      if (player->ShouldWriteToCombatLog()) {
        int currentShadowPower = player->stats->shadow_power;
        player->CombatLog("Shadow Power + " + std::to_string(stats->shadow_power) + " (" +
                          std::to_string(currentShadowPower) + " -> " +
                          std::to_string(currentShadowPower + stats->shadow_power) + ")");
      }
      player->stats->shadow_power += stats->shadow_power;
      recalculatePetStats = true;
    }
    if (stats != NULL && stats->fire_power > 0) {
      if (player->ShouldWriteToCombatLog()) {
        int currentFirePower = player->stats->fire_power;
        player->CombatLog("Fire Power + " + std::to_string(stats->fire_power) + " (" +
                          std::to_string(currentFirePower) + " -> " +
                          std::to_string(currentFirePower + stats->fire_power) + ")");
      }
      player->stats->fire_power += stats->fire_power;
      recalculatePetStats = true;
    }
    if (stats != NULL && stats->haste_rating > 0) {
      if (player->ShouldWriteToCombatLog()) {
        int currentHasteRating = player->stats->haste_rating;
        player->CombatLog("Haste Rating + " + std::to_string(stats->haste_rating) + " (" +
                          std::to_string(currentHasteRating) + " -> " +
                          std::to_string(currentHasteRating + stats->haste_rating) + ")");
      }
      player->stats->haste_rating += stats->haste_rating;
    }
    if (stats != NULL && stats->haste_percent > 0) {
      if (player->ShouldWriteToCombatLog()) {
        player->CombatLog(
            "Haste % * " + std::to_string(stats->haste_percent) + " (" +
            std::to_string((player->stats->haste_percent - 1) * 100) + " -> " +
            std::to_string((player->stats->haste_percent * (1 + (stats->haste_percent / 100)) - 1) * 100) + ")");
      }
      player->stats->haste_percent *= (1 + stats->haste_percent / 100.0);
      if (player->pet != NULL && group_wide) {
        player->pet->stats->haste_percent *= (1 + stats->haste_percent / 100.0);
      }
    }
    if (stats != NULL && stats->mana_cost_modifier > 0) {
      if (player->ShouldWriteToCombatLog()) {
        double currentmanaCostModifier = player->stats->mana_cost_modifier;
        player->CombatLog(
            "Mana Cost Modifier * " + TruncateTrailingZeros(std::to_string(stats->mana_cost_modifier), 2) + " (" +
            TruncateTrailingZeros(std::to_string(currentmanaCostModifier), 2) + " -> " +
            TruncateTrailingZeros(std::to_string(currentmanaCostModifier * stats->mana_cost_modifier), 2) + ")");
      }
      player->stats->mana_cost_modifier *= stats->mana_cost_modifier;
    }

    if (recalculatePetStats && player->pet != NULL) {
      player->pet->CalculateStatsFromPlayer();
    }
    if (player->ShouldWriteToCombatLog()) {
      player->CombatLog(name + " applied");
    }

    active = true;
  }

  player->combat_log_breakdown.at(name)->count++;
  duration_remaining = duration;
}

void Aura::Fade() {
  if (!active) {
    player->ThrowError("Attempting to fade " + name + " when it isn't active");
  }

  active = false;
  bool recalculatePetStats = false;

  if (stats != NULL && stats->spell_power > 0) {
    if (player->ShouldWriteToCombatLog()) {
      int currentSpellPower = player->GetSpellPower();
      player->CombatLog("Spell Power - " + std::to_string(stats->spell_power) + " (" +
                        std::to_string(currentSpellPower) + " -> " +
                        std::to_string(currentSpellPower - stats->spell_power) + ")");
    }

    player->stats->spell_power -= stats->spell_power;
    recalculatePetStats = true;
  }
  if (stats != NULL && stats->shadow_power > 0) {
    if (player->ShouldWriteToCombatLog()) {
      int currentShadowPower = player->stats->shadow_power;
      player->CombatLog("Shadow Power - " + std::to_string(stats->shadow_power) + " (" +
                        std::to_string(currentShadowPower) + " -> " +
                        std::to_string(currentShadowPower - stats->shadow_power) + ")");
    }
    player->stats->shadow_power -= stats->shadow_power;
    recalculatePetStats = true;
  }
  if (stats != NULL && stats->fire_power > 0) {
    if (player->ShouldWriteToCombatLog()) {
      int currentFirePower = player->stats->fire_power;
      player->CombatLog("Fire Power - " + std::to_string(stats->fire_power) + " (" + std::to_string(currentFirePower) +
                        " -> " + std::to_string(currentFirePower - stats->fire_power) + ")");
    }
    player->stats->fire_power -= stats->fire_power;
    recalculatePetStats = true;
  }
  if (stats != NULL && stats->haste_rating > 0) {
    if (player->ShouldWriteToCombatLog()) {
      int currentHasteRating = player->stats->haste_rating;
      player->CombatLog("Haste Rating - " + std::to_string(stats->haste_rating) + " (" +
                        std::to_string(currentHasteRating) + " -> " +
                        std::to_string(currentHasteRating - stats->haste_rating) + ")");
    }
    player->stats->haste_rating -= stats->haste_rating;
  }
  if (stats != NULL && stats->haste_percent > 0) {
    if (player->ShouldWriteToCombatLog()) {
      player->CombatLog("Haste % / " + std::to_string(stats->haste_percent) + "% (" +
                        std::to_string((player->stats->haste_percent - 1) * 100) + " -> " +
                        std::to_string((player->stats->haste_percent / (1 + (stats->haste_percent / 100)) - 1) * 100) +
                        ")");
    }
    player->stats->haste_percent /= (1 + stats->haste_percent / 100.0);
    if (player->pet != NULL && group_wide) {
      player->pet->stats->haste_percent /= (1 + stats->haste_percent / 100.0);
    }
  }
  if (stats != NULL && stats->mana_cost_modifier > 0) {
    if (player->ShouldWriteToCombatLog()) {
      double currentmanaCostModifier = player->stats->mana_cost_modifier;
      player->CombatLog("Mana Cost Modifier / " + TruncateTrailingZeros(std::to_string(stats->mana_cost_modifier), 2) +
                        " (" + TruncateTrailingZeros(std::to_string(currentmanaCostModifier), 2) + " -> " +
                        TruncateTrailingZeros(std::to_string(currentmanaCostModifier / stats->mana_cost_modifier), 2) +
                        ")");
    }
    player->stats->mana_cost_modifier /= stats->mana_cost_modifier;
  }

  if (recalculatePetStats && player->pet != NULL) {
    player->pet->CalculateStatsFromPlayer();
  }

  if (player->ShouldWriteToCombatLog()) {
    player->CombatLog(name + " faded");
  }

  double auraUptime = player->fight_time - player->combat_log_breakdown.at(name)->applied_at;
  player->combat_log_breakdown.at(name)->uptime += auraUptime;
}

void Aura::DecrementStacks() {}

ImprovedShadowBoltAura::ImprovedShadowBoltAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Improved Shadow Bolt";
  duration = 12;
  stacks = 0;
  max_stacks = 4;
  Aura::modifier = 1 + player->talents->improved_shadow_bolt * 0.04;
  uptime_so_far = 0;
  Setup();
}

void ImprovedShadowBoltAura::Apply() {
  Aura::Apply();
  stacks = max_stacks;
}

void ImprovedShadowBoltAura::DecrementStacks() {
  stacks--;

  if (stacks <= 0) {
    Fade();
  } else if (player->ShouldWriteToCombatLog()) {
    player->CombatLog(name + " (" + std::to_string(stacks) + ")");
  }
}

void ImprovedShadowBoltAura::Fade() {
  Aura::Fade();
  uptime_so_far = player->combat_log_breakdown.at(name)->uptime / player->total_duration;
}

CurseOfTheElementsAura::CurseOfTheElementsAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Curse of the Elements";
  duration = 300;
  Setup();
}

CurseOfRecklessnessAura::CurseOfRecklessnessAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Curse of Recklessness";
  duration = 120;
  Setup();
}

ShadowTranceAura::ShadowTranceAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Shadow Trance (Nightfall)";
  duration = 10;
  Setup();
}

FlameshadowAura::FlameshadowAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Flameshadow";
  duration = 15;
  proc_chance = 5;
  Aura::stats = std::make_unique<AuraStats>(0, 135, 0, 0, 0, 0);
  Setup();
}

ShadowflameAura::ShadowflameAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Shadowflame";
  duration = 15;
  proc_chance = 5;
  Aura::stats = std::make_unique<AuraStats>(0, 0, 135, 0, 0, 0);
  Setup();
}

SpellstrikeAura::SpellstrikeAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Spellstrike";
  duration = 10;
  proc_chance = 5;
  Aura::stats = std::make_unique<AuraStats>(92, 0, 0, 0, 0, 0);
  Setup();
}

PowerInfusionAura::PowerInfusionAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Power Infusion";
  duration = 15;
  Aura::stats = std::make_unique<AuraStats>(0, 0, 0, 0, 20, 0.8);
  Setup();
}

EyeOfMagtheridonAura::EyeOfMagtheridonAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Eye of Magtheridon";
  duration = 10;
  Aura::stats = std::make_unique<AuraStats>(170, 0, 0, 0, 0, 0);
  Setup();
}

SextantOfUnstableCurrentsAura::SextantOfUnstableCurrentsAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Sextant of Unstable Currents";
  duration = 15;
  Aura::stats = std::make_unique<AuraStats>(190, 0, 0, 0, 0, 0);
  Setup();
}

QuagmirransEyeAura::QuagmirransEyeAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Quagmirran's Eye";
  duration = 6;
  Aura::stats = std::make_unique<AuraStats>(0, 0, 0, 320, 0, 0);
  Setup();
}

ShiffarsNexusHornAura::ShiffarsNexusHornAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Shiffar's Nexus-Horn";
  duration = 10;
  Aura::stats = std::make_unique<AuraStats>(225, 0, 0, 0, 0, 0);
  Setup();
}

ManaEtched4SetAura::ManaEtched4SetAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Mana-Etched 4-Set Bonus";
  duration = 15;
  proc_chance = 2;
  Aura::stats = std::make_unique<AuraStats>(110, 0, 0, 0, 0, 0);
  Setup();
}

DestructionPotionAura::DestructionPotionAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Destruction Potion";
  duration = 15;
  Setup();
}

void DestructionPotionAura::Apply() {
  if (!active) {
    if (player->ShouldWriteToCombatLog()) {
      int currentSpellPower = player->GetSpellPower();
      double currentCritChance = player->GetCritChance(SpellType::DESTRUCTION);
      player->CombatLog("Spell Power + 120 (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower + 120) +  + ")"")");
      player->CombatLog("Crit Chance + 2% (" + std::to_string(round(currentCritChance * 100) / 100) + "% -> " +
                        std::to_string(round((currentCritChance + 2) * 100) / 100) + "%)");
    }
    player->stats->spell_power += 120;
    player->stats->crit_chance += 2;
    if (player->pet != NULL) {
      player->pet->CalculateStatsFromPlayer();
    }
  }
  Aura::Apply();
}

void DestructionPotionAura::Fade() {
  if (active) {
    if (player->ShouldWriteToCombatLog()) {
      int currentSpellPower = player->GetSpellPower();
      double currentCritChance = player->GetCritChance(SpellType::DESTRUCTION);
      player->CombatLog("Spell Power - 120 (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower - 120) +  + ")"")");
      player->CombatLog("Crit Chance - 2% (" + std::to_string(round(currentCritChance * 100) / 100) + "% -> " +
                        std::to_string(round((currentCritChance - 2) * 100) / 100) + "%)");
    }
    player->stats->spell_power -= 120;
    player->stats->crit_chance -= 2;
    if (player->pet != NULL) {
      player->pet->CalculateStatsFromPlayer();
    }
  }
  Aura::Fade();
}

FlameCapAura::FlameCapAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Flame Cap";
  duration = 60;
  Aura::stats = std::make_unique<AuraStats>(0, 0, 80, 0, 0, 0);
  Setup();
}

BloodFuryAura::BloodFuryAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Blood Fury";
  duration = 15;
  Aura::stats = std::make_unique<AuraStats>(140, 0, 0, 0, 0, 0);
  Setup();
}

BloodlustAura::BloodlustAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Bloodlust";
  duration = 40;
  group_wide = true;
  Aura::stats = std::make_unique<AuraStats>(0, 0, 0, 0, 30, 0);
  Setup();
}

DrumsOfBattleAura::DrumsOfBattleAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Drums of Battle";
  duration = 30;
  group_wide = true;
  Aura::stats = std::make_unique<AuraStats>(0, 0, 0, 80, 0, 0);
  Setup();
}

DrumsOfWarAura::DrumsOfWarAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Drums of War";
  duration = 30;
  group_wide = true;
  Aura::stats = std::make_unique<AuraStats>(30, 0, 0, 0, 0, 0);
  Setup();
}

DrumsOfRestorationAura::DrumsOfRestorationAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Drums of Restoration";
  duration = 15;
  group_wide = true;
  tick_timer_total = 3;
  ticks_total = duration / tick_timer_total;
  manaGain = 600.0 / (duration / tick_timer_total);
  Setup();
}

void DrumsOfRestorationAura::Apply() {
  tick_timer_remaining = tick_timer_total;
  ticks_remaining = ticks_total;
  Aura::Apply();
}

void DrumsOfRestorationAura::Tick(double t) {
  tick_timer_remaining -= t;

  if (tick_timer_remaining <= 0) {
    int currentMana = player->stats->mana;
    player->stats->mana = std::min(player->stats->max_mana, player->stats->mana + manaGain);
    int manaGained = player->stats->mana - currentMana;
    player->combat_log_breakdown.at(name)->casts++;
    player->AddIterationDamageAndMana(name, manaGained, 0);
    if (player->ShouldWriteToCombatLog()) {
      player->CombatLog("Player gains " + std::to_string(manaGained) + " mana from Drums of Restoration (" +
                        std::to_string(currentMana) + " -> " + std::to_string(player->stats->mana) + ")" + ")");
    }
    // todo pet

    ticks_remaining--;
    tick_timer_remaining = tick_timer_total;

    if (ticks_remaining <= 0) {
      Aura::Fade();
    }
  }
}

AshtongueTalismanOfShadowsAura::AshtongueTalismanOfShadowsAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Ashtongue Talisman of Shadows";
  duration = 5;
  proc_chance = 20;
  Aura::stats = std::make_unique<AuraStats>(220, 0, 0, 0, 0, 0);
  Setup();
}

DarkmoonCardCrusadeAura::DarkmoonCardCrusadeAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Darkmoon Card: Crusade";
  duration = 10;
  max_stacks = 10;
  stacks = 0;
  spell_power_per_stack = 8;
  Setup();
}

void DarkmoonCardCrusadeAura::Apply() {
  if (stacks < max_stacks) {
    if (player->ShouldWriteToCombatLog()) {
      int currentSpellPower = player->GetSpellPower();
      player->CombatLog("Spell Power + " + std::to_string(spell_power_per_stack) + " (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower + spell_power_per_stack) +  + ")"")");
    }
    player->stats->spell_power += spell_power_per_stack;
    stacks++;
    if (player->pet != NULL) {
      player->pet->CalculateStatsFromPlayer();
    }
  }
  Aura::Apply();
}

void DarkmoonCardCrusadeAura::Fade() {
  if (player->ShouldWriteToCombatLog()) {
    int currentSpellPower = player->GetSpellPower();
    player->CombatLog("Spell Power - " + std::to_string(spell_power_per_stack) + " (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower - spell_power_per_stack) +  + ")"")");
  }
  if (player->pet != NULL) {
    player->pet->CalculateStatsFromPlayer();
  }
  player->stats->spell_power -= spell_power_per_stack * stacks;
  stacks = 0;
  Aura::Fade();
}

TheLightningCapacitorAura::TheLightningCapacitorAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "The Lightning Capacitor";
  has_duration = false;
  max_stacks = 3;
  Setup();
}

void TheLightningCapacitorAura::Apply() {
  if (stacks < max_stacks) {
    stacks++;
  }
  active = true;

  if (player->ShouldWriteToCombatLog()) {
    player->CombatLog(name + " + 1 stack (" + std::to_string(stacks) + ")");
  }
}

void TheLightningCapacitorAura::Fade() {
  stacks = 0;
  Aura::Fade();
}

BandOfTheEternalSageAura::BandOfTheEternalSageAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Band of the Eternal Sage";
  duration = 10;
  Aura::stats = std::make_unique<AuraStats>(95, 0, 0, 0, 0, 0);
  Setup();
}

BladeOfWizardryAura::BladeOfWizardryAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Blade of Wizardry";
  duration = 6;
  proc_chance = 15;
  Aura::stats = std::make_unique<AuraStats>(0, 0, 0, 280, 0, 0);
  Setup();
}

ShatteredSunPendantOfAcumenAura::ShatteredSunPendantOfAcumenAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Shattered Sun Pendant of Acumen (Aldor)";
  duration = 10;
  Aura::stats = std::make_unique<AuraStats>(120, 0, 0, 0, 0, 0);
  Setup();
}

RobeOfTheElderScribesAura::RobeOfTheElderScribesAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Robe of the Elder Scribes";
  duration = 10;
  Aura::stats = std::make_unique<AuraStats>(130, 0, 0, 0, 0, 0);
  Setup();
}

MysticalSkyfireDiamondAura::MysticalSkyfireDiamondAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Mystical Skyfire Diamond";
  duration = 4;
  Aura::stats = std::make_unique<AuraStats>(0, 0, 0, 320, 0, 0);
  Setup();
}

AmplifyCurseAura::AmplifyCurseAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Amplify Curse";
  duration = 30;
  Setup();
};

WrathOfCenariusAura::WrathOfCenariusAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Wrath of Cenarius";
  duration = 10;
  proc_chance = 5;
  Aura::stats = std::make_unique<AuraStats>(132, 0, 0, 0, 0, 0);
  Setup();
}

InnervateAura::InnervateAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Innervate";
  duration = 20;
  Setup();
}

ChippedPowerCoreAura::ChippedPowerCoreAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Chipped Power Core";
  duration = 30;
  Aura::stats = std::make_unique<AuraStats>(25, 0, 0, 0, 0, 0);
  Setup();
}

CrackedPowerCoreAura::CrackedPowerCoreAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Cracked Power Core";
  duration = 30;
  Aura::stats = std::make_unique<AuraStats>(15, 0, 0, 0, 0, 0);
  Setup();
}

ManaTideTotemAura::ManaTideTotemAura(std::shared_ptr<Player> player) : Aura(player) {
  name = "Mana Tide Totem";
  duration = 12;
  tick_timer_total = 3;
  ticks_total = duration / tick_timer_total;
  Setup();
};

void ManaTideTotemAura::Apply() {
  tick_timer_remaining = tick_timer_total;
  ticks_remaining = ticks_total;
  Aura::Apply();
}

void ManaTideTotemAura::Tick(double t) {
  tick_timer_remaining -= t;

  if (tick_timer_remaining <= 0) {
    int currentMana = player->stats->mana;
    int manaGain = player->stats->max_mana * 0.06;
    player->stats->mana = std::min(player->stats->max_mana, player->stats->mana + manaGain);
    int manaGained = player->stats->mana - currentMana;
    player->combat_log_breakdown.at(name)->casts++;
    player->AddIterationDamageAndMana(name, manaGained, 0);
    if (player->ShouldWriteToCombatLog()) {
      player->CombatLog("Player gains " + std::to_string(manaGained) + " mana from Mana Tide Totem (" +
                        std::to_string(currentMana) + " -> " + std::to_string(player->stats->mana) + ")");
    }

    ticks_remaining--;
    tick_timer_remaining = tick_timer_total;

    if (ticks_remaining <= 0) {
      Aura::Fade();
    }
  }
}