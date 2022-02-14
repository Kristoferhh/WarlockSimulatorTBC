export enum ItemSlotKey {
  Head = "head",
  Neck = "neck",
  Shoulders = "shoulders",
  Back = "back",
  Chest = "chest",
  Bracer = "bracer",
  Gloves = "gloves",
  Belt = "belt",
  Legs = "legs",
  Boots = "boots",
  Ring = "ring",
  Trinket = "trinket",
  Mainhand = "mainhand",
  Offhand = "offhand",
  Twohand = "twohand",
  Wand = "wand"
}

export enum ItemSlot {
  head = 'head',
  neck = 'neck',
  shoulders = 'shoulders',
  back = 'back',
  chest = 'chest',
  bracer = 'bracer',
  gloves = 'gloves',
  belt = 'belt',
  legs = 'legs',
  boots = 'boots',
  ring1 = 'ring1',
  ring2 = 'ring2',
  trinket1 = 'trinket1',
  trinket2 = 'trinket2',
  mainhand = 'mainhand',
  offhand = 'offhand',
  twohand = 'twohand',
  wand = 'wand',
}

export interface Item {
  name: string,
  varName: string,
  id: number,
  itemSlot: ItemSlotKey,
  quality: Quality,
  iconName: string,
  stamina?: number,
  intellect?: number,
  spirit?: number,
  sockets?: SocketColor[],
  socketBonus?: SocketBonus,
  spellPower?: number,
  shadowPower?: number,
  firePower?: number,
  hasteRating?: number,
  hitRating?: number,
  critRating?: number,
  resilienceRating?: number,
  spellPenetration?: number,
  shadowResist?: number,
  fireResist?: number,
  mp5?: number,
  setId?: ItemSet,
  displayId?: number,
  unique?: boolean,
  source: ItemSource
  phase: Phase,
}

export interface Enchant {
  name: string,
  varName: string,
  itemSlot: ItemSlotKey,
  quality: Quality,
  spellPower?: number,
  hitRating?: number,
  id: number,
  source: ItemSource,
  phase: Phase,
  mp5?: number,
  mana?: number,
  fireResist?: number,
  shadowResist?: number,
  arcaneResist?: number,
  natureResist?: number,
  frostResist?: number,
  stamina?: number,
  critRating?: number,
  threatReduction?: number,
  spellPenetration?: number,
  intellect?: number,
  resilienceRating?: number,
  spirit?: number,
  shadowPower?: number,
  firePower?: number,
}

export interface Aura {
  name: string,
  varName: AuraName,
  group?: AuraGroup,
  id: number,
  iconName: string,
  stats?: StatsCollection,
  drums?: boolean,
  weaponOil?: boolean,
  foodBuff?: boolean,
  forPet?: boolean,
  demonicRune?: boolean,
  guardianElixir?: boolean,
  battleElixir?: boolean,
  potion?: boolean,
  alcohol?: boolean,
}

export enum PetName {
  IMP = '0',
  SUCCUBUS = '2',
  FELHUNTER = '3',
  FELGUARD = '4',
}

export interface Spell {
  group?: RotationGroup,
  name: string,
  varName: string,
  iconName: string,
  id: number,
}

export enum RotationGroup {
  Dots = 'Dots',
  Filler = 'Filler',
  Curse = 'Curse',
  Finishers = 'Finishers',
  Other = 'Other',
}

export interface IRotationGroup {
  header: RotationGroup,
  varName: string
}

export const rotationGroups: IRotationGroup[] = [
  { header: RotationGroup.Dots, varName: 'dot' },
  { header: RotationGroup.Filler, varName: 'filler' },
  { header: RotationGroup.Curse, varName: 'curse' },
  { header: RotationGroup.Finishers, varName: 'finisher' },
  { header: RotationGroup.Other, varName: 'other' },
]

export interface IAuraGroup {
  heading: AuraGroup,
  varName: string,
  type: 'spell' | 'item'
}

export enum AuraGroup {
  Buffs = 'Buffs',
  Debuffs = 'Debuffs',
  Consumables = 'Consumables',
  PetBuffs = 'Pet Buffs (your pet also inherits group-wide auras selected above)'
}

export interface SocketBonus {
  spellPower?: number,
  stamina?: number,
  spirit?: number,
  hitRating?: number,
  resilienceRating?: number,
  critRating?: number,
  mp5?: number,
  intellect?: number,
}

export enum Race {
  Gnome = 'gnome',
  Human = 'human',
  Orc = 'orc',
  Undead = 'undead',
  BloodElf = 'bloodElf'
}

export enum Stat {
  health = 'health',
  mana = 'mana',
  stamina = 'stamina',
  intellect = 'intellect',
  spirit = 'spirit',
  spellPower = 'spellPower',
  shadowPower = 'shadowPower',
  firePower = 'firePower',
  critRating = 'critRating',
  hitRating = 'hitRating',
  hasteRating = 'hasteRating',
  shadowModifier = 'shadowModifier',
  fireModifier = 'fireModifier',
  mp5 = 'mp5',
  enemyArmor = 'enemyArmor',
  arcaneResist = 'arcaneResist',
  intellectModifier = 'intellectModifier',
  spiritModifier = 'spiritModifier',
  petDamageModifier = 'petDamageModifier',
  shadowResist = 'shadowResist',
  fireResist = 'fireResist',
  frostResist = 'frostResist',
  natureResist = 'natureResist',
  staminaModifier = 'staminaModifier',
  arcaneModifier = 'arcaneModifier',
  frostModifier = 'frostModifier',
  natureModifier = 'natureModifier',
  frostPower = 'frostPower',
  spellPenetration = 'spellPenetration',
}

export interface CombatLogBreakdownData {
  name: string,
  casts: number,
  crits: number,
  misses: number,
  count: number,
  uptime: number,
  dodges: number,
  glancingBlows: number,
  damage: number,
  manaGain: number,
}

export type StatsCollection = {
  [key in Stat]?: number
}

export type PlayerStats = {
  base: StatsCollection,
  auras: StatsCollection,
  items: StatsCollection,
  gems: StatsCollection,
  enchants: StatsCollection,
}

export enum Setting {
  race = 'race',
  improvedDivineSpirit = 'improvedDivineSpirit',
  improvedCurseOfTheElements = 'improvedCurseOfTheElements',
  iterations = 'iterations',
  'min-fight-length' = 'min-fight-length',
  'max-fight-length' = 'max-fight-length',
  'target-level' = 'target-level',
  petChoice = 'petChoice',
  totemOfWrathAmount = 'totemOfWrathAmount',
  chippedPowerCoreAmount = 'chippedPowerCoreAmount',
  crackedPowerCoreAmount = 'crackedPowerCoreAmount',
  petMode = 'petMode',
  sacrificePet = 'sacrificePet',
  enemyArmor = 'enemyArmor',
  mageAtieshAmount = 'mageAtieshAmount',
  warlockAtieshAmount = 'warlockAtieshAmount',
  'target-shadow-resistance' = 'target-shadow-resistance',
  'target-fire-resistance' = 'target-fire-resistance',
  improvedExposeArmor = 'improvedExposeArmor',
  battleSquawkAmount = 'battleSquawkAmount',
  improvedFaerieFire = 'improvedFaerieFire',
  bloodlustAmount = 'bloodlustAmount',
  shadowPriestDps = 'shadowPriestDps',
  survivalHunterAgility = 'survivalHunterAgility',
  exposeWeaknessUptime = 'exposeWeaknessUptime',
  lashOfPainUsage = 'lashOfPainUsage',
  shattrathFaction = 'shattrathFaction',
  shattrathFactionReputation = 'shattrathFactionReputation',
  'automatically-open-sim-details' = 'automatically-open-sim-details',
  customIsbUptime = 'customIsbUptime',
  customIsbUptimeValue = 'customIsbUptimeValue',
  randomizeValues = 'randomizeValues',
  infinitePlayerMana = 'infinitePlayerMana',
  infinitePetMana = 'infinitePetMana',
  prepopBlackBook = 'prepopBlackBook',
  rotationOption = 'rotationOption',
  fightType = 'fightType',
  enemyAmount = 'enemyAmount',
  powerInfusionAmount = 'powerInfusionAmount',
  innervateAmount = 'innervateAmount',
  ferociousInspirationAmount = 'ferociousInspirationAmount',
  improvedImpSetting = 'improvedImpSetting',
  improvedWrathOfAirTotem = 'improvedWrathOfAirTotem',
  maxWebWorkers = 'maxWebWorkers',
}

export type Settings = {
  [key in Setting]: string
}

export const InitialSettings: { [key in Setting]: string } = {
  race: Race.Gnome,
  improvedDivineSpirit: '0',
  improvedCurseOfTheElements: '0',
  iterations: '30000',
  'min-fight-length': '150',
  'max-fight-length': '210',
  'target-level': '73',
  petChoice: '2',
  totemOfWrathAmount: '1',
  petMode: '1',
  sacrificePet: 'yes',
  enemyArmor: '7700',
  mageAtieshAmount: '1',
  warlockAtieshAmount: '1',
  'target-shadow-resistance': '0',
  'target-fire-resistance': '0',
  improvedExposeArmor: '0',
  battleSquawkAmount: '1',
  improvedFaerieFire: 'no',
  bloodlustAmount: '1',
  shadowPriestDps: '1000',
  survivalHunterAgility: '1000',
  exposeWeaknessUptime: '90',
  lashOfPainUsage: 'noISB',
  shattrathFaction: 'Aldor',
  shattrathFactionReputation: 'yes',
  'automatically-open-sim-details': 'no',
  customIsbUptime: 'yes',
  customIsbUptimeValue: '70',
  randomizeValues: 'no',
  infinitePlayerMana: 'no',
  infinitePetMana: 'no',
  prepopBlackBook: 'no',
  rotationOption: 'simChooses',
  fightType: 'singleTarget',
  enemyAmount: '6',
  powerInfusionAmount: '1',
  innervateAmount: '1',
  ferociousInspirationAmount: '1',
  improvedImpSetting: '0',
  improvedWrathOfAirTotem: 'no',
  maxWebWorkers: '0',
  chippedPowerCoreAmount: '1',
  crackedPowerCoreAmount: '1',
}

export type TalentStore = {
  [key in TalentName]: number
}

export type ItemAndEnchantStruct = {
  [key in ItemSlot]: number
}

export type SelectedGemsStruct = {
  [key in ItemSlotKey]: {
    [key: string]: [string, number][]
  }
}

export enum ItemSet {
  T3 = '529',
  VengefulGladiatorsFelshroud = '735',
  VengefulGladiatorsDreadgear = '734',
  MercilessGladiatorsFelshroud = '704',
  MercilessGladiatorsDreadgear = '702',
  Spellfire = '552',
  FrozenShadoweave = '553',
  Spellstrike = '559',
  Oblivion = '644',
  T4 = '645',
  T5 = '646',
  ManaEtchedRegalia = '658',
  IncantersRegalia = '647',
  TwinStars = '667',
  T6 = '670',
  GladiatorsFelshroud = '615',
  GladiatorsDreadgear = '568',
  HighWarlordsDreadgear = '592',
}

export const InitialSelectedItemsAndEnchants: ItemAndEnchantStruct = {
  head: 0,
  neck: 0,
  shoulders: 0,
  back: 0,
  chest: 0,
  bracer: 0,
  gloves: 0,
  belt: 0,
  legs: 0,
  boots: 0,
  ring1: 0,
  ring2: 0,
  trinket1: 0,
  trinket2: 0,
  mainhand: 0,
  offhand: 0,
  twohand: 0,
  wand: 0
}

export const InitialPlayerStats: StatsCollection = {
  health: 0,
  mana: 0,
  stamina: 0,
  intellect: 0,
  spirit: 0,
  spellPower: 0,
  shadowPower: 0,
  firePower: 0,
  critRating: 0,
  hitRating: 0,
  hasteRating: 0,
  shadowModifier: 1,
  fireModifier: 1,
  mp5: 0,
  enemyArmor: 0,
  arcaneResist: 0,
  staminaModifier: 1,
  intellectModifier: 1,
  spiritModifier: 1,
  petDamageModifier: 1,
  arcaneModifier: 1,
  frostModifier: 1,
  natureModifier: 1,
  shadowResist: 0,
  fireResist: 0,
  frostResist: 0,
  natureResist: 0,
  frostPower: 0,
  spellPenetration: 0,
}

export const InitialSelectedGems: SelectedGemsStruct = {
  head: {},
  neck: {},
  shoulders: {},
  back: {},
  chest: {},
  bracer: {},
  gloves: {},
  belt: {},
  legs: {},
  boots: {},
  ring: {},
  trinket: {},
  mainhand: {},
  offhand: {},
  twohand: {},
  wand: {},
}

export interface RotationStruct {
  [key: string]: {
    [key: string]: boolean
  }
}

export type AurasStruct = {
  [key in AuraName]: boolean
}

export interface Profile {
  auras: AurasStruct,
  gems: SelectedGemsStruct,
  items: ItemAndEnchantStruct,
  talents: TalentStore,
  rotation: RotationStruct,
  enchants: ItemAndEnchantStruct,
  simSettings: Settings,
}

export interface PlayerState {
  talents: TalentStore,
  talentPointsRemaining: number,
  selectedItems: ItemAndEnchantStruct,
  selectedEnchants: ItemAndEnchantStruct,
  selectedGems: SelectedGemsStruct,
  auras: AurasStruct,
  rotation: RotationStruct,
  stats: PlayerStats,
  settings: Settings,
  profiles: { [key: string]: Profile },
  sets: SetsStruct,
}

export type SetsStruct = {
  [key in ItemSet]: number
}

export const InitialSetCounts: SetsStruct = {
  [ItemSet.T3]: 0,
  [ItemSet.Spellfire]: 0,
  [ItemSet.FrozenShadoweave]: 0,
  [ItemSet.Spellstrike]: 0,
  [ItemSet.GladiatorsDreadgear]: 0,
  [ItemSet.HighWarlordsDreadgear]: 0,
  [ItemSet.GladiatorsFelshroud]: 0,
  [ItemSet.Oblivion]: 0,
  [ItemSet.T4]: 0,
  [ItemSet.T5]: 0,
  [ItemSet.IncantersRegalia]: 0,
  [ItemSet.ManaEtchedRegalia]: 0,
  [ItemSet.TwinStars]: 0,
  [ItemSet.T6]: 0,
  [ItemSet.MercilessGladiatorsDreadgear]: 0,
  [ItemSet.MercilessGladiatorsFelshroud]: 0,
  [ItemSet.VengefulGladiatorsDreadgear]: 0,
  [ItemSet.VengefulGladiatorsFelshroud]: 0
}

export enum Quality {
  Legendary = 'legendary',
  Epic = 'epic',
  Rare = 'rare',
  Uncommon = 'uncommon',
  Common = 'common'
}

export type SourcesStruct = {
  phase: {
    [key in Phase]: boolean
  }
}

export type SavedItemDps = {
  [key in ItemSlot]: {
    [key: number]: number
  }
}

export interface UiState {
  sources: SourcesStruct,
  gemSelectionTable: GemSelectionTableStruct,
  gemPreferences: { hidden: number[], favorites: number[] },
  selectedProfile: string,
  importExportWindowVisible: boolean,
  equippedItemsWindowVisible: boolean,
  fillItemSocketsWindowVisible: boolean,
  hiddenItems: number[],
  selectedItemSlot: ItemSlotKey,
  selectedItemSubSlot: SubSlotValue,
  savedItemDps: SavedItemDps,
  combatLog: { visible: boolean, data: string[] },
  combatLogBreakdown: CombatLogBreakdown,
  histogram: { visible: boolean, data?: { [key: string]: number } },
  simulationInProgress: boolean,
  statWeights: {
    visible: boolean,
    statValues: { [key in Stat]?: number }
  },
}

export interface StatWeightStats {
  [Stat.stamina]: number,
  [Stat.intellect]: number,
  [Stat.spirit]: number,
  [Stat.spellPower]: number,
  [Stat.shadowPower]: number,
  [Stat.firePower]: number,
  [Stat.hitRating]: number,
  [Stat.critRating]: number,
  [Stat.hasteRating]: number,
  [Stat.mp5]: number,
}

export type CombatLogBreakdown = {
  totalDamageDone: number,
  totalManaGained: number,
  totalSimulationFightLength: number,
  totalIterationAmount: number,
  spellDamageDict: { [key: string]: number },
  spellManaGainDict: { [key: string]: number },
  data: CombatLogBreakdownData[],
}

export interface WorkerParams {
  playerSettings: {
    auras: AurasStruct,
    items: ItemAndEnchantStruct,
    enchants: ItemAndEnchantStruct,
    gems: SelectedGemsStruct,
    talents: TalentStore,
    rotation: RotationStruct,
    stats: StatsCollection,
    sets: SetsStruct,
    simSettings: Settings,
    metaGemId: number,
  },
  simulationSettings: {
    iterations: number,
    minTime: number,
    maxTime: number,
  },
  randomSeed: number,
  itemId: number,
  simulationType: SimulationType,
  itemSubSlot: SubSlotValue,
  customStat: string,
  equippedItemSimulation: boolean,
}

export enum SimulationType {
  Normal,
  AllItems,
  StatWeights
}

export interface GemSelectionTableStruct {
  visible: boolean,
  socketNumber: number,
  itemSlot: ItemSlotKey,
  itemId: string,
  socketColor: SocketColor,
  itemSubSlot?: string
}

export enum SocketColor {
  Meta = 'meta',
  Red = 'red',
  Yellow = 'yellow',
  Blue = 'blue'
}

export const InitialGemSelectionTableValue = {
  visible: false,
  socketNumber: 0,
  itemSlot: ItemSlotKey.Mainhand,
  itemId: '0',
  socketColor: SocketColor.Meta
}

export enum GemColor {
  Meta = 'meta',
  Red = 'red',
  Yellow = 'yellow',
  Blue = 'blue',
  Orange = 'orange',
  Green = 'green',
  Purple = 'purple',
  Void = 'void',
}

export const Languages: { iso: string, name: string, wowheadPrefix: string }[] = [
  { iso: 'en', name: 'English', wowheadPrefix: '' },
  { iso: 'zh', name: '中文', wowheadPrefix: 'cn.' },
]

export interface Gem {
  name: string,
  id: number,
  color: GemColor,
  iconName: string,
  phase: Phase,
  stats?: StatsCollection
}

export interface Talent {
  name?: string,
  varName?: TalentName,
  rankIDs?: number[],
  iconName?: string,
  requirement?: TalentRequirement
}

export interface TalentRequirement {
  name: string,
  points: number
}

export enum TalentTree {
  Affliction = "Affliction",
  Demonology = "Demonology",
  Destruction = "Destruction"
}

export enum ItemSource {
  Sunwell = "Sunwell Plateau",
  ZulAman = "Zul'Aman",
  BlackTemple = "Black Temple",
  MountHyjal = "Mount Hyjal",
  BlackTempleAndMountHyjal = "Hyjal Summit & Black Temple",
  TempestKeep = "Tempest Keep",
  SerpentshrineCavern = "Serpentshrine Cavern",
  MagtheridonsLair = "Magtheridon's Lair",
  GruulsLair = "Gruul's Lair",
  Karazhan = "Karazhan",
  Naxxramas = "Naxxramas",
  AQ40 = "AQ40",
  BlackwingLair = "Blackwing Lair",
  ZulGurub = "Zul'Gurub",
  DoomLordKazzak = "Doom Lord Kazzak",
  Doomwalker = "Doomwalker",
  HeroicBloodFurnace = "Heroic Blood Furnace",
  HeroicUnderbog = "Heroic Underbog",
  HeroicSlavePens = "Heroic Slave Pens",
  HeroicBlackMorass = "Heroic Black Morass",
  HeroicArcatraz = "Heroic Arcatraz",
  HeroicSethekkHalls = "Heroic Sethekk Halls",
  HeroicManaTombs = "Heroic Mana-Tombs",
  HeroicOldHillsbradFoothills = "Heroic Old Hillsbrad Foothills",
  HeroicBotanica = "Heroic Botanica",
  HeroicHellfireRamparts = "Heroic Hellfire Ramparts",
  HeroicAuchenaiCrypts = "Heroic Auchenai Crypts",
  HeroicMagistersTerrace = "Heroic Magisters' Terrace",
  HeroicShadowLabyrinth = "Heroic Shadow Labyrinth",
  Mechanar = "The Mechanar",
  HellfireRamparts = "Hellfire Ramparts",
  BlackrockDepths = "Blackrock Depths",
  BlackMorass = "Black Morass",
  Steamvaults = "The Steamvaults",
  OldHillsbradFoothills = "Old Hillsbrad Foothills",
  AuchenaiCrypts = "Auchenai Crypts",
  ManaTombs = "Mana-Tombs",
  SethekkHalls = "Sethekk Halls",
  Arcatraz = "The Arcatraz",
  Underbog = "The Underbog",
  SlavePens = "The Slave Pens",
  ShadowLabyrinth = "Shadow Labyrinth",
  Botanica = "The Botanica",
  BloodFurnace = "The Blood Furnace",
  ShatteredHalls = "The Shattered Halls",
  MagistersTerrace = "Magisters' Terrace",
  StratholmeLive = "Stratholme (Live)",
  BadgeOfJustice = "Badge of Justice",
  MarkOfHonorHoldOrThrallmar = "Mark of Honor Hold/Thrallmar",
  SpiritShard = "Spirit Shard",
  ApexisShard = "Apexis Shard",
  Tailoring = "Tailoring",
  TailoringBoE = "Tailoring BoE",
  Engineering = "Engineering",
  Leatherworking = "Leatherworking",
  Enchanting = "Enchanting",
  Jewelcrafting = "Jewelcrafting",
  JewelcraftingBoE = "Jewelcrafting BoE",
  BlacksmithingBoE = "Blacksmithing BoE",
  Arena = "Arena",
  PVP = "PVP",
  BlessingsDeck = "Blessings Deck",
  WorldDrop = "World Drop",
  NetherstormRareSpawn = "Netherstorm Rare Spawn",
  BladesEdgeMountainsRareSpawn = "BEM Rare Spawn",
  ShadowmoonValleyRareSpawn = "Shadowmoon Valley Rare Spawn",
  KaelThasEncounter = "Kael'Thas (only during encounter)",
  LordAhune = "Lord Ahune",
  WarlockSpell = "Warlock Spell",
  LowerCityRevered = "Lower City - Revered",
  LowerCityHonored = "Lower City - Honored",
  AldorExalted = "The Aldor - Exalted",
  AldorRevered = "The Aldor - Revered",
  AldorHonored = "The Aldor - Honored",
  ScryersExalted = "The Scryers - Exalted",
  ScryersRevered = "The Scryers - Revered",
  ScryersHonored = "The Scryers - Honored",
  ScaleOfTheSandsExalted = "The Scale of the Sands - Exalted",
  ScaleOfTheSandsRevered = "The Scale of the Sands - Revered",
  ScaleOfTheSandsHonored = "The Scale of the Sands - Honored",
  ScaleOfTheSandsFriendly = "The Scale of the Sands - Friendly",
  VioletEyeExalted = "The Violet Eye - Exalted",
  VioletEyeRevered = "The Violet Eye - Revered",
  VioletEyeHonored = "The Violet Eye - Honored",
  VioletEyeFriendly = "The Violet Eye - Friendly",
  CenarionExpeditionExalted = "Cenarion Expedition - Exalted",
  CenarionExpeditionHonored = "Cenarion Expedition - Honored",
  KeepersOfTimeRevered = "Keepers of Time - Revered",
  KeepersOfTimeHonored = "Keepers of Time - Honored",
  ShatarRevered = "The Sha'tar - Revered",
  ShatarHonored = "The Sha'tar - Honored",
  ZandalarTribeExalted = "Zandalar Tribe - Exalted",
  ShatteredSunOffensiveExalted = "Shattered Sun Offensive - Exalted",
  ShatteredSunOffensiveRevered = "Shattered Sun Offensive - Revered",
  HonorHoldExalted = "Honor Hold - Exalted",
  ThrallmarExalted = "Thrallmar - Exalted",
  HonorHoldOrThrallmarRevered = "Honor Hold/Thrallmar - Revered",
  HonorHoldOrThrallmarHonored = "Honor Hold/Thrallmar - Honored",
  AshtongueDeathswornExalted = "Ashtongue Deathsworn - Exalted",
  ShadowmoonValleyQuest = "Shadowmoon Valley Quest",
  SteamvaultsQuest = "Steamvaults Quest",
  BladesEdgeMountainsQuest = "Blade's Edge Mountains Quest",
  SethekkHallsQuest = "Sethekk Halls Quest",
  AuchenaiCryptsQuest = "Auchenai Crypts Quest",
  NetherstormQuest = "Netherstorm Quest",
  TempestKeepQuest = "Tempest Keep Quest",
  CavernsOfTimeQuest = "Caverns of Time Quest",
  AuchindonQuest = "Auchindon Quest",
  HellfirePeninsulaQuest = "Hellfire Peninsula Quest",
  BloodFurnaceQuest = "The Blood Furnace Quest",
  SilithusQuest = "Silithus Quest",
  NagrandQuest = "Nagrand Quest",
  ZangarmarshQuest = "Zangarmarsh Quest",
  TerokkarForestQuest = "Terokkar Forest Quest",
  ShatteredHallsQuest = "Shattered Halls Quest",
  NagrandQuestHordeOnly = 'Nagrand Quest (Horde)',
  OgrilaBoss = "Ogri'la Boss",
  EthereumPrisoner = 'Ethereum Prisoner',
}

export interface Source {
  phase: { [key in Phase]: boolean }
}

export type Phase = 0 | 1 | 2 | 3 | 4 | 5;

export type SubSlotValue = '' | '1' | '2';

export enum TalentName {
  suppression = 'suppression',
  improvedCorruption = 'improvedCorruption',
  improvedCurseOfWeakness = 'improvedCurseOfWeakness',
  improvedDrainSoul = 'improvedDrainSoul',
  improvedLifeTap = 'improvedLifeTap',
  soulSiphon = 'soulSiphon',
  improvedCurseOfAgony = 'improvedCurseOfAgony',
  felConcentration = 'felConcentration',
  amplifyCurse = 'amplifyCurse',
  grimReach = 'grimReach',
  nightfall = 'nightfall',
  empoweredCorruption = 'empoweredCorruption',
  shadowEmbrace = 'shadowEmbrace',
  siphonLife = 'siphonLife',
  curseOfExhaustion = 'curseOfExhaustion',
  shadowMastery = 'shadowMastery',
  contagion = 'contagion',
  darkPact = 'darkPact',
  improvedHowlOfTerror = 'improvedHowlOfTerror',
  malediction = 'malediction',
  unstableAffliction = 'unstableAffliction',
  improvedHealthstone = 'improvedHealthstone',
  improvedImp = 'improvedImp',
  demonicEmbrace = 'demonicEmbrace',
  improvedHealthFunnel = 'improvedHealthFunnel',
  improvedVoidwalker = 'improvedVoidwalker',
  felIntellect = 'felIntellect',
  improvedSuccubus = 'improvedSuccubus',
  felDomination = 'felDomination',
  felStamina = 'felStamina',
  demonicAegis = 'demonicAegis',
  masterSummoner = 'masterSummoner',
  unholyPower = 'unholyPower',
  improvedEnslaveDemon = 'improvedEnslaveDemon',
  demonicSacrifice = 'demonicSacrifice',
  improvedFirestone = 'improvedFirestone',
  manaFeed = 'manaFeed',
  masterDemonologist = 'masterDemonologist',
  demonicResilience = 'demonicResilience',
  soulLink = 'soulLink',
  demonicKnowledge = 'demonicKnowledge',
  demonicTactics = 'demonicTactics',
  summonFelguard = 'summonFelguard',
  improvedShadowBolt = 'improvedShadowBolt',
  cataclysm = 'cataclysm',
  bane = 'bane',
  aftermath = 'aftermath',
  improvedFirebolt = 'improvedFirebolt',
  improvedLashOfPain = 'improvedLashOfPain',
  devastation = 'devastation',
  shadowburn = 'shadowburn',
  intensity = 'intensity',
  destructiveReach = 'destructiveReach',
  improvedSearingPain = 'improvedSearingPain',
  pyroclasm = 'pyroclasm',
  improvedImmolate = 'improvedImmolate',
  ruin = 'ruin',
  netherProtection = 'netherProtection',
  emberstorm = 'emberstorm',
  backlash = 'backlash',
  conflagrate = 'conflagrate',
  soulLeech = 'soulLeech',
  shadowAndFlame = 'shadowAndFlame',
  shadowfury = 'shadowfury'
}

export const StatConstant = {
  healthPerStamina: 10,
  manaPerIntellect: 15,
  hitRatingPerPercent: 12.62,
  critRatingPerPercent: 22.08,
  hasteRatingPerPercent: 15.77,
  critPercentPerIntellect: 1 / 81.95,
  baseCritChancePercent: 1.701,
  hitPercentCap: 16,
}

export enum AuraName {
  felArmor = 'felArmor',
  blessingOfKings = 'blessingOfKings',
  blessingOfWisdom = 'blessingOfWisdom',
  judgementOfWisdom = 'judgementOfWisdom',
  manaSpringTotem = 'manaSpringTotem',
  wrathOfAirTotem = 'wrathOfAirTotem',
  totemOfWrath = 'totemOfWrath',
  markOfTheWild = 'markOfTheWild',
  arcaneIntellect = 'arcaneIntellect',
  prayerOfFortitude = 'prayerOfFortitude',
  prayerOfSpirit = 'prayerOfSpirit',
  bloodPact = 'bloodPact',
  inspiringPresence = 'inspiringPresence',
  moonkinAura = 'moonkinAura',
  powerInfusion = 'powerInfusion',
  powerOfTheGuardianWarlock = 'powerOfTheGuardianWarlock',
  powerOfTheGuardianMage = 'powerOfTheGuardianMage',
  eyeOfTheNight = 'eyeOfTheNight',
  chainOfTheTwilightOwl = 'chainOfTheTwilightOwl',
  jadePendantOfBlasting = 'jadePendantOfBlasting',
  idolOfTheRavenGoddess = 'idolOfTheRavenGoddess',
  drumsOfBattle = 'drumsOfBattle',
  drumsOfWar = 'drumsOfWar',
  drumsOfRestoration = 'drumsOfRestoration',
  bloodlust = 'bloodlust',
  ferociousInspiration = 'ferociousInspiration',
  innervate = 'innervate',
  manaTideTotem = 'manaTideTotem',
  airmansRibbonOfGallantry = 'airmansRibbonOfGallantry',
  curseOfTheElements = 'curseOfTheElements',
  shadowWeaving = 'shadowWeaving',
  improvedScorch = 'improvedScorch',
  misery = 'misery',
  judgementOfTheCrusader = 'judgementOfTheCrusader',
  vampiricTouch = 'vampiricTouch',
  faerieFire = 'faerieFire',
  sunderArmor = 'sunderArmor',
  exposeArmor = 'exposeArmor',
  curseOfRecklessness = 'curseOfRecklessness',
  bloodFrenzy = 'bloodFrenzy',
  exposeWeakness = 'exposeWeakness',
  annihilator = 'annihilator',
  improvedHuntersMark = 'improvedHuntersMark',
  flaskOfPureDeath = 'flaskOfPureDeath',
  elixirOfEmpowerment = 'elixirOfEmpowerment',
  elixirOfMajorShadowPower = 'elixirOfMajorShadowPower',
  elixirOfMajorFirepower = 'elixirOfMajorFirepower',
  greaterArcaneElixir = 'greaterArcaneElixir',
  adeptsElixir = 'adeptsElixir',
  elixirOfDraenicWisdom = 'elixirOfDraenicWisdom',
  elixirOfMajorMageblood = 'elixirOfMajorMageblood',
  superManaPotion = 'superManaPotion',
  destructionPotion = 'destructionPotion',
  brilliantWizardOil = 'brilliantWizardOil',
  superiorWizardOil = 'superiorWizardOil',
  blessedWizardOil = 'blessedWizardOil',
  demonicRune = 'demonicRune',
  flameCap = 'flameCap',
  rumseyRumBlackLabel = 'rumseyRumBlackLabel',
  kreegsStoutBeatdown = 'kreegsStoutBeatdown',
  blackenedBasilisk = 'blackenedBasilisk',
  skullfishSoup = 'skullfishSoup',
  veryBerryCream = 'veryBerryCream',
  midsummerSausage = 'midsummerSausage',
  bloodthistle = 'bloodthistle',
  chippedPowerCore = 'chippedPowerCore',
  crackedPowerCore = 'crackedPowerCore',
  crystalOfInsight = 'crystalOfInsight',
  blessingOfKingsPet = 'blessingOfKingsPet',
  blessingOfWisdomPet = 'blessingOfWisdomPet',
  blessingOfMight = 'blessingOfMight',
  battleSquawk = 'battleSquawk',
  arcaneIntellectPet = 'arcaneIntellectPet',
  markOfTheWildPet = 'markOfTheWildPet',
  prayerOfFortitudePet = 'prayerOfFortitudePet',
  prayerOfSpiritPet = 'prayerOfSpiritPet',
  kiblersBits = 'kiblersBits',
  heroicPresence = 'heroicPresence',
  strengthOfEarthTotem = 'strengthOfEarthTotem',
  graceOfAirTotem = 'graceOfAirTotem',
  battleShout = 'battleShout',
  trueshotAura = 'trueshotAura',
  leaderOfThePack = 'leaderOfThePack',
  unleashedRage = 'unleashedRage',
  scrollOfStaminaV = 'scrollOfStaminaV',
  scrollOfIntellectV = 'scrollOfIntellectV',
  scrollOfStrengthV = 'scrollOfStrengthV',
  scrollOfAgilityV = 'scrollOfAgilityV',
  scrollOfSpiritV = 'scrollOfSpiritV',
  improvedShadowBolt = 'improvedShadowBolt',
  flameshadow = 'flameshadow',
  shadowflame = 'shadowflame',
  nightfall = 'nightfall',
  spellstrike = 'spellstrike',
  manaEtched4Set = 'manaEtched4Set',
  bloodFury = 'bloodFury',
  shatteredSunPendantOfAcumenAldor = 'shatteredSunPendantOfAcumenAldor',
  shatteredSunPendantOfAcumenScryers = 'shatteredSunPendantOfAcumenScryers',
  mysticalSkyfireDiamond = 'mysticalSkyfireDiamond',
  demonicFrenzy = 'demonicFrenzy',
  blackBook = 'theBlackBook',
  felEnergy = 'felEnergy',
}