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
enum class EntityType { kNoType, kPlayer, kPet };
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
  kSpellHasteRating,
  kSpellHitRating,
  kSpellCritRating,
  kSpellCritChance,
  kSpellHitChance,
  kExtraSpellHitChance,
  kMp5,
  kSpellPenetration,
  kFireModifier,
  kSpellHastePercent,
  kShadowModifier,
  kStaminaModifier,
  kIntellectModifier,
  kSpiritModifier,
  kManaCostModifier,
  kStrength,
  kAgility,
  kAttackPower,
  kMeleeCritChance,
  kMeleeHitChance,
  kStrengthModifier,
  kAgilityModifier,
  kAttackPowerModifier,
  kDamageModifier,
  kMeleeHastePercent,
};

#endif