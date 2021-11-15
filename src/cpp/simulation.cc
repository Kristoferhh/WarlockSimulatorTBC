#include "simulation.h"

#include <stdlib.h>

#include <algorithm>
#include <chrono>
#include <vector>

#include "bindings.h"
#include "common.h"
#include "enums.h"
#include "spell.h"

Simulation::Simulation(std::shared_ptr<Player> player, std::shared_ptr<SimulationSettings> simulation_settings)
    : player(player), settings(simulation_settings) {}

void Simulation::Start() {
  uint32_t* random_seeds = AllocRandomSeeds(settings->iterations);
  std::vector<double> dps_vector;
  player->total_duration = 0;
  player->Initialize();
  double min_dps = 99999;
  double max_dps = 0;
  std::mt19937 gen;
  std::uniform_int_distribution<> random_fight_length(settings->min_time, settings->max_time);

  for (player->iteration = 0; player->iteration < settings->iterations; player->iteration++) {
    player->gen.seed(random_seeds[player->iteration]);
    gen.seed(random_seeds[player->iteration]);
    const int kFightLength = random_fight_length(gen);
    player->Reset();
    if (player->pet != NULL) {
      player->pet->Reset();
    }
    player->iteration_damage = 0;
    player->fight_time = 0;
    if (player->ShouldWriteToCombatLog()) {
      player->CombatLog("Fight length: " + std::to_string(kFightLength) + " seconds");
    }

    while (player->fight_time < kFightLength) {
      const double kFightTimeRemaining = kFightLength - player->fight_time;

      // Use Drums
      if (player->spells->drums_of_battle != NULL && !player->auras->drums_of_battle->active &&
          player->spells->drums_of_battle->Ready()) {
        player->spells->drums_of_battle->StartCast();
      } else if (player->spells->drums_of_war != NULL && !player->auras->drums_of_war->active &&
                 player->spells->drums_of_war->Ready()) {
        player->spells->drums_of_war->StartCast();
      } else if (player->spells->drums_of_restoration != NULL && !player->auras->drums_of_restoration->active &&
                 player->spells->drums_of_restoration->Ready()) {
        player->spells->drums_of_restoration->StartCast();
      }
      // Use Bloodlust
      if (!player->spells->bloodlust.empty() && !player->auras->bloodlust->active) {
        for (auto bloodlust : player->spells->bloodlust) {
          if (bloodlust->Ready()) {
            bloodlust->StartCast();
            break;
          }
        }
      }
      // Use Mana Tide Totem if there's <= 12 sec remaning or the player's mana
      // is at 50% or lower
      if (player->spells->mana_tide_totem != NULL && player->spells->mana_tide_totem->Ready() &&
          (kFightTimeRemaining <= player->auras->mana_tide_totem->duration ||
           player->stats->mana / static_cast<double>(player->stats->max_mana) <= 0.50)) {
        player->spells->mana_tide_totem->StartCast();
      }

      // Player
      if (player->cast_time_remaining <= 0) {
        // Spells not on the GCD
        // Demonic Rune
        if ((player->fight_time > 5 || player->stats->mp5 == 0) && player->spells->demonic_rune != NULL &&
            (player->stats->max_mana - player->stats->mana) > player->spells->demonic_rune->average_mana_value &&
            player->spells->demonic_rune->Ready() &&
            (!player->spells->chipped_power_core || player->spells->chipped_power_core->cooldown_remaining > 0) &&
            (!player->spells->cracked_power_core || player->spells->cracked_power_core->cooldown_remaining > 0)) {
          player->spells->demonic_rune->StartCast();
        }
        // Super Mana Potion
        if ((player->fight_time > 5 || player->stats->mp5 == 0) && player->spells->super_mana_potion != NULL &&
            (player->stats->max_mana - player->stats->mana) > player->spells->super_mana_potion->average_mana_value &&
            player->spells->super_mana_potion->Ready()) {
          player->spells->super_mana_potion->StartCast();
        }

        // Spells on the GCD
        if (player->gcd_remaining <= 0) {
          if (player->settings->is_single_target) {
            const bool kNotEnoughTimeForFillerSpell = kFightTimeRemaining < player->filler->GetCastTime();

            // Map of spells with their predicted Damage as the value. This is
            // used by the sim to determine what the best spell to Cast is.
            std::map<std::shared_ptr<Spell>, double> predicted_damage_of_spells;

            // If the sim is choosing the rotation for the user then predict the
            // Damage of the three filler spells if they're available (maybe
            // just skip Searing Pain to save time, there's no way it will ever
            // be the best spell to Cast)
            if (player->settings->sim_choosing_rotation) {
              if (kFightTimeRemaining >= player->spells->shadow_bolt->GetCastTime()) {
                predicted_damage_of_spells.insert(
                    std::make_pair(player->spells->shadow_bolt, player->spells->shadow_bolt->PredictDamage()));
              }
              if (kFightTimeRemaining >= player->spells->incinerate->GetCastTime()) {
                predicted_damage_of_spells.insert(
                    std::make_pair(player->spells->incinerate, player->spells->incinerate->PredictDamage()));
              }
              if (kFightTimeRemaining >= player->spells->searing_pain->GetCastTime()) {
                predicted_damage_of_spells.insert(
                    std::make_pair(player->spells->searing_pain, player->spells->searing_pain->PredictDamage()));
              }
            }

            // Cast Conflagrate if there's not enough time for another filler
            // and Immolate is up
            if (kNotEnoughTimeForFillerSpell && player->spells->conflagrate != NULL &&
                player->auras->immolate != NULL && player->spells->conflagrate->CanCast() &&
                player->auras->immolate->active) {
              SelectedSpellHandler(player->spells->conflagrate, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Shadowburn if there's not enough time for another filler
            if (player->gcd_remaining <= 0 && kNotEnoughTimeForFillerSpell && player->spells->shadowburn != NULL &&
                player->spells->shadowburn->CanCast()) {
              SelectedSpellHandler(player->spells->shadowburn, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Death Coil if there's not enough time for another filler
            if (player->gcd_remaining <= 0 && kNotEnoughTimeForFillerSpell && player->spells->death_coil != NULL &&
                player->spells->death_coil->CanCast()) {
              SelectedSpellHandler(player->spells->death_coil, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Curse of the Elements or Curse of Recklessness if they're
            // the selected curse and they're not active
            if (kFightTimeRemaining >= 10 && player->gcd_remaining <= 0 &&
                (player->curse_spell == player->spells->curse_of_recklessness ||
                 player->curse_spell == player->spells->curse_of_the_elements) &&
                !player->curse_aura->active && player->curse_spell->CanCast()) {
              if (player->curse_spell->HasEnoughMana()) {
                player->curse_spell->StartCast();
              } else {
                player->CastLifeTapOrDarkPact();
              }
            }
            // Cast Curse of Doom if it's the selected curse and there's more
            // than 60 seconds remaining
            if (player->gcd_remaining <= 0 && kFightTimeRemaining > 60 &&
                player->curse_spell == player->spells->curse_of_doom && !player->auras->curse_of_doom->active &&
                player->spells->curse_of_doom->CanCast()) {
              SelectedSpellHandler(player->spells->curse_of_doom, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Curse of Agony if CoA is the selected curse or if Curse of
            // Doom is the selected curse and there's less than 60 seconds
            // remaining of the fight
            if (player->gcd_remaining <= 0 && player->auras->curse_of_agony != NULL &&
                !player->auras->curse_of_agony->active && player->spells->curse_of_agony->CanCast() &&
                kFightTimeRemaining > player->auras->curse_of_agony->minimum_duration &&
                ((player->curse_spell == player->spells->curse_of_doom && !player->auras->curse_of_doom->active &&
                  (player->spells->curse_of_doom->cooldown_remaining >
                       player->auras->curse_of_agony->minimum_duration ||
                   kFightTimeRemaining < 60)) ||
                 player->curse_spell == player->spells->curse_of_agony)) {
              SelectedSpellHandler(player->spells->curse_of_agony, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Corruption if Corruption isn't up or if it will expire
            // before the Cast finishes (if no instant Corruption)
            if (player->gcd_remaining <= 0 && player->spells->corruption != NULL &&
                (!player->auras->corruption->active ||
                 (player->auras->corruption->ticks_remaining == 1 &&
                  player->auras->corruption->tick_timer_remaining < player->spells->corruption->GetCastTime())) &&
                player->spells->corruption->CanCast() &&
                (kFightTimeRemaining - player->spells->corruption->GetCastTime()) >=
                    player->auras->corruption->minimum_duration) {
              SelectedSpellHandler(player->spells->corruption, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Shadow Bolt if Shadow Trance (Nightfall) is active and
            // Corruption is active as well to avoid potentially wasting another
            // Nightfall proc
            if (player->gcd_remaining <= 0 && player->spells->shadow_bolt != NULL &&
                player->auras->shadow_trance != NULL && player->auras->shadow_trance->active &&
                player->auras->corruption->active && player->spells->shadow_bolt->CanCast()) {
              SelectedSpellHandler(player->spells->shadow_bolt, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Unstable Affliction if it's not up or if it's about to
            // expire
            if (player->gcd_remaining <= 0 && player->spells->unstable_affliction != NULL &&
                player->spells->unstable_affliction->CanCast() &&
                (!player->auras->unstable_affliction->active ||
                 (player->auras->unstable_affliction->ticks_remaining == 1 &&
                  player->auras->unstable_affliction->tick_timer_remaining <
                      player->spells->unstable_affliction->GetCastTime())) &&
                (kFightTimeRemaining - player->spells->unstable_affliction->GetCastTime()) >=
                    player->auras->unstable_affliction->minimum_duration) {
              SelectedSpellHandler(player->spells->unstable_affliction, predicted_damage_of_spells,
                                   kFightTimeRemaining);
            }
            // Cast Siphon Life if it's not up (todo: add option to only Cast it
            // while ISB is active if not using custom ISB uptime %)
            if (player->gcd_remaining <= 0 && player->spells->siphon_life != NULL &&
                !player->auras->siphon_life->active && player->spells->siphon_life->CanCast() &&
                kFightTimeRemaining >= player->auras->siphon_life->minimum_duration) {
              SelectedSpellHandler(player->spells->siphon_life, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Immolate if it's not up or about to expire
            if (player->gcd_remaining <= 0 && player->spells->immolate != NULL && player->spells->immolate->CanCast() &&
                (!player->auras->immolate->active ||
                 (player->auras->immolate->ticks_remaining == 1 &&
                  player->auras->immolate->tick_timer_remaining < player->spells->immolate->GetCastTime())) &&
                (kFightTimeRemaining - player->spells->immolate->GetCastTime()) >=
                    player->auras->immolate->minimum_duration) {
              SelectedSpellHandler(player->spells->immolate, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Shadow Bolt if Shadow Trance (Nightfall) is active
            if (player->gcd_remaining <= 0 && player->spells->shadow_bolt != NULL &&
                player->auras->shadow_trance != NULL && player->auras->shadow_trance->active &&
                player->spells->shadow_bolt->CanCast()) {
              SelectedSpellHandler(player->spells->shadow_bolt, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Shadowfury
            if (player->gcd_remaining <= 0 && player->spells->shadowfury != NULL &&
                player->spells->shadowfury->CanCast()) {
              SelectedSpellHandler(player->spells->shadowfury, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast filler spell if sim is not choosing the rotation for the
            // user or if the predicted_damage_of_spells map is empty
            if (player->gcd_remaining <= 0 &&
                ((!kNotEnoughTimeForFillerSpell && !player->settings->sim_choosing_rotation) ||
                 predicted_damage_of_spells.size() == 0) &&
                player->filler->CanCast()) {
              SelectedSpellHandler(player->filler, predicted_damage_of_spells, kFightTimeRemaining);
            }

            // If the predicted_damage_of_spells map is not empty then check now
            // which spell would be the best to Cast
            if (player->gcd_remaining <= 0 && player->cast_time_remaining <= 0 &&
                predicted_damage_of_spells.size() != 0) {
              std::shared_ptr<Spell> max_damage_spell;
              double max_damage_spell_value = 0;

              for (std::map<std::shared_ptr<Spell>, double>::iterator it = predicted_damage_of_spells.begin();
                   it != predicted_damage_of_spells.end(); it++) {
                if (it->second > max_damage_spell_value &&
                    (kFightTimeRemaining > player->GetGcdValue(it->first) || it->first->HasEnoughMana())) {
                  max_damage_spell = it->first;
                  max_damage_spell_value = it->second;
                }
              }

              // If a max Damage spell was not found or if the max Damage spell
              // isn't Ready (no mana), then Cast Life Tap
              if (max_damage_spell != NULL && max_damage_spell->HasEnoughMana()) {
                CastSelectedSpell(max_damage_spell, kFightTimeRemaining, max_damage_spell_value);
              } else {
                player->CastLifeTapOrDarkPact();
              }
            }
          }
          // AoE (currently just does Seed of Corruption by default)
          else {
            if (player->spells->seed_of_corruption->Ready()) {
              player->UseCooldowns(kFightTimeRemaining);
              player->spells->seed_of_corruption->StartCast();
            } else {
              player->CastLifeTapOrDarkPact();
            }
          }
        }
      }

      // Pet
      if (player->pet != NULL && player->pet->mode == PetMode::kAggressive) {
        // Auto Attack
        if (player->pet->spells->melee != NULL && player->pet->spells->melee->Ready()) {
          player->pet->spells->melee->StartCast();
        }
        // Felguard Cleave
        if (player->pet->spells->cleave != NULL && player->pet->spells->cleave->Ready()) {
          player->pet->spells->cleave->StartCast();
        }
        // Succubus Lash of Pain
        if (player->pet->spells->lash_of_pain != NULL && player->pet->spells->lash_of_pain->Ready() &&
            (player->settings->using_lash_of_pain_on_cooldown ||
             (!player->settings->using_custom_isb_uptime &&
              (player->auras->improved_shadow_bolt == NULL || !player->auras->improved_shadow_bolt->active)))) {
          player->pet->spells->lash_of_pain->StartCast();
        }
        // Imp Firebolt
        if (player->pet->spells->firebolt != NULL && player->pet->spells->firebolt->Ready()) {
          player->pet->spells->firebolt->StartCast();
        }
      }

      if (PassTime() <= 0) {
        std::cout << "Iteration " << std::to_string(player->iteration)
                  << " fightTime: " << std::to_string(player->fight_time) << "/" << std::to_string(kFightLength)
                  << " PassTime() returned <= 0" << std::endl;
        player->ThrowError(
            "The simulation got stuck in an endless loop. If you'd like to "
            "help with fixing this bug then please "
            "export your current settings and send it to Kristofer#8003 on "
            "Discord.");
      }
    }

    if (player->ShouldWriteToCombatLog()) {
      player->CombatLog("Fight end");
    }

    player->EndAuras();

    player->total_duration += kFightLength;
    const double kDps = player->iteration_damage / static_cast<double>(kFightLength);
    if (kDps > max_dps) {
      max_dps = kDps;
    }
    if (kDps < min_dps) {
      min_dps = kDps;
    }
    dps_vector.push_back(kDps);

    // Only send the iteration's dps to the web worker if we're doing a normal
    // simulation (this is just for the dps histogram)
    if (settings->simulation_type == SimulationType::kNormal && player->customStat == "normal") {
      DpsUpdate(kDps);
    }

    if (player->iteration % static_cast<int>(std::floor(settings->iterations / 100.0)) == 0) {
      SimulationUpdate(player->iteration, settings->iterations, Median(dps_vector), player->settings->item_id,
                       player->customStat.c_str());
    }
  }

  delete[] random_seeds;

  // Send the contents of the combat log to the web worker
  if (player->settings->equipped_item_simulation) {
    player->SendCombatLogEntries();
  }

  // Send the combat log breakdown info
  if (player->settings->recording_combat_log_breakdown && player->settings->equipped_item_simulation) {
    for (std::map<std::string, std::unique_ptr<CombatLogBreakdown>>::iterator it = player->combat_log_breakdown.begin();
         it != player->combat_log_breakdown.end(); it++) {
      if (it->second->iteration_damage > 0 || it->second->iteration_mana_gain > 0) {
        player->PostIterationDamageAndMana(it->first);
      }
      PostCombatLogBreakdown(it->second->name.c_str(), it->second->casts, it->second->crits, it->second->misses,
                             it->second->count, it->second->uptime, it->second->dodge, it->second->glancing_blows);
    }
  }
  SimulationEnd(Median(dps_vector), min_dps, max_dps, player->settings->item_id, settings->iterations,
                player->total_duration, player->customStat.c_str());
}

double Simulation::PassTime() {
  double time = player->cast_time_remaining;
  if (time <= 0) time = player->gcd_remaining;

  // Find the lowest time until the next action needs to be taken
  // Pet
  if (player->pet != NULL) {
    if ((player->talents->dark_pact > 0 || player->pet->mode == PetMode::kAggressive) &&
        player->pet->spirit_tick_timer_remaining < time)
      time = player->pet->spirit_tick_timer_remaining;

    // Pet's attacks/abilities and such
    if (player->pet->mode == PetMode::kAggressive) {
      if (player->pet->spells->melee != NULL && player->pet->spells->melee->cooldown_remaining > 0 &&
          player->pet->spells->melee->cooldown_remaining < time)
        time = player->pet->spells->melee->cooldown_remaining;
      else if (player->pet->cast_time_remaining > 0 && player->pet->cast_time_remaining < time)
        time = player->pet->cast_time_remaining;
      if (player->pet->spells->lash_of_pain != NULL && player->pet->spells->lash_of_pain->cooldown_remaining > 0 &&
          player->pet->spells->lash_of_pain->cooldown_remaining < time)
        time = player->pet->spells->lash_of_pain->cooldown_remaining;
      else if (player->pet->spells->cleave != NULL && player->pet->spells->cleave->cooldown_remaining > 0 &&
               player->pet->spells->cleave->cooldown_remaining < time)
        time = player->pet->spells->cleave->cooldown_remaining;
      if (player->pet->auras->black_book != NULL && player->pet->auras->black_book->active &&
          player->pet->auras->black_book->duration_remaining < time)
        time = player->pet->auras->black_book->duration_remaining;
    }
  }

#pragma region Spells
  if (player->spells->curse_of_doom != NULL && player->spells->curse_of_doom->cooldown_remaining > 0 &&
      player->spells->curse_of_doom->cooldown_remaining < time)
    time = player->spells->curse_of_doom->cooldown_remaining;
  if (player->spells->conflagrate != NULL && player->spells->conflagrate->cooldown_remaining > 0 &&
      player->spells->conflagrate->cooldown_remaining < time)
    time = player->spells->conflagrate->cooldown_remaining;
  if (player->spells->shadowburn != NULL && player->spells->shadowburn->cooldown_remaining > 0 &&
      player->spells->shadowburn->cooldown_remaining < time)
    time = player->spells->shadowburn->cooldown_remaining;
  if (player->spells->death_coil != NULL && player->spells->death_coil->cooldown_remaining > 0 &&
      player->spells->death_coil->cooldown_remaining < time)
    time = player->spells->death_coil->cooldown_remaining;
  if (player->spells->shadowfury != NULL && player->spells->shadowfury->cooldown_remaining > 0 &&
      player->spells->shadowfury->cooldown_remaining < time)
    time = player->spells->shadowfury->cooldown_remaining;
  if (player->spells->amplify_curse != NULL && player->spells->amplify_curse->cooldown_remaining > 0 &&
      player->spells->amplify_curse->cooldown_remaining < time)
    time = player->spells->amplify_curse->cooldown_remaining;
  if (player->spells->destruction_potion != NULL && player->spells->destruction_potion->cooldown_remaining > 0 &&
      player->spells->destruction_potion->cooldown_remaining < time)
    time = player->spells->destruction_potion->cooldown_remaining;
  if (player->spells->super_mana_potion != NULL && player->spells->super_mana_potion->cooldown_remaining > 0 &&
      player->spells->super_mana_potion->cooldown_remaining < time)
    time = player->spells->super_mana_potion->cooldown_remaining;
  if (player->spells->demonic_rune != NULL && player->spells->demonic_rune->cooldown_remaining > 0 &&
      player->spells->demonic_rune->cooldown_remaining < time)
    time = player->spells->demonic_rune->cooldown_remaining;
  if (player->spells->flame_cap != NULL && player->spells->flame_cap->cooldown_remaining > 0 &&
      player->spells->flame_cap->cooldown_remaining < time)
    time = player->spells->flame_cap->cooldown_remaining;
  if (player->spells->blood_fury != NULL && player->spells->blood_fury->cooldown_remaining > 0 &&
      player->spells->blood_fury->cooldown_remaining < time)
    time = player->spells->blood_fury->cooldown_remaining;
  if (player->spells->drums_of_battle != NULL && player->spells->drums_of_battle->cooldown_remaining > 0 &&
      player->spells->drums_of_battle->cooldown_remaining < time)
    time = player->spells->drums_of_battle->cooldown_remaining;
  if (player->spells->drums_of_war != NULL && player->spells->drums_of_war->cooldown_remaining > 0 &&
      player->spells->drums_of_war->cooldown_remaining < time)
    time = player->spells->drums_of_war->cooldown_remaining;
  if (player->spells->drums_of_restoration != NULL && player->spells->drums_of_restoration->cooldown_remaining > 0 &&
      player->spells->drums_of_restoration->cooldown_remaining < time)
    time = player->spells->drums_of_restoration->cooldown_remaining;
  if (player->spells->blade_of_wizardry != NULL && player->spells->blade_of_wizardry->cooldown_remaining > 0 &&
      player->spells->blade_of_wizardry->cooldown_remaining < time)
    time = player->spells->blade_of_wizardry->cooldown_remaining;
  if (player->spells->shattered_sun_pendant_of_acumen != NULL &&
      player->spells->shattered_sun_pendant_of_acumen->cooldown_remaining > 0 &&
      player->spells->shattered_sun_pendant_of_acumen->cooldown_remaining < time)
    time = player->spells->shattered_sun_pendant_of_acumen->cooldown_remaining;
  if (player->spells->robe_of_the_elder_scribes != NULL &&
      player->spells->robe_of_the_elder_scribes->cooldown_remaining > 0 &&
      player->spells->robe_of_the_elder_scribes->cooldown_remaining < time)
    time = player->spells->robe_of_the_elder_scribes->cooldown_remaining;
  if (player->spells->mystical_skyfire_diamond != NULL &&
      player->spells->mystical_skyfire_diamond->cooldown_remaining > 0 &&
      player->spells->mystical_skyfire_diamond->cooldown_remaining < time)
    time = player->spells->mystical_skyfire_diamond->cooldown_remaining;
  if (player->spells->insightful_earthstorm_diamond != NULL &&
      player->spells->insightful_earthstorm_diamond->cooldown_remaining > 0 &&
      player->spells->insightful_earthstorm_diamond->cooldown_remaining < time)
    time = player->spells->insightful_earthstorm_diamond->cooldown_remaining;
  if (player->spells->timbals_focusing_crystal != NULL &&
      player->spells->timbals_focusing_crystal->cooldown_remaining > 0 &&
      player->spells->timbals_focusing_crystal->cooldown_remaining < time)
    time = player->spells->timbals_focusing_crystal->cooldown_remaining;
  if (player->spells->mark_of_defiance != NULL && player->spells->mark_of_defiance->cooldown_remaining > 0 &&
      player->spells->mark_of_defiance->cooldown_remaining < time)
    time = player->spells->mark_of_defiance->cooldown_remaining;
  if (player->spells->the_lightning_capacitor != NULL &&
      player->spells->the_lightning_capacitor->cooldown_remaining > 0 &&
      player->spells->the_lightning_capacitor->cooldown_remaining < time)
    time = player->spells->the_lightning_capacitor->cooldown_remaining;
  if (player->spells->quagmirrans_eye != NULL && player->spells->quagmirrans_eye->cooldown_remaining > 0 &&
      player->spells->quagmirrans_eye->cooldown_remaining < time)
    time = player->spells->quagmirrans_eye->cooldown_remaining;
  if (player->spells->shiffars_nexus_horn != NULL && player->spells->shiffars_nexus_horn->cooldown_remaining > 0 &&
      player->spells->shiffars_nexus_horn->cooldown_remaining < time)
    time = player->spells->shiffars_nexus_horn->cooldown_remaining;
  if (player->spells->sextant_of_unstable_currents != NULL &&
      player->spells->sextant_of_unstable_currents->cooldown_remaining > 0 &&
      player->spells->sextant_of_unstable_currents->cooldown_remaining < time)
    time = player->spells->sextant_of_unstable_currents->cooldown_remaining;
  if (player->spells->band_of_the_eternal_sage != NULL &&
      player->spells->band_of_the_eternal_sage->cooldown_remaining > 0 &&
      player->spells->band_of_the_eternal_sage->cooldown_remaining < time)
    time = player->spells->band_of_the_eternal_sage->cooldown_remaining;
  if (player->spells->chipped_power_core != NULL && player->spells->chipped_power_core->cooldown_remaining > 0 &&
      player->spells->chipped_power_core->cooldown_remaining < time)
    time = player->spells->chipped_power_core->cooldown_remaining;
  if (player->spells->cracked_power_core != NULL && player->spells->cracked_power_core->cooldown_remaining > 0 &&
      player->spells->cracked_power_core->cooldown_remaining < time)
    time = player->spells->cracked_power_core->cooldown_remaining;
  if (player->spells->mana_tide_totem != NULL && player->spells->mana_tide_totem->cooldown_remaining > 0 &&
      player->spells->mana_tide_totem->cooldown_remaining < time)
    time = player->spells->mana_tide_totem->cooldown_remaining;
  for (std::vector<std::shared_ptr<Spell>>::iterator it = player->spells->power_infusion.begin();
       it != player->spells->power_infusion.end(); it++) {
    if ((*it)->cooldown_remaining > 0 && (*it)->cooldown_remaining < time) time = (*it)->cooldown_remaining;
  }
  for (std::vector<std::shared_ptr<Spell>>::iterator it = player->spells->bloodlust.begin();
       it != player->spells->bloodlust.end(); it++) {
    if ((*it)->cooldown_remaining > 0 && (*it)->cooldown_remaining < time) time = (*it)->cooldown_remaining;
  }
  for (std::vector<std::shared_ptr<Spell>>::iterator it = player->spells->innervate.begin();
       it != player->spells->innervate.end(); it++) {
    if ((*it)->cooldown_remaining > 0 && (*it)->cooldown_remaining < time) time = (*it)->cooldown_remaining;
  }
#pragma endregion

#pragma region Auras& Dots
  if (player->auras->corruption != NULL && player->auras->corruption->active &&
      player->auras->corruption->tick_timer_remaining < time)
    time = player->auras->corruption->tick_timer_remaining;
  if (player->auras->unstable_affliction != NULL && player->auras->unstable_affliction->active &&
      player->auras->unstable_affliction->tick_timer_remaining < time)
    time = player->auras->unstable_affliction->tick_timer_remaining;
  if (player->auras->siphon_life != NULL && player->auras->siphon_life->active &&
      player->auras->siphon_life->tick_timer_remaining < time)
    time = player->auras->siphon_life->tick_timer_remaining;
  if (player->auras->immolate != NULL && player->auras->immolate->active &&
      player->auras->immolate->tick_timer_remaining < time)
    time = player->auras->immolate->tick_timer_remaining;
  if (player->auras->curse_of_agony != NULL && player->auras->curse_of_agony->active &&
      player->auras->curse_of_agony->tick_timer_remaining < time)
    time = player->auras->curse_of_agony->tick_timer_remaining;
  if (player->auras->curse_of_doom != NULL && player->auras->curse_of_doom->active &&
      player->auras->curse_of_doom->tick_timer_remaining < time)
    time = player->auras->curse_of_doom->tick_timer_remaining;
  if (player->auras->improved_shadow_bolt != NULL && player->auras->improved_shadow_bolt->active &&
      player->auras->improved_shadow_bolt->duration_remaining < time)
    time = player->auras->improved_shadow_bolt->duration_remaining;
  if (player->auras->curse_of_the_elements != NULL && player->auras->curse_of_the_elements->active &&
      player->auras->curse_of_the_elements->duration_remaining < time)
    time = player->auras->curse_of_the_elements->duration_remaining;
  if (player->auras->curse_of_recklessness != NULL && player->auras->curse_of_recklessness->active &&
      player->auras->curse_of_recklessness->duration_remaining < time)
    time = player->auras->curse_of_recklessness->duration_remaining;
  if (player->auras->shadow_trance != NULL && player->auras->shadow_trance->active &&
      player->auras->shadow_trance->duration_remaining < time)
    time = player->auras->shadow_trance->duration_remaining;
  if (player->auras->amplify_curse != NULL && player->auras->amplify_curse->active &&
      player->auras->amplify_curse->duration_remaining < time)
    time = player->auras->amplify_curse->duration_remaining;
  if (player->auras->power_infusion != NULL && player->auras->power_infusion->active &&
      player->auras->power_infusion->duration_remaining < time)
    time = player->auras->power_infusion->duration_remaining;
  if (player->auras->innervate != NULL && player->auras->innervate->active &&
      player->auras->innervate->duration_remaining < time)
    time = player->auras->innervate->duration_remaining;
  if (player->auras->blood_fury != NULL && player->auras->blood_fury->active &&
      player->auras->blood_fury->duration_remaining < time)
    time = player->auras->blood_fury->duration_remaining;
  if (player->auras->destruction_potion != NULL && player->auras->destruction_potion->active &&
      player->auras->destruction_potion->duration_remaining < time)
    time = player->auras->destruction_potion->duration_remaining;
  if (player->auras->flame_cap != NULL && player->auras->flame_cap->active &&
      player->auras->flame_cap->duration_remaining < time)
    time = player->auras->flame_cap->duration_remaining;
  if (player->auras->bloodlust != NULL && player->auras->bloodlust->active &&
      player->auras->bloodlust->duration_remaining < time)
    time = player->auras->bloodlust->duration_remaining;
  if (player->auras->drums_of_battle != NULL && player->auras->drums_of_battle->active &&
      player->auras->drums_of_battle->duration_remaining < time)
    time = player->auras->drums_of_battle->duration_remaining;
  if (player->auras->drums_of_war != NULL && player->auras->drums_of_war->active &&
      player->auras->drums_of_war->duration_remaining < time)
    time = player->auras->drums_of_war->duration_remaining;
  if (player->auras->drums_of_restoration != NULL && player->auras->drums_of_restoration->active &&
      player->auras->drums_of_restoration->tick_timer_remaining < time)
    time = player->auras->drums_of_restoration->tick_timer_remaining;
  if (player->auras->band_of_the_eternal_sage != NULL && player->auras->band_of_the_eternal_sage->active &&
      player->auras->band_of_the_eternal_sage->duration_remaining < time)
    time = player->auras->band_of_the_eternal_sage->duration_remaining;
  if (player->auras->wrath_of_cenarius != NULL && player->auras->wrath_of_cenarius->active &&
      player->auras->wrath_of_cenarius->duration_remaining < time)
    time = player->auras->wrath_of_cenarius->duration_remaining;
  if (player->auras->blade_of_wizardry != NULL && player->auras->blade_of_wizardry->active &&
      player->auras->blade_of_wizardry->duration_remaining < time)
    time = player->auras->blade_of_wizardry->duration_remaining;
  if (player->auras->shattered_sun_pendant_of_acumen != NULL &&
      player->auras->shattered_sun_pendant_of_acumen->active &&
      player->auras->shattered_sun_pendant_of_acumen->duration_remaining < time)
    time = player->auras->shattered_sun_pendant_of_acumen->duration_remaining;
  if (player->auras->robe_of_the_elder_scribes != NULL && player->auras->robe_of_the_elder_scribes->active &&
      player->auras->robe_of_the_elder_scribes->duration_remaining < time)
    time = player->auras->robe_of_the_elder_scribes->duration_remaining;
  if (player->auras->mystical_skyfire_diamond != NULL && player->auras->mystical_skyfire_diamond->active &&
      player->auras->mystical_skyfire_diamond->duration_remaining < time)
    time = player->auras->mystical_skyfire_diamond->duration_remaining;
  if (player->auras->eye_of_magtheridon != NULL && player->auras->eye_of_magtheridon->active &&
      player->auras->eye_of_magtheridon->duration_remaining < time)
    time = player->auras->eye_of_magtheridon->duration_remaining;
  if (player->auras->sextant_of_unstable_currents != NULL && player->auras->sextant_of_unstable_currents->active &&
      player->auras->sextant_of_unstable_currents->duration_remaining < time)
    time = player->auras->sextant_of_unstable_currents->duration_remaining;
  if (player->auras->quagmirrans_eye != NULL && player->auras->quagmirrans_eye->active &&
      player->auras->quagmirrans_eye->duration_remaining < time)
    time = player->auras->quagmirrans_eye->duration_remaining;
  if (player->auras->shiffars_nexus_horn != NULL && player->auras->shiffars_nexus_horn->active &&
      player->auras->shiffars_nexus_horn->duration_remaining < time)
    time = player->auras->shiffars_nexus_horn->duration_remaining;
  if (player->auras->ashtongue_talisman_of_shadows != NULL && player->auras->ashtongue_talisman_of_shadows->active &&
      player->auras->ashtongue_talisman_of_shadows->duration_remaining < time)
    time = player->auras->ashtongue_talisman_of_shadows->duration_remaining;
  if (player->auras->darkmoon_card_crusade != NULL && player->auras->darkmoon_card_crusade->active &&
      player->auras->darkmoon_card_crusade->duration_remaining < time)
    time = player->auras->darkmoon_card_crusade->duration_remaining;
  if (player->auras->flameshadow != NULL && player->auras->flameshadow->active &&
      player->auras->flameshadow->duration_remaining < time)
    time = player->auras->flameshadow->duration_remaining;
  if (player->auras->shadowflame != NULL && player->auras->shadowflame->active &&
      player->auras->shadowflame->duration_remaining < time)
    time = player->auras->shadowflame->duration_remaining;
  if (player->auras->spellstrike != NULL && player->auras->spellstrike->active &&
      player->auras->spellstrike->duration_remaining < time)
    time = player->auras->spellstrike->duration_remaining;
  if (player->auras->mana_etched_4_set != NULL && player->auras->mana_etched_4_set->active &&
      player->auras->mana_etched_4_set->duration_remaining < time)
    time = player->auras->mana_etched_4_set->duration_remaining;
  if (player->auras->chipped_power_core != NULL && player->auras->chipped_power_core->active &&
      player->auras->chipped_power_core->duration_remaining < time)
    time = player->auras->chipped_power_core->duration_remaining;
  if (player->auras->cracked_power_core != NULL && player->auras->cracked_power_core->active &&
      player->auras->cracked_power_core->duration_remaining < time)
    time = player->auras->cracked_power_core->duration_remaining;
  if (player->auras->mana_tide_totem != NULL && player->auras->mana_tide_totem->active &&
      player->auras->mana_tide_totem->duration_remaining < time)
    time = player->auras->mana_tide_totem->duration_remaining;
#pragma endregion

  // MP5
  if (player->mp5_timer < time && player->mp5_timer > 0) {
    time = player->mp5_timer;
  }

  // Trinkets
  for (auto& trinket_ptr : player->trinkets) {
    if (trinket_ptr->active && trinket_ptr->duration_remaining < time) {
      time = trinket_ptr->duration_remaining;
    }
    if (trinket_ptr->cooldown_remaining > 0 && trinket_ptr->cooldown_remaining < time) {
      time = trinket_ptr->cooldown_remaining;
    }
  }

  // Pass time
  // This needs to be the first modified value since the time in combat needs to
  // be updated before spells start dealing Damage/auras expiring etc. for the
  // combat logging.
  player->fight_time += time;
  player->cast_time_remaining -= time;

  // Pet
  if (player->pet != NULL) player->pet->Tick(time);

// Auras need to tick before Spells because otherwise you'll, for example,
// finish casting Corruption and then immediately afterwards, in the same
// millisecond, immediately tick down the aura This was also causing buffs like
// e.g. the t4 4pc buffs to expire sooner than they should.
#pragma region Auras
  if (player->auras->corruption != NULL && player->auras->corruption->active &&
      player->auras->corruption->tick_timer_remaining > 0)
    player->auras->corruption->Tick(time);
  if (player->auras->unstable_affliction != NULL && player->auras->unstable_affliction->active &&
      player->auras->unstable_affliction->tick_timer_remaining > 0)
    player->auras->unstable_affliction->Tick(time);
  if (player->auras->siphon_life != NULL && player->auras->siphon_life->active &&
      player->auras->siphon_life->tick_timer_remaining > 0)
    player->auras->siphon_life->Tick(time);
  if (player->auras->immolate != NULL && player->auras->immolate->active &&
      player->auras->immolate->tick_timer_remaining > 0)
    player->auras->immolate->Tick(time);
  if (player->auras->curse_of_agony != NULL && player->auras->curse_of_agony->active &&
      player->auras->curse_of_agony->tick_timer_remaining > 0)
    player->auras->curse_of_agony->Tick(time);
  if (player->auras->curse_of_doom != NULL && player->auras->curse_of_doom->active &&
      player->auras->curse_of_doom->tick_timer_remaining > 0)
    player->auras->curse_of_doom->Tick(time);
  if (player->auras->improved_shadow_bolt != NULL && player->auras->improved_shadow_bolt->active)
    player->auras->improved_shadow_bolt->Tick(time);
  if (player->auras->curse_of_the_elements != NULL && player->auras->curse_of_the_elements->active)
    player->auras->curse_of_the_elements->Tick(time);
  if (player->auras->curse_of_recklessness != NULL && player->auras->curse_of_recklessness->active)
    player->auras->curse_of_recklessness->Tick(time);
  if (player->auras->shadow_trance != NULL && player->auras->shadow_trance->active)
    player->auras->shadow_trance->Tick(time);
  if (player->auras->amplify_curse != NULL && player->auras->amplify_curse->active)
    player->auras->amplify_curse->Tick(time);
  if (player->auras->power_infusion != NULL && player->auras->power_infusion->active)
    player->auras->power_infusion->Tick(time);
  if (player->auras->innervate != NULL && player->auras->innervate->active) player->auras->innervate->Tick(time);
  if (player->auras->blood_fury != NULL && player->auras->blood_fury->active) player->auras->blood_fury->Tick(time);
  if (player->auras->destruction_potion != NULL && player->auras->destruction_potion->active)
    player->auras->destruction_potion->Tick(time);
  if (player->auras->flame_cap != NULL && player->auras->flame_cap->active) player->auras->flame_cap->Tick(time);
  if (player->auras->bloodlust != NULL && player->auras->bloodlust->active) player->auras->bloodlust->Tick(time);
  if (player->auras->drums_of_battle != NULL && player->auras->drums_of_battle->active)
    player->auras->drums_of_battle->Tick(time);
  if (player->auras->drums_of_war != NULL && player->auras->drums_of_war->active)
    player->auras->drums_of_war->Tick(time);
  if (player->auras->drums_of_restoration != NULL && player->auras->drums_of_restoration->active)
    player->auras->drums_of_restoration->Tick(time);
  if (player->auras->band_of_the_eternal_sage != NULL && player->auras->band_of_the_eternal_sage->active)
    player->auras->band_of_the_eternal_sage->Tick(time);
  if (player->auras->wrath_of_cenarius != NULL && player->auras->wrath_of_cenarius->active)
    player->auras->wrath_of_cenarius->Tick(time);
  if (player->auras->blade_of_wizardry != NULL && player->auras->blade_of_wizardry->active)
    player->auras->blade_of_wizardry->Tick(time);
  if (player->auras->shattered_sun_pendant_of_acumen != NULL && player->auras->shattered_sun_pendant_of_acumen->active)
    player->auras->shattered_sun_pendant_of_acumen->Tick(time);
  if (player->auras->robe_of_the_elder_scribes != NULL && player->auras->robe_of_the_elder_scribes->active)
    player->auras->robe_of_the_elder_scribes->Tick(time);
  if (player->auras->mystical_skyfire_diamond != NULL && player->auras->mystical_skyfire_diamond->active)
    player->auras->mystical_skyfire_diamond->Tick(time);
  if (player->auras->eye_of_magtheridon != NULL && player->auras->eye_of_magtheridon->active)
    player->auras->eye_of_magtheridon->Tick(time);
  if (player->auras->sextant_of_unstable_currents != NULL && player->auras->sextant_of_unstable_currents->active)
    player->auras->sextant_of_unstable_currents->Tick(time);
  if (player->auras->quagmirrans_eye != NULL && player->auras->quagmirrans_eye->active)
    player->auras->quagmirrans_eye->Tick(time);
  if (player->auras->shiffars_nexus_horn != NULL && player->auras->shiffars_nexus_horn->active)
    player->auras->shiffars_nexus_horn->Tick(time);
  if (player->auras->ashtongue_talisman_of_shadows != NULL && player->auras->ashtongue_talisman_of_shadows->active)
    player->auras->ashtongue_talisman_of_shadows->Tick(time);
  if (player->auras->darkmoon_card_crusade != NULL && player->auras->darkmoon_card_crusade->active)
    player->auras->darkmoon_card_crusade->Tick(time);
  if (player->auras->the_lightning_capacitor != NULL && player->auras->the_lightning_capacitor->active)
    player->auras->the_lightning_capacitor->Tick(time);
  if (player->auras->flameshadow != NULL && player->auras->flameshadow->active) player->auras->flameshadow->Tick(time);
  if (player->auras->shadowflame != NULL && player->auras->shadowflame->active) player->auras->shadowflame->Tick(time);
  if (player->auras->spellstrike != NULL && player->auras->spellstrike->active) player->auras->spellstrike->Tick(time);
  if (player->auras->mana_etched_4_set != NULL && player->auras->mana_etched_4_set->active)
    player->auras->mana_etched_4_set->Tick(time);
  if (player->auras->chipped_power_core != NULL && player->auras->chipped_power_core->active)
    player->auras->chipped_power_core->Tick(time);
  if (player->auras->cracked_power_core != NULL && player->auras->cracked_power_core->active)
    player->auras->cracked_power_core->Tick(time);
  if (player->auras->mana_tide_totem != NULL && player->auras->mana_tide_totem->active)
    player->auras->mana_tide_totem->Tick(time);
#pragma endregion

#pragma region Spells
  if (player->spells->seed_of_corruption != NULL && player->spells->seed_of_corruption->casting)
    player->spells->seed_of_corruption->Tick(time);
  if (player->spells->shadow_bolt != NULL && player->spells->shadow_bolt->casting)
    player->spells->shadow_bolt->Tick(time);
  if (player->spells->incinerate != NULL && player->spells->incinerate->casting) player->spells->incinerate->Tick(time);
  if (player->spells->searing_pain != NULL && player->spells->searing_pain->casting)
    player->spells->searing_pain->Tick(time);
  if (player->spells->corruption != NULL && player->spells->corruption->casting) player->spells->corruption->Tick(time);
  if (player->spells->unstable_affliction != NULL && player->spells->unstable_affliction->casting)
    player->spells->unstable_affliction->Tick(time);
  if (player->spells->immolate != NULL && player->spells->immolate->casting) player->spells->immolate->Tick(time);
  if (player->spells->curse_of_doom != NULL &&
      (player->spells->curse_of_doom->cooldown_remaining > 0 || player->spells->curse_of_doom->casting))
    player->spells->curse_of_doom->Tick(time);
  if (player->spells->conflagrate != NULL &&
      (player->spells->conflagrate->cooldown_remaining > 0 || player->spells->conflagrate->casting))
    player->spells->conflagrate->Tick(time);
  if (player->spells->shadowburn != NULL &&
      (player->spells->shadowburn->cooldown_remaining > 0 || player->spells->shadowburn->casting))
    player->spells->shadowburn->Tick(time);
  if (player->spells->death_coil != NULL &&
      (player->spells->death_coil->cooldown_remaining > 0 || player->spells->death_coil->casting))
    player->spells->death_coil->Tick(time);
  if (player->spells->shadowfury != NULL &&
      (player->spells->shadowfury->cooldown_remaining > 0 || player->spells->shadowfury->casting))
    player->spells->shadowfury->Tick(time);
  if (player->spells->amplify_curse != NULL &&
      (player->spells->amplify_curse->cooldown_remaining > 0 || player->spells->amplify_curse->casting))
    player->spells->amplify_curse->Tick(time);
  if (player->spells->destruction_potion != NULL &&
      (player->spells->destruction_potion->cooldown_remaining > 0 || player->spells->destruction_potion->casting))
    player->spells->destruction_potion->Tick(time);
  if (player->spells->super_mana_potion != NULL &&
      (player->spells->super_mana_potion->cooldown_remaining > 0 || player->spells->super_mana_potion->casting))
    player->spells->super_mana_potion->Tick(time);
  if (player->spells->demonic_rune != NULL &&
      (player->spells->demonic_rune->cooldown_remaining > 0 || player->spells->demonic_rune->casting))
    player->spells->demonic_rune->Tick(time);
  if (player->spells->flame_cap != NULL &&
      (player->spells->flame_cap->cooldown_remaining > 0 || player->spells->flame_cap->casting))
    player->spells->flame_cap->Tick(time);
  if (player->spells->blood_fury != NULL &&
      (player->spells->blood_fury->cooldown_remaining > 0 || player->spells->blood_fury->casting))
    player->spells->blood_fury->Tick(time);
  if (player->spells->drums_of_battle != NULL &&
      (player->spells->drums_of_battle->cooldown_remaining > 0 || player->spells->drums_of_battle->casting))
    player->spells->drums_of_battle->Tick(time);
  if (player->spells->drums_of_war != NULL &&
      (player->spells->drums_of_war->cooldown_remaining > 0 || player->spells->drums_of_war->casting))
    player->spells->drums_of_war->Tick(time);
  if (player->spells->drums_of_restoration != NULL &&
      (player->spells->drums_of_restoration->cooldown_remaining > 0 || player->spells->drums_of_restoration->casting))
    player->spells->drums_of_restoration->Tick(time);
  if (player->spells->blade_of_wizardry != NULL &&
      (player->spells->blade_of_wizardry->cooldown_remaining > 0 || player->spells->blade_of_wizardry->casting))
    player->spells->blade_of_wizardry->Tick(time);
  if (player->spells->shattered_sun_pendant_of_acumen != NULL &&
      (player->spells->shattered_sun_pendant_of_acumen->cooldown_remaining > 0 ||
       player->spells->shattered_sun_pendant_of_acumen->casting))
    player->spells->shattered_sun_pendant_of_acumen->Tick(time);
  if (player->spells->robe_of_the_elder_scribes != NULL &&
      (player->spells->robe_of_the_elder_scribes->cooldown_remaining > 0 ||
       player->spells->robe_of_the_elder_scribes->casting))
    player->spells->robe_of_the_elder_scribes->Tick(time);
  if (player->spells->mystical_skyfire_diamond != NULL &&
      (player->spells->mystical_skyfire_diamond->cooldown_remaining > 0 ||
       player->spells->mystical_skyfire_diamond->casting))
    player->spells->mystical_skyfire_diamond->Tick(time);
  if (player->spells->insightful_earthstorm_diamond != NULL &&
      (player->spells->insightful_earthstorm_diamond->cooldown_remaining > 0 ||
       player->spells->insightful_earthstorm_diamond->casting))
    player->spells->insightful_earthstorm_diamond->Tick(time);
  if (player->spells->timbals_focusing_crystal != NULL &&
      (player->spells->timbals_focusing_crystal->cooldown_remaining > 0 ||
       player->spells->timbals_focusing_crystal->casting))
    player->spells->timbals_focusing_crystal->Tick(time);
  if (player->spells->mark_of_defiance != NULL &&
      (player->spells->mark_of_defiance->cooldown_remaining > 0 || player->spells->mark_of_defiance->casting))
    player->spells->mark_of_defiance->Tick(time);
  if (player->spells->the_lightning_capacitor != NULL &&
      (player->spells->the_lightning_capacitor->cooldown_remaining > 0 ||
       player->spells->the_lightning_capacitor->casting))
    player->spells->the_lightning_capacitor->Tick(time);
  if (player->spells->quagmirrans_eye != NULL &&
      (player->spells->quagmirrans_eye->cooldown_remaining > 0 || player->spells->quagmirrans_eye->casting))
    player->spells->quagmirrans_eye->Tick(time);
  if (player->spells->shiffars_nexus_horn != NULL &&
      (player->spells->shiffars_nexus_horn->cooldown_remaining > 0 || player->spells->shiffars_nexus_horn->casting))
    player->spells->shiffars_nexus_horn->Tick(time);
  if (player->spells->sextant_of_unstable_currents != NULL &&
      (player->spells->sextant_of_unstable_currents->cooldown_remaining > 0 ||
       player->spells->sextant_of_unstable_currents->casting))
    player->spells->sextant_of_unstable_currents->Tick(time);
  if (player->spells->band_of_the_eternal_sage != NULL &&
      (player->spells->band_of_the_eternal_sage->cooldown_remaining > 0 ||
       player->spells->band_of_the_eternal_sage->casting))
    player->spells->band_of_the_eternal_sage->Tick(time);
  if (player->spells->chipped_power_core != NULL &&
      (player->spells->chipped_power_core->cooldown_remaining > 0 || player->spells->chipped_power_core->casting))
    player->spells->chipped_power_core->Tick(time);
  if (player->spells->cracked_power_core != NULL &&
      (player->spells->cracked_power_core->cooldown_remaining > 0 || player->spells->cracked_power_core->casting))
    player->spells->cracked_power_core->Tick(time);
  if (player->spells->mana_tide_totem != NULL &&
      (player->spells->mana_tide_totem->cooldown_remaining > 0 || player->spells->mana_tide_totem->casting))
    player->spells->mana_tide_totem->Tick(time);
  for (std::vector<std::shared_ptr<Spell>>::iterator it = player->spells->power_infusion.begin();
       it != player->spells->power_infusion.end(); it++) {
    if ((*it)->cooldown_remaining > 0) (*it)->Tick(time);
  }
  for (std::vector<std::shared_ptr<Spell>>::iterator it = player->spells->bloodlust.begin();
       it != player->spells->bloodlust.end(); it++) {
    if ((*it)->cooldown_remaining > 0) (*it)->Tick(time);
  }
  for (std::vector<std::shared_ptr<Spell>>::iterator it = player->spells->innervate.begin();
       it != player->spells->innervate.end(); it++) {
    if ((*it)->cooldown_remaining > 0) (*it)->Tick(time);
  }
#pragma endregion

  for (auto& trinket : player->trinkets) {
    trinket->Tick(time);
  }

  player->gcd_remaining -= time;
  player->mp5_timer -= time;
  player->five_second_rule_timer -= time;
  if (player->mp5_timer <= 0) {
    player->mp5_timer = 5;

    if (player->stats->mp5 > 0 || player->five_second_rule_timer <= 0 ||
        (player->auras->innervate != NULL && player->auras->innervate->active)) {
      const bool kInnervateIsActive = player->auras->innervate != NULL && player->auras->innervate->active;
      const int kCurrentPlayerMana = player->stats->mana;

      // MP5
      if (player->stats->mp5 > 0) {
        player->stats->mana += player->stats->mp5;
      }
      // Spirit mana regen
      if (player->five_second_rule_timer <= 0 || kInnervateIsActive) {
        // Formula from
        // https://wowwiki-archive.fandom.com/wiki/Spirit?oldid=1572910
        int mp5_from_spirit = 5 * (0.001 + std::sqrt(player->stats->intellect * player->stats->intellect_modifier) *
                                               (player->stats->spirit * player->stats->spirit_modifier) * 0.009327);
        if (kInnervateIsActive) {
          mp5_from_spirit *= 4;
        }
        player->stats->mana += mp5_from_spirit;
      }

      if (player->stats->mana > player->stats->max_mana) {
        player->stats->mana = player->stats->max_mana;
      }

      const int kManaGained = player->stats->mana - kCurrentPlayerMana;
      player->combat_log_breakdown.at("Mp5")->casts++;
      player->AddIterationDamageAndMana("Mp5", kManaGained, 0);

      if (player->ShouldWriteToCombatLog()) {
        player->CombatLog("Player gains " + TruncateTrailingZeros(std::to_string(round(kManaGained))) +
                          " mana from MP5 (" + std::to_string(kCurrentPlayerMana) + " -> " +
                          std::to_string(player->stats->mana) + ")");
      }
    }
  }

  return time;
}

void Simulation::SelectedSpellHandler(std::shared_ptr<Spell>& spell,
                                      std::map<std::shared_ptr<Spell>, double>& predicted_damage_of_spells,
                                      double fight_time_remaining) {
  if ((player->settings->sim_choosing_rotation || spell->is_finisher) && predicted_damage_of_spells.count(spell) == 0) {
    predicted_damage_of_spells.insert(std::make_pair(spell, spell->PredictDamage()));
  } else if (spell->HasEnoughMana()) {
    CastSelectedSpell(spell, fight_time_remaining);
  } else {
    player->CastLifeTapOrDarkPact();
  }
}

void Simulation::CastSelectedSpell(std::shared_ptr<Spell>& spell, double fight_time_remaining,
                                   double predicted_damage) {
  player->UseCooldowns(fight_time_remaining);

  if (player->spells->amplify_curse != NULL && player->spells->amplify_curse->Ready() &&
      ((player->spells->curse_of_agony != NULL && spell == player->spells->curse_of_agony) ||
       (player->spells->curse_of_doom != NULL && spell == player->spells->curse_of_doom))) {
    player->spells->amplify_curse->StartCast();
  }

  spell->StartCast(predicted_damage);
}