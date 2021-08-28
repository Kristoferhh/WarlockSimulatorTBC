#include "player.h"
#include "spell.h"
#include "common.h"

Player::Player(PlayerSettings* playerSettings) : auras(playerSettings->auras), talents(playerSettings->talents), sets(playerSettings->sets), stats(playerSettings->stats), itemId(playerSettings->itemId)
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
    critChanceMultiplier = 1000;
    minimumGcdValue = 1.5;
}

void Player::initialize()
{
    this->spells.insert(std::make_pair("lifeTap", new LifeTap(this)));
    this->spells.insert(std::make_pair("shadowBolt", new ShadowBolt(this)));
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

double Player::getSpellPower()
{
    double spellPower = stats->spellPower + demonicKnowledgeSpellPower;
    if (sets->spellfire == 3)
    {
        spellPower += stats->intellect * stats->intellectModifier * 0.07;
    }
    return spellPower;
}

double Player::getCritChance(SpellType spellType)
{
    double critChance = stats->critPercent + ((stats->intellect * stats->intellectModifier) * critPerInt);
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

double Player::getPartialResistMultiplier(int resist)
{
    return 1 - ((75 * resist) / (level * 5)) / 100;
}

bool Player::shouldWriteToCombatLog()
{
    return iteration == 2;
}

void Player::combatLog(std::string &entry)
{
    if (shouldWriteToCombatLog())
    {
        combatLogEntries.push_back(entry);
    }
}