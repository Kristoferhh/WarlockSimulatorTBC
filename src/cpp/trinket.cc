#include "trinket.h"

#include "common.h"
#include "player.h"

Trinket::Trinket(std::shared_ptr<Player> player) : player(player) {
  cooldown = 0;
  cooldownRemaining = 0;
  duration = 0;
  durationRemaining = 0;
  active = false;
  sharesCooldown = true;
}

bool Trinket::ready() { return cooldownRemaining <= 0; }

void Trinket::reset() { cooldownRemaining = 0; }

void Trinket::setup() {
  if (player->combatLogBreakdown.count(name) == 0) {
    player->combatLogBreakdown.insert(
        std::make_pair(name, new CombatLogBreakdown(name)));
  }
}

void Trinket::use() {
  bool recalculatePetStats = false;
  if (player->shouldWriteToCombatLog()) {
    player->combatLog(name + " used");
  }
  player->combatLogBreakdown.at(name)->appliedAt = player->fightTime;
  player->combatLogBreakdown.at(name)->count++;

  if (duration > 0) {
    if (stats->spellPower > 0) {
      if (player->shouldWriteToCombatLog()) {
        int currentSpellPower = player->getSpellPower();
        player->combatLog(
            "Spell Power + " + std::to_string(stats->spellPower) + " (" +
            std::to_string(currentSpellPower) + " -> " +
            std::to_string(currentSpellPower + stats->spellPower) + ")");
      }
      player->stats->spellPower += stats->spellPower;
      recalculatePetStats = true;
    }
    if (stats->hasteRating > 0) {
      if (player->shouldWriteToCombatLog()) {
        int currentHasteRating = player->stats->hasteRating;
        player->combatLog(
            "Haste Rating + " + std::to_string(stats->hasteRating) + " (" +
            std::to_string(currentHasteRating) + " -> " +
            std::to_string(currentHasteRating + stats->hasteRating) + ")");
      }
      player->stats->hasteRating += stats->hasteRating;
    }
  }

  if (recalculatePetStats && player->pet != NULL) {
    player->pet->calculateStatsFromPlayer();
  }

  active = true;
  durationRemaining = duration;
  cooldownRemaining = cooldown;
}

void Trinket::fade() {
  bool recalculatePetStats = false;
  if (player->shouldWriteToCombatLog()) {
    player->combatLog(name + " faded");
  }

  if (stats->spellPower > 0) {
    if (player->shouldWriteToCombatLog()) {
      int currentSpellPower = player->getSpellPower();
      player->combatLog("Spell Power - " + std::to_string(stats->spellPower) +
                        " (" + std::to_string(currentSpellPower) + " -> " +
                        std::to_string(currentSpellPower - stats->spellPower) +
                        ")");
    }
    player->stats->spellPower -= stats->spellPower;
    recalculatePetStats = true;
  }
  if (stats->hasteRating > 0) {
    if (player->shouldWriteToCombatLog()) {
      int currentHasteRating = player->stats->hasteRating;
      player->combatLog(
          "Haste Rating - " + std::to_string(stats->hasteRating) + " (" +
          std::to_string(currentHasteRating) + " -> " +
          std::to_string(currentHasteRating - stats->hasteRating) + ")");
    }
    player->stats->hasteRating -= stats->hasteRating;
  }

  if (recalculatePetStats && player->pet != NULL) {
    player->pet->calculateStatsFromPlayer();
  }

  active = false;
  double auraUptime =
      player->fightTime - player->combatLogBreakdown.at(name)->appliedAt;
  player->combatLogBreakdown.at(name)->uptime += auraUptime;
}

void Trinket::tick(double t) {
  if (player->shouldWriteToCombatLog() && cooldownRemaining > 0 &&
      cooldownRemaining - t <= 0) {
    player->combatLog(name + " off cooldown");
  }
  cooldownRemaining -= t;
  durationRemaining -= t;
  if (active && durationRemaining <= 0) {
    fade();
  }
}

RestrainedEssenceOfSapphiron::RestrainedEssenceOfSapphiron(
    std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Restrained Essence of Sapphiron";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(130, 0, 0, 0, 0, 0);
  setup();
}

ShiftingNaaruSliver::ShiftingNaaruSliver(std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Shifting Naaru Sliver";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::make_unique<AuraStats>(320, 0, 0, 0, 0, 0);
  setup();
}

SkullOfGuldan::SkullOfGuldan(std::shared_ptr<Player> player) : Trinket(player) {
  name = "Skull of Gul'dan";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(0, 0, 0, 175, 0, 0);
  setup();
}

HexShrunkenHead::HexShrunkenHead(std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Hex Shrunken Head";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(211, 0, 0, 0, 0, 0);
  setup();
}

IconOfTheSilverCrescent::IconOfTheSilverCrescent(std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Icon of the Silver Crescent";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(155, 0, 0, 0, 0, 0);
  setup();
}

ScryersBloodgem::ScryersBloodgem(std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Scryer's Bloodgem";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::make_unique<AuraStats>(150, 0, 0, 0, 0, 0);
  setup();
}

AncientCrystalTalisman::AncientCrystalTalisman(std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Ancient Crystal Talisman";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(104, 0, 0, 0, 0, 0);
  setup();
}

ArcanistsStone::ArcanistsStone(std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Arcanist's Stone";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(167, 0, 0, 0, 0, 0);
  setup();
}

TerokkarTabletOfVim::TerokkarTabletOfVim(std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Terokkar Table of Vim";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::make_unique<AuraStats>(84, 0, 0, 0, 0, 0);
  setup();
}

XirisGift::XirisGift(std::shared_ptr<Player> player) : Trinket(player) {
  name = "Xi'ri's Gift";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::make_unique<AuraStats>(150, 0, 0, 0, 0, 0);
  setup();
}

VengeanceOfTheIllidari::VengeanceOfTheIllidari(std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Vengeance of the Illidari";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::make_unique<AuraStats>(120, 0, 0, 0, 0, 0);
  setup();
}

FigurineLivingRubySerpent::FigurineLivingRubySerpent(
    std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Figurine: Living Ruby Serpent";
  cooldown = 300;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(150, 0, 0, 0, 0, 0);
  setup();
}

EssenceOfTheMartyr::EssenceOfTheMartyr(std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Essence of the Martyr";
  cooldown = 120;
  duration = 20;
  sharesCooldown = false;
  Trinket::stats = std::make_unique<AuraStats>(99, 0, 0, 0, 0, 0);
  setup();
}

StarkillersBauble::StarkillersBauble(std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Starkiller's Bauble";
  cooldown = 90;
  duration = 15;
  Trinket::stats = std::make_unique<AuraStats>(125, 0, 0, 0, 0, 0);
  setup();
}

DarkIronSmokingPipe::DarkIronSmokingPipe(std::shared_ptr<Player> player)
    : Trinket(player) {
  name = "Dark Iron Smoking Pipe";
  cooldown = 120;
  duration = 20;
  Trinket::stats = std::make_unique<AuraStats>(155, 0, 0, 0, 0, 0);
  setup();
}