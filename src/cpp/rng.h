#ifndef WARLOCK_SIMULATOR_TBC_RNG
#define WARLOCK_SIMULATOR_TBC_RNG

#include <stdint.h>

#include <array>

/**
 * @brief All Random Number Generation code is taken from the SimulationCraft project.
 * All credit goes to SimulationCraft and their contributors for their RNG implementation.
 * https://github.com/simulationcraft/simc/
 *
 */

template <typename Engine>
struct basic_rng {
 public:
  explicit basic_rng() = default;

  basic_rng(const basic_rng&) = delete;
  basic_rng& operator=(const basic_rng&) = delete;

  /// Seed rng engine
  void seed(uint64_t s) { engine.seed(s); }

  /// Reseed using current state
  uint64_t reseed();

  /// Reset any state
  void reset();

  /// Uniform distribution in range [0..1)
  double real();

  /// Uniform distribution in the range [min..max)
  double range(double min, double max);

  /// Uniform distribution in the range [min..max)
  template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
  T range(T min, T max) {
    return static_cast<T>(range(static_cast<double>(min), static_cast<double>(max)));
  }

  /// Uniform distribution in the range [0..max)
  template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
  T range(T max) {
    return range<T>(T{}, max);
  }

 private:
  Engine engine;
};

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
struct xoshiro256plus {
  uint64_t next() noexcept;
  void seed(uint64_t start) noexcept;

 private:
  std::array<uint64_t, 4> s;
};

struct rng : public basic_rng<xoshiro256plus> {};

#endif