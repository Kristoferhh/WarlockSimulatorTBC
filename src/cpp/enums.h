#ifndef WARLOCK_SIMULATOR_TBC_ENUMS
#define WARLOCK_SIMULATOR_TBC_ENUMS

enum SpellSchool { kNoSchool, kShadow, kFire };
enum SpellType { kNoSpellType, kAffliction, kDestruction };
enum PetName { kNoName, kImp, kVoidwalker, kSuccubus, kFelhunter, kFelguard };
enum PetType { kNoPetType, kMelee, kRanged };
enum PetMode { kNoMode, kPassive, kAggressive };
enum AttackType { kNoAttackType, kPhysical, kMagical };
enum SimulationType { kNormal, kAllItems, kStatWeights };

#endif