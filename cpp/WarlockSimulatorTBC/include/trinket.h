#pragma once

struct Player;
#include <iostream>
#include <optional>
#include <vector>

#include "stat.h"

struct Trinket {
  Player& player;
  std::vector<Stat> stats;
  int duration = 0;
  double duration_remaining = 0;
  int cooldown = 0;
  double cooldown_remaining = 0;
  bool active = false;
  bool shares_cooldown = true;
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