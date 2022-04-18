#pragma once

struct CharacterStats {
  double health;
  double mana;
  double max_mana;
  double stamina;
  double intellect;
  double spirit;
  double spell_power;
  double shadow_power;
  double fire_power;
  double spell_haste_rating;
  double spell_hit_rating;
  double spell_crit_rating;
  double spell_crit_chance;
  double spell_hit_chance;
  double extra_spell_hit_chance;
  double mp5;
  double spell_penetration;
  double fire_modifier;
  double spell_haste_percent;
  double shadow_modifier;
  double stamina_modifier;
  double intellect_modifier;
  double spirit_modifier;
  double mana_cost_modifier;
  double strength;
  double agility;
  double attack_power;
  double melee_crit_chance;
  double melee_hit_chance;
  double strength_modifier;
  double agility_modifier;
  double attack_power_modifier;
  double damage_modifier;
  double melee_haste_percent;
  double physical_modifier;

  CharacterStats()
    : health(0),
      mana(0),
      max_mana(0),
      stamina(0),
      intellect(0),
      spirit(0),
      spell_power(0),
      shadow_power(0),
      fire_power(0),
      spell_haste_rating(0),
      spell_hit_rating(0),
      spell_crit_rating(0),
      spell_crit_chance(0),
      spell_hit_chance(0),
      extra_spell_hit_chance(0),
      mp5(0),
      spell_penetration(0),
      fire_modifier(1),
      spell_haste_percent(1),
      shadow_modifier(1),
      stamina_modifier(1),
      intellect_modifier(1),
      spirit_modifier(1),
      mana_cost_modifier(1),
      strength(0),
      agility(0),
      attack_power(0),
      melee_crit_chance(0),
      melee_hit_chance(0),
      strength_modifier(1),
      agility_modifier(1),
      attack_power_modifier(1),
      damage_modifier(1),
      melee_haste_percent(1),
      physical_modifier(1) {
  }
};
