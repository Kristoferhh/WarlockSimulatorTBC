#pragma once
#include <map>
#include <memory>
#include <vector>

struct Spell;
struct SimulationSettings;
struct Player;

struct Simulation {
  Player& player;
  const SimulationSettings& kSettings;
  std::vector<double> dps_vector;
  int iteration = 0;
  double current_fight_time = 0;
  double min_dps = 0;
  double max_dps = 0;

  Simulation(Player& player, const SimulationSettings& kSimulationSettings);
  void Start();
  void IterationReset(double kFightLength);
  void CastNonPlayerCooldowns(double kFightTimeRemaining) const;
  void CastNonGcdSpells() const;
  void CastGcdSpells(double kFightTimeRemaining) const;
  void CastPetSpells() const;
  void IterationEnd(double kFightLength, double kDps);
  void SimulationEnd(long long kSimulationDuration) const;
  double PassTime(double kFightTimeRemaining);
  void Tick(double kTime);
  void SelectedSpellHandler(const std::shared_ptr<Spell>& kSpell,
                            std::map<std::shared_ptr<Spell>, double>& predicted_damage_of_spells,
                            double kFightTimeRemaining) const;
  void CastSelectedSpell(const std::shared_ptr<Spell>& kSpell, double kFightTimeRemaining,
                         double kPredictedDamage = 0) const;
};
