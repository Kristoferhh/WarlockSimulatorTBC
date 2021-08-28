#include "spell.h"
#include "player.h"

Spell::Spell(Player* player) : player(player)
{
    modifier = 1;
}

void Spell::reset()
{
    cooldownRemaining = 0;
    casting = false;
}

void Spell::setup()
{
    varName = name; //todo implement camelCase()
    if (minDmg > 0 && maxDmg > 0)
    {
      dmg = (minDmg + maxDmg) / 2;
    }
    if (minMana > 0 && maxMana > 0)
    {
      avgManaValue = (minMana + maxMana) / 2;
    }
}

double Spell::getCastTime()
{
    return castTime;
}

bool Spell::canCast()
{
    return (!onGcd || isNonWarlockAbility || player->gcdRemaining <= 0) && (isProc || isNonWarlockAbility || player->castTimeRemaining <= 0) && cooldownRemaining <= 0;
}

bool Spell::hasEnoughMana()
{
    return manaCost * player->stats->manaCostModifier <= player->stats->mana;
}

bool Spell::ready()
{
    return canCast() && hasEnoughMana();
}

void Spell::startCast(double predictedDamage)
{
    if (onGcd)
    {
        player->gcdRemaining = player->getGcdValue(varName);
    }

    std::string combatLogMsg = "";
    if (castTime > 0)
    {
        casting = true;
        player->castTimeRemaining = getCastTime();
        if (!isProc && player->iteration == 2)
        {
            combatLogMsg.append("Started casting " + name + " - Cast time: " + std::to_string(player->castTimeRemaining - player->spellDelay)) + " (" + std::to_string(round((player->stats->hasteRating / player->hasteRatingPerPercent + player->stats->hastePercent) * 10000) / 10000) + "% haste at a base cast speed of " + std::to_string(castTime) + ").";
        }
    }
    else
    {
        if (!isProc && player->iteration == 2)
        {
            combatLogMsg.append("Cast " + name);
        }
        cast();
    }
    if (onGcd && !isNonWarlockAbility && player->iteration == 2)
    {
        combatLogMsg.append(" - Global cooldown: " + std::to_string(player->gcdRemaining));
    }
    if (predictedDamage > 0 && player->iteration == 2)
    {
        combatLogMsg.append(" - Estimated damage / Cast time: " + std::to_string(round(predictedDamage)));
    }
    player->combatLog(combatLogMsg);
}

void Spell::tick(int t)
{
    if (cooldownRemaining > 0 && cooldownRemaining - t <= 0 && player->iteration == 2)
    {
        std::string combatLogEntry = name + " is off cooldown";
        player->combatLog(combatLogEntry);
    }
    cooldownRemaining -= t;

    if (casting && player->castTimeRemaining <= 0)
    {
        cast();
    }
}

void Spell::cast()
{
    if (manaCost > 0)
    {
        player->stats->mana -= manaCost * player->stats->manaCostModifier;
    }

    cooldownRemaining = cooldown;

    if (doesDamage)
    {
        damage(false);
    }
}

void Spell::damage(bool isCrit)
{
    int dmg = this->dmg;
    int spellPower = player->getSpellPower();
    if (school == SpellSchool::SHADOW)
    {
        spellPower += player->stats->shadowPower;
    }
    else if (school == SpellSchool::FIRE)
    {
        spellPower += player->stats->firePower;
    }
    dmg += spellPower * coefficient;
    dmg *= modifier;
    if (school == SpellSchool::SHADOW)
    {
        dmg *= player->stats->shadowModifier;
    }
    else if (school == SpellSchool::FIRE)
    {
        dmg *= player->stats->fireModifier;
    }

    player->iterationDamage += dmg;
}

ShadowBolt::ShadowBolt(Player* player) : Spell(player)
{
    castTime = calculateCastTime();
    manaCost = 420 * (1 - 0.01 * player->talents->cataclysm);
    coefficient = (3 / 3.5) + (0.04 * player->talents->shadowAndFlame);
    minDmg = 544;
    maxDmg = 607;
    name = "Shadow Bolt";
    doesDamage = true;
    canCrit = true;
    school = SpellSchool::SHADOW;
    type = SpellType::DESTRUCTION;
    setup();

    // T6 4pc bonus
    if (player->sets->t6 >= 4)
    {
      modifier *= 1.06;
    }
}

void ShadowBolt::startCast(double predictedDamage = 0)
{
    //bool hasShadowTrance = player->auras.count("shadowTrance") > 0;

    /*if (hasShadowTrance && player->auras["shadowTrance"].active)
    {
      castTime = 0;
    }*/

    Spell::startCast();
    
    /*if (hasShadowTrance && player->auras["shadowTrance"].active)
    {
      castTime = calculateCastTime();
      player->auras["shadowTrance"].fade();
    }*/
}

double ShadowBolt::calculateCastTime()
{
    return 3 - (0.1 * player->talents->bane);
}

LifeTap::LifeTap(Player* player) : Spell(player)
{
    name = "Life Tap";
    manaReturn = 582;
    coefficient = 0.8;
    modifier = 1 * (1 + 0.1 * player->talents->improvedLifeTap);
    breakdownTable = "manaGain";
    setup();
}

int LifeTap::manaGain()
{
    return (manaReturn + ((player->getSpellPower() + player->stats->shadowPower) * coefficient)) * modifier;
}

void LifeTap::cast()
{
    const int manaGain = this->manaGain();
    player->totalManaRegenerated += manaGain;
    
    if (player->iteration == 2 && player->stats->mana + manaGain > player->stats->maxMana)
    {
        std::string combatLogEntry = "Life Tap used at too high mana (mana wasted)";
        player->combatLog(combatLogEntry);
    }
    player->stats->mana = std::min(player->stats->maxMana, player->stats->mana + manaGain);
}