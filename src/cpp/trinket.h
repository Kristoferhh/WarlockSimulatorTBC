#ifndef WARLOCK_SIMULATOR_TBC_TRINKET
#define WARLOCK_SIMULATOR_TBC_TRINKET

struct Player;
#include <iostream>
#include <optional>

#include "aura_stats.h"

struct Trinket {
  Player& player;
  std::optional<AuraStats> stats;
  int duration;
  double duration_remaining;
  int cooldown;
  double cooldown_remaining;
  bool active;
  bool shares_cooldown;
  std::string name;

  Trinket(Player& player);
  bool Ready();
  void Reset();
  void Setup();
  void Use();
  void Fade();
  void Tick(double time);
};

struct RestrainedEssenceOfSapphiron : public Trinket {
  RestrainedEssenceOfSapphiron(Player& player);
};

struct ShiftingNaaruSliver : public Trinket {
  ShiftingNaaruSliver(Player& player);
};

struct SkullOfGuldan : public Trinket {
  SkullOfGuldan(Player& player);
};

struct HexShrunkenHead : public Trinket {
  HexShrunkenHead(Player& player);
};

struct IconOfTheSilverCrescent : public Trinket {
  IconOfTheSilverCrescent(Player& player);
};

struct ScryersBloodgem : public Trinket {
  ScryersBloodgem(Player& player);
};

struct AncientCrystalTalisman : public Trinket {
  AncientCrystalTalisman(Player& player);
};

struct ArcanistsStone : public Trinket {
  ArcanistsStone(Player& player);
};

struct TerokkarTabletOfVim : public Trinket {
  TerokkarTabletOfVim(Player& player);
};

struct XirisGift : public Trinket {
  XirisGift(Player& player);
};

struct VengeanceOfTheIllidari : public Trinket {
  VengeanceOfTheIllidari(Player& player);
};

struct FigurineLivingRubySerpent : public Trinket {
  FigurineLivingRubySerpent(Player& player);
};

struct EssenceOfTheMartyr : public Trinket {
  EssenceOfTheMartyr(Player& player);
};

struct StarkillersBauble : public Trinket {
  StarkillersBauble(Player& player);
};

struct DarkIronSmokingPipe : public Trinket {
  DarkIronSmokingPipe(Player& player);
};

struct HazzarahsCharmOfDestruction : public Trinket {
  HazzarahsCharmOfDestruction(Player& player);
};

#endif