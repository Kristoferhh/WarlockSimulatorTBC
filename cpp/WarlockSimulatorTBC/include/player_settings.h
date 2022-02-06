#pragma once

#include "aura_selection.h"
#include "embind_constant.h"
#include "items.h"
#include "sets.h"
#include "talents.h"
#include <vector>

struct PlayerSettings {
  AuraSelection& auras;
  Talents& talents;
  Sets& sets;
  CharacterStats stats;
  Items& items;
  EmbindConstant custom_stat = EmbindConstant::kUnused;
  EmbindConstant shattrath_faction = EmbindConstant::kUnused;
  EmbindConstant selected_pet = EmbindConstant::kUnused;
  EmbindConstant fight_type = EmbindConstant::kUnused;
  EmbindConstant race = EmbindConstant::kUnused;
  EmbindConstant lash_of_pain_usage = EmbindConstant::kUnused;
  EmbindConstant pet_mode = EmbindConstant::kUnused;
  EmbindConstant rotation_option = EmbindConstant::kUnused;
  std::vector<uint32_t> random_seeds;
  int item_id = 0;
  int meta_gem_id = 0;
  bool equipped_item_simulation = false;
  bool recording_combat_log_breakdown = false;
  int enemy_level = 0;
  int enemy_shadow_resist = 0;
  int enemy_fire_resist = 0;
  int mage_atiesh_amount = 0;
  int totem_of_wrath_amount = 0;
  bool sacrificing_pet = false;
  int ferocious_inspiration_amount = 0;
  int improved_curse_of_the_elements = 0;
  bool using_custom_isb_uptime = false;
  int custom_isb_uptime_value = 0;
  int improved_divine_spirit = 0;
  int improved_imp = 0;
  int shadow_priest_dps = 0;
  int warlock_atiesh_amount = 0;
  int improved_expose_armor = 0;
  int enemy_amount = 0;
  int power_infusion_amount = 0;
  int bloodlust_amount = 0;
  int innervate_amount = 0;
  int chipped_power_core_amount = 0;
  int cracked_power_core_amount = 0;
  int battle_squawk_amount = 0;
  int enemy_armor = 0;
  int expose_weakness_uptime = 0;
  bool improved_faerie_fire = false;
  bool infinite_player_mana = false;
  bool infinite_pet_mana = false;
  bool prepop_black_book = false;
  bool randomize_values = false;
  bool exalted_with_shattrath_faction = false;
  int survival_hunter_agility = 0;
  bool has_immolate = false;
  bool has_corruption = false;
  bool has_siphon_life = false;
  bool has_unstable_affliction = false;
  bool has_searing_pain = false;
  bool has_shadow_bolt = false;
  bool has_incinerate = false;
  bool has_curse_of_recklessness = false;
  bool has_curse_of_the_elements = false;
  bool has_curse_of_agony = false;
  bool has_curse_of_doom = false;
  bool has_death_coil = false;
  bool has_shadow_burn = false;
  bool has_conflagrate = false;
  bool has_shadowfury = false;
  bool has_amplify_curse = false;
  bool has_dark_pact = false;
  bool has_elemental_shaman_t4_bonus = false;

  PlayerSettings(AuraSelection& auras, Talents& talents, Sets& sets, CharacterStats& stats, Items& items)
      : auras(auras), talents(talents), sets(sets), stats(stats), items(items) {}
};