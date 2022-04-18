#pragma once
#include <random>

struct Rng {
  void Seed(uint32_t kSeed);
  double Next();
  int Range(int kMin, int kMax);

private:
  std::mt19937 _random_engine;
  std::uniform_real_distribution<double> _random{0, 1};
};
