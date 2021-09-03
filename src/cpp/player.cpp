#include "player.h"
#include "spell.h"
#include "common.h"
#include "emscripten.h"
#include "damageOverTime.h"

Player::Player(PlayerSettings* playerSettings)
    : selectedAuras(playerSettings->auras), talents(playerSettings->talents), sets(playerSettings->sets), stats(playerSettings->stats), items(playerSettings->items), settings(playerSettings)
{
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
    minimumGcdValue = 1;
    critMultiplier = 1.5;
    // I don't know if this formula only works for bosses or not, so for the moment I'm only using it for lvl >=73 targets.
    const double enemyBaseResistance = settings->enemyLevel >= 73 ? (6 * level * 5) / 75.0 : 0;
    settings->enemyShadowResist = std::max(static_cast<double>(settings->enemyShadowResist), enemyBaseResistance);
    settings->enemyFireResist = std::max(static_cast<double>(settings->enemyFireResist), enemyBaseResistance);

    stats->health = (stats->health + (stats->stamina * stats->staminaModifier) * healthPerStamina) * (1 + (0.01 * static_cast<double>(talents->felStamina)));
    stats->maxMana = (stats->mana + (stats->intellect * stats->intellectModifier) * manaPerInt) * (1 + (0.01 * static_cast<double>(talents->felIntellect)));
    stats->shadowModifier *= (1 + (0.02 * talents->shadowMastery));

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
        stats->spellPower += 100 * (1 + 0.01 * talents->demonicAegis);
    }
    // If using a custom isb uptime % then just add to the shadow modifier % (this assumes 5/5 ISB giving 20% shadow damage)
    if (settings->usingCustomIsbUptime)
    {
        stats->shadowModifier *= (1 + 0.02 * (static_cast<double>(settings->customIsbUptimeValue) / 100.0));
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
    combatLogEntries.push_back("Health: " + std::to_string(round(stats->health)));
    combatLogEntries.push_back("Mana: " + std::to_string(round(stats->maxMana)));
    combatLogEntries.push_back("Stamina: " + std::to_string(round(stats->stamina * stats->staminaModifier)));
    combatLogEntries.push_back("Intellect: " + std::to_string(round(stats->intellect * stats->intellectModifier)));
    combatLogEntries.push_back("Spell Power: " + std::to_string(round(getSpellPower())));
    combatLogEntries.push_back("Shadow Power: " + std::to_string(stats->shadowPower));
    combatLogEntries.push_back("Fire Power: " + std::to_string(stats->firePower));
    combatLogEntries.push_back("Crit Chance: " + std::to_string(round(getCritChance(SpellType::DESTRUCTION) * 100) / 100) + "%");
    combatLogEntries.push_back("Hit Chance: " + std::to_string(std::min(static_cast<double>(16), round((stats->extraHitChance) * 100) / 100)) + "%");
    combatLogEntries.push_back("Haste: " + std::to_string(round((stats->hasteRating / hasteRatingPerPercent) * 100) / 100) + "%");
    combatLogEntries.push_back("Shadow Modifier: " + std::to_string((stats->shadowModifier * 100) / 100) + "%");
    combatLogEntries.push_back("Fire Modifier: " + std::to_string(round(stats->fireModifier * 100) / 100) + "%");
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

    // Spells
    spells.push_back(new LifeTap(this));
    if (!settings->isSingleTarget)
    {
        spells.push_back(new SeedOfCorruption(this));
    }
    else
    {
        if (settings->hasShadowBolt || talents->nightfall > 0 || settings->simChoosingRotation) spells.push_back(new ShadowBolt(this));
        if (settings->hasIncinerate || settings->simChoosingRotation) spells.push_back(new Incinerate(this));
        if (settings->hasSearingPain || settings->simChoosingRotation) spells.push_back(new SearingPain(this));
        if (settings->hasCorruption || settings->simChoosingRotation) spells.push_back(new Corruption(this));
        if (talents->unstableAffliction == 1 && (settings->hasUnstableAffliction || settings->simChoosingRotation)) spells.push_back(new UnstableAffliction(this));
        if (talents->siphonLife == 1 && (settings->hasSiphonLife || settings->simChoosingRotation)) spells.push_back(new SiphonLife(this));
        if (settings->hasImmolate || settings->simChoosingRotation) spells.push_back(new Immolate(this));
        if (settings->hasCurseOfAgony || settings->hasCurseOfDoom) spells.push_back(new CurseOfAgony(this));
        if (settings->hasCurseOfTheElements) spells.push_back(new CurseOfTheElements(this));
        if (settings->hasCurseOfRecklessness) spells.push_back(new CurseOfRecklessness(this));
        if (settings->hasCurseOfDoom) spells.push_back(new CurseOfDoom(this));
        if (talents->conflagrate == 1 && (settings->hasConflagrate || settings->simChoosingRotation)) spells.push_back(new Conflagrate(this));
        if (talents->shadowburn == 1 && (settings->hasShadowburn || settings->simChoosingRotation)) spells.push_back(new Shadowburn(this));
        if (settings->hasDeathCoil || settings->simChoosingRotation) spells.push_back(new DeathCoil(this));
        if (talents->shadowfury == 1 && (settings->hasShadowfury || settings->simChoosingRotation)) spells.push_back(new Shadowfury(this));
        if (talents->amplifyCurse == 1 && (settings->hasAmplifyCurse || settings->simChoosingRotation)) spells.push_back(new AmplifyCurse(this));
    }
    if (talents->darkPact == 1 && (settings->hasDarkPact || settings->simChoosingRotation)) spells.push_back(new DarkPact(this));
    if (selectedAuras->destructionPotion) spells.push_back(new DestructionPotion(this));
    if (selectedAuras->superManaPotion) spells.push_back(new SuperManaPotion(this));
    if (selectedAuras->demonicRune) spells.push_back(new DemonicRune(this));
    if (selectedAuras->flameCap) spells.push_back(new FlameCap(this));
    if (settings->isOrc) spells.push_back(new BloodFury(this));
    if (selectedAuras->drumsOfBattle) spells.push_back(new DrumsOfBattle(this));
    else if (selectedAuras->drumsOfWar) spells.push_back(new DrumsOfWar(this));
    else if (selectedAuras->drumsOfRestoration) spells.push_back(new DrumsOfRestoration(this));
    if (items->mainHand == 31336) spells.push_back(new BladeOfWizardry(this));
    if (items->neck == 34678) spells.push_back(new ShatteredSunPendantOfAcumen(this));
    if (items->chest == 28602) spells.push_back(new RobeOfTheElderScribes(this));
    if (settings->metaGemId == 25893) spells.push_back(new MysticalSkyfireDiamond(this));
    if (settings->metaGemId == 25901) spells.push_back(new InsightfulEarthstormDiamond(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 34470) != trinketIds.end()) spells.push_back(new TimbalsFocusingCrystal(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 27922) != trinketIds.end()) spells.push_back(new MarkOfDefiance(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 28785) != trinketIds.end()) spells.push_back(new TheLightningCapacitor(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 27683) != trinketIds.end()) spells.push_back(new QuagmirransEye(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 28418) != trinketIds.end()) spells.push_back(new ShiffarsNexusHorn(this));
    if (std::find(trinketIds.begin(), trinketIds.end(), 30626) != trinketIds.end()) spells.push_back(new SextantOfUnstableCurrents(this));
    if (items->ring1 == 29305 || items->ring2 == 29305) spells.push_back(new BandOfTheEternalSage(this));
    if (selectedAuras->powerInfusion)
    {
        spellVectors.push_back(std::vector<Spell*>(settings->powerInfusionAmount, new PowerInfusion(this)));
    }
    if (selectedAuras->bloodlust)
    {
        spellVectors.push_back(std::vector<Spell*>(settings->bloodlustAmount, new Bloodlust(this)));
    }
    if (selectedAuras->innervate)
    {
        spellVectors.push_back(std::vector<Spell*>(settings->innervateAmount, new Innervate(this)));
    }

    // Auras
    if (talents->improvedShadowBolt > 0)
    {
        if (settings->isSingleTarget)
        {
            if (talents->improvedShadowBolt > 0) auras.push_back(new ImprovedShadowBolt(this));
            if (getSpell("corruption") != nullptr) dots.push_back(new CorruptionDot(this));
            if (getSpell("unstableAffliction") != nullptr) dots.push_back(new UnstableAfflictionDot(this));
            if (getSpell("siphonLife") != nullptr) dots.push_back(new SiphonLifeDot(this));
            if (getSpell("immolate") != nullptr) dots.push_back(new ImmolateDot(this));
            if (getSpell("curseOfAgony") != nullptr) dots.push_back(new CurseOfAgonyDot(this));
            if (settings->hasCurseOfTheElements)  auras.push_back(new CurseOfTheElementsAura(this));
            if (settings->hasCurseOfRecklessness) auras.push_back(new CurseOfRecklessnessAura(this));
            if (settings->hasCurseOfDoom) dots.push_back(new CurseOfDoomDot(this));
            if (talents->nightfall > 0) auras.push_back(new ShadowTranceAura(this));
            if (settings->hasCurseOfDoom) auras.push_back(new AmplifyCurseAura(this));
        }
        if (selectedAuras->powerInfusion) auras.push_back(new PowerInfusionAura(this));
        if (selectedAuras->innervate) auras.push_back(new InnervateAura(this));
        if (settings->isOrc) auras.push_back(new BloodFuryAura(this));
        if (selectedAuras->destructionPotion) auras.push_back(new DestructionPotionAura(this));
        if (selectedAuras->flameCap) auras.push_back(new FlameCapAura(this));
        if (selectedAuras->bloodlust) auras.push_back(new BloodlustAura(this));
        if (selectedAuras->drumsOfBattle) auras.push_back(new DrumsOfBattleAura(this));
        else if (selectedAuras->drumsOfWar) auras.push_back(new DrumsOfWarAura(this));
        else if (selectedAuras->drumsOfRestoration) auras.push_back(new DrumsOfRestorationAura(this));
        if (items->ring1 == 29305 || items->ring2 == 29305) auras.push_back(new BandOfTheEternalSageAura(this));
        if (items->ring1 == 21190 || items->ring2 == 21190) auras.push_back(new WrathOfCenariusAura(this));
        if (items->mainHand == 31336) auras.push_back(new BladeOfWizardryAura(this));
        if (items->neck == 34678 && settings->isAldor) auras.push_back(new ShatteredSunpendantOfAcumenAura(this));
        if (items->chest == 28602) auras.push_back(new RobeOfTheElderScribesAura(this));
        if (settings->metaGemId == 25893) auras.push_back(new MysticalSkyfireDiamondAura(this));
        if (std::find(trinketIds.begin(), trinketIds.end(), 28789) != trinketIds.end()) auras.push_back(new EyeOfMagtheridonAura(this));
        if (std::find(trinketIds.begin(), trinketIds.end(), 30626) != trinketIds.end()) auras.push_back(new SextantOfUnstableCurrentsAura(this));
        if (std::find(trinketIds.begin(), trinketIds.end(), 27683) != trinketIds.end()) auras.push_back(new QuagmirransEyeAura(this));
        if (std::find(trinketIds.begin(), trinketIds.end(), 28418) != trinketIds.end()) auras.push_back(new ShiffarsNexusHornAura(this));
        if (std::find(trinketIds.begin(), trinketIds.end(), 32493) != trinketIds.end()) auras.push_back(new AshtongueTalismanOfShadowsAura(this));
        if (std::find(trinketIds.begin(), trinketIds.end(), 31856) != trinketIds.end()) auras.push_back(new DarkmoonCardCrusadeAura(this));
        if (std::find(trinketIds.begin(), trinketIds.end(), 28785) != trinketIds.end()) auras.push_back(new TheLightningCapacitorAura(this));
        if (sets->t4 >= 2)
        {
            auras.push_back(new Flameshadow(this));
            auras.push_back(new Shadowflame(this));
        }
        if (sets->spellstrike >= 2) auras.push_back(new SpellstrikeAura(this));
        if (sets->manaEtched >= 4) auras.push_back(new ManaEtched4SetAura(this));
    }
}

void Player::reset()
{
    castTimeRemaining = 0;
    gcdRemaining = 0;
    stats->mana = stats->maxMana;
    mp5Timer = 5;
    fiveSecondRuleTimer = 5;

    // Reset spells
    for (std::vector<Spell*>::iterator it = spells.begin(); it != spells.end(); it++)
    {
        (*it)->reset();
    }
    for (std::vector<std::vector<Spell*>>::iterator it = spellVectors.begin(); it != spellVectors.end(); it++)
    {
        for (auto spell : *it)
        {
            spell->reset();
        }
    }
}

Spell* Player::getSpell(std::string spellName)
{
    for (std::vector<Spell*>::iterator it = spells.begin(); it != spells.end(); it++)
    {
        if ((*it)->varName == spellName)
        {
            return *it;
        }
    }
    
    return nullptr;
}

bool Player::spellVectorExists(std::string spellName)
{
    for (std::vector<std::vector<Spell*>>::iterator it = spellVectors.begin(); it != spellVectors.end(); it++)
    {
        if ((*it)[0]->varName == spellName)
        {
            return true;
        }
    }

    return false;
}

std::vector<Spell*>& Player::getSpellVector(std::string spellName)
{
    for (std::vector<std::vector<Spell*>>::iterator it = spellVectors.begin(); it != spellVectors.end(); it++)
    {
        if ((*it)[0]->varName == spellName)
        {
            return *it;
        }
    }
}

Aura* Player::getAura(std::string auraName)
{
    for (std::vector<Aura*>::iterator it = auras.begin(); it != auras.end(); it++)
    {
        if ((*it)->varName == auraName)
        {
            return *it;
        }
    }
    
    return nullptr;
}

DamageOverTime* Player::getDot(std::string dotName)
{
    for (std::vector<DamageOverTime*>::iterator it = dots.begin(); it != dots.end(); it++)
    {
        if ((*it)->varName == dotName)
        {
            return *it;
        }
    }
    
    return nullptr;
}

double Player::getGcdValue(std::string varName)
{
    if (varName != "shadowfury" || getSpell("shadowfury") == nullptr)
    {
        return std::max(minimumGcdValue, round((gcdValue / (1 + ((stats->hasteRating / hasteRatingPerPercent + stats->hastePercent) / 100))) * 10000) / 10000);
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
    if (getSpell("bloodlust") != nullptr && !getAura("bloodlust")->active)
    {
        for (int i = 0; i < settings->bloodlustAmount; i++)
        {
            if (getSpellVector("bloodlust")[i]->ready())
            {
                return true;
            }
        }
    }
    if (getSpell("powerInfusion") != nullptr && !getAura("powerInfusion")->active)
    {
        for (int i = 0; i < settings->powerInfusionAmount; i++)
        {
            if (getSpellVector("powerInfusion")[i]->ready())
            {
                return true;
            }
        }
    }
    if (getSpell("innervate") != nullptr && !getAura("innervate")->active)
    {
        for (int i = 0; i < settings->innervateAmount; i++)
        {
            if (getSpellVector("innervate")[i]->ready())
            {
                return true;
            }
        }
    }
    if (getSpell("destructionPotion") != nullptr && getSpell("destructionPotion")->ready())
    {
        return true;
    }
    if (getSpell("bloodFury") != nullptr && getSpell("bloodFury")->ready())
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
    if (spellVectorExists("bloodlust") && !getAura("bloodlust")->active)
    {
        for (int i = 0; i < settings->bloodlustAmount; i++)
        {
            if (getSpellVector("bloodlust")[i]->ready())
            {
                getSpellVector("bloodlust")[i]->startCast();
                break;
            }
        }
    }
    if (spellVectorExists("powerInfusion") && !getAura("powerInfusion")->active)
    {
        for (int i = 0; i < settings->powerInfusionAmount; i++)
        {
            if (getSpellVector("powerInfusion")[i]->ready())
            {
                getSpellVector("powerInfusion")[i]->startCast();
                break;
            }
        }
    }
    if (spellVectorExists("innervate") && !getAura("innervate")->active)
    {
        for (int i = 0; i < settings->innervateAmount; i++)
        {
            if (getSpellVector("innervate")[i]->ready())
            {
                getSpellVector("innervate")[i]->startCast();
                break;
            }
        }
    }
    if (getSpell("destructionPotion") != nullptr && getSpell("destructionPotion")->ready())
    {
        getSpell("destructionPotion")->startCast();
    }
    if (getSpell("flameCap") != nullptr && getSpell("flameCap")->ready())
    {
        getSpell("flameCap")->startCast();
    }
    if (getSpell("bloodFury") != nullptr && getSpell("bloodFury")->ready())
    {
        getSpell("bloodFury")->startCast();
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

void Player::castLifeTapOrDarkPact()
{
    if (getSpell("darkPact") != nullptr && getSpell("darkPact")->ready())
    {
        getSpell("darkPact")->startCast();
    }
    else
    {
        getSpell("lifeTap")->startCast();
    }
}

double Player::getPartialResistMultiplier(SpellSchool school)
{
    if (school == SpellSchool::SHADOW)
    {
        return 1 - ((75 * settings->enemyShadowResist) / (level * 5)) / 100;
    }
    else if (school == SpellSchool::FIRE)
    {
        return 1 - ((75 * settings->enemyFireResist) / (level * 5)) / 100;
    }

    return 1;
}

bool Player::shouldWriteToCombatLog()
{
    return iteration == 10;
}

void Player::combatLog(std::string &entry)
{
    std::cout << fightTime << " " << entry << std::endl;
    combatLogEntries.push_back(std::to_string(fightTime) + " " + entry);
}