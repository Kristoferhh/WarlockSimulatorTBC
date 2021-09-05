#include "player.h"
#include "spell.h"
#include "common.h"
#include "emscripten.h"
#include "damageOverTime.h"

Player::Player(PlayerSettings* playerSettings)
    : selectedAuras(playerSettings->auras), talents(playerSettings->talents), sets(playerSettings->sets), stats(playerSettings->stats), items(playerSettings->items), settings(playerSettings)
{
    spells = new PlayerSpells();
    auras = new PlayerAuras();
    filler = "";
    curse = "";
    combatLogEntries = {};
    level = 70;
    castTimeRemaining = 0;
    totalManaRegenerated = 0;
    gcdRemaining = 0;
    gcdValue = 1.5;
    // Increases the cast time of spells by a very small amount to e.g. make it so that if Immolate has 1.5 seconds remaining, the sim can start precasting Immolate and it won't refresh before it expires.
    spellDelay = 0.0001;
    // Multiply hit and crit percent by this number to get rid of the decimals when calling random() since we need integers
    critChanceMultiplier = 1000;
    minimumGcdValue = 1; //todo change to 0.75 // TBCC is bugged cause Blizzard is a shit company so the minimum gcd value as of 2021/9/4 is 0.75s instead of 1s
    critMultiplier = 1.5;
    // I don't know if this formula only works for bosses or not, so for the moment I'm only using it for lvl >=73 targets.
    const double enemyBaseResistance = settings->enemyLevel >= 73 ? (6 * level * 5) / 75.0 : 0;
    settings->enemyShadowResist = std::max(static_cast<double>(settings->enemyShadowResist), enemyBaseResistance);
    settings->enemyFireResist = std::max(static_cast<double>(settings->enemyFireResist), enemyBaseResistance);

    stats->health = (stats->health + (stats->stamina * stats->staminaModifier) * healthPerStamina) * (1 + (0.01 * static_cast<double>(talents->felStamina)));
    stats->maxMana = (stats->mana + (stats->intellect * stats->intellectModifier) * manaPerInt) * (1 + (0.01 * static_cast<double>(talents->felIntellect)));
    stats->shadowModifier *= (1 + (0.02 * talents->shadowMastery));

    combatLogBreakdown.insert(std::make_pair("mp5", new CombatLogBreakdown("Mp5")));
    if (selectedAuras->judgementOfWisdom)
    {
        combatLogBreakdown.insert(std::make_pair("judgementOfWisdom", new CombatLogBreakdown("Judgement of Wisdom")));
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
        stats->spellPower += 100 * (1 + 0.1 * talents->demonicAegis);
    }
    // If using a custom isb uptime % then just add to the shadow modifier % (this assumes 5/5 ISB giving 20% shadow damage)
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
    // Add extra stamina from Blood Pact from Improved Imp
    if (selectedAuras->bloodPact)
    {
        double staminaGain = 70;
        if (settings->petIsImp)
        {
            staminaGain += 0.1 * settings->improvedImp;
        }
        stats->stamina += staminaGain;
    }
    // Add stamina from Demonic Embrace
    stats->stamina *= 1 + (0.03 * talents->demonicEmbrace);
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

    // Assign the filler spell
    if (settings->hasShadowBolt) filler = "shadowBolt";
    else if (settings->hasIncinerate) filler = "incinerate";
    else if (settings->hasSearingPain) filler = "searingPain";

    // Assign the curse (if selected)
    if (settings->hasCurseOfRecklessness) curse = "curseOfRecklessness";
    else if (settings->hasCurseOfTheElements) curse = "curseOfTheElements";
    else if (settings->hasCurseOfDoom) curse = "curseOfDoom";
    else if (settings->hasCurseOfAgony) curse = "curseOfAgony";

    // Records all information about damage done for each spell such as crit %, miss %, average damage per cast etc.
    //this.damageBreakdown = {}
    // Records all information about auras such as how often it was applied and the uptime %.
    //this.auraBreakdown = {}
    // Records all information about mana gain abilities like Life Tap, Mana Pots, and Demonic Runes
    //this.manaGainBreakdown = { mp5: { name: 'Mp5' } }
    //if (this.selectedAuras.judgementOfWisdom) this.manaGainBreakdown.judgementOfWisdom = { name: 'Judgement of Wisdom' }

    // Pet
    // Spell Power from the Demonic Knowledge talent
    demonicKnowledgeSpellPower = 0;
    /*if (settings.simSettings.sacrificePet == 'no' || settings.talents.demonicSacrifice == 0) {
      const selectedPet = settings.simSettings.petChoice
      if (selectedPet == PetName.IMP) this.pet = new Imp(this, settings)
      else if (selectedPet == PetName.VOIDWALKER) this.pet = new Voidwalker(this, settings)
      else if (selectedPet == PetName.SUCCUBUS) this.pet = new Succubus(this, settings)
      else if (selectedPet == PetName.FELHUNTER) this.pet = new Felhunter(this, settings)
      else if (selectedPet == PetName.FELGUARD && settings.talents.summonFelguard > 0) this.pet = new Felguard(this, settings)
    }*/

    combatLogEntries.push_back("---------------- Player stats ----------------");
    combatLogEntries.push_back("Health: " + truncateTrailingZeros(std::to_string(round(stats->health))));
    combatLogEntries.push_back("Mana: " + truncateTrailingZeros(std::to_string(round(stats->maxMana))));
    combatLogEntries.push_back("Stamina: " + truncateTrailingZeros(std::to_string(round(stats->stamina) * stats->staminaModifier)));
    combatLogEntries.push_back("Intellect: " + truncateTrailingZeros(std::to_string(round(stats->intellect) * stats->intellectModifier)));
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
    /*if (this.pet) {
      let petAp = this.pet.stats.ap * this.pet.stats.apModifier
      // Divide away the hidden 10% Felguard attack power bonus for the combat log to avoid confusion
      if (this.pet.pet == PetName.FELGUARD) {
        petAp /= 1.1
      } else if (this.pet.pet == PetName.SUCCUBUS) {
        petAp /= 1.05
      }
      this.combatlog.push('---------------- Pet stats ----------------')
      this.combatlog.push('Stamina: ' + Math.round(this.pet.stats.stamina * this.pet.stats.staminaModifier))
      this.combatlog.push('Intellect: ' + Math.round(this.pet.stats.intellect * this.pet.stats.intellectModifier))
      this.combatlog.push('Strength: ' + Math.round((this.pet.stats.baseStats.strength + this.pet.stats.buffs.strength) * this.pet.stats.strengthModifier))
      this.combatlog.push('Agility: ' + Math.round(this.pet.stats.agility * this.pet.stats.agilityModifier))
      this.combatlog.push('Spirit: ' + Math.round((this.pet.stats.baseStats.spirit + this.pet.stats.spirit) * this.pet.stats.spiritModifier))
      this.combatlog.push('Attack Power: ' + Math.round(petAp))
      this.combatlog.push('Spell Power: ' + Math.round(this.pet.stats.spellPower))
      this.combatlog.push('Mana: ' + Math.round(this.pet.stats.maxMana))
      this.combatlog.push('MP5: ' + Math.round(this.pet.stats.mp5))
      if (this.pet.pet !== PetName.IMP) {
        this.combatlog.push('Physical Hit Chance: ' + Math.round(this.pet.getMeleeHitChance() * 100) / 100 + '%')
        this.combatlog.push('Physical Crit Chance: ' + Math.round(this.pet.getMeleeCritChance() * 100) / 100 + '% (' + this.pet.critSuppression + '% Crit Suppression Applied)')
        this.combatlog.push('Glancing Blow Chance: ' + Math.round(this.pet.glancingBlowChance * 100) / 100 + '%')
      }
      if (this.pet.pet === PetName.IMP || this.pet.pet === PetName.SUCCUBUS) {
        this.combatlog.push('Spell Hit Chance: ' + Math.round(this.pet.getSpellHitChance() * 100) / 100 + '%')
        this.combatlog.push('Spell Crit Chance: ' + Math.round(this.pet.getSpellCritChance() * 100) / 100 + '%')
      }
      this.combatlog.push('Damage Modifier: ' + Math.round(this.pet.stats.damageModifier * 100) + '%')
    }*/
    combatLogEntries.push_back("---------------- Enemy stats ----------------");
    combatLogEntries.push_back("Level: " + std::to_string(settings->enemyLevel));
    combatLogEntries.push_back("Shadow Resistance: " + std::to_string(settings->enemyShadowResist));
    combatLogEntries.push_back("Fire Resistance: " + std::to_string(settings->enemyFireResist));
    /*if (this.pet && this.pet.pet != PetName.IMP) {
      this.combatlog.push('Dodge Chance: ' + this.pet.enemyDodgeChance + '%')
      this.combatlog.push('Armor: ' + this.enemy.armor)
      this.combatlog.push('Damage Reduction From Armor: ' + Math.round((1 - this.pet.armorMultiplier) * 10000) / 100 + '%')
    }*/
    combatLogEntries.push_back("---------------------------------------------");
}

void Player::initialize()
{
    // Trinkets
    std::vector<int> trinketIds {items->trinket1, items->trinket2};
    if (std::find(trinketIds.begin(), trinketIds.end(), 32483) != trinketIds.end()) trinkets.push_back(new SkullOfGuldan(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 34429) != trinketIds.end()) trinkets.push_back(new ShiftingNaaruSliver(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 33829) != trinketIds.end()) trinkets.push_back(new HexShrunkenHead(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 29370) != trinketIds.end()) trinkets.push_back(new IconOfTheSilverCrescent(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 29132) != trinketIds.end()) trinkets.push_back(new ScryersBloodgem(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 23046) != trinketIds.end()) trinkets.push_back(new RestrainedEssenceOfSapphiron(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 29179) != trinketIds.end()) trinkets.push_back(new XirisGift(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 25620) != trinketIds.end()) trinkets.push_back(new AncientCrystalTalisman(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 28223) != trinketIds.end()) trinkets.push_back(new ArcanistsStone(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 25936) != trinketIds.end()) trinkets.push_back(new TerokkarTabletOfVim(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 28040) != trinketIds.end()) trinkets.push_back(new VengeanceOfTheIllidari(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 24126) != trinketIds.end()) trinkets.push_back(new FigurineLivingRubySerpent(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 29376) != trinketIds.end()) trinkets.push_back(new EssenceOfTheMartyr(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 30340) != trinketIds.end()) trinkets.push_back(new StarkillersBauble(this));

    // Auras
    if (settings->isSingleTarget)
    {
        if (talents->improvedShadowBolt > 0) auras->ImprovedShadowBolt = new ImprovedShadowBoltAura(this);
        if (settings->hasCorruption || settings->simChoosingRotation) auras->Corruption = new CorruptionDot(this);
        if (talents->unstableAffliction == 1 && (settings->hasUnstableAffliction || settings->simChoosingRotation)) auras->UnstableAffliction = new UnstableAfflictionDot(this);
        if (talents->siphonLife == 1 && (settings->hasSiphonLife || settings->simChoosingRotation)) auras->SiphonLife = new SiphonLifeDot(this);
        if (settings->hasImmolate || settings->simChoosingRotation) auras->Immolate = new ImmolateDot(this);
        if (settings->hasCurseOfAgony || settings->hasCurseOfDoom) auras->CurseOfAgony = new CurseOfAgonyDot(this);
        if (settings->hasCurseOfTheElements) auras->CurseOfTheElements = new CurseOfTheElementsAura(this);
        if (settings->hasCurseOfRecklessness) auras->CurseOfRecklessness = new CurseOfRecklessnessAura(this);
        if (settings->hasCurseOfDoom) auras->CurseOfDoom = new CurseOfDoomDot(this);
        if (talents->nightfall > 0) auras->ShadowTrance = new ShadowTranceAura(this);
        if (talents->amplifyCurse == 1 && (settings->hasAmplifyCurse || settings->simChoosingRotation)) auras->AmplifyCurse = new AmplifyCurseAura(this);
    }
    if (selectedAuras->powerInfusion) auras->PowerInfusion = new PowerInfusionAura(this);
    if (selectedAuras->innervate) auras->Innervate = new InnervateAura(this);
    if (selectedAuras->bloodlust) auras->Bloodlust = new BloodlustAura(this);
    if (selectedAuras->destructionPotion) auras->DestructionPotion = new DestructionPotionAura(this);
    if (selectedAuras->flameCap) auras->FlameCap = new FlameCapAura(this);
    if (settings->isOrc) auras->BloodFury = new BloodFuryAura(this);
    if (selectedAuras->drumsOfBattle) auras->DrumsOfBattle = new DrumsOfBattleAura(this);
    else if (selectedAuras->drumsOfWar) auras->DrumsOfWar = new DrumsOfWarAura(this);
    else if (selectedAuras->drumsOfRestoration) auras->DrumsOfRestoration = new DrumsOfRestorationAura(this);
    if (items->mainHand == 31336) auras->BladeOfWizardry = new BladeOfWizardryAura(this);
    if (items->neck == 34678) auras->ShatteredSunPendantOfAcumen = new ShatteredSunPendantOfAcumenAura(this);
    if (items->chest == 28602) auras->RobeOfTheElderScribes = new RobeOfTheElderScribesAura(this);
    if (settings->metaGemId == 25893) auras->MysticalSkyfireDiamond = new MysticalSkyfireDiamondAura(this);
    if (std::find(trinketIds.begin(), trinketIds.end(), 28789) != trinketIds.end()) auras->EyeOfMagtheridon = new EyeOfMagtheridonAura(this);
    if (std::find(trinketIds.begin(), trinketIds.end(), 32493) != trinketIds.end()) auras->AshtongueTalismanOfShadows = new AshtongueTalismanOfShadowsAura(this);
    if (std::find(trinketIds.begin(), trinketIds.end(), 31856) != trinketIds.end()) auras->DarkmoonCardCrusade = new DarkmoonCardCrusadeAura(this);
    if (std::find(trinketIds.begin(), trinketIds.end(), 28785) != trinketIds.end()) auras->TheLightningCapacitor = new TheLightningCapacitorAura(this);
    if (std::find(trinketIds.begin(), trinketIds.end(), 27683) != trinketIds.end()) auras->QuagmirransEye = new QuagmirransEyeAura(this);
    if (std::find(trinketIds.begin(), trinketIds.end(), 28418) != trinketIds.end()) auras->ShiffarsNexusHorn = new ShiffarsNexusHornAura(this);
    if (std::find(trinketIds.begin(), trinketIds.end(), 30626) != trinketIds.end()) auras->SextantOfUnstableCurrents = new SextantOfUnstableCurrentsAura(this);
    if (items->ring1 == 29305 || items->ring2 == 29305) auras->BandOfTheEternalSage = new BandOfTheEternalSageAura(this);
    if (items->ring1 == 21190 || items->ring2 == 21190) auras->WrathOfCenarius = new WrathOfCenariusAura(this);
    if (sets->t4 >= 2)
    {
        auras->Flameshadow = new FlameshadowAura(this);
        auras->Shadowflame = new ShadowflameAura(this);
    }
    if (sets->spellstrike >= 2) auras->Spellstrike = new SpellstrikeAura(this);
    if (sets->manaEtched >= 4) auras->ManaEtched4Set = new ManaEtched4SetAura(this);

    // Spells
    spells->LifeTap = new LifeTap(this);
    if (!settings->isSingleTarget)
    {
        spells->SeedOfCorruption = new SeedOfCorruption(this);
    }
    else
    {
        if (settings->hasShadowBolt || talents->nightfall > 0 || settings->simChoosingRotation) spells->ShadowBolt = new ShadowBolt(this);
        if (settings->hasIncinerate || settings->simChoosingRotation) spells->Incinerate = new Incinerate(this);
        if (settings->hasSearingPain || settings->simChoosingRotation) spells->SearingPain = new SearingPain(this);
        if (settings->hasDeathCoil || settings->simChoosingRotation) spells->DeathCoil = new DeathCoil(this);
        if (talents->conflagrate == 1 && (settings->hasConflagrate || settings->simChoosingRotation)) spells->Conflagrate = new Conflagrate(this);
        if (talents->shadowburn == 1 && (settings->hasShadowburn || settings->simChoosingRotation)) spells->Shadowburn = new Shadowburn(this);
        if (talents->shadowfury == 1 && (settings->hasShadowfury || settings->simChoosingRotation)) spells->Shadowfury = new Shadowfury(this);
        if (auras->Corruption != NULL) spells->Corruption = new Corruption(this, nullptr, auras->Corruption);
        if (auras->UnstableAffliction != NULL) spells->UnstableAffliction = new UnstableAffliction(this, nullptr, auras->UnstableAffliction);
        if (auras->SiphonLife != NULL) spells->SiphonLife = new SiphonLife(this, nullptr, auras->UnstableAffliction);
        if (auras->Immolate != NULL) spells->Immolate = new Immolate(this, nullptr, auras->Immolate);
        if (auras->CurseOfAgony != NULL || auras->CurseOfDoom != NULL) spells->CurseOfAgony = new CurseOfAgony(this, nullptr, auras->CurseOfAgony);
        if (auras->CurseOfTheElements != NULL) spells->CurseOfTheElements = new CurseOfTheElements(this, auras->CurseOfTheElements);
        if (auras->CurseOfRecklessness != NULL) spells->CurseOfRecklessness = new CurseOfRecklessness(this, auras->CurseOfRecklessness);
        if (auras->CurseOfDoom != NULL) spells->CurseOfDoom = new CurseOfDoom(this, nullptr, auras->CurseOfDoom);
        if (auras->AmplifyCurse != NULL) spells->AmplifyCurse = new AmplifyCurse(this, auras->AmplifyCurse);
    }
    if (selectedAuras->superManaPotion) spells->SuperManaPotion = new SuperManaPotion(this);
    if (selectedAuras->demonicRune) spells->DemonicRune = new DemonicRune(this);
    if (talents->darkPact == 1 && (settings->hasDarkPact || settings->simChoosingRotation)) spells->DarkPact = new DarkPact(this);
    if (auras->DestructionPotion != NULL) spells->DestructionPotion = new DestructionPotion(this, auras->DestructionPotion);
    if (auras->FlameCap != NULL) spells->FlameCap = new FlameCap(this, auras->FlameCap);
    if (auras->BloodFury != NULL) spells->BloodFury = new BloodFury(this, auras->BloodFury);
    if (auras->DrumsOfBattle != NULL) spells->DrumsOfBattle = new DrumsOfBattle(this, auras->DrumsOfBattle);
    else if (auras->DrumsOfWar != NULL) spells->DrumsOfWar = new DrumsOfWar(this, auras->DrumsOfWar);
    else if (auras->DrumsOfRestoration != NULL) spells->DrumsOfRestoration = new DrumsOfRestoration(this, auras->DrumsOfRestoration);
    if (auras->BladeOfWizardry != NULL) spells->BladeOfWizardry = new BladeOfWizardry(this, auras->BladeOfWizardry);
    if (auras->ShatteredSunPendantOfAcumen != NULL) spells->ShatteredSunPendantOfAcumen = new ShatteredSunPendantOfAcumen(this, auras->ShatteredSunPendantOfAcumen);
    if (auras->RobeOfTheElderScribes != NULL) spells->RobeOfTheElderScribes = new RobeOfTheElderScribes(this, auras->RobeOfTheElderScribes);
    if (auras->MysticalSkyfireDiamond != NULL) spells->MysticalSkyfireDiamond = new MysticalSkyfireDiamond(this, auras->MysticalSkyfireDiamond);
    if (settings->metaGemId == 25901) spells->InsightfulEarthstormDiamond = new InsightfulEarthstormDiamond(this);
    if (std::find(trinketIds.begin(), trinketIds.end(), 34470) != trinketIds.end()) spells->TimbalsFocusingCrystal = new TimbalsFocusingCrystal(this);
    if (std::find(trinketIds.begin(), trinketIds.end(), 27922) != trinketIds.end()) spells->MarkOfDefiance = new MarkOfDefiance(this);
    if (auras->TheLightningCapacitor != NULL) spells->TheLightningCapacitor = new TheLightningCapacitor(this, auras->TheLightningCapacitor);
    if (auras->QuagmirransEye != NULL) spells->QuagmirransEye = new QuagmirransEye(this, auras->QuagmirransEye);
    if (auras->ShiffarsNexusHorn != NULL) spells->ShiffarsNexusHorn = new ShiffarsNexusHorn(this, auras->ShiffarsNexusHorn);
    if (auras->SextantOfUnstableCurrents != NULL) spells->SextantOfUnstableCurrents = new SextantOfUnstableCurrents(this, auras->SextantOfUnstableCurrents);
    if (items->ring1 == 29305 || items->ring2 == 29305) spells->BandOfTheEternalSage = new BandOfTheEternalSage(this, auras->BandOfTheEternalSage);
    if (auras->PowerInfusion != NULL)
    {
        for (int i = 0; i < settings->powerInfusionAmount; i++)
        {
            spells->PowerInfusion.push_back(new PowerInfusion(this, auras->PowerInfusion));
        }
    }
    if (auras->Bloodlust != NULL)
    {
        for (int i = 0; i < settings->bloodlustAmount; i++)
        {
            spells->Bloodlust.push_back(new Bloodlust(this, auras->Bloodlust));
        }
    }
    if (auras->Innervate != NULL)
    {
        for (int i = 0; i < settings->innervateAmount; i++)
        {
            spells->Innervate.push_back(new Innervate(this, auras->Innervate));
        }
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
    spells->LifeTap->reset();
    spells->SeedOfCorruption->reset();
    spells->ShadowBolt->reset();
    spells->Incinerate->reset();
    spells->SearingPain->reset();
    spells->Corruption->reset();
    spells->UnstableAffliction->reset();
    spells->SiphonLife->reset();
    spells->Immolate->reset();
    spells->CurseOfAgony->reset();
    spells->CurseOfTheElements->reset();
    spells->CurseOfRecklessness->reset();
    spells->CurseOfDoom->reset();
    spells->Conflagrate->reset();
    spells->Shadowburn->reset();
    spells->DeathCoil->reset();
    spells->Shadowfury->reset();
    spells->AmplifyCurse->reset();
    spells->DarkPact->reset();
    spells->DestructionPotion->reset();
    spells->SuperManaPotion->reset();
    spells->DemonicRune->reset();
    spells->FlameCap->reset();
    spells->BloodFury->reset();
    spells->DrumsOfBattle->reset();
    spells->DrumsOfWar->reset();
    spells->DrumsOfRestoration->reset();
    spells->BladeOfWizardry->reset();
    spells->ShatteredSunPendantOfAcumen->reset();
    spells->RobeOfTheElderScribes->reset();
    spells->MysticalSkyfireDiamond->reset();
    spells->InsightfulEarthstormDiamond->reset();
    spells->TimbalsFocusingCrystal->reset();
    spells->MarkOfDefiance->reset();
    spells->TheLightningCapacitor->reset();
    spells->QuagmirransEye->reset();
    spells->ShiffarsNexusHorn->reset();
    spells->SextantOfUnstableCurrents->reset();
    spells->BandOfTheEternalSage->reset();
    for (std::vector<Spell*>::iterator it = spells->PowerInfusion.begin(); it != spells->PowerInfusion.end(); it++)
    {
        (*it)->reset();
    }
    for (std::vector<Spell*>::iterator it = spells->Bloodlust.begin(); it != spells->Bloodlust.end(); it++)
    {
        (*it)->reset();
    }
    for (std::vector<Spell*>::iterator it = spells->Innervate.begin(); it != spells->Innervate.end(); it++)
    {
        (*it)->reset();
    }
}

double Player::getHastePercent()
{
    return stats->hastePercent + (stats->hasteRating / hasteRatingPerPercent / 100.0);
}

double Player::getGcdValue(std::string varName)
{
    if (varName != "shadowfury" || spells->Shadowfury == NULL)
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
    return (random(1, (100 * critChanceMultiplier)) <= ((getCritChance(spellType) + extraCrit) * critChanceMultiplier));
}

bool Player::isHit(SpellType spellType)
{
    double hit = (random(1, 100 * critChanceMultiplier) <= getHitChance(spellType) * critChanceMultiplier);
    /*if (!hit && items.count(28789) == 1)
    {
        auras->eyeOfMagtheridon->apply();
    }*/
    return hit;
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

bool Player::areAnyCooldownsReady()
{
    if (!spells->Bloodlust.empty() && !auras->Bloodlust->active)
    {
        for (int i = 0; i < settings->bloodlustAmount; i++)
        {
            if (spells->Bloodlust[i]->ready())
            {
                return true;
            }
        }
    }
    if (!spells->PowerInfusion.empty() && !auras->PowerInfusion->active)
    {
        for (int i = 0; i < settings->powerInfusionAmount; i++)
        {
            if (spells->PowerInfusion[i]->ready())
            {
                return true;
            }
        }
    }
    if (!spells->Innervate.empty() && !auras->Innervate->active)
    {
        for (int i = 0; i < settings->innervateAmount; i++)
        {
            if (spells->Innervate[i]->ready())
            {
                return true;
            }
        }
    }
    if (spells->DestructionPotion != NULL && spells->DestructionPotion->ready())
    {
        return true;
    }
    if (spells->BloodFury != NULL && spells->BloodFury->ready())
    {
        return true;
    }
    for (auto trinketPtr : trinkets)
    {
        if (trinketPtr->ready())
        {
            return true;
        }
    }

    return false;
}

void Player::useCooldowns()
{
    if (!spells->Bloodlust.empty() && !auras->Bloodlust->active)
    {
        for (int i = 0; i < settings->bloodlustAmount; i++)
        {
            if (spells->Bloodlust[i]->ready())
            {
                spells->Bloodlust[i]->startCast();
                break;
            }
        }
    }
    if (!spells->PowerInfusion.empty() && !auras->PowerInfusion->active)
    {
        for (int i = 0; i < settings->powerInfusionAmount; i++)
        {
            if (spells->PowerInfusion[i]->ready())
            {
                spells->PowerInfusion[i]->startCast();
                break;
            }
        }
    }
    //todo don't use innervate until x% mana
    if (!spells->Innervate.empty() && !auras->Innervate->active)
    {
        for (int i = 0; i < settings->innervateAmount; i++)
        {
            if (spells->Innervate[i]->ready())
            {
                spells->Innervate[i]->startCast();
                break;
            }
        }
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
                trinkets[otherTrinketSlot]->cooldownRemaining = std::max(trinkets[otherTrinketSlot]->cooldownRemaining, trinkets[i]->duration);
            }
        }
    }
}

Aura* Player::getCurseAura()
{
    if (auras->CurseOfTheElements != NULL)
    {
        return auras->CurseOfTheElements;
    }
    else if (auras->CurseOfRecklessness != NULL)
    {
        return auras->CurseOfRecklessness;
    }
    else
    {
        return nullptr;
    }
}

Spell* Player::getCurseSpell()
{
    if (spells->CurseOfDoom != NULL)
    {
        return spells->CurseOfDoom;
    }
    else if (spells->CurseOfTheElements != NULL)
    {
        return spells->CurseOfTheElements;
    }
    else if (spells->CurseOfRecklessness != NULL)
    {
        return spells->CurseOfRecklessness;
    }
    else if (spells->CurseOfAgony != NULL)
    {
        return spells->CurseOfAgony;
    }
    else
    {
        return nullptr;
    }
}

Spell* Player::getFiller()
{
    if (spells->ShadowBolt != NULL) return spells->ShadowBolt;
    else if (spells->Incinerate != NULL) return spells->Incinerate;
    else if (spells->SearingPain != NULL) return spells->SearingPain;
    else return nullptr;
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

bool Player::shouldWriteToCombatLog()
{
    return iteration == 0;
}

void Player::combatLog(const std::string &entry)
{
    // Truncate the fightTime down to 4 decimal places
    combatLogEntries.push_back("|" + std::to_string(fightTime).substr(0, std::to_string(fightTime).find(".") + 5) + "| " + entry );
}