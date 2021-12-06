#ifndef WARLOCK_SIMULATOR_TBC_BINDINGS
#define WARLOCK_SIMULATOR_TBC_BINDINGS

#include <chrono>

#include "aura.h"
#include "auras.h"
#include "combat_log_breakdown.h"
#include "enums.h"
#include "items.h"
#include "sets.h"
#include "simulation.h"
#include "talents.h"

#ifdef EMSCRIPTEN
#include "emscripten.h"
#include "emscripten/bind.h"
#endif

Player AllocPlayer(PlayerSettings& settings);
PlayerSettings AllocPlayerSettings(Auras& auras, Talents& talents, Sets& sets, CharacterStats& stats, Items& items);
Items AllocItems();
Auras AllocAuras();
Talents AllocTalents();
Sets AllocSets();
CharacterStats AllocStats();
SimulationSettings AllocSimSettings();
Simulation AllocSim(Player& player, SimulationSettings& simulation_settings);
std::vector<uint32_t> AllocRandomSeeds(int amount_of_seeds, uint32_t rand_seed);

void DpsUpdate(double dps);
void ErrorCallback(const char* error_msg);
void PostCombatLogBreakdownVector(const char* name, double mana_gain, double damage);
void PostCombatLogBreakdown(const char* name, uint32_t casts, uint32_t crits, uint32_t misses, uint32_t count,
                            double uptime, uint32_t dodges, uint32_t glancing_blows);
void CombatLogUpdate(const char* combat_log_entry);
void SimulationUpdate(int iteration, int iteration_amount, double median_dps, int item_id, const char* custom_stat);
void SimulationEnd(double median_dps, double min_dps, double max_dps, int item_id, int iteration_amount,
                   int total_duration, const char* custom_stat);
std::string GetExceptionMessage(intptr_t exception_ptr);

#endif
