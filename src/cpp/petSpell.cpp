#include "petSpell.h"
#include "common.h"
#include "player.h"
#include "petAura.h"

PetSpell::PetSpell(Pet* pet) : pet(pet)
{
    casting = false;
    canCrit = true;
    cooldownRemaining = 0;
    castTime = 0;
    manaCost = 0;
    modifier = 1;
    school = SpellSchool::NO_SCHOOL;
    cooldown = 0;
}

void PetSpell::setup()
{
    if (pet->player->combatLogBreakdown.count(name) == 0)
    {
        pet->player->combatLogBreakdown.insert(std::make_pair(name, new CombatLogBreakdown(name)));
    }
}

bool PetSpell::ready()
{
    return cooldownRemaining <= 0 && pet->stats->mana >= manaCost && pet->castTimeRemaining <= 0;
}

double PetSpell::getBaseDamage()
{
    return dmg;
}

double PetSpell::getCastTime()
{
    return round((castTime / pet->stats->hastePercent) * 10000) / 10000;
}

double PetSpell::getCooldown()
{
    return cooldown;
}

void PetSpell::tick(double t)
{
    if (pet->player->shouldWriteToCombatLog() && cooldownRemaining > 0 && cooldownRemaining - t <= 0)
    {
        pet->player->combatLog(pet->name + "'s " + name + " off cooldown");
    }

    cooldownRemaining -= t;

    if (casting && pet->castTimeRemaining <= 0)
    {
        casting = false;
        cast();
    }
}

void PetSpell::startCast()
{
    // Error: Starting to cast a spell while casting another spell
    if (pet->castTimeRemaining > 0)
    {
        pet->player->throwError("Pet attempting to cast " + name + " while pet's cast time remaining is at " + std::to_string(pet->castTimeRemaining) + " sec");
    }

    // Error: Casting a spell while it's on cooldown
    if (cooldownRemaining > 0)
    {
        pet->player->throwError("Pet attempting to cast " + name + " while it's still on cooldown (" + std::to_string(cooldownRemaining) + " seconds remaining)");
    }
    
    if (castTime > 0)
    {
        casting = true;
        pet->castTimeRemaining = getCastTime();

        if (pet->player->shouldWriteToCombatLog())
        {
            pet->player->combatLog(pet->name + " started casting " + name + ". Cast time: " + std::to_string(pet->castTimeRemaining) + " (" + std::to_string(round(pet->stats->hastePercent * 10000) / 10000.0) + "% haste at a base cast speed of " + std::to_string(castTime) + ")");
        }
    }
    else
    {
        cast();
    }
}

void PetSpell::reset()
{
    cooldownRemaining = 0;
    casting = false;
}

void PetSpell::cast()
{
    cooldownRemaining = getCooldown();

    std::string combatLogMsg = pet->name;
    if (pet->player->shouldWriteToCombatLog())
    {
        if (castTime > 0)
        {
            combatLogMsg.append(" finished casting " + name);
        }
        else
        {
            combatLogMsg.append(" casts " + name);

            if (pet->spells->Melee != NULL && name == pet->spells->Melee->name)
            {
                combatLogMsg.append(" - Attack Speed: " + truncateTrailingZeros(std::to_string(pet->spells->Melee->getCooldown()), 2) + " (" + truncateTrailingZeros(std::to_string(round(pet->stats->hastePercent * 10000) / 100.0 - 100), 4) + "% haste at a base attack speed of " + truncateTrailingZeros(std::to_string(pet->spells->Melee->cooldown), 2) + ")");
            }
        }
    }

    if (manaCost > 0 && !pet->player->settings->infinitePetMana)
    {
        pet->stats->mana -= manaCost;
        pet->fiveSecondRuleTimerRemaining = 5;

        if (pet->player->shouldWriteToCombatLog())
        {
            combatLogMsg.append(" - Pet mana: " + std::to_string(pet->stats->mana) + "/" + std::to_string(pet->stats->maxMana));
        }
    }

    pet->player->combatLogBreakdown.at(name)->casts++;

    if (pet->player->shouldWriteToCombatLog())
    {
        pet->player->combatLog(combatLogMsg);
    }

    // Physical dmg spell
    if (type == AttackType::PHYSICAL)
    {
        bool isCrit = false;
        bool isGlancing = false;
        double critChance = pet->getMeleeCritChance() * pet->player->critChanceMultiplier;
        double dodgeChance = critChance + pet->enemyDodgeChance * pet->player->critChanceMultiplier;
        double missChance = dodgeChance + (100 - pet->getMeleeHitChance()) * pet->player->critChanceMultiplier;
        double glancingChance = missChance;

        // Only check for a glancing if it's a normal melee attack
        if (pet->spells->Melee != NULL && name == pet->spells->Melee->name)
        {
            glancingChance += pet->glancingBlowChance * pet->player->critChanceMultiplier;
        }

        // Check whether the roll is a crit, dodge, miss, glancing, or just a normal hit.
        //todo should maybe give the pet its own random generator object
        int attackRoll = pet->player->getRand();

        // Crit
        if (attackRoll <= critChance)
        {
            pet->player->combatLogBreakdown.at(name)->crits++;
            isCrit = true;
        }
        // Dodge
        else if (attackRoll <= dodgeChance)
        {
            pet->player->combatLogBreakdown.at(name)->dodge++;
            
            if (pet->player->shouldWriteToCombatLog())
            {
                pet->player->combatLog(pet->name + " " + name + " *dodge*");
            }
            return;
        }
        // Miss
        else if (attackRoll <= missChance)
        {
            pet->player->combatLogBreakdown.at(name)->misses++;

            if (pet->player->shouldWriteToCombatLog())
            {
                pet->player->combatLog(pet->name + " " + name + " *miss*");
            }
            return;
        }
        // Glancing Blow
        else if (attackRoll <= glancingChance && pet->spells->Melee != NULL && name == pet->spells->Melee->name)
        {
            pet->player->combatLogBreakdown.at(name)->glancingBlows++;
            isGlancing = true;
        }

        damage(isCrit, isGlancing);
    }
    // Magic dmg spell
    else if (type == AttackType::MAGICAL)
    {
        // Check for resist
        if (!pet->isHit(type))
        {
            pet->player->combatLogBreakdown.at(name)->misses++;
            
            if (pet->player->shouldWriteToCombatLog())
            {
                pet->player->combatLog(pet->name + " " + name + " *resist*");
            }
        }
        else
        {
            // Check for crit
            bool isCrit = false;

            if (canCrit && pet->isCrit(type))
            {
                isCrit = true;
                pet->player->combatLogBreakdown.at(name)->crits++;
            }

            damage(isCrit, false);
        }
    }
}

void PetSpell::damage(bool isCrit, bool isGlancing)
{
    double baseDamage = getBaseDamage();
    double dmg = baseDamage;
    double dmgModifier = modifier;

    // Add damage from Spell Power
    if (type == AttackType::MAGICAL)
    {
        dmg += pet->stats->spellPower * coefficient;
    }

    // Multiply if it's a crit
    if (isCrit)
    {
        dmg *= pet->critMultiplier;
    }

    // Magic damage multipliers
    if (type == AttackType::MAGICAL)
    {
        // Curse of the Elements
        if (pet->player->selectedAuras->curseOfTheElements && (school == SpellSchool::SHADOW || school == SpellSchool::FIRE))
        {
            dmgModifier *= (1.1 + 0.01 * pet->player->settings->improvedCurseOfTheElements);
        }
        
        // Misery
        if (pet->player->selectedAuras->misery)
        {
            dmgModifier *= 1.05;
        }

        // Shadow Damage Multipliers
        if (school == SpellSchool::SHADOW)
        {
            // Shadow Weaving
            if (pet->player->selectedAuras->shadowWeaving)
            {
                dmgModifier *= 1.1;
            }

            // ISB
            if ((pet->player->auras->ImprovedShadowBolt != NULL && pet->player->auras->ImprovedShadowBolt->active) || pet->player->settings->usingCustomIsbUptime)
            {
                // Custom ISB Uptime
                if (pet->player->settings->usingCustomIsbUptime)
                {
                    dmgModifier *= (1 + 0.2 * (pet->player->settings->customIsbUptimeValue / 100.0));
                }
                // Normal ISB
                else
                {
                    dmgModifier *= pet->player->auras->ImprovedShadowBolt->modifier;
                    pet->player->auras->ImprovedShadowBolt->decrementStacks();
                }
            }
        }
        // Fire Damage Multipliers
        else if (school == SpellSchool::FIRE)
        {
            // Improved Scorch
            if (pet->player->selectedAuras->improvedScorch)
            {
                dmgModifier *= 1.15;
            }
        }
    }
    // Physical Damage Multipliers
    else if (type == AttackType::PHYSICAL)
    {
        // Blood Frenzy
        if (pet->player->selectedAuras->bloodFrenzy)
        {
            dmgModifier *= 1.04;
        }

        // Armor Damage Reduction
        dmg *= pet->armorMultiplier;
    }

    // Pet Damage Modifier (from Unholy Power, Master Demonologist, etc.)
    dmgModifier *= pet->stats->damageModifier;

    if (isGlancing)
    {
        dmg *= pet->glancingBlowMultiplier;
    }

    dmg *= dmgModifier;

    // Partial Resist Reduction
    double partialResistMultiplier = pet->player->getPartialResistMultiplier(school);
    if (type == AttackType::MAGICAL)
    {
        dmg *= partialResistMultiplier;
    }

    pet->player->addIterationDamageAndMana(name, 0, dmg);
    pet->player->iterationDamage += dmg;

    if (pet->pet == PetName::FELGUARD)
    {
        pet->auras->DemonicFrenzy->apply();
    }

    std::string combatLogMsg = pet->name + " " + name + " ";
    if (pet->player->shouldWriteToCombatLog())
    {
        if (isCrit) combatLogMsg.append("*");
        combatLogMsg.append(truncateTrailingZeros(std::to_string(round(dmg))));
        if (isCrit) combatLogMsg.append("*");
        if (isGlancing) combatLogMsg.append(" Glancing");
        combatLogMsg.append(" (" + truncateTrailingZeros(std::to_string(round(baseDamage))) + " Base Damage");
        if (type == AttackType::MAGICAL)
        {
            combatLogMsg.append(" - " + std::to_string(round(coefficient * 1000) / 1000.0) + " Coefficient");
            combatLogMsg.append(" - " + std::to_string(pet->stats->spellPower) + " Spell Power");
            combatLogMsg.append(" - " + std::to_string(round(partialResistMultiplier * 1000) / 10.0) + "% Partial Resist Multiplier");
        }
        else if (type == AttackType::PHYSICAL)
        {
            if (isGlancing) combatLogMsg.append(" - " + truncateTrailingZeros(std::to_string(pet->glancingBlowMultiplier * 100), 1) + "% Glancing Blow Multiplier");
            combatLogMsg.append(" - " + truncateTrailingZeros(std::to_string(round(pet->getAttackPower()))) + " Attack Power");
            combatLogMsg.append(" - " + truncateTrailingZeros(std::to_string(round(pet->armorMultiplier * 10000) / 100.0), 2) + "% Damage Modifier (Armor)");
        }
        if (isCrit) combatLogMsg.append(" - " + truncateTrailingZeros(std::to_string(pet->critMultiplier * 100), 1) + "% Crit Multiplier");
        combatLogMsg.append(" - " + truncateTrailingZeros(std::to_string(round(dmgModifier * 10000) / 100.0), 2) + "% Damage Modifier");
        combatLogMsg.append(")");
        pet->player->combatLog(combatLogMsg);
    }
}

ImpFirebolt::ImpFirebolt(Pet* pet) : PetSpell(pet)
{
    name = "Firebolt";
    castTime = 2 - (0.25 * pet->player->talents->improvedFirebolt);
    manaCost = 145;
    dmg = 119.5 * (1 + 0.1 * pet->player->talents->improvedImp);
    coefficient = 2 / 3.5;
    school = SpellSchool::FIRE;
    type = AttackType::MAGICAL;
    setup();
}

Melee::Melee(Pet* pet) : PetSpell(pet)
{
    cooldown = 2;
    name = "Melee";
    type = AttackType::PHYSICAL;
    setup();
}

double Melee::getBaseDamage()
{
    return pet->dmg;
}

double Melee::getCooldown()
{
    return round((cooldown / pet->stats->hastePercent) * 10000) / 10000.0;
}

FelguardCleave::FelguardCleave(Pet* pet) : PetSpell(pet)
{
    cooldown = 6;
    manaCost = 417;
    name = "Cleave";
    type = AttackType::PHYSICAL;
    setup();
}

double FelguardCleave::getBaseDamage()
{
    return pet->spells->Melee->getBaseDamage() + 78;
}

SuccubusLashOfPain::SuccubusLashOfPain(Pet* pet) : PetSpell(pet)
{
    cooldown = 12;
    manaCost = 190;
    name = "Lash of Pain";
    dmg = 123;
    school = SpellSchool::SHADOW;
    coefficient = 0.429;
    type = AttackType::MAGICAL;
    canCrit = true;
    setup();
}