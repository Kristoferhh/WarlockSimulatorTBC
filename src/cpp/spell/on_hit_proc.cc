#include "on_hit_proc.h"

#include "../player/player.h"

OnHitProc::OnHitProc(Player& player, std::shared_ptr<Aura> aura) : SpellProc(player, aura) { procs_on_hit = true; }

void OnHitProc::Setup() {
  SpellProc::Setup();

  if (procs_on_hit && on_hit_procs_enabled) {
    player.on_hit_procs.push_back(this);
  }
}

MarkOfDefiance::MarkOfDefiance(Player& player) : OnHitProc(player) {
  name = SpellName::kMarkOfDefiance;
  cooldown = 17;
  proc_chance = 15;
  is_item = true;
  gain_mana_on_cast = true;
  min_mana_gain = 128;
  max_mana_gain = 172;
  Setup();
}

InsightfulEarthstormDiamond::InsightfulEarthstormDiamond(Player& player) : OnHitProc(player) {
  name = SpellName::kInsightfulEarthstormDiamond;
  cooldown = 15;
  proc_chance = 5;
  is_item = true;
  gain_mana_on_cast = true;
  mana_gain = 300;
  Setup();
}

BladeOfWizardry::BladeOfWizardry(Player& player, std::shared_ptr<Aura> aura) : OnHitProc(player, aura) {
  name = SpellName::kBladeOfWizardry;
  cooldown = 50;
  proc_chance = 15;
  is_item = true;
  Setup();
}

RobeOfTheElderScribes::RobeOfTheElderScribes(Player& player, std::shared_ptr<Aura> aura) : OnHitProc(player, aura) {
  name = SpellName::kRobeOfTheElderScribes;
  cooldown = 50;
  proc_chance = 20;
  is_item = true;
  Setup();
}

QuagmirransEye::QuagmirransEye(Player& player, std::shared_ptr<Aura> aura) : OnHitProc(player, aura) {
  name = SpellName::kQuagmirransEye;
  cooldown = 45;
  proc_chance = 10;
  is_item = true;
  Setup();
}

BandOfTheEternalSage::BandOfTheEternalSage(Player& player, std::shared_ptr<Aura> aura) : OnHitProc(player, aura) {
  name = SpellName::kBandOfTheEternalSage;
  cooldown = 60;
  proc_chance = 10;
  is_item = true;
  Setup();
}

MysticalSkyfireDiamond::MysticalSkyfireDiamond(Player& player, std::shared_ptr<Aura> aura) : OnHitProc(player, aura) {
  name = SpellName::kMysticalSkyfireDiamond;
  cooldown = 35;
  proc_chance = 15;
  is_item = true;
  Setup();
}

JudgementOfWisdom::JudgementOfWisdom(Player& player) : OnHitProc(player) {
  name = SpellName::kJudgementOfWisdom;
  mana_gain = 74;
  gain_mana_on_cast = true;
  proc_chance = 50;
  Setup();
}

Flameshadow::Flameshadow(Player& player, std::shared_ptr<Aura> aura) : OnHitProc(player, aura) {
  name = SpellName::kFlameshadow;
  proc_chance = 5;
  on_hit_procs_enabled = player.sets.t4 >= 2;
  Setup();
}

bool Flameshadow::ShouldProc(Spell* spell) { return spell->school == SpellSchool::kShadow; }

Shadowflame::Shadowflame(Player& player, std::shared_ptr<Aura> aura) : OnHitProc(player, aura) {
  name = SpellName::kShadowflame;
  proc_chance = 5;
  on_hit_procs_enabled = player.sets.t4 >= 2;
  Setup();
}

bool Shadowflame::ShouldProc(Spell* spell) { return spell->school == SpellSchool::kFire; }

Spellstrike::Spellstrike(Player& player, std::shared_ptr<Aura> aura) : OnHitProc(player, aura) {
  name = SpellName::kSpellstrike;
  proc_chance = 5;
  on_hit_procs_enabled = player.sets.spellstrike == 2;
  Setup();
}

ManaEtched4Set::ManaEtched4Set(Player& player, std::shared_ptr<Aura> aura) : OnHitProc(player, aura) {
  name = SpellName::kManaEtched4Set;
  proc_chance = 2;
  on_hit_procs_enabled = player.sets.mana_etched >= 4;
  Setup();
}

WrathOfCenarius::WrathOfCenarius(Player& player, std::shared_ptr<Aura> aura) : OnHitProc(player, aura) {
  name = SpellName::kWrathOfCenarius;
  proc_chance = 5;
  Setup();
}

DarkmoonCardCrusade::DarkmoonCardCrusade(Player& player, std::shared_ptr<Aura> aura) : OnHitProc(player, aura) {
  name = SpellName::kDarkmoonCardCrusade;
  proc_chance = 100;
  Setup();
}