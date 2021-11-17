#ifndef WARLOCK_SIMULATOR_TBC_PET_STATS
#define WARLOCK_SIMULATOR_TBC_PET_STATS

struct Player;

struct PetStats {
  Player& player;
  int stamina;
  int intellect;
  int strength;
  int agility;
  int max_mana;
  int attack_power;
  int spell_power;
  int mana;
  int mp5;
  int spirit;
  double melee_crit_chance;
  double melee_hit_chance;
  double spell_crit_chance;
  double spell_hit_chance;
  double haste_percent;
  double stamina_modifier;
  double intellect_modifier;
  double strength_modifier;
  double agility_modifier;
  double spirit_modifier;
  double attack_power_modifier;
  double damage_modifier;
  double melee_haste_percent;

  PetStats(Player& player)
      : player(player),
        damage_modifier(1),
        stamina_modifier(1),
        intellect_modifier(1),
        strength_modifier(1),
        agility_modifier(1),
        spirit_modifier(1),
        attack_power_modifier(1),
        haste_percent(1),
        melee_haste_percent(1) {}
};

#endif