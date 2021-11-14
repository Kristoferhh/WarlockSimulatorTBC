#ifndef WARLOCK_SIMULATOR_TBC_BINDINGS
#define WARLOCK_SIMULATOR_TBC_BINDINGS

#include <chrono>

#include "aura.h"
#include "auras.h"
#include "character_stats.h"
#include "combat_log_breakdown.h"
#include "constant.h"
#include "items.h"
#include "sets.h"
#include "simulation.h"
#include "talents.h"

#ifdef EMSCRIPTEN
#include "emscripten.h"
#include "emscripten/bind.h"
#endif

std::shared_ptr<Player> AllocPlayer(std::shared_ptr<PlayerSettings> settings);
std::shared_ptr<PlayerSettings> AllocPlayerSettings();
std::shared_ptr<Items> AllocItems();
std::shared_ptr<Auras> AllocAuras();
std::shared_ptr<Talents> AllocTalents();
std::shared_ptr<Sets> AllocSets();
std::shared_ptr<CharacterStats> AllocStats();
std::shared_ptr<SimulationSettings> AllocSimSettings();
std::shared_ptr<Simulation> AllocSim(std::shared_ptr<Player> player,
                                     std::shared_ptr<SimulationSettings> simulation_settings);
uint32_t* AllocRandomSeeds(int amount_of_seeds);

void DpsUpdate(double dps);
void ErrorCallback(const char* error_msg);
void PostCombatLogBreakdownVector(const char* name, int mana_gain, int damage);
void PostCombatLogBreakdown(const char* name, uint32_t casts, uint32_t crits, uint32_t misses, uint32_t count,
                            double uptime, uint32_t dodges, uint32_t glancing_blows);
void CombatLogUpdate(const char* combat_log_entry);
void SimulationUpdate(int iteration, int iteration_amount, double median_dps, int item_id, const char* custom_stat);
void SimulationEnd(double median_dps, double min_dps, double max_dps, int item_id, int iteration_amount,
                   int total_duration, const char* custom_stat);
std::string GetExceptionMessage(intptr_t exception_ptr);

#endif
