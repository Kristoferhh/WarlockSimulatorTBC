#include "trinket.h"

#include "common.h"
#include "player.h"

Trinket::Trinket(Player& player) : player(player) {
  cooldown = 0;
  cooldown_remaining = 0;
  duration = 0;
  duration_remaining = 0;
  active = false;
  shares_cooldown = true;
}

bool Trinket::Ready() { return cooldown_remaining <= 0; }

void Trinket::Reset() { cooldown_remaining = 0; }

void Trinket::Setup() {
  if (player.recording_combat_log_breakdown && player.combat_log_breakdown.count(name) == 0) {
    player.combat_log_breakdown.insert({name, std::make_unique<CombatLogBreakdown>(name)});
  }
}

void Trinket::Use() {
  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog(name + " used");
  }

  if (player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->applied_at = player.fight_time;
    player.combat_log_breakdown.at(name)->count++;
  }

  for (auto& stat : stats) {
    stat.AddStat(player);
  }

  active = true;
  duration_remaining = duration;
  cooldown_remaining = cooldown;
}

void Trinket::Fade() {
  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog(name + " faded");
  }

  if (player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->uptime +=
        player.fight_time - player.combat_log_breakdown.at(name)->applied_at;
  }

  for (auto& stat : stats) {
    stat.RemoveStat(player);
  }

  active = false;
}

void Trinket::Tick(double t) {
  if (player.ShouldWriteToCombatLog() && cooldown_remaining > 0 && cooldown_remaining - t <= 0) {
    player.CombatLog(name + " off cooldown");
  }
  cooldown_remaining -= t;
  duration_remaining -= t;
  if (active && duration_remaining <= 0) {
    Fade();
  }
}

RestrainedEssenceOfSapphiron::RestrainedEssenceOfSapphiron(Player& player) : Trinket(player) {
  name = "Restrained Essence of Sapphiron";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::vector<Stat>{SpellPower(130)};
  Setup();
}

ShiftingNaaruSliver::ShiftingNaaruSliver(Player& player) : Trinket(player) {
  name = "Shifting Naaru Sliver";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::vector<Stat>{SpellPower(320)};
  Setup();
}

SkullOfGuldan::SkullOfGuldan(Player& player) : Trinket(player) {
  name = "Skull of Gul'dan";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::vector<Stat>{HasteRating(175)};
  Setup();
}

HexShrunkenHead::HexShrunkenHead(Player& player) : Trinket(player) {
  name = "Hex Shrunken Head";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::vector<Stat>{SpellPower(211)};
  Setup();
}

IconOfTheSilverCrescent::IconOfTheSilverCrescent(Player& player) : Trinket(player) {
  name = "Icon of the Silver Crescent";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::vector<Stat>{SpellPower(155)};
  Setup();
}

ScryersBloodgem::ScryersBloodgem(Player& player) : Trinket(player) {
  name = "Scryer's Bloodgem";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::vector<Stat>{SpellPower(150)};
  Setup();
}

AncientCrystalTalisman::AncientCrystalTalisman(Player& player) : Trinket(player) {
  name = "Ancient Crystal Talisman";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::vector<Stat>{SpellPower(104)};
  Setup();
}

ArcanistsStone::ArcanistsStone(Player& player) : Trinket(player) {
  name = "Arcanist's Stone";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::vector<Stat>{SpellPower(167)};
  Setup();
}

TerokkarTabletOfVim::TerokkarTabletOfVim(Player& player) : Trinket(player) {
  name = "Terokkar Table of Vim";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::vector<Stat>{SpellPower(84)};
  Setup();
}

XirisGift::XirisGift(Player& player) : Trinket(player) {
  name = "Xi'ri's Gift";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::vector<Stat>{SpellPower(150)};
  Setup();
}

VengeanceOfTheIllidari::VengeanceOfTheIllidari(Player& player) : Trinket(player) {
  name = "Vengeance of the Illidari";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::vector<Stat>{SpellPower(120)};
  Setup();
}

FigurineLivingRubySerpent::FigurineLivingRubySerpent(Player& player) : Trinket(player) {
  name = "Figurine: Living Ruby Serpent";
  cooldown = 300;
  duration = 20;
  Trinket::stats = std::vector<Stat>{SpellPower(150)};
  Setup();
}

EssenceOfTheMartyr::EssenceOfTheMartyr(Player& player) : Trinket(player) {
  name = "Essence of the Martyr";
  cooldown = 120;
  duration = 20;
  shares_cooldown = false;
  Trinket::stats = std::vector<Stat>{SpellPower(99)};
  Setup();
}

StarkillersBauble::StarkillersBauble(Player& player) : Trinket(player) {
  name = "Starkiller's Bauble";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::vector<Stat>{SpellPower(125)};
  Setup();
}

DarkIronSmokingPipe::DarkIronSmokingPipe(Player& player) : Trinket(player) {
  name = "Dark Iron Smoking Pipe";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::vector<Stat>{SpellPower(155)};
  Setup();
}

HazzarahsCharmOfDestruction::HazzarahsCharmOfDestruction(Player& player) : Trinket(player) {
  name = "Hazza'rah's Charm of Destruction";
  cooldown = 180;
  duration = 20;
  Trinket::stats = std::vector<Stat>{CritRating(140)};
  Setup();
}