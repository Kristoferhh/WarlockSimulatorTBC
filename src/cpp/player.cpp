#include "player.h"
#include "spell.h"
#include "common.h"
#include "emscripten.h"

Player::Player(PlayerSettings* playerSettings)
    : selectedAuras(playerSettings->auras), talents(playerSettings->talents), sets(playerSettings->sets), stats(playerSettings->stats), settings(playerSettings)
{
    stats->maxMana = stats->mana;
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
    // Spells
    spells.insert(std::make_pair("lifeTap", new LifeTap(this)));
    spells.insert(std::make_pair("shadowBolt", new ShadowBolt(this)));

    // Auras
    if (talents->improvedShadowBolt > 0)
    {
        auras.insert(std::make_pair("improvedShadowBolt", new ImprovedShadowBolt(this)));
        auras.insert(std::make_pair("spellstrike", new SpellstrikeAura(this)));
    }
}

void Player::reset()
{
    castTimeRemaining = 0;
    gcdRemaining = 0;
    stats->mana = stats->maxMana;
    mp5Timer = 5;
    fiveSecondRuleTimer = 5;
}

double Player::getGcdValue(std::string varName)
{
    if (spells.count("shadowfury") == 0 || varName != spells.at("shadowfury")->varName)
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

void Player::castLifeTapOrDarkPact()
{
    if (spells.count("darkPact") == 1 && spells.at("darkPact")->ready())
    {
        spells.at("darkPact")->startCast();
    }
    else
    {
        spells.at("lifeTap")->startCast();
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