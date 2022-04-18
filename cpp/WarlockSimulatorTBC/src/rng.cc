#include "../include/rng.h"

#include <cmath>

void Rng::Seed(const uint32_t kSeed) { _random_engine.seed(kSeed); }

double Rng::Next() { return _random(_random_engine); }

int Rng::Range(const int kMin, const int kMax) { return static_cast<int>(floor(Next() * (kMax - kMin + 1) + kMin)); }