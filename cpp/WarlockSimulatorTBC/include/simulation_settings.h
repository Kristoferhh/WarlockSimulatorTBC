#pragma once

#include <iostream>

#include "enums.h"

struct SimulationSettings {
  int iterations;
  int min_time;
  int max_time;
  SimulationType simulation_type;
};