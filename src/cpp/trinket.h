#ifndef WARLOCK_SIMULATOR_TBC_TRINKET
#define WARLOCK_SIMULATOR_TBC_TRINKET

struct Player;
#include <iostream>

#include "aura_stats.h"

struct Trinket {
  std::shared_ptr<Player> player;
  std::unique_ptr<AuraStats> stats;
  int duration;
  double durationRemaining;
  int cooldown;
  double cooldownRemaining;
  bool active;
  bool sharesCooldown;
  std::string name;

  Trinket(std::shared_ptr<Player> player);
  bool ready();
  void reset();
  void setup();
  void use();
  void fade();
  void tick(double time);
};

struct RestrainedEssenceOfSapphiron : public Trinket {
  RestrainedEssenceOfSapphiron(std::shared_ptr<Player> player);
};

struct ShiftingNaaruSliver : public Trinket {
  ShiftingNaaruSliver(std::shared_ptr<Player> player);
};

struct SkullOfGuldan : public Trinket {
  SkullOfGuldan(std::shared_ptr<Player> player);
};

struct HexShrunkenHead : public Trinket {
  HexShrunkenHead(std::shared_ptr<Player> player);
};

struct IconOfTheSilverCrescent : public Trinket {
  IconOfTheSilverCrescent(std::shared_ptr<Player> player);
};

struct ScryersBloodgem : public Trinket {
  ScryersBloodgem(std::shared_ptr<Player> player);
};

struct AncientCrystalTalisman : public Trinket {
  AncientCrystalTalisman(std::shared_ptr<Player> player);
};

struct ArcanistsStone : public Trinket {
  ArcanistsStone(std::shared_ptr<Player> player);
};

struct TerokkarTabletOfVim : public Trinket {
  TerokkarTabletOfVim(std::shared_ptr<Player> player);
};

struct XirisGift : public Trinket {
  XirisGift(std::shared_ptr<Player> player);
};

struct VengeanceOfTheIllidari : public Trinket {
  VengeanceOfTheIllidari(std::shared_ptr<Player> player);
};

struct FigurineLivingRubySerpent : public Trinket {
  FigurineLivingRubySerpent(std::shared_ptr<Player> player);
};

struct EssenceOfTheMartyr : public Trinket {
  EssenceOfTheMartyr(std::shared_ptr<Player> player);
};

struct StarkillersBauble : public Trinket {
  StarkillersBauble(std::shared_ptr<Player> player);
};

struct DarkIronSmokingPipe : public Trinket {
  DarkIronSmokingPipe(std::shared_ptr<Player> player);
};

#endif