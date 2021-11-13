#ifndef WARLOCK_SIMULATOR_TBC_BINDINGS
#define WARLOCK_SIMULATOR_TBC_BINDINGS

#include "simulation.h"
#include "auras.h"
#include "talents.h"
#include "sets.h"
#include "character_stats.h"
#include <chrono>
#include "constant.h"
#include "aura.h"
#include "items.h"
#include "combat_log_breakdown.h"

#ifdef EMSCRIPTEN
#include "emscripten.h"
#include "emscripten/bind.h"
#endif

std::shared_ptr<Player> allocPlayer(std::shared_ptr<PlayerSettings> settings);
std::shared_ptr<PlayerSettings> allocPlayerSettings();
std::shared_ptr<Items> allocItems();
std::shared_ptr<Auras> allocAuras();
std::shared_ptr<Talents> allocTalents();
std::shared_ptr<Sets> allocSets();
std::shared_ptr<CharacterStats> allocStats();
std::shared_ptr<SimulationSettings> allocSimSettings();
std::shared_ptr<Simulation> allocSim(std::shared_ptr<Player> player, std::shared_ptr<SimulationSettings> simulationSettings);
uint32_t* allocRandomSeeds(int amountOfSeeds);

void dpsUpdate(double dps);
void errorCallback(const char* errorMsg);
void postCombatLogBreakdownVector(const char* name, int manaGain, int damage);
void postCombatLogBreakdown(const char* name, int casts, int crits, int misses, int count, double uptime, int dodges, int glancingBlows);
void combatLogUpdate(const char* combatLogEntry);
void simulationUpdate(int iteration, int iterationAmount, double medianDps, int itemId, const char* customStat);
void simulationEnd(double medianDps, double minDps, double maxDps, int itemId, int iterationAmount, int totalDuration, const char* customStat);
std::string getExceptionMessage(intptr_t exceptionPtr);

#endif
