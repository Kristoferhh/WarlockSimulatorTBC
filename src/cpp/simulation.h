#pragma once

#include "player/player.h"
#include "simulation_settings.h"

struct Simulation {
  Player& player;
  const SimulationSettings& settings;
  std::vector<double> dps_vector;
  int iteration;
  double current_fight_time;
  double min_dps;
  double max_dps;

  Simulation(Player& player, const SimulationSettings& sim_settings);
  void Start();
  void IterationReset(double fight_length);
  void CastNonPlayerCooldowns(double fight_time_remaining);
  void CastNonGcdSpells();
  void CastGcdSpells(double fight_time_remaining);
  void CastPetSpells();
  void IterationEnd(double fight_length, double dps);
  void SimulationEnd();
  double PassTime();
  void Tick(double time);
  void SelectedSpellHandler(const std::shared_ptr<Spell>& spell,
                            std::map<std::shared_ptr<Spell>, double>& predicted_damage_of_spells,
                            double fight_time_remaining);
  void CastSelectedSpell(const std::shared_ptr<Spell>& spell, double fight_time_remaining, double predicted_damage = 0);
};