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

void PostCombatLogBreakdown(const char* name, int casts, int crits, int misses, int count, double uptime, int dodges,
                            int glancing_blows) {
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

std::shared_ptr<Items> AllocItems() { return std::make_shared<Items>(); }

std::shared_ptr<Auras> AllocAuras() { return std::make_shared<Auras>(); }

std::shared_ptr<Talents> AllocTalents() { return std::make_shared<Talents>(); }

std::shared_ptr<Sets> AllocSets() { return std::make_shared<Sets>(); }

std::shared_ptr<CharacterStats> AllocStats() { return std::make_shared<CharacterStats>(); }

std::shared_ptr<PlayerSettings> AllocPlayerSettings() { return std::make_shared<PlayerSettings>(); }

std::shared_ptr<Player> AllocPlayer(std::shared_ptr<PlayerSettings> settings) {
  return std::make_shared<Player>(settings);
}

std::shared_ptr<SimulationSettings> AllocSimSettings() { return std::make_shared<SimulationSettings>(); }

std::shared_ptr<Simulation> AllocSim(std::shared_ptr<Player> player,
                                     std::shared_ptr<SimulationSettings> simulation_settings) {
  return std::make_shared<Simulation>(player, simulation_settings);
}

std::string GetExceptionMessage(intptr_t exception_ptr) {
  return std::string(reinterpret_cast<std::exception*>(exception_ptr)->what());
}

#ifdef EMSCRIPTEN
EMSCRIPTEN_BINDINGS(module) {
  emscripten::class_<Player>("Player").smart_ptr<std::shared_ptr<Player>>("Player");

  emscripten::class_<Simulation>("Simulation")
      .smart_ptr<std::shared_ptr<Simulation>>("Simulation")
      .property("player", &Simulation::player)
      .property("simSettings", &Simulation::settings)
      .function("start", &Simulation::Start);

  emscripten::class_<Items>("Items")
      .smart_ptr<std::shared_ptr<Items>>("Items")
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
      .smart_ptr<std::shared_ptr<Auras>>("Auras")
      .property("felArmor", &Auras::felArmor)
      .property("blessingOfKings", &Auras::blessingOfKings)
      .property("blessingOfWisdom", &Auras::blessingOfWisdom)
      .property("judgementOfWisdom", &Auras::judgementOfWisdom)
      .property("manaSpringTotem", &Auras::manaSpringTotem)
      .property("wrathOfAirTotem", &Auras::wrathOfAirTotem)
      .property("totemOfWrath", &Auras::totemOfWrath)
      .property("markOfTheWild", &Auras::markOfTheWild)
      .property("arcaneIntellect", &Auras::arcaneIntellect)
      .property("prayerOfFortitude", &Auras::prayerOfFortitude)
      .property("prayerOfSpirit", &Auras::prayerOfSpirit)
      .property("bloodPact", &Auras::bloodPact)
      .property("inspiringPresence", &Auras::inspiringPresence)
      .property("moonkinAura", &Auras::moonkinAura)
      .property("powerInfusion", &Auras::powerInfusion)
      .property("powerOfTheGuardianWarlock", &Auras::powerOfTheGuardianWarlock)
      .property("powerOfTheGuardianMage", &Auras::powerOfTheGuardianMage)
      .property("eyeOfTheNight", &Auras::eyeOfTheNight)
      .property("chainOfTheTwilightOwl", &Auras::chainOfTheTwilightOwl)
      .property("jadePendantOfBlasting", &Auras::jadePendantOfBlasting)
      .property("idolOfTheRavenGoddess", &Auras::idolOfTheRavenGoddess)
      .property("drumsOfBattle", &Auras::drumsOfBattle)
      .property("drumsOfWar", &Auras::drumsOfWar)
      .property("drumsOfRestoration", &Auras::drumsOfRestoration)
      .property("bloodlust", &Auras::bloodlust)
      .property("ferociousInspiration", &Auras::ferociousInspiration)
      .property("innervate", &Auras::innervate)
      .property("manaTideTotem", &Auras::manaTideTotem)
      .property("curseOfTheElements", &Auras::curseOfTheElements)
      .property("shadowWeaving", &Auras::shadowWeaving)
      .property("improvedScorch", &Auras::improvedScorch)
      .property("misery", &Auras::misery)
      .property("judgementOfTheCrusader", &Auras::judgementOfTheCrusader)
      .property("vampiricTouch", &Auras::vampiricTouch)
      .property("faerieFire", &Auras::faerieFire)
      .property("sunderArmor", &Auras::sunderArmor)
      .property("exposeArmor", &Auras::exposeArmor)
      .property("curseOfRecklessness", &Auras::curseOfRecklessness)
      .property("bloodFrenzy", &Auras::bloodFrenzy)
      .property("exposeWeakness", &Auras::exposeWeakness)
      .property("annihilator", &Auras::annihilator)
      .property("improvedHuntersMark", &Auras::improvedHuntersMark)
      .property("flaskOfPureDeath", &Auras::flaskOfPureDeath)
      .property("elixirOfMajorShadowPower", &Auras::elixirOfMajorShadowPower)
      .property("elixirOfMajorFirepower", &Auras::elixirOfMajorFirepower)
      .property("greaterArcaneElixir", &Auras::greaterArcaneElixir)
      .property("adeptsElixir", &Auras::adeptsElixir)
      .property("elixirOfDraenicWisdom", &Auras::elixirOfDraenicWisdom)
      .property("elixirOfMajorMageblood", &Auras::elixirOfMajorMageblood)
      .property("superManaPotion", &Auras::superManaPotion)
      .property("destructionPotion", &Auras::destructionPotion)
      .property("brilliantWizardOil", &Auras::brilliantWizardOil)
      .property("superiorWizardOil", &Auras::superiorWizardOil)
      .property("blessedWizardOil", &Auras::blessedWizardOil)
      .property("demonicRune", &Auras::demonicRune)
      .property("flameCap", &Auras::flameCap)
      .property("rumseyRumBlackLabel", &Auras::rumseyRumBlackLabel)
      .property("kreegsStoutBeatdown", &Auras::kreegsStoutBeatdown)
      .property("blackenedBasilisk", &Auras::blackenedBasilisk)
      .property("skullfishSoup", &Auras::skullfishSoup)
      .property("veryBerryCream", &Auras::veryBerryCream)
      .property("midsummerSausage", &Auras::midsummerSausage)
      .property("bloodthistle", &Auras::bloodthistle)
      .property("chippedPowerCore", &Auras::chippedPowerCore)
      .property("crackedPowerCore", &Auras::crackedPowerCore)
      .property("petBlessingOfKings", &Auras::petBlessingOfKings)
      .property("petBlessingOfWisdom", &Auras::petBlessingOfWisdom)
      .property("petBlessingOfMight", &Auras::petBlessingOfMight)
      .property("petArcaneIntellect", &Auras::petArcaneIntellect)
      .property("petMarkOfTheWild", &Auras::petMarkOfTheWild)
      .property("petPrayerOfFortitude", &Auras::petPrayerOfFortitude)
      .property("petPrayerOfSpirit", &Auras::petPrayerOfSpirit)
      .property("petKiblersBits", &Auras::petKiblersBits)
      .property("petHeroicPresence", &Auras::petHeroicPresence)
      .property("petStrengthOfEarthTotem", &Auras::petStrengthOfEarthTotem)
      .property("petGraceOfAirTotem", &Auras::petGraceOfAirTotem)
      .property("petBattleShout", &Auras::petBattleShout)
      .property("petTrueshotAura", &Auras::petTrueshotAura)
      .property("petLeaderOfThePack", &Auras::petLeaderOfThePack)
      .property("petUnleashedRage", &Auras::petUnleashedRage)
      .property("petStaminaScroll", &Auras::petStaminaScroll)
      .property("petIntellectScroll", &Auras::petIntellectScroll)
      .property("petStrengthScroll", &Auras::petStrengthScroll)
      .property("petAgilityScroll", &Auras::petAgilityScroll)
      .property("petSpiritScroll", &Auras::petSpiritScroll);

  emscripten::class_<Talents>("Talents")
      .smart_ptr<std::shared_ptr<Talents>>("Talents")
      .property("suppression", &Talents::suppression)
      .property("improvedCorruption", &Talents::improved_corruption)
      .property("improvedLifeTap", &Talents::improved_life_tap)
      .property("improvedCurseOfAgony", &Talents::improved_curse_of_agony)
      .property("amplifyCurse", &Talents::amplify_curse)
      .property("nightfall", &Talents::nightfall)
      .property("empoweredCorruption", &Talents::empowered_corruption)
      .property("siphonLife", &Talents::siphon_life)
      .property("shadowMastery", &Talents::shadowMastery)
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
      .smart_ptr<std::shared_ptr<Sets>>("Sets")
      .property("plagueheart", &Sets::plagueheart)
      .property("spellfire", &Sets::spellfire)
      .property("frozenShadoweave", &Sets::frozen_shadoweave)
      .property("spellstrike", &Sets::spellstrike)
      .property("oblivion", &Sets::oblivion)
      .property("manaEtched", &Sets::mana_etched)
      .property("twinStars", &Sets::twin_stars)
      .property("t4", &Sets::t4)
      .property("t5", &Sets::t5)
      .property("t6", &Sets::t6);

  emscripten::class_<CharacterStats>("CharacterStats")
      .smart_ptr<std::shared_ptr<CharacterStats>>("CharacterStats")
      .property("health", &CharacterStats::health)
      .property("mana", &CharacterStats::mana)
      .property("max_mana", &CharacterStats::max_mana)
      .property("stamina", &CharacterStats::stamina)
      .property("intellect", &CharacterStats::intellect)
      .property("spirit", &CharacterStats::spirit)
      .property("spell_power", &CharacterStats::spell_power)
      .property("shadow_power", &CharacterStats::shadow_power)
      .property("fire_power", &CharacterStats::fire_power)
      .property("haste_rating", &CharacterStats::haste_rating)
      .property("haste_percent", &CharacterStats::haste_percent)
      .property("hit_rating", &CharacterStats::hit_rating)
      .property("crit_rating", &CharacterStats::crit_rating)
      .property("crit_chance", &CharacterStats::crit_chance)
      .property("mp5", &CharacterStats::mp5)
      .property("mana_cost_modifier", &CharacterStats::mana_cost_modifier)
      .property("spellPenetration", &CharacterStats::spell_penetration)
      .property("fire_modifier", &CharacterStats::fire_modifier)
      .property("frost_modifier", &CharacterStats::frost_modifier)
      .property("shadow_modifier", &CharacterStats::shadow_modifier)
      .property("stamina_modifier", &CharacterStats::stamina_modifier)
      .property("intellect_modifier", &CharacterStats::intellect_modifier)
      .property("spirit_modifier", &CharacterStats::spirit_modifier)
      .property("arcane_modifier", &CharacterStats::arcane_modifier)
      .property("nature_modifier", &CharacterStats::nature_modifier)
      .property("nature_resist", &CharacterStats::nature_resist)
      .property("arcane_resist", &CharacterStats::arcane_resist)
      .property("fire_resist", &CharacterStats::fire_resist)
      .property("frost_resist", &CharacterStats::frost_resist)
      .property("shadow_resist", &CharacterStats::shadow_resist);

  emscripten::class_<PlayerSettings>("PlayerSettings")
      .smart_ptr<std::shared_ptr<PlayerSettings>>("PlayerSettings")
      .property("auras", &PlayerSettings::auras)
      .property("talents", &PlayerSettings::talents)
      .property("sets", &PlayerSettings::sets)
      .property("stats", &PlayerSettings::stats)
      .property("items", &PlayerSettings::items)
      .property("item_id", &PlayerSettings::item_id)
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
      .property("mageAtieshAmount", &PlayerSettings::mageAtieshAmount)
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
      .smart_ptr<std::shared_ptr<SimulationSettings>>("SimulationSettings")
      .property("iterations", &SimulationSettings::iterations)
      .property("minTime", &SimulationSettings::min_time)
      .property("maxTime", &SimulationSettings::max_time)
      .property("simulationType", &SimulationSettings::simulation_type);

  emscripten::enum_<SimulationType>("SimulationType")
      .value("normal", SimulationType::NORMAL)
      .value("allItems", SimulationType::ALL_ITEMS)
      .value("statWeights", SimulationType::STAT_WEIGHTS);

  emscripten::function("AllocItems", &AllocItems);
  emscripten::function("AllocAuras", &AllocAuras);
  emscripten::function("AllocTalents", &AllocTalents);
  emscripten::function("AllocSets", &AllocSets);
  emscripten::function("AllocStats", &AllocStats);
  emscripten::function("AllocPlayerSettings", &AllocPlayerSettings);
  emscripten::function("AllocPlayer", &AllocPlayer);
  emscripten::function("AllocSimSettings", &AllocSimSettings);
  emscripten::function("AllocSim", &AllocSim);
  emscripten::function("GetExceptionMessage", &GetExceptionMessage);

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