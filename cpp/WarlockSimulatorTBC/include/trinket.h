#pragma once
#include <string>
#include <vector>

struct Stat;
struct Player;

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

  explicit Trinket(Player& player);
  [[nodiscard]] bool Ready() const;
  void Reset();
  void Setup();
  void Use();
  void Fade();
  void Tick(double kTime);
};

struct RestrainedEssenceOfSapphiron : Trinket {
  explicit RestrainedEssenceOfSapphiron(Player& player);
};

struct ShiftingNaaruSliver : Trinket {
  explicit ShiftingNaaruSliver(Player& player);
};

struct SkullOfGuldan : Trinket {
  explicit SkullOfGuldan(Player& player);
};

struct HexShrunkenHead : Trinket {
  explicit HexShrunkenHead(Player& player);
};

struct IconOfTheSilverCrescent : Trinket {
  explicit IconOfTheSilverCrescent(Player& player);
};

struct ScryersBloodgem : Trinket {
  explicit ScryersBloodgem(Player& player);
};

struct AncientCrystalTalisman : Trinket {
  explicit AncientCrystalTalisman(Player& player);
};

struct ArcanistsStone : Trinket {
  explicit ArcanistsStone(Player& player);
};

struct TerokkarTabletOfVim : Trinket {
  explicit TerokkarTabletOfVim(Player& player);
};

struct XirisGift : Trinket {
  explicit XirisGift(Player& player);
};

struct VengeanceOfTheIllidari : Trinket {
  explicit VengeanceOfTheIllidari(Player& player);
};

struct FigurineLivingRubySerpent : Trinket {
  explicit FigurineLivingRubySerpent(Player& player);
};

struct EssenceOfTheMartyr : Trinket {
  explicit EssenceOfTheMartyr(Player& player);
};

struct StarkillersBauble : Trinket {
  explicit StarkillersBauble(Player& player);
};

struct DarkIronSmokingPipe : Trinket {
  explicit DarkIronSmokingPipe(Player& player);
};

struct HazzarahsCharmOfDestruction : Trinket {
  explicit HazzarahsCharmOfDestruction(Player& player);
};
