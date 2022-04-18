#include "../include/aura.h"

#include <memory>

#include "../include/entity.h"
#include "../include/combat_log_breakdown.h"
#include "../include/simulation.h"
#include "../include/stat.h"
#include "../include/player.h"
#include "../include/talents.h"
#include "../include/pet.h"
#include "../include/player_settings.h"

Aura::Aura(Entity& entity_param)
  : entity(entity_param) {
}

void Aura::Setup() {
  if (entity.recording_combat_log_breakdown && !entity.combat_log_breakdown.contains(name)) {
    entity.combat_log_breakdown.insert({name, std::make_shared<CombatLogBreakdown>(name)});
  }

  entity.aura_list.push_back(this);
}

void Aura::Tick(const double kTime) {
  if (has_duration) {
    duration_remaining -= kTime;
    if (duration_remaining <= 0) {
      Fade();
    }
  }
}

void Aura::Apply() {
  if (active && entity.ShouldWriteToCombatLog() && stacks == max_stacks) {
    entity.CombatLog(name + " refreshed");
  } else if (!active) {
    if (entity.recording_combat_log_breakdown) {
      entity.combat_log_breakdown.at(name)->applied_at = entity.simulation->current_fight_time;
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
    entity.player->ThrowError("Attempting to fade " + name + " when it isn't active");
  }

  for (auto& stat : stats) {
    stat.RemoveStat();
  }

  if (entity.ShouldWriteToCombatLog()) {
    entity.CombatLog(name + " faded");
  }

  if (entity.recording_combat_log_breakdown) {
    entity.combat_log_breakdown.at(name)->uptime +=
        entity.simulation->current_fight_time - entity.combat_log_breakdown.at(name)->applied_at;
  }

  if (stacks > 0) {
    for (auto& stat : stats_per_stack) {
      stat.RemoveStat(stacks);
    }
  }

  active = false;
  stacks = 0;
}

void Aura::DecrementStacks() {
}

ImprovedShadowBoltAura::ImprovedShadowBoltAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kImprovedShadowBolt;
  duration = 12;
  max_stacks = 4;
  modifier = 1 + entity_param.player->talents.improved_shadow_bolt * 0.04;
  Aura::Setup();
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

CurseOfTheElementsAura::CurseOfTheElementsAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kCurseOfTheElements;
  duration = 300;
  Aura::Setup();
}

CurseOfRecklessnessAura::CurseOfRecklessnessAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kCurseOfRecklessness;
  duration = 120;
  Aura::Setup();
}

ShadowTranceAura::ShadowTranceAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kNightfall;
  duration = 10;
  Aura::Setup();
}

FlameshadowAura::FlameshadowAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kFlameshadow;
  duration = 15;
  stats.push_back(ShadowPower(entity_param, 135));
  Aura::Setup();
}

ShadowflameAura::ShadowflameAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kShadowflame;
  duration = 15;
  stats.push_back(FirePower(entity_param, 135));
  Aura::Setup();
}

SpellstrikeAura::SpellstrikeAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kSpellstrike;
  duration = 10;
  stats.push_back(SpellPower(entity_param, 92));
  Aura::Setup();
}

PowerInfusionAura::PowerInfusionAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kPowerInfusion;
  duration = 15;
  stats.push_back(SpellHastePercent(entity_param, 1.2));
  stats.push_back(ManaCostModifier(entity_param, 0.8));
  Aura::Setup();
}

EyeOfMagtheridonAura::EyeOfMagtheridonAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kEyeOfMagtheridon;
  duration = 10;
  stats.push_back(SpellPower(entity_param, 170));
  Aura::Setup();
}

SextantOfUnstableCurrentsAura::SextantOfUnstableCurrentsAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kSextantOfUnstableCurrents;
  duration = 15;
  stats.push_back(SpellPower(entity_param, 190));
  Aura::Setup();
}

QuagmirransEyeAura::QuagmirransEyeAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kQuagmirransEye;
  duration = 6;
  stats.push_back(SpellHasteRating(entity_param, 320));
  Aura::Setup();
}

ShiffarsNexusHornAura::ShiffarsNexusHornAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kShiffarsNexusHorn;
  duration = 10;
  stats.push_back(SpellPower(entity_param, 225));
  Aura::Setup();
}

ManaEtched4SetAura::ManaEtched4SetAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kManaEtched4Set;
  duration = 15;
  stats.push_back(SpellPower(entity_param, 110));
  Aura::Setup();
}

DestructionPotionAura::DestructionPotionAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kDestructionPotion;
  duration = 15;
  stats.push_back(SpellPower(entity_param, 120));
  stats.push_back(SpellCritChance(entity_param, 2));
  Aura::Setup();
}

FlameCapAura::FlameCapAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kFlameCap;
  duration = 60;
  stats.push_back(FirePower(entity_param, 80));
  Aura::Setup();
}

BloodFuryAura::BloodFuryAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kBloodFury;
  duration = 15;
  stats.push_back(SpellPower(entity_param, 140));
  Aura::Setup();
}

BloodlustAura::BloodlustAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kBloodlust;
  duration = 40;
  group_wide = true;
  stats.push_back(SpellHastePercent(entity_param, 1.3));
  if (entity_param.pet != nullptr) {
    stats.push_back(SpellHastePercent(*entity_param.pet, 1.3));
    stats.push_back(MeleeHastePercent(*entity_param.pet, 1.3));
  }
  Aura::Setup();
}

DrumsOfBattleAura::DrumsOfBattleAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kDrumsOfBattle;
  duration = 30;
  group_wide = true;
  stats.push_back(SpellHasteRating(entity_param, 80));
  Aura::Setup();
}

DrumsOfWarAura::DrumsOfWarAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kDrumsOfWar;
  duration = 30;
  group_wide = true;
  stats.push_back(SpellPower(entity_param, 30));
  Aura::Setup();
}

AshtongueTalismanOfShadowsAura::AshtongueTalismanOfShadowsAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kAshtongueTalismanOfShadows;
  duration = 5;
  stats.push_back(SpellPower(entity_param, 220));
  Aura::Setup();
}

DarkmoonCardCrusadeAura::DarkmoonCardCrusadeAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kDarkmoonCardCrusade;
  duration = 10;
  max_stacks = 10;
  stats_per_stack.push_back(SpellPower(entity_param, 8));
  Aura::Setup();
}

TheLightningCapacitorAura::TheLightningCapacitorAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kTheLightningCapacitor;
  has_duration = false;
  max_stacks = 3;
  Aura::Setup();
}

BandOfTheEternalSageAura::BandOfTheEternalSageAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kBandOfTheEternalSage;
  duration = 10;
  stats.push_back(SpellPower(entity_param, 95));
  Aura::Setup();
}

BladeOfWizardryAura::BladeOfWizardryAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kBladeOfWizardry;
  duration = 6;
  stats.push_back(SpellHasteRating(entity_param, 280));
  Aura::Setup();
}

ShatteredSunPendantOfAcumenAldorAura::ShatteredSunPendantOfAcumenAldorAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kShatteredSunPendantOfAcumenAldor;
  duration = 10;
  stats.push_back(SpellPower(entity_param, 120));
  Aura::Setup();
}

RobeOfTheElderScribesAura::RobeOfTheElderScribesAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kRobeOfTheElderScribes;
  duration = 10;
  stats.push_back(SpellPower(entity_param, 130));
  Aura::Setup();
}

MysticalSkyfireDiamondAura::MysticalSkyfireDiamondAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kMysticalSkyfireDiamond;
  duration = 4;
  stats.push_back(SpellHasteRating(entity_param, 320));
  Aura::Setup();
}

AmplifyCurseAura::AmplifyCurseAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kAmplifyCurse;
  duration = 30;
  Aura::Setup();
}

WrathOfCenariusAura::WrathOfCenariusAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kWrathOfCenarius;
  duration = 10;
  stats.push_back(SpellPower(entity_param, 132));
  Aura::Setup();
}

InnervateAura::InnervateAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kInnervate;
  duration = 20;
  Aura::Setup();
}

ChippedPowerCoreAura::ChippedPowerCoreAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kChippedPowerCore;
  duration = 30;
  stats.push_back(SpellPower(entity_param, 25));
  Aura::Setup();
}

CrackedPowerCoreAura::CrackedPowerCoreAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kCrackedPowerCore;
  duration = 30;
  stats.push_back(SpellPower(entity_param, 15));
  Aura::Setup();
}

AirmansRibbonOfGallantryAura::AirmansRibbonOfGallantryAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kAirmansRibbonOfGallantry;
  duration = 30;  // should maybe lower this to 25 or so for more realism
  stats.push_back(SpellPower(entity_param, 80));
  Aura::Setup();
}

DemonicFrenzyAura::DemonicFrenzyAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kDemonicFrenzy;
  duration = 10;
  max_stacks = 10;
  Aura::Setup();
}

BlackBookAura::BlackBookAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kBlackBook;
  duration = 30;
  stats.push_back(SpellPower(entity_param, 200));
  stats.push_back(AttackPower(entity_param, 325));
  Aura::Setup();
}

BattleSquawkAura::BattleSquawkAura(Entity& entity_param)
  : Aura(entity_param) {
  name = SpellName::kBattleSquawk;
  duration = 300;
  stats.push_back(MeleeHastePercent(entity_param, std::pow(1.05, entity_param.player->settings.battle_squawk_amount)));
  Aura::Setup();
}