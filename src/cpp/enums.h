#ifndef WARLOCK_SIMULATOR_TBC_ENUMS
#define WARLOCK_SIMULATOR_TBC_ENUMS

enum class SpellSchool { kNoSchool, kShadow, kFire };
enum class SpellType { kNoSpellType, kAffliction, kDestruction };
enum class PetName { kNoName, kImp, kVoidwalker, kSuccubus, kFelhunter, kFelguard };
enum class PetType { kNoPetType, kMelee, kRanged };
enum class PetMode { kNoMode, kPassive, kAggressive };
enum class AttackType { kNoAttackType, kPhysical, kMagical };
enum class SimulationType { kNormal, kAllItems, kStatWeights };
enum class CalculationType { kNoType, kAdditive, kMultiplicative };
enum CharacterStat {
  kHealth,
  kMana,
  kMaxMana,
  kStamina,
  kIntellect,
  kSpirit,
  kSpellPower,
  kShadowPower,
  kFirePower,
  kHasteRating,
  kHitRating,
  kCritRating,
  kCritChance,
  kHitChance,
  kExtraHitChance,
  kMp5,
  kSpellPenetration,
  kFireModifier,
  kHastePercent,
  kShadowModifier,
  kStaminaModifier,
  kIntellectModifier,
  kSpiritModifier,
  kManaCostModifier,
};

#endif