#include "pet.h"
#include "player.h"

Pet::Pet(Player* player) : player(player)
{
    spells = std::make_unique<PetSpells>();
    auras = std::make_unique<PetAuras>();
    baseStats = std::make_unique<PetStats>();
    buffStats = std::make_unique<PetStats>();
    debuffStats = std::make_unique<PetStats>();
    stats = std::make_unique<PetStats>(player);
    enemyDodgeChance = 6.5;
    castTimeRemaining = 0;
    fiveSecondRuleTimerRemaining = 5;
    spiritTickTimerRemaining = 2;
    mode = player->settings->petIsAggressive ? PetMode::AGGRESSIVE : PetMode::PASSIVE;
    critMultiplier = 1.5;
    glancingBlowMultiplier = 1 - (0.1 + (player->settings->enemyLevel * 5 - player->level * 5) / 100.0);
    glancingBlowChance = std::max(0.0, 6 + (player->settings->enemyLevel * 5 - player->level * 5) * 1.2);
    critSuppression = 4.73;
}

void Pet::initialize()
{
    if (pet == PetName::IMP)
    {
        spells->Firebolt = std::make_unique<ImpFirebolt>(this);
    }
    else
    {
        spells->Melee = std::make_unique<Melee>(this);
        
        if (pet == PetName::SUCCUBUS)
        {
            spells->LashOfPain = std::make_unique<SuccubusLashOfPain>(this);
        }
        else if (pet == PetName::FELGUARD)
        {
            spells->Cleave = std::make_unique<FelguardCleave>(this);
            auras->DemonicFrenzy = std::make_unique<DemonicFrenzy>(this);
        }
    }

    if (player->settings->prepopBlackBook)
    {
        auras->BlackBook = std::make_unique<BlackBook>(this);
    }
}

void Pet::calculateStatsFromAuras()
{
    // Calculate melee hit chance
    // Formula from https://wowwiki-archive.fandom.com/wiki/Hit?oldid=1584399
    int levelDifference = player->settings->enemyLevel - player->level;
    if (levelDifference <= 2)
    {
        stats->meleeHitChance = 100 - (5 + levelDifference * 0.5);
    }
    else
    {
        stats->meleeHitChance = 100 - (7 + (levelDifference - 2) * 2);
    }

    // Calculate spell hit chance
    // Formula from https://web.archive.org/web/20161015101615/https://dwarfpriest.wordpress.com/2008/01/07/spell-hit-spell-penetration-and-resistances/
    if (levelDifference <= 2)
    {
        stats->spellHitChance = std::min(99, 100 - levelDifference - 4);
    }
    else if (levelDifference == 3)
    {
        stats->spellHitChance = 83;
    }
    else if (levelDifference == 4)
    {
        stats->spellHitChance = 72;
    }
    else
    {
        stats->spellHitChance = 61;
    }

    // Auras
    if (player->selectedAuras->petBlessingOfKings)
    {
        stats->staminaModifier *= 1.1;
        stats->intellectModifier *= 1.1;
        stats->agilityModifier *= 1.1;
        stats->strengthModifier *= 1.1;
        stats->spiritModifier *= 1.1;
    }
    if (player->selectedAuras->petBlessingOfWisdom)
    {
        stats->mp5 += 41;
    }
    if (player->selectedAuras->manaSpringTotem)
    {
        stats->mp5 += 50;
    }
    if (player->selectedAuras->wrathOfAirTotem)
    {
        buffStats->spellPower += 101;
    }
    if (player->selectedAuras->totemOfWrath)
    {
        int hitAndCritAmount = player->settings->totemOfWrathAmount * 3;

        buffStats->spellCritChance += hitAndCritAmount;
        stats->spellHitChance = std::min(99.0, stats->spellHitChance + hitAndCritAmount);
    }
    //todo improved motw
    if (player->selectedAuras->petMarkOfTheWild)
    {
        buffStats->stamina += 14;
        buffStats->intellect += 14;
        buffStats->strength += 14;
        buffStats->agility += 14;
        stats->spirit += 14;
    }
    //todo improved imp
    if (player->selectedAuras->bloodPact)
    {
        buffStats->stamina += 70;
    }
    if (player->selectedAuras->petArcaneIntellect)
    {
        buffStats->intellect += 40;
    }
    if (player->selectedAuras->petPrayerOfFortitude)
    {
        buffStats->stamina += 79;
    }
    if (player->selectedAuras->petPrayerOfSpirit)
    {
        stats->spirit += 50 * (1 + (0.1 * player->settings->improvedDivineSpirit));
    }
    if (player->selectedAuras->inspiringPresence)
    {
        stats->spellHitChance = std::min(99.0, stats->spellHitChance + 1);
    }
    if (player->selectedAuras->moonkinAura)
    {
        buffStats->spellCritChance += 5;
    }
    if (player->selectedAuras->eyeOfTheNight)
    {
        buffStats->spellPower += 34;
    }
    if (player->selectedAuras->chainOfTheTwilightOwl)
    {
        buffStats->spellCritChance += 2;
    }
    if (player->selectedAuras->jadePendantOfBlasting)
    {
        buffStats->spellPower += 15;
    }
    if (player->selectedAuras->idolOfTheRavenGoddess)
    {
        buffStats->spellCritRating += 20;
    }
    // Atiesh auras
    // Add 33sp if the player has Atiesh equipped since the aura's spell power is just added to the item itself
    if (player->items->twoHand == 22630)
    {
        buffStats->spellPower += 33;
    }
    if (player->selectedAuras->powerOfTheGuardianWarlock)
    {
        buffStats->spellPower += 33 * player->settings->warlockAtieshAmount;
    }
    if (player->selectedAuras->wrathOfAirTotem && player->settings->hasElementalShamanT4Bonus)
    {
        buffStats->spellPower += 20;
    }
    // todo: do pets even get crit from crit rating buffs?
    if (player->selectedAuras->powerOfTheGuardianMage)
    {
        buffStats->spellCritRating += 28 * player->settings->mageAtieshAmount;
    }
    if (player->selectedAuras->judgementOfTheCrusader)
    {
        buffStats->meleeCritChance += 3;
        buffStats->spellCritChance += 3;
    }
    // Multiply by 25% instead of 5% since it's happening every 5 seconds instead of every 1 second
    if (player->selectedAuras->vampiricTouch)
    {
        stats->mp5 += player->settings->shadowPriestDps * 0.25;
    }
    if (player->selectedAuras->faerieFire && player->settings->improvedFaerieFire)
    {
        stats->meleeHitChance += 3;
    }
    if (player->selectedAuras->exposeWeakness)
    {
        debuffStats->attackPower += (player->settings->survivalHunterAgility * 0.25 * (player->settings->exposeWeaknessUptime / 100.0));
    }
    if (player->selectedAuras->improvedHuntersMark)
    {
        debuffStats->attackPower += 110;
    }
    if (player->selectedAuras->petHeroicPresence)
    {
        stats->meleeHitChance++;
    }
    if (player->selectedAuras->petBlessingOfMight)
    {
        buffStats->attackPower += 220;
    }
    if (player->selectedAuras->petStrengthOfEarthTotem)
    {
        buffStats->strength += 86;
    }
    if (player->selectedAuras->petGraceOfAirTotem)
    {
        buffStats->agility += 67;
    }
    if (player->selectedAuras->petBattleShout)
    {
        buffStats->attackPower += 306;
    }
    if (player->selectedAuras->petTrueshotAura)
    {
        buffStats->attackPower += 300;
    }
    if (player->selectedAuras->petLeaderOfThePack)
    {
        buffStats->meleeCritChance += 5;
    }
    if (player->selectedAuras->petUnleashedRage)
    {
        stats->attackPowerModifier *= 1.1;
    }
    if (player->selectedAuras->petStaminaScroll)
    {
        buffStats->stamina += 20;
    }
    if (player->selectedAuras->petIntellectScroll)
    {
        buffStats->intellect += 20;
    }
    if (player->selectedAuras->petStrengthScroll)
    {
        buffStats->strength += 20;
    }
    if (player->selectedAuras->petAgilityScroll)
    {
        buffStats->agility += 20;
    }
    if (player->selectedAuras->petSpiritScroll)
    {
        stats->spirit += 20;
    }
    if (player->selectedAuras->petKiblersBits)
    {
        buffStats->strength += 20;
        stats->spirit += 20;
    }
    if (player->selectedAuras->ferociousInspiration)
    {
        stats->damageModifier *= std::pow(1.03, player->settings->ferociousInspirationAmount);
    }
    if (player->settings->isOrc)
    {
        stats->damageModifier *= 1.05;
    }

    // Hidden attack power modifiers (source: Max on warlock discord)
    if (pet == PetName::FELGUARD)
    {
        stats->attackPowerModifier *= 1.1;
    }
    else if (pet == PetName::SUCCUBUS)
    {
        stats->attackPowerModifier *= 1.05;
    }

    // Calculate armor
    if (petType == PetType::MELEE)
    {
        // Formula from https://wowwiki-archive.fandom.com/wiki/Damage_reduction?oldid=807810
        if (player->settings->enemyLevel <= 59)
        {
            armorMultiplier  = 1 - player->settings->enemyArmor / (player->settings->enemyArmor + 400 + 85 * player->settings->enemyLevel);
        }
        else
        {
            armorMultiplier = 1 - player->settings->enemyArmor / (player->settings->enemyArmor - 22167.5 + 467.5 * player->settings->enemyLevel);
        }
    }
    armorMultiplier = std::max(0.25, armorMultiplier);
}

void Pet::calculateStatsFromPlayer(bool announceInCombatLog)
{
    stats->stamina = baseStats->stamina + buffStats->stamina + (0.3 * player->stats->stamina * player->stats->staminaModifier);
    stats->intellect = baseStats->intellect + buffStats->intellect + (0.3 * player->stats->intellect * player->stats->intellectModifier);
    player->demonicKnowledgeSpellPower = ((stats->stamina * stats->staminaModifier) + (stats->intellect * stats->intellectModifier)) * (0.04 * player->talents->demonicKnowledge);
    baseStats->attackPower = (baseStats->strength + buffStats->strength) * stats->strengthModifier * 2 - 20;
    stats->attackPower = baseStats->attackPower + buffStats->attackPower + debuffStats->attackPower + (player->getSpellPower() + std::max(player->stats->shadowPower, player->stats->firePower)) * 0.57;
    stats->agility = baseStats->agility + buffStats->agility;
    stats->meleeCritChance = player->talents->demonicTactics + (stats->agility * stats->agilityModifier) * 0.04 + 0.65 + buffStats->meleeCritChance;
    stats->spellPower = buffStats->spellPower + (player->getSpellPower() + std::max(player->stats->shadowPower, player->stats->firePower)) * 0.15;
    if (petType == PetType::MELEE)
    {
        // Halp, need confirmation that this is actually the right way to get its average melee damage.
        dmg = (getAttackPower() / 14 + 51.7) * baseMeleeSpeed;
        stats->maxMana = baseStats->mana + stats->intellect * 11.555 * stats->intellectModifier;
    }
    else if (pet == PetName::IMP)
    {
        stats->maxMana = baseStats->mana + stats->intellect * stats->intellectModifier * 4.95;
    }
    if (pet == PetName::IMP || pet == PetName::SUCCUBUS)
    {
        stats->spellCritChance = 0.0125 * (stats->intellect * stats->intellectModifier) + 0.91 + player->talents->demonicTactics + buffStats->spellCritChance + (buffStats->spellCritRating / player->critRatingPerPercent);
    }
    if (announceInCombatLog && player->shouldWriteToCombatLog())
    {
        player->combatLog("Recalculated " + name + "'s stats");
    }
}

void Pet::setup()
{
    initialize();
    calculateStatsFromAuras();
    calculateStatsFromPlayer(false);
}

void Pet::reset()
{
    stats->mana = stats->maxMana;
    fiveSecondRuleTimerRemaining = 5;
    spiritTickTimerRemaining = 2;
    castTimeRemaining = 0;

    // Reset spells
    if (spells->Melee != NULL) spells->Melee->reset();
    if (spells->Firebolt != NULL) spells->Firebolt->reset();
    if (spells->LashOfPain != NULL) spells->LashOfPain->reset();
    if (spells->Cleave != NULL) spells->Cleave->reset();

    // End Auras
    if (auras->BlackBook != NULL && auras->BlackBook->active) auras->BlackBook->fade(true);
    if (auras->DemonicFrenzy != NULL && auras->DemonicFrenzy->active) auras->DemonicFrenzy->fade(true);
    
    calculateStatsFromPlayer(false);
}

double Pet::getMeleeCritChance()
{
    return stats->meleeCritChance - critSuppression;
}

double Pet::getSpellCritChance()
{
    return stats->spellCritChance;
}

bool Pet::isCrit(AttackType type)
{
    if (type == AttackType::PHYSICAL)
    {
        return player->getRand() <= getMeleeCritChance() * player->critChanceMultiplier;
    }
    else if (type == AttackType::MAGICAL)
    {
        return player->getRand() <= getSpellCritChance() * player->critChanceMultiplier;
    }

    return false;
}

double Pet::getMeleeHitChance()
{
    return stats->meleeHitChance;
}

double Pet::getSpellHitChance()
{
    return std::min(99.0, stats->spellHitChance);
}

bool Pet::isHit(AttackType type)
{
    if (type == AttackType::PHYSICAL)
    {
        return player->getRand() <= getMeleeHitChance() * player->critChanceMultiplier;
    }
    else
    {
        return player->getRand() <= getSpellHitChance() * player->critChanceMultiplier;
    }
}

int Pet::getAttackPower()
{
    // Remove AP from debuffs on the boss before multiplying by the AP multiplier since it doesn't affect those debuffs
    double ap = (stats->attackPower - debuffStats->attackPower) * stats->attackPowerModifier;
    if (auras->DemonicFrenzy != NULL)
    {
        ap *= (1 + 0.05 * auras->DemonicFrenzy->stacks);
    }
    // Add the AP from boss debuffs back and return
    ap += debuffStats->attackPower;

    return ap;
}

void Pet::tick(double t)
{
    castTimeRemaining -= t;
    fiveSecondRuleTimerRemaining -= t;
    spiritTickTimerRemaining -= t;

    // Auras
    if (auras->BlackBook != NULL && auras->BlackBook->durationRemaining > 0) auras->BlackBook->tick(t);

    // Spells
    if (spells->Melee != NULL && spells->Melee->cooldownRemaining > 0) spells->Melee->tick(t);
    if (spells->Cleave != NULL && spells->Cleave->cooldownRemaining > 0) spells->Cleave->tick(t);
    if (spells->Firebolt != NULL && spells->Firebolt->casting) spells->Firebolt->tick(t);
    if (spells->LashOfPain != NULL && spells->LashOfPain->cooldownRemaining > 0) spells->LashOfPain->tick(t);

    // MP5
    if (spiritTickTimerRemaining <= 0)
    {
        double manaGain = stats->mp5;
        spiritTickTimerRemaining = 5;

        // Formulas from Max on the warlock discord https://discord.com/channels/253210018697052162/823476479550816266/836007015762886707 & https://discord.com/channels/253210018697052162/823476479550816266/839484387741138994
        // Mana regen from spirit
        if (fiveSecondRuleTimerRemaining <= 0)
        {
            if (pet == PetName::IMP)
            {
                manaGain += ((baseStats->spirit + stats->spirit) * stats->spiritModifier) + 0.7 * (stats->intellect * stats->intellectModifier) - 258;
            }
            else if (pet == PetName::FELGUARD || pet == PetName::SUCCUBUS)
            {
                manaGain += 0.75 * ((baseStats->spirit + stats->spirit) * stats->spiritModifier) + 0.62 * (stats->intellect * stats->intellectModifier) - 108;
            }
        }
        // Mana regen while the 5 second spirit regen timer is active (no bonus from spirit)
        else
        {
            if (pet == PetName::IMP)
            {
                manaGain += 0.375 * (stats->intellect * stats->intellectModifier) - 123;
            }
            else if (pet == PetName::FELGUARD || pet == PetName::SUCCUBUS)
            {
                manaGain += 0.365 * (stats->intellect * stats->intellectModifier) - 48;
            }
        }

        int currentMana = stats->mana;
        
        stats->mana = std::min(stats->maxMana, stats->mana + static_cast<int>(manaGain));
        if (stats->mana > currentMana && player->shouldWriteToCombatLog())
        {
            player->combatLog(name + " gains " + std::to_string(round(manaGain)) + " mana from Mp5/Spirit regeneration (" + std::to_string(currentMana) + " -> " + std::to_string(stats->mana) + ")");
        }
    }
}

Imp::Imp(Player* player) : Pet(player)
{
    name = "Imp";
    pet = PetName::IMP;
    petType = PetType::RANGED;
    baseStats->stamina = 101;
    baseStats->intellect = 327;
    baseStats->mana = 756;
    baseStats->spirit = 263;
    baseStats->strength = 145;
    baseStats->agility = 38;
    setup();
}

Succubus::Succubus(Player* player) : Pet(player)
{
    name = "Succubus";
    pet = PetName::SUCCUBUS;
    petType = PetType::MELEE;
    baseStats->stamina = 280;
    baseStats->intellect = 133;
    baseStats->mana = 849;
    baseStats->spirit = 122;
    baseStats->strength = 153;
    baseStats->agility = 109;
    baseMeleeSpeed = 2;
    stats->damageModifier *= 1 + (0.02 * player->talents->masterDemonologist);
    setup();
}

Felguard::Felguard(Player* player) : Pet(player)
{
    name = "Felguard";
    petType = PetType::MELEE;
    pet = PetName::FELGUARD;
    baseStats->stamina = 280;
    baseStats->strength = 153;
    baseStats->agility = 108;
    baseStats->intellect = 133;
    baseStats->spirit = 122;
    baseStats->mana = 893;
    baseMeleeSpeed = 2;
    stats->damageModifier *= 1 + (0.01 * player->talents->masterDemonologist);
    setup();
}