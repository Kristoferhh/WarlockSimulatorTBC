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
    return this->castTime;
}

bool Spell::canCast()
{
    return (!onGcd || isNonWarlockAbility || player->gcdRemaining <= 0) && (isProc || isNonWarlockAbility || player->castTimeRemaining <= 0) && cooldownRemaining <= 0;
}

bool Spell::hasEnoughMana()
{
    return this->manaCost * this->player->stats->manaCostModifier <= this->player->stats->mana;
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
        if (!isProc)
        {
            //combatLogMsg.append("Started casting " + name + " - Cast time: " + player->castTimeRemaining - player->spellDelay) + " (" + round((player->stats.hasteRating / hasteRatingPerPercent + player->stats.hastePercent) * 10000) / 10000 + "% haste at a base cast speed of " + castTime + ").";
        }
    }
    else
    {
        if (!isProc)
        {
            combatLogMsg.append("Cast " + name);
        }
        cast();
    }
    if (onGcd && !isNonWarlockAbility)
    {
        combatLogMsg.append(" - Global cooldown: " + std::to_string(player->gcdRemaining));
    }
    if (predictedDamage > 0)
    {
        combatLogMsg.append(" - Estimated damage / Cast time: " + std::to_string(round(predictedDamage)));
    }
    player->combatLog(combatLogMsg);
}

void Spell::tick(int t)
{
    if (cooldownRemaining > 0 && cooldownRemaining - t <= 0)
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
    if (this->manaCost > 0)
    {
        this->player->stats->mana -= this->manaCost * this->player->stats->manaCostModifier;
    }
    if (this->doesDamage)
    {
        damage(false);
    }
}

void Spell::damage(bool isCrit)
{
    int dmg = this->dmg;
    int spellPower = this->player->getSpellPower();
    if (this->school == School::SHADOW)
    {
        spellPower += this->player->stats->shadowPower;
    }
    else if (this->school == School::FIRE)
    {
        spellPower += this->player->stats->firePower;
    }
    dmg += spellPower * this->coefficient;
    dmg *= this->modifier;
    if (this->school == School::SHADOW)
    {
        dmg *= this->player->stats->shadowModifier;
    }
    else if (this->school == School::FIRE)
    {
        dmg *= this->player->stats->fireModifier;
    }

    this->player->iterationDamage += dmg;
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
    school = School::SHADOW;
    type = Type::DESTRUCTION;
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

bool LifeTap::ready()
{
    return Spell::ready() && manaGain() + player->stats->mana < player->stats->maxMana;
}

int LifeTap::manaGain()
{
    return (manaReturn + ((player->getSpellPower() + player->stats->shadowPower) * coefficient)) * modifier;
}

void LifeTap::cast()
{
    const int manaGain = this->manaGain();
    this->player->totalManaRegenerated += manaGain;
    
    if (player->stats->mana + manaGain > player->stats->maxMana)
    {
        std::string combatLogEntry = "Life Tap used at too high mana (mana wasted)";
        player->combatLog(combatLogEntry);
    }
    player->stats->mana = std::min(player->stats->maxMana, player->stats->mana + manaGain);
}