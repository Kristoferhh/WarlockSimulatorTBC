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
        if (!isProc && player->shouldWriteToCombatLog())
        {
            combatLogMsg.append("Started casting " + name + " - Cast time: " + std::to_string(player->castTimeRemaining - player->spellDelay)) + " (" + std::to_string(round((player->stats->hasteRating / player->hasteRatingPerPercent + player->stats->hastePercent) * 10000) / 10000) + "% haste at a base cast speed of " + std::to_string(castTime) + ").";
        }
    }
    else
    {
        if (!isProc && player->shouldWriteToCombatLog())
        {
            combatLogMsg.append("Cast " + name);
        }
        cast();
    }
    if (onGcd && !isNonWarlockAbility && player->shouldWriteToCombatLog())
    {
        combatLogMsg.append(" - Global cooldown: " + std::to_string(player->gcdRemaining));
    }
    if (predictedDamage > 0 && player->shouldWriteToCombatLog())
    {
        combatLogMsg.append(" - Estimated damage / Cast time: " + std::to_string(round(predictedDamage)));
    }
    player->combatLog(combatLogMsg);
}

void Spell::tick(int t)
{
    if (cooldownRemaining > 0 && cooldownRemaining - t <= 0 && player->shouldWriteToCombatLog())
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
    const int currentMana = player->stats->mana;
    if (manaCost > 0)
    {
        player->stats->mana -= manaCost * player->stats->manaCostModifier;
        player->fiveSecondRuleTimer = 5;
    }

    cooldownRemaining = cooldown;
    casting = false;

    if (castTime > 0 && player->shouldWriteToCombatLog())
    {
        std::string msg = "Finished casting " + name + " - Mana: " + std::to_string(round(currentMana)) + " -> " + std::to_string(round(player->stats->mana)) + " - Mana Cost: " + std::to_string(round(manaCost)) + " - Mana Cost Modifier: " + std::to_string(round(player->stats->manaCostModifier * 100)) + "%";
        player->combatLog(msg);
    }

    bool isCrit = false;
    if (canCrit)
    {
        isCrit = player->isCrit(type, bonusCrit);
        if (isCrit)
        {
            //todo
        }
    }

    if (((!isItem && !isNonWarlockAbility && varName != "amplifyCurse") || doesDamage) && !player->isHit(type))
    {
        if (player->shouldWriteToCombatLog())
        {
            std::string msg = name + " *resist*";
            player->combatLog(msg);
        }
        //todo
        return;
    }

    if (isDot || isAura)
    {
        //player->auras.at(varName)->apply();
    }

    if (doesDamage)
    {
        damage(isCrit);
    }
}

double Spell::getModifier()
{
    if (school == SpellSchool::SHADOW)
    {
        return player->stats->shadowModifier * modifier;
    }
    else if (school == SpellSchool::FIRE)
    {
        return player->stats->fireModifier * modifier;
    }
    return modifier;
}

void Spell::damage(bool isCrit)
{
    int dmg = this->dmg;
    int spellPower = player->getSpellPower();
    double critMultiplier = player->critMultiplier;

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

    if (isCrit)
    {
        critMultiplier = getCritMultiplier(critMultiplier);
        dmg *= critMultiplier;
        onCritProcs();
    }
    else
    {
        //decrement isb
    }

    onDamageProcs();
    player->iterationDamage += dmg;

    //T5 4pc
    if (player->sets->t5 >= 4)
    {
        /*if (varName == "shadowBolt" && player->auras.count("corruption") && player->auras->corruption->active)
        {
            player->auras->corruption->t5BonusModifier *= 1.1;
        }
        else if (varName == "incinerate" && player->auras.count("immolate") && player->auras->immolate->active)
        {

            player->auras->immolate->t5BonusModifier *= 1.1;
        }*/
    }
}

double Spell::getCritMultiplier(double critMult)
{
    double critMultiplier = critMult;

    // Chaotic Skyfire Diamond
    if (player->metaGemId == "34220")
    {
        critMultiplier *= 1.03;
    }
    // Ruin
    if (type == SpellType::DESTRUCTION && player->talents->ruin == 1)
    {
        // Ruin doubles the *bonus* of your crits, not the damage of the crit itself
        // So if your crit damage % is e.g. 154.5% it would become 209% because the 54.5% is being doubled
        critMultiplier -= 1;
        critMultiplier *= 2;
        critMultiplier += 1;
    }
    return critMultiplier;
}

void Spell::onCritProcs()
{

}

void Spell::onDamageProcs()
{

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
    
    if (player->shouldWriteToCombatLog() && player->stats->mana + manaGain > player->stats->maxMana)
    {
        std::string combatLogEntry = "Life Tap used at too high mana (mana wasted)";
        player->combatLog(combatLogEntry);
    }
    player->stats->mana = std::min(player->stats->maxMana, player->stats->mana + manaGain);
}