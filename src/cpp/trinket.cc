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
    player.combat_log_breakdown.insert(std::make_pair(name, new CombatLogBreakdown(name)));
  }
}

void Trinket::Use() {
  bool recalculating_pet_stats = false;
  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog(name + " used");
  }
  if (player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->applied_at = player.fight_time;
    player.combat_log_breakdown.at(name)->count++;
  }

  if (duration > 0) {
    if (stats->spell_power > 0) {
      if (player.ShouldWriteToCombatLog()) {
        int kCurrentSpellPower = player.GetSpellPower();
        player.CombatLog("Spell Power + " + std::to_string(stats->spell_power) + " (" +
                         std::to_string(kCurrentSpellPower) + " -> " +
                         std::to_string(kCurrentSpellPower + stats->spell_power) + ")");
      }
      player.stats.spell_power += stats->spell_power;
      recalculating_pet_stats = true;
    }
    if (stats->haste_rating > 0) {
      if (player.ShouldWriteToCombatLog()) {
        const int kCurrentHasteRating = player.stats.haste_rating;
        player.CombatLog("Haste Rating + " + std::to_string(stats->haste_rating) + " (" +
                         std::to_string(kCurrentHasteRating) + " -> " +
                         std::to_string(kCurrentHasteRating + stats->haste_rating) + ")");
      }
      player.stats.haste_rating += stats->haste_rating;
    }
  }

  if (recalculating_pet_stats && player.pet != NULL) {
    player.pet->CalculateStatsFromPlayer();
  }

  active = true;
  duration_remaining = duration;
  cooldown_remaining = cooldown;
}

void Trinket::Fade() {
  bool recalculating_pet_stats = false;
  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog(name + " faded");
  }

  if (stats->spell_power > 0) {
    if (player.ShouldWriteToCombatLog()) {
      const int kCurrentSpellPower = player.GetSpellPower();
      player.CombatLog("Spell Power - " + std::to_string(stats->spell_power) + " (" +
                       std::to_string(kCurrentSpellPower) + " -> " +
                       std::to_string(kCurrentSpellPower - stats->spell_power) + ")");
    }
    player.stats.spell_power -= stats->spell_power;
    recalculating_pet_stats = true;
  }
  if (stats->haste_rating > 0) {
    if (player.ShouldWriteToCombatLog()) {
      int kCurrentHasteRating = player.stats.haste_rating;
      player.CombatLog("Haste Rating - " + std::to_string(stats->haste_rating) + " (" +
                       std::to_string(kCurrentHasteRating) + " -> " +
                       std::to_string(kCurrentHasteRating - stats->haste_rating) + ")");
    }
    player.stats.haste_rating -= stats->haste_rating;
  }

  if (recalculating_pet_stats && player.pet != NULL) {
    player.pet->CalculateStatsFromPlayer();
  }
  if (player.recording_combat_log_breakdown) {
    player.combat_log_breakdown.at(name)->uptime +=
        player.fight_time - player.combat_log_breakdown.at(name)->applied_at;
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
  Trinket::stats = std::make_unique<AuraStats>(130, 0, 0, 0, 0, 0, 0);
  Setup();
}

ShiftingNaaruSliver::ShiftingNaaruSliver(Player& player) : Trinket(player) {
  name = "Shifting Naaru Sliver";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::make_unique<AuraStats>(320, 0, 0, 0, 0, 0, 0);
  Setup();
}

SkullOfGuldan::SkullOfGuldan(Player& player) : Trinket(player) {
  name = "Skull of Gul'dan";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(0, 0, 0, 175, 0, 0, 0);
  Setup();
}

HexShrunkenHead::HexShrunkenHead(Player& player) : Trinket(player) {
  name = "Hex Shrunken Head";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(211, 0, 0, 0, 0, 0, 0);
  Setup();
}

IconOfTheSilverCrescent::IconOfTheSilverCrescent(Player& player) : Trinket(player) {
  name = "Icon of the Silver Crescent";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(155, 0, 0, 0, 0, 0, 0);
  Setup();
}

ScryersBloodgem::ScryersBloodgem(Player& player) : Trinket(player) {
  name = "Scryer's Bloodgem";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::make_unique<AuraStats>(150, 0, 0, 0, 0, 0, 0);
  Setup();
}

AncientCrystalTalisman::AncientCrystalTalisman(Player& player) : Trinket(player) {
  name = "Ancient Crystal Talisman";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(104, 0, 0, 0, 0, 0, 0);
  Setup();
}

ArcanistsStone::ArcanistsStone(Player& player) : Trinket(player) {
  name = "Arcanist's Stone";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(167, 0, 0, 0, 0, 0, 0);
  Setup();
}

TerokkarTabletOfVim::TerokkarTabletOfVim(Player& player) : Trinket(player) {
  name = "Terokkar Table of Vim";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::make_unique<AuraStats>(84, 0, 0, 0, 0, 0, 0);
  Setup();
}

XirisGift::XirisGift(Player& player) : Trinket(player) {
  name = "Xi'ri's Gift";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::make_unique<AuraStats>(150, 0, 0, 0, 0, 0, 0);
  Setup();
}

VengeanceOfTheIllidari::VengeanceOfTheIllidari(Player& player) : Trinket(player) {
  name = "Vengeance of the Illidari";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::make_unique<AuraStats>(120, 0, 0, 0, 0, 0, 0);
  Setup();
}

FigurineLivingRubySerpent::FigurineLivingRubySerpent(Player& player) : Trinket(player) {
  name = "Figurine: Living Ruby Serpent";
  cooldown = 300;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(150, 0, 0, 0, 0, 0, 0);
  Setup();
}

EssenceOfTheMartyr::EssenceOfTheMartyr(Player& player) : Trinket(player) {
  name = "Essence of the Martyr";
  cooldown = 120;
  duration = 20;
  shares_cooldown = false;
  Trinket::stats = std::make_unique<AuraStats>(99, 0, 0, 0, 0, 0, 0);
  Setup();
}

StarkillersBauble::StarkillersBauble(Player& player) : Trinket(player) {
  name = "Starkiller's Bauble";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::make_unique<AuraStats>(125, 0, 0, 0, 0, 0, 0);
  Setup();
}

DarkIronSmokingPipe::DarkIronSmokingPipe(Player& player) : Trinket(player) {
  name = "Dark Iron Smoking Pipe";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(155, 0, 0, 0, 0, 0, 0);
  Setup();
}