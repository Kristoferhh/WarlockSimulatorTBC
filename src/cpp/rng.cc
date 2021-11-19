#include "rng.h"

#include <stdint.h>

/**
 * @brief All Random Number Generation code is taken from the SimulationCraft project.
 * All credit goes to SimulationCraft and their contributors for their RNG implementation.
 * https://github.com/simulationcraft/simc/
 *
 */

/**
 * @brief xoshiro256+ Random Number Generator
 *
 * If, however, one has to generate only 64-bit floating-point numbers (by extracting the upper 53 bits) xoshiro256+
 * is a slightly (≈15%) faster [compared to xoshiro256** or xoshiro256++] generator with analogous statistical
 * properties.
 *
 * All credit goes to David Blackman and Sebastiano Vigna (vigna@acm.org) @2018
 * http://prng.di.unimi.it/
 */
uint64_t xoshiro256plus::next() noexcept {
  const uint64_t result = s[0] + s[3];

  const uint64_t t = s[1] << 17;

  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];

  s[2] ^= t;

  s[3] = rotl(s[3], 45);

  return result;
}

void xoshiro256plus::seed(uint64_t start) noexcept { init_state_from_mix64(s, start); }

constexpr uint64_t rotl(const uint64_t x, int k) { return (x << k) | (x >> (64 - k)); }

template <typename Range>
void init_state_from_mix64(Range& range, uint64_t start) {
  split_mix64_t mix64;
  mix64.seed(start);
  for (auto& elem : range) elem = mix64.next();
}