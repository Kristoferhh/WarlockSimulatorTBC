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

namespace SpellName {
const std::string kShadowBolt = "Shadow Bolt";
const std::string kLifeTap = "Life Tap";
const std::string kIncinerate = "Incinerate";
const std::string kSearingPain = "Searing Pain";
const std::string kCorruption = "Corruption";
const std::string kImmolate = "Immolate";
const std::string kUnstableAffliction = "Unstable Affliction";
const std::string kSiphonLife = "Siphon Life";
const std::string kCurseOfDoom = "Curse of Doom";
const std::string kCurseOfAgony = "Curse of Agony";
const std::string kCurseOfTheElements = "Curse of the Elements";
const std::string kCurseOfRecklessness = "Curse of Recklessness";
const std::string kSoulFire = "Soul Fire";
const std::string kShadowburn = "Shadowburn";
const std::string kDeathCoil = "Death Coil";
const std::string kShadowfury = "Shadowfury";
const std::string kSeedOfCorruption = "Seed of Corruption";
const std::string kConflagrate = "Conflagrate";
const std::string kDestructionPotion = "DestructionPotion";
const std::string kFlameCap = "Flame Cap";
const std::string kBloodFury = "Blood Fury";
const std::string kBloodlust = "Bloodlust";
const std::string kDrumsOfBattle = "Drums of Battle";
const std::string kDrumsOfWar = "Drums of War";
const std::string kDrumsOfRestoration = "Drums of Restoration";
const std::string kTimbalsFocusingCrystal = "Timbal's Focusing Crystal";
const std::string kMarkOfDefiance = "Mark of Defiance";
const std::string kTheLightningCapacitor = "The Lightning Capacitor";
const std::string kBladeOfWizardry = "Blade of Wizardry";
const std::string kShatteredSunPendantOfAcumenAldor = "Shattered Sun Pendant of Acumen (Aldor)";
const std::string kShatteredSunPendantOfAcumenScryers = "Shattered Sun Pendant of Acumen (Scryers)";
const std::string kRobeOfTheElderScribes = "Robe of the Elder Scribes";
const std::string kQuagmirransEye = "Quagmirran's Eye";
const std::string kShiffarsNexusHorn = "Shiffar's Nexus Horn";
const std::string kSextantOfUnstableCurrents = "Sextant of Unstable Currents";
const std::string kBandOfTheEternalSage = "Band of the Eternal Sage";
const std::string kMysticalSkyfireDiamond = "Mystical Skyfire Diamond";
const std::string kInsightfulEarthstormDiamond = "Insightful Earthstorm Diamond";
const std::string kAmplifyCurse = "Amplify Curse";
const std::string kPowerInfusion = "Power Infusion";
const std::string kInnervate = "Innervate";
const std::string kChippedPowerCore = "Chipped Power Core";
const std::string kCrackedPowerCore = "Cracked Power Core";
const std::string kNightfall = "Nightfall";
const std::string kManaTideTotem = "Mana Tide Totem";
const std::string kJudgementOfWisdom = "Judgement of Wisdom";
const std::string kFlameshadow = "Flameshadow";
const std::string kShadowflame = "Shadowflame";
const std::string kSpellstrike = "Spellstrike";
const std::string kManaEtched4Set = "Mana-Etched 4-Set";
const std::string kAshtongueTalismanOfShadows = "Ashtongue Talisman of Shadows";
const std::string kWrathOfCenarius = "Wrath of Cenarius";
const std::string kDarkmoonCardCrusade = "Darkmoon Card: Crusade";
const std::string kDarkPact = "Dark Pact";
const std::string kSuperManaPotion = "Super Mana Potion";
const std::string kDemonicRune = "Demonic Rune";
const std::string kFirebolt = "Firebolt";
const std::string kDemonicFrenzy = "Demonic Frenzy";
const std::string kLashOfPain = "Lash of Pain";
const std::string kMelee = "Melee";
const std::string kCleave = "Cleave";
const std::string kBlackBook = "The Black Book";
const std::string kBattleSquawk = "Battle Squawk";
const std::string kImprovedShadowBolt = "Improved Shadow Bolt";
const std::string kEyeOfMagtheridon = "Eye of Magtheridon";
const std::string kAirmansRibbonOfGallantry = "Airman's Ribbon of Gallantry";
}  // namespace SpellName

namespace StatName {
const std::string kSpellPower = "Spell Power";
const std::string kShadowPower = "Shadow Power";
const std::string kFirePower = "Fire Power";
const std::string kSpellHasteRating = "Spell Haste Rating";
const std::string kSpellHastePercent = "Spell Haste Percent";
const std::string kMeleeHastePercent = "Melee Haste Percent";
const std::string kManaCostModifier = "Mana Cost Modifier";
const std::string kSpellCritChance = "Spell Crit Chance";
const std::string kSpellCritRating = "Spell Crit Rating";
const std::string kAttackPower = "Attack Power";
}  // namespace StatName

namespace ItemId {
const int kDarkIronSmokingPipe = 38290;
const int kShatteredSunPendantOfAcumen = 34678;
const int kTimbalsFocusingCrystal = 34470;
const int kShiftingNaaruSliver = 34429;
const int kHexShrunkenHead = 33829;
const int kAshtongueTalismanOfShadows = 32493;
const int kSkullOfGuldan = 32483;
const int kDarkmoonCardCrusade = 31856;
const int kBladeOfWizardry = 31336;
const int kSextantOfUnstableCurrents = 30626;
const int kStarkillersBauble = 30340;
const int kEssenceOfTheMartyr = 29376;
const int kIconOfTheSilverCrescent = 29370;
const int kBandOfTheEternalSage = 29305;
const int kXirisGift = 29179;
const int kScryersBloodgem = 29132;
const int kEyeOfMagtheridon = 28789;
const int kRobeOfTheElderScribes = 28602;
const int kShiffarsNexusHorn = 28418;
const int kTheLightningCapacitor = 28785;
const int kArcanistsStone = 28223;
const int kVengeanceOfTheIllidari = 28040;
const int kMarkOfDefiance = 27922;
const int kQuagmirransEye = 27683;
const int kTerokkarTabletOfVim = 25936;
const int kInsightfulEarthstormDiamond = 25901;
const int kMysticalSkyfireDiamond = 25893;
const int kAncientCrystalTalisman = 25620;
const int kFigurineLivingRubySerpent = 24126;
const int kMarkOfTheChampion = 23207;
const int kRestrainedEssenceOfSapphiron = 23046;
const int kWrathOfCenarius = 21190;
const int kHazzarahsCharmOfDestruction = 19957;
}  // namespace ItemId

#endif