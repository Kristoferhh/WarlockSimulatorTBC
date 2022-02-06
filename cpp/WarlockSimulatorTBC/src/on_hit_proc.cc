#include "../include/on_hit_proc.h"

#include "../include/player.h"

OnHitProc::OnHitProc(Entity& entity, std::shared_ptr<Aura> aura) : SpellProc(entity, aura) { procs_on_hit = true; }

void OnHitProc::Setup() {
  SpellProc::Setup();

  if (procs_on_hit && on_hit_procs_enabled) {
    entity.on_hit_procs.push_back(this);
  }
}

MarkOfDefiance::MarkOfDefiance(Entity& entity) : OnHitProc(entity) {
  name = SpellName::kMarkOfDefiance;
  cooldown = 17;
  proc_chance = 15;
  is_item = true;
  gain_mana_on_cast = true;
  min_mana_gain = 128;
  max_mana_gain = 172;
  Setup();
}

InsightfulEarthstormDiamond::InsightfulEarthstormDiamond(Entity& entity) : OnHitProc(entity) {
  name = SpellName::kInsightfulEarthstormDiamond;
  cooldown = 15;
  proc_chance = 5;
  is_item = true;
  gain_mana_on_cast = true;
  mana_gain = 300;
  Setup();
}

BladeOfWizardry::BladeOfWizardry(Entity& entity, std::shared_ptr<Aura> aura) : OnHitProc(entity, aura) {
  name = SpellName::kBladeOfWizardry;
  cooldown = 50;
  proc_chance = 15;
  is_item = true;
  Setup();
}

RobeOfTheElderScribes::RobeOfTheElderScribes(Entity& entity, std::shared_ptr<Aura> aura) : OnHitProc(entity, aura) {
  name = SpellName::kRobeOfTheElderScribes;
  cooldown = 50;
  proc_chance = 20;
  is_item = true;
  Setup();
}

QuagmirransEye::QuagmirransEye(Entity& entity, std::shared_ptr<Aura> aura) : OnHitProc(entity, aura) {
  name = SpellName::kQuagmirransEye;
  cooldown = 45;
  proc_chance = 10;
  is_item = true;
  Setup();
}

BandOfTheEternalSage::BandOfTheEternalSage(Entity& entity, std::shared_ptr<Aura> aura) : OnHitProc(entity, aura) {
  name = SpellName::kBandOfTheEternalSage;
  cooldown = 60;
  proc_chance = 10;
  is_item = true;
  Setup();
}

MysticalSkyfireDiamond::MysticalSkyfireDiamond(Entity& entity, std::shared_ptr<Aura> aura) : OnHitProc(entity, aura) {
  name = SpellName::kMysticalSkyfireDiamond;
  cooldown = 35;
  proc_chance = 15;
  is_item = true;
  Setup();
}

JudgementOfWisdom::JudgementOfWisdom(Entity& entity) : OnHitProc(entity) {
  name = SpellName::kJudgementOfWisdom;
  mana_gain = 74;
  gain_mana_on_cast = true;
  proc_chance = 50;
  Setup();
}

Flameshadow::Flameshadow(Entity& entity, std::shared_ptr<Aura> aura) : OnHitProc(entity, aura) {
  name = SpellName::kFlameshadow;
  proc_chance = 5;
  on_hit_procs_enabled = entity.player->sets.t4 >= 2;
  Setup();
}

bool Flameshadow::ShouldProc(Spell* spell) { return spell->spell_school == SpellSchool::kShadow; }

Shadowflame::Shadowflame(Entity& entity, std::shared_ptr<Aura> aura) : OnHitProc(entity, aura) {
  name = SpellName::kShadowflame;
  proc_chance = 5;
  on_hit_procs_enabled = entity.player->sets.t4 >= 2;
  Setup();
}

bool Shadowflame::ShouldProc(Spell* spell) { return spell->spell_school == SpellSchool::kFire; }

Spellstrike::Spellstrike(Entity& entity, std::shared_ptr<Aura> aura) : OnHitProc(entity, aura) {
  name = SpellName::kSpellstrike;
  proc_chance = 5;
  on_hit_procs_enabled = entity.player->sets.spellstrike == 2;
  Setup();
}

ManaEtched4Set::ManaEtched4Set(Entity& entity, std::shared_ptr<Aura> aura) : OnHitProc(entity, aura) {
  name = SpellName::kManaEtched4Set;
  proc_chance = 2;
  on_hit_procs_enabled = entity.player->sets.mana_etched >= 4;
  Setup();
}

WrathOfCenarius::WrathOfCenarius(Entity& entity, std::shared_ptr<Aura> aura) : OnHitProc(entity, aura) {
  name = SpellName::kWrathOfCenarius;
  proc_chance = 5;
  Setup();
}

DarkmoonCardCrusade::DarkmoonCardCrusade(Entity& entity, std::shared_ptr<Aura> aura) : OnHitProc(entity, aura) {
  name = SpellName::kDarkmoonCardCrusade;
  proc_chance = 100;
  Setup();
}

DemonicFrenzy::DemonicFrenzy(Entity& entity, std::shared_ptr<Aura> aura) : OnHitProc(entity, aura) {
  name = SpellName::kDemonicFrenzy;
  proc_chance = 100;
  Setup();
}