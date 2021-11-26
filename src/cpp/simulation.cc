#include "simulation.h"

#include <stdlib.h>

#include <algorithm>
#include <chrono>
#include <limits>
#include <vector>

#include "bindings.h"
#include "common.h"
#include "enums.h"
#include "spell.h"

Simulation::Simulation(Player& player, const SimulationSettings& simulation_settings)
    : player(player), settings(simulation_settings) {}

void Simulation::Start() {
  std::vector<double> dps_vector;
  player.total_duration = 0;
  player.Initialize();
  double min_dps = std::numeric_limits<double>::max();
  double max_dps = 0;

  for (player.iteration = 0; player.iteration < settings.iterations; player.iteration++) {
    player.Reset();
    if (player.pet != NULL) {
      player.pet->Reset();
    }
    player.iteration_damage = 0;
    player.fight_time = 0;
    player.rng.seed(player.settings.random_seeds[player.iteration]);
    const int kFightLength = player.rng.range(settings.min_time, settings.max_time);
    if (player.ShouldWriteToCombatLog()) {
      player.CombatLog("Fight length: " + std::to_string(kFightLength) + " seconds");
    }

    if (player.pet->auras.battle_squawk != NULL) {
      player.pet->auras.battle_squawk->Apply();
    }
    if (player.settings.prepop_black_book && player.pet->auras.black_book != NULL) {
      player.pet->auras.black_book->Apply();
    }

    while (player.fight_time < kFightLength) {
      const double kFightTimeRemaining = kFightLength - player.fight_time;

      // Use Drums
      if (player.spells.drums_of_battle != NULL && !player.auras.drums_of_battle->active &&
          player.spells.drums_of_battle->Ready()) {
        player.spells.drums_of_battle->StartCast();
      } else if (player.spells.drums_of_war != NULL && !player.auras.drums_of_war->active &&
                 player.spells.drums_of_war->Ready()) {
        player.spells.drums_of_war->StartCast();
      } else if (player.spells.drums_of_restoration != NULL && !player.auras.drums_of_restoration->active &&
                 player.spells.drums_of_restoration->Ready()) {
        player.spells.drums_of_restoration->StartCast();
      }
      // Use Bloodlust
      if (!player.spells.bloodlust.empty() && !player.auras.bloodlust->active) {
        for (auto bloodlust : player.spells.bloodlust) {
          if (bloodlust->Ready()) {
            bloodlust->StartCast();
            break;
          }
        }
      }
      // Use Mana Tide Totem if there's <= 12 sec remaning or the player's mana
      // is at 50% or lower
      if (player.spells.mana_tide_totem != NULL && player.spells.mana_tide_totem->Ready() &&
          (kFightTimeRemaining <= player.auras.mana_tide_totem->duration ||
           player.stats.at(CharacterStat::kMana) / static_cast<double>(player.stats.at(CharacterStat::kMaxMana)) <=
               0.50)) {
        player.spells.mana_tide_totem->StartCast();
      }

      // Player
      if (player.cast_time_remaining <= 0) {
        // Spells not on the GCD
        // Demonic Rune
        if ((player.fight_time > 5 || player.stats.at(CharacterStat::kMp5) == 0) &&
            player.spells.demonic_rune != NULL &&
            (player.stats.at(CharacterStat::kMaxMana) - player.stats.at(CharacterStat::kMana)) >
                player.spells.demonic_rune->max_mana &&
            player.spells.demonic_rune->Ready() &&
            (!player.spells.chipped_power_core || player.spells.chipped_power_core->cooldown_remaining > 0) &&
            (!player.spells.cracked_power_core || player.spells.cracked_power_core->cooldown_remaining > 0)) {
          player.spells.demonic_rune->StartCast();
        }
        // Super Mana Potion
        if ((player.fight_time > 5 || player.stats.at(CharacterStat::kMp5) == 0) &&
            player.spells.super_mana_potion != NULL &&
            (player.stats.at(CharacterStat::kMaxMana) - player.stats.at(CharacterStat::kMana)) >
                player.spells.super_mana_potion->max_mana &&
            player.spells.super_mana_potion->Ready()) {
          player.spells.super_mana_potion->StartCast();
        }

        // Spells on the GCD
        if (player.gcd_remaining <= 0) {
          if (player.settings.fight_type == EmbindConstant::kSingleTarget) {
            const bool kNotEnoughTimeForFillerSpell = kFightTimeRemaining < player.filler->GetCastTime();

            // Map of spells with their predicted Damage as the value. This is
            // used by the sim to determine what the best spell to Cast is.
            std::map<std::shared_ptr<Spell>, double> predicted_damage_of_spells;

            // If the sim is choosing the rotation for the user then predict the
            // Damage of the three filler spells if they're available (maybe
            // just skip Searing Pain to save time, there's no way it will ever
            // be the best spell to Cast)
            if (player.settings.rotation_option == EmbindConstant::kSimChooses) {
              if (kFightTimeRemaining >= player.spells.shadow_bolt->GetCastTime()) {
                predicted_damage_of_spells.insert(
                    {player.spells.shadow_bolt, player.spells.shadow_bolt->PredictDamage()});
              }
              if (kFightTimeRemaining >= player.spells.incinerate->GetCastTime()) {
                predicted_damage_of_spells.insert(
                    {player.spells.incinerate, player.spells.incinerate->PredictDamage()});
              }
              if (kFightTimeRemaining >= player.spells.searing_pain->GetCastTime()) {
                predicted_damage_of_spells.insert(
                    {player.spells.searing_pain, player.spells.searing_pain->PredictDamage()});
              }
            }

            // Cast Conflagrate if there's not enough time for another filler
            // and Immolate is up
            if (kNotEnoughTimeForFillerSpell && player.spells.conflagrate != NULL && player.auras.immolate != NULL &&
                player.spells.conflagrate->CanCast() && player.auras.immolate->active) {
              SelectedSpellHandler(player.spells.conflagrate, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Shadowburn if there's not enough time for another filler
            if (player.gcd_remaining <= 0 && kNotEnoughTimeForFillerSpell && player.spells.shadowburn != NULL &&
                player.spells.shadowburn->CanCast()) {
              SelectedSpellHandler(player.spells.shadowburn, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Death Coil if there's not enough time for another filler
            if (player.gcd_remaining <= 0 && kNotEnoughTimeForFillerSpell && player.spells.death_coil != NULL &&
                player.spells.death_coil->CanCast()) {
              SelectedSpellHandler(player.spells.death_coil, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Curse of the Elements or Curse of Recklessness if they're
            // the selected curse and they're not active
            if (kFightTimeRemaining >= 10 && player.gcd_remaining <= 0 &&
                (player.curse_spell == player.spells.curse_of_recklessness ||
                 player.curse_spell == player.spells.curse_of_the_elements) &&
                !player.curse_aura->active && player.curse_spell->CanCast()) {
              if (player.curse_spell->HasEnoughMana()) {
                player.curse_spell->StartCast();
              } else {
                player.CastLifeTapOrDarkPact();
              }
            }
            // Cast Curse of Doom if it's the selected curse and there's more
            // than 60 seconds remaining
            if (player.gcd_remaining <= 0 && kFightTimeRemaining > 60 &&
                player.curse_spell == player.spells.curse_of_doom && !player.auras.curse_of_doom->active &&
                player.spells.curse_of_doom->CanCast()) {
              SelectedSpellHandler(player.spells.curse_of_doom, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Curse of Agony if CoA is the selected curse or if Curse of
            // Doom is the selected curse and there's less than 60 seconds
            // remaining of the fight
            if (player.gcd_remaining <= 0 && player.auras.curse_of_agony != NULL &&
                !player.auras.curse_of_agony->active && player.spells.curse_of_agony->CanCast() &&
                kFightTimeRemaining > player.auras.curse_of_agony->minimum_duration &&
                ((player.curse_spell == player.spells.curse_of_doom && !player.auras.curse_of_doom->active &&
                  (player.spells.curse_of_doom->cooldown_remaining > player.auras.curse_of_agony->minimum_duration ||
                   kFightTimeRemaining < 60)) ||
                 player.curse_spell == player.spells.curse_of_agony)) {
              SelectedSpellHandler(player.spells.curse_of_agony, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Corruption if Corruption isn't up or if it will expire
            // before the Cast finishes (if no instant Corruption)
            if (player.gcd_remaining <= 0 && player.spells.corruption != NULL &&
                (!player.auras.corruption->active ||
                 (player.auras.corruption->ticks_remaining == 1 &&
                  player.auras.corruption->tick_timer_remaining < player.spells.corruption->GetCastTime())) &&
                player.spells.corruption->CanCast() &&
                (kFightTimeRemaining - player.spells.corruption->GetCastTime()) >=
                    player.auras.corruption->minimum_duration) {
              SelectedSpellHandler(player.spells.corruption, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Shadow Bolt if Shadow Trance (Nightfall) is active and
            // Corruption is active as well to avoid potentially wasting another
            // Nightfall proc
            if (player.gcd_remaining <= 0 && player.spells.shadow_bolt != NULL && player.auras.shadow_trance != NULL &&
                player.auras.shadow_trance->active && player.auras.corruption->active &&
                player.spells.shadow_bolt->CanCast()) {
              SelectedSpellHandler(player.spells.shadow_bolt, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Unstable Affliction if it's not up or if it's about to
            // expire
            if (player.gcd_remaining <= 0 && player.spells.unstable_affliction != NULL &&
                player.spells.unstable_affliction->CanCast() &&
                (!player.auras.unstable_affliction->active || (player.auras.unstable_affliction->ticks_remaining == 1 &&
                                                               player.auras.unstable_affliction->tick_timer_remaining <
                                                                   player.spells.unstable_affliction->GetCastTime())) &&
                (kFightTimeRemaining - player.spells.unstable_affliction->GetCastTime()) >=
                    player.auras.unstable_affliction->minimum_duration) {
              SelectedSpellHandler(player.spells.unstable_affliction, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Siphon Life if it's not up (todo: add option to only Cast it
            // while ISB is active if not using custom ISB uptime %)
            if (player.gcd_remaining <= 0 && player.spells.siphon_life != NULL && !player.auras.siphon_life->active &&
                player.spells.siphon_life->CanCast() &&
                kFightTimeRemaining >= player.auras.siphon_life->minimum_duration) {
              SelectedSpellHandler(player.spells.siphon_life, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Immolate if it's not up or about to expire
            if (player.gcd_remaining <= 0 && player.spells.immolate != NULL && player.spells.immolate->CanCast() &&
                (!player.auras.immolate->active ||
                 (player.auras.immolate->ticks_remaining == 1 &&
                  player.auras.immolate->tick_timer_remaining < player.spells.immolate->GetCastTime())) &&
                (kFightTimeRemaining - player.spells.immolate->GetCastTime()) >=
                    player.auras.immolate->minimum_duration) {
              SelectedSpellHandler(player.spells.immolate, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Shadow Bolt if Shadow Trance (Nightfall) is active
            if (player.gcd_remaining <= 0 && player.spells.shadow_bolt != NULL && player.auras.shadow_trance != NULL &&
                player.auras.shadow_trance->active && player.spells.shadow_bolt->CanCast()) {
              SelectedSpellHandler(player.spells.shadow_bolt, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast Shadowfury
            if (player.gcd_remaining <= 0 && player.spells.shadowfury != NULL && player.spells.shadowfury->CanCast()) {
              SelectedSpellHandler(player.spells.shadowfury, predicted_damage_of_spells, kFightTimeRemaining);
            }
            // Cast filler spell if sim is not choosing the rotation for the
            // user or if the predicted_damage_of_spells map is empty
            if (player.gcd_remaining <= 0 &&
                ((!kNotEnoughTimeForFillerSpell && player.settings.rotation_option == EmbindConstant::kUserChooses) ||
                 predicted_damage_of_spells.size() == 0) &&
                player.filler->CanCast()) {
              SelectedSpellHandler(player.filler, predicted_damage_of_spells, kFightTimeRemaining);
            }

            // If the predicted_damage_of_spells map is not empty then check now
            // which spell would be the best to Cast
            if (player.gcd_remaining <= 0 && player.cast_time_remaining <= 0 &&
                predicted_damage_of_spells.size() != 0) {
              std::shared_ptr<Spell> max_damage_spell;
              double max_damage_spell_value = 0;

              for (auto& spell : predicted_damage_of_spells) {
                if (spell.second > max_damage_spell_value &&
                    (kFightTimeRemaining > player.GetGcdValue(spell.first) || spell.first->HasEnoughMana())) {
                  max_damage_spell = spell.first;
                  max_damage_spell_value = spell.second;
                }
              }

              // If a max Damage spell was not found or if the max Damage spell
              // isn't Ready (no mana), then Cast Life Tap
              if (max_damage_spell != NULL && max_damage_spell->HasEnoughMana()) {
                CastSelectedSpell(max_damage_spell, kFightTimeRemaining, max_damage_spell_value);
              } else {
                player.CastLifeTapOrDarkPact();
              }
            }
          }
          // AoE (currently just does Seed of Corruption by default)
          else {
            if (player.spells.seed_of_corruption->Ready()) {
              player.UseCooldowns(kFightTimeRemaining);
              player.spells.seed_of_corruption->StartCast();
            } else {
              player.CastLifeTapOrDarkPact();
            }
          }
        }
      }

      // Pet
      if (player.pet != NULL && player.settings.pet_mode == EmbindConstant::kAggressive) {
        // Auto Attack
        if (player.pet->spells.melee != NULL && player.pet->spells.melee->Ready()) {
          player.pet->spells.melee->StartCast();
        }
        // Felguard Cleave
        if (player.pet->spells.cleave != NULL && player.pet->spells.cleave->Ready()) {
          player.pet->spells.cleave->StartCast();
        }
        // Succubus Lash of Pain
        if (player.pet->spells.lash_of_pain != NULL && player.pet->spells.lash_of_pain->Ready() &&
            (player.settings.lash_of_pain_usage == EmbindConstant::kOnCooldown ||
             (!player.settings.using_custom_isb_uptime &&
              (player.auras.improved_shadow_bolt == NULL || !player.auras.improved_shadow_bolt->active)))) {
          player.pet->spells.lash_of_pain->StartCast();
        }
        // Imp Firebolt
        if (player.pet->spells.firebolt != NULL && player.pet->spells.firebolt->Ready()) {
          player.pet->spells.firebolt->StartCast();
        }
      }

      if (PassTime() <= 0) {
        std::cout << "Iteration " << std::to_string(player.iteration)
                  << " fightTime: " << std::to_string(player.fight_time) << "/" << std::to_string(kFightLength)
                  << " PassTime() returned <= 0" << std::endl;
        player.ThrowError(
            "The simulation got stuck in an endless loop. If you'd like to "
            "help with fixing this bug then please "
            "export your current settings and send it to Kristofer#8003 on "
            "Discord.");
      }
    }

    if (player.ShouldWriteToCombatLog()) {
      player.CombatLog("Fight end");
    }

    player.EndAuras();
    if (player.pet != NULL) {
      player.pet->EndAuras();
    }

    player.total_duration += kFightLength;
    const double kDps = player.iteration_damage / static_cast<double>(kFightLength);
    if (kDps > max_dps) {
      max_dps = kDps;
    }
    if (kDps < min_dps) {
      min_dps = kDps;
    }
    dps_vector.push_back(kDps);

    // Only send the iteration's dps to the web worker if we're doing a normal
    // simulation (this is just for the dps histogram)
    if (settings.simulation_type == SimulationType::kNormal && player.custom_stat == "normal") {
      DpsUpdate(kDps);
    }

    if (player.iteration % static_cast<int>(std::floor(settings.iterations / 100.0)) == 0) {
      SimulationUpdate(player.iteration, settings.iterations, Median(dps_vector), player.settings.item_id,
                       player.custom_stat.c_str());
    }
  }

  // Send the contents of the combat log to the web worker
  if (player.settings.equipped_item_simulation) {
    player.SendCombatLogEntries();
  }

  // Send the combat log breakdown info
  if (player.recording_combat_log_breakdown) {
    for (auto& combat_log_breakdown : player.combat_log_breakdown) {
      if (combat_log_breakdown.second->iteration_damage > 0 || combat_log_breakdown.second->iteration_mana_gain > 0) {
        player.PostIterationDamageAndMana(combat_log_breakdown.first);
      }
      PostCombatLogBreakdown(combat_log_breakdown.second->name.c_str(), combat_log_breakdown.second->casts,
                             combat_log_breakdown.second->crits, combat_log_breakdown.second->misses,
                             combat_log_breakdown.second->count, combat_log_breakdown.second->uptime,
                             combat_log_breakdown.second->dodge, combat_log_breakdown.second->glancing_blows);
    }
  }
  SimulationEnd(Median(dps_vector), min_dps, max_dps, player.settings.item_id, settings.iterations,
                player.total_duration, player.custom_stat.c_str());
}

double Simulation::PassTime() {
  double time = player.cast_time_remaining;
  if (time <= 0) time = player.gcd_remaining;

  // Find the lowest time until the next action needs to be taken
  // Pet
  if (player.pet != NULL) {
    if ((player.talents.dark_pact > 0 || player.settings.pet_mode == EmbindConstant::kAggressive) &&
        player.pet->spirit_tick_timer_remaining < time)
      time = player.pet->spirit_tick_timer_remaining;

    // Pet's attacks/abilities and such
    if (player.settings.pet_mode == EmbindConstant::kAggressive) {
      if (player.pet->spells.melee != NULL && player.pet->spells.melee->cooldown_remaining > 0 &&
          player.pet->spells.melee->cooldown_remaining < time)
        time = player.pet->spells.melee->cooldown_remaining;
      else if (player.pet->cast_time_remaining > 0 && player.pet->cast_time_remaining < time)
        time = player.pet->cast_time_remaining;
      if (player.pet->spells.lash_of_pain != NULL && player.pet->spells.lash_of_pain->cooldown_remaining > 0 &&
          player.pet->spells.lash_of_pain->cooldown_remaining < time)
        time = player.pet->spells.lash_of_pain->cooldown_remaining;
      else if (player.pet->spells.cleave != NULL && player.pet->spells.cleave->cooldown_remaining > 0 &&
               player.pet->spells.cleave->cooldown_remaining < time)
        time = player.pet->spells.cleave->cooldown_remaining;
      if (player.pet->auras.black_book != NULL && player.pet->auras.black_book->active &&
          player.pet->auras.black_book->duration_remaining < time)
        time = player.pet->auras.black_book->duration_remaining;
      if (player.pet->auras.battle_squawk != NULL && player.pet->auras.battle_squawk->active &&
          player.pet->auras.battle_squawk->duration_remaining < time)
        time = player.pet->auras.battle_squawk->duration_remaining;
    }
  }

  // Spells
  for (auto& spell : player.spell_list) {
    if (spell->cooldown_remaining < time && spell->cooldown_remaining > 0) {
      time = spell->cooldown_remaining;
    }
  }
  for (auto& pi : player.spells.power_infusion) {
    if (pi->cooldown_remaining > 0 && pi->cooldown_remaining < time) time = pi->cooldown_remaining;
  }
  for (auto& bloodlust : player.spells.bloodlust) {
    if (bloodlust->cooldown_remaining > 0 && bloodlust->cooldown_remaining < time) time = bloodlust->cooldown_remaining;
  }
  for (auto& innervate : player.spells.innervate) {
    if (innervate->cooldown_remaining > 0 && innervate->cooldown_remaining < time) time = innervate->cooldown_remaining;
  }

  // Auras
  for (auto& aura : player.aura_list) {
    if (aura->active && aura->duration_remaining < time) {
      time = aura->duration_remaining;
    }
  }

  // Dots
  for (auto& dot : player.dot_list) {
    if (dot->active && dot->tick_timer_remaining < time) {
      time = dot->tick_timer_remaining;
    }
  }

  // MP5
  if (player.mp5_timer < time && player.mp5_timer > 0) {
    time = player.mp5_timer;
  }

  // Trinkets
  for (auto& trinket : player.trinkets) {
    if (trinket.active && trinket.duration_remaining < time) {
      time = trinket.duration_remaining;
    }
    if (trinket.cooldown_remaining > 0 && trinket.cooldown_remaining < time) {
      time = trinket.cooldown_remaining;
    }
  }

  // Pass time
  // This needs to be the first modified value since the time in combat needs to
  // be updated before spells start dealing Damage/auras expiring etc. for the
  // combat logging.
  player.fight_time += time;
  player.cast_time_remaining -= time;

  // Pet
  if (player.pet != NULL) player.pet->Tick(time);

  // Auras need to tick before Spells because otherwise you'll, for example,
  // finish casting Corruption and then immediately afterwards, in the same
  // millisecond, immediately tick down the aura This was also causing buffs like
  // e.g. the t4 4pc buffs to expire sooner than they should.
  for (auto& aura : player.aura_list) {
    if (aura->active && aura->duration_remaining > 0) {
      aura->Tick(time);
    }
  }

  for (auto& dot : player.dot_list) {
    if (dot->active && dot->tick_timer_remaining > 0) {
      dot->Tick(time);
    }
  }

  for (auto& spell : player.spell_list) {
    if (spell->casting || spell->cooldown_remaining > 0) {
      spell->Tick(time);
    }
  }
  for (auto& pi : player.spells.power_infusion) {
    if (pi->cooldown_remaining > 0) pi->Tick(time);
  }
  for (auto& bloodlust : player.spells.bloodlust) {
    if (bloodlust->cooldown_remaining > 0) bloodlust->Tick(time);
  }
  for (auto& innervate : player.spells.innervate) {
    if (innervate->cooldown_remaining > 0) innervate->Tick(time);
  }

  for (auto& trinket : player.trinkets) {
    trinket.Tick(time);
  }

  player.gcd_remaining -= time;
  player.mp5_timer -= time;
  player.five_second_rule_timer -= time;
  if (player.mp5_timer <= 0) {
    player.mp5_timer = 5;

    if (player.stats.at(CharacterStat::kMp5) > 0 || player.five_second_rule_timer <= 0 ||
        (player.auras.innervate != NULL && player.auras.innervate->active)) {
      const bool kInnervateIsActive = player.auras.innervate != NULL && player.auras.innervate->active;
      const int kCurrentPlayerMana = player.stats.at(CharacterStat::kMana);

      // MP5
      if (player.stats.at(CharacterStat::kMp5) > 0) {
        player.stats.at(CharacterStat::kMana) += player.stats.at(CharacterStat::kMp5);
      }
      // Spirit mana regen
      if (kInnervateIsActive || player.five_second_rule_timer <= 0) {
        // Formula from
        // https://wowwiki-archive.fandom.com/wiki/Spirit?oldid=1572910
        int mp5_from_spirit = 5 * (0.001 + std::sqrt(player.GetIntellect()) * player.GetSpirit() * 0.009327);
        if (kInnervateIsActive) {
          mp5_from_spirit *= 4;
        }
        player.stats.at(CharacterStat::kMana) += mp5_from_spirit;
      }

      if (player.stats.at(CharacterStat::kMana) > player.stats.at(CharacterStat::kMaxMana)) {
        player.stats.at(CharacterStat::kMana) = player.stats.at(CharacterStat::kMaxMana);
      }

      const int kManaGained = player.stats.at(CharacterStat::kMana) - kCurrentPlayerMana;
      if (player.recording_combat_log_breakdown) {
        player.combat_log_breakdown.at("Mp5")->casts++;
        player.AddIterationDamageAndMana("Mp5", kManaGained, 0);
      }
      if (player.ShouldWriteToCombatLog()) {
        player.CombatLog("Player gains " + DoubleToString(round(kManaGained)) + " mana from MP5 (" +
                         std::to_string(kCurrentPlayerMana) + " -> " +
                         DoubleToString(player.stats.at(CharacterStat::kMana)) + ")");
      }
    }
  }

  return time;
}

void Simulation::SelectedSpellHandler(const std::shared_ptr<Spell>& spell,
                                      std::map<std::shared_ptr<Spell>, double>& predicted_damage_of_spells,
                                      double fight_time_remaining) {
  if ((player.settings.rotation_option == EmbindConstant::kSimChooses || spell->is_finisher) &&
      predicted_damage_of_spells.count(spell) == 0) {
    predicted_damage_of_spells.insert({spell, spell->PredictDamage()});
  } else if (spell->HasEnoughMana()) {
    CastSelectedSpell(spell, fight_time_remaining);
  } else {
    player.CastLifeTapOrDarkPact();
  }
}

void Simulation::CastSelectedSpell(const std::shared_ptr<Spell>& spell, double fight_time_remaining,
                                   double predicted_damage) {
  player.UseCooldowns(fight_time_remaining);

  if (player.spells.amplify_curse != NULL && player.spells.amplify_curse->Ready() &&
      ((player.spells.curse_of_agony != NULL && spell == player.spells.curse_of_agony) ||
       (player.spells.curse_of_doom != NULL && spell == player.spells.curse_of_doom))) {
    player.spells.amplify_curse->StartCast();
  }

  spell->StartCast(predicted_damage);
}