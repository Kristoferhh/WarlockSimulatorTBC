#include "player.h"

#include <math.h>

#include "../bindings.h"
#include "../common.h"
#include "../spell/damage_over_time.h"
#include "../spell/life_tap.h"
#include "../spell/mana_over_time.h"
#include "../spell/mana_potion.h"
#include "../spell/spell.h"

Player::Player(PlayerSettings& player_settings)
    : selected_auras(player_settings.auras),
      talents(player_settings.talents),
      sets(player_settings.sets),
      stats(player_settings.stats),
      items(player_settings.items),
      settings(player_settings),
      spells(PlayerSpells()),
      auras(PlayerAuras()),
      cast_time_remaining(0),
      gcd_remaining(0),
      recording_combat_log_breakdown(settings.recording_combat_log_breakdown && settings.equipped_item_simulation),
      entity_type(EntityType::kPlayer) {
  // I don't know if this formula only works for bosses or not, so for the
  // moment I'm only using it for targets 3+ levels above.
  const double enemy_base_resistance = settings.enemy_level >= (kLevel + 3) ? (6 * kLevel * 5) / 75.0 : 0;
  settings.enemy_shadow_resist = std::max(static_cast<double>(settings.enemy_shadow_resist), enemy_base_resistance);
  settings.enemy_fire_resist = std::max(static_cast<double>(settings.enemy_fire_resist), enemy_base_resistance);

  if (recording_combat_log_breakdown) {
    combat_log_breakdown.insert({"Mp5", std::make_unique<CombatLogBreakdown>("Mp5")});
  }

  if (settings.custom_stat == EmbindConstant::kStamina)
    custom_stat = "stamina";
  else if (settings.custom_stat == EmbindConstant::kIntellect)
    custom_stat = "intellect";
  else if (settings.custom_stat == EmbindConstant::kSpirit)
    custom_stat = "spirit";
  else if (settings.custom_stat == EmbindConstant::kSpellPower)
    custom_stat = "spellPower";
  else if (settings.custom_stat == EmbindConstant::kShadowPower)
    custom_stat = "shadowPower";
  else if (settings.custom_stat == EmbindConstant::kFirePower)
    custom_stat = "firePower";
  else if (settings.custom_stat == EmbindConstant::kCritRating)
    custom_stat = "critRating";
  else if (settings.custom_stat == EmbindConstant::kHitRating)
    custom_stat = "hitRating";
  else if (settings.custom_stat == EmbindConstant::kHasteRating)
    custom_stat = "hasteRating";
  else if (settings.custom_stat == EmbindConstant::kMp5)
    custom_stat = "mp5";
  else
    custom_stat = "normal";

  // Crit chance
  if (selected_auras.atiesh_mage) {
    stats.spell_crit_rating += 28 * settings.mage_atiesh_amount;
  }
  stats.spell_crit_chance = kBaseCritChancePercent + talents.devastation + talents.backlash + talents.demonic_tactics;
  if (selected_auras.moonkin_aura) {
    stats.spell_crit_chance += 5;
  }
  if (selected_auras.judgement_of_the_crusader) {
    stats.spell_crit_chance += 3;
  }
  if (selected_auras.totem_of_wrath) {
    stats.spell_crit_chance += (3 * settings.totem_of_wrath_amount);
  }
  if (selected_auras.chain_of_the_twilight_owl) {
    stats.spell_crit_chance += 2;
  }

  // Hit chance
  if (sets.mana_etched >= 2) {
    stats.spell_hit_rating += 35;
  }
  stats.extra_spell_hit_chance = stats.spell_hit_rating / kHitRatingPerPercent;
  if (selected_auras.totem_of_wrath) {
    stats.extra_spell_hit_chance += (3 * settings.totem_of_wrath_amount);
  }
  if (selected_auras.inspiring_presence) {
    stats.extra_spell_hit_chance += 1;
  }
  stats.spell_hit_chance = GetBaseHitChance(kLevel, settings.enemy_level);

  // Add bonus damage % from Demonic Sacrifice
  if (talents.demonic_sacrifice == 1 && settings.sacrificing_pet) {
    if (settings.selected_pet == EmbindConstant::kImp) {
      stats.fire_modifier *= 1.15;
    } else if (settings.selected_pet == EmbindConstant::kSuccubus) {
      stats.shadow_modifier *= 1.15;
    } else if (settings.selected_pet == EmbindConstant::kFelguard) {
      stats.shadow_modifier *= 1.1;
    }
    // todo add felhunter mana regen maybe
  } else {
    // Add damage % multiplier from Master Demonologist and Soul Link
    if (talents.soul_link == 1) {
      stats.shadow_modifier *= 1.05;
      stats.fire_modifier *= 1.05;
    }
    if (talents.master_demonologist > 0) {
      if (settings.selected_pet == EmbindConstant::kSuccubus) {
        stats.shadow_modifier *= (1 + 0.02 * talents.master_demonologist);
        stats.fire_modifier *= (1 + 0.02 * talents.master_demonologist);
      } else if (settings.selected_pet == EmbindConstant::kFelguard) {
        stats.shadow_modifier *= (1 + 0.01 * talents.master_demonologist);
        stats.fire_modifier *= (1 + 0.01 * talents.master_demonologist);
      }
    }
  }
  // Shadow Mastery
  stats.shadow_modifier *= (1 + (0.02 * talents.shadow_mastery));
  // Ferocious Inspiration
  if (selected_auras.ferocious_inspiration) {
    stats.shadow_modifier *= std::pow(1.03, settings.ferocious_inspiration_amount);
    stats.fire_modifier *= std::pow(1.03, settings.ferocious_inspiration_amount);
  }
  // Add % dmg modifiers from Curse of the Elements + Malediction
  if (selected_auras.curse_of_the_elements) {
    stats.shadow_modifier *= 1.1 + (0.01 * settings.improved_curse_of_the_elements);
    stats.fire_modifier *= 1.1 + (0.01 * settings.improved_curse_of_the_elements);
  }
  // Add fire dmg % from Emberstorm
  if (talents.emberstorm > 0) {
    stats.fire_modifier *= 1 + (0.02 * talents.emberstorm);
  }
  // Add spell power from Fel Armor
  if (selected_auras.fel_armor) {
    stats.spell_power += 100 * (0 + 0.1 * talents.demonic_aegis);
  }
  // If using a custom isb uptime % then just add to the shadow modifier % (this
  // assumes 5/5 ISB giving 20% shadow Damage)
  if (settings.using_custom_isb_uptime) {
    stats.shadow_modifier *= 1.0 + GetCustomImprovedShadowBoltDamageModifier();
  }
  stats.spirit_modifier *= (1 - (0.01 * talents.demonic_embrace));
  // Elemental shaman t4 2pc bonus
  if (selected_auras.wrath_of_air_totem && settings.has_elemental_shaman_t4_bonus) {
    stats.spell_power += 20;
  }
  // Add extra stamina from Blood Pact from Improved Imp
  if (selected_auras.blood_pact) {
    int improved_imp_points = settings.improved_imp;

    if (settings.selected_pet == EmbindConstant::kImp &&
        (!settings.sacrificing_pet || talents.demonic_sacrifice == 0) && talents.improved_imp > improved_imp_points) {
      improved_imp_points = talents.improved_imp;
    }

    stats.stamina += 70 * 0.1 * improved_imp_points;
  }
  // Add stamina from Demonic Embrace
  stats.stamina_modifier *= 1 + (0.03 * talents.demonic_embrace);
  // Add mp5 from Vampiric Touch (add 25% instead of 5% since we're adding it to
  // the mana per 5 seconds variable)
  if (selected_auras.vampiric_touch) {
    stats.mp5 += settings.shadow_priest_dps * 0.25;
  }
  if (selected_auras.atiesh_warlock) {
    stats.spell_power += 33 * settings.warlock_atiesh_amount;
  }
  if (sets.twin_stars == 2) {
    stats.spell_power += 15;
  }

  // Enemy Armor Reduction
  if (selected_auras.faerie_fire) {
    settings.enemy_armor -= 610;
  }
  if ((selected_auras.sunder_armor && selected_auras.expose_armor && settings.improved_expose_armor == 2) ||
      (selected_auras.expose_armor && !selected_auras.sunder_armor)) {
    settings.enemy_armor -= 2050 * (1 + 0.25 * settings.improved_expose_armor);
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
  stats.health =
      (stats.health + GetStamina() * kHealthPerStamina) * (1 + (0.01 * static_cast<double>(talents.fel_stamina)));
  stats.max_mana =
      (stats.mana + GetIntellect() * kManaPerIntellect) * (1 + (0.01 * static_cast<double>(talents.fel_intellect)));
}

void Player::Initialize() {
  demonic_knowledge_spell_power = 0;
  if (!settings.sacrificing_pet || talents.demonic_sacrifice == 0) {
    if (settings.selected_pet == EmbindConstant::kImp) {
      pet = std::make_shared<Imp>(*this);
    } else if (settings.selected_pet == EmbindConstant::kSuccubus) {
      pet = std::make_shared<Succubus>(*this);
    } else if (settings.selected_pet == EmbindConstant::kFelguard) {
      pet = std::make_shared<Felguard>(*this);
    }
    if (pet != NULL) {
      pet->Initialize();
    }
  }

  std::vector<int> equipped_trinket_ids{items.trinket_1, items.trinket_2};
  std::vector<int> equipped_ring_ids{items.ring_1, items.ring_2};
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kSkullOfGuldan) !=
      equipped_trinket_ids.end())
    trinkets.push_back(SkullOfGuldan(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kShiftingNaaruSliver) !=
      equipped_trinket_ids.end())
    trinkets.push_back(ShiftingNaaruSliver(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kHexShrunkenHead) !=
      equipped_trinket_ids.end())
    trinkets.push_back(HexShrunkenHead(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kIconOfTheSilverCrescent) !=
      equipped_trinket_ids.end())
    trinkets.push_back(IconOfTheSilverCrescent(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kScryersBloodgem) !=
      equipped_trinket_ids.end())
    trinkets.push_back(ScryersBloodgem(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kRestrainedEssenceOfSapphiron) !=
      equipped_trinket_ids.end())
    trinkets.push_back(RestrainedEssenceOfSapphiron(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kXirisGift) !=
      equipped_trinket_ids.end())
    trinkets.push_back(XirisGift(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kAncientCrystalTalisman) !=
      equipped_trinket_ids.end())
    trinkets.push_back(AncientCrystalTalisman(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kArcanistsStone) !=
      equipped_trinket_ids.end())
    trinkets.push_back(ArcanistsStone(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kTerokkarTabletOfVim) !=
      equipped_trinket_ids.end())
    trinkets.push_back(TerokkarTabletOfVim(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kVengeanceOfTheIllidari) !=
      equipped_trinket_ids.end())
    trinkets.push_back(VengeanceOfTheIllidari(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kFigurineLivingRubySerpent) !=
      equipped_trinket_ids.end())
    trinkets.push_back(FigurineLivingRubySerpent(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kEssenceOfTheMartyr) !=
      equipped_trinket_ids.end())
    trinkets.push_back(EssenceOfTheMartyr(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kStarkillersBauble) !=
      equipped_trinket_ids.end())
    trinkets.push_back(StarkillersBauble(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kDarkIronSmokingPipe) !=
      equipped_trinket_ids.end())
    trinkets.push_back(DarkIronSmokingPipe(*this));
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kHazzarahsCharmOfDestruction) !=
      equipped_trinket_ids.end())
    trinkets.push_back(HazzarahsCharmOfDestruction(*this));

  // Auras
  if (settings.fight_type == EmbindConstant::kSingleTarget) {
    if (talents.improved_shadow_bolt > 0) auras.improved_shadow_bolt = std::make_unique<ImprovedShadowBoltAura>(*this);
    if (settings.has_corruption || settings.rotation_option == EmbindConstant::kSimChooses)
      auras.corruption = std::make_unique<CorruptionDot>(*this);
    if (talents.unstable_affliction == 1 &&
        (settings.has_unstable_affliction || settings.rotation_option == EmbindConstant::kSimChooses))
      auras.unstable_affliction = std::make_unique<UnstableAfflictionDot>(*this);
    if (talents.siphon_life == 1 &&
        (settings.has_siphon_life || settings.rotation_option == EmbindConstant::kSimChooses))
      auras.siphon_life = std::make_unique<SiphonLifeDot>(*this);
    if (settings.has_immolate || settings.rotation_option == EmbindConstant::kSimChooses)
      auras.immolate = std::make_unique<ImmolateDot>(*this);
    if (settings.has_curse_of_agony || settings.has_curse_of_doom)
      auras.curse_of_agony = std::make_unique<CurseOfAgonyDot>(*this);
    if (settings.has_curse_of_the_elements)
      auras.curse_of_the_elements = std::make_unique<CurseOfTheElementsAura>(*this);
    if (settings.has_curse_of_recklessness)
      auras.curse_of_recklessness = std::make_unique<CurseOfRecklessnessAura>(*this);
    if (settings.has_curse_of_doom) auras.curse_of_doom = std::make_unique<CurseOfDoomDot>(*this);
    if (talents.nightfall > 0) auras.shadow_trance = std::make_unique<ShadowTranceAura>(*this);
    if (talents.amplify_curse == 1 &&
        (settings.has_amplify_curse || settings.rotation_option == EmbindConstant::kSimChooses))
      auras.amplify_curse = std::make_unique<AmplifyCurseAura>(*this);
  }
  if (selected_auras.mana_tide_totem) auras.mana_tide_totem = std::make_unique<ManaTideTotemAura>(*this);
  if (selected_auras.chipped_power_core) auras.chipped_power_core = std::make_unique<ChippedPowerCoreAura>(*this);
  if (selected_auras.cracked_power_core) auras.cracked_power_core = std::make_unique<CrackedPowerCoreAura>(*this);
  if (selected_auras.power_infusion) auras.power_infusion = std::make_unique<PowerInfusionAura>(*this);
  if (selected_auras.innervate) auras.innervate = std::make_unique<InnervateAura>(*this);
  if (selected_auras.bloodlust) auras.bloodlust = std::make_unique<BloodlustAura>(*this);
  if (selected_auras.destruction_potion) auras.destruction_potion = std::make_unique<DestructionPotionAura>(*this);
  if (selected_auras.flame_cap) auras.flame_cap = std::make_unique<FlameCapAura>(*this);
  if (settings.race == EmbindConstant::kOrc) auras.blood_fury = std::make_unique<BloodFuryAura>(*this);
  if (selected_auras.drums_of_battle)
    auras.drums_of_battle = std::make_unique<DrumsOfBattleAura>(*this);
  else if (selected_auras.drums_of_war)
    auras.drums_of_war = std::make_unique<DrumsOfWarAura>(*this);
  else if (selected_auras.drums_of_restoration)
    auras.drums_of_restoration = std::make_unique<DrumsOfRestorationAura>(*this);
  if (items.main_hand == ItemId::kBladeOfWizardry)
    auras.blade_of_wizardry = std::make_unique<BladeOfWizardryAura>(*this);
  if (items.neck == ItemId::kShatteredSunPendantOfAcumen && settings.exalted_with_shattrath_faction) {
    if (settings.shattrath_faction == EmbindConstant::kAldor)
      auras.shattered_sun_pendant_of_acumen_aldor = std::make_unique<ShatteredSunPendantOfAcumenAldorAura>(*this);
    else if (settings.shattrath_faction == EmbindConstant::kScryers)
      spells.shattered_sun_pendant_of_acumen_scryers = std::make_unique<ShatteredSunPendantOfAcumenScryers>(*this);
  }
  if (items.chest == ItemId::kRobeOfTheElderScribes)
    auras.robe_of_the_elder_scribes = std::make_unique<RobeOfTheElderScribesAura>(*this);
  if (settings.meta_gem_id == ItemId::kMysticalSkyfireDiamond)
    auras.mystical_skyfire_diamond = std::make_unique<MysticalSkyfireDiamondAura>(*this);
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kEyeOfMagtheridon) !=
      equipped_trinket_ids.end()) {
    auras.eye_of_magtheridon = std::make_unique<EyeOfMagtheridonAura>(*this);
    spells.eye_of_magtheridon = std::make_unique<EyeOfMagtheridon>(*this, auras.eye_of_magtheridon);
  }
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kAshtongueTalismanOfShadows) !=
      equipped_trinket_ids.end())
    auras.ashtongue_talisman_of_shadows = std::make_unique<AshtongueTalismanOfShadowsAura>(*this);
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kDarkmoonCardCrusade) !=
      equipped_trinket_ids.end())
    auras.darkmoon_card_crusade = std::make_unique<DarkmoonCardCrusadeAura>(*this);
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kTheLightningCapacitor) !=
      equipped_trinket_ids.end())
    auras.the_lightning_capacitor = std::make_unique<TheLightningCapacitorAura>(*this);
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kQuagmirransEye) !=
      equipped_trinket_ids.end())
    auras.quagmirrans_eye = std::make_unique<QuagmirransEyeAura>(*this);
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kShiffarsNexusHorn) !=
      equipped_trinket_ids.end())
    auras.shiffars_nexus_horn = std::make_unique<ShiffarsNexusHornAura>(*this);
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kSextantOfUnstableCurrents) !=
      equipped_trinket_ids.end())
    auras.sextant_of_unstable_currents = std::make_unique<SextantOfUnstableCurrentsAura>(*this);
  if (std::find(equipped_ring_ids.begin(), equipped_ring_ids.end(), ItemId::kBandOfTheEternalSage) !=
      equipped_ring_ids.end())
    auras.band_of_the_eternal_sage = std::make_unique<BandOfTheEternalSageAura>(*this);
  if (std::find(equipped_ring_ids.begin(), equipped_ring_ids.end(), ItemId::kWrathOfCenarius) !=
      equipped_ring_ids.end())
    auras.wrath_of_cenarius = std::make_unique<WrathOfCenariusAura>(*this);
  if (sets.t4 >= 2) {
    auras.flameshadow = std::make_unique<FlameshadowAura>(*this);
    auras.shadowflame = std::make_unique<ShadowflameAura>(*this);
  }
  if (sets.spellstrike >= 2) auras.spellstrike = std::make_unique<SpellstrikeAura>(*this);
  if (sets.mana_etched >= 4) auras.mana_etched_4_set = std::make_unique<ManaEtched4SetAura>(*this);

  // Spells
  spells.life_tap = std::make_unique<LifeTap>(*this);
  if (settings.fight_type == EmbindConstant::kAoe) {
    spells.seed_of_corruption = std::make_unique<SeedOfCorruption>(*this);
  } else {
    if (settings.has_shadow_bolt || talents.nightfall > 0 || settings.rotation_option == EmbindConstant::kSimChooses)
      spells.shadow_bolt = std::make_unique<ShadowBolt>(*this);
    if (settings.has_incinerate || settings.rotation_option == EmbindConstant::kSimChooses)
      spells.incinerate = std::make_unique<Incinerate>(*this);
    if (settings.has_searing_pain || settings.rotation_option == EmbindConstant::kSimChooses)
      spells.searing_pain = std::make_unique<SearingPain>(*this);
    if (settings.has_death_coil || settings.rotation_option == EmbindConstant::kSimChooses)
      spells.death_coil = std::make_unique<DeathCoil>(*this);
    if (talents.conflagrate == 1 &&
        (settings.has_conflagrate || settings.rotation_option == EmbindConstant::kSimChooses))
      spells.conflagrate = std::make_unique<Conflagrate>(*this);
    if (talents.shadowburn == 1 &&
        (settings.has_shadow_burn || settings.rotation_option == EmbindConstant::kSimChooses))
      spells.shadowburn = std::make_unique<Shadowburn>(*this);
    if (talents.shadowfury == 1 && (settings.has_shadowfury || settings.rotation_option == EmbindConstant::kSimChooses))
      spells.shadowfury = std::make_unique<Shadowfury>(*this);
    if (auras.corruption != NULL) spells.corruption = std::make_unique<Corruption>(*this, nullptr, auras.corruption);
    if (auras.unstable_affliction != NULL)
      spells.unstable_affliction = std::make_unique<UnstableAffliction>(*this, nullptr, auras.unstable_affliction);
    if (auras.siphon_life != NULL) spells.siphon_life = std::make_unique<SiphonLife>(*this, nullptr, auras.siphon_life);
    if (auras.immolate != NULL) spells.immolate = std::make_unique<Immolate>(*this, nullptr, auras.immolate);
    if (auras.curse_of_agony != NULL || auras.curse_of_doom != NULL)
      spells.curse_of_agony = std::make_unique<CurseOfAgony>(*this, nullptr, auras.curse_of_agony);
    if (auras.curse_of_the_elements != NULL)
      spells.curse_of_the_elements = std::make_unique<CurseOfTheElements>(*this, auras.curse_of_the_elements);
    if (auras.curse_of_recklessness != NULL)
      spells.curse_of_recklessness = std::make_unique<CurseOfRecklessness>(*this, auras.curse_of_recklessness);
    if (auras.curse_of_doom != NULL)
      spells.curse_of_doom = std::make_unique<CurseOfDoom>(*this, nullptr, auras.curse_of_doom);
    if (auras.amplify_curse != NULL) spells.amplify_curse = std::make_unique<AmplifyCurse>(*this, auras.amplify_curse);
  }
  if (auras.improved_shadow_bolt != NULL)
    spells.improved_shadow_bolt = std::make_unique<ImprovedShadowBolt>(*this, auras.improved_shadow_bolt);
  if (auras.mana_tide_totem != NULL)
    spells.mana_tide_totem = std::make_unique<ManaTideTotem>(*this, auras.mana_tide_totem);
  if (auras.chipped_power_core != NULL)
    spells.chipped_power_core = std::make_unique<ChippedPowerCore>(*this, auras.chipped_power_core);
  if (auras.cracked_power_core != NULL)
    spells.cracked_power_core = std::make_unique<CrackedPowerCore>(*this, auras.cracked_power_core);
  if (selected_auras.super_mana_potion) spells.super_mana_potion = std::make_unique<SuperManaPotion>(*this);
  if (selected_auras.demonic_rune) spells.demonic_rune = std::make_unique<DemonicRune>(*this);
  if (talents.dark_pact == 1 && (settings.has_dark_pact || settings.rotation_option == EmbindConstant::kSimChooses))
    spells.dark_pact = std::make_unique<DarkPact>(*this);
  if (auras.destruction_potion != NULL)
    spells.destruction_potion = std::make_unique<DestructionPotion>(*this, auras.destruction_potion);
  if (auras.flame_cap != NULL) spells.flame_cap = std::make_unique<FlameCap>(*this, auras.flame_cap);
  if (auras.blood_fury != NULL) spells.blood_fury = std::make_unique<BloodFury>(*this, auras.blood_fury);
  if (auras.drums_of_battle != NULL)
    spells.drums_of_battle = std::make_unique<DrumsOfBattle>(*this, auras.drums_of_battle);
  else if (auras.drums_of_war != NULL)
    spells.drums_of_war = std::make_unique<DrumsOfWar>(*this, auras.drums_of_war);
  else if (auras.drums_of_restoration != NULL)
    spells.drums_of_restoration = std::make_unique<DrumsOfRestoration>(*this, auras.drums_of_restoration);
  if (auras.blade_of_wizardry != NULL)
    spells.blade_of_wizardry = std::make_unique<BladeOfWizardry>(*this, auras.blade_of_wizardry);
  if (auras.shattered_sun_pendant_of_acumen_aldor != NULL)
    spells.shattered_sun_pendant_of_acumen_aldor =
        std::make_unique<ShatteredSunPendantOfAcumenAldor>(*this, auras.shattered_sun_pendant_of_acumen_aldor);
  if (auras.robe_of_the_elder_scribes != NULL)
    spells.robe_of_the_elder_scribes = std::make_unique<RobeOfTheElderScribes>(*this, auras.robe_of_the_elder_scribes);
  if (auras.mystical_skyfire_diamond != NULL)
    spells.mystical_skyfire_diamond = std::make_unique<MysticalSkyfireDiamond>(*this, auras.mystical_skyfire_diamond);
  if (settings.meta_gem_id == ItemId::kInsightfulEarthstormDiamond)
    spells.insightful_earthstorm_diamond = std::make_unique<InsightfulEarthstormDiamond>(*this);
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kTimbalsFocusingCrystal) !=
      equipped_trinket_ids.end())
    spells.timbals_focusing_crystal = std::make_unique<TimbalsFocusingCrystal>(*this);
  if (std::find(equipped_trinket_ids.begin(), equipped_trinket_ids.end(), ItemId::kMarkOfDefiance) !=
      equipped_trinket_ids.end())
    spells.mark_of_defiance = std::make_unique<MarkOfDefiance>(*this);
  if (auras.the_lightning_capacitor != NULL)
    spells.the_lightning_capacitor = std::make_unique<TheLightningCapacitor>(*this);
  if (auras.quagmirrans_eye != NULL)
    spells.quagmirrans_eye = std::make_unique<QuagmirransEye>(*this, auras.quagmirrans_eye);
  if (auras.shiffars_nexus_horn != NULL)
    spells.shiffars_nexus_horn = std::make_unique<ShiffarsNexusHorn>(*this, auras.shiffars_nexus_horn);
  if (auras.sextant_of_unstable_currents != NULL)
    spells.sextant_of_unstable_currents =
        std::make_unique<SextantOfUnstableCurrents>(*this, auras.sextant_of_unstable_currents);
  if (auras.band_of_the_eternal_sage != NULL)
    spells.band_of_the_eternal_sage = std::make_unique<BandOfTheEternalSage>(*this, auras.band_of_the_eternal_sage);
  if (selected_auras.judgement_of_wisdom) spells.judgement_of_wisdom = std::make_unique<JudgementOfWisdom>(*this);
  if (auras.flameshadow != NULL) spells.flameshadow = std::make_unique<Flameshadow>(*this, auras.flameshadow);
  if (auras.shadowflame != NULL) spells.shadowflame = std::make_unique<Shadowflame>(*this, auras.shadowflame);
  if (auras.spellstrike != NULL) spells.spellstrike = std::make_unique<Spellstrike>(*this, auras.spellstrike);
  if (auras.mana_etched_4_set != NULL)
    spells.mana_etched_4_set = std::make_unique<ManaEtched4Set>(*this, auras.mana_etched_4_set);
  if (auras.ashtongue_talisman_of_shadows != NULL)
    spells.ashtongue_talisman_of_shadows =
        std::make_unique<AshtongueTalismanOfShadows>(*this, auras.ashtongue_talisman_of_shadows);
  if (auras.wrath_of_cenarius != NULL)
    spells.wrath_of_cenarius = std::make_unique<WrathOfCenarius>(*this, auras.wrath_of_cenarius);
  if (auras.darkmoon_card_crusade != NULL)
    spells.darkmoon_card_crusade = std::make_unique<DarkmoonCardCrusade>(*this, auras.darkmoon_card_crusade);
  if (auras.power_infusion != NULL) {
    for (int i = 0; i < settings.power_infusion_amount; i++) {
      spells.power_infusion.push_back(std::make_unique<PowerInfusion>(*this, auras.power_infusion));
    }
  }
  if (auras.bloodlust != NULL) {
    for (int i = 0; i < settings.bloodlust_amount; i++) {
      spells.bloodlust.push_back(std::make_unique<Bloodlust>(*this, auras.bloodlust));
    }
  }
  if (auras.innervate != NULL) {
    for (int i = 0; i < settings.innervate_amount; i++) {
      spells.innervate.push_back(std::make_unique<Innervate>(*this, auras.innervate));
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
  if (spells.curse_of_the_elements != NULL) {
    curse_spell = spells.curse_of_the_elements;
    curse_aura = auras.curse_of_the_elements;
  } else if (spells.curse_of_recklessness != NULL) {
    curse_spell = spells.curse_of_recklessness;
    curse_aura = auras.curse_of_recklessness;
  } else if (spells.curse_of_doom != NULL) {
    curse_spell = spells.curse_of_doom;
  } else if (spells.curse_of_agony != NULL) {
    curse_spell = spells.curse_of_agony;
  }

  SendPlayerInfoToCombatLog();
}

void Player::Reset() {
  cast_time_remaining = 0;
  gcd_remaining = 0;
  mp5_timer = 5;
  five_second_rule_timer = 5;
  stats.mana = stats.max_mana;
  power_infusions_ready = settings.power_infusion_amount;

  for (auto& trinket : trinkets) {
    trinket.Reset();
  }

  for (auto& spell : spell_list) {
    spell->Reset();
  }
}

void Player::EndAuras() {
  for (auto& trinket : trinkets) {
    if (trinket.active) {
      trinket.Fade();
    }
  }

  for (auto& aura : aura_list) {
    if (aura->active) {
      aura->Fade();
    }
  }

  for (auto& dot : dot_list) {
    if (dot->active) {
      dot->Fade();
    }
  }
}

double Player::GetHastePercent() {
  double haste_percent = stats.spell_haste_percent;

  // If both Bloodlust and Power Infusion are active then remove the 20% PI
  // bonus since they don't stack
  if (auras.bloodlust != NULL && auras.power_infusion != NULL && auras.bloodlust->active &&
      auras.power_infusion->active) {
    for (auto& stat : auras.power_infusion->stats) {
      if (stat.name == StatName::kSpellHastePercent) {
        haste_percent /= stat.value;
      }
    }
  }

  return haste_percent * (1 + stats.spell_haste_rating / kHasteRatingPerPercent / 100.0);
}

double Player::GetGcdValue(const std::string& spell_name) {
  return spell_name == SpellName::kShadowfury ? 0 : std::max(kMinimumGcdValue, kGcdValue / GetHastePercent());
}

double Player::GetSpellPower(bool dealing_damage, SpellSchool school) {
  double spell_power = stats.spell_power + demonic_knowledge_spell_power;

  // If Mark of the Champion is equipped and the player isn't dealing damage then remove the spell power so it doesn't
  // affect spells like Life Tap
  if (!dealing_damage &&
      (items.trinket_1 == ItemId::kMarkOfTheChampion || items.trinket_2 == ItemId::kMarkOfTheChampion)) {
    spell_power -= 85;
  }
  if (sets.spellfire == 3) {
    spell_power += GetIntellect() * 0.07;
  }
  if (school == SpellSchool::kShadow) {
    spell_power += stats.shadow_power;
  } else if (school == SpellSchool::kFire) {
    spell_power += stats.fire_power;
  }
  // Spell Power from spirit if player has Improved Divine Spirit buffed
  if (selected_auras.prayer_of_spirit && settings.improved_divine_spirit > 0) {
    spell_power += GetSpirit() * (0 + (static_cast<double>(settings.improved_divine_spirit) / 20.0));
  }
  return spell_power;
}

double Player::GetCritChance(SpellType spell_type) {
  double crit_chance = stats.spell_crit_chance + (GetIntellect() * kCritChancePerIntellect) +
                       (stats.spell_crit_rating / kCritRatingPerPercent);
  if (spell_type != SpellType::kDestruction) {
    crit_chance -= talents.devastation;
  }
  return crit_chance;
}

double Player::GetHitChance(SpellType spell_type) {
  return std::min(99.0, stats.spell_hit_chance + stats.extra_spell_hit_chance +
                            (spell_type == SpellType::kAffliction ? talents.suppression * 2 : 0));
}

bool Player::IsCrit(SpellType spell_type, double extra_crit) { return RollRng(GetCritChance(spell_type) + extra_crit); }

bool Player::IsHit(SpellType spell_type) { return RollRng(GetHitChance(spell_type)); }

int Player::GetRand() { return rng.range(0, 100 * kFloatNumberMultiplier); }

bool Player::RollRng(double chance) { return GetRand() <= chance * kFloatNumberMultiplier; }

double Player::GetStamina() { return stats.stamina * stats.stamina_modifier; }

double Player::GetIntellect() { return stats.intellect * stats.intellect_modifier; }

double Player::GetSpirit() { return stats.spirit * stats.spirit_modifier; }

// formula from
// https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/
// && https://royalgiraffe.github.io/resist-guide
double Player::GetBaseHitChance(int player_level, int enemy_level) {
  const int kLevelDifference = enemy_level - player_level;
  return kLevelDifference <= 2   ? std::min(99, 100 - kLevelDifference - 4)
         : kLevelDifference == 3 ? 83
                                 : 83 - 11 * kLevelDifference;
}

void Player::UseCooldowns(double fight_time_remaining) {
  // Only use PI if Bloodlust isn't selected or if Bloodlust isn't active since they don't stack, or if there are enough
  // Power Infusions available to last until the end of the fight for the mana cost reduction
  if (!spells.power_infusion.empty() && !auras.power_infusion->active &&
      (spells.bloodlust.empty() || !auras.bloodlust->active ||
       power_infusions_ready * auras.power_infusion->duration >= fight_time_remaining)) {
    for (auto& pi : spells.power_infusion) {
      if (pi->Ready()) {
        pi->StartCast();
        break;
      }
    }
  }
  // TODO don't use innervate until x% mana
  if (!spells.innervate.empty() && !auras.innervate->active) {
    for (auto& innervate : spells.innervate) {
      if (innervate->Ready()) {
        innervate->StartCast();
        break;
      }
    }
  }
  if (spells.chipped_power_core != NULL && spells.chipped_power_core->Ready()) {
    spells.chipped_power_core->StartCast();
  } else if (spells.cracked_power_core != NULL && spells.cracked_power_core->Ready()) {
    spells.cracked_power_core->StartCast();
  }
  if (spells.destruction_potion != NULL && spells.destruction_potion->Ready()) {
    spells.destruction_potion->StartCast();
  }
  if (spells.flame_cap != NULL && spells.flame_cap->Ready()) {
    spells.flame_cap->StartCast();
  }
  if (spells.blood_fury != NULL && spells.blood_fury->Ready()) {
    spells.blood_fury->StartCast();
  }
  for (int i = 0; i < trinkets.size(); i++) {
    if (trinkets[i].Ready()) {
      trinkets[i].Use();
      // Set the other on-use trinket (if another is equipped) on cooldown for
      // the duration of the trinket just used if the trinkets share cooldown
      const int kOtherTrinketSlot = i == 1 ? 0 : 1;
      if (trinkets.size() > kOtherTrinketSlot && trinkets[kOtherTrinketSlot].shares_cooldown &&
          trinkets[i].shares_cooldown) {
        trinkets[kOtherTrinketSlot].cooldown_remaining =
            std::max(trinkets[kOtherTrinketSlot].cooldown_remaining, static_cast<double>(trinkets[i].duration));
      }
    }
  }
}

void Player::CastLifeTapOrDarkPact() {
  if (spells.dark_pact != NULL && spells.dark_pact->Ready()) {
    spells.dark_pact->StartCast();
  } else {
    spells.life_tap->StartCast();
  }
}

double Player::GetPartialResistMultiplier(SpellSchool school) {
  if (school != SpellSchool::kShadow && school != SpellSchool::kFire) {
    return 1;
  }

  const int kEnemyResist = school == SpellSchool::kShadow ? settings.enemy_shadow_resist : settings.enemy_fire_resist;

  return 1.0 - ((75 * kEnemyResist) / (kLevel * 5)) / 100.0;
}

void Player::PostIterationDamageAndMana(const std::string& spell_name) {
  PostCombatLogBreakdownVector(spell_name.c_str(), combat_log_breakdown.at(spell_name)->iteration_mana_gain,
                               combat_log_breakdown.at(spell_name)->iteration_damage);
  combat_log_breakdown.at(spell_name)->iteration_damage = 0;
  combat_log_breakdown.at(spell_name)->iteration_mana_gain = 0;
}

void Player::ThrowError(const std::string& error) {
  SendCombatLogEntries();
  ErrorCallback(error.c_str());
  throw std::runtime_error(error);
}

bool Player::ShouldWriteToCombatLog() { return iteration == 10 && settings.equipped_item_simulation; }

void Player::SendCombatLogEntries() {
  for (const auto& value : combat_log_entries) {
    CombatLogUpdate(value.c_str());
  }
}

void Player::CombatLog(const std::string& entry) {
  combat_log_entries.push_back("|" + DoubleToString(fight_time, 4) + "| " + entry);
}

double Player::FindTimeUntilNextAction() {
  double time = cast_time_remaining;
  if (time <= 0) {
    time = gcd_remaining;
  }

  // Pet
  if (pet != NULL) {
    if ((talents.dark_pact > 0 || settings.pet_mode == EmbindConstant::kAggressive) &&
        pet->spirit_tick_timer_remaining < time)
      time = pet->spirit_tick_timer_remaining;

    if (settings.pet_mode == EmbindConstant::kAggressive) {
      if (pet->cast_time_remaining > 0 && pet->cast_time_remaining < time) {
        time = pet->cast_time_remaining;
      }

      // Pet spells
      for (auto& pet_spell : pet_spell_list) {
        if (pet_spell->cooldown_remaining > 0 && pet_spell->cooldown_remaining < time) {
          time = pet_spell->cooldown_remaining;
        }
      }

      // Pet auras
      for (auto& pet_aura : pet_aura_list) {
        if (pet_aura->active && pet_aura->duration_remaining < time) {
          time = pet_aura->duration_remaining;
        }
      }
    }
  }

  // Spells
  for (auto& spell : spell_list) {
    if (spell->cooldown_remaining < time && spell->cooldown_remaining > 0) {
      time = spell->cooldown_remaining;
    }
  }

  // Auras
  for (auto& aura : aura_list) {
    if (aura->active && aura->has_duration && aura->duration_remaining < time) {
      time = aura->duration_remaining;
    }
  }

  // Dots
  for (auto& dot : dot_list) {
    if (dot->active && dot->tick_timer_remaining < time) {
      time = dot->tick_timer_remaining;
    }
  }

  // MP5
  if (mp5_timer < time && mp5_timer > 0) {
    time = mp5_timer;
  }

  // Trinkets
  for (auto& trinket : trinkets) {
    if (trinket.active && trinket.duration_remaining < time) {
      time = trinket.duration_remaining;
    }
    if (trinket.cooldown_remaining > 0 && trinket.cooldown_remaining < time) {
      time = trinket.cooldown_remaining;
    }
  }

  return time;
}

void Player::Tick(double time) {
  fight_time += time;  // This needs to be the first modified value since the time in combat needs to be updated before
                       // spells start dealing Damage/auras expiring etc. for the combat logging.
  cast_time_remaining -= time;
  gcd_remaining -= time;
  mp5_timer -= time;
  five_second_rule_timer -= time;

  // Pet
  if (pet != NULL) {
    pet->Tick(time);
  }

  // Auras need to tick before Spells because otherwise you'll, for example,
  // finish casting Corruption and then immediately afterwards, in the same
  // millisecond, immediately tick down the aura This was also causing buffs like
  // e.g. the t4 4pc buffs to expire sooner than they should.
  for (auto& aura : aura_list) {
    if (aura->active && aura->duration_remaining > 0) {
      aura->Tick(time);
    }
  }

  for (auto& dot : dot_list) {
    if (dot->active && dot->tick_timer_remaining > 0) {
      dot->Tick(time);
    }
  }

  for (auto& spell : spell_list) {
    if (spell->casting || spell->cooldown_remaining > 0) {
      spell->Tick(time);
    }
  }

  for (auto& trinket : trinkets) {
    trinket.Tick(time);
  }

  if (mp5_timer <= 0) {
    mp5_timer = 5;

    if (stats.mp5 > 0 || five_second_rule_timer <= 0 || (auras.innervate != NULL && auras.innervate->active)) {
      const bool kInnervateIsActive = auras.innervate != NULL && auras.innervate->active;
      const double kCurrentPlayerMana = stats.mana;

      // MP5
      if (stats.mp5 > 0) {
        stats.mana += stats.mp5;
      }
      // Spirit mana regen
      if (kInnervateIsActive || five_second_rule_timer <= 0) {
        // Formula from https://wowwiki-archive.fandom.com/wiki/Spirit?oldid=1572910
        double mp5_from_spirit = 5 * (0.001 + std::sqrt(GetIntellect()) * GetSpirit() * 0.009327);
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
        combat_log_breakdown.at("Mp5")->casts++;
        combat_log_breakdown.at("Mp5")->iteration_mana_gain += kManaGained;
      }

      if (ShouldWriteToCombatLog()) {
        CombatLog("Player gains " + DoubleToString(kManaGained) + " mana from MP5 (" +
                  DoubleToString(kCurrentPlayerMana) + " -> " + DoubleToString(stats.mana) + ")");
      }
    }
  }
}

double Player::GetCustomImprovedShadowBoltDamageModifier() { return 0.2 * (settings.custom_isb_uptime_value / 100.0); }

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
      "Crit Chance: " + DoubleToString(round(GetCritChance(SpellType::kDestruction) * 100) / 100, 2) + "%");
  combat_log_entries.push_back(
      "Hit Chance: " + DoubleToString(std::min(16.0, round((stats.extra_spell_hit_chance) * 100) / 100), 2) + "%");
  combat_log_entries.push_back(
      "Haste: " + DoubleToString(round((stats.spell_haste_rating / kHasteRatingPerPercent) * 100) / 100, 2) + "%");
  combat_log_entries.push_back("Shadow Modifier: " + DoubleToString(stats.shadow_modifier * 100, 2) + "%");
  combat_log_entries.push_back("Fire Modifier: " + DoubleToString(stats.fire_modifier * 100, 2) + "%");
  combat_log_entries.push_back("MP5: " + DoubleToString(stats.mp5));
  combat_log_entries.push_back("Spell Penetration: " + DoubleToString(stats.spell_penetration));
  if (pet != NULL) {
    combat_log_entries.push_back("---------------- Pet stats ----------------");
    combat_log_entries.push_back("Stamina: " + DoubleToString(pet->GetStamina()));
    combat_log_entries.push_back("Intellect: " + DoubleToString(pet->GetIntellect()));
    combat_log_entries.push_back("Strength: " + DoubleToString(pet->GetStrength()));
    combat_log_entries.push_back("Agility: " + DoubleToString(pet->GetAgility()));
    combat_log_entries.push_back("Spirit: " + DoubleToString(pet->GetSpirit()));
    combat_log_entries.push_back("Attack Power: " + DoubleToString(round(pet->stats.attack_power)) +
                                 " (without attack power % modifiers)");
    combat_log_entries.push_back("Spell Power: " + DoubleToString(pet->stats.spell_power));
    combat_log_entries.push_back("Mana: " + DoubleToString(pet->stats.max_mana));
    combat_log_entries.push_back("MP5: " + DoubleToString(pet->stats.mp5));
    if (pet->pet_type == PetType::kMelee) {
      combat_log_entries.push_back(
          "Physical Hit Chance: " + DoubleToString(round(pet->stats.melee_hit_chance * 100) / 100.0, 2) + "%");
      combat_log_entries.push_back(
          "Physical Crit Chance: " + DoubleToString(round(pet->GetMeleeCritChance() * 100) / 100.0, 2) + "% (" +
          DoubleToString(pet->crit_suppression, 2) + "% Crit Suppression Applied)");
      combat_log_entries.push_back(
          "Glancing Blow Chance: " + DoubleToString(round(pet->glancing_blow_chance * 100) / 100.0, 2) + "%");
      combat_log_entries.push_back(
          "Attack Power Modifier: " + DoubleToString(round(pet->stats.attack_power_modifier * 10000) / 100.0, 2) + "%");
    }
    if (pet->pet == PetName::kImp || pet->pet == PetName::kSuccubus) {
      combat_log_entries.push_back(
          "Spell Hit Chance: " + DoubleToString(round(pet->stats.spell_hit_chance * 100) / 100.0, 2) + "%");
      combat_log_entries.push_back(
          "Spell Crit Chance: " + DoubleToString(round(pet->GetSpellCritChance() * 100) / 100.0, 2) + "%");
    }
    combat_log_entries.push_back(
        "Damage Modifier: " + DoubleToString(round(pet->stats.damage_modifier * 10000) / 100, 2) + "%");
  }
  combat_log_entries.push_back("---------------- Enemy stats ----------------");
  combat_log_entries.push_back("Level: " + std::to_string(settings.enemy_level));
  combat_log_entries.push_back("Shadow Resistance: " + std::to_string(settings.enemy_shadow_resist));
  combat_log_entries.push_back("Fire Resistance: " + std::to_string(settings.enemy_fire_resist));
  if (pet != NULL && pet->pet != PetName::kImp) {
    combat_log_entries.push_back("Dodge Chance: " + DoubleToString(pet->enemy_dodge_chance) + "%");
    combat_log_entries.push_back("Armor: " + std::to_string(settings.enemy_armor));
    combat_log_entries.push_back(
        "Damage Reduction From Armor: " +
        DoubleToString(round((1 - pet->enemy_damage_reduction_from_armor) * 10000) / 100.0, 2) + "%");
  }
  combat_log_entries.push_back("---------------------------------------------");
}