#include <iostream>

#include "../include/aura_selection.h"
#include "../include/bindings.h"
#include "../include/character_stats.h"
#include "../include/items.h"
#include "../include/player.h"
#include "../include/player_settings.h"
#include "../include/sets.h"
#include "../include/simulation.h"
#include "../include/simulation_settings.h"
#include "../include/talents.h"

int main() {
  auto auras = AuraSelection();
  auras.fel_armor = true;
  auras.mana_spring_totem = true;
  auras.wrath_of_air_totem = true;
  auras.totem_of_wrath = true;
  auras.mark_of_the_wild = true;
  auras.prayer_of_spirit = true;
  auras.inspiring_presence = true;
  auras.moonkin_aura = true;
  auras.eye_of_the_night = true;
  auras.chain_of_the_twilight_owl = true;
  auras.drums_of_battle = true;
  auras.bloodlust = true;
  auras.curse_of_the_elements = true;
  auras.shadow_weaving = true;
  auras.misery = true;
  auras.judgement_of_wisdom = true;
  auras.judgement_of_the_crusader = true;
  auras.super_mana_potion = true;
  auras.demonic_rune = true;
  auras.blood_pact = true;
  auras.power_infusion = true;
  auras.atiesh_mage = true;
  auras.atiesh_warlock = true;
  auras.jade_pendant_of_blasting = true;
  auras.ferocious_inspiration = true;
  auras.innervate = true;
  auras.mana_tide_totem = true;
  auras.airmans_ribbon_of_gallantry = true;
  auras.improved_scorch = true;
  auras.vampiric_touch = true;
  auras.faerie_fire = true;
  auras.sunder_armor = true;
  auras.expose_armor = true;
  auras.curse_of_recklessness = true;
  auras.blood_frenzy = true;
  auras.expose_weakness = true;
  auras.annihilator = true;
  auras.improved_hunters_mark = true;
  auras.pet_blessing_of_kings = true;
  auras.pet_blessing_of_wisdom = true;
  auras.pet_blessing_of_might = true;
  auras.pet_arcane_intellect = true;
  auras.pet_mark_of_the_wild = true;
  auras.pet_prayer_of_fortitude = true;
  auras.pet_prayer_of_spirit = true;
  auras.pet_kiblers_bits = true;
  auras.pet_heroic_presence = true;
  auras.pet_strength_of_earth_totem = true;
  auras.pet_grace_of_air_totem = true;
  auras.pet_battle_shout = true;
  auras.pet_trueshot_aura = true;
  auras.pet_leader_of_the_pack = true;
  auras.pet_unleashed_rage = true;
  auras.pet_stamina_scroll = true;
  auras.pet_intellect_scroll = true;
  auras.pet_strength_scroll = true;
  auras.pet_agility_scroll = true;
  auras.pet_spirit_scroll = true;

  auto talents = Talents();
  talents.improved_corruption = 1;
  talents.demonic_embrace = 5;
  talents.fel_intellect = 3;
  talents.improved_succubus = 3;
  talents.fel_stamina = 3;
  talents.demonic_aegis = 3;
  talents.unholy_power = 5;
  talents.demonic_sacrifice = 1;
  talents.mana_feed = 1;
  talents.master_demonologist = 5;
  talents.soul_link = 1;
  talents.demonic_knowledge = 3;
  talents.demonic_tactics = 5;
  talents.felguard = 1;
  talents.improved_shadow_bolt = 5;
  talents.bane = 5;
  talents.devastation = 5;

  auto sets = Sets();
  sets.t6 = 4;

  auto stats = CharacterStats();
  stats.health = 3310;
  stats.mana = 2335;
  stats.stamina = 786;
  stats.intellect = 516;
  stats.spirit = 247;
  stats.spell_power = 1451;
  stats.shadow_power = 134;
  stats.fire_power = 80;
  stats.spell_haste_rating = 227;
  stats.spell_hit_rating = 163;
  stats.spell_crit_rating = 316;
  stats.spell_crit_chance = 0;
  stats.mp5 = 91;
  stats.mana_cost_modifier = 1;
  stats.spell_penetration = 88;
  stats.fire_modifier = 1.2075;
  stats.shadow_modifier = 1.155;
  stats.stamina_modifier = 1.1;
  stats.intellect_modifier = 1.155;
  stats.spirit_modifier = 1.1;

  auto items = Items();
  items.head = 31051;
  items.neck = 32349;
  items.shoulders = 31054;
  items.back = 32331;
  items.chest = 30107;
  items.bracers = 32586;
  items.gloves = 31050;
  items.belt = 32256;
  items.legs = 31053;
  items.boots = 32239;
  items.ring_1 = 32527;
  items.ring_2 = 32527;
  items.trinket_1 = 32483;
  items.trinket_2 = 27683;
  items.two_hand = 32374;
  items.wand = 29982;

  auto iterations = 1000;
  auto player_settings = PlayerSettings(auras, talents, sets, stats, items);
  player_settings.equipped_item_simulation = true;
  player_settings.random_seeds = AllocRandomSeeds(iterations);
  player_settings.shattrath_faction = EmbindConstant::kAldor;
  player_settings.selected_pet = EmbindConstant::kSuccubus;
  player_settings.fight_type = EmbindConstant::kSingleTarget;
  player_settings.race = EmbindConstant::kGnome;
  player_settings.rotation_option = EmbindConstant::kSimChooses;
  player_settings.meta_gem_id = 34220;
  player_settings.recording_combat_log_breakdown = true;
  player_settings.enemy_level = 73;
  player_settings.totem_of_wrath_amount = 1;
  player_settings.sacrificing_pet = false;
  player_settings.improved_curse_of_the_elements = 3;
  player_settings.using_custom_isb_uptime = true;
  player_settings.custom_isb_uptime_value = 70;
  player_settings.improved_divine_spirit = 2;
  player_settings.bloodlust_amount = 1;
  player_settings.infinite_player_mana = false;
  player_settings.exalted_with_shattrath_faction = true;
  player_settings.has_curse_of_doom = true;
  player_settings.prepop_black_book = false;
  player_settings.pet_mode = EmbindConstant::kAggressive;
  player_settings.lash_of_pain_usage = EmbindConstant::kOnCooldown;
  player_settings.enemy_armor = 7700;
  player_settings.power_infusion_amount = 1;
  player_settings.innervate_amount = 1;
  player_settings.mage_atiesh_amount = 1;
  player_settings.warlock_atiesh_amount = 1;
  player_settings.ferocious_inspiration_amount = 1;
  player_settings.shadow_priest_dps = 1000;
  player_settings.battle_squawk_amount = 1;
  player_settings.improved_faerie_fire = true;
  player_settings.improved_expose_armor = 2;
  player_settings.survival_hunter_agility = 800;
  player_settings.expose_weakness_uptime = 70;

  auto player = Player(player_settings);
  auto simulation_settings = SimulationSettings();
  simulation_settings.iterations = iterations;
  simulation_settings.min_time = 150;
  simulation_settings.max_time = 210;
  simulation_settings.simulation_type = SimulationType::kNormal;

  auto simulation = Simulation(player, simulation_settings);
  simulation.Start();
}