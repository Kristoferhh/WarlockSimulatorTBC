#include "rng.h"

#include <math.h>

void Rng::seed(uint32_t s) { _random_engine.seed(s); }

double Rng::next() { return _random(_random_engine); }

int Rng::range(int min, int max) { return floor(next() * (max - min + 1) + min); }