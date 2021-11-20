#include "bindings.h"

#include "embind_constant.h"
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

std::vector<uint32_t> AllocRandomSeeds(int amount_of_seeds, uint32_t rand_seed) {
  srand(rand_seed);
  std::vector<uint32_t> seeds(amount_of_seeds);

  for (int i = 0; i < amount_of_seeds; i++) {
    seeds[i] = rand();
  }

  return seeds;
}

Items AllocItems() { return Items(); }

Auras AllocAuras() { return Auras(); }

Talents AllocTalents() { return Talents(); }

Sets AllocSets() { return Sets(); }

std::map<CharacterStat, double> AllocStats() {
  return std::map<CharacterStat, double>{
      {CharacterStat::kHealth, 0},
      {CharacterStat::kMana, 0},
      {CharacterStat::kMaxMana, 0},
      {CharacterStat::kStamina, 0},
      {CharacterStat::kIntellect, 0},
      {CharacterStat::kSpirit, 0},
      {CharacterStat::kSpellPower, 0},
      {CharacterStat::kShadowPower, 0},
      {CharacterStat::kFirePower, 0},
      {CharacterStat::kSpellHasteRating, 0},
      {CharacterStat::kSpellHitRating, 0},
      {CharacterStat::kSpellCritRating, 0},
      {CharacterStat::kSpellCritChance, 0},
      {CharacterStat::kSpellHitChance, 0},
      {CharacterStat::kMp5, 0},
      {CharacterStat::kSpellPenetration, 0},
      {CharacterStat::kFireModifier, 1},
      {CharacterStat::kSpellHastePercent, 1},
      {CharacterStat::kShadowModifier, 1},
      {CharacterStat::kStaminaModifier, 1},
      {CharacterStat::kIntellectModifier, 1},
      {CharacterStat::kSpiritModifier, 1},
      {CharacterStat::kManaCostModifier, 1},
      {CharacterStat::kStrength, 0},
      {CharacterStat::kAgility, 0},
      {CharacterStat::kAttackPower, 0},
      {CharacterStat::kMeleeCritChance, 0},
      {CharacterStat::kMeleeHitChance, 0},
      {CharacterStat::kStrengthModifier, 1},
      {CharacterStat::kAgilityModifier, 1},
      {CharacterStat::kAttackPowerModifier, 1},
      {CharacterStat::kDamageModifier, 1},
      {CharacterStat::kMeleeHastePercent, 1},
  };
}

PlayerSettings AllocPlayerSettings(Auras& auras, Talents& talents, Sets& sets, std::map<CharacterStat, double>& stats,
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
      .property("petBattleSquawk", &Auras::pet_battle_squawk)
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

  emscripten::class_<PlayerSettings>("PlayerSettings")
      .constructor<Auras&, Talents&, Sets&, std::map<CharacterStat, double>&, Items&>()
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

  emscripten::enum_<CharacterStat>("CharacterStat")
      .value("health", CharacterStat::kHealth)
      .value("mana", CharacterStat::kMana)
      .value("stamina", CharacterStat::kStamina)
      .value("intellect", CharacterStat::kIntellect)
      .value("spirit", CharacterStat::kSpirit)
      .value("spellPower", CharacterStat::kSpellPower)
      .value("shadowPower", CharacterStat::kShadowPower)
      .value("firePower", CharacterStat::kFirePower)
      .value("spellHasteRating", CharacterStat::kSpellHasteRating)
      .value("spellHitRating", CharacterStat::kSpellHitRating)
      .value("spellCritRating", CharacterStat::kSpellCritRating)
      .value("spellCritChance", CharacterStat::kSpellCritChance)
      .value("spellHitChance", CharacterStat::kSpellHitChance)
      .value("mp5", CharacterStat::kMp5)
      .value("spellPenetration", CharacterStat::kSpellPenetration)
      .value("fireModifier", CharacterStat::kFireModifier)
      .value("spellHastePercent", CharacterStat::kSpellHastePercent)
      .value("shadowModifier", CharacterStat::kShadowModifier)
      .value("staminaModifier", CharacterStat::kStaminaModifier)
      .value("intellectModifier", CharacterStat::kIntellectModifier)
      .value("spiritModifier", CharacterStat::kSpiritModifier)
      .value("manaCostModifier", CharacterStat::kManaCostModifier)
      .value("strength", CharacterStat::kStrength)
      .value("agility", CharacterStat::kAgility)
      .value("attackPower", CharacterStat::kAttackPower)
      .value("meleeCritChance", CharacterStat::kMeleeCritChance)
      .value("meleeHitChance", CharacterStat::kMeleeHitChance)
      .value("strengthModifier", CharacterStat::kStrengthModifier)
      .value("agilityModifier", CharacterStat::kAgilityModifier)
      .value("attackPowerModifier", CharacterStat::kAttackPowerModifier)
      .value("damageModifier", CharacterStat::kDamageModifier)
      .value("meleeHastePercent", CharacterStat::kMeleeHastePercent);

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
  emscripten::register_map<CharacterStat, double>("MapCharacterStatDouble");
}
#endif