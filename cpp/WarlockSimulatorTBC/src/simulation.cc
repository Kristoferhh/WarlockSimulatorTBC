#include "../include/simulation.h"

#include <stdlib.h>

#include <algorithm>
#include <chrono>
#include <limits>
#include <vector>

#include "../include/bindings.h"
#include "../include/common.h"
#include "../include/enums.h"

Simulation::Simulation(Player& player, const SimulationSettings& simulation_settings)
    : player(player), settings(simulation_settings) {}

void Simulation::Start() {
  player.total_fight_duration = 0;
  player.Initialize(this);
  min_dps = std::numeric_limits<double>::max();
  max_dps = 0;
  auto start = std::chrono::high_resolution_clock::now();

  for (iteration = 0; iteration < settings.iterations; iteration++) {
    const int kFightLength = player.rng.range(settings.min_time, settings.max_time);

    IterationReset(kFightLength);

    while (current_fight_time < kFightLength) {
      const double kFightTimeRemaining = kFightLength - current_fight_time;

      CastNonPlayerCooldowns(kFightTimeRemaining);

      if (player.cast_time_remaining <= 0) {
        CastNonGcdSpells();

        if (player.gcd_remaining <= 0) {
          CastGcdSpells(kFightTimeRemaining);
        }
      }

      if (player.pet != NULL && player.settings.pet_mode == EmbindConstant::kAggressive) {
        CastPetSpells();
      }

      if (PassTime() <= 0) {
        std::cout << "Iteration " << std::to_string(iteration) << " fightTime: " << std::to_string(current_fight_time)
                  << "/" << std::to_string(kFightLength) << " PassTime() returned <= 0" << std::endl;
        player.ThrowError(
            "The simulation got stuck in an endless loop. If you'd like to "
            "help with fixing this bug then please "
            "export your current settings and send it to Kristofer#8003 on "
            "Discord.");
      }
    }

    IterationEnd(kFightLength, player.iteration_damage / static_cast<double>(kFightLength));
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

  SimulationEnd(microseconds);
}

double Simulation::PassTime() {
  auto time_until_next_action = player.FindTimeUntilNextAction();

  Tick(time_until_next_action);

  return time_until_next_action;
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
      (spell->name == SpellName::kCurseOfAgony || spell->name == SpellName::kCurseOfDoom)) {
    player.spells.amplify_curse->StartCast();
  }

  spell->StartCast(predicted_damage);
}

void Simulation::Tick(double time) {
  current_fight_time += time;
  player.Tick(time);
  if (player.pet != NULL) {
    player.pet->Tick(time);
  }
}

void Simulation::IterationReset(double fight_length) {
  current_fight_time = 0;
  player.Reset();
  if (player.pet != NULL) {
    player.pet->Reset();
  }
  player.rng.seed(player.settings.random_seeds[iteration]);

  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog("Fight length: " + DoubleToString(fight_length) + " seconds");
  }

  if (player.auras.airmans_ribbon_of_gallantry != NULL) {
    player.auras.airmans_ribbon_of_gallantry->Apply();
  }

  if (player.pet != NULL) {
    if (player.pet->auras.battle_squawk != NULL) {
      player.pet->auras.battle_squawk->Apply();
    }

    if (player.settings.prepop_black_book && player.pet->auras.black_book != NULL) {
      player.pet->auras.black_book->Apply();
    }
  }
}

void Simulation::CastNonPlayerCooldowns(double fight_time_remaining) {
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
    for (auto& bloodlust : player.spells.bloodlust) {
      if (bloodlust->Ready()) {
        bloodlust->StartCast();
        break;
      }
    }
  }

  // Use Mana Tide Totem if there's <= 12 sec remaning or the player's mana
  // is at 50% or lower
  if (player.spells.mana_tide_totem != NULL && player.spells.mana_tide_totem->Ready() &&
      (fight_time_remaining <= player.auras.mana_tide_totem->duration ||
       player.stats.mana / static_cast<double>(player.stats.max_mana) <= 0.50)) {
    player.spells.mana_tide_totem->StartCast();
  }
}

void Simulation::CastNonGcdSpells() {
  // Demonic Rune
  if ((current_fight_time > 5 || player.stats.mp5 == 0) && player.spells.demonic_rune != NULL &&
      (player.stats.max_mana - player.stats.mana) > player.spells.demonic_rune->max_mana_gain &&
      player.spells.demonic_rune->Ready() &&
      (!player.spells.chipped_power_core || !player.spells.chipped_power_core->Ready()) &&
      (!player.spells.cracked_power_core || !player.spells.cracked_power_core->Ready())) {
    player.spells.demonic_rune->StartCast();
  }

  // Super Mana Potion
  if ((current_fight_time > 5 || player.stats.mp5 == 0) && player.spells.super_mana_potion != NULL &&
      (player.stats.max_mana - player.stats.mana) > player.spells.super_mana_potion->max_mana_gain &&
      player.spells.super_mana_potion->Ready()) {
    player.spells.super_mana_potion->StartCast();
  }
}

void Simulation::CastGcdSpells(double fight_time_remaining) {
  if (player.settings.fight_type == EmbindConstant::kSingleTarget) {
    const bool kNotEnoughTimeForFillerSpell = fight_time_remaining < player.filler->GetCastTime();

    // Map of spells with their predicted Damage as the value. This is
    // used by the sim to determine what the best spell to Cast is.
    std::map<std::shared_ptr<Spell>, double> predicted_damage_of_spells;

    // If the sim is choosing the rotation for the user then predict the
    // damage of the three filler spells if they're available
    if (player.settings.rotation_option == EmbindConstant::kSimChooses) {
      if (fight_time_remaining >= player.spells.shadow_bolt->GetCastTime()) {
        predicted_damage_of_spells.insert({player.spells.shadow_bolt, player.spells.shadow_bolt->PredictDamage()});
      }

      if (fight_time_remaining >= player.spells.incinerate->GetCastTime()) {
        predicted_damage_of_spells.insert({player.spells.incinerate, player.spells.incinerate->PredictDamage()});
      }

      if (fight_time_remaining >= player.spells.searing_pain->GetCastTime()) {
        predicted_damage_of_spells.insert({player.spells.searing_pain, player.spells.searing_pain->PredictDamage()});
      }
    }

    // Cast Conflagrate if there's not enough time for another filler
    // and Immolate is up
    if (kNotEnoughTimeForFillerSpell && player.spells.conflagrate != NULL && player.spells.conflagrate->CanCast()) {
      SelectedSpellHandler(player.spells.conflagrate, predicted_damage_of_spells, fight_time_remaining);
    }

    // Cast Shadowburn if there's not enough time for another filler
    if (player.gcd_remaining <= 0 && kNotEnoughTimeForFillerSpell && player.spells.shadowburn != NULL &&
        player.spells.shadowburn->CanCast()) {
      SelectedSpellHandler(player.spells.shadowburn, predicted_damage_of_spells, fight_time_remaining);
    }

    // Cast Death Coil if there's not enough time for another filler
    if (player.gcd_remaining <= 0 && kNotEnoughTimeForFillerSpell && player.spells.death_coil != NULL &&
        player.spells.death_coil->CanCast()) {
      SelectedSpellHandler(player.spells.death_coil, predicted_damage_of_spells, fight_time_remaining);
    }

    // Cast Curse of the Elements or Curse of Recklessness if they're
    // the selected curse and they're not active
    if (fight_time_remaining >= 10 && player.gcd_remaining <= 0 && player.curse_spell != NULL &&
        (player.curse_spell->name == SpellName::kCurseOfRecklessness ||
         player.curse_spell->name == SpellName::kCurseOfTheElements) &&
        !player.curse_aura->active && player.curse_spell->CanCast()) {
      if (player.curse_spell->HasEnoughMana()) {
        player.curse_spell->StartCast();
      } else {
        player.CastLifeTapOrDarkPact();
      }
    }

    // Cast Curse of Doom if it's the selected curse and there's more
    // than 60 seconds remaining
    if (player.gcd_remaining <= 0 && fight_time_remaining > 60 && player.curse_spell != NULL &&
        player.curse_spell->name == SpellName::kCurseOfDoom && !player.auras.curse_of_doom->active &&
        player.spells.curse_of_doom->CanCast()) {
      SelectedSpellHandler(player.spells.curse_of_doom, predicted_damage_of_spells, fight_time_remaining);
    }

    // Cast Curse of Agony if CoA is the selected curse or if Curse of
    // Doom is the selected curse and there's less than 60 seconds
    // remaining of the fight
    if (player.gcd_remaining <= 0 && player.auras.curse_of_agony != NULL && !player.auras.curse_of_agony->active &&
        player.spells.curse_of_agony->CanCast() && fight_time_remaining > player.auras.curse_of_agony->duration &&
        ((player.curse_spell->name == SpellName::kCurseOfDoom && !player.auras.curse_of_doom->active &&
          (player.spells.curse_of_doom->cooldown_remaining > player.auras.curse_of_agony->duration ||
           fight_time_remaining < 60)) ||
         player.curse_spell->name == SpellName::kCurseOfAgony)) {
      SelectedSpellHandler(player.spells.curse_of_agony, predicted_damage_of_spells, fight_time_remaining);
    }

    // Cast Corruption if Corruption isn't up or if it will expire
    // before the Cast finishes (if no instant Corruption)
    if (player.gcd_remaining <= 0 && player.spells.corruption != NULL &&
        (!player.auras.corruption->active ||
         (player.auras.corruption->ticks_remaining == 1 &&
          player.auras.corruption->tick_timer_remaining < player.spells.corruption->GetCastTime())) &&
        player.spells.corruption->CanCast() &&
        (fight_time_remaining - player.spells.corruption->GetCastTime()) >= player.auras.corruption->duration) {
      SelectedSpellHandler(player.spells.corruption, predicted_damage_of_spells, fight_time_remaining);
    }

    // Cast Shadow Bolt if Shadow Trance (Nightfall) is active and
    // Corruption is active as well to avoid potentially wasting another
    // Nightfall proc
    if (player.gcd_remaining <= 0 && player.spells.shadow_bolt != NULL && player.auras.shadow_trance != NULL &&
        player.auras.shadow_trance->active && player.auras.corruption->active && player.spells.shadow_bolt->CanCast()) {
      SelectedSpellHandler(player.spells.shadow_bolt, predicted_damage_of_spells, fight_time_remaining);
    }

    // Cast Unstable Affliction if it's not up or if it's about to
    // expire
    if (player.gcd_remaining <= 0 && player.spells.unstable_affliction != NULL &&
        player.spells.unstable_affliction->CanCast() &&
        (!player.auras.unstable_affliction->active ||
         (player.auras.unstable_affliction->ticks_remaining == 1 &&
          player.auras.unstable_affliction->tick_timer_remaining < player.spells.unstable_affliction->GetCastTime())) &&
        (fight_time_remaining - player.spells.unstable_affliction->GetCastTime()) >=
            player.auras.unstable_affliction->duration) {
      SelectedSpellHandler(player.spells.unstable_affliction, predicted_damage_of_spells, fight_time_remaining);
    }

    // Cast Siphon Life if it's not up (todo: add option to only Cast it
    // while ISB is active if not using custom ISB uptime %)
    if (player.gcd_remaining <= 0 && player.spells.siphon_life != NULL && !player.auras.siphon_life->active &&
        player.spells.siphon_life->CanCast() && fight_time_remaining >= player.auras.siphon_life->duration) {
      SelectedSpellHandler(player.spells.siphon_life, predicted_damage_of_spells, fight_time_remaining);
    }

    // Cast Immolate if it's not up or about to expire
    if (player.gcd_remaining <= 0 && player.spells.immolate != NULL && player.spells.immolate->CanCast() &&
        (!player.auras.immolate->active ||
         (player.auras.immolate->ticks_remaining == 1 &&
          player.auras.immolate->tick_timer_remaining < player.spells.immolate->GetCastTime())) &&
        (fight_time_remaining - player.spells.immolate->GetCastTime()) >= player.auras.immolate->duration) {
      SelectedSpellHandler(player.spells.immolate, predicted_damage_of_spells, fight_time_remaining);
    }

    // Cast Shadow Bolt if Shadow Trance (Nightfall) is active
    if (player.gcd_remaining <= 0 && player.spells.shadow_bolt != NULL && player.auras.shadow_trance != NULL &&
        player.auras.shadow_trance->active && player.spells.shadow_bolt->CanCast()) {
      SelectedSpellHandler(player.spells.shadow_bolt, predicted_damage_of_spells, fight_time_remaining);
    }

    // Cast Shadowfury
    if (player.gcd_remaining <= 0 && player.spells.shadowfury != NULL && player.spells.shadowfury->CanCast()) {
      SelectedSpellHandler(player.spells.shadowfury, predicted_damage_of_spells, fight_time_remaining);
    }

    // Cast filler spell if sim is not choosing the rotation for the
    // user or if the predicted_damage_of_spells map is empty
    if (player.gcd_remaining <= 0 &&
        ((!kNotEnoughTimeForFillerSpell && player.settings.rotation_option == EmbindConstant::kUserChooses) ||
         predicted_damage_of_spells.size() == 0) &&
        player.filler->CanCast()) {
      SelectedSpellHandler(player.filler, predicted_damage_of_spells, fight_time_remaining);
    }

    // If the predicted_damage_of_spells map is not empty then check now
    // which spell would be the best to Cast
    if (player.gcd_remaining <= 0 && player.cast_time_remaining <= 0 && predicted_damage_of_spells.size() != 0) {
      std::shared_ptr<Spell> max_damage_spell;
      auto max_damage_spell_value = 0.0;

      for (auto& spell : predicted_damage_of_spells) {
        if (spell.second > max_damage_spell_value &&
            (fight_time_remaining > player.GetGcdValue() || spell.first->HasEnoughMana())) {
          max_damage_spell = spell.first;
          max_damage_spell_value = spell.second;
        }
      }

      // If a max Damage spell was not found or if the max Damage spell
      // isn't Ready (no mana), then Cast Life Tap
      if (max_damage_spell != NULL && max_damage_spell->HasEnoughMana()) {
        CastSelectedSpell(max_damage_spell, fight_time_remaining, max_damage_spell_value);
      } else {
        player.CastLifeTapOrDarkPact();
      }
    }
  }
  // AoE (currently just does Seed of Corruption by default)
  else {
    if (player.spells.seed_of_corruption->Ready()) {
      player.UseCooldowns(fight_time_remaining);
      player.spells.seed_of_corruption->StartCast();
    } else {
      player.CastLifeTapOrDarkPact();
    }
  }
}

void Simulation::CastPetSpells() {
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

void Simulation::IterationEnd(double fight_length, double dps) {
  player.EndAuras();
  if (player.pet != NULL) {
    player.pet->EndAuras();
  }

  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog("Fight end");
  }

  player.total_fight_duration += fight_length;

  if (dps > max_dps) {
    max_dps = dps;
  }

  if (dps < min_dps) {
    min_dps = dps;
  }

  dps_vector.push_back(dps);

  // Only send the iteration's dps to the web worker if we're doing a normal
  // simulation (this is just for the dps histogram)
  if (settings.simulation_type == SimulationType::kNormal && player.custom_stat == "normal") {
    DpsUpdate(dps);
  }

  if (iteration % static_cast<int>(std::floor(settings.iterations / 100.0)) == 0) {
    SimulationUpdate(iteration, settings.iterations, Median(dps_vector), player.settings.item_id,
                     player.custom_stat.c_str());
  }
}

void Simulation::SimulationEnd(long long simulation_duration) {
  // Send the contents of the combat log to the web worker
  if (player.equipped_item_simulation) {
    player.SendCombatLogEntries();
  }

  // Send the combat log breakdown info
  if (player.recording_combat_log_breakdown) {
    player.SendCombatLogBreakdown();

    if (player.pet != NULL) {
      player.pet->SendCombatLogBreakdown();
    }
  }

  SendSimulationResults(Median(dps_vector), min_dps, max_dps, player.settings.item_id, settings.iterations,
                        static_cast<int>(player.total_fight_duration), player.custom_stat.c_str(), simulation_duration);
}