#include "player.h"

#include "bindings.h"
#include "common.h"
#include "damage_over_time.h"
#include "spell.h"

Player::Player(std::shared_ptr<PlayerSettings> player_settings)
    : selected_auras(player_settings->auras),
      talents(player_settings->talents),
      sets(player_settings->sets),
      stats(player_settings->stats),
      items(player_settings->items),
      settings(player_settings) {
  spells = std::make_unique<PlayerSpells>();
  auras = std::make_unique<PlayerAuras>();
  combat_log_entries = {};
  cast_time_remaining = 0;
  gcd_remaining = 0;
  // I don't know if this formula only works for bosses or not, so for the
  // moment I'm only using it for lvl >=73 targets.
  const double enemy_base_resistance = settings->enemy_level >= 73 ? (6 * kLevel * 5) / 75.0 : 0;
  settings->enemy_shadow_resist = std::max(static_cast<double>(settings->enemy_shadow_resist), enemy_base_resistance);
  settings->enemy_fire_resist = std::max(static_cast<double>(settings->enemy_fire_resist), enemy_base_resistance);

  combat_log_breakdown.insert(std::make_pair("Mp5", std::make_unique<CombatLogBreakdown>("Mp5")));
  if (selected_auras->judgementOfWisdom) {
    combat_log_breakdown.insert(
        std::make_pair("Judgement of Wisdom", std::make_unique<CombatLogBreakdown>("Judgement of Wisdom")));
  }

  // Crit chance
  if (selected_auras->powerOfTheGuardianMage) {
    stats->crit_rating += 28 * settings->mage_atiesh_amount;
  }
  stats->crit_chance = kBaseCritChancePercent + (stats->crit_rating / kCritRatingPerPercent) + talents->devastation +
                       talents->backlash + talents->demonic_tactics;
  if (selected_auras->moonkinAura) {
    stats->crit_chance += 5;
  }
  if (selected_auras->judgementOfTheCrusader) {
    stats->crit_chance += 3;
  }
  if (selected_auras->totemOfWrath) {
    stats->crit_chance += (3 * settings->totem_of_wrath_amount);
  }
  if (selected_auras->chainOfTheTwilightOwl) {
    stats->crit_chance += 2;
  }

  // Hit chance
  if (sets->mana_etched >= 2) {
    stats->hit_rating += 35;
  }
  stats->extra_hit_chance = stats->hit_rating / kHitRatingPerPercent;
  if (selected_auras->totemOfWrath) {
    stats->extra_hit_chance += (3 * settings->totem_of_wrath_amount);
  }
  if (selected_auras->inspiringPresence) {
    stats->extra_hit_chance += 1;
  }
  stats->hit_chance = round(GetBaseHitChance(kLevel, settings->enemy_level));

  // Add bonus damage % from Demonic Sacrifice
  if (talents->demonic_sacrifice == 1 && settings->sacrificing_pet) {
    if (settings->pet_is_imp) {
      stats->fire_modifier *= 1.15;
    } else if (settings->pet_is_succubus) {
      stats->shadow_modifier *= 1.15;
    } else if (settings->pet_is_felguard) {
      stats->shadow_modifier *= 1.1;
    }
    // todo add felhunter mana regen maybe
  } else {
    // Add damage % multiplier from Master Demonologist and Soul Link
    if (talents->soul_link == 1) {
      stats->shadow_modifier *= 1.05;
      stats->fire_modifier *= 1.05;
    }
    if (talents->master_demonologist > 0) {
      if (settings->pet_is_succubus) {
        stats->shadow_modifier *= (1 + 0.02 * talents->master_demonologist);
        stats->fire_modifier *= (1 + 0.02 * talents->master_demonologist);
      } else if (settings->pet_is_felguard) {
        stats->shadow_modifier *= (1 + 0.01 * talents->master_demonologist);
        stats->fire_modifier *= (1 + 0.01 * talents->master_demonologist);
      }
    }
  }
  // Shadow Mastery
  stats->shadow_modifier *= (1 + (0.02 * talents->shadow_mastery));
  // Ferocious Inspiration
  if (selected_auras->ferociousInspiration) {
    stats->shadow_modifier *= std::pow(1.03, settings->ferocious_inspiration_amount);
    stats->fire_modifier *= std::pow(1.03, settings->ferocious_inspiration_amount);
  }
  // Add % dmg modifiers from Curse of the Elements + Malediction
  if (selected_auras->curseOfTheElements) {
    stats->shadow_modifier *= 1.1 + (0.01 * settings->improved_curse_of_the_elements);
    stats->fire_modifier *= 1.1 + (0.01 * settings->improved_curse_of_the_elements);
  }
  // Add fire dmg % from Emberstorm
  if (talents->emberstorm > 0) {
    stats->fire_modifier *= 1 + (0.02 * talents->emberstorm);
  }
  // Add spell power from Fel Armor
  if (selected_auras->felArmor) {
    stats->spell_power += 100 * (0 + 0.1 * talents->demonic_aegis);
  }
  // If using a custom isb uptime % then just add to the shadow modifier % (this
  // assumes 5/5 ISB giving 20% shadow Damage)
  if (settings->using_custom_isb_uptime) {
    stats->shadow_modifier *= (1.0 + 0.2 * (settings->custom_isb_uptime_value / 100.0));
  }
  // Add spell power from Improved Divine Spirit
  stats->spirit_modifier *= (1 - (0.01 * talents->demonic_embrace));
  if (selected_auras->prayerOfSpirit && settings->improved_divine_spirit > 0) {
    stats->spell_power +=
        (stats->spirit * stats->spirit_modifier * (0 + (static_cast<double>(settings->improved_divine_spirit) / 20.0)));
  }
  // Elemental shaman t4 2pc bonus
  if (selected_auras->wrathOfAirTotem && settings->has_elemental_shaman_t4_bonus) {
    stats->spell_power += 20;
  }
  // Add extra stamina from Blood Pact from Improved Imp
  if (selected_auras->bloodPact) {
    int improved_imp_points = settings->improved_imp;

    if (settings->pet_is_imp && (!settings->sacrificing_pet || talents->demonic_sacrifice == 0) &&
        talents->improved_imp > improved_imp_points) {
      improved_imp_points = talents->improved_imp;
    }

    stats->stamina += 70 * 0.1 * improved_imp_points;
  }
  // Add stamina from Demonic Embrace
  stats->stamina_modifier *= 1 + (0.03 * talents->demonic_embrace);
  // Add mp5 from Vampiric Touch (add 25% instead of 5% since we're adding it to
  // the mana per 5 seconds variable)
  if (selected_auras->vampiricTouch) {
    stats->mp5 += settings->shadow_priest_dps * 0.25;
  }
  if (selected_auras->powerOfTheGuardianWarlock) {
    stats->spell_power += 33 * settings->warlock_atiesh_amount;
  }
  if (sets->twin_stars == 2) {
    stats->spell_power += 15;
  }

  // Enemy Armor Reduction
  if (selected_auras->faerieFire) {
    settings->enemy_armor -= 610;
  }
  if ((selected_auras->sunderArmor && selected_auras->exposeArmor && settings->improved_expose_armor == 2) ||
      (selected_auras->exposeArmor && !selected_auras->sunderArmor)) {
    settings->enemy_armor -= 2050 * (1 + 0.25 * settings->improved_expose_armor);
  } else if (selected_auras->sunderArmor) {
    settings->enemy_armor -= 520 * 5;
  }
  if (selected_auras->curseOfRecklessness) {
    settings->enemy_armor -= 800;
  }
  if (selected_auras->annihilator) {
    settings->enemy_armor -= 600;
  }
  settings->enemy_armor = std::max(0, settings->enemy_armor);

  // Health & Mana
  stats->health = (stats->health + (stats->stamina * stats->stamina_modifier) * kHealthPerStamina) *
                  (1 + (0.01 * static_cast<double>(talents->fel_stamina)));
  stats->max_mana = (stats->mana + (stats->intellect * stats->intellect_modifier) * kManaPerIntellect) *
                    (1 + (0.01 * static_cast<double>(talents->fel_intellect)));
}

void Player::Initialize() {
  demonic_knowledge_spell_power = 0;
  if (!settings->sacrificing_pet || talents->demonic_sacrifice == 0) {
    if (settings->pet_is_imp) {
      pet = std::make_shared<Imp>(shared_from_this());
    } else if (settings->pet_is_succubus) {
      pet = std::make_shared<Succubus>(shared_from_this());
    } else if (settings->pet_is_felguard) {
      pet = std::make_shared<Felguard>(shared_from_this());
    }
    if (pet != NULL) {
      pet->Initialize();
    }
  }

  std::vector<int> trinketIds{items->trinket_1, items->trinket_2};
  if (std::find(trinketIds.begin(), trinketIds.end(), 32483) != trinketIds.end())
    trinkets.push_back(std::make_unique<SkullOfGuldan>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 34429) != trinketIds.end())
    trinkets.push_back(std::make_unique<ShiftingNaaruSliver>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 33829) != trinketIds.end())
    trinkets.push_back(std::make_unique<HexShrunkenHead>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 29370) != trinketIds.end())
    trinkets.push_back(std::make_unique<IconOfTheSilverCrescent>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 29132) != trinketIds.end())
    trinkets.push_back(std::make_unique<ScryersBloodgem>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 23046) != trinketIds.end())
    trinkets.push_back(std::make_unique<RestrainedEssenceOfSapphiron>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 29179) != trinketIds.end())
    trinkets.push_back(std::make_unique<XirisGift>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 25620) != trinketIds.end())
    trinkets.push_back(std::make_unique<AncientCrystalTalisman>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 28223) != trinketIds.end())
    trinkets.push_back(std::make_unique<ArcanistsStone>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 25936) != trinketIds.end())
    trinkets.push_back(std::make_unique<TerokkarTabletOfVim>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 28040) != trinketIds.end())
    trinkets.push_back(std::make_unique<VengeanceOfTheIllidari>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 24126) != trinketIds.end())
    trinkets.push_back(std::make_unique<FigurineLivingRubySerpent>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 29376) != trinketIds.end())
    trinkets.push_back(std::make_unique<EssenceOfTheMartyr>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 30340) != trinketIds.end())
    trinkets.push_back(std::make_unique<StarkillersBauble>(shared_from_this()));
  if (std::find(trinketIds.begin(), trinketIds.end(), 38290) != trinketIds.end())
    trinkets.push_back(std::make_unique<DarkIronSmokingPipe>(shared_from_this()));

  // Auras
  if (settings->is_single_target) {
    if (talents->improved_shadow_bolt > 0)
      auras->improved_shadow_bolt = std::make_shared<ImprovedShadowBoltAura>(shared_from_this());
    if (settings->has_corruption || settings->sim_choosing_rotation)
      auras->corruption = std::make_shared<CorruptionDot>(shared_from_this());
    if (talents->unstable_affliction == 1 && (settings->has_unstable_affliction || settings->sim_choosing_rotation))
      auras->unstable_affliction = std::make_shared<UnstableAfflictionDot>(shared_from_this());
    if (talents->siphon_life == 1 && (settings->has_siphon_life || settings->sim_choosing_rotation))
      auras->siphon_life = std::make_shared<SiphonLifeDot>(shared_from_this());
    if (settings->has_immolate || settings->sim_choosing_rotation)
      auras->immolate = std::make_shared<ImmolateDot>(shared_from_this());
    if (settings->has_curse_of_agony || settings->has_curse_of_doom)
      auras->curse_of_agony = std::make_shared<CurseOfAgonyDot>(shared_from_this());
    if (settings->has_curse_of_the_elements)
      auras->curse_of_the_elements = std::make_shared<CurseOfTheElementsAura>(shared_from_this());
    if (settings->has_curse_of_recklessness)
      auras->curse_of_recklessness = std::make_shared<CurseOfRecklessnessAura>(shared_from_this());
    if (settings->has_curse_of_doom) auras->curse_of_doom = std::make_shared<CurseOfDoomDot>(shared_from_this());
    if (talents->nightfall > 0) auras->shadow_trance = std::make_shared<ShadowTranceAura>(shared_from_this());
    if (talents->amplify_curse == 1 && (settings->has_amplify_curse || settings->sim_choosing_rotation))
      auras->amplify_curse = std::make_shared<AmplifyCurseAura>(shared_from_this());
  }
  if (selected_auras->manaTideTotem) auras->mana_tide_totem = std::make_shared<ManaTideTotemAura>(shared_from_this());
  if (selected_auras->chippedPowerCore)
    auras->chipped_power_core = std::make_shared<ChippedPowerCoreAura>(shared_from_this());
  if (selected_auras->crackedPowerCore)
    auras->cracked_power_core = std::make_shared<CrackedPowerCoreAura>(shared_from_this());
  if (selected_auras->powerInfusion) auras->power_infusion = std::make_shared<PowerInfusionAura>(shared_from_this());
  if (selected_auras->innervate) auras->innervate = std::make_shared<InnervateAura>(shared_from_this());
  if (selected_auras->bloodlust) auras->bloodlust = std::make_shared<BloodlustAura>(shared_from_this());
  if (selected_auras->destructionPotion)
    auras->destruction_potion = std::make_shared<DestructionPotionAura>(shared_from_this());
  if (selected_auras->flameCap) auras->flame_cap = std::make_shared<FlameCapAura>(shared_from_this());
  if (settings->is_orc) auras->blood_fury = std::make_shared<BloodFuryAura>(shared_from_this());
  if (selected_auras->drumsOfBattle)
    auras->drums_of_battle = std::make_shared<DrumsOfBattleAura>(shared_from_this());
  else if (selected_auras->drumsOfWar)
    auras->drums_of_war = std::make_shared<DrumsOfWarAura>(shared_from_this());
  else if (selected_auras->drumsOfRestoration)
    auras->drums_of_restoration = std::make_shared<DrumsOfRestorationAura>(shared_from_this());
  if (items->main_hand == 31336) auras->blade_of_wizardry = std::make_shared<BladeOfWizardryAura>(shared_from_this());
  if (items->neck == 34678)
    auras->shattered_sun_pendant_of_acumen = std::make_shared<ShatteredSunPendantOfAcumenAura>(shared_from_this());
  if (items->chest == 28602)
    auras->robe_of_the_elder_scribes = std::make_shared<RobeOfTheElderScribesAura>(shared_from_this());
  if (settings->meta_gem_id == 25893)
    auras->mystical_skyfire_diamond = std::make_shared<MysticalSkyfireDiamondAura>(shared_from_this());
  if (std::find(trinketIds.begin(), trinketIds.end(), 28789) != trinketIds.end())
    auras->eye_of_magtheridon = std::make_shared<EyeOfMagtheridonAura>(shared_from_this());
  if (std::find(trinketIds.begin(), trinketIds.end(), 32493) != trinketIds.end())
    auras->ashtongue_talisman_of_shadows = std::make_shared<AshtongueTalismanOfShadowsAura>(shared_from_this());
  if (std::find(trinketIds.begin(), trinketIds.end(), 31856) != trinketIds.end())
    auras->darkmoon_card_crusade = std::make_shared<DarkmoonCardCrusadeAura>(shared_from_this());
  if (std::find(trinketIds.begin(), trinketIds.end(), 28785) != trinketIds.end())
    auras->the_lightning_capacitor = std::make_shared<TheLightningCapacitorAura>(shared_from_this());
  if (std::find(trinketIds.begin(), trinketIds.end(), 27683) != trinketIds.end())
    auras->quagmirrans_eye = std::make_shared<QuagmirransEyeAura>(shared_from_this());
  if (std::find(trinketIds.begin(), trinketIds.end(), 28418) != trinketIds.end())
    auras->shiffars_nexus_horn = std::make_shared<ShiffarsNexusHornAura>(shared_from_this());
  if (std::find(trinketIds.begin(), trinketIds.end(), 30626) != trinketIds.end())
    auras->sextant_of_unstable_currents = std::make_shared<SextantOfUnstableCurrentsAura>(shared_from_this());
  if (items->ring_1 == 29305 || items->ring_2 == 29305)
    auras->band_of_the_eternal_sage = std::make_shared<BandOfTheEternalSageAura>(shared_from_this());
  if (items->ring_1 == 21190 || items->ring_2 == 21190)
    auras->wrath_of_cenarius = std::make_shared<WrathOfCenariusAura>(shared_from_this());
  if (sets->t4 >= 2) {
    auras->flameshadow = std::make_shared<FlameshadowAura>(shared_from_this());
    auras->shadowflame = std::make_shared<ShadowflameAura>(shared_from_this());
  }
  if (sets->spellstrike >= 2) auras->spellstrike = std::make_shared<SpellstrikeAura>(shared_from_this());
  if (sets->mana_etched >= 4) auras->mana_etched_4_set = std::make_shared<ManaEtched4SetAura>(shared_from_this());

  // Spells
  spells->life_tap = std::make_shared<LifeTap>(shared_from_this());
  if (!settings->is_single_target) {
    spells->seed_of_corruption = std::make_shared<SeedOfCorruption>(shared_from_this());
  } else {
    if (settings->has_shadow_bolt || talents->nightfall > 0 || settings->sim_choosing_rotation)
      spells->shadow_bolt = std::make_shared<ShadowBolt>(shared_from_this());
    if (settings->has_incinerate || settings->sim_choosing_rotation)
      spells->incinerate = std::make_shared<Incinerate>(shared_from_this());
    if (settings->has_searing_pain || settings->sim_choosing_rotation)
      spells->searing_pain = std::make_shared<SearingPain>(shared_from_this());
    if (settings->has_death_coil || settings->sim_choosing_rotation)
      spells->death_coil = std::make_shared<DeathCoil>(shared_from_this());
    if (talents->conflagrate == 1 && (settings->has_conflagrate || settings->sim_choosing_rotation))
      spells->conflagrate = std::make_shared<Conflagrate>(shared_from_this());
    if (talents->shadowburn == 1 && (settings->has_shadow_burn || settings->sim_choosing_rotation))
      spells->shadowburn = std::make_shared<Shadowburn>(shared_from_this());
    if (talents->shadowfury == 1 && (settings->has_shadowfury || settings->sim_choosing_rotation))
      spells->shadowfury = std::make_shared<Shadowfury>(shared_from_this());
    if (auras->corruption != NULL)
      spells->corruption = std::make_shared<Corruption>(shared_from_this(), nullptr, auras->corruption);
    if (auras->unstable_affliction != NULL)
      spells->unstable_affliction =
          std::make_shared<UnstableAffliction>(shared_from_this(), nullptr, auras->unstable_affliction);
    if (auras->siphon_life != NULL)
      spells->siphon_life = std::make_shared<SiphonLife>(shared_from_this(), nullptr, auras->siphon_life);
    if (auras->immolate != NULL)
      spells->immolate = std::make_shared<Immolate>(shared_from_this(), nullptr, auras->immolate);
    if (auras->curse_of_agony != NULL || auras->curse_of_doom != NULL)
      spells->curse_of_agony = std::make_shared<CurseOfAgony>(shared_from_this(), nullptr, auras->curse_of_agony);
    if (auras->curse_of_the_elements != NULL)
      spells->curse_of_the_elements =
          std::make_shared<CurseOfTheElements>(shared_from_this(), auras->curse_of_the_elements);
    if (auras->curse_of_recklessness != NULL)
      spells->curse_of_recklessness =
          std::make_shared<CurseOfRecklessness>(shared_from_this(), auras->curse_of_recklessness);
    if (auras->curse_of_doom != NULL)
      spells->curse_of_doom = std::make_shared<CurseOfDoom>(shared_from_this(), nullptr, auras->curse_of_doom);
    if (auras->amplify_curse != NULL)
      spells->amplify_curse = std::make_shared<AmplifyCurse>(shared_from_this(), auras->amplify_curse);
  }
  if (auras->mana_tide_totem != NULL)
    spells->mana_tide_totem = std::make_shared<ManaTideTotem>(shared_from_this(), auras->mana_tide_totem);
  if (auras->chipped_power_core != NULL)
    spells->chipped_power_core = std::make_shared<ChippedPowerCore>(shared_from_this(), auras->chipped_power_core);
  if (auras->cracked_power_core != NULL)
    spells->cracked_power_core = std::make_shared<CrackedPowerCore>(shared_from_this(), auras->cracked_power_core);
  if (selected_auras->superManaPotion)
    spells->super_mana_potion = std::make_shared<SuperManaPotion>(shared_from_this());
  if (selected_auras->demonicRune) spells->demonic_rune = std::make_shared<DemonicRune>(shared_from_this());
  if (talents->dark_pact == 1 && (settings->has_dark_pact || settings->sim_choosing_rotation))
    spells->dark_pact = std::make_shared<DarkPact>(shared_from_this());
  if (auras->destruction_potion != NULL)
    spells->destruction_potion = std::make_shared<DestructionPotion>(shared_from_this(), auras->destruction_potion);
  if (auras->flame_cap != NULL) spells->flame_cap = std::make_shared<FlameCap>(shared_from_this(), auras->flame_cap);
  if (auras->blood_fury != NULL)
    spells->blood_fury = std::make_shared<BloodFury>(shared_from_this(), auras->blood_fury);
  if (auras->drums_of_battle != NULL)
    spells->drums_of_battle = std::make_shared<DrumsOfBattle>(shared_from_this(), auras->drums_of_battle);
  else if (auras->drums_of_war != NULL)
    spells->drums_of_war = std::make_shared<DrumsOfWar>(shared_from_this(), auras->drums_of_war);
  else if (auras->drums_of_restoration != NULL)
    spells->drums_of_restoration =
        std::make_shared<DrumsOfRestoration>(shared_from_this(), auras->drums_of_restoration);
  if (auras->blade_of_wizardry != NULL)
    spells->blade_of_wizardry = std::make_shared<BladeOfWizardry>(shared_from_this(), auras->blade_of_wizardry);
  if (auras->shattered_sun_pendant_of_acumen != NULL)
    spells->shattered_sun_pendant_of_acumen =
        std::make_shared<ShatteredSunPendantOfAcumen>(shared_from_this(), auras->shattered_sun_pendant_of_acumen);
  if (auras->robe_of_the_elder_scribes != NULL)
    spells->robe_of_the_elder_scribes =
        std::make_shared<RobeOfTheElderScribes>(shared_from_this(), auras->robe_of_the_elder_scribes);
  if (auras->mystical_skyfire_diamond != NULL)
    spells->mystical_skyfire_diamond =
        std::make_shared<MysticalSkyfireDiamond>(shared_from_this(), auras->mystical_skyfire_diamond);
  if (settings->meta_gem_id == 25901)
    spells->insightful_earthstorm_diamond = std::make_shared<InsightfulEarthstormDiamond>(shared_from_this());
  if (std::find(trinketIds.begin(), trinketIds.end(), 34470) != trinketIds.end())
    spells->timbals_focusing_crystal = std::make_shared<TimbalsFocusingCrystal>(shared_from_this());
  if (std::find(trinketIds.begin(), trinketIds.end(), 27922) != trinketIds.end())
    spells->mark_of_defiance = std::make_shared<MarkOfDefiance>(shared_from_this());
  if (auras->the_lightning_capacitor != NULL)
    spells->the_lightning_capacitor =
        std::make_shared<TheLightningCapacitor>(shared_from_this(), auras->the_lightning_capacitor);
  if (auras->quagmirrans_eye != NULL)
    spells->quagmirrans_eye = std::make_shared<QuagmirransEye>(shared_from_this(), auras->quagmirrans_eye);
  if (auras->shiffars_nexus_horn != NULL)
    spells->shiffars_nexus_horn = std::make_shared<ShiffarsNexusHorn>(shared_from_this(), auras->shiffars_nexus_horn);
  if (auras->sextant_of_unstable_currents != NULL)
    spells->sextant_of_unstable_currents =
        std::make_shared<SextantOfUnstableCurrents>(shared_from_this(), auras->sextant_of_unstable_currents);
  if (items->ring_1 == 29305 || items->ring_2 == 29305)
    spells->band_of_the_eternal_sage =
        std::make_shared<BandOfTheEternalSage>(shared_from_this(), auras->band_of_the_eternal_sage);
  if (auras->power_infusion != NULL) {
    for (int i = 0; i < settings->power_infusion_amount; i++) {
      spells->power_infusion.push_back(std::make_shared<PowerInfusion>(shared_from_this(), auras->power_infusion));
    }
  }
  if (auras->bloodlust != NULL) {
    for (int i = 0; i < settings->bloodlust_amount; i++) {
      spells->bloodlust.push_back(std::make_shared<Bloodlust>(shared_from_this(), auras->bloodlust));
    }
  }
  if (auras->innervate != NULL) {
    for (int i = 0; i < settings->innervate_amount; i++) {
      spells->innervate.push_back(std::make_shared<Innervate>(shared_from_this(), auras->innervate));
    }
  }

  // Set the filler property
  if (settings->has_incinerate) {
    filler = spells->incinerate;
  } else if (settings->has_searing_pain) {
    filler = spells->searing_pain;
  } else {
    filler = spells->shadow_bolt;
  }

  // Set the curseSpell and curseAura properties
  if (spells->curse_of_the_elements != NULL) {
    curse_spell = spells->curse_of_the_elements;
    curse_aura = auras->curse_of_the_elements;
  } else if (spells->curse_of_recklessness != NULL) {
    curse_spell = spells->curse_of_recklessness;
    curse_aura = auras->curse_of_recklessness;
  } else if (spells->curse_of_doom != NULL) {
    curse_spell = spells->curse_of_doom;
  } else if (spells->curse_of_agony != NULL) {
    curse_spell = spells->curse_of_agony;
  }

  SendPlayerInfoToCombatLog();
}

void Player::Reset() {
  cast_time_remaining = 0;
  gcd_remaining = 0;
  stats->mana = stats->max_mana;
  mp5_timer = 5;
  five_second_rule_timer = 5;

  // Reset trinkets
  for (int i = 0; i < trinkets.size(); i++) {
    trinkets[i]->Reset();
  }

  // Reset spells
  if (spells->life_tap != NULL) spells->life_tap->Reset();
  if (spells->seed_of_corruption != NULL) spells->seed_of_corruption->Reset();
  if (spells->shadow_bolt != NULL) spells->shadow_bolt->Reset();
  if (spells->incinerate != NULL) spells->incinerate->Reset();
  if (spells->searing_pain != NULL) spells->searing_pain->Reset();
  if (spells->corruption != NULL) spells->corruption->Reset();
  if (spells->unstable_affliction != NULL) spells->unstable_affliction->Reset();
  if (spells->siphon_life != NULL) spells->siphon_life->Reset();
  if (spells->immolate != NULL) spells->immolate->Reset();
  if (spells->curse_of_agony != NULL) spells->curse_of_agony->Reset();
  if (spells->curse_of_the_elements != NULL) spells->curse_of_the_elements->Reset();
  if (spells->curse_of_recklessness != NULL) spells->curse_of_recklessness->Reset();
  if (spells->curse_of_doom != NULL) spells->curse_of_doom->Reset();
  if (spells->conflagrate != NULL) spells->conflagrate->Reset();
  if (spells->shadowburn != NULL) spells->shadowburn->Reset();
  if (spells->death_coil != NULL) spells->death_coil->Reset();
  if (spells->shadowfury != NULL) spells->shadowfury->Reset();
  if (spells->amplify_curse != NULL) spells->amplify_curse->Reset();
  if (spells->dark_pact != NULL) spells->dark_pact->Reset();
  if (spells->destruction_potion != NULL) spells->destruction_potion->Reset();
  if (spells->super_mana_potion != NULL) spells->super_mana_potion->Reset();
  if (spells->demonic_rune != NULL) spells->demonic_rune->Reset();
  if (spells->flame_cap != NULL) spells->flame_cap->Reset();
  if (spells->blood_fury != NULL) spells->blood_fury->Reset();
  if (spells->drums_of_battle != NULL) spells->drums_of_battle->Reset();
  if (spells->drums_of_war != NULL) spells->drums_of_war->Reset();
  if (spells->drums_of_restoration != NULL) spells->drums_of_restoration->Reset();
  if (spells->blade_of_wizardry != NULL) spells->blade_of_wizardry->Reset();
  if (spells->shattered_sun_pendant_of_acumen != NULL) spells->shattered_sun_pendant_of_acumen->Reset();
  if (spells->robe_of_the_elder_scribes != NULL) spells->robe_of_the_elder_scribes->Reset();
  if (spells->mystical_skyfire_diamond != NULL) spells->mystical_skyfire_diamond->Reset();
  if (spells->insightful_earthstorm_diamond != NULL) spells->insightful_earthstorm_diamond->Reset();
  if (spells->timbals_focusing_crystal != NULL) spells->timbals_focusing_crystal->Reset();
  if (spells->mark_of_defiance != NULL) spells->mark_of_defiance->Reset();
  if (spells->the_lightning_capacitor != NULL) spells->the_lightning_capacitor->Reset();
  if (spells->quagmirrans_eye != NULL) spells->quagmirrans_eye->Reset();
  if (spells->shiffars_nexus_horn != NULL) spells->shiffars_nexus_horn->Reset();
  if (spells->sextant_of_unstable_currents != NULL) spells->sextant_of_unstable_currents->Reset();
  if (spells->band_of_the_eternal_sage != NULL) spells->band_of_the_eternal_sage->Reset();
  if (spells->chipped_power_core != NULL) spells->chipped_power_core->Reset();
  if (spells->cracked_power_core != NULL) spells->cracked_power_core->Reset();
  if (spells->mana_tide_totem != NULL) spells->mana_tide_totem->Reset();
  for (std::vector<std::shared_ptr<Spell>>::iterator it = spells->power_infusion.begin();
       it != spells->power_infusion.end(); it++) {
    (*it)->Reset();
  }
  for (std::vector<std::shared_ptr<Spell>>::iterator it = spells->bloodlust.begin(); it != spells->bloodlust.end();
       it++) {
    (*it)->Reset();
  }
  for (std::vector<std::shared_ptr<Spell>>::iterator it = spells->innervate.begin(); it != spells->innervate.end();
       it++) {
    (*it)->Reset();
  }
}

void Player::EndAuras() {
  for (auto& trinket : trinkets) {
    if (trinket->active) {
      trinket->Fade();
    }
  }
  if (auras->corruption != NULL && auras->corruption->active) auras->corruption->Fade();
  if (auras->unstable_affliction != NULL && auras->unstable_affliction->active) auras->unstable_affliction->Fade();
  if (auras->siphon_life != NULL && auras->siphon_life->active) auras->siphon_life->Fade();
  if (auras->immolate != NULL && auras->immolate->active) auras->immolate->Fade();
  if (auras->curse_of_agony != NULL && auras->curse_of_agony->active) auras->curse_of_agony->Fade();
  if (auras->curse_of_doom != NULL && auras->curse_of_doom->active) auras->curse_of_doom->Fade();
  if (auras->improved_shadow_bolt != NULL && auras->improved_shadow_bolt->active) auras->improved_shadow_bolt->Fade();
  if (auras->curse_of_the_elements != NULL && auras->curse_of_the_elements->active)
    auras->curse_of_the_elements->Fade();
  if (auras->curse_of_recklessness != NULL && auras->curse_of_recklessness->active)
    auras->curse_of_recklessness->Fade();
  if (auras->shadow_trance != NULL && auras->shadow_trance->active) auras->shadow_trance->Fade();
  if (auras->amplify_curse != NULL && auras->amplify_curse->active) auras->amplify_curse->Fade();
  if (auras->power_infusion != NULL && auras->power_infusion->active) auras->power_infusion->Fade();
  if (auras->innervate != NULL && auras->innervate->active) auras->innervate->Fade();
  if (auras->blood_fury != NULL && auras->blood_fury->active) auras->blood_fury->Fade();
  if (auras->destruction_potion != NULL && auras->destruction_potion->active) auras->destruction_potion->Fade();
  if (auras->flame_cap != NULL && auras->flame_cap->active) auras->flame_cap->Fade();
  if (auras->bloodlust != NULL && auras->bloodlust->active) auras->bloodlust->Fade();
  if (auras->drums_of_battle != NULL && auras->drums_of_battle->active) auras->drums_of_battle->Fade();
  if (auras->drums_of_war != NULL && auras->drums_of_war->active) auras->drums_of_war->Fade();
  if (auras->drums_of_restoration != NULL && auras->drums_of_restoration->active) auras->drums_of_restoration->Fade();
  if (auras->band_of_the_eternal_sage != NULL && auras->band_of_the_eternal_sage->active)
    auras->band_of_the_eternal_sage->Fade();
  if (auras->wrath_of_cenarius != NULL && auras->wrath_of_cenarius->active) auras->wrath_of_cenarius->Fade();
  if (auras->blade_of_wizardry != NULL && auras->blade_of_wizardry->active) auras->blade_of_wizardry->Fade();
  if (auras->shattered_sun_pendant_of_acumen != NULL && auras->shattered_sun_pendant_of_acumen->active)
    auras->shattered_sun_pendant_of_acumen->Fade();
  if (auras->robe_of_the_elder_scribes != NULL && auras->robe_of_the_elder_scribes->active)
    auras->robe_of_the_elder_scribes->Fade();
  if (auras->mystical_skyfire_diamond != NULL && auras->mystical_skyfire_diamond->active)
    auras->mystical_skyfire_diamond->Fade();
  if (auras->eye_of_magtheridon != NULL && auras->eye_of_magtheridon->active) auras->eye_of_magtheridon->Fade();
  if (auras->sextant_of_unstable_currents != NULL && auras->sextant_of_unstable_currents->active)
    auras->sextant_of_unstable_currents->Fade();
  if (auras->quagmirrans_eye != NULL && auras->quagmirrans_eye->active) auras->quagmirrans_eye->Fade();
  if (auras->shiffars_nexus_horn != NULL && auras->shiffars_nexus_horn->active) auras->shiffars_nexus_horn->Fade();
  if (auras->ashtongue_talisman_of_shadows != NULL && auras->ashtongue_talisman_of_shadows->active)
    auras->ashtongue_talisman_of_shadows->Fade();
  if (auras->darkmoon_card_crusade != NULL && auras->darkmoon_card_crusade->active)
    auras->darkmoon_card_crusade->Fade();
  if (auras->the_lightning_capacitor != NULL && auras->the_lightning_capacitor->active)
    auras->the_lightning_capacitor->Fade();
  if (auras->flameshadow != NULL && auras->flameshadow->active) auras->flameshadow->Fade();
  if (auras->shadowflame != NULL && auras->shadowflame->active) auras->shadowflame->Fade();
  if (auras->spellstrike != NULL && auras->spellstrike->active) auras->spellstrike->Fade();
  if (auras->mana_etched_4_set != NULL && auras->mana_etched_4_set->active) auras->mana_etched_4_set->Fade();
  if (auras->chipped_power_core != NULL && auras->chipped_power_core->active) auras->chipped_power_core->Fade();
  if (auras->cracked_power_core != NULL && auras->cracked_power_core->active) auras->cracked_power_core->Fade();
  if (auras->mana_tide_totem != NULL && auras->mana_tide_totem->active) auras->mana_tide_totem->Fade();
}

double Player::GetHastePercent() {
  double haste_percent = stats->haste_percent;

  // If both Bloodlust and Power Infusion are active then remove the 20% PI
  // bonus since they don't stack
  if (auras->bloodlust != NULL && auras->power_infusion != NULL && auras->bloodlust->active &&
      auras->power_infusion->active) {
    haste_percent /= (1 + auras->power_infusion->stats->haste_percent / 100);
  }

  return haste_percent * (1 + stats->haste_rating / kHasteRatingPerPercent / 100.0);
}

double Player::GetGcdValue(const std::shared_ptr<Spell>& spell) {
  if (spells->shadowfury == NULL || spell != spells->shadowfury) {
    return std::max(kMinimumGcdValue, round((kGcdValue / GetHastePercent()) * 10000) / 10000);
  }
  return 0;
}

double Player::GetSpellPower(SpellSchool school) {
  double spell_power = stats->spell_power + demonic_knowledge_spell_power;
  if (sets->spellfire == 3) {
    spell_power += stats->intellect * stats->intellect_modifier * 0.07;
  }
  if (school == SpellSchool::SHADOW) {
    spell_power += stats->shadow_power;
  } else if (school == SpellSchool::FIRE) {
    spell_power += stats->fire_power;
  }
  return spell_power;
}

double Player::GetCritChance(SpellType spell_type) {
  double crit_chance = stats->crit_chance + ((stats->intellect * stats->intellect_modifier) * kCritChancePerIntellect);
  if (spell_type != SpellType::DESTRUCTION) {
    crit_chance -= talents->devastation;
  }
  return crit_chance;
}

double Player::GetHitChance(SpellType spell_type) {
  double hit_chance = stats->hit_chance + stats->extra_hit_chance;
  if (spell_type == SpellType::AFFLICTION) {
    hit_chance += talents->suppression * 2;
  }
  return std::min(99.0, hit_chance);
}

bool Player::IsCrit(SpellType spell_type, double extra_crit) {
  return GetRand() <= ((GetCritChance(spell_type) + extra_crit) * kFloatNumberMultiplier);
}

bool Player::IsHit(SpellType spell_type) {
  double hit = GetRand() <= (GetHitChance(spell_type) * kFloatNumberMultiplier);
  if (!hit && auras->eye_of_magtheridon != NULL) {
    auras->eye_of_magtheridon->Apply();
  }
  return hit;
}

int Player::GetRand() { return random_num(gen); }

// formula from
// https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/
// && https://royalgiraffe.github.io/resist-guide
double Player::GetBaseHitChance(int player_level, int enemy_level) {
  const int level_difference = enemy_level - player_level;
  if (level_difference <= 2) {
    return std::min(99, 100 - level_difference - 4);
  } else if (level_difference == 3) {
    return 83;
  } else if (level_difference >= 4) {
    return 83 - 11 * level_difference;
  }
  return 0;
}

void Player::UseCooldowns() {
  // Only use PI if Bloodlust isn't selected or if Bloodlust isn't active since
  // they don't stack
  if (!spells->power_infusion.empty() && !auras->power_infusion->active &&
      (spells->bloodlust.empty() || !auras->bloodlust->active)) {
    for (auto pi : spells->power_infusion) {
      if (pi->Ready()) {
        pi->StartCast();
        break;
      }
    }
  }
  // TODO don't use innervate until x% mana
  if (!spells->innervate.empty() && !auras->innervate->active) {
    for (auto innervate : spells->innervate) {
      if (innervate->Ready()) {
        innervate->StartCast();
        break;
      }
    }
  }
  if (spells->chipped_power_core != NULL && spells->chipped_power_core->Ready()) {
    spells->chipped_power_core->StartCast();
  } else if (spells->cracked_power_core != NULL && spells->cracked_power_core->Ready()) {
    spells->cracked_power_core->StartCast();
  }
  if (spells->destruction_potion != NULL && spells->destruction_potion->Ready()) {
    spells->destruction_potion->StartCast();
  }
  if (spells->flame_cap != NULL && spells->flame_cap->Ready()) {
    spells->flame_cap->StartCast();
  }
  if (spells->blood_fury != NULL && spells->blood_fury->Ready()) {
    spells->blood_fury->StartCast();
  }
  for (int i = 0; i < trinkets.size(); i++) {
    if (trinkets[i]->Ready()) {
      trinkets[i]->Use();
      // Set the other on-use trinket (if another is equipped) on cooldown for
      // the duration of the trinket just used if the trinkets share cooldown
      int otherTrinketSlot = i == 1 ? 0 : 1;
      if (trinkets.size() > otherTrinketSlot && trinkets[otherTrinketSlot] != NULL &&
          trinkets[otherTrinketSlot]->shares_cooldown && trinkets[i]->shares_cooldown) {
        trinkets[otherTrinketSlot]->cooldown_remaining =
            std::max(trinkets[otherTrinketSlot]->cooldown_remaining, static_cast<double>(trinkets[i]->duration));
      }
    }
  }
}

void Player::CastLifeTapOrDarkPact() {
  if (spells->dark_pact != NULL && spells->dark_pact->Ready()) {
    spells->dark_pact->StartCast();
  } else {
    spells->life_tap->StartCast();
  }
}

double Player::GetPartialResistMultiplier(SpellSchool school) {
  if (school == SpellSchool::SHADOW) {
    return 1.0 - ((75 * settings->enemy_shadow_resist) / (kLevel * 5)) / 100.0;
  } else if (school == SpellSchool::FIRE) {
    return 1.0 - ((75 * settings->enemy_fire_resist) / (kLevel * 5)) / 100.0;
  }

  return 1;
}

void Player::AddIterationDamageAndMana(const std::string& spell_name, int mana_gain, int damage) {
  if (combat_log_breakdown.count(spell_name) == 0 || !settings->recording_combat_log_breakdown) {
    return;
  }

  int iteration_mana_gain = combat_log_breakdown.at(spell_name)->iteration_mana_gain;
  int iteration_damage = combat_log_breakdown.at(spell_name)->iteration_damage;

  // Check for integer overflow
  if (iteration_mana_gain + mana_gain < 0 || iteration_damage + damage < 0) {
    PostIterationDamageAndMana(spell_name);
  }

  combat_log_breakdown.at(spell_name)->iteration_mana_gain += mana_gain;
  combat_log_breakdown.at(spell_name)->iteration_damage += damage;
}

void Player::PostIterationDamageAndMana(const std::string& spell_name) {
  if (!settings->recording_combat_log_breakdown) {
    return;
  }

  PostCombatLogBreakdownVector(spell_name.c_str(), combat_log_breakdown.at(spell_name)->iteration_mana_gain,
                               combat_log_breakdown.at(spell_name)->iteration_damage);
  combat_log_breakdown.at(spell_name)->iteration_damage = 0;
  combat_log_breakdown.at(spell_name)->iteration_mana_gain = 0;
}

std::string Player::GetCustomStat() {
  if (settings->simming_stamina) return "stamina";
  if (settings->simming_intellect) return "intellect";
  if (settings->simming_spirit) return "spirit";
  if (settings->simming_spell_power) return "spell_power";
  if (settings->simming_shadow_power) return "shadowPower";
  if (settings->simming_fire_power) return "firePower";
  if (settings->simming_crit_rating) return "critRating";
  if (settings->simming_hit_rating) return "hitRating";
  if (settings->simming_haste_rating) return "hasteRating";
  if (settings->simming_mp5) return "mp5";
  return "normal";
}

void Player::ThrowError(const std::string& error) {
  SendCombatLogEntries();
  ErrorCallback(error.c_str());
  throw std::runtime_error(error);
}

bool Player::ShouldWriteToCombatLog() { return iteration == 10 && settings->equipped_item_simulation; }

void Player::SendCombatLogEntries() {
  for (const auto& value : combat_log_entries) {
    CombatLogUpdate(value.c_str());
  }
}

void Player::CombatLog(const std::string& entry) {
  combat_log_entries.push_back("|" + TruncateTrailingZeros(std::to_string(fight_time), 4) + "| " + entry);
}

void Player::SendPlayerInfoToCombatLog() {
  combat_log_entries.push_back("---------------- Player stats ----------------");
  combat_log_entries.push_back("Health: " + TruncateTrailingZeros(std::to_string(round(stats->health))));
  combat_log_entries.push_back("Mana: " + TruncateTrailingZeros(std::to_string(round(stats->max_mana))));
  combat_log_entries.push_back("Stamina: " +
                               TruncateTrailingZeros(std::to_string(round(stats->stamina * stats->stamina_modifier))));
  combat_log_entries.push_back(
      "Intellect: " + TruncateTrailingZeros(std::to_string(round(stats->intellect * stats->intellect_modifier))));
  combat_log_entries.push_back("Spell Power: " + TruncateTrailingZeros(std::to_string(round(GetSpellPower()))));
  combat_log_entries.push_back("Shadow Power: " + std::to_string(stats->shadow_power));
  combat_log_entries.push_back("Fire Power: " + std::to_string(stats->fire_power));
  combat_log_entries.push_back(
      "Crit Chance: " +
      TruncateTrailingZeros(std::to_string(round(GetCritChance(SpellType::DESTRUCTION) * 100) / 100), 2) + "%");
  combat_log_entries.push_back(
      "Hit Chance: " +
      TruncateTrailingZeros(
          std::to_string(std::min(static_cast<double>(16), round((stats->extra_hit_chance) * 100) / 100)), 2) +
      "%");
  combat_log_entries.push_back(
      "Haste: " +
      TruncateTrailingZeros(std::to_string(round((stats->haste_rating / kHasteRatingPerPercent) * 100) / 100), 2) +
      "%");
  combat_log_entries.push_back(
      "Shadow Modifier: " + TruncateTrailingZeros(std::to_string(stats->shadow_modifier * 100)) + "%");
  combat_log_entries.push_back("Fire Modifier: " + TruncateTrailingZeros(std::to_string(stats->fire_modifier * 100)) +
                               "%");
  combat_log_entries.push_back("MP5: " + std::to_string(stats->mp5));
  combat_log_entries.push_back("Spell Penetration: " + std::to_string(stats->spell_penetration));
  if (pet != NULL) {
    combat_log_entries.push_back("---------------- Pet stats ----------------");
    combat_log_entries.push_back(
        "Stamina: " + TruncateTrailingZeros(std::to_string(round(pet->stats->stamina * pet->stats->stamina_modifier))));
    combat_log_entries.push_back("Intellect: " + TruncateTrailingZeros(std::to_string(
                                                     round(pet->stats->intellect * pet->stats->intellect_modifier))));
    combat_log_entries.push_back(
        "Strength: " + TruncateTrailingZeros(std::to_string(
                           round((pet->base_stats->strength + pet->buff_stats->strength + pet->stats->strength) *
                                 pet->stats->strength_modifier))));
    combat_log_entries.push_back(
        "Agility: " + TruncateTrailingZeros(std::to_string(round(pet->stats->agility * pet->stats->agility_modifier))));
    combat_log_entries.push_back(
        "Spirit: " +
        TruncateTrailingZeros(std::to_string(round(
            (pet->base_stats->spirit + pet->buff_stats->spirit + pet->stats->spirit) * pet->stats->spirit_modifier))));
    combat_log_entries.push_back(
        "Attack Power: " + TruncateTrailingZeros(std::to_string(round(pet->stats->attack_power))) +
        " (without attack power % modifiers)");
    combat_log_entries.push_back("Spell Power: " + TruncateTrailingZeros(std::to_string(pet->stats->spell_power)));
    combat_log_entries.push_back("Mana: " + TruncateTrailingZeros(std::to_string(pet->stats->max_mana)));
    combat_log_entries.push_back("MP5: " + std::to_string(pet->stats->mp5));
    if (pet->pet_type == PetType::MELEE) {
      combat_log_entries.push_back(
          "Physical Hit Chance: " +
          TruncateTrailingZeros(std::to_string(round(pet->GetMeleeHitChance() * 100) / 100.0), 2) + "%");
      combat_log_entries.push_back(
          "Physical Crit Chance: " +
          TruncateTrailingZeros(std::to_string(round(pet->GetMeleeCritChance() * 100) / 100.0), 2) + "% (" +
          TruncateTrailingZeros(std::to_string(pet->crit_suppression), 2) + "% Crit Suppression Applied)");
      combat_log_entries.push_back(
          "Glancing Blow Chance: " +
          TruncateTrailingZeros(std::to_string(round(pet->glancing_blow_chance * 100) / 100.0), 2) + "%");
      combat_log_entries.push_back(
          "Attack Power Modifier: " +
          TruncateTrailingZeros(std::to_string(round(pet->stats->attack_power_modifier * 10000) / 100.0), 2) + "%");
    }
    if (pet->pet == PetName::IMP || pet->pet == PetName::SUCCUBUS) {
      combat_log_entries.push_back(
          "Spell Hit Chance: " +
          TruncateTrailingZeros(std::to_string(round(pet->GetSpellHitChance() * 100) / 100.0), 2) + "%");
      combat_log_entries.push_back(
          "Spell Crit Chance: " +
          TruncateTrailingZeros(std::to_string(round(pet->GetSpellCritChance() * 100) / 100.0), 2) + "%");
    }
    combat_log_entries.push_back(
        "Damage Modifier: " +
        TruncateTrailingZeros(std::to_string(round(pet->stats->damage_modifier * 10000) / 100), 2) + "%");
  }
  combat_log_entries.push_back("---------------- Enemy stats ----------------");
  combat_log_entries.push_back("Level: " + std::to_string(settings->enemy_level));
  combat_log_entries.push_back("Shadow Resistance: " + std::to_string(settings->enemy_shadow_resist));
  combat_log_entries.push_back("Fire Resistance: " + std::to_string(settings->enemy_fire_resist));
  if (pet != NULL && pet->pet != PetName::IMP) {
    combat_log_entries.push_back("Dodge Chance: " + std::to_string(pet->enemy_dodge_chance) + "%");
    combat_log_entries.push_back("Armor: " + std::to_string(settings->enemy_armor));
    combat_log_entries.push_back(
        "Damage Reduction From Armor: " + std::to_string(round((1 - pet->armor_multiplier) * 10000) / 100.0) + "%");
  }
  combat_log_entries.push_back("---------------------------------------------");
}