#include "aura.h"

#include <iostream>

#include "common.h"
#include "player/player.h"

Aura::Aura(Player& player)
    : player(player),
      duration_remaining(0),
      has_duration(true),
      group_wide(false),
      modifier(1),
      stacks(0),
      ticks_remaining(0),
      tick_timer_remaining(0),
      active(false) {}

void Aura::Setup() {
  if (player.recording_combat_log_breakdown && player.combat_log_breakdown.count(name) == 0) {
    player.combat_log_breakdown.insert({name, std::make_unique<CombatLogBreakdown>(name)});
  }
  player.aura_list.push_back(this);
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
  if (active && player.ShouldWriteToCombatLog() && (stacks == max_stacks)) {
    player.CombatLog(name + " refreshed");
  } else if (!active) {
    if (player.recording_combat_log_breakdown) {
      player.combat_log_breakdown.at(name)->applied_at = player.fight_time;
    }

    for (auto& stat : stats) {
      stat.AddStat();
    }

    if (player.ShouldWriteToCombatLog()) {
      player.CombatLog(name + " applied");
    }

    active = true;
  }

  if (stacks < max_stacks) {
    stacks++;

    if (player.ShouldWriteToCombatLog()) {
      player.CombatLog(name + " (" + std::to_string(stacks) + ")");
    }

    for (auto& stat : stats_per_stack) {
      stat.AddStat();
    }
  }

  if (player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->count++;
  }
  duration_remaining = duration;
}

void Aura::Fade() {
  if (!active) {
    player.ThrowError("Attempting to fade " + name + " when it isn't active");
  }

  for (auto& stat : stats) {
    stat.RemoveStat();
  }

  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog(name + " faded");
  }

  if (player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->uptime +=
        player.fight_time - player.combat_log_breakdown.at(name)->applied_at;
  }

  if (stacks > 0) {
    for (auto& stat : stats_per_stack) {
      stat.RemoveStat(stacks);
    }
  }

  active = false;
  stacks = 0;
}

void Aura::DecrementStacks() {}

ImprovedShadowBoltAura::ImprovedShadowBoltAura(Player& player) : Aura(player) {
  name = SpellName::kImprovedShadowBolt;
  duration = 12;
  max_stacks = 4;
  Aura::modifier = 1 + player.talents.improved_shadow_bolt * 0.04;
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
  } else if (player.ShouldWriteToCombatLog()) {
    player.CombatLog(name + " (" + std::to_string(stacks) + ")");
  }
}

CurseOfTheElementsAura::CurseOfTheElementsAura(Player& player) : Aura(player) {
  name = SpellName::kCurseOfTheElements;
  duration = 300;
  Setup();
}

CurseOfRecklessnessAura::CurseOfRecklessnessAura(Player& player) : Aura(player) {
  name = SpellName::kCurseOfRecklessness;
  duration = 120;
  Setup();
}

ShadowTranceAura::ShadowTranceAura(Player& player) : Aura(player) {
  name = SpellName::kNightfall;
  duration = 10;
  Setup();
}

FlameshadowAura::FlameshadowAura(Player& player) : Aura(player) {
  name = SpellName::kFlameshadow;
  duration = 15;
  stats.push_back(ShadowPower(player, player.stats.shadow_power, EntityType::kPlayer, 135));
  Setup();
}

ShadowflameAura::ShadowflameAura(Player& player) : Aura(player) {
  name = SpellName::kShadowflame;
  duration = 15;
  stats.push_back(FirePower(player, player.stats.fire_power, EntityType::kPlayer, 135));
  Setup();
}

SpellstrikeAura::SpellstrikeAura(Player& player) : Aura(player) {
  name = SpellName::kSpellstrike;
  duration = 10;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 92));
  Setup();
}

PowerInfusionAura::PowerInfusionAura(Player& player) : Aura(player) {
  name = SpellName::kPowerInfusion;
  duration = 15;
  stats.push_back(SpellHastePercent(player, player.stats.spell_haste_percent, EntityType::kPlayer, 1.2));
  stats.push_back(ManaCostModifier(player, player.stats.mana_cost_modifier, EntityType::kPlayer, 0.8));
  Setup();
}

EyeOfMagtheridonAura::EyeOfMagtheridonAura(Player& player) : Aura(player) {
  name = SpellName::kEyeOfMagtheridon;
  duration = 10;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 170));
  Setup();
}

SextantOfUnstableCurrentsAura::SextantOfUnstableCurrentsAura(Player& player) : Aura(player) {
  name = SpellName::kSextantOfUnstableCurrents;
  duration = 15;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 190));
  Setup();
}

QuagmirransEyeAura::QuagmirransEyeAura(Player& player) : Aura(player) {
  name = SpellName::kQuagmirransEye;
  duration = 6;
  stats.push_back(SpellHasteRating(player, player.stats.spell_haste_rating, EntityType::kPlayer, 320));
  Setup();
}

ShiffarsNexusHornAura::ShiffarsNexusHornAura(Player& player) : Aura(player) {
  name = SpellName::kShiffarsNexusHorn;
  duration = 10;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 225));
  Setup();
}

ManaEtched4SetAura::ManaEtched4SetAura(Player& player) : Aura(player) {
  name = SpellName::kManaEtched4Set;
  duration = 15;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 110));
  Setup();
}

DestructionPotionAura::DestructionPotionAura(Player& player) : Aura(player) {
  name = SpellName::kDestructionPotion;
  duration = 15;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 120));
  stats.push_back(SpellCritChance(player, player.stats.spell_crit_chance, EntityType::kPlayer, 2));
  Setup();
}

FlameCapAura::FlameCapAura(Player& player) : Aura(player) {
  name = SpellName::kFlameCap;
  duration = 60;
  stats.push_back(FirePower(player, player.stats.fire_power, EntityType::kPlayer, 80));
  Setup();
}

BloodFuryAura::BloodFuryAura(Player& player) : Aura(player) {
  name = SpellName::kBloodFury;
  duration = 15;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 140));
  Setup();
}

BloodlustAura::BloodlustAura(Player& player) : Aura(player) {
  name = SpellName::kBloodlust;
  duration = 40;
  group_wide = true;
  stats.push_back(SpellHastePercent(player, player.stats.spell_haste_percent, EntityType::kPlayer, 1.3));
  if (player.pet != NULL) {
    stats.push_back(SpellHastePercent(player, player.pet->stats.spell_haste_percent, EntityType::kPet, 1.3));
    stats.push_back(MeleeHastePercent(player, player.pet->stats.melee_haste_percent, EntityType::kPet, 1.3));
  }
  Setup();
}

DrumsOfBattleAura::DrumsOfBattleAura(Player& player) : Aura(player) {
  name = SpellName::kDrumsOfBattle;
  duration = 30;
  group_wide = true;
  stats.push_back(SpellHasteRating(player, player.stats.spell_haste_rating, EntityType::kPlayer, 80));
  Setup();
}

DrumsOfWarAura::DrumsOfWarAura(Player& player) : Aura(player) {
  name = SpellName::kDrumsOfWar;
  duration = 30;
  group_wide = true;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 30));
  Setup();
}

AshtongueTalismanOfShadowsAura::AshtongueTalismanOfShadowsAura(Player& player) : Aura(player) {
  name = SpellName::kAshtongueTalismanOfShadows;
  duration = 5;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 220));
  Setup();
}

DarkmoonCardCrusadeAura::DarkmoonCardCrusadeAura(Player& player) : Aura(player) {
  name = SpellName::kDarkmoonCardCrusade;
  duration = 10;
  max_stacks = 10;
  Aura::stats_per_stack.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 8));
  Setup();
}

TheLightningCapacitorAura::TheLightningCapacitorAura(Player& player) : Aura(player) {
  name = SpellName::kTheLightningCapacitor;
  has_duration = false;
  max_stacks = 3;
  Setup();
}

BandOfTheEternalSageAura::BandOfTheEternalSageAura(Player& player) : Aura(player) {
  name = SpellName::kBandOfTheEternalSage;
  duration = 10;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 95));
  Setup();
}

BladeOfWizardryAura::BladeOfWizardryAura(Player& player) : Aura(player) {
  name = SpellName::kBladeOfWizardry;
  duration = 6;
  stats.push_back(SpellHasteRating(player, player.stats.spell_haste_rating, EntityType::kPlayer, 280));
  Setup();
}

ShatteredSunPendantOfAcumenAldorAura::ShatteredSunPendantOfAcumenAldorAura(Player& player) : Aura(player) {
  name = SpellName::kShatteredSunPendantOfAcumenAldor;
  duration = 10;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 120));
  Setup();
}

RobeOfTheElderScribesAura::RobeOfTheElderScribesAura(Player& player) : Aura(player) {
  name = SpellName::kRobeOfTheElderScribes;
  duration = 10;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 130));
  Setup();
}

MysticalSkyfireDiamondAura::MysticalSkyfireDiamondAura(Player& player) : Aura(player) {
  name = SpellName::kMysticalSkyfireDiamond;
  duration = 4;
  stats.push_back(SpellHasteRating(player, player.stats.spell_haste_rating, EntityType::kPlayer, 320));
  Setup();
}

AmplifyCurseAura::AmplifyCurseAura(Player& player) : Aura(player) {
  name = SpellName::kAmplifyCurse;
  duration = 30;
  Setup();
};

WrathOfCenariusAura::WrathOfCenariusAura(Player& player) : Aura(player) {
  name = SpellName::kWrathOfCenarius;
  duration = 10;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 132));
  Setup();
}

InnervateAura::InnervateAura(Player& player) : Aura(player) {
  name = SpellName::kInnervate;
  duration = 20;
  Setup();
}

ChippedPowerCoreAura::ChippedPowerCoreAura(Player& player) : Aura(player) {
  name = SpellName::kChippedPowerCore;
  duration = 30;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 25));
  Setup();
}

CrackedPowerCoreAura::CrackedPowerCoreAura(Player& player) : Aura(player) {
  name = SpellName::kCrackedPowerCore;
  duration = 30;
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 15));
  Setup();
}

AirmansRibbonOfGallantryAura::AirmansRibbonOfGallantryAura(Player& player) : Aura(player) {
  name = SpellName::kAirmansRibbonOfGallantry;
  duration = 30;  // should maybe lower this to 25 or so for more realism
  stats.push_back(SpellPower(player, player.stats.spell_power, EntityType::kPlayer, 80));
  Setup();
}