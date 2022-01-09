#pragma once

#include <random>

struct Rng {
  void seed(uint32_t s);
  double next();
  int range(int min, int max);

 private:
  std::mt19937 _random_engine;
  std::uniform_real_distribution<double> _random{0, 1};
};