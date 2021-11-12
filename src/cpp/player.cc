#include "player.h"
#include "spell.h"
#include "common.h"
#include "damage_over_time.h"
#include "bindings.h"

Player::Player(std::shared_ptr<PlayerSettings> playerSettings)
    : selectedAuras(playerSettings->auras), talents(playerSettings->talents), sets(playerSettings->sets), stats(playerSettings->stats), items(playerSettings->items), settings(playerSettings)
{
    spells = std::make_unique<PlayerSpells>();
    auras = std::make_unique<PlayerAuras>();
    combatLogEntries = {};
    level = 70;
    castTimeRemaining = 0;
    gcdRemaining = 0;
    gcdValue = 1.5;
    // Increases the cast time of spells by a very small amount to e.g. make it so that if Immolate has 1.5 seconds remaining, the sim can start precasting Immolate and it won't refresh before it expires.
    spellDelay = 0.0001;
    // Multiply hit and crit percent by this number to get rid of the decimals when calling random() since we need integers
    critChanceMultiplier = 1000;
    minimumGcdValue = 1;
    critMultiplier = 1.5;
    // I don't know if this formula only works for bosses or not, so for the moment I'm only using it for lvl >=73 targets.
    const double enemyBaseResistance = settings->enemyLevel >= 73 ? (6 * level * 5) / 75.0 : 0;
    settings->enemyShadowResist = std::max(static_cast<double>(settings->enemyShadowResist), enemyBaseResistance);
    settings->enemyFireResist = std::max(static_cast<double>(settings->enemyFireResist), enemyBaseResistance);

    combatLogBreakdown.insert(std::make_pair("Mp5", std::make_unique<CombatLogBreakdown>("Mp5")));
    if (selectedAuras->judgementOfWisdom)
    {
        combatLogBreakdown.insert(std::make_pair("Judgement of Wisdom", std::make_unique<CombatLogBreakdown>("Judgement of Wisdom")));
    }

    // Crit chance
    if (selectedAuras->powerOfTheGuardianMage)
    {
        stats->critRating += 28 * settings->mageAtieshAmount;
    }
    stats->critChance = baseCritChancePercent + (stats->critRating / critRatingPerPercent) + talents->devastation + talents->backlash + talents->demonicTactics;
    if (selectedAuras->moonkinAura)
    {
        stats->critChance += 5;
    }
    if (selectedAuras->judgementOfTheCrusader)
    {
        stats->critChance += 3;
    }
    if (selectedAuras->totemOfWrath)
    {
        stats->critChance += (3 * settings->totemOfWrathAmount);
    }
    if (selectedAuras->chainOfTheTwilightOwl)
    {
        stats->critChance += 2;
    }

    // Hit chance
    if (sets->manaEtched >= 2)
    {
        stats->hitRating += 35;
    }
    stats->extraHitChance = stats->hitRating / hitRatingPerPercent;
    if (selectedAuras->totemOfWrath)
    {
        stats->extraHitChance += (3 * settings->totemOfWrathAmount);
    }
    if (selectedAuras->inspiringPresence)
    {
        stats->extraHitChance += 1;
    }
    stats->hitChance = round(getBaseHitChance(level, settings->enemyLevel));

    // Add bonus damage % from Demonic Sacrifice
    if (talents->demonicSacrifice == 1 && settings->sacrificingPet)
    {
        if (settings->petIsImp)
        {
            stats->fireModifier *= 1.15;
        }
        else if (settings->petIsSuccubus)
        {
            stats->shadowModifier *= 1.15;
        }
        else if (settings->petIsFelguard)
        {
            stats->shadowModifier *= 1.1;
        }
        //todo add felhunter mana regen maybe
    }
    else
    {
        // Add damage % multiplier from Master Demonologist and Soul Link
        if (talents->soulLink == 1)
        {
            stats->shadowModifier *= 1.05;
            stats->fireModifier *= 1.05;
        }
        if (talents->masterDemonologist > 0)
        {
            if (settings->petIsSuccubus)
            {
                stats->shadowModifier *= (1 + 0.02 * talents->masterDemonologist);
                stats->fireModifier *= (1 + 0.02 * talents->masterDemonologist);
            }
            else if (settings->petIsFelguard)
            {
                stats->shadowModifier *= (1 + 0.01 * talents->masterDemonologist);
                stats->fireModifier *= (1 + 0.01 * talents->masterDemonologist);
            }
        }
    }
    // Shadow Mastery
    stats->shadowModifier *= (1 + (0.02 * talents->shadowMastery));
    // Ferocious Inspiration
    if (selectedAuras->ferociousInspiration)
    {
        stats->shadowModifier *= std::pow(1.03, settings->ferociousInspirationAmount);
        stats->fireModifier *= std::pow(1.03, settings->ferociousInspirationAmount);
    }
    // Add % dmg modifiers from Curse of the Elements + Malediction
    if (selectedAuras->curseOfTheElements)
    {
        stats->shadowModifier *= 1.1 + (0.01 * settings->improvedCurseOfTheElements);
        stats->fireModifier *= 1.1 + (0.01 * settings->improvedCurseOfTheElements);
    }
    // Add fire dmg % from Emberstorm
    if (talents->emberstorm > 0)
    {
        stats->fireModifier *= 1 + (0.02 * talents->emberstorm);
    }
    // Add spell power from Fel Armor
    if (selectedAuras->felArmor)
    {
        stats->spellPower += 100 * (0 + 0.1 * talents->demonicAegis);
    }
    // If using a custom isb uptime % then just add to the shadow modifier % (shared_from_this() assumes 5/5 ISB giving 20% shadow damage)
    if (settings->usingCustomIsbUptime)
    {
        stats->shadowModifier *= (1.0 + 0.2 * (settings->customIsbUptimeValue / 100.0));
    }
    // Add spell power from Improved Divine Spirit
    stats->spiritModifier *= (1 - (0.01 * talents->demonicEmbrace));
    if (selectedAuras->prayerOfSpirit && settings->improvedDivineSpirit > 0)
    {
        stats->spellPower += (stats->spirit * stats->spiritModifier * (0 + (static_cast<double>(settings->improvedDivineSpirit) / 20.0)));
    }
    // Elemental shaman t4 2pc bonus
    if (selectedAuras->wrathOfAirTotem && settings->hasElementalShamanT4Bonus)
    {
        stats->spellPower += 20;
    }
    // Add extra stamina from Blood Pact from Improved Imp
    if (selectedAuras->bloodPact)
    {
        int improvedImpPoints = settings->improvedImp;

        if (settings->petIsImp && (!settings->sacrificingPet || talents->demonicSacrifice == 0) && talents->improvedImp > improvedImpPoints)
        {
            improvedImpPoints = talents->improvedImp;
        }

        stats->stamina += 70 * 0.1 * improvedImpPoints;
    }
    // Add stamina from Demonic Embrace
    stats->staminaModifier *= 1 + (0.03 * talents->demonicEmbrace);
    // Add mp5 from Vampiric Touch (add 25% instead of 5% since we're adding it to the mana per 5 seconds variable)
    if (selectedAuras->vampiricTouch)
    {
        stats->mp5 += settings->shadowPriestDps * 0.25;
    }
    if (selectedAuras->powerOfTheGuardianWarlock)
    {
        stats->spellPower += 33 * settings->warlockAtieshAmount;
    }
    if (sets->twinStars == 2)
    {
        stats->spellPower += 15;
    }

    // Enemy Armor Reduction
    if (selectedAuras->faerieFire)
    {
        settings->enemyArmor -= 610;
    }
    if ((selectedAuras->sunderArmor && selectedAuras->exposeArmor && settings->improvedExposeArmor == 2) || (selectedAuras->exposeArmor && !selectedAuras->sunderArmor))
    {
        settings->enemyArmor -= 2050 * (1 + 0.25 * settings->improvedExposeArmor);
    }
    else if (selectedAuras->sunderArmor)
    {
        settings->enemyArmor -= 520 * 5;
    }
    if (selectedAuras->curseOfRecklessness)
    {
        settings->enemyArmor -= 800;
    }
    if (selectedAuras->annihilator)
    {
        settings->enemyArmor -= 600;
    }
    settings->enemyArmor = std::max(0, settings->enemyArmor);

    // Health & Mana
    stats->health = (stats->health + (stats->stamina * stats->staminaModifier) * healthPerStamina) * (1 + (0.01 * static_cast<double>(talents->felStamina)));
    stats->maxMana = (stats->mana + (stats->intellect * stats->intellectModifier) * manaPerInt) * (1 + (0.01 * static_cast<double>(talents->felIntellect)));

    // Pet
    // Spell Power from the Demonic Knowledge talent
    demonicKnowledgeSpellPower = 0;
    if (!settings->sacrificingPet || talents->demonicSacrifice == 0)
    {
        if (settings->petIsImp)
        {
            pet = std::make_unique<Imp>(shared_from_this());
        }
        else if (settings->petIsSuccubus)
        {
            pet = std::make_unique<Succubus>(shared_from_this());
        }
        else if (settings->petIsFelguard)
        {
            pet = std::make_unique<Felguard>(shared_from_this());
        }
    }

    combatLogEntries.push_back("---------------- Player stats ----------------");
    combatLogEntries.push_back("Health: " + truncateTrailingZeros(std::to_string(round(stats->health))));
    combatLogEntries.push_back("Mana: " + truncateTrailingZeros(std::to_string(round(stats->maxMana))));
    combatLogEntries.push_back("Stamina: " + truncateTrailingZeros(std::to_string(round(stats->stamina * stats->staminaModifier))));
    combatLogEntries.push_back("Intellect: " + truncateTrailingZeros(std::to_string(round(stats->intellect * stats->intellectModifier))));
    combatLogEntries.push_back("Spell Power: " + truncateTrailingZeros(std::to_string(round(getSpellPower()))));
    combatLogEntries.push_back("Shadow Power: " + std::to_string(stats->shadowPower));
    combatLogEntries.push_back("Fire Power: " + std::to_string(stats->firePower));
    combatLogEntries.push_back("Crit Chance: " + truncateTrailingZeros(std::to_string(round(getCritChance(SpellType::DESTRUCTION) * 100) / 100), 2) + "%");
    combatLogEntries.push_back("Hit Chance: " + truncateTrailingZeros(std::to_string(std::min(static_cast<double>(16), round((stats->extraHitChance) * 100) / 100)), 2) + "%");
    combatLogEntries.push_back("Haste: " + truncateTrailingZeros(std::to_string(round((stats->hasteRating / hasteRatingPerPercent) * 100) / 100), 2) + "%");
    combatLogEntries.push_back("Shadow Modifier: " + truncateTrailingZeros(std::to_string(stats->shadowModifier * 100)) + "%");
    combatLogEntries.push_back("Fire Modifier: " + truncateTrailingZeros(std::to_string(stats->fireModifier * 100)) + "%");
    combatLogEntries.push_back("MP5: " + std::to_string(stats->mp5));
    combatLogEntries.push_back("Spell Penetration: " + std::to_string(stats->spellPen));
    if (pet != NULL)
    {
        combatLogEntries.push_back("---------------- Pet stats ----------------");
        combatLogEntries.push_back("Stamina: " + truncateTrailingZeros(std::to_string(round(pet->stats->stamina * pet->stats->staminaModifier))));
        combatLogEntries.push_back("Intellect: " + truncateTrailingZeros(std::to_string(round(pet->stats->intellect * pet->stats->intellectModifier))));
        combatLogEntries.push_back("Strength: " + truncateTrailingZeros(std::to_string(round((pet->baseStats->strength + pet->buffStats->strength + pet->stats->strength) * pet->stats->strengthModifier))));
        combatLogEntries.push_back("Agility: " + truncateTrailingZeros(std::to_string(round(pet->stats->agility * pet->stats->agilityModifier))));
        combatLogEntries.push_back("Spirit: " + truncateTrailingZeros(std::to_string(round((pet->baseStats->spirit + pet->buffStats->spirit + pet->stats->spirit) * pet->stats->spiritModifier))));
        combatLogEntries.push_back("Attack Power: " + truncateTrailingZeros(std::to_string(round(pet->stats->attackPower))) + " (without attack power % modifiers)");
        combatLogEntries.push_back("Spell Power: " + truncateTrailingZeros(std::to_string(pet->stats->spellPower)));
        combatLogEntries.push_back("Mana: " + truncateTrailingZeros(std::to_string(pet->stats->maxMana)));
        combatLogEntries.push_back("MP5: " + std::to_string(pet->stats->mp5));
        if (pet->petType == PetType::MELEE)
        {
            combatLogEntries.push_back("Physical Hit Chance: " + truncateTrailingZeros(std::to_string(round(pet->getMeleeHitChance() * 100) / 100.0), 2) + "%");
            combatLogEntries.push_back("Physical Crit Chance: " + truncateTrailingZeros(std::to_string(round(pet->getMeleeCritChance() * 100) / 100.0), 2) + "% (" + truncateTrailingZeros(std::to_string(pet->critSuppression), 2) + "% Crit Suppression Applied)");
            combatLogEntries.push_back("Glancing Blow Chance: " + truncateTrailingZeros(std::to_string(round(pet->glancingBlowChance * 100) / 100.0), 2) + "%");
            combatLogEntries.push_back("Attack Power Modifier: " + truncateTrailingZeros(std::to_string(round(pet->stats->attackPowerModifier * 10000) / 100.0), 2) + "%");
        }
        if (pet->pet == PetName::IMP || pet->pet == PetName::SUCCUBUS)
        {
            combatLogEntries.push_back("Spell Hit Chance: " + truncateTrailingZeros(std::to_string(round(pet->getSpellHitChance() * 100) / 100.0), 2) + "%");
            combatLogEntries.push_back("Spell Crit Chance: " + truncateTrailingZeros(std::to_string(round(pet->getSpellCritChance() * 100) / 100.0), 2) + "%");
        }
        combatLogEntries.push_back("Damage Modifier: " + truncateTrailingZeros(std::to_string(round(pet->stats->damageModifier * 10000) / 100), 2) + "%");
    }
    combatLogEntries.push_back("---------------- Enemy stats ----------------");
    combatLogEntries.push_back("Level: " + std::to_string(settings->enemyLevel));
    combatLogEntries.push_back("Shadow Resistance: " + std::to_string(settings->enemyShadowResist));
    combatLogEntries.push_back("Fire Resistance: " + std::to_string(settings->enemyFireResist));
    if (pet != NULL && pet->pet != PetName::IMP)
    {
        combatLogEntries.push_back("Dodge Chance: " + std::to_string(pet->enemyDodgeChance) + "%");
        combatLogEntries.push_back("Armor: " + std::to_string(settings->enemyArmor));
        combatLogEntries.push_back("Damage Reduction From Armor: " + std::to_string(round((1 - pet->armorMultiplier) * 10000) / 100.0) + "%");
    }
    combatLogEntries.push_back("---------------------------------------------");
}

void Player::initialize()
{
    // Trinkets
    std::vector<int> trinketIds { items->trinket1, items->trinket2 };
    if (std::find(trinketIds.begin(), trinketIds.end(), 32483) != trinketIds.end()) trinkets.push_back(std::make_unique<SkullOfGuldan>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 34429) != trinketIds.end()) trinkets.push_back(std::make_unique<ShiftingNaaruSliver>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 33829) != trinketIds.end()) trinkets.push_back(std::make_unique<HexShrunkenHead>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 29370) != trinketIds.end()) trinkets.push_back(std::make_unique<IconOfTheSilverCrescent>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 29132) != trinketIds.end()) trinkets.push_back(std::make_unique<ScryersBloodgem>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 23046) != trinketIds.end()) trinkets.push_back(std::make_unique<RestrainedEssenceOfSapphiron>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 29179) != trinketIds.end()) trinkets.push_back(std::make_unique<XirisGift>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 25620) != trinketIds.end()) trinkets.push_back(std::make_unique<AncientCrystalTalisman>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 28223) != trinketIds.end()) trinkets.push_back(std::make_unique<ArcanistsStone>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 25936) != trinketIds.end()) trinkets.push_back(std::make_unique<TerokkarTabletOfVim>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 28040) != trinketIds.end()) trinkets.push_back(std::make_unique<VengeanceOfTheIllidari>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 24126) != trinketIds.end()) trinkets.push_back(std::make_unique<FigurineLivingRubySerpent>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 29376) != trinketIds.end()) trinkets.push_back(std::make_unique<EssenceOfTheMartyr>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 30340) != trinketIds.end()) trinkets.push_back(std::make_unique<StarkillersBauble>(shared_from_this()));
    if (std::find(trinketIds.begin(), trinketIds.end(), 38290) != trinketIds.end()) trinkets.push_back(std::make_unique<DarkIronSmokingPipe>(shared_from_this()));

    // Auras
    if (settings->isSingleTarget)
    {
        if (talents->improvedShadowBolt > 0) auras->ImprovedShadowBolt = std::make_shared<ImprovedShadowBoltAura>(shared_from_this());
        if (settings->hasCorruption || settings->simChoosingRotation) auras->Corruption = std::make_shared<CorruptionDot>(shared_from_this());
        if (talents->unstableAffliction == 1 && (settings->hasUnstableAffliction || settings->simChoosingRotation)) auras->UnstableAffliction = std::make_shared<UnstableAfflictionDot>(shared_from_this());
        if (talents->siphonLife == 1 && (settings->hasSiphonLife || settings->simChoosingRotation)) auras->SiphonLife = std::make_shared<SiphonLifeDot>(shared_from_this());
        if (settings->hasImmolate || settings->simChoosingRotation) auras->Immolate = std::make_shared<ImmolateDot>(shared_from_this());
        if (settings->hasCurseOfAgony || settings->hasCurseOfDoom) auras->CurseOfAgony = std::make_shared<CurseOfAgonyDot>(shared_from_this());
        if (settings->hasCurseOfTheElements) auras->CurseOfTheElements = std::make_shared<CurseOfTheElementsAura>(shared_from_this());
        if (settings->hasCurseOfRecklessness) auras->CurseOfRecklessness = std::make_shared<CurseOfRecklessnessAura>(shared_from_this());
        if (settings->hasCurseOfDoom) auras->CurseOfDoom = std::make_shared<CurseOfDoomDot>(shared_from_this());
        if (talents->nightfall > 0) auras->ShadowTrance = std::make_shared<ShadowTranceAura>(shared_from_this());
        if (talents->amplifyCurse == 1 && (settings->hasAmplifyCurse || settings->simChoosingRotation)) auras->AmplifyCurse = std::make_shared<AmplifyCurseAura>(shared_from_this());
    }
    if (selectedAuras->manaTideTotem) auras->ManaTideTotem = std::make_shared<ManaTideTotemAura>(shared_from_this());
    if (selectedAuras->chippedPowerCore) auras->ChippedPowerCore = std::make_shared<ChippedPowerCoreAura>(shared_from_this());
    if (selectedAuras->crackedPowerCore) auras->CrackedPowerCore = std::make_shared<CrackedPowerCoreAura>(shared_from_this());
    if (selectedAuras->powerInfusion) auras->PowerInfusion = std::make_shared<PowerInfusionAura>(shared_from_this());
    if (selectedAuras->innervate) auras->Innervate = std::make_shared<InnervateAura>(shared_from_this());
    if (selectedAuras->bloodlust) auras->Bloodlust = std::make_shared<BloodlustAura>(shared_from_this());
    if (selectedAuras->destructionPotion) auras->DestructionPotion = std::make_shared<DestructionPotionAura>(shared_from_this());
    if (selectedAuras->flameCap) auras->FlameCap = std::make_shared<FlameCapAura>(shared_from_this());
    if (settings->isOrc) auras->BloodFury = std::make_shared<BloodFuryAura>(shared_from_this());
    if (selectedAuras->drumsOfBattle) auras->DrumsOfBattle = std::make_shared<DrumsOfBattleAura>(shared_from_this());
    else if (selectedAuras->drumsOfWar) auras->DrumsOfWar = std::make_shared<DrumsOfWarAura>(shared_from_this());
    else if (selectedAuras->drumsOfRestoration) auras->DrumsOfRestoration = std::make_shared<DrumsOfRestorationAura>(shared_from_this());
    if (items->mainHand == 31336) auras->BladeOfWizardry = std::make_shared<BladeOfWizardryAura>(shared_from_this());
    if (items->neck == 34678) auras->ShatteredSunPendantOfAcumen = std::make_shared<ShatteredSunPendantOfAcumenAura>(shared_from_this());
    if (items->chest == 28602) auras->RobeOfTheElderScribes = std::make_shared<RobeOfTheElderScribesAura>(shared_from_this());
    if (settings->metaGemId == 25893) auras->MysticalSkyfireDiamond = std::make_shared<MysticalSkyfireDiamondAura>(shared_from_this());
    if (std::find(trinketIds.begin(), trinketIds.end(), 28789) != trinketIds.end()) auras->EyeOfMagtheridon = std::make_shared<EyeOfMagtheridonAura>(shared_from_this());
    if (std::find(trinketIds.begin(), trinketIds.end(), 32493) != trinketIds.end()) auras->AshtongueTalismanOfShadows = std::make_shared<AshtongueTalismanOfShadowsAura>(shared_from_this());
    if (std::find(trinketIds.begin(), trinketIds.end(), 31856) != trinketIds.end()) auras->DarkmoonCardCrusade = std::make_shared<DarkmoonCardCrusadeAura>(shared_from_this());
    if (std::find(trinketIds.begin(), trinketIds.end(), 28785) != trinketIds.end()) auras->TheLightningCapacitor = std::make_shared<TheLightningCapacitorAura>(shared_from_this());
    if (std::find(trinketIds.begin(), trinketIds.end(), 27683) != trinketIds.end()) auras->QuagmirransEye = std::make_shared<QuagmirransEyeAura>(shared_from_this());
    if (std::find(trinketIds.begin(), trinketIds.end(), 28418) != trinketIds.end()) auras->ShiffarsNexusHorn = std::make_shared<ShiffarsNexusHornAura>(shared_from_this());
    if (std::find(trinketIds.begin(), trinketIds.end(), 30626) != trinketIds.end()) auras->SextantOfUnstableCurrents = std::make_shared<SextantOfUnstableCurrentsAura>(shared_from_this());
    if (items->ring1 == 29305 || items->ring2 == 29305) auras->BandOfTheEternalSage = std::make_shared<BandOfTheEternalSageAura>(shared_from_this());
    if (items->ring1 == 21190 || items->ring2 == 21190) auras->WrathOfCenarius = std::make_shared<WrathOfCenariusAura>(shared_from_this());
    if (sets->t4 >= 2)
    {
        auras->Flameshadow = std::make_shared<FlameshadowAura>(shared_from_this());
        auras->Shadowflame = std::make_shared<ShadowflameAura>(shared_from_this());
    }
    if (sets->spellstrike >= 2) auras->Spellstrike = std::make_shared<SpellstrikeAura>(shared_from_this());
    if (sets->manaEtched >= 4) auras->ManaEtched4Set = std::make_shared<ManaEtched4SetAura>(shared_from_this());

    // Spells
    spells->LifeTap = std::make_shared<LifeTap>(shared_from_this());
    if (!settings->isSingleTarget)
    {
        spells->SeedOfCorruption = std::make_shared<SeedOfCorruption>(shared_from_this());
    }
    else
    {
        if (settings->hasShadowBolt || talents->nightfall > 0 || settings->simChoosingRotation) spells->ShadowBolt = std::make_shared<ShadowBolt>(shared_from_this());
        if (settings->hasIncinerate || settings->simChoosingRotation) spells->Incinerate = std::make_shared<Incinerate>(shared_from_this());
        if (settings->hasSearingPain || settings->simChoosingRotation) spells->SearingPain = std::make_shared<SearingPain>(shared_from_this());
        if (settings->hasDeathCoil || settings->simChoosingRotation) spells->DeathCoil = std::make_shared<DeathCoil>(shared_from_this());
        if (talents->conflagrate == 1 && (settings->hasConflagrate || settings->simChoosingRotation)) spells->Conflagrate = std::make_shared<Conflagrate>(shared_from_this());
        if (talents->shadowburn == 1 && (settings->hasShadowburn || settings->simChoosingRotation)) spells->Shadowburn = std::make_shared<Shadowburn>(shared_from_this());
        if (talents->shadowfury == 1 && (settings->hasShadowfury || settings->simChoosingRotation)) spells->Shadowfury = std::make_shared<Shadowfury>(shared_from_this());
        if (auras->Corruption != NULL) spells->Corruption = std::make_shared<Corruption>(shared_from_this(), nullptr, auras->Corruption);
        if (auras->UnstableAffliction != NULL) spells->UnstableAffliction = std::make_shared<UnstableAffliction>(shared_from_this(), nullptr, auras->UnstableAffliction);
        if (auras->SiphonLife != NULL) spells->SiphonLife = std::make_shared<SiphonLife>(shared_from_this(), nullptr, auras->SiphonLife);
        if (auras->Immolate != NULL) spells->Immolate = std::make_shared<Immolate>(shared_from_this(), nullptr, auras->Immolate);
        if (auras->CurseOfAgony != NULL || auras->CurseOfDoom != NULL) spells->CurseOfAgony = std::make_shared<CurseOfAgony>(shared_from_this(), nullptr, auras->CurseOfAgony);
        if (auras->CurseOfTheElements != NULL) spells->CurseOfTheElements = std::make_shared<CurseOfTheElements>(shared_from_this(), auras->CurseOfTheElements);
        if (auras->CurseOfRecklessness != NULL) spells->CurseOfRecklessness = std::make_shared<CurseOfRecklessness>(shared_from_this(), auras->CurseOfRecklessness);
        if (auras->CurseOfDoom != NULL) spells->CurseOfDoom = std::make_shared<CurseOfDoom>(shared_from_this(), nullptr, auras->CurseOfDoom);
        if (auras->AmplifyCurse != NULL) spells->AmplifyCurse = std::make_shared<AmplifyCurse>(shared_from_this(), auras->AmplifyCurse);
    }
    if (auras->ManaTideTotem != NULL) spells->ManaTideTotem = std::make_shared<ManaTideTotem>(shared_from_this(), auras->ManaTideTotem);
    if (auras->ChippedPowerCore != NULL) spells->ChippedPowerCore = std::make_shared<ChippedPowerCore>(shared_from_this(), auras->ChippedPowerCore);
    if (auras->CrackedPowerCore != NULL) spells->CrackedPowerCore = std::make_shared<CrackedPowerCore>(shared_from_this(), auras->CrackedPowerCore);
    if (selectedAuras->superManaPotion) spells->SuperManaPotion = std::make_shared<SuperManaPotion>(shared_from_this());
    if (selectedAuras->demonicRune) spells->DemonicRune = std::make_shared<DemonicRune>(shared_from_this());
    if (talents->darkPact == 1 && (settings->hasDarkPact || settings->simChoosingRotation)) spells->DarkPact = std::make_shared<DarkPact>(shared_from_this());
    if (auras->DestructionPotion != NULL) spells->DestructionPotion = std::make_shared<DestructionPotion>(shared_from_this(), auras->DestructionPotion);
    if (auras->FlameCap != NULL) spells->FlameCap = std::make_shared<FlameCap>(shared_from_this(), auras->FlameCap);
    if (auras->BloodFury != NULL) spells->BloodFury = std::make_shared<BloodFury>(shared_from_this(), auras->BloodFury);
    if (auras->DrumsOfBattle != NULL) spells->DrumsOfBattle = std::make_shared<DrumsOfBattle>(shared_from_this(), auras->DrumsOfBattle);
    else if (auras->DrumsOfWar != NULL) spells->DrumsOfWar = std::make_shared<DrumsOfWar>(shared_from_this(), auras->DrumsOfWar);
    else if (auras->DrumsOfRestoration != NULL) spells->DrumsOfRestoration = std::make_shared<DrumsOfRestoration>(shared_from_this(), auras->DrumsOfRestoration);
    if (auras->BladeOfWizardry != NULL) spells->BladeOfWizardry = std::make_shared<BladeOfWizardry>(shared_from_this(), auras->BladeOfWizardry);
    if (auras->ShatteredSunPendantOfAcumen != NULL) spells->ShatteredSunPendantOfAcumen = std::make_shared<ShatteredSunPendantOfAcumen>(shared_from_this(), auras->ShatteredSunPendantOfAcumen);
    if (auras->RobeOfTheElderScribes != NULL) spells->RobeOfTheElderScribes = std::make_shared<RobeOfTheElderScribes>(shared_from_this(), auras->RobeOfTheElderScribes);
    if (auras->MysticalSkyfireDiamond != NULL) spells->MysticalSkyfireDiamond = std::make_shared<MysticalSkyfireDiamond>(shared_from_this(), auras->MysticalSkyfireDiamond);
    if (settings->metaGemId == 25901) spells->InsightfulEarthstormDiamond = std::make_shared<InsightfulEarthstormDiamond>(shared_from_this());
    if (std::find(trinketIds.begin(), trinketIds.end(), 34470) != trinketIds.end()) spells->TimbalsFocusingCrystal = std::make_shared<TimbalsFocusingCrystal>(shared_from_this());
    if (std::find(trinketIds.begin(), trinketIds.end(), 27922) != trinketIds.end()) spells->MarkOfDefiance = std::make_shared<MarkOfDefiance>(shared_from_this());
    if (auras->TheLightningCapacitor != NULL) spells->TheLightningCapacitor = std::make_shared<TheLightningCapacitor>(shared_from_this(), auras->TheLightningCapacitor);
    if (auras->QuagmirransEye != NULL) spells->QuagmirransEye = std::make_shared<QuagmirransEye>(shared_from_this(), auras->QuagmirransEye);
    if (auras->ShiffarsNexusHorn != NULL) spells->ShiffarsNexusHorn = std::make_shared<ShiffarsNexusHorn>(shared_from_this(), auras->ShiffarsNexusHorn);
    if (auras->SextantOfUnstableCurrents != NULL) spells->SextantOfUnstableCurrents = std::make_shared<SextantOfUnstableCurrents>(shared_from_this(), auras->SextantOfUnstableCurrents);
    if (items->ring1 == 29305 || items->ring2 == 29305) spells->BandOfTheEternalSage = std::make_shared<BandOfTheEternalSage>(shared_from_this(), auras->BandOfTheEternalSage);
    if (auras->PowerInfusion != NULL)
    {
        for (int i = 0; i < settings->powerInfusionAmount; i++)
        {
            spells->PowerInfusion.push_back(std::make_shared<PowerInfusion>(shared_from_this(), auras->PowerInfusion));
        }
    }
    if (auras->Bloodlust != NULL)
    {
        for (int i = 0; i < settings->bloodlustAmount; i++)
        {
            spells->Bloodlust.push_back(std::make_shared<Bloodlust>(shared_from_this(), auras->Bloodlust));
        }
    }
    if (auras->Innervate != NULL)
    {
        for (int i = 0; i < settings->innervateAmount; i++)
        {
            spells->Innervate.push_back(std::make_shared<Innervate>(shared_from_this(), auras->Innervate));
        }
    }

    // Set the filler property
    if (settings->hasIncinerate)
    {
        filler = spells->Incinerate;
    }
    else if (settings->hasSearingPain)
    {
        filler = spells->SearingPain;
    }
    else
    {
        filler = spells->ShadowBolt;
    }

    // Set the curseSpell and curseAura properties
    if (spells->CurseOfTheElements != NULL)
    {
        curseSpell = spells->CurseOfTheElements;
        curseAura = auras->CurseOfTheElements;
    }
    else if (spells->CurseOfRecklessness != NULL)
    {
        curseSpell = spells->CurseOfRecklessness;
        curseAura = auras->CurseOfRecklessness;
    }
    else if (spells->CurseOfDoom != NULL)
    {
        curseSpell = spells->CurseOfDoom;
    }
    else if (spells->CurseOfAgony != NULL)
    {
        curseSpell = spells->CurseOfAgony;
    }
}

void Player::reset()
{
    castTimeRemaining = 0;
    gcdRemaining = 0;
    stats->mana = stats->maxMana;
    mp5Timer = 5;
    fiveSecondRuleTimer = 5;

    // Reset trinkets
    for (int i = 0; i < trinkets.size(); i++)
    {
        trinkets[i]->reset();
    }

    // Reset spells
    if (spells->LifeTap != NULL) spells->LifeTap->reset();
    if (spells->SeedOfCorruption != NULL) spells->SeedOfCorruption->reset();
    if (spells->ShadowBolt != NULL) spells->ShadowBolt->reset();
    if (spells->Incinerate != NULL) spells->Incinerate->reset();
    if (spells->SearingPain != NULL) spells->SearingPain->reset();
    if (spells->Corruption != NULL) spells->Corruption->reset();
    if (spells->UnstableAffliction != NULL) spells->UnstableAffliction->reset();
    if (spells->SiphonLife != NULL) spells->SiphonLife->reset();
    if (spells->Immolate != NULL) spells->Immolate->reset();
    if (spells->CurseOfAgony != NULL) spells->CurseOfAgony->reset();
    if (spells->CurseOfTheElements != NULL) spells->CurseOfTheElements->reset();
    if (spells->CurseOfRecklessness != NULL) spells->CurseOfRecklessness->reset();
    if (spells->CurseOfDoom != NULL) spells->CurseOfDoom->reset();
    if (spells->Conflagrate != NULL) spells->Conflagrate->reset();
    if (spells->Shadowburn != NULL) spells->Shadowburn->reset();
    if (spells->DeathCoil != NULL) spells->DeathCoil->reset();
    if (spells->Shadowfury != NULL) spells->Shadowfury->reset();
    if (spells->AmplifyCurse != NULL) spells->AmplifyCurse->reset();
    if (spells->DarkPact != NULL) spells->DarkPact->reset();
    if (spells->DestructionPotion != NULL) spells->DestructionPotion->reset();
    if (spells->SuperManaPotion != NULL) spells->SuperManaPotion->reset();
    if (spells->DemonicRune != NULL) spells->DemonicRune->reset();
    if (spells->FlameCap != NULL) spells->FlameCap->reset();
    if (spells->BloodFury != NULL) spells->BloodFury->reset();
    if (spells->DrumsOfBattle != NULL) spells->DrumsOfBattle->reset();
    if (spells->DrumsOfWar != NULL) spells->DrumsOfWar->reset();
    if (spells->DrumsOfRestoration != NULL) spells->DrumsOfRestoration->reset();
    if (spells->BladeOfWizardry != NULL) spells->BladeOfWizardry->reset();
    if (spells->ShatteredSunPendantOfAcumen != NULL) spells->ShatteredSunPendantOfAcumen->reset();
    if (spells->RobeOfTheElderScribes != NULL) spells->RobeOfTheElderScribes->reset();
    if (spells->MysticalSkyfireDiamond != NULL) spells->MysticalSkyfireDiamond->reset();
    if (spells->InsightfulEarthstormDiamond != NULL) spells->InsightfulEarthstormDiamond->reset();
    if (spells->TimbalsFocusingCrystal != NULL) spells->TimbalsFocusingCrystal->reset();
    if (spells->MarkOfDefiance != NULL) spells->MarkOfDefiance->reset();
    if (spells->TheLightningCapacitor != NULL) spells->TheLightningCapacitor->reset();
    if (spells->QuagmirransEye != NULL) spells->QuagmirransEye->reset();
    if (spells->ShiffarsNexusHorn != NULL) spells->ShiffarsNexusHorn->reset();
    if (spells->SextantOfUnstableCurrents != NULL) spells->SextantOfUnstableCurrents->reset();
    if (spells->BandOfTheEternalSage != NULL) spells->BandOfTheEternalSage->reset();
    if (spells->ChippedPowerCore != NULL) spells->ChippedPowerCore->reset();
    if (spells->CrackedPowerCore != NULL) spells->CrackedPowerCore->reset();
    if (spells->ManaTideTotem != NULL) spells->ManaTideTotem->reset();
    for (std::vector<std::shared_ptr<Spell>>::iterator it = spells->PowerInfusion.begin(); it != spells->PowerInfusion.end(); it++)
    {
        (*it)->reset();
    }
    for (std::vector<std::shared_ptr<Spell>>::iterator it = spells->Bloodlust.begin(); it != spells->Bloodlust.end(); it++)
    {
        (*it)->reset();
    }
    for (std::vector<std::shared_ptr<Spell>>::iterator it = spells->Innervate.begin(); it != spells->Innervate.end(); it++)
    {
        (*it)->reset();
    }
}

void Player::endAuras()
{
    for (int i = 0; i < trinkets.size(); i++)
    {
        if (trinkets[i]->active)
        {
            trinkets[i]->fade();
        }
    }
    if (auras->Corruption != NULL && auras->Corruption->active) auras->Corruption->fade();
    if (auras->UnstableAffliction != NULL && auras->UnstableAffliction->active) auras->UnstableAffliction->fade();
    if (auras->SiphonLife != NULL && auras->SiphonLife->active) auras->SiphonLife->fade();
    if (auras->Immolate != NULL && auras->Immolate->active) auras->Immolate->fade();
    if (auras->CurseOfAgony != NULL && auras->CurseOfAgony->active) auras->CurseOfAgony->fade();
    if (auras->CurseOfDoom != NULL && auras->CurseOfDoom->active) auras->CurseOfDoom->fade();
    if (auras->ImprovedShadowBolt != NULL && auras->ImprovedShadowBolt->active) auras->ImprovedShadowBolt->fade();
    if (auras->CurseOfTheElements != NULL && auras->CurseOfTheElements->active) auras->CurseOfTheElements->fade();
    if (auras->CurseOfRecklessness != NULL && auras->CurseOfRecklessness->active) auras->CurseOfRecklessness->fade();
    if (auras->ShadowTrance != NULL && auras->ShadowTrance->active) auras->ShadowTrance->fade();
    if (auras->AmplifyCurse != NULL && auras->AmplifyCurse->active) auras->AmplifyCurse->fade();
    if (auras->PowerInfusion != NULL && auras->PowerInfusion->active) auras->PowerInfusion->fade();
    if (auras->Innervate != NULL && auras->Innervate->active) auras->Innervate->fade();
    if (auras->BloodFury != NULL && auras->BloodFury->active) auras->BloodFury->fade();
    if (auras->DestructionPotion != NULL && auras->DestructionPotion->active) auras->DestructionPotion->fade();
    if (auras->FlameCap != NULL && auras->FlameCap->active) auras->FlameCap->fade();
    if (auras->Bloodlust != NULL && auras->Bloodlust->active) auras->Bloodlust->fade();
    if (auras->DrumsOfBattle != NULL && auras->DrumsOfBattle->active) auras->DrumsOfBattle->fade();
    if (auras->DrumsOfWar != NULL && auras->DrumsOfWar->active) auras->DrumsOfWar->fade();
    if (auras->DrumsOfRestoration != NULL && auras->DrumsOfRestoration->active) auras->DrumsOfRestoration->fade();
    if (auras->BandOfTheEternalSage != NULL && auras->BandOfTheEternalSage->active) auras->BandOfTheEternalSage->fade();
    if (auras->WrathOfCenarius != NULL && auras->WrathOfCenarius->active) auras->WrathOfCenarius->fade();
    if (auras->BladeOfWizardry != NULL && auras->BladeOfWizardry->active) auras->BladeOfWizardry->fade();
    if (auras->ShatteredSunPendantOfAcumen != NULL && auras->ShatteredSunPendantOfAcumen->active) auras->ShatteredSunPendantOfAcumen->fade();
    if (auras->RobeOfTheElderScribes != NULL && auras->RobeOfTheElderScribes->active) auras->RobeOfTheElderScribes->fade();
    if (auras->MysticalSkyfireDiamond != NULL && auras->MysticalSkyfireDiamond->active) auras->MysticalSkyfireDiamond->fade();
    if (auras->EyeOfMagtheridon != NULL && auras->EyeOfMagtheridon->active) auras->EyeOfMagtheridon->fade();
    if (auras->SextantOfUnstableCurrents != NULL && auras->SextantOfUnstableCurrents->active) auras->SextantOfUnstableCurrents->fade();
    if (auras->QuagmirransEye != NULL && auras->QuagmirransEye->active) auras->QuagmirransEye->fade();
    if (auras->ShiffarsNexusHorn != NULL && auras->ShiffarsNexusHorn->active) auras->ShiffarsNexusHorn->fade();
    if (auras->AshtongueTalismanOfShadows != NULL && auras->AshtongueTalismanOfShadows->active) auras->AshtongueTalismanOfShadows->fade();
    if (auras->DarkmoonCardCrusade != NULL && auras->DarkmoonCardCrusade->active) auras->DarkmoonCardCrusade->fade();
    if (auras->TheLightningCapacitor != NULL && auras->TheLightningCapacitor->active) auras->TheLightningCapacitor->fade();
    if (auras->Flameshadow != NULL && auras->Flameshadow->active) auras->Flameshadow->fade();
    if (auras->Shadowflame != NULL && auras->Shadowflame->active) auras->Shadowflame->fade();
    if (auras->Spellstrike != NULL && auras->Spellstrike->active) auras->Spellstrike->fade();
    if (auras->ManaEtched4Set != NULL && auras->ManaEtched4Set->active) auras->ManaEtched4Set->fade();
    if (auras->ChippedPowerCore != NULL && auras->ChippedPowerCore->active) auras->ChippedPowerCore->fade();
    if (auras->CrackedPowerCore != NULL && auras->CrackedPowerCore->active) auras->CrackedPowerCore->fade();
    if (auras->ManaTideTotem != NULL && auras->ManaTideTotem->active) auras->ManaTideTotem->fade();
}

double Player::getHastePercent()
{
    double hastePercent = stats->hastePercent;

    // If both Bloodlust and Power Infusion are active then remove the 20% PI bonus since they don't stack
    if (auras->Bloodlust != NULL && auras->PowerInfusion != NULL && auras->Bloodlust->active && auras->PowerInfusion->active)
    {
        hastePercent /= (1 + auras->PowerInfusion->stats->hastePercent / 100);
    }

    return hastePercent * (1 + stats->hasteRating / hasteRatingPerPercent / 100.0);
}

double Player::getGcdValue(const std::shared_ptr<Spell>& spell)
{
    if (spells->Shadowfury == NULL || spell != spells->Shadowfury)
    {
        return std::max(minimumGcdValue, round((gcdValue / getHastePercent()) * 10000) / 10000);
    }
    return 0;
}

double Player::getSpellPower(SpellSchool school)
{
    double spellPower = stats->spellPower + demonicKnowledgeSpellPower;
    if (sets->spellfire == 3)
    {
        spellPower += stats->intellect * stats->intellectModifier * 0.07;
    }
    if (school == SpellSchool::SHADOW)
    {
        spellPower += stats->shadowPower;
    }
    else if (school == SpellSchool::FIRE)
    {
        spellPower += stats->firePower;
    }
    return spellPower;
}

double Player::getCritChance(SpellType spellType)
{
    double critChance = stats->critChance + ((stats->intellect * stats->intellectModifier) * critPerInt);
    if (spellType != SpellType::DESTRUCTION)
    {
        critChance -= talents->devastation;
    }
    return critChance;
}

double Player::getHitChance(SpellType spellType)
{
    double hitChance = stats->hitChance + stats->extraHitChance;
    if (spellType == SpellType::AFFLICTION)
    {
        hitChance += talents->suppression * 2;
    }
    return std::min(99.0, hitChance);
}

bool Player::isCrit(SpellType spellType, double extraCrit)
{
    return getRand() <= ((getCritChance(spellType) + extraCrit) * critChanceMultiplier);
}

bool Player::isHit(SpellType spellType)
{
    double hit = getRand() <= (getHitChance(spellType) * critChanceMultiplier);
    if (!hit && auras->EyeOfMagtheridon != NULL)
    {
        auras->EyeOfMagtheridon->apply();
    }
    return hit;
}

int Player::getRand()
{
    return randomNum(gen);
}

// formula from https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/ && https://royalgiraffe.github.io/resist-guide
double Player::getBaseHitChance(int playerLevel, int enemyLevel)
{
    const int levelDifference = enemyLevel - playerLevel;
    if (levelDifference <= 2)
    {
        return std::min(99, 100 - levelDifference - 4);
    }
    else if (levelDifference == 3)
    {
        return 83;
    }
    else if (levelDifference >= 4)
    {
        return 83 - 11 * levelDifference;
    }
    return 0;
}

void Player::useCooldowns()
{
    // Only use PI if Bloodlust isn't selected or if Bloodlust isn't active since they don't stack
    if (!spells->PowerInfusion.empty() && !auras->PowerInfusion->active && (spells->Bloodlust.empty() || !auras->Bloodlust->active))
    {
        for (auto pi : spells->PowerInfusion)
        {
            if (pi->ready())
            {
                pi->startCast();
                break;
            }
        }
    }
    //todo don't use innervate until x% mana
    if (!spells->Innervate.empty() && !auras->Innervate->active)
    {
        for (auto innervate : spells->Innervate)
        {
            if (innervate->ready())
            {
                innervate->startCast();
                break;
            }
        }
    }
    if (spells->ChippedPowerCore != NULL && spells->ChippedPowerCore->ready())
    {
        spells->ChippedPowerCore->startCast();
    }
    else if (spells->CrackedPowerCore != NULL && spells->CrackedPowerCore->ready())
    {
        spells->CrackedPowerCore->startCast();
    }
    if (spells->DestructionPotion != NULL && spells->DestructionPotion->ready())
    {
        spells->DestructionPotion->startCast();
    }
    if (spells->FlameCap != NULL && spells->FlameCap->ready())
    {
        spells->FlameCap->startCast();
    }
    if (spells->BloodFury != NULL && spells->BloodFury->ready())
    {
        spells->BloodFury->startCast();
    }
    for (int i = 0; i < trinkets.size(); i++)
    {
        if (trinkets[i]->ready())
        {
            trinkets[i]->use();
            // Set the other on-use trinket (if another is equipped) on cooldown for the duration of the trinket just used if the trinkets share cooldown
            int otherTrinketSlot = i == 1 ? 0 : 1;
            if (trinkets.size() > otherTrinketSlot && trinkets[otherTrinketSlot] != NULL && trinkets[otherTrinketSlot]->sharesCooldown && trinkets[i]->sharesCooldown)
            {
                trinkets[otherTrinketSlot]->cooldownRemaining = std::max(trinkets[otherTrinketSlot]->cooldownRemaining, static_cast<double>(trinkets[i]->duration));
            }
        }
    }
}

void Player::castLifeTapOrDarkPact()
{
    if (spells->DarkPact != NULL && spells->DarkPact->ready())
    {
        spells->DarkPact->startCast();
    }
    else
    {
        spells->LifeTap->startCast();
    }
}

double Player::getPartialResistMultiplier(SpellSchool school)
{
    if (school == SpellSchool::SHADOW)
    {
        return 1.0 - ((75 * settings->enemyShadowResist) / (level * 5)) / 100.0;
    }
    else if (school == SpellSchool::FIRE)
    {
        return 1.0 - ((75 * settings->enemyFireResist) / (level * 5)) / 100.0;
    }

    return 1;
}

void Player::addIterationDamageAndMana(const std::string& spellName, int manaGain, int damage)
{
    if (combatLogBreakdown.count(spellName) == 0 || !settings->recordingCombatLogBreakdown)
    {
        return;
    }

    int iterationManaGain = combatLogBreakdown.at(spellName)->iterationManaGain;
    int iterationDamage = combatLogBreakdown.at(spellName)->iterationDamage;

    // Check for integer overflow
    if (iterationManaGain + manaGain < 0 || iterationDamage + damage < 0)
    {
        postIterationDamageAndMana(spellName);
    }

    combatLogBreakdown.at(spellName)->iterationManaGain += manaGain;
    combatLogBreakdown.at(spellName)->iterationDamage += damage;
}

void Player::postIterationDamageAndMana(const std::string& spellName)
{
    if (!settings->recordingCombatLogBreakdown)
    {
        return;
    }
    
    postCombatLogBreakdownVector(spellName.c_str(), combatLogBreakdown.at(spellName)->iterationManaGain, combatLogBreakdown.at(spellName)->iterationDamage);
    combatLogBreakdown.at(spellName)->iterationDamage = 0;
    combatLogBreakdown.at(spellName)->iterationManaGain = 0;
}

std::string Player::getCustomStat()
{
    if (settings->simmingStamina)       return "stamina";
    if (settings->simmingIntellect)     return "intellect";
    if (settings->simmingSpirit)        return "spirit";
    if (settings->simmingSpellPower)    return "spellPower";
    if (settings->simmingShadowPower)   return "shadowPower";
    if (settings->simmingFirePower)     return "firePower";
    if (settings->simmingCritRating)    return "critRating";
    if (settings->simmingHitRating)     return "hitRating";
    if (settings->simmingHasteRating)   return "hasteRating";
    if (settings->simmingMp5)           return "mp5";
    return "normal";
}

void Player::throwError(const std::string& error)
{
    sendCombatLogEntries();
    errorCallback(error.c_str());
    throw std::runtime_error(error);
}

bool Player::shouldWriteToCombatLog()
{
    return iteration == 10 && settings->equippedItemSimulation;
}

void Player::sendCombatLogEntries()
{
    for (const auto& value: combatLogEntries)
    {
        combatLogUpdate(value.c_str());
    }
}

void Player::combatLog(const std::string &entry)
{
    // Truncate the fightTime down to 4 decimal places
    combatLogEntries.push_back("|" + truncateTrailingZeros(std::to_string(fightTime), 4) + "| " + entry);
}