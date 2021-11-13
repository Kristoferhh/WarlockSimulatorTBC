#ifndef WARLOCK_SIMULATOR_TBC_CHARACTER_STATS
#define WARLOCK_SIMULATOR_TBC_CHARACTER_STATS

struct CharacterStats {
  int health;
  int mana;
  int maxMana;
  double stamina;
  double intellect;
  double spirit;
  double spellPower;
  int shadowPower;
  int firePower;
  int hasteRating;
  int hitRating;
  int critRating;
  double critChance;
  double hitChance;
  double extraHitChance;
  int mp5;
  int spellPen;
  double fireModifier;
  double frostModifier;
  double hastePercent;
  double shadowModifier;
  double staminaModifier;
  double intellectModifier;
  double spiritModifier;
  double manaCostModifier;
  double arcaneModifier;
  double natureModifier;
  int natureResist;
  int arcaneResist;
  int fireResist;
  int frostResist;
  int shadowResist;
};

#endif