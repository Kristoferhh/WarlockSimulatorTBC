#include "../include/simulation.h"

#include <chrono>
#include <iostream>

#include "../include/player.h"
#include "../include/simulation_settings.h"
#include "../include/player_settings.h"
#include "../include/spell.h"
#include "../include/pet.h"
#include "../include/common.h"
#include "../include/aura.h"
#include "../include/trinket.h"
#include "../include/damage_over_time.h"
#include "../include/bindings.h"

Simulation::Simulation(Player& player, const SimulationSettings& kSimulationSettings)
  : player(player),
    kSettings(kSimulationSettings) {
}

void Simulation::Start() {
  player.total_fight_duration = 0;
  player.Initialize(this);
  min_dps = std::numeric_limits<double>::max();
  max_dps = 0;
  const auto kStart = std::chrono::high_resolution_clock::now();

  for (iteration = 0; iteration < kSettings.iterations; iteration++) {
    const int kFightLength = player.rng.Range(kSettings.min_time, kSettings.max_time);

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

      if (player.pet != nullptr && player.settings.pet_mode == EmbindConstant::kAggressive) {
        CastPetSpells();
      }

      if (PassTime(kFightTimeRemaining) <= 0) {
        std::cout << "Iteration " << std::to_string(iteration) << " fightTime: " << std::to_string(current_fight_time)
            << "/" << std::to_string(kFightLength) << " PassTime() returned <= 0" << std::endl;
        player.ThrowError(
            "The simulation got stuck in an endless loop. If you'd like to help with fixing this bug then please "
            "export your current settings and post it in the #sim-bug-report channel on the Warlock Classic discord.");
      }
    }

    IterationEnd(kFightLength, player.iteration_damage / static_cast<double>(kFightLength));
  }

  const auto kEnd = std::chrono::high_resolution_clock::now();
  const auto kMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(kEnd - kStart).count();

  SimulationEnd(kMicroseconds);
}

double Simulation::PassTime(const double kFightTimeRemaining) {
  auto time_until_next_action = player.FindTimeUntilNextAction();

  time_until_next_action = std::min(time_until_next_action, kFightTimeRemaining);

  Tick(time_until_next_action);

  return time_until_next_action;
}

void Simulation::SelectedSpellHandler(const std::shared_ptr<Spell>& kSpell,
                                      std::map<std::shared_ptr<Spell>, double>& predicted_damage_of_spells,
                                      const double kFightTimeRemaining) const {
  if ((player.settings.rotation_option == EmbindConstant::kSimChooses || kSpell->is_finisher) &&
      !predicted_damage_of_spells.contains(kSpell)) {
    predicted_damage_of_spells.insert({kSpell, kSpell->PredictDamage()});
  } else if (kSpell->HasEnoughMana()) {
    CastSelectedSpell(kSpell, kFightTimeRemaining);
  } else {
    player.CastLifeTapOrDarkPact();
  }
}

void Simulation::CastSelectedSpell(const std::shared_ptr<Spell>& kSpell, const double kFightTimeRemaining,
                                   const double kPredictedDamage) const {
  player.UseCooldowns(kFightTimeRemaining);

  if (player.spells.amplify_curse != nullptr && player.spells.amplify_curse->Ready() &&
      (kSpell->name == SpellName::kCurseOfAgony || kSpell->name == SpellName::kCurseOfDoom)) {
    player.spells.amplify_curse->StartCast();
  }

  kSpell->StartCast(kPredictedDamage);
}

void Simulation::Tick(const double kTime) {
  current_fight_time += kTime;
  player.Tick(kTime);
  if (player.pet != nullptr) {
    player.pet->Tick(kTime);
  }
}

void Simulation::IterationReset(const double kFightLength) {
  current_fight_time = 0;

  player.Reset();
  if (player.pet != nullptr) {
    player.pet->Reset();
  }

  player.rng.Seed(player.settings.random_seeds[iteration]);

  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog("Fight length: " + DoubleToString(kFightLength) + " seconds");
  }

  if (player.auras.airmans_ribbon_of_gallantry != nullptr) {
    player.auras.airmans_ribbon_of_gallantry->Apply();
  }

  if (player.auras.fel_energy != nullptr) {
    player.auras.fel_energy->Apply();
  }

  if (player.pet != nullptr) {
    if (player.pet->auras.battle_squawk != nullptr) {
      player.pet->auras.battle_squawk->Apply();
    }

    if (player.settings.prepop_black_book && player.pet->auras.black_book != nullptr) {
      // If the player only has one on-use trinket equipped or if the first trinket doesn't share cooldowns with other
      // trinkets, then assume that Black Book is equipped in the second trinket slot, otherwise the first slot

      if (const auto kBlackBookTrinketSlot = player.trinkets.size() == 1 || !player.trinkets[0].shares_cooldown ? 1 : 0; player.trinkets.size() > kBlackBookTrinketSlot) {
        player.trinkets[kBlackBookTrinketSlot].cooldown_remaining = player.pet->auras.black_book->duration;
      }

      player.pet->auras.black_book->Apply();
    }
  }
}

void Simulation::CastNonPlayerCooldowns(const double kFightTimeRemaining) const {
  // Use Drums
  if (player.spells.drums_of_battle != nullptr && !player.auras.drums_of_battle->active &&
      player.spells.drums_of_battle->Ready()) {
    player.spells.drums_of_battle->StartCast();
  } else if (player.spells.drums_of_war != nullptr && !player.auras.drums_of_war->active &&
             player.spells.drums_of_war->Ready()) {
    player.spells.drums_of_war->StartCast();
  } else if (player.spells.drums_of_restoration != nullptr && !player.auras.drums_of_restoration->active &&
             player.spells.drums_of_restoration->Ready()) {
    player.spells.drums_of_restoration->StartCast();
  }

  // Use Bloodlust
  if (!player.spells.bloodlust.empty() && !player.auras.bloodlust->active) {
    for (const auto& kBloodlust : player.spells.bloodlust) {
      if (kBloodlust->Ready()) {
        kBloodlust->StartCast();
        break;
      }
    }
  }

  // Use Mana Tide Totem if there's <= 12 sec remaining or the player's mana
  // is at 50% or lower
  if (player.spells.mana_tide_totem != nullptr && player.spells.mana_tide_totem->Ready() &&
      (kFightTimeRemaining <= player.auras.mana_tide_totem->duration ||
       player.stats.mana / player.stats.max_mana <= 0.50)) {
    player.spells.mana_tide_totem->StartCast();
  }
}

void Simulation::CastNonGcdSpells() const {
  // Demonic Rune
  if ((current_fight_time > 5 || player.stats.mp5 == 0.0) && player.spells.demonic_rune != nullptr &&
      player.stats.max_mana - player.stats.mana > player.spells.demonic_rune->max_mana_gain &&
      player.spells.demonic_rune->Ready() &&
      (!player.spells.chipped_power_core || !player.spells.chipped_power_core->Ready()) &&
      (!player.spells.cracked_power_core || !player.spells.cracked_power_core->Ready())) {
    player.spells.demonic_rune->StartCast();
  }

  // Super Mana Potion
  if ((current_fight_time > 5 || player.stats.mp5 == 0.0) && player.spells.super_mana_potion != nullptr &&
      player.stats.max_mana - player.stats.mana > player.spells.super_mana_potion->max_mana_gain &&
      player.spells.super_mana_potion->Ready()) {
    player.spells.super_mana_potion->StartCast();
  }
}

void Simulation::CastGcdSpells(const double kFightTimeRemaining) const {
  if (player.settings.fight_type == EmbindConstant::kSingleTarget) {
    const bool kNotEnoughTimeForFillerSpell = kFightTimeRemaining < player.filler->GetCastTime();

    // Map of spells with their predicted Damage as the value. This is
    // used by the sim to determine what the best spell to Cast is.
    std::map<std::shared_ptr<Spell>, double> predicted_damage_of_spells;

    // If the sim is choosing the rotation for the user then predict the
    // damage of the three filler spells if they're available
    if (player.settings.rotation_option == EmbindConstant::kSimChooses) {
      if (kFightTimeRemaining >= player.spells.shadow_bolt->GetCastTime()) {
        predicted_damage_of_spells.insert({player.spells.shadow_bolt, player.spells.shadow_bolt->PredictDamage()});
      }

      if (kFightTimeRemaining >= player.spells.incinerate->GetCastTime()) {
        predicted_damage_of_spells.insert({player.spells.incinerate, player.spells.incinerate->PredictDamage()});
      }

      if (kFightTimeRemaining >= player.spells.searing_pain->GetCastTime()) {
        predicted_damage_of_spells.insert({player.spells.searing_pain, player.spells.searing_pain->PredictDamage()});
      }
    }

    // Cast Conflagrate if there's not enough time for another filler
    // and Immolate is up
    if (kNotEnoughTimeForFillerSpell && player.spells.conflagrate != nullptr && player.spells.conflagrate->CanCast()) {
      SelectedSpellHandler(player.spells.conflagrate, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // Cast Shadowburn if there's not enough time for another filler
    if (player.gcd_remaining <= 0 && kNotEnoughTimeForFillerSpell && player.spells.shadowburn != nullptr &&
        player.spells.shadowburn->CanCast()) {
      SelectedSpellHandler(player.spells.shadowburn, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // Cast Death Coil if there's not enough time for another filler
    if (player.gcd_remaining <= 0 && kNotEnoughTimeForFillerSpell && player.spells.death_coil != nullptr &&
        player.spells.death_coil->CanCast()) {
      SelectedSpellHandler(player.spells.death_coil, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // Cast Curse of the Elements or Curse of Recklessness if they're
    // the selected curse and they're not active
    if (kFightTimeRemaining >= 10 && player.gcd_remaining <= 0 && player.curse_spell != nullptr &&
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
    if (player.gcd_remaining <= 0 && kFightTimeRemaining > 60 && player.curse_spell != nullptr &&
        player.curse_spell->name == SpellName::kCurseOfDoom && !player.auras.curse_of_doom->active &&
        player.spells.curse_of_doom->CanCast()) {
      SelectedSpellHandler(player.spells.curse_of_doom, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // Cast Curse of Agony if CoA is the selected curse or if Curse of
    // Doom is the selected curse and there's less than 60 seconds
    // remaining of the fight
    if (player.gcd_remaining <= 0 && player.auras.curse_of_agony != nullptr && !player.auras.curse_of_agony->active &&
        player.spells.curse_of_agony->CanCast() && kFightTimeRemaining > player.auras.curse_of_agony->duration &&
        (player.curse_spell->name == SpellName::kCurseOfDoom && !player.auras.curse_of_doom->active &&
         (player.spells.curse_of_doom->cooldown_remaining > player.auras.curse_of_agony->duration ||
          kFightTimeRemaining < 60) ||
         player.curse_spell->name == SpellName::kCurseOfAgony)) {
      SelectedSpellHandler(player.spells.curse_of_agony, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // Cast Corruption if Corruption isn't up or if it will expire
    // before the Cast finishes (if no instant Corruption)
    if (player.gcd_remaining <= 0 && player.spells.corruption != nullptr &&
        (!player.auras.corruption->active ||
         player.auras.corruption->ticks_remaining == 1 &&
         player.auras.corruption->tick_timer_remaining < player.spells.corruption->GetCastTime()) &&
        player.spells.corruption->CanCast() &&
        kFightTimeRemaining - player.spells.corruption->GetCastTime() >= player.auras.corruption->duration) {
      SelectedSpellHandler(player.spells.corruption, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // Cast Shadow Bolt if Shadow Trance (Nightfall) is active and
    // Corruption is active as well to avoid potentially wasting another
    // Nightfall proc
    if (player.gcd_remaining <= 0 && player.spells.shadow_bolt != nullptr && player.auras.shadow_trance != nullptr &&
        player.auras.shadow_trance->active && player.auras.corruption->active && player.spells.shadow_bolt->CanCast()) {
      SelectedSpellHandler(player.spells.shadow_bolt, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // Cast Unstable Affliction if it's not up or if it's about to
    // expire
    if (player.gcd_remaining <= 0 && player.spells.unstable_affliction != nullptr &&
        player.spells.unstable_affliction->CanCast() &&
        (!player.auras.unstable_affliction->active ||
         player.auras.unstable_affliction->ticks_remaining == 1 &&
         player.auras.unstable_affliction->tick_timer_remaining < player.spells.unstable_affliction->GetCastTime()) &&
        kFightTimeRemaining - player.spells.unstable_affliction->GetCastTime() >=
        player.auras.unstable_affliction->duration) {
      SelectedSpellHandler(player.spells.unstable_affliction, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // Cast Siphon Life if it's not up (todo: add option to only Cast it
    // while ISB is active if not using custom ISB uptime %)
    if (player.gcd_remaining <= 0 && player.spells.siphon_life != nullptr && !player.auras.siphon_life->active &&
        player.spells.siphon_life->CanCast() && kFightTimeRemaining >= player.auras.siphon_life->duration) {
      SelectedSpellHandler(player.spells.siphon_life, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // Cast Immolate if it's not up or about to expire
    if (player.gcd_remaining <= 0 && player.spells.immolate != nullptr && player.spells.immolate->CanCast() &&
        (!player.auras.immolate->active ||
         player.auras.immolate->ticks_remaining == 1 &&
         player.auras.immolate->tick_timer_remaining < player.spells.immolate->GetCastTime()) &&
        kFightTimeRemaining - player.spells.immolate->GetCastTime() >= player.auras.immolate->duration) {
      SelectedSpellHandler(player.spells.immolate, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // Cast Shadow Bolt if Shadow Trance (Nightfall) is active
    if (player.gcd_remaining <= 0 && player.spells.shadow_bolt != nullptr && player.auras.shadow_trance != nullptr &&
        player.auras.shadow_trance->active && player.spells.shadow_bolt->CanCast()) {
      SelectedSpellHandler(player.spells.shadow_bolt, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // Cast Shadowfury
    if (player.gcd_remaining <= 0 && player.spells.shadowfury != nullptr && player.spells.shadowfury->CanCast()) {
      SelectedSpellHandler(player.spells.shadowfury, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // Cast filler spell if sim is not choosing the rotation for the
    // user or if the predicted_damage_of_spells map is empty
    if (player.gcd_remaining <= 0 &&
        (!kNotEnoughTimeForFillerSpell && player.settings.rotation_option == EmbindConstant::kUserChooses ||
         predicted_damage_of_spells.empty()) &&
        player.filler->CanCast()) {
      SelectedSpellHandler(player.filler, predicted_damage_of_spells, kFightTimeRemaining);
    }

    // If the predicted_damage_of_spells map is not empty then check now
    // which spell would be the best to Cast
    if (player.gcd_remaining <= 0 && player.cast_time_remaining <= 0 && !predicted_damage_of_spells.empty()) {
      std::shared_ptr<Spell> max_damage_spell;
      auto max_damage_spell_value = 0.0;

      for (const auto& [kSpell, kDamage] : predicted_damage_of_spells) {
        if (kDamage > max_damage_spell_value &&
            (kFightTimeRemaining > player.GetGcdValue() || kSpell->HasEnoughMana())) {
          max_damage_spell = kSpell;
          max_damage_spell_value = kDamage;
        }
      }

      // If a max Damage spell was not found or if the max Damage spell
      // isn't Ready (no mana), then Cast Life Tap
      if (max_damage_spell != nullptr && max_damage_spell->HasEnoughMana()) {
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

void Simulation::CastPetSpells() const {
  // Auto Attack
  if (player.pet->spells.melee != nullptr && player.pet->spells.melee->Ready()) {
    player.pet->spells.melee->StartCast();
  }

  // Felguard Cleave
  if (player.pet->spells.cleave != nullptr && player.pet->spells.cleave->Ready()) {
    player.pet->spells.cleave->StartCast();
  }

  // Succubus Lash of Pain
  if (player.pet->spells.lash_of_pain != nullptr && player.pet->spells.lash_of_pain->Ready() &&
      (player.settings.lash_of_pain_usage == EmbindConstant::kOnCooldown ||
       !player.settings.using_custom_isb_uptime &&
       (player.auras.improved_shadow_bolt == nullptr || !player.auras.improved_shadow_bolt->active))) {
    player.pet->spells.lash_of_pain->StartCast();
  }

  // Imp Firebolt
  if (player.pet->spells.firebolt != nullptr && player.pet->spells.firebolt->Ready()) {
    player.pet->spells.firebolt->StartCast();
  }
}

void Simulation::IterationEnd(const double kFightLength, const double kDps) {
  player.EndAuras();
  if (player.pet != nullptr) {
    player.pet->EndAuras();
  }

  if (player.ShouldWriteToCombatLog()) {
    player.CombatLog("Fight end");
  }

  player.total_fight_duration += kFightLength;

  if (kDps > max_dps) {
    max_dps = kDps;
  }

  if (kDps < min_dps) {
    min_dps = kDps;
  }

  dps_vector.push_back(kDps);

  // Only send the iteration's dps to the web worker if we're doing a normal
  // simulation (this is just for the dps histogram)
  if (kSettings.simulation_type == SimulationType::kNormal && player.custom_stat == "normal") {
    DpsUpdate(kDps);
  }

  if (iteration % static_cast<int>(std::floor(kSettings.iterations / 100.0)) == 0) {
    SimulationUpdate(iteration, kSettings.iterations, Median(dps_vector), player.settings.item_id,
                     player.custom_stat.c_str());
  }
}

void Simulation::SimulationEnd(const long long kSimulationDuration) const {
  // Send the contents of the combat log to the web worker
  if (player.equipped_item_simulation) {
    player.SendCombatLogEntries();
  }

  // Send the combat log breakdown info
  if (player.recording_combat_log_breakdown) {
    player.SendCombatLogBreakdown();

    if (player.pet != nullptr) {
      player.pet->SendCombatLogBreakdown();
    }
  }

  SendSimulationResults(Median(dps_vector), min_dps, max_dps, player.settings.item_id, kSettings.iterations,
                        static_cast<int>(player.total_fight_duration), player.custom_stat.c_str(), kSimulationDuration);
}
