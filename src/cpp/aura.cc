#include "aura.h"

#include <iostream>

#include "common.h"
#include "entity.h"
#include "player/player.h"

Aura::Aura(Entity& entity)
    : entity(entity),
      duration_remaining(0),
      has_duration(true),
      group_wide(false),
      modifier(1),
      stacks(0),
      ticks_remaining(0),
      tick_timer_remaining(0),
      active(false) {}

void Aura::Setup() {
  if (entity.recording_combat_log_breakdown && entity.combat_log_breakdown.count(name) == 0) {
    entity.combat_log_breakdown.insert({name, std::make_unique<CombatLogBreakdown>(name)});
  }

  entity.aura_list.push_back(this);
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
  if (active && entity.ShouldWriteToCombatLog() && (stacks == max_stacks)) {
    entity.CombatLog(name + " refreshed");
  } else if (!active) {
    if (entity.recording_combat_log_breakdown) {
      entity.combat_log_breakdown.at(name)->applied_at = entity.simulation->fight_time;
    }

    for (auto& stat : stats) {
      stat.AddStat();
    }

    if (entity.ShouldWriteToCombatLog()) {
      entity.CombatLog(name + " applied");
    }

    active = true;
  }

  if (stacks < max_stacks) {
    stacks++;

    if (entity.ShouldWriteToCombatLog()) {
      entity.CombatLog(name + " (" + std::to_string(stacks) + ")");
    }

    for (auto& stat : stats_per_stack) {
      stat.AddStat();
    }
  }

  if (entity.recording_combat_log_breakdown) {
    entity.combat_log_breakdown.at(name)->count++;
  }

  duration_remaining = duration;
}

void Aura::Fade() {
  if (!active) {
    entity.player.ThrowError("Attempting to fade " + name + " when it isn't active");
  }

  for (auto& stat : stats) {
    stat.RemoveStat();
  }

  if (entity.ShouldWriteToCombatLog()) {
    entity.CombatLog(name + " faded");
  }

  if (entity.recording_combat_log_breakdown) {
    entity.combat_log_breakdown.at(name)->uptime +=
        entity.simulation->fight_time - entity.combat_log_breakdown.at(name)->applied_at;
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

ImprovedShadowBoltAura::ImprovedShadowBoltAura(Entity& entity) : Aura(entity) {
  name = SpellName::kImprovedShadowBolt;
  duration = 12;
  max_stacks = 4;
  Aura::modifier = 1 + entity.player.talents.improved_shadow_bolt * 0.04;
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
  } else if (entity.ShouldWriteToCombatLog()) {
    entity.CombatLog(name + " (" + std::to_string(stacks) + ")");
  }
}

CurseOfTheElementsAura::CurseOfTheElementsAura(Entity& entity) : Aura(entity) {
  name = SpellName::kCurseOfTheElements;
  duration = 300;
  Setup();
}

CurseOfRecklessnessAura::CurseOfRecklessnessAura(Entity& entity) : Aura(entity) {
  name = SpellName::kCurseOfRecklessness;
  duration = 120;
  Setup();
}

ShadowTranceAura::ShadowTranceAura(Entity& entity) : Aura(entity) {
  name = SpellName::kNightfall;
  duration = 10;
  Setup();
}

FlameshadowAura::FlameshadowAura(Entity& entity) : Aura(entity) {
  name = SpellName::kFlameshadow;
  duration = 15;
  stats.push_back(ShadowPower(entity, 135));
  Setup();
}

ShadowflameAura::ShadowflameAura(Entity& entity) : Aura(entity) {
  name = SpellName::kShadowflame;
  duration = 15;
  stats.push_back(FirePower(entity, 135));
  Setup();
}

SpellstrikeAura::SpellstrikeAura(Entity& entity) : Aura(entity) {
  name = SpellName::kSpellstrike;
  duration = 10;
  stats.push_back(SpellPower(entity, 92));
  Setup();
}

PowerInfusionAura::PowerInfusionAura(Entity& entity) : Aura(entity) {
  name = SpellName::kPowerInfusion;
  duration = 15;
  stats.push_back(SpellHastePercent(entity, 1.2));
  stats.push_back(ManaCostModifier(entity, 0.8));
  Setup();
}

EyeOfMagtheridonAura::EyeOfMagtheridonAura(Entity& entity) : Aura(entity) {
  name = SpellName::kEyeOfMagtheridon;
  duration = 10;
  stats.push_back(SpellPower(entity, 170));
  Setup();
}

SextantOfUnstableCurrentsAura::SextantOfUnstableCurrentsAura(Entity& entity) : Aura(entity) {
  name = SpellName::kSextantOfUnstableCurrents;
  duration = 15;
  stats.push_back(SpellPower(entity, 190));
  Setup();
}

QuagmirransEyeAura::QuagmirransEyeAura(Entity& entity) : Aura(entity) {
  name = SpellName::kQuagmirransEye;
  duration = 6;
  stats.push_back(SpellHasteRating(entity, 320));
  Setup();
}

ShiffarsNexusHornAura::ShiffarsNexusHornAura(Entity& entity) : Aura(entity) {
  name = SpellName::kShiffarsNexusHorn;
  duration = 10;
  stats.push_back(SpellPower(entity, 225));
  Setup();
}

ManaEtched4SetAura::ManaEtched4SetAura(Entity& entity) : Aura(entity) {
  name = SpellName::kManaEtched4Set;
  duration = 15;
  stats.push_back(SpellPower(entity, 110));
  Setup();
}

DestructionPotionAura::DestructionPotionAura(Entity& entity) : Aura(entity) {
  name = SpellName::kDestructionPotion;
  duration = 15;
  stats.push_back(SpellPower(entity, 120));
  stats.push_back(SpellCritChance(entity, 2));
  Setup();
}

FlameCapAura::FlameCapAura(Entity& entity) : Aura(entity) {
  name = SpellName::kFlameCap;
  duration = 60;
  stats.push_back(FirePower(entity, 80));
  Setup();
}

BloodFuryAura::BloodFuryAura(Entity& entity) : Aura(entity) {
  name = SpellName::kBloodFury;
  duration = 15;
  stats.push_back(SpellPower(entity, 140));
  Setup();
}

BloodlustAura::BloodlustAura(Entity& entity) : Aura(entity) {
  name = SpellName::kBloodlust;
  duration = 40;
  group_wide = true;
  stats.push_back(SpellHastePercent(entity, 1.3));
  if (entity.pet != NULL) {
    stats.push_back(SpellHastePercent(*entity.pet.get(), 1.3));
    stats.push_back(MeleeHastePercent(*entity.pet.get(), 1.3));
  }
  Setup();
}

DrumsOfBattleAura::DrumsOfBattleAura(Entity& entity) : Aura(entity) {
  name = SpellName::kDrumsOfBattle;
  duration = 30;
  group_wide = true;
  stats.push_back(SpellHasteRating(entity, 80));
  Setup();
}

DrumsOfWarAura::DrumsOfWarAura(Entity& entity) : Aura(entity) {
  name = SpellName::kDrumsOfWar;
  duration = 30;
  group_wide = true;
  stats.push_back(SpellPower(entity, 30));
  Setup();
}

AshtongueTalismanOfShadowsAura::AshtongueTalismanOfShadowsAura(Entity& entity) : Aura(entity) {
  name = SpellName::kAshtongueTalismanOfShadows;
  duration = 5;
  stats.push_back(SpellPower(entity, 220));
  Setup();
}

DarkmoonCardCrusadeAura::DarkmoonCardCrusadeAura(Entity& entity) : Aura(entity) {
  name = SpellName::kDarkmoonCardCrusade;
  duration = 10;
  max_stacks = 10;
  stats_per_stack.push_back(SpellPower(entity, 8));
  Setup();
}

TheLightningCapacitorAura::TheLightningCapacitorAura(Entity& entity) : Aura(entity) {
  name = SpellName::kTheLightningCapacitor;
  has_duration = false;
  max_stacks = 3;
  Setup();
}

BandOfTheEternalSageAura::BandOfTheEternalSageAura(Entity& entity) : Aura(entity) {
  name = SpellName::kBandOfTheEternalSage;
  duration = 10;
  stats.push_back(SpellPower(entity, 95));
  Setup();
}

BladeOfWizardryAura::BladeOfWizardryAura(Entity& entity) : Aura(entity) {
  name = SpellName::kBladeOfWizardry;
  duration = 6;
  stats.push_back(SpellHasteRating(entity, 280));
  Setup();
}

ShatteredSunPendantOfAcumenAldorAura::ShatteredSunPendantOfAcumenAldorAura(Entity& entity) : Aura(entity) {
  name = SpellName::kShatteredSunPendantOfAcumenAldor;
  duration = 10;
  stats.push_back(SpellPower(entity, 120));
  Setup();
}

RobeOfTheElderScribesAura::RobeOfTheElderScribesAura(Entity& entity) : Aura(entity) {
  name = SpellName::kRobeOfTheElderScribes;
  duration = 10;
  stats.push_back(SpellPower(entity, 130));
  Setup();
}

MysticalSkyfireDiamondAura::MysticalSkyfireDiamondAura(Entity& entity) : Aura(entity) {
  name = SpellName::kMysticalSkyfireDiamond;
  duration = 4;
  stats.push_back(SpellHasteRating(entity, 320));
  Setup();
}

AmplifyCurseAura::AmplifyCurseAura(Entity& entity) : Aura(entity) {
  name = SpellName::kAmplifyCurse;
  duration = 30;
  Setup();
};

WrathOfCenariusAura::WrathOfCenariusAura(Entity& entity) : Aura(entity) {
  name = SpellName::kWrathOfCenarius;
  duration = 10;
  stats.push_back(SpellPower(entity, 132));
  Setup();
}

InnervateAura::InnervateAura(Entity& entity) : Aura(entity) {
  name = SpellName::kInnervate;
  duration = 20;
  Setup();
}

ChippedPowerCoreAura::ChippedPowerCoreAura(Entity& entity) : Aura(entity) {
  name = SpellName::kChippedPowerCore;
  duration = 30;
  stats.push_back(SpellPower(entity, 25));
  Setup();
}

CrackedPowerCoreAura::CrackedPowerCoreAura(Entity& entity) : Aura(entity) {
  name = SpellName::kCrackedPowerCore;
  duration = 30;
  stats.push_back(SpellPower(entity, 15));
  Setup();
}

AirmansRibbonOfGallantryAura::AirmansRibbonOfGallantryAura(Entity& entity) : Aura(entity) {
  name = SpellName::kAirmansRibbonOfGallantry;
  duration = 30;  // should maybe lower this to 25 or so for more realism
  stats.push_back(SpellPower(entity, 80));
  Setup();
}

DemonicFrenzy::DemonicFrenzy(Entity& entity) : Aura(entity) {
  name = SpellName::kDemonicFrenzy;
  duration = 10;
  max_stacks = 10;
  Setup();
}

BlackBook::BlackBook(Entity& entity) : Aura(entity) {
  name = SpellName::kBlackBook;
  duration = 30;
  stats.push_back(SpellPower(entity, 200));
  stats.push_back(AttackPower(entity, 325));
  Setup();
}

BattleSquawk::BattleSquawk(Entity& entity) : Aura(entity) {
  name = SpellName::kBattleSquawk;
  duration = 300;
  stats.push_back(MeleeHastePercent(entity, std::pow(1.05, entity.player.settings.battle_squawk_amount)));
  Setup();
}