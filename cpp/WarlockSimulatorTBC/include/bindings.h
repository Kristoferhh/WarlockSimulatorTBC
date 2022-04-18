#pragma once

#include <chrono>

#include "player.h"
#include "simulation_settings.h"

#ifdef EMSCRIPTEN
#include "emscripten.h"
#include "emscripten/bind.h"
#endif

Player AllocPlayer(PlayerSettings& settings);
PlayerSettings AllocPlayerSettings(AuraSelection& auras, Talents& talents, Sets& sets, CharacterStats& stats,
                                   Items& items);
Items AllocItems();
AuraSelection AllocAuras();
Talents AllocTalents();
Sets AllocSets();
CharacterStats AllocStats();
SimulationSettings AllocSimSettings();
Simulation AllocSim(Player& player, SimulationSettings& simulation_settings);
std::vector<uint32_t> AllocRandomSeeds(int kAmountOfSeeds, uint32_t kRandSeed = time(nullptr));

void DpsUpdate(double dps);
void ErrorCallback(const char* error_msg);
void PostCombatLogBreakdownVector(const char* name, double mana_gain, double damage);
void PostCombatLogBreakdown(const char* name, uint32_t casts, uint32_t crits, uint32_t misses, uint32_t count,
                            double uptime, uint32_t dodges, uint32_t glancing_blows);
void CombatLogUpdate(const char* combat_log_entry);
void SimulationUpdate(int iteration, int iteration_amount, double median_dps, int item_id, const char* custom_stat);
void SendSimulationResults(double median_dps, double min_dps, double max_dps, int item_id, int iteration_amount,
                           int total_fight_duration, const char* custom_stat, long long simulation_duration);
std::string GetExceptionMessage(intptr_t kExceptionPtr);
