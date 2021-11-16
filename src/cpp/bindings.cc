#include "bindings.h"

#include <random>

#include "constant.h"
#include "simulation.h"

void DpsUpdate(double dps) {
#ifdef EMSCRIPTEN
  EM_ASM({postMessage({event : "dpsUpdate", data : {dps : $0}})}, dps);
#endif
}

void ErrorCallback(const char* error_msg) {
#ifdef EMSCRIPTEN
  EM_ASM({postMessage({event : "errorCallback", data : {errorMsg : UTF8ToString($0)}})}, error_msg);
#endif
}

void PostCombatLogBreakdownVector(const char* name, int mana_gain, int damage) {
#ifdef EMSCRIPTEN
  EM_ASM({postMessage({event : "combatLogVector", data : {name : UTF8ToString($0), manaGain : $1, damage : $2}})}, name,
         mana_gain, damage);
#endif
}

void PostCombatLogBreakdown(const char* name, uint32_t casts, uint32_t crits, uint32_t misses, uint32_t count,
                            double uptime, uint32_t dodges, uint32_t glancing_blows) {
#ifdef EMSCRIPTEN
  EM_ASM({postMessage({
           event : "combatLogBreakdown",
           data : {
             name : UTF8ToString($0),
             casts : $1,
             crits : $2,
             misses : $3,
             count : $4,
             uptime : $5,
             dodges : $6,
             glancing_blows : $7,
             damage : 0,
             manaGain : 0
           }
         })},
         name, casts, crits, misses, count, uptime, dodges, glancing_blows);
#endif
}

void CombatLogUpdate(const char* combat_log_entry) {
#ifdef EMSCRIPTEN
  EM_ASM({postMessage({event : "combatLogUpdate", data : {combatLogEntry : UTF8ToString($0)}})}, combat_log_entry);
#endif
}

void SimulationUpdate(int iteration, int iteration_amount, double median_dps, int item_id, const char* custom_stat) {
#ifdef EMSCRIPTEN
  EM_ASM({postMessage({
           event : "update",
           data : {medianDps : $0, iteration : $1, iterationAmount : $2, itemId : $3, customStat : UTF8ToString($4)}
         })},
         median_dps, iteration, iteration_amount, item_id, custom_stat);
#else
  std::cout << "Iteration: " << std::to_string(iteration) << "/" << std::to_string(iteration_amount)
            << ". Median DPS: " << std::to_string(median_dps) << std::endl;
#endif
}

void SimulationEnd(double median_dps, double min_dps, double max_dps, int item_id, int iteration_amount,
                   int total_duration, const char* custom_stat) {
#ifdef EMSCRIPTEN
  EM_ASM({postMessage({
           event : "end",
           data : {
             medianDps : $0,
             minDps : $1,
             maxDps : $2,
             itemId : $3,
             iterationAmount : $4,
             totalDuration : $5,
             customStat : UTF8ToString($6)
           }
         })},
         median_dps, min_dps, max_dps, item_id, iteration_amount, total_duration, custom_stat);
#else
  std::cout << "Median DPS: " << std::to_string(median_dps) << ". Min DPS: " << std::to_string(min_dps)
            << ". Max DPS: " << std::to_string(max_dps) << std::endl;
#endif
}

uint32_t* AllocRandomSeeds(int amount_of_seeds) {
  srand(time(nullptr));
  uint32_t* seeds = new uint32_t[amount_of_seeds];

  for (int i = 0; i < amount_of_seeds; i++) {
    seeds[i] = rand();
  }

  return seeds;
}

Items AllocItems() { return Items(); }

Auras AllocAuras() { return Auras(); }

Talents AllocTalents() { return Talents(); }

Sets AllocSets() { return Sets(); }

CharacterStats AllocStats() { return CharacterStats(); }

PlayerSettings AllocPlayerSettings(Auras& auras, Talents& talents, Sets& sets, CharacterStats& stats, Items& items) {
  return PlayerSettings(auras, talents, sets, stats, items);
}

Player AllocPlayer(PlayerSettings& settings) { return Player(settings); }

SimulationSettings AllocSimSettings() { return SimulationSettings(); }

Simulation AllocSim(Player& player, SimulationSettings& simulation_settings) {
  return Simulation(player, simulation_settings);
}

std::string GetExceptionMessage(intptr_t exception_ptr) {
  return std::string(reinterpret_cast<std::exception*>(exception_ptr)->what());
}

#ifdef EMSCRIPTEN
EMSCRIPTEN_BINDINGS(module) {
  emscripten::class_<Player>("Player").smart_ptr<std::shared_ptr<Player>>("Player");

  emscripten::class_<Simulation>("Simulation")
      .constructor<Player&, SimulationSettings&>()
      .function("start", &Simulation::Start);

  emscripten::class_<Items>("Items")
      .property("head", &Items::head)
      .property("neck", &Items::neck)
      .property("shoulders", &Items::shoulders)
      .property("back", &Items::back)
      .property("chest", &Items::chest)
      .property("bracer", &Items::bracers)
      .property("gloves", &Items::gloves)
      .property("belt", &Items::belt)
      .property("legs", &Items::legs)
      .property("boots", &Items::boots)
      .property("ring1", &Items::ring_1)
      .property("ring2", &Items::ring_2)
      .property("trinket1", &Items::trinket_1)
      .property("trinket2", &Items::trinket_2)
      .property("mainhand", &Items::main_hand)
      .property("offhand", &Items::off_hand)
      .property("twohand", &Items::two_hand)
      .property("wand", &Items::wand);

  emscripten::class_<Auras>("Auras")
      .constructor<>()
      .property("felArmor", &Auras::fel_armor)
      .property("judgementOfWisdom", &Auras::judgement_of_wisdom)
      .property("manaSpringTotem", &Auras::mana_spring_totem)
      .property("wrathOfAirTotem", &Auras::wrath_of_air_totem)
      .property("totemOfWrath", &Auras::totem_of_wrath)
      .property("markOfTheWild", &Auras::mark_of_the_wild)
      .property("prayerOfSpirit", &Auras::prayer_of_spirit)
      .property("bloodPact", &Auras::blood_pact)
      .property("inspiringPresence", &Auras::inspiring_presence)
      .property("moonkinAura", &Auras::moonkin_aura)
      .property("powerInfusion", &Auras::power_infusion)
      .property("powerOfTheGuardianWarlock", &Auras::atiesh_warlock)
      .property("powerOfTheGuardianMage", &Auras::atiesh_mage)
      .property("eyeOfTheNight", &Auras::eye_of_the_night)
      .property("chainOfTheTwilightOwl", &Auras::chain_of_the_twilight_owl)
      .property("jadePendantOfBlasting", &Auras::jade_pendant_of_blasting)
      .property("drumsOfBattle", &Auras::drums_of_battle)
      .property("drumsOfWar", &Auras::drums_of_war)
      .property("drumsOfRestoration", &Auras::drums_of_restoration)
      .property("bloodlust", &Auras::bloodlust)
      .property("ferociousInspiration", &Auras::ferocious_inspiration)
      .property("innervate", &Auras::innervate)
      .property("manaTideTotem", &Auras::mana_tide_totem)
      .property("curseOfTheElements", &Auras::curse_of_the_elements)
      .property("shadowWeaving", &Auras::shadow_weaving)
      .property("improvedScorch", &Auras::improved_scorch)
      .property("misery", &Auras::misery)
      .property("judgementOfTheCrusader", &Auras::judgement_of_the_crusader)
      .property("vampiricTouch", &Auras::vampiric_touch)
      .property("faerieFire", &Auras::faerie_fire)
      .property("sunderArmor", &Auras::sunder_armor)
      .property("exposeArmor", &Auras::expose_armor)
      .property("curseOfRecklessness", &Auras::curse_of_recklessness)
      .property("bloodFrenzy", &Auras::blood_frenzy)
      .property("exposeWeakness", &Auras::expose_weakness)
      .property("annihilator", &Auras::annihilator)
      .property("improvedHuntersMark", &Auras::improved_hunters_mark)
      .property("superManaPotion", &Auras::super_mana_potion)
      .property("destructionPotion", &Auras::destruction_potion)
      .property("demonicRune", &Auras::demonic_rune)
      .property("flameCap", &Auras::flame_cap)
      .property("chippedPowerCore", &Auras::chipped_power_core)
      .property("crackedPowerCore", &Auras::cracked_power_core)
      .property("petBlessingOfKings", &Auras::pet_blessing_of_kings)
      .property("petBlessingOfWisdom", &Auras::pet_blessing_of_wisdom)
      .property("petBlessingOfMight", &Auras::pet_blessing_of_might)
      .property("petArcaneIntellect", &Auras::pet_arcane_intellect)
      .property("petMarkOfTheWild", &Auras::pet_mark_of_the_wild)
      .property("petPrayerOfFortitude", &Auras::pet_prayer_of_fortitude)
      .property("petPrayerOfSpirit", &Auras::pet_prayer_of_spirit)
      .property("petKiblersBits", &Auras::pet_kiblers_bits)
      .property("petHeroicPresence", &Auras::pet_heroic_presence)
      .property("petStrengthOfEarthTotem", &Auras::pet_strength_of_earth_totem)
      .property("petGraceOfAirTotem", &Auras::pet_grace_of_air_totem)
      .property("petBattleShout", &Auras::pet_battle_shout)
      .property("petTrueshotAura", &Auras::pet_trueshot_aura)
      .property("petLeaderOfThePack", &Auras::pet_leader_of_the_pack)
      .property("petUnleashedRage", &Auras::pet_unleashed_rage)
      .property("petStaminaScroll", &Auras::pet_stamina_scroll)
      .property("petIntellectScroll", &Auras::pet_intellect_scroll)
      .property("petStrengthScroll", &Auras::pet_strength_scroll)
      .property("petAgilityScroll", &Auras::pet_agility_scroll)
      .property("petSpiritScroll", &Auras::pet_spirit_scroll);

  emscripten::class_<Talents>("Talents")
      .constructor<>()
      .property("suppression", &Talents::suppression)
      .property("improvedCorruption", &Talents::improved_corruption)
      .property("improvedLifeTap", &Talents::improved_life_tap)
      .property("improvedCurseOfAgony", &Talents::improved_curse_of_agony)
      .property("amplifyCurse", &Talents::amplify_curse)
      .property("nightfall", &Talents::nightfall)
      .property("empoweredCorruption", &Talents::empowered_corruption)
      .property("siphonLife", &Talents::siphon_life)
      .property("shadowMastery", &Talents::shadow_mastery)
      .property("contagion", &Talents::contagion)
      .property("darkPact", &Talents::dark_pact)
      .property("unstableAffliction", &Talents::unstable_affliction)
      .property("improvedImp", &Talents::improved_imp)
      .property("demonicEmbrace", &Talents::demonic_embrace)
      .property("felIntellect", &Talents::fel_intellect)
      .property("felStamina", &Talents::fel_stamina)
      .property("improvedSuccubus", &Talents::improved_succubus)
      .property("demonicAegis", &Talents::demonic_aegis)
      .property("unholyPower", &Talents::unholy_power)
      .property("demonicSacrifice", &Talents::demonic_sacrifice)
      .property("manaFeed", &Talents::mana_feed)
      .property("masterDemonologist", &Talents::master_demonologist)
      .property("soulLink", &Talents::soul_link)
      .property("demonicKnowledge", &Talents::demonic_knowledge)
      .property("demonicTactics", &Talents::demonic_tactics)
      .property("felguard", &Talents::felguard)
      .property("improvedShadowBolt", &Talents::improved_shadow_bolt)
      .property("cataclysm", &Talents::cataclysm)
      .property("bane", &Talents::bane)
      .property("improvedFirebolt", &Talents::improved_firebolt)
      .property("improvedLashOfPain", &Talents::improved_lash_of_pain)
      .property("devastation", &Talents::devastation)
      .property("shadowburn", &Talents::shadowburn)
      .property("improvedSearingPain", &Talents::improved_searing_pain)
      .property("improvedImmolate", &Talents::improved_immolate)
      .property("ruin", &Talents::ruin)
      .property("emberstorm", &Talents::emberstorm)
      .property("backlash", &Talents::backlash)
      .property("conflagrate", &Talents::conflagrate)
      .property("shadowAndFlame", &Talents::shadow_and_flame)
      .property("shadowfury", &Talents::shadowfury);

  emscripten::class_<Sets>("Sets")
      .constructor<>()
      .property("plagueheart", &Sets::plagueheart)
      .property("spellfire", &Sets::spellfire)
      .property("spellstrike", &Sets::spellstrike)
      .property("oblivion", &Sets::oblivion)
      .property("manaEtched", &Sets::mana_etched)
      .property("twinStars", &Sets::twin_stars)
      .property("t4", &Sets::t4)
      .property("t5", &Sets::t5)
      .property("t6", &Sets::t6);

  emscripten::class_<CharacterStats>("CharacterStats")
      .constructor<>()
      .property("health", &CharacterStats::health)
      .property("mana", &CharacterStats::mana)
      .property("stamina", &CharacterStats::stamina)
      .property("intellect", &CharacterStats::intellect)
      .property("spirit", &CharacterStats::spirit)
      .property("spellPower", &CharacterStats::spell_power)
      .property("shadowPower", &CharacterStats::shadow_power)
      .property("firePower", &CharacterStats::fire_power)
      .property("hasteRating", &CharacterStats::haste_rating)
      .property("hastePercent", &CharacterStats::haste_percent)
      .property("hitRating", &CharacterStats::hit_rating)
      .property("critRating", &CharacterStats::crit_rating)
      .property("critChance", &CharacterStats::crit_chance)
      .property("mp5", &CharacterStats::mp5)
      .property("manaCostModifier", &CharacterStats::mana_cost_modifier)
      .property("spellPenetration", &CharacterStats::spell_penetration)
      .property("fireModifier", &CharacterStats::fire_modifier)
      .property("shadowModifier", &CharacterStats::shadow_modifier)
      .property("staminaModifier", &CharacterStats::stamina_modifier)
      .property("intellectModifier", &CharacterStats::intellect_modifier)
      .property("spiritModifier", &CharacterStats::spirit_modifier);

  emscripten::class_<PlayerSettings>("PlayerSettings")
      .constructor<Auras&, Talents&, Sets&, CharacterStats&, Items&>()
      .property("itemId", &PlayerSettings::item_id)
      .property("metaGemId", &PlayerSettings::meta_gem_id)
      .property("equippedItemSimulation", &PlayerSettings::equipped_item_simulation)
      .property("recordingCombatLogBreakdown", &PlayerSettings::recording_combat_log_breakdown)
      .property("simmingStamina", &PlayerSettings::simming_stamina)
      .property("simmingIntellect", &PlayerSettings::simming_intellect)
      .property("simmingSpirit", &PlayerSettings::simming_spirit)
      .property("simmingSpellPower", &PlayerSettings::simming_spell_power)
      .property("simmingShadowPower", &PlayerSettings::simming_shadow_power)
      .property("simmingFirePower", &PlayerSettings::simming_fire_power)
      .property("simmingHitRating", &PlayerSettings::simming_hit_rating)
      .property("simmingCritRating", &PlayerSettings::simming_crit_rating)
      .property("simmingHasteRating", &PlayerSettings::simming_haste_rating)
      .property("simmingMp5", &PlayerSettings::simming_mp5)
      .property("isAldor", &PlayerSettings::is_aldor)
      .property("enemyLevel", &PlayerSettings::enemy_level)
      .property("enemyShadowResist", &PlayerSettings::enemy_shadow_resist)
      .property("enemyFireResist", &PlayerSettings::enemy_fire_resist)
      .property("mageAtieshAmount", &PlayerSettings::mage_atiesh_amount)
      .property("totemOfWrathAmount", &PlayerSettings::totem_of_wrath_amount)
      .property("sacrificingPet", &PlayerSettings::sacrificing_pet)
      .property("petIsImp", &PlayerSettings::pet_is_imp)
      .property("petIsSuccubus", &PlayerSettings::pet_is_succubus)
      .property("petIsFelguard", &PlayerSettings::pet_is_felguard)
      .property("ferociousInspirationAmount", &PlayerSettings::ferocious_inspiration_amount)
      .property("improvedCurseOfTheElements", &PlayerSettings::improved_curse_of_the_elements)
      .property("usingCustomIsbUptime", &PlayerSettings::using_custom_isb_uptime)
      .property("customIsbUptimeValue", &PlayerSettings::custom_isb_uptime_value)
      .property("improvedDivineSpirit", &PlayerSettings::improved_divine_spirit)
      .property("improvedImp", &PlayerSettings::improved_imp)
      .property("shadowPriestDps", &PlayerSettings::shadow_priest_dps)
      .property("warlockAtieshAmount", &PlayerSettings::warlock_atiesh_amount)
      .property("improvedExposeArmor", &PlayerSettings::improved_expose_armor)
      .property("isSingleTarget", &PlayerSettings::is_single_target)
      .property("enemyAmount", &PlayerSettings::enemy_amount)
      .property("isOrc", &PlayerSettings::is_orc)
      .property("powerInfusionAmount", &PlayerSettings::power_infusion_amount)
      .property("bloodlustAmount", &PlayerSettings::bloodlust_amount)
      .property("innervateAmount", &PlayerSettings::innervate_amount)
      .property("enemyArmor", &PlayerSettings::enemy_armor)
      .property("exposeWeaknessUptime", &PlayerSettings::expose_weakness_uptime)
      .property("improvedFaerieFire", &PlayerSettings::improved_faerie_fire)
      .property("infinitePlayerMana", &PlayerSettings::infinite_player_mana)
      .property("infinitePetMana", &PlayerSettings::infinite_pet_mana)
      .property("usingLashOfPainOnCooldown", &PlayerSettings::using_lash_of_pain_on_cooldown)
      .property("petIsAggressive", &PlayerSettings::pet_is_aggressive)
      .property("prepopBlackBook", &PlayerSettings::prepop_black_book)
      .property("randomizeValues", &PlayerSettings::randomize_values)
      .property("simChoosingRotation", &PlayerSettings::sim_choosing_rotation)
      .property("exaltedWithShattrathFaction", &PlayerSettings::exalted_with_shattrath_faction)
      .property("survivalHunterAgility", &PlayerSettings::survival_hunter_agility)
      .property("hasImmolate", &PlayerSettings::has_immolate)
      .property("hasCorruption", &PlayerSettings::has_corruption)
      .property("hasSiphonLife", &PlayerSettings::has_siphon_life)
      .property("hasUnstableAffliction", &PlayerSettings::has_unstable_affliction)
      .property("hasSearingPain", &PlayerSettings::has_searing_pain)
      .property("hasShadowBolt", &PlayerSettings::has_shadow_bolt)
      .property("hasIncinerate", &PlayerSettings::has_incinerate)
      .property("hasCurseOfRecklessness", &PlayerSettings::has_curse_of_recklessness)
      .property("hasCurseOfTheElements", &PlayerSettings::has_curse_of_the_elements)
      .property("hasCurseOfAgony", &PlayerSettings::has_curse_of_agony)
      .property("hasCurseOfDoom", &PlayerSettings::has_curse_of_doom)
      .property("hasDeathCoil", &PlayerSettings::has_death_coil)
      .property("hasShadowburn", &PlayerSettings::has_shadow_burn)
      .property("hasConflagrate", &PlayerSettings::has_conflagrate)
      .property("hasShadowfury", &PlayerSettings::has_shadowfury)
      .property("hasAmplifyCurse", &PlayerSettings::has_amplify_curse)
      .property("hasDarkPact", &PlayerSettings::has_dark_pact)
      .property("hasElementalShamanT4Bonus", &PlayerSettings::has_elemental_shaman_t4_bonus);

  emscripten::class_<SimulationSettings>("SimulationSettings")
      .constructor<>()
      .property("iterations", &SimulationSettings::iterations)
      .property("minTime", &SimulationSettings::min_time)
      .property("maxTime", &SimulationSettings::max_time)
      .property("simulationType", &SimulationSettings::simulation_type);

  emscripten::enum_<SimulationType>("SimulationType")
      .value("normal", SimulationType::kNormal)
      .value("allItems", SimulationType::kAllItems)
      .value("statWeights", SimulationType::kStatWeights);

  emscripten::function("allocItems", &AllocItems);
  emscripten::function("allocAuras", &AllocAuras);
  emscripten::function("allocTalents", &AllocTalents);
  emscripten::function("allocSets", &AllocSets);
  emscripten::function("allocStats", &AllocStats);
  emscripten::function("allocPlayerSettings", &AllocPlayerSettings);
  emscripten::function("allocPlayer", &AllocPlayer);
  emscripten::function("allocSimSettings", &AllocSimSettings);
  emscripten::function("allocSim", &AllocSim);
  emscripten::function("getExceptionMessage", &GetExceptionMessage);

  /*emscripten::enum_<Constant>("Constant")
      .value("ALDOR", ALDOR)
      .value("Scryers", SCRYER)
      .value("yes", YES)
      .value("no", NO)
      .value("onCooldown", ON_COOLDOWN)
      .value("singleTarget", SINGLE_TARGET)
      .value("aoe", AOE)
      .value("noISB", NO_ISB)
      .value("human", HUMAN)
      .value("gnome", GNOME)
      .value("orc", ORC)
      .value("undead", UNDEAD)
      .value("bloodElf", BLOOD_ELF)
      .value("simChooses", SIM_CHOOSES)
      .value("userChooses", USER_CHOOSES)
      ;*/
}
#endif