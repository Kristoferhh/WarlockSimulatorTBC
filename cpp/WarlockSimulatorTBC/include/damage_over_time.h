#pragma once
#include <memory>
#include <string>
#include <vector>

enum class SpellSchool;
struct Player;
struct Spell;

struct DamageOverTime {
  virtual ~DamageOverTime() = default;
  Player& player;
  std::shared_ptr<Spell> parent_spell;
  SpellSchool school;
  int duration = 0;          // Total duration of the dot
  int original_duration = 0; // Used for T4 4pc since we're increasing the duration
  // by 3 seconds but need to know what the original
  // duration was
  int tick_timer_total = 3; // Total duration of each tick (default is 3 seconds
  // between ticks)
  double tick_timer_remaining = 0; // Time until next tick
  int ticks_remaining = 0;         // Amount of ticks remaining before the dot expires
  int ticks_total = 0;
  double spell_power = 0; // Spell Power amount when dot was applied
  double base_damage = 0;
  double coefficient = 0;
  double t5_bonus_modifier = 0; // T5 4pc damage modifier
  bool active = false;
  bool applied_with_amplify_curse = false;
  bool isb_is_active = false; // Siphon Life
  std::string name;

  explicit DamageOverTime(Player& player_param);
  void Setup();
  virtual void Apply();
  void Fade();
  void Tick(double kTime);
  [[nodiscard]] std::vector<double> GetConstantDamage() const;
  [[nodiscard]] double PredictDamage() const;
};

struct CorruptionDot final : DamageOverTime {
  explicit CorruptionDot(Player& player_param);
  void Apply() override;
};

struct UnstableAfflictionDot final : DamageOverTime {
  explicit UnstableAfflictionDot(Player& player_param);
};

struct SiphonLifeDot final : DamageOverTime {
  explicit SiphonLifeDot(Player& player_param);
};

struct ImmolateDot final : DamageOverTime {
  explicit ImmolateDot(Player& player_param);
  void Apply() override;
};

struct CurseOfAgonyDot final : DamageOverTime {
  explicit CurseOfAgonyDot(Player& player_param);
};

struct CurseOfDoomDot final : DamageOverTime {
  explicit CurseOfDoomDot(Player& player_param);
};
