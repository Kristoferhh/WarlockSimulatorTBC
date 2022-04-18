#include "../include/player.h"

#include <cmath>

#include "../include/player_settings.h"
#include "../include/combat_log_breakdown.h"
#include "../include/talents.h"
#include "../include/trinket.h"
#include "../include/damage_over_time.h"
#include "../include/aura_selection.h"
#include "../include/aura.h"
#include "../include/spell.h"
#include "../include/on_hit_proc.h"
#include "../include/on_crit_proc.h"
#include "../include/common.h"
#include "../include/pet.h"
#include "../include/sets.h"
#include "../include/stat.h"
#include "../include/on_dot_tick_proc.h"
#include "../include/items.h"
#include "../include/mana_over_time.h"
#include "../include/on_damage_proc.h"
#include "../include/mana_potion.h"
#include "../include/life_tap.h"
#include "../include/on_resist_proc.h"
#include "../include/bindings.h"

Player::Player(PlayerSettings& settings)
  : Entity(nullptr, settings, EntityType::kPlayer),
    selected_auras(settings.auras),
    talents(settings.talents),
    sets(settings.sets),
    items(settings.items),
    settings(settings) {
  name = "Player";
  infinite_mana = settings.infinite_player_mana;

  if (recording_combat_log_breakdown) {
    combat_log_breakdown.insert({StatName::kMp5, std::make_shared<CombatLogBreakdown>(StatName::kMp5)});
  }

  if (settings.custom_stat == EmbindConstant::kStamina) {
    custom_stat = "stamina";
  } else if (settings.custom_stat == EmbindConstant::kIntellect) {
    custom_stat = "intellect";
  } else if (settings.custom_stat == EmbindConstant::kSpirit) {
    custom_stat = "spirit";
  } else if (settings.custom_stat == EmbindConstant::kSpellPower) {
    custom_stat = "spellPower";
  } else if (settings.custom_stat == EmbindConstant::kShadowPower) {
    custom_stat = "shadowPower";
  } else if (settings.custom_stat == EmbindConstant::kFirePower) {
    custom_stat = "firePower";
  } else if (settings.custom_stat == EmbindConstant::kCritRating) {
    custom_stat = "critRating";
  } else if (settings.custom_stat == EmbindConstant::kHitRating) {
    custom_stat = "hitRating";
  } else if (settings.custom_stat == EmbindConstant::kHasteRating) {
    custom_stat = "hasteRating";
  } else if (settings.custom_stat == EmbindConstant::kMp5) {
    custom_stat = "mp5";
  } else {
    custom_stat = "normal";
  }

  // Demonic Sacrifice
  if (talents.demonic_sacrifice == 1 && settings.sacrificing_pet) {
    if (settings.selected_pet == EmbindConstant::kImp) {
      stats.fire_modifier *= 1.15;
    } else if (settings.selected_pet == EmbindConstant::kSuccubus) {
      stats.shadow_modifier *= 1.15;
    } else if (settings.selected_pet == EmbindConstant::kFelguard) {
      stats.shadow_modifier *= 1.1;
    }
  } else {
    if (talents.master_demonologist > 0) {
      auto damage_modifier = 1.0;

      if (settings.selected_pet == EmbindConstant::kSuccubus) {
        damage_modifier += 0.02 * talents.master_demonologist;
      } else if (settings.selected_pet == EmbindConstant::kFelguard) {
        damage_modifier += 0.01 * talents.master_demonologist;
      }

      stats.shadow_modifier *= damage_modifier;
      stats.fire_modifier *= damage_modifier;
    }
  }

  if (selected_auras.fel_armor) {
    stats.spell_power += 100 * (0 + 0.1 * talents.demonic_aegis);
  }

  stats.spirit_modifier *= 1 - 0.01 * talents.demonic_embrace;
  stats.stamina_modifier *= 1 + 0.03 * talents.demonic_embrace;

  if (sets.twin_stars == 2) {
    stats.spell_power += 15;
  }

  // Enemy Armor Reduction
  if (selected_auras.faerie_fire) {
    settings.enemy_armor -= 610;
  }
  if (selected_auras.sunder_armor && selected_auras.expose_armor && settings.improved_expose_armor == 2 ||
      selected_auras.expose_armor && !selected_auras.sunder_armor) {
    settings.enemy_armor -= static_cast<int>(2050 * (1 + 0.25 * settings.improved_expose_armor));
  } else if (selected_auras.sunder_armor) {
    settings.enemy_armor -= 520 * 5;
  }
  if (selected_auras.curse_of_recklessness) {
    settings.enemy_armor -= 800;
  }
  if (selected_auras.annihilator) {
    settings.enemy_armor -= 600;
  }
  settings.enemy_armor = std::max(0, settings.enemy_armor);

  // Health & Mana
  stats.health = (stats.health + Entity::GetStamina() * StatConstant::kHealthPerStamina) *
                 (1 + 0.01 * static_cast<double>(talents.fel_stamina));
  stats.max_mana = (stats.mana + Entity::GetIntellect() * StatConstant::kManaPerIntellect) *
                   (1 + 0.01 * static_cast<double>(talents.fel_intellect));
}

void Player::Initialize(Simulation* simulation_ptr) {
  std::vector equipped_trinket_ids{items.trinket_1, items.trinket_2};
  std::vector equipped_ring_ids{items.ring_1, items.ring_2};

  Entity::Initialize(simulation_ptr);
  player = this;

  if (!settings.sacrificing_pet || talents.demonic_sacrifice == 0) {
    pet = std::make_shared<Pet>(*this, settings.selected_pet);
    pet->Initialize(simulation_ptr);
  } else if (talents.demonic_sacrifice == 1 && settings.sacrificing_pet &&
             settings.selected_pet == EmbindConstant::kFelhunter) {
    auras.fel_energy = std::make_shared<FelEnergyAura>(*this);
  }

  for (const auto& trinket_id : equipped_trinket_ids) {
    if (trinket_id == ItemId::kSkullOfGuldan) {
      trinkets.push_back(SkullOfGuldan(*this));
    }
    if (trinket_id == ItemId::kShiftingNaaruSliver) {
      trinkets.push_back(ShiftingNaaruSliver(*this));
    }
    if (trinket_id == ItemId::kHexShrunkenHead) {
      trinkets.push_back(HexShrunkenHead(*this));
    }
    if (trinket_id == ItemId::kIconOfTheSilverCrescent) {
      trinkets.push_back(IconOfTheSilverCrescent(*this));
    }
    if (trinket_id == ItemId::kDarkIronSmokingPipe) {
      trinkets.push_back(DarkIronSmokingPipe(*this));
    }
    if (trinket_id == ItemId::kScryersBloodgem) {
      trinkets.push_back(ScryersBloodgem(*this));
    }
    if (trinket_id == ItemId::kRestrainedEssenceOfSapphiron) {
      trinkets.push_back(RestrainedEssenceOfSapphiron(*this));
    }
    if (trinket_id == ItemId::kXirisGift) {
      trinkets.push_back(XirisGift(*this));
    }
    if (trinket_id == ItemId::kAncientCrystalTalisman) {
      trinkets.push_back(AncientCrystalTalisman(*this));
    }
    if (trinket_id == ItemId::kArcanistsStone) {
      trinkets.push_back(ArcanistsStone(*this));
    }
    if (trinket_id == ItemId::kTerokkarTabletOfVim) {
      trinkets.push_back(TerokkarTabletOfVim(*this));
    }
    if (trinket_id == ItemId::kVengeanceOfTheIllidari) {
      trinkets.push_back(VengeanceOfTheIllidari(*this));
    }
    if (trinket_id == ItemId::kFigurineLivingRubySerpent) {
      trinkets.push_back(FigurineLivingRubySerpent(*this));
    }
    if (trinket_id == ItemId::kEssenceOfTheMartyr) {
      trinkets.push_back(EssenceOfTheMartyr(*this));
    }
    if (trinket_id == ItemId::kStarkillersBauble) {
      trinkets.push_back(StarkillersBauble(*this));
    }
    if (trinket_id == ItemId::kHazzarahsCharmOfDestruction) {
      trinkets.push_back(HazzarahsCharmOfDestruction(*this));
    }
  }

  // Auras
  if (settings.fight_type == EmbindConstant::kSingleTarget) {
    if (talents.improved_shadow_bolt > 0) {
      auras.improved_shadow_bolt = std::make_shared<ImprovedShadowBoltAura>(*this);
    }
    if (settings.has_corruption || settings.rotation_option == EmbindConstant::kSimChooses) {
      auras.corruption = std::make_shared<CorruptionDot>(*this);
    }
    if (talents.unstable_affliction == 1 &&
        (settings.has_unstable_affliction || settings.rotation_option == EmbindConstant::kSimChooses)) {
      auras.unstable_affliction = std::make_shared<UnstableAfflictionDot>(*this);
    }
    if (talents.siphon_life == 1 &&
        (settings.has_siphon_life || settings.rotation_option == EmbindConstant::kSimChooses)) {
      auras.siphon_life = std::make_shared<SiphonLifeDot>(*this);
    }
    if (settings.has_immolate || settings.rotation_option == EmbindConstant::kSimChooses) {
      auras.immolate = std::make_shared<ImmolateDot>(*this);
    }
    if (settings.has_curse_of_agony || settings.has_curse_of_doom) {
      auras.curse_of_agony = std::make_shared<CurseOfAgonyDot>(*this);
    }
    if (settings.has_curse_of_the_elements) {
      auras.curse_of_the_elements = std::make_shared<CurseOfTheElementsAura>(*this);
    }
    if (settings.has_curse_of_recklessness) {
      auras.curse_of_recklessness = std::make_shared<CurseOfRecklessnessAura>(*this);
    }
    if (settings.has_curse_of_doom) {
      auras.curse_of_doom = std::make_shared<CurseOfDoomDot>(*this);
    }
    if (talents.nightfall > 0) {
      auras.shadow_trance = std::make_shared<ShadowTranceAura>(*this);
    }
    if (talents.amplify_curse == 1 &&
        (settings.has_amplify_curse || settings.rotation_option == EmbindConstant::kSimChooses)) {
      auras.amplify_curse = std::make_shared<AmplifyCurseAura>(*this);
    }
  }
  if (selected_auras.airmans_ribbon_of_gallantry) {
    auras.airmans_ribbon_of_gallantry = std::make_shared<AirmansRibbonOfGallantryAura>(*this);
  }
  if (selected_auras.mana_tide_totem) {
    auras.mana_tide_totem = std::make_shared<ManaTideTotemAura>(*this);
  }
  if (selected_auras.chipped_power_core) {
    auras.chipped_power_core = std::make_shared<ChippedPowerCoreAura>(*this);
  }
  if (selected_auras.cracked_power_core) {
    auras.cracked_power_core = std::make_shared<CrackedPowerCoreAura>(*this);
  }
  if (selected_auras.power_infusion) {
    auras.power_infusion = std::make_shared<PowerInfusionAura>(*this);
  }
  if (selected_auras.innervate) {
    auras.innervate = std::make_shared<InnervateAura>(*this);
  }
  if (selected_auras.bloodlust) {
    auras.bloodlust = std::make_shared<BloodlustAura>(*this);
  }
  if (selected_auras.destruction_potion) {
    auras.destruction_potion = std::make_shared<DestructionPotionAura>(*this);
  }
  if (selected_auras.flame_cap) {
    auras.flame_cap = std::make_shared<FlameCapAura>(*this);
  }
  if (settings.race == EmbindConstant::kOrc) {
    auras.blood_fury = std::make_shared<BloodFuryAura>(*this);
  }
  if (selected_auras.drums_of_battle) {
    auras.drums_of_battle = std::make_shared<DrumsOfBattleAura>(*this);
  } else if (selected_auras.drums_of_war) {
    auras.drums_of_war = std::make_shared<DrumsOfWarAura>(*this);
  } else if (selected_auras.drums_of_restoration) {
    auras.drums_of_restoration = std::make_shared<DrumsOfRestorationAura>(*this);
  }
  if (items.main_hand == ItemId::kBladeOfWizardry) {
    auras.blade_of_wizardry = std::make_shared<BladeOfWizardryAura>(*this);
  }
  if (items.neck == ItemId::kShatteredSunPendantOfAcumen && settings.exalted_with_shattrath_faction) {
    if (settings.shattrath_faction == EmbindConstant::kAldor) {
      auras.shattered_sun_pendant_of_acumen_aldor = std::make_shared<ShatteredSunPendantOfAcumenAldorAura>(*this);
    } else if (settings.shattrath_faction == EmbindConstant::kScryers) {
      spells.shattered_sun_pendant_of_acumen_scryers = std::make_shared<ShatteredSunPendantOfAcumenScryers>(*this);
    }
  }
  if (items.chest == ItemId::kRobeOfTheElderScribes) {
    auras.robe_of_the_elder_scribes = std::make_shared<RobeOfTheElderScribesAura>(*this);
  }
  if (settings.meta_gem_id == ItemId::kMysticalSkyfireDiamond) {
    auras.mystical_skyfire_diamond = std::make_shared<MysticalSkyfireDiamondAura>(*this);
  }
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kEyeOfMagtheridon) !=
      equipped_trinket_ids.end()) {
    auras.eye_of_magtheridon = std::make_shared<EyeOfMagtheridonAura>(*this);
    spells.eye_of_magtheridon = std::make_shared<EyeOfMagtheridon>(*this, auras.eye_of_magtheridon);
  }
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kAshtongueTalismanOfShadows) !=
      equipped_trinket_ids.end()) {
    auras.ashtongue_talisman_of_shadows = std::make_shared<AshtongueTalismanOfShadowsAura>(*this);
  }
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kDarkmoonCardCrusade) !=
      equipped_trinket_ids.end()) {
    auras.darkmoon_card_crusade = std::make_shared<DarkmoonCardCrusadeAura>(*this);
  }
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kTheLightningCapacitor) !=
      equipped_trinket_ids.end()) {
    auras.the_lightning_capacitor = std::make_shared<TheLightningCapacitorAura>(*this);
  }
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kQuagmirransEye) !=
      equipped_trinket_ids.end()) {
    auras.quagmirrans_eye = std::make_shared<QuagmirransEyeAura>(*this);
  }
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kShiffarsNexusHorn) !=
      equipped_trinket_ids.end()) {
    auras.shiffars_nexus_horn = std::make_shared<ShiffarsNexusHornAura>(*this);
  }
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kSextantOfUnstableCurrents) !=
      equipped_trinket_ids.end()) {
    auras.sextant_of_unstable_currents = std::make_shared<SextantOfUnstableCurrentsAura>(*this);
  }
  if (std::find(equipped_ring_ids.begin(), equipped_ring_ids.end(), ItemId::kBandOfTheEternalSage) !=
      equipped_ring_ids.end()) {
    auras.band_of_the_eternal_sage = std::make_shared<BandOfTheEternalSageAura>(*this);
  }
  if (std::find(equipped_ring_ids.begin(), equipped_ring_ids.end(), ItemId::kWrathOfCenarius) !=
      equipped_ring_ids.end()) {
    auras.wrath_of_cenarius = std::make_shared<WrathOfCenariusAura>(*this);
  }
  if (sets.t4 >= 2) {
    auras.flameshadow = std::make_shared<FlameshadowAura>(*this);
    auras.shadowflame = std::make_shared<ShadowflameAura>(*this);
  }
  if (sets.spellstrike >= 2) {
    auras.spellstrike = std::make_shared<SpellstrikeAura>(*this);
  }
  if (sets.mana_etched >= 4) {
    auras.mana_etched_4_set = std::make_shared<ManaEtched4SetAura>(*this);
  }

  // Spells
  spells.life_tap = std::make_shared<LifeTap>(*this);
  if (settings.fight_type == EmbindConstant::kAoe) {
    spells.seed_of_corruption = std::make_shared<SeedOfCorruption>(*this);
  } else {
    if (settings.has_shadow_bolt || talents.nightfall > 0 || settings.rotation_option == EmbindConstant::kSimChooses) {
      spells.shadow_bolt = std::make_shared<ShadowBolt>(*this);
    }
    if (settings.has_incinerate || settings.rotation_option == EmbindConstant::kSimChooses) {
      spells.incinerate = std::make_shared<Incinerate>(*this);
    }
    if (settings.has_searing_pain || settings.rotation_option == EmbindConstant::kSimChooses) {
      spells.searing_pain = std::make_shared<SearingPain>(*this);
    }
    if (settings.has_death_coil || settings.rotation_option == EmbindConstant::kSimChooses) {
      spells.death_coil = std::make_shared<DeathCoil>(*this);
    }
    if (talents.conflagrate == 1 &&
        (settings.has_conflagrate || settings.rotation_option == EmbindConstant::kSimChooses)) {
      spells.conflagrate = std::make_shared<Conflagrate>(*this);
    }
    if (talents.shadowburn == 1 &&
        (settings.has_shadow_burn || settings.rotation_option == EmbindConstant::kSimChooses)) {
      spells.shadowburn = std::make_shared<Shadowburn>(*this);
    }
    if (talents.shadowfury == 1 && (settings.has_shadowfury || settings.rotation_option == EmbindConstant::kSimChooses)) {
      spells.shadowfury = std::make_shared<Shadowfury>(*this);
    }
    if (auras.corruption != nullptr) {
      spells.corruption = std::make_shared<Corruption>(*this, nullptr, auras.corruption);
      auras.corruption->parent_spell = spells.corruption;
    }
    if (auras.unstable_affliction != nullptr) {
      spells.unstable_affliction = std::make_shared<UnstableAffliction>(*this, nullptr, auras.unstable_affliction);
      auras.unstable_affliction->parent_spell = spells.unstable_affliction;
    }
    if (auras.siphon_life != nullptr) {
      spells.siphon_life = std::make_shared<SiphonLife>(*this, nullptr, auras.siphon_life);
      auras.siphon_life->parent_spell = spells.siphon_life;
    }
    if (auras.immolate != nullptr) {
      spells.immolate = std::make_shared<Immolate>(*this, nullptr, auras.immolate);
      auras.immolate->parent_spell = spells.immolate;
    }
    if (auras.curse_of_agony != nullptr || auras.curse_of_doom != nullptr) {
      spells.curse_of_agony = std::make_shared<CurseOfAgony>(*this, nullptr, auras.curse_of_agony);
      auras.curse_of_agony->parent_spell = spells.curse_of_agony;
    }
    if (auras.curse_of_the_elements != nullptr) {
      spells.curse_of_the_elements = std::make_shared<CurseOfTheElements>(*this, auras.curse_of_the_elements);
    }
    if (auras.curse_of_recklessness != nullptr) {
      spells.curse_of_recklessness = std::make_shared<CurseOfRecklessness>(*this, auras.curse_of_recklessness);
    }
    if (auras.curse_of_doom != nullptr) {
      spells.curse_of_doom = std::make_shared<CurseOfDoom>(*this, nullptr, auras.curse_of_doom);
      auras.curse_of_doom->parent_spell = spells.curse_of_doom;
    }
    if (auras.amplify_curse != nullptr) {
      spells.amplify_curse = std::make_shared<AmplifyCurse>(*this, auras.amplify_curse);
    }
  }
  if (auras.improved_shadow_bolt != nullptr) {
    spells.improved_shadow_bolt = std::make_shared<ImprovedShadowBolt>(*this, auras.improved_shadow_bolt);
  }
  if (auras.mana_tide_totem != nullptr) {
    spells.mana_tide_totem = std::make_shared<ManaTideTotem>(*this, auras.mana_tide_totem);
  }
  if (auras.chipped_power_core != nullptr) {
    spells.chipped_power_core = std::make_shared<ChippedPowerCore>(*this, auras.chipped_power_core);
  }
  if (auras.cracked_power_core != nullptr) {
    spells.cracked_power_core = std::make_shared<CrackedPowerCore>(*this, auras.cracked_power_core);
  }
  if (selected_auras.super_mana_potion) {
    spells.super_mana_potion = std::make_shared<SuperManaPotion>(*this);
  }
  if (selected_auras.demonic_rune) {
    spells.demonic_rune = std::make_shared<DemonicRune>(*this);
  }
  if (talents.dark_pact == 1 && (settings.has_dark_pact || settings.rotation_option == EmbindConstant::kSimChooses)) {
    spells.dark_pact = std::make_shared<DarkPact>(*this);
  }
  if (auras.destruction_potion != nullptr) {
    spells.destruction_potion = std::make_shared<DestructionPotion>(*this, auras.destruction_potion);
  }
  if (auras.flame_cap != nullptr) {
    spells.flame_cap = std::make_shared<FlameCap>(*this, auras.flame_cap);
  }
  if (auras.blood_fury != nullptr) {
    spells.blood_fury = std::make_shared<BloodFury>(*this, auras.blood_fury);
  }
  if (auras.drums_of_battle != nullptr) {
    spells.drums_of_battle = std::make_shared<DrumsOfBattle>(*this, auras.drums_of_battle);
  } else if (auras.drums_of_war != nullptr) {
    spells.drums_of_war = std::make_shared<DrumsOfWar>(*this, auras.drums_of_war);
  } else if (auras.drums_of_restoration != nullptr) {
    spells.drums_of_restoration = std::make_shared<DrumsOfRestoration>(*this, auras.drums_of_restoration);
  }
  if (auras.blade_of_wizardry != nullptr) {
    spells.blade_of_wizardry = std::make_shared<BladeOfWizardry>(*this, auras.blade_of_wizardry);
  }
  if (auras.shattered_sun_pendant_of_acumen_aldor != nullptr) {
    spells.shattered_sun_pendant_of_acumen_aldor =
        std::make_shared<ShatteredSunPendantOfAcumenAldor>(*this, auras.shattered_sun_pendant_of_acumen_aldor);
  }
  if (auras.robe_of_the_elder_scribes != nullptr) {
    spells.robe_of_the_elder_scribes = std::make_shared<RobeOfTheElderScribes>(*this, auras.robe_of_the_elder_scribes);
  }
  if (auras.mystical_skyfire_diamond != nullptr) {
    spells.mystical_skyfire_diamond = std::make_shared<MysticalSkyfireDiamond>(*this, auras.mystical_skyfire_diamond);
  }
  if (settings.meta_gem_id == ItemId::kInsightfulEarthstormDiamond) {
    spells.insightful_earthstorm_diamond = std::make_shared<InsightfulEarthstormDiamond>(*this);
  }
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kTimbalsFocusingCrystal) !=
      equipped_trinket_ids.end()) {
    spells.timbals_focusing_crystal = std::make_shared<TimbalsFocusingCrystal>(*this);
  }
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kMarkOfDefiance) !=
      equipped_trinket_ids.end()) {
    spells.mark_of_defiance = std::make_shared<MarkOfDefiance>(*this);
  }
  if (auras.the_lightning_capacitor != nullptr) {
    spells.the_lightning_capacitor = std::make_shared<TheLightningCapacitor>(*this);
  }
  if (auras.quagmirrans_eye != nullptr) {
    spells.quagmirrans_eye = std::make_shared<QuagmirransEye>(*this, auras.quagmirrans_eye);
  }
  if (auras.shiffars_nexus_horn != nullptr) {
    spells.shiffars_nexus_horn = std::make_shared<ShiffarsNexusHorn>(*this, auras.shiffars_nexus_horn);
  }
  if (auras.sextant_of_unstable_currents != nullptr) {
    spells.sextant_of_unstable_currents =
        std::make_shared<SextantOfUnstableCurrents>(*this, auras.sextant_of_unstable_currents);
  }
  if (auras.band_of_the_eternal_sage != nullptr) {
    spells.band_of_the_eternal_sage = std::make_shared<BandOfTheEternalSage>(*this, auras.band_of_the_eternal_sage);
  }
  if (selected_auras.judgement_of_wisdom) {
    spells.judgement_of_wisdom = std::make_shared<JudgementOfWisdom>(*this);
  }
  if (auras.flameshadow != nullptr) {
    spells.flameshadow = std::make_shared<Flameshadow>(*this, auras.flameshadow);
  }
  if (auras.shadowflame != nullptr) {
    spells.shadowflame = std::make_shared<Shadowflame>(*this, auras.shadowflame);
  }
  if (auras.spellstrike != nullptr) {
    spells.spellstrike = std::make_shared<Spellstrike>(*this, auras.spellstrike);
  }
  if (auras.mana_etched_4_set != nullptr) {
    spells.mana_etched_4_set = std::make_shared<ManaEtched4Set>(*this, auras.mana_etched_4_set);
  }
  if (auras.ashtongue_talisman_of_shadows != nullptr) {
    spells.ashtongue_talisman_of_shadows =
        std::make_shared<AshtongueTalismanOfShadows>(*this, auras.ashtongue_talisman_of_shadows);
  }
  if (auras.wrath_of_cenarius != nullptr) {
    spells.wrath_of_cenarius = std::make_shared<WrathOfCenarius>(*this, auras.wrath_of_cenarius);
  }
  if (auras.darkmoon_card_crusade != nullptr) {
    spells.darkmoon_card_crusade = std::make_shared<DarkmoonCardCrusade>(*this, auras.darkmoon_card_crusade);
  }
  if (auras.power_infusion != nullptr) {
    for (int i = 0; i < settings.power_infusion_amount; i++) {
      spells.power_infusion.push_back(std::make_shared<PowerInfusion>(*this, auras.power_infusion));
    }
  }
  if (auras.bloodlust != nullptr) {
    for (int i = 0; i < settings.bloodlust_amount; i++) {
      spells.bloodlust.push_back(std::make_shared<Bloodlust>(*this, auras.bloodlust));
    }
  }
  if (auras.innervate != nullptr) {
    for (int i = 0; i < settings.innervate_amount; i++) {
      spells.innervate.push_back(std::make_shared<Innervate>(*this, auras.innervate));
    }
  }

  // Set the filler property
  if (settings.has_incinerate) {
    filler = spells.incinerate;
  } else if (settings.has_searing_pain) {
    filler = spells.searing_pain;
  } else {
    filler = spells.shadow_bolt;
  }

  // Set the curseSpell and curseAura properties
  if (spells.curse_of_the_elements != nullptr) {
    curse_spell = spells.curse_of_the_elements;
    curse_aura = auras.curse_of_the_elements;
  } else if (spells.curse_of_recklessness != nullptr) {
    curse_spell = spells.curse_of_recklessness;
    curse_aura = auras.curse_of_recklessness;
  } else if (spells.curse_of_doom != nullptr) {
    curse_spell = spells.curse_of_doom;
  } else if (spells.curse_of_agony != nullptr) {
    curse_spell = spells.curse_of_agony;
  }

  SendPlayerInfoToCombatLog();
}

void Player::Reset() {
  Entity::Reset();
  stats.mana = stats.max_mana;
  iteration_damage = 0;
  power_infusions_ready = settings.power_infusion_amount;

  for (auto& trinket : trinkets) {
    trinket.Reset();
  }
}

void Player::EndAuras() {
  Entity::EndAuras();

  for (auto& trinket : trinkets) {
    if (trinket.active) {
      trinket.Fade();
    }
  }

  for (const auto& dot : dot_list) {
    if (dot->active) {
      dot->Fade();
    }
  }
}

double Player::GetHastePercent() {
  auto haste_percent = stats.spell_haste_percent;

  // If both Bloodlust and Power Infusion are active then remove the 20% PI
  // bonus since they don't stack
  if (auras.bloodlust != nullptr && auras.power_infusion != nullptr && auras.bloodlust->active &&
      auras.power_infusion->active) {
    for (auto& stat : auras.power_infusion->stats) {
      if (stat.name == StatName::kSpellHastePercent) {
        haste_percent /= stat.value;
      }
    }
  }

  return haste_percent * (1 + stats.spell_haste_rating / StatConstant::kHasteRatingPerPercent / 100.0);
}

double Player::GetSpellPower(const bool kDealingDamage, const SpellSchool kSchool) {
  auto spell_power = stats.spell_power;

  if (pet != nullptr && talents.demonic_knowledge > 0) {
    spell_power += (pet->GetStamina() + pet->GetIntellect()) * 0.04 * talents.demonic_knowledge;
  }

  // If Mark of the Champion is equipped and the player isn't dealing damage then remove the spell power so it doesn't
  // affect spells like Life Tap
  if (!kDealingDamage &&
      (items.trinket_1 == ItemId::kMarkOfTheChampion || items.trinket_2 == ItemId::kMarkOfTheChampion)) {
    spell_power -= 85;
  }

  if (sets.spellfire == 3) {
    spell_power += GetIntellect() * 0.07;
  }

  if (kSchool == SpellSchool::kShadow) {
    spell_power += stats.shadow_power;
  } else if (kSchool == SpellSchool::kFire) {
    spell_power += stats.fire_power;
  }

  // Spell Power from spirit if player has Improved Divine Spirit buffed
  if (selected_auras.prayer_of_spirit && settings.improved_divine_spirit > 0) {
    spell_power += GetSpirit() * (0 + static_cast<double>(settings.improved_divine_spirit) / 20.0);
  }

  return spell_power;
}

double Player::GetSpellCritChance(const SpellType kSpellType) {
  auto crit_chance = stats.spell_crit_chance + GetIntellect() * StatConstant::kCritChancePerIntellect +
                     stats.spell_crit_rating / StatConstant::kCritRatingPerPercent;

  if (kSpellType != SpellType::kDestruction) {
    crit_chance -= talents.devastation;
  }

  return crit_chance;
}

int Player::GetRand() { return rng.Range(0, 100 * kFloatNumberMultiplier); }

bool Player::RollRng(const double kChance) { return GetRand() <= kChance * kFloatNumberMultiplier; }

void Player::UseCooldowns(const double kFightTimeRemaining) {
  // Only use PI if Bloodlust isn't selected or if Bloodlust isn't active since they don't stack, or if there are enough
  // Power Infusions available to last until the end of the fight for the mana cost reduction
  if (!spells.power_infusion.empty() && !auras.power_infusion->active &&
      (spells.bloodlust.empty() || !auras.bloodlust->active ||
       power_infusions_ready * auras.power_infusion->duration >= kFightTimeRemaining)) {
    for (const auto& pi : spells.power_infusion) {
      if (pi->Ready()) {
        pi->StartCast();
        break;
      }
    }
  }

  if (stats.mana / stats.max_mana <= 0.5 && !spells.innervate.empty() && !auras.innervate->active) {
    for (const auto& innervate : spells.innervate) {
      if (innervate->Ready()) {
        innervate->StartCast();
        break;
      }
    }
  }

  if (spells.chipped_power_core != nullptr && spells.chipped_power_core->Ready()) {
    spells.chipped_power_core->StartCast();
  } else if (spells.cracked_power_core != nullptr && spells.cracked_power_core->Ready()) {
    spells.cracked_power_core->StartCast();
  }

  if (spells.destruction_potion != nullptr && spells.destruction_potion->Ready()) {
    spells.destruction_potion->StartCast();
  }

  if (spells.flame_cap != nullptr && spells.flame_cap->Ready()) {
    spells.flame_cap->StartCast();
  }

  if (spells.blood_fury != nullptr && spells.blood_fury->Ready()) {
    spells.blood_fury->StartCast();
  }

  for (auto i = 0; i < trinkets.size(); i++) {
    if (trinkets[i].Ready()) {
      trinkets[i].Use();
      // Set the other on-use trinket (if another is equipped) on cooldown for
      // the duration of the trinket just used if the trinkets share cooldown
      if (const auto kOtherTrinketSlot = i == 1 ? 0 : 1; trinkets.size() > kOtherTrinketSlot 
                                                         && trinkets[kOtherTrinketSlot].shares_cooldown && trinkets[i].shares_cooldown) {
        trinkets[kOtherTrinketSlot].cooldown_remaining =
            std::max(trinkets[kOtherTrinketSlot].cooldown_remaining, static_cast<double>(trinkets[i].duration));
      }
    }
  }
}

// TODO remove this is_dot parameter
double Player::GetDamageModifier(Spell& spell, const bool kIsDot) {
  auto additive_modifier = 1.0;
  const auto multiplicative_modifier = GetMultiplicativeDamageModifier(spell, kIsDot);

  if (sets.t6 >= 4 && (spell.name == SpellName::kShadowBolt || spell.name == SpellName::kIncinerate)) {
    additive_modifier += 0.06;
  }

  if (sets.t3 >= 4 && spell.name == SpellName::kCorruption) {
    additive_modifier += 0.12;
  }

  if (spell.spell_school == SpellSchool::kShadow && spell.name != SpellName::kCurseOfDoom) {
    additive_modifier += 0.02 * talents.shadow_mastery;
  }

  if (spell.name == SpellName::kCurseOfAgony) {
    additive_modifier += 0.05 * talents.improved_curse_of_agony;
  }

  if (spell.name == SpellName::kCurseOfAgony || spell.name == SpellName::kCorruption ||
      spell.name == SpellName::kSeedOfCorruption) {
    additive_modifier += 0.01 * talents.contagion;
  }

  if (spell.spell_school == SpellSchool::kFire) {
    additive_modifier += 0.02 * talents.emberstorm;

    if (spell.name == SpellName::kImmolate && !kIsDot) {
      additive_modifier += 0.05 * talents.improved_immolate;
    }
  }

  return additive_modifier * multiplicative_modifier;
}

void Player::CastLifeTapOrDarkPact() const {
  if (spells.dark_pact != nullptr && spells.dark_pact->Ready()) {
    spells.dark_pact->StartCast();
  } else {
    spells.life_tap->StartCast();
  }
}

void Player::ThrowError(const std::string& kError) const {
  SendCombatLogEntries();
  ErrorCallback(kError.c_str());
  throw std::runtime_error(kError);
}

void Player::SendCombatLogEntries() const {
  for (const auto& kValue : combat_log_entries) {
    CombatLogUpdate(kValue.c_str());
  }
}

double Player::FindTimeUntilNextAction() {
  auto time = Entity::FindTimeUntilNextAction();

  if (pet != nullptr) {

    if (const double kTimeUntilNextPetAction = pet->FindTimeUntilNextAction(); kTimeUntilNextPetAction > 0 && kTimeUntilNextPetAction < time) {
      time = kTimeUntilNextPetAction;
    }
  }

  for (const auto& trinket : trinkets) {
    if (trinket.active && trinket.duration_remaining < time) {
      time = trinket.duration_remaining;
    }
    if (trinket.cooldown_remaining > 0 && trinket.cooldown_remaining < time) {
      time = trinket.cooldown_remaining;
    }
  }

  return time;
}

void Player::Tick(const double kTime) {
  Entity::Tick(kTime);

  for (auto& trinket : trinkets) {
    trinket.Tick(kTime);
  }

  if (mp5_timer_remaining <= 0) {
    mp5_timer_remaining = 5;

    if (stats.mp5 > 0 || five_second_rule_timer_remaining <= 0 ||
        auras.innervate != nullptr && auras.innervate->active) {
      const bool kInnervateIsActive = auras.innervate != nullptr && auras.innervate->active;
      const double kCurrentPlayerMana = stats.mana;

      // MP5
      if (stats.mp5 > 0) {
        stats.mana += stats.mp5;
      }
      // Spirit mana regen
      if (kInnervateIsActive || five_second_rule_timer_remaining <= 0) {
        // Formula from https://wowwiki-archive.fandom.com/wiki/Spirit?oldid=1572910
        auto mp5_from_spirit = 5 * (0.001 + std::sqrt(GetIntellect()) * GetSpirit() * 0.009327);

        if (kInnervateIsActive) {
          mp5_from_spirit *= 4;
        }

        stats.mana += mp5_from_spirit;
      }

      if (stats.mana > stats.max_mana) {
        stats.mana = stats.max_mana;
      }

      const double kManaGained = stats.mana - kCurrentPlayerMana;
      if (recording_combat_log_breakdown) {
        combat_log_breakdown.at(StatName::kMp5)->casts++;
        combat_log_breakdown.at(StatName::kMp5)->iteration_mana_gain += kManaGained;
      }

      if (ShouldWriteToCombatLog()) {
        CombatLog("Player gains " + DoubleToString(kManaGained) + " mana from MP5 (" +
                  DoubleToString(kCurrentPlayerMana) + " -> " + DoubleToString(stats.mana) + ")");
      }
    }
  }
}

void Player::SendPlayerInfoToCombatLog() {
  combat_log_entries.push_back("---------------- Player stats ----------------");
  combat_log_entries.push_back("Health: " + DoubleToString(round(stats.health)));
  combat_log_entries.push_back("Mana: " + DoubleToString(round(stats.max_mana)));
  combat_log_entries.push_back("Stamina: " + DoubleToString(round(GetStamina())));
  combat_log_entries.push_back("Intellect: " + DoubleToString(round(GetIntellect())));
  combat_log_entries.push_back("Spell Power: " + DoubleToString(round(GetSpellPower(true))));
  combat_log_entries.push_back("Shadow Power: " + DoubleToString(stats.shadow_power));
  combat_log_entries.push_back("Fire Power: " + DoubleToString(stats.fire_power));
  combat_log_entries.push_back(
      "Crit Chance: " + DoubleToString(round(GetSpellCritChance(SpellType::kDestruction) * 100) / 100, 2) + "%");
  combat_log_entries.push_back(
      "Hit Chance: " + DoubleToString(std::min(16.0, round(stats.extra_spell_hit_chance * 100) / 100), 2) + "%");
  combat_log_entries.push_back(
      "Haste: " +
      DoubleToString(round(stats.spell_haste_rating / StatConstant::kHasteRatingPerPercent * 100) / 100, 2) + "%");
  combat_log_entries.push_back(
      "Shadow Modifier: " + DoubleToString(stats.shadow_modifier * (1 + 0.02 * talents.shadow_mastery) * 100, 2) + "%");
  combat_log_entries.push_back(
      "Fire Modifier: " + DoubleToString(stats.fire_modifier * (1 + 0.02 * talents.emberstorm) * 100, 2) + "%");
  combat_log_entries.push_back("MP5: " + DoubleToString(stats.mp5));
  combat_log_entries.push_back("Spell Penetration: " + DoubleToString(stats.spell_penetration));
  if (pet != nullptr) {
    combat_log_entries.push_back("---------------- Pet stats ----------------");
    combat_log_entries.push_back("Stamina: " + DoubleToString(pet->GetStamina()));
    combat_log_entries.push_back("Intellect: " + DoubleToString(pet->GetIntellect()));
    combat_log_entries.push_back("Strength: " + DoubleToString(pet->GetStrength()));
    combat_log_entries.push_back("Agility: " + DoubleToString(pet->GetAgility()));
    combat_log_entries.push_back("Spirit: " + DoubleToString(pet->GetSpirit()));
    combat_log_entries.push_back("Attack Power: " + DoubleToString(round(pet->GetAttackPower())));
    combat_log_entries.push_back("Spell Power: " + DoubleToString(pet->GetSpellPower(false, SpellSchool::kNoSchool)));
    combat_log_entries.push_back("Mana: " + DoubleToString(pet->CalculateMaxMana()));
    combat_log_entries.push_back("MP5: " + DoubleToString(pet->stats.mp5));
    if (pet->pet_type == PetType::kMelee) {
      combat_log_entries.push_back(
          "Physical Hit Chance: " + DoubleToString(round(pet->stats.melee_hit_chance * 100) / 100.0, 2) + "%");
      combat_log_entries.push_back(
          "Physical Crit Chance: " + DoubleToString(round(pet->GetMeleeCritChance() * 100) / 100.0, 2) + "% (" +
          DoubleToString(StatConstant::kMeleeCritChanceSuppression, 2) + "% Crit Suppression Applied)");
      combat_log_entries.push_back("Glancing Blow Chance: " + DoubleToString(pet->glancing_blow_chance, 2) + "%");
      combat_log_entries.push_back(
          "Attack Power Modifier: " + DoubleToString(pet->stats.attack_power_modifier * 100, 2) + "%");
    }
    if (pet->pet_name == PetName::kImp || pet->pet_name == PetName::kSuccubus) {
      combat_log_entries.push_back(
          "Spell Hit Chance: " +
          DoubleToString(round(pet->GetSpellHitChance(SpellType::kNoSpellType) * 100) / 100.0, 2) + "%");
      combat_log_entries.push_back(
          "Spell Crit Chance: " +
          DoubleToString(round(pet->GetSpellCritChance(SpellType::kNoSpellType) * 100) / 100.0, 2) + "%");
    }
    combat_log_entries.push_back(
        "Damage Modifier: " +
        DoubleToString(round(pet->GetDamageModifier(
                                 *(pet->pet_name == PetName::kImp ? pet->spells.firebolt : pet->spells.melee), false) *
                             10000) /
                       100,
                       2) +
        "%");
  }
  combat_log_entries.push_back("---------------- Enemy stats ----------------");
  combat_log_entries.push_back("Level: " + std::to_string(settings.enemy_level));
  combat_log_entries.push_back("Shadow Resistance: " + std::to_string(std::max(settings.enemy_shadow_resist,
                                                                               enemy_level_difference_resistance)));
  combat_log_entries.push_back("Fire Resistance: " +
                               std::to_string(std::max(settings.enemy_fire_resist, enemy_level_difference_resistance)));
  if (pet != nullptr && pet->pet_name != PetName::kImp) {
    combat_log_entries.push_back("Dodge Chance: " + DoubleToString(StatConstant::kBaseEnemyDodgeChance, 2) + "%");
    combat_log_entries.push_back("Armor: " + std::to_string(settings.enemy_armor));
    combat_log_entries.push_back(
        "Damage Reduction From Armor: " +
        DoubleToString(round((1 - pet->enemy_damage_reduction_from_armor) * 10000) / 100.0, 2) + "%");
  }
  combat_log_entries.push_back("---------------------------------------------");
}