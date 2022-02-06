#include "../include/bindings.h"

#include "../include/common.h"
#include "../include/embind_constant.h"
#include "../include/simulation.h"

#pragma warning(disable:4100)
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

void PostCombatLogBreakdownVector(const char* name, double mana_gain, double damage) {
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
             glancingBlows : $7,
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
#else
  std::cout << combat_log_entry << std::endl;
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
  /*std::cout << "Iteration: " << std::to_string(iteration) << "/" << std::to_string(iteration_amount)
            << ". Median DPS: " << std::to_string(median_dps) << std::endl;*/
#endif
}

void SendSimulationResults(double median_dps, double min_dps, double max_dps, int item_id, int iteration_amount,
                           int total_fight_duration, const char* custom_stat, long long simulation_duration) {
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
         median_dps, min_dps, max_dps, item_id, iteration_amount, total_fight_duration, custom_stat);
#else
  std::cout << "Median DPS: " << std::to_string(median_dps) << ". Min DPS: " << std::to_string(min_dps)
            << ". Max DPS: " << std::to_string(max_dps) << std::endl;
  std::cout << std::to_string(iteration_amount) << " itereations in " << DoubleToString(round(simulation_duration / 1000) / 1000, 3) << " seconds"
            << std::endl;
#endif
}

std::vector<uint32_t> AllocRandomSeeds(int amount_of_seeds, uint32_t rand_seed) {
  srand(rand_seed);
  std::vector<uint32_t> seeds(amount_of_seeds);

  for (int i = 0; i < amount_of_seeds; i++) {
    seeds[i] = rand();
  }

  return seeds;
}

Items AllocItems() { return Items(); }

AuraSelection AllocAuras() { return AuraSelection(); }

Talents AllocTalents() { return Talents(); }

Sets AllocSets() { return Sets(); }

CharacterStats AllocStats() { return CharacterStats(); }

PlayerSettings AllocPlayerSettings(AuraSelection& auras, Talents& talents, Sets& sets, CharacterStats& stats,
                                   Items& items) {
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
  emscripten::class_<Player>("Player").constructor<PlayerSettings&>();

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

  emscripten::class_<AuraSelection>("Auras")
      .constructor<>()
      .property("felArmor", &AuraSelection::fel_armor)
      .property("judgementOfWisdom", &AuraSelection::judgement_of_wisdom)
      .property("manaSpringTotem", &AuraSelection::mana_spring_totem)
      .property("wrathOfAirTotem", &AuraSelection::wrath_of_air_totem)
      .property("totemOfWrath", &AuraSelection::totem_of_wrath)
      .property("markOfTheWild", &AuraSelection::mark_of_the_wild)
      .property("prayerOfSpirit", &AuraSelection::prayer_of_spirit)
      .property("bloodPact", &AuraSelection::blood_pact)
      .property("inspiringPresence", &AuraSelection::inspiring_presence)
      .property("moonkinAura", &AuraSelection::moonkin_aura)
      .property("powerInfusion", &AuraSelection::power_infusion)
      .property("powerOfTheGuardianWarlock", &AuraSelection::atiesh_warlock)
      .property("powerOfTheGuardianMage", &AuraSelection::atiesh_mage)
      .property("eyeOfTheNight", &AuraSelection::eye_of_the_night)
      .property("chainOfTheTwilightOwl", &AuraSelection::chain_of_the_twilight_owl)
      .property("jadePendantOfBlasting", &AuraSelection::jade_pendant_of_blasting)
      .property("drumsOfBattle", &AuraSelection::drums_of_battle)
      .property("drumsOfWar", &AuraSelection::drums_of_war)
      .property("drumsOfRestoration", &AuraSelection::drums_of_restoration)
      .property("bloodlust", &AuraSelection::bloodlust)
      .property("ferociousInspiration", &AuraSelection::ferocious_inspiration)
      .property("innervate", &AuraSelection::innervate)
      .property("manaTideTotem", &AuraSelection::mana_tide_totem)
      .property("airmansRibbonOfGallantry", &AuraSelection::airmans_ribbon_of_gallantry)
      .property("curseOfTheElements", &AuraSelection::curse_of_the_elements)
      .property("shadowWeaving", &AuraSelection::shadow_weaving)
      .property("improvedScorch", &AuraSelection::improved_scorch)
      .property("misery", &AuraSelection::misery)
      .property("judgementOfTheCrusader", &AuraSelection::judgement_of_the_crusader)
      .property("vampiricTouch", &AuraSelection::vampiric_touch)
      .property("faerieFire", &AuraSelection::faerie_fire)
      .property("sunderArmor", &AuraSelection::sunder_armor)
      .property("exposeArmor", &AuraSelection::expose_armor)
      .property("curseOfRecklessness", &AuraSelection::curse_of_recklessness)
      .property("bloodFrenzy", &AuraSelection::blood_frenzy)
      .property("exposeWeakness", &AuraSelection::expose_weakness)
      .property("annihilator", &AuraSelection::annihilator)
      .property("improvedHuntersMark", &AuraSelection::improved_hunters_mark)
      .property("superManaPotion", &AuraSelection::super_mana_potion)
      .property("destructionPotion", &AuraSelection::destruction_potion)
      .property("demonicRune", &AuraSelection::demonic_rune)
      .property("flameCap", &AuraSelection::flame_cap)
      .property("chippedPowerCore", &AuraSelection::chipped_power_core)
      .property("crackedPowerCore", &AuraSelection::cracked_power_core)
      .property("petBlessingOfKings", &AuraSelection::pet_blessing_of_kings)
      .property("petBlessingOfWisdom", &AuraSelection::pet_blessing_of_wisdom)
      .property("petBlessingOfMight", &AuraSelection::pet_blessing_of_might)
      .property("petBattleSquawk", &AuraSelection::pet_battle_squawk)
      .property("petArcaneIntellect", &AuraSelection::pet_arcane_intellect)
      .property("petMarkOfTheWild", &AuraSelection::pet_mark_of_the_wild)
      .property("petPrayerOfFortitude", &AuraSelection::pet_prayer_of_fortitude)
      .property("petPrayerOfSpirit", &AuraSelection::pet_prayer_of_spirit)
      .property("petKiblersBits", &AuraSelection::pet_kiblers_bits)
      .property("petHeroicPresence", &AuraSelection::pet_heroic_presence)
      .property("petStrengthOfEarthTotem", &AuraSelection::pet_strength_of_earth_totem)
      .property("petGraceOfAirTotem", &AuraSelection::pet_grace_of_air_totem)
      .property("petBattleShout", &AuraSelection::pet_battle_shout)
      .property("petTrueshotAura", &AuraSelection::pet_trueshot_aura)
      .property("petLeaderOfThePack", &AuraSelection::pet_leader_of_the_pack)
      .property("petUnleashedRage", &AuraSelection::pet_unleashed_rage)
      .property("petStaminaScroll", &AuraSelection::pet_stamina_scroll)
      .property("petIntellectScroll", &AuraSelection::pet_intellect_scroll)
      .property("petStrengthScroll", &AuraSelection::pet_strength_scroll)
      .property("petAgilityScroll", &AuraSelection::pet_agility_scroll)
      .property("petSpiritScroll", &AuraSelection::pet_spirit_scroll);

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
      .property("hasteRating", &CharacterStats::spell_haste_rating)
      .property("hastePercent", &CharacterStats::spell_haste_percent)
      .property("hitRating", &CharacterStats::spell_hit_rating)
      .property("critRating", &CharacterStats::spell_crit_rating)
      .property("critChance", &CharacterStats::spell_crit_chance)
      .property("mp5", &CharacterStats::mp5)
      .property("manaCostModifier", &CharacterStats::mana_cost_modifier)
      .property("spellPenetration", &CharacterStats::spell_penetration)
      .property("fireModifier", &CharacterStats::fire_modifier)
      .property("shadowModifier", &CharacterStats::shadow_modifier)
      .property("staminaModifier", &CharacterStats::stamina_modifier)
      .property("intellectModifier", &CharacterStats::intellect_modifier)
      .property("spiritModifier", &CharacterStats::spirit_modifier);

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
      .property("plagueheart", &Sets::t3)
      .property("spellfire", &Sets::spellfire)
      .property("spellstrike", &Sets::spellstrike)
      .property("oblivion", &Sets::oblivion)
      .property("manaEtched", &Sets::mana_etched)
      .property("twinStars", &Sets::twin_stars)
      .property("t4", &Sets::t4)
      .property("t5", &Sets::t5)
      .property("t6", &Sets::t6);

  emscripten::class_<PlayerSettings>("PlayerSettings")
      .constructor<AuraSelection&, Talents&, Sets&, CharacterStats&, Items&>()
      .property("randomSeeds", &PlayerSettings::random_seeds)
      .property("itemId", &PlayerSettings::item_id)
      .property("metaGemId", &PlayerSettings::meta_gem_id)
      .property("equippedItemSimulation", &PlayerSettings::equipped_item_simulation)
      .property("recordingCombatLogBreakdown", &PlayerSettings::recording_combat_log_breakdown)
      .property("customStat", &PlayerSettings::custom_stat)
      .property("shattrathFaction", &PlayerSettings::shattrath_faction)
      .property("enemyLevel", &PlayerSettings::enemy_level)
      .property("enemyShadowResist", &PlayerSettings::enemy_shadow_resist)
      .property("enemyFireResist", &PlayerSettings::enemy_fire_resist)
      .property("mageAtieshAmount", &PlayerSettings::mage_atiesh_amount)
      .property("totemOfWrathAmount", &PlayerSettings::totem_of_wrath_amount)
      .property("chippedPowerCoreAmount", &PlayerSettings::chipped_power_core_amount)
      .property("crackedPowerCoreAmount", &PlayerSettings::cracked_power_core_amount)
      .property("sacrificingPet", &PlayerSettings::sacrificing_pet)
      .property("selectedPet", &PlayerSettings::selected_pet)
      .property("ferociousInspirationAmount", &PlayerSettings::ferocious_inspiration_amount)
      .property("improvedCurseOfTheElements", &PlayerSettings::improved_curse_of_the_elements)
      .property("usingCustomIsbUptime", &PlayerSettings::using_custom_isb_uptime)
      .property("customIsbUptimeValue", &PlayerSettings::custom_isb_uptime_value)
      .property("improvedDivineSpirit", &PlayerSettings::improved_divine_spirit)
      .property("improvedImp", &PlayerSettings::improved_imp)
      .property("shadowPriestDps", &PlayerSettings::shadow_priest_dps)
      .property("warlockAtieshAmount", &PlayerSettings::warlock_atiesh_amount)
      .property("improvedExposeArmor", &PlayerSettings::improved_expose_armor)
      .property("battleSquawkAmount", &PlayerSettings::battle_squawk_amount)
      .property("fightType", &PlayerSettings::fight_type)
      .property("enemyAmount", &PlayerSettings::enemy_amount)
      .property("race", &PlayerSettings::race)
      .property("powerInfusionAmount", &PlayerSettings::power_infusion_amount)
      .property("bloodlustAmount", &PlayerSettings::bloodlust_amount)
      .property("innervateAmount", &PlayerSettings::innervate_amount)
      .property("enemyArmor", &PlayerSettings::enemy_armor)
      .property("exposeWeaknessUptime", &PlayerSettings::expose_weakness_uptime)
      .property("improvedFaerieFire", &PlayerSettings::improved_faerie_fire)
      .property("infinitePlayerMana", &PlayerSettings::infinite_player_mana)
      .property("infinitePetMana", &PlayerSettings::infinite_pet_mana)
      .property("lashOfPainUsage", &PlayerSettings::lash_of_pain_usage)
      .property("petMode", &PlayerSettings::pet_mode)
      .property("prepopBlackBook", &PlayerSettings::prepop_black_book)
      .property("randomizeValues", &PlayerSettings::randomize_values)
      .property("rotationOption", &PlayerSettings::rotation_option)
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

  emscripten::enum_<EmbindConstant>("EmbindConstant")
      .value("aldor", EmbindConstant::kAldor)
      .value("scryers", EmbindConstant::kScryers)
      .value("onCooldown", EmbindConstant::kOnCooldown)
      .value("singleTarget", EmbindConstant::kSingleTarget)
      .value("aoe", EmbindConstant::kAoe)
      .value("noIsb", EmbindConstant::kNoIsb)
      .value("human", EmbindConstant::kHuman)
      .value("gnome", EmbindConstant::kGnome)
      .value("orc", EmbindConstant::kOrc)
      .value("undead", EmbindConstant::kUndead)
      .value("bloodElf", EmbindConstant::kBloodElf)
      .value("simChooses", EmbindConstant::kSimChooses)
      .value("userChooses", EmbindConstant::kUserChooses)
      .value("stamina", EmbindConstant::kStamina)
      .value("intellect", EmbindConstant::kIntellect)
      .value("spirit", EmbindConstant::kSpirit)
      .value("spellPower", EmbindConstant::kSpellPower)
      .value("shadowPower", EmbindConstant::kShadowPower)
      .value("firePower", EmbindConstant::kFirePower)
      .value("hitRating", EmbindConstant::kHitRating)
      .value("critRating", EmbindConstant::kCritRating)
      .value("hasteRating", EmbindConstant::kHasteRating)
      .value("mp5", EmbindConstant::kMp5)
      .value("normal", EmbindConstant::kNormal)
      .value("imp", EmbindConstant::kImp)
      .value("succubus", EmbindConstant::kSuccubus)
      .value("felguard", EmbindConstant::kFelguard)
      .value("passive", EmbindConstant::kPassive)
      .value("aggressive", EmbindConstant::kAggressive);

  emscripten::function("allocRandomSeeds", &AllocRandomSeeds);
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

  emscripten::register_vector<uint32_t>("vector<uint32_t>");
}
#endif

#pragma warning(default:4100)