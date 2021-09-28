#include "spell.h"
#include "player.h"
#include "common.h"
#include <iomanip>
#include "bindings.h"

Spell::Spell(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot) : player(player), auraEffect(aura), dotEffect(dot)
{
    modifier = 1;
    coefficient = 0;
    cooldown = 0;
    school = SpellSchool::NO_SCHOOL;
    isNonWarlockAbility = false;
    isDot = false;
    doesDamage = false;
    canCrit = false;
    isItem = false;
    isAura = false;
    onGcd = true;
    isProc = false;
    isFinisher = false;
    castTime = 0;
}

void Spell::reset()
{
    cooldownRemaining = 0;
    casting = false;
}

void Spell::setup()
{
    if (minDmg > 0 && maxDmg > 0)
    {
      dmg = (minDmg + maxDmg) / 2;
    }
    if (minMana > 0 && maxMana > 0)
    {
      avgManaValue = (minMana + maxMana) / 2;
    }
    if (player->combatLogBreakdown.count(name) == 0)
    {
        player->combatLogBreakdown.insert(std::make_pair(name, new CombatLogBreakdown(name)));
    }
}

double Spell::getCastTime()
{
    return round((castTime / player->getHastePercent()) * 10000) / 10000 + player->spellDelay;
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
        // Error: Casting a spell while GCD is active
        if (player->gcdRemaining > 0)
        {
            player->throwError("Attempting to cast " + name + " while player's GCD is at " + std::to_string(player->gcdRemaining) + " seconds remaining");
        }
        player->gcdRemaining = player->getGcdValue(shared_from_this());
    }

    // Error: Starting to cast a spell while casting another spell
    if (player->castTimeRemaining > 0 && !isNonWarlockAbility && !isProc)
    {
        player->throwError("Attempting to cast " + name + " while player's cast time remaining is at " + std::to_string(player->castTimeRemaining) + " sec");
    }

    // Error: Casting a spell while it's on cooldown
    if (cooldown > 0 && cooldownRemaining > 0)
    {
        player->throwError("Attempting to cast " + name + " while it's still on cooldown (" + std::to_string(cooldownRemaining) + " seconds remaining)");
    }

    std::string combatLogMsg = "";
    if (castTime > 0)
    {
        casting = true;
        player->castTimeRemaining = getCastTime();
        if (!isProc && player->shouldWriteToCombatLog())
        {
            combatLogMsg.append("Started casting " + name + " - Cast time: " + truncateTrailingZeros(std::to_string(player->castTimeRemaining - player->spellDelay), 4) + " (" + truncateTrailingZeros(std::to_string((player->getHastePercent() - 1) * 100), 4) + "% haste at a base cast speed of " + truncateTrailingZeros(std::to_string(castTime), 2) + ")");
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
        combatLogMsg.append(" - Global cooldown: " + truncateTrailingZeros(std::to_string(player->gcdRemaining), 4));
    }
    if (predictedDamage > 0 && player->shouldWriteToCombatLog())
    {
        combatLogMsg.append(" - Estimated damage / Cast time: " + truncateTrailingZeros(std::to_string(round(predictedDamage))));
    }

    if (player->shouldWriteToCombatLog())
    {
        player->combatLog(combatLogMsg);
    }
}

void Spell::tick(double t)
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
    int currentMana = player->stats->mana;
    bool isCrit = false;
    cooldownRemaining = cooldown;
    casting = false;

    if (!isAura)
    {
        player->combatLogBreakdown.at(name)->casts++;
    }

    if (manaCost > 0 && !player->settings->infinitePlayerMana)
    {
        player->stats->mana -= manaCost * player->stats->manaCostModifier;
        player->fiveSecondRuleTimer = 5;
    }

    if (castTime > 0 && player->shouldWriteToCombatLog())
    {
        std::string msg = "Finished casting " + name + " - Mana: " + truncateTrailingZeros(std::to_string(currentMana)) + " -> " + truncateTrailingZeros(std::to_string(player->stats->mana)) + " - Mana Cost: " + truncateTrailingZeros(std::to_string(round(manaCost))) + " - Mana Cost Modifier: " + truncateTrailingZeros(std::to_string(round(player->stats->manaCostModifier * 100))) + "%";
        player->combatLog(msg);
    }

    if (canCrit)
    {
        isCrit = player->isCrit(type, bonusCrit);
        if (isCrit)
        {
            // Increment the crit counter whether the spell hits or not so that the crit % on the damage breakdown is correct. Otherwise the crit % will be lower due to lost crits when the spell misses.
            player->combatLogBreakdown.at(name)->crits++;
        }
    }

    if (((!isItem && !isNonWarlockAbility && (!player->auras->AmplifyCurse || name != player->auras->AmplifyCurse->name)) || doesDamage) && !player->isHit(type))
    {
        if (player->shouldWriteToCombatLog())
        {
            std::string msg = name + " *resist*";
            player->combatLog(msg);
        }
        player->combatLogBreakdown.at(name)->misses++;
        return;
    }

    if (isAura)
    {
        auraEffect->apply();
    }
    if (isDot)
    {
        dotEffect->apply();
    }

    if (doesDamage)
    {
        damage(isCrit);
    }

    // If it's an item such as mana potion, demonic rune, destruction potion, or if it's a proc with a hidden cooldown like Blade of Wizardry or Robe of the Elder Scribes then don't check for on-hit procs
    if (!isItem && !isProc && !isNonWarlockAbility && (!player->auras->AmplifyCurse || name != player->auras->AmplifyCurse->name))
    {
        onHitProcs();
    }
}

double Spell::getModifier()
{
    double dmgModifier = modifier;
    if (school == SpellSchool::SHADOW)
    {
        dmgModifier *= player->stats->shadowModifier;
        
        // Improved Shadow Bolt
        if (!player->settings->usingCustomIsbUptime && player->auras->ImprovedShadowBolt != NULL && player->auras->ImprovedShadowBolt->active)
        {
            dmgModifier *= player->auras->ImprovedShadowBolt->modifier;
        }
    }
    else if (school == SpellSchool::FIRE)
    {
        dmgModifier *= player->stats->fireModifier;
    }

    return dmgModifier;
}

void Spell::damage(bool isCrit)
{
    std::vector<double> constantDamage = getConstantDamage();
    double baseDamage = constantDamage[0];
    double totalDamage = constantDamage[1];
    double dmgModifier = constantDamage[2];
    double partialResistMultiplier = constantDamage[3];
    double spellPower = constantDamage[4];
    double critMultiplier = player->critMultiplier;
    
    if (isCrit)
    {
        critMultiplier = getCritMultiplier(critMultiplier);
        totalDamage *= critMultiplier;
        onCritProcs();
    }
    else if (school == SpellSchool::SHADOW && !isDot && player->auras->ImprovedShadowBolt != NULL && player->auras->ImprovedShadowBolt->active && !player->settings->usingCustomIsbUptime)
    {
        player->auras->ImprovedShadowBolt->decrementStacks();
    }

    onDamageProcs();
    player->iterationDamage += totalDamage;

    // Combat Log
    player->addIterationDamageAndMana(name, 0, totalDamage);
    if (player->shouldWriteToCombatLog())
    {
        std::string msg = name + " ";
        if (isCrit)
        {
            msg += "*";
        }
        msg += truncateTrailingZeros(std::to_string(round(totalDamage)));
        if (isCrit)
        {
            msg += "*";
        }
        msg += " (" + truncateTrailingZeros(std::to_string(dmg), 1) + " Base Damage - " + truncateTrailingZeros(std::to_string(round(coefficient * 1000) / 1000), 3) + " Coefficient - " + truncateTrailingZeros(std::to_string(round(spellPower))) + " Spell Power - ";
        if (isCrit)
        {
            msg += truncateTrailingZeros(std::to_string(critMultiplier * 100), 3) + "% Crit Multiplier - ";
        }
        msg += truncateTrailingZeros(std::to_string(round(dmgModifier * 10000) / 100), 2) + "% Damage Modifier - " + truncateTrailingZeros(std::to_string(round(partialResistMultiplier * 1000) / 10)) + "% Partial Resist Multiplier)";
        player->combatLog(msg);
    }

    //T5 4pc
    if (player->sets->t5 >= 4)
    {
        if (player->spells->ShadowBolt != NULL && name == player->spells->ShadowBolt->name && player->auras->Corruption != NULL && player->auras->Corruption->active)
        {
            player->auras->Corruption->t5BonusModifier *= 1.1;
        }
        else if (player->spells->Incinerate != NULL && name == player->spells->Incinerate->name && player->auras->Immolate != NULL && player->auras->Immolate->active)
        {

            player->auras->Immolate->t5BonusModifier *= 1.1;
        }
    }
}

// Returns the non-RNG damage of the spell (basically just the base damage + spell power + damage modifiers, no crit/miss etc.)
// todo: investigate this noRng variable
std::vector<double> Spell::getConstantDamage(bool noRng)
{
    double totalDmg = player->settings->randomizeValues && minDmg > 0 && maxDmg > 0 && !noRng ? random(minDmg, maxDmg) : dmg;
    double baseDamage = totalDmg;
    double spellPower = player->getSpellPower(school);
    double dmgModifier = getModifier();
    double partialResistMultiplier = player->getPartialResistMultiplier(school);

    // If casting Incinerate and Immolate is up, add the bonus damage
    if (player->spells->Incinerate != NULL && name == player->spells->Incinerate->name && player->auras->Immolate != NULL && player->auras->Immolate->active)
    {
        totalDmg += player->settings->randomizeValues && noRng ? random(bonusDamageFromImmolateMin, bonusDamageFromImmolateMax) : bonusDamageFromImmolate;
    }
    // Add damage from Spell Power
    totalDmg += spellPower * coefficient;
    // Modifier & Partial Resist
    totalDmg *= dmgModifier;
    totalDmg *= partialResistMultiplier;

    return std::vector<double> {baseDamage, totalDmg  , dmgModifier, partialResistMultiplier, spellPower};
}

double Spell::getCritMultiplier(double critMult)
{
    double critMultiplier = critMult;

    // Chaotic Skyfire Diamond
    if (player->settings->metaGemId == 34220)
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

double Spell::predictDamage()
{
    std::vector<double> constantDamage = getConstantDamage();
    double normalDamage = constantDamage[1];
    double critDamage = 0;
    double critChance = 0;
    double chanceToNotCrit = 0;

    if (canCrit)
    {
        critDamage = normalDamage * getCritMultiplier(player->critMultiplier);
        critChance = player->getCritChance(type) / 100;
        chanceToNotCrit = 1 - critChance;
    }

    double hitChance = player->getHitChance(type) / 100;
    double estimatedDamage = canCrit ? (normalDamage * chanceToNotCrit) + (critDamage * critChance) : normalDamage;

    // Add the predicted damage of the DoT over its full duration
    if (isDot)
    {
        estimatedDamage += dotEffect->predictDamage();
    }

    // If the player is not using a custom ISB uptime, they have the ISB talent selected, but the ISB aura is not active, then give some % modifier as an "average" for the damage.
    // Without this, the sim will choose Incinerate over Shadow Bolt because it basically just doesn't know that ISB exists
    if (school == SpellSchool::SHADOW && !player->settings->usingCustomIsbUptime && player->auras->ImprovedShadowBolt != NULL && !player->auras->ImprovedShadowBolt->active)
    {
        // If this isn't the player's first iteration then check what their ISB uptime is and add that %
        if (player->iteration > 1)
        {
            estimatedDamage *= (1 + 0.2 * player->auras->ImprovedShadowBolt->uptimeSoFar);
        }
        // If it's the first iteration where we don't have enough data to assume what the player's ISB uptime is, then add a fixed amount
        else
        {
            estimatedDamage *= 1.15;
        }
    }

    return (estimatedDamage * hitChance) / std::max(player->getGcdValue(shared_from_this()), getCastTime());
}

void Spell::onCritProcs()
{
    // ISB
    if (player->spells->ShadowBolt != NULL && name == player->spells->ShadowBolt->name && !player->settings->usingCustomIsbUptime && player->talents->improvedShadowBolt > 0)
    {
        player->auras->ImprovedShadowBolt->apply();
    }
    // The Lightning Capacitor
    if (player->spells->TheLightningCapacitor != NULL)
    {
        player->spells->TheLightningCapacitor->startCast();
    }
    // Sextant of Unstable Currents
    if (player->spells->SextantOfUnstableCurrents != NULL && player->spells->SextantOfUnstableCurrents->ready() && player->getRand() <= player->spells->SextantOfUnstableCurrents->procChance * player->critChanceMultiplier)
    {
        player->spells->SextantOfUnstableCurrents->startCast();
    }
    // Shiffar's Nexus-Horn
    if (player->spells->ShiffarsNexusHorn != NULL && player->spells->ShiffarsNexusHorn->ready() && player->getRand() <= player->spells->ShiffarsNexusHorn->procChance * player->critChanceMultiplier)
    {
        player->spells->ShiffarsNexusHorn->startCast();
    }
}

void Spell::onDamageProcs()
{
    // Confirm that this procs on dealing damage
    // Shattered Sun Pendant of Acumen
    if (player->settings->exaltedWithShattrathFaction && player->spells->ShatteredSunPendantOfAcumen != NULL && player->spells->ShatteredSunPendantOfAcumen->cooldownRemaining <= 0 && player->getRand() <= player->spells->ShatteredSunPendantOfAcumen->procChance * player->critChanceMultiplier)
    {
        player->spells->ShatteredSunPendantOfAcumen->startCast();
    }
}

void Spell::onHitProcs()
{
    // Judgement of Wisdom (50% proc rate)
    if (player->selectedAuras->judgementOfWisdom && player->getRand() <= 50 * player->critChanceMultiplier)
    {
        int manaVal = 74;
        int currentMana = player->stats->mana;
        int manaGained = std::min(player->stats->maxMana - currentMana, manaVal);
        player->stats->mana += manaGained;
        player->combatLogBreakdown.at("Judgement of Wisdom")->casts++;
        player->addIterationDamageAndMana("Judgement of Wisdom", manaGained, 0);
        if (player->shouldWriteToCombatLog())
        {
            player->combatLog("Player gains " + std::to_string(manaGained) + " mana from Judgement of Wisdom (" + std::to_string(currentMana) + " -> " + std::to_string(player->stats->mana) + ")");
        }
    }
    // T4 2pc
    if (player->sets->t4 >= 2 && (school == SpellSchool::SHADOW || school == SpellSchool::FIRE) && player->getRand() <= player->auras->Flameshadow->procChance * player->critChanceMultiplier)
    {
        if (school == SpellSchool::SHADOW)
        {
            player->auras->Flameshadow->apply();
        }
        else if (school == SpellSchool::FIRE)
        {
            player->auras->Shadowflame->apply();
        }
    }
    // Spellstrike
    if (player->sets->spellstrike == 2 && player->getRand() <= player->auras->Spellstrike->procChance * player->critChanceMultiplier)
    {
        player->auras->Spellstrike->apply();
    }
    // Quagmirran's Eye
    if (player->spells->QuagmirransEye != NULL && player->spells->QuagmirransEye->ready() && player->getRand() <= player->spells->QuagmirransEye->procChance * player->critChanceMultiplier)
    {
        player->spells->QuagmirransEye->startCast();
    }
    // Mana-Etched Regalia 4pc
    if (player->sets->manaEtched >= 4 && player->getRand() <= player->auras->ManaEtched4Set->procChance * player->critChanceMultiplier)
    {
        player->auras->ManaEtched4Set->apply();
    }
    // Mark of Defiance
    if (player->spells->MarkOfDefiance != NULL && player->spells->MarkOfDefiance->ready() && player->getRand() <= player->spells->MarkOfDefiance->procChance * player->critChanceMultiplier)
    {
        player->spells->MarkOfDefiance->startCast();
    }
    // Darkmoon Card: Crusade
    if (player->auras->DarkmoonCardCrusade != NULL)
    {
        player->auras->DarkmoonCardCrusade->apply();
    }
    // Band of the Eternal Sage
    if (player->spells->BandOfTheEternalSage != NULL && player->spells->BandOfTheEternalSage->ready() && player->getRand() <= player->spells->BandOfTheEternalSage->procChance * player->critChanceMultiplier)
    {
        player->spells->BandOfTheEternalSage->startCast();
    }
    // Blade of Wizardry
    if (player->spells->BladeOfWizardry != NULL && player->spells->BladeOfWizardry->ready() && player->getRand() <= player->auras->BladeOfWizardry->procChance * player->critChanceMultiplier)
    {
        player->spells->BladeOfWizardry->startCast();
    }
    // Mystical Skyfire Diamond
    if (player->spells->MysticalSkyfireDiamond != NULL && player->spells->MysticalSkyfireDiamond->ready() && player->getRand() <= player->spells->MysticalSkyfireDiamond->procChance * player->critChanceMultiplier)
    {
        player->spells->MysticalSkyfireDiamond->startCast();
    }
    // Robe of the Elder Scribes
    if (player->spells->RobeOfTheElderScribes != NULL && player->spells->RobeOfTheElderScribes->ready() && player->getRand() <= player->spells->RobeOfTheElderScribes->procChance * player->critChanceMultiplier)
    {
        player->spells->RobeOfTheElderScribes->startCast();
    }
    // Insightful Earthstorm Diamond
    if (player->spells->InsightfulEarthstormDiamond != NULL && player->spells->InsightfulEarthstormDiamond->ready() && player->getRand() <= player->spells->InsightfulEarthstormDiamond->procChance * player->critChanceMultiplier)
    {
        player->spells->InsightfulEarthstormDiamond->startCast();
    }
    // Wrath of Cenarius
    if (player->auras->WrathOfCenarius != NULL && player->getRand() <= player->auras->WrathOfCenarius->procChance * player->critChanceMultiplier)
    {
        player->auras->WrathOfCenarius->apply();
    }
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
    bool hasShadowTrance = player->auras->ShadowTrance != NULL;

    if (hasShadowTrance && player->auras->ShadowTrance->active)
    {
        castTime = 0;
    }

    Spell::startCast();
    
    if (hasShadowTrance && player->auras->ShadowTrance->active)
    {
        castTime = calculateCastTime();
        player->auras->ShadowTrance->fade();
    }
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
    setup();
}

int LifeTap::manaGain()
{
    return (manaReturn + ((player->getSpellPower() + player->stats->shadowPower) * coefficient)) * modifier;
}

void LifeTap::cast()
{
    const int manaGain = this->manaGain();
    player->combatLogBreakdown.at(name)->casts++;
    player->addIterationDamageAndMana(name, manaGain, 0);
    
    if (player->shouldWriteToCombatLog() && player->stats->mana + manaGain > player->stats->maxMana)
    {
        player->combatLog("Life Tap used at too high mana (mana wasted)");
    }
    player->stats->mana = std::min(player->stats->maxMana, player->stats->mana + manaGain);
}

Incinerate::Incinerate(Player* player) : Spell(player)
{
    name = "Incinerate";
    castTime = round((2.5 * (1 - 0.02 * player->talents->emberstorm)) * 100) / 100;
    manaCost = 355 * (1 - 0.01 * player->talents->cataclysm);
    coefficient = (2.5 / 3.5) + (0.04 * player->talents->shadowAndFlame);
    minDmg = 444;
    maxDmg = 514;
    bonusDamageFromImmolateMin = 111;
    bonusDamageFromImmolateMax = 128;
    bonusDamageFromImmolate = (bonusDamageFromImmolateMin + bonusDamageFromImmolateMax) / 2;
    doesDamage = true;
    canCrit = true;
    school = SpellSchool::FIRE;
    type = SpellType::DESTRUCTION;
    setup();

    if (player->sets->t6 >= 4)
    {
        modifier *= 1.06;
    }
}

SearingPain::SearingPain(Player* player) : Spell(player)
{
    name = "Searing Pain";
    castTime = 1.5;
    manaCost = 205 * (1 - 0.01 * player->talents->cataclysm);
    coefficient = 1.5 / 3.5;
    minDmg = 270;
    maxDmg = 320;
    doesDamage = true;
    canCrit = true;
    school = SpellSchool::FIRE;
    type = SpellType::DESTRUCTION;
    bonusCrit = 4 * player->talents->improvedSearingPain;
    setup();
};

SoulFire::SoulFire(Player* player) : Spell(player)
{
    name = "Soul Fire";
    castTime = 6 - (0.4 * player->talents->bane);
    manaCost = 250 * (1 - 0.01 * player->talents->cataclysm);
    coefficient = 1.15;
    minDmg = 1003;
    maxDmg = 1257;
    doesDamage = true;
    canCrit = true;
    school = SpellSchool::FIRE;
    type = SpellType::DESTRUCTION;
    setup();
};

Shadowburn::Shadowburn(Player* player) : Spell(player)
{
    name = "Shadowburn";
    cooldown = 15;
    manaCost = 515 * (1 - 0.01 * player->talents->cataclysm);
    coefficient = 0.22;
    minDmg = 597;
    maxDmg = 665;
    doesDamage = true;
    canCrit = true;
    isFinisher = true;
    school = SpellSchool::SHADOW;
    type = SpellType::DESTRUCTION;
    setup();
};

DeathCoil::DeathCoil(Player* player) : Spell(player)
{
    name = "Death Coil";
    cooldown = 120;
    manaCost = 600;
    coefficient = 0.4286;
    dmg = 526;
    doesDamage = true;
    isFinisher = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
};

Shadowfury::Shadowfury(Player* player) : Spell(player)
{
    name = "Shadowfury";
    castTime = 0.5;
    manaCost = 710 * (1 - 0.01 * player->talents->cataclysm);
    minDmg = 612;
    maxDmg = 728;
    doesDamage = true;
    canCrit = true;
    school = SpellSchool::SHADOW;
    type = SpellType::DESTRUCTION;
    cooldown = 20;
    coefficient = 0.195;
    setup();
}

SeedOfCorruption::SeedOfCorruption(Player* player) : Spell(player)
{
    name = "Seed of Corruption";
    minDmg = 1110;
    maxDmg = 1290;
    manaCost = 882;
    castTime = 2;
    dmgCap = 13580;
    doesDamage = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    coefficient = 0.214;
    setup();
};

void SeedOfCorruption::damage(bool isCrit)
{
    int baseDamage = player->settings->randomizeValues && minDmg && maxDmg ? random(minDmg, maxDmg) : dmg;
    int enemyAmount = player->settings->enemyAmount - 1; // Minus one because the enemy that Seed is being cast on doesn't get hit
    int resistAmount = 0;
    int critAmount = 0;
    int spellPower = player->getSpellPower(school);
    double modifier = getModifier(); //todo debuffs increase dmg past the aoe cap
    double critMultiplier = 0;

    for (int i = 0; i < enemyAmount; i++)
    {
        // Check for a resist
        if (!player->isHit(type))
        {
            resistAmount++;
            continue;
        }
        else
        {
            onDamageProcs();
        }
        // Check for a crit
        if (player->isCrit(type))
        {
            critAmount++;
            onCritProcs();
        }
    }

    double individualSeedDamage = baseDamage + (spellPower * coefficient);
    // Oblivion Raiment (dungeon set) 4pc bonus
    if (player->sets->oblivion >= 4)
    {
        individualSeedDamage += 180;
    }
    individualSeedDamage *= modifier;

    int enemiesHit = enemyAmount - resistAmount;
    double totalSeedDamage = individualSeedDamage * enemiesHit;
    // If the total damage goes above the aoe cap then we need to reduce the amount of each seed's damage
    if (totalSeedDamage > dmgCap)
    {
        // Set the damage of each individual seed to the aoe cap divided by the amount of enemies hit
        // There's a bug with Seed of Corruption where if you hit the AoE cap,
        // the number used to divide here is 1 higher because it's including the enemy that Seed is being cast on,
        // even though that enemy doesn't actually get damaged by the Seed. Nice game :)
        individualSeedDamage = dmgCap / (enemiesHit + 1);
        // Re-calculate the total damage done by all seed hits
        totalSeedDamage = individualSeedDamage * enemiesHit;
    }
    // Add damage from Seed crits
    if (critAmount > 0)
    {
        critMultiplier = getCritMultiplier(player->critMultiplier);
        double individualSeedCrit = individualSeedDamage * critMultiplier;
        double bonusDamageFromCrit = individualSeedCrit - individualSeedDamage;
        totalSeedDamage += bonusDamageFromCrit * critAmount;
    }
    // Partial resists (probably need to calculate a partial resist for each seed hit, not sure how it interacts for the aoe cap)
    double partialResistMultiplier = player->getPartialResistMultiplier(school);
    totalSeedDamage *= partialResistMultiplier;

    player->iterationDamage += totalSeedDamage;

    if (player->shouldWriteToCombatLog())
    {
        std::string msg = name + " " + truncateTrailingZeros(std::to_string(round(totalSeedDamage))) + " (" + std::to_string(enemyAmount) + " Enemies (" + std::to_string(resistAmount) + " Resists & " + std::to_string(critAmount) + " Crits) - " + std::to_string(baseDamage) + " Base Damage - " + truncateTrailingZeros(std::to_string(coefficient), 3) + " Coefficient - " + std::to_string(spellPower) + " Spell Power - " + truncateTrailingZeros(std::to_string(round(modifier * 1000) / 10), 1) + "% Modifier - ";
        if (critAmount > 0)
        {
            msg += truncateTrailingZeros(std::to_string(critMultiplier), 3) + "% Crit Multiplier";
        }
        msg += " - " + truncateTrailingZeros(std::to_string(round(partialResistMultiplier * 1000) / 10)) + "% Partial Resist Multiplier)";
        player->combatLog(msg);
    }
    player->addIterationDamageAndMana(name, 0, totalSeedDamage);
    player->combatLogBreakdown.at(name)->crits += critAmount;
    player->combatLogBreakdown.at(name)->misses += resistAmount;
    // the cast() function already adds 1 to the amount of casts so we only need to add enemiesHit - 1 to the cast amount
    player->combatLogBreakdown.at(name)->casts += (enemiesHit - 1);
}

double SeedOfCorruption::getModifier()
{
    double modifier = Spell::getModifier();
    if (player->talents->shadowMastery > 0 && player->talents->contagion > 0)
    {
        // Divide away the bonus from Shadow Mastery
        modifier /= (1 + (player->talents->shadowMastery * 0.02));
        // Multiply the modifier with the bonus from Shadow Mastery + Contagion
        modifier *= (1 * (1 + ((player->talents->shadowMastery * 0.02) + (player->talents->contagion / 100.0))));
    }
    return modifier;
}

DarkPact::DarkPact(Player* player) : Spell(player)
{
    name = "Dark Pact";
    manaReturn = 700;
    coefficient = 0.96;
    setup();
}

double DarkPact::manaGain()
{
    return 0;
}

bool DarkPact::ready()
{
    return false;
}

void DarkPact::cast()
{

}

Corruption::Corruption(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot) : Spell(player, aura, dot)
{
    name = "Corruption";
    manaCost = 370;
    castTime = round((2 - (0.4 * player->talents->improvedCorruption)) * 100) / 100.0;
    isDot = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
}

UnstableAffliction::UnstableAffliction(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot) : Spell(player, aura, dot)
{
    name = "Unstable Affliction";
    manaCost = 400;
    castTime = 1.5;
    isDot = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
}

SiphonLife::SiphonLife(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot) : Spell(player, aura, dot)
{
    name = "Siphon Life";
    manaCost = 410;
    isDot = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
}

Immolate::Immolate(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot) : Spell(player, aura, dot)
{
    name = "Immolate";
    manaCost = 445 * (1 - 0.01 * player->talents->cataclysm);
    castTime = 2 - (0.1 * player->talents->bane);
    isDot = true;
    doesDamage = true;
    canCrit = true;
    dmg = 331;
    coefficient = 0.2;
    school = SpellSchool::FIRE;
    type = SpellType::DESTRUCTION;
    setup();
}

double Immolate::getModifier()
{
    double modifier = Spell::getModifier();
    if (player->talents->emberstorm > 0)
    {
        modifier /= (1 + 0.02 * player->talents->emberstorm);
    }
    modifier *= (1 + (0.02 * player->talents->emberstorm + 0.05 * player->talents->improvedImmolate));
    return modifier;
}

CurseOfAgony::CurseOfAgony(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot) : Spell(player, aura, dot)
{
    name = "Curse of Agony";
    manaCost = 265;
    isDot = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
}

CurseOfTheElements::CurseOfTheElements(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Curse of the Elements";
    manaCost = 260;
    type = SpellType::AFFLICTION;
    isAura = true;
    setup();
}

CurseOfRecklessness::CurseOfRecklessness(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Curse of Recklessness";
    manaCost = 160;
    type = SpellType::AFFLICTION;
    isAura = true;
    setup();
}

CurseOfDoom::CurseOfDoom(Player* player, std::shared_ptr<Aura> aura, std::shared_ptr<DamageOverTime> dot) : Spell(player, aura, dot)
{
    name = "Curse of Doom";
    manaCost = 380;
    cooldown = 60;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    isDot = true;
    setup();
}

Conflagrate::Conflagrate(Player* player) : Spell(player)
{
    name = "Conflagrate";
    manaCost = 305 * (1 - 0.01 * player->talents->cataclysm);
    cooldown = 10;
    minDmg = 579;
    maxDmg = 721;
    coefficient = 1.5 / 3.5;
    doesDamage = true;
    isFinisher = true;
    canCrit = true;
    school = SpellSchool::FIRE;
    type = SpellType::DESTRUCTION;
    setup();
}

void Conflagrate::startCast()
{
    /*if (player->auras->Immolate != NULL && player->auras->immolate->active)
    {
        Spell::startCast();
        player->auras->immolate->active = false;
    }*/
}

DestructionPotion::DestructionPotion(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Destruction Potion";
    cooldown = 120;
    isItem = true;
    isAura = true;
    onGcd = false;
    setup();
}

SuperManaPotion::SuperManaPotion(Player* player) : Spell(player)
{
    name = "Super Mana Potion";
    cooldown = 120;
    isItem = true;
    minMana = 1800;
    maxMana = 3000;
    onGcd = false;
    setup();
}

void SuperManaPotion::cast()
{
    Spell::cast();
    const int currentPlayerMana = player->stats->mana;
    //todo check for the randomize values option
    const int manaGain = random(minMana, maxMana);
    player->addIterationDamageAndMana(name, manaGain, 0);
    player->stats->mana = std::min(player->stats->maxMana, currentPlayerMana + manaGain);
    if (player->shouldWriteToCombatLog())
    {
        std::string msg = "Player gains " + truncateTrailingZeros(std::to_string(round(player->stats->mana - currentPlayerMana))) + " mana from " + name + " (" + truncateTrailingZeros(std::to_string(round(currentPlayerMana))) + " -> " + truncateTrailingZeros(std::to_string(round(player->stats->mana))) + ")";
        player->combatLog(msg);
    }
}

DemonicRune::DemonicRune(Player* player) : Spell(player)
{
    name = "Demonic Rune";
    cooldown = 120;
    isItem = true;
    minMana = 900;
    maxMana = 1500;
    onGcd = false;
    setup();
}

void DemonicRune::cast()
{
    Spell::cast();
    const int currentPlayerMana = player->stats->mana;
    //todo check for the randomize values option
    const int manaGain = random(minMana, maxMana);
    player->addIterationDamageAndMana(name, manaGain, 0);
    player->stats->mana = std::min(player->stats->maxMana, currentPlayerMana + manaGain);
    if (player->shouldWriteToCombatLog())
    {
        std::string msg = "Player gains " + truncateTrailingZeros(std::to_string(round(player->stats->mana - currentPlayerMana))) + " mana from " + name + " (" + truncateTrailingZeros(std::to_string(round(currentPlayerMana))) + " -> " + truncateTrailingZeros(std::to_string(round(player->stats->mana))) + ")";
        player->combatLog(msg);
    }
}

FlameCap::FlameCap(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Flame Cap";
    cooldown = 180;
    isItem = true;
    isAura = true;
    onGcd = false;
    setup();
}

BloodFury::BloodFury(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Blood Fury";
    cooldown = 120;
    isAura = true;
    onGcd = false;
    setup();
}

Bloodlust::Bloodlust(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Bloodlust";
    cooldown = 600;
    isItem = true;
    isAura = true;
    onGcd = false;
    isNonWarlockAbility = true;
    setup();
}

DrumsOfBattle::DrumsOfBattle(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Drums of Battle";
    cooldown = 120;
    isAura = true;
    onGcd = false;
    isNonWarlockAbility = true;
    isItem = true;
    setup();
}

bool DrumsOfBattle::ready()
{
    return cooldownRemaining <= 0;
}

DrumsOfWar::DrumsOfWar(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Drums of War";
    cooldown = 120;
    isAura = true;
    onGcd = false;
    isNonWarlockAbility = true;
    isItem = true;
    setup();
}

bool DrumsOfWar::ready()
{
    return cooldownRemaining <= 0;
}

DrumsOfRestoration::DrumsOfRestoration(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Drums of Restoration";
    cooldown = 120;
    isAura = true;
    onGcd = false;
    isNonWarlockAbility = true;
    isItem = true;
    setup();
}

bool DrumsOfRestoration::ready()
{
    return cooldownRemaining <= 0;
}

TimbalsFocusingCrystal::TimbalsFocusingCrystal(Player* player) : Spell(player)
{
    name = "Timbal's Focusing Crystal";
    cooldown = 15;
    onGcd = false;
    procChance = 10;
    minDmg = 285;
    maxDmg = 475;
    doesDamage = true;
    isProc = true;
    school = SpellSchool::SHADOW;
    canCrit = true;
    setup();
}

MarkOfDefiance::MarkOfDefiance(Player* player) : Spell(player)
{
    name = "Mark of Defiance";
    cooldown = 17;
    procChance = 15;
    onGcd = false;
    minMana = 128;
    maxMana = 172;
    setup();
}

void MarkOfDefiance::cast()
{
    if (cooldownRemaining <= 0)
    {
        const int currentPlayerMana = player->stats->mana;
        player->stats->mana = std::min(static_cast<double>(player->stats->maxMana), currentPlayerMana + avgManaValue);
        if (player->shouldWriteToCombatLog())
        {
            std::string msg = "Player gains " + truncateTrailingZeros(std::to_string(round(player->stats->mana - currentPlayerMana))) + " mana from " + name + " (" + truncateTrailingZeros(std::to_string(round(currentPlayerMana))) + " -> " + truncateTrailingZeros(std::to_string(round(player->stats->mana))) + ")";
            player->combatLog(msg);
        }
        cooldownRemaining = cooldown;
    }
}

TheLightningCapacitor::TheLightningCapacitor(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "The Lightning Capacitor";
    cooldown = 2.5;
    minDmg = 694;
    maxDmg = 806;
    doesDamage = true;
    canCrit = true;
    onGcd = false;
    setup();
}

void TheLightningCapacitor::startCast(double predictedDamage)
{
    if (cooldownRemaining <= 0)
    {
        player->auras->TheLightningCapacitor->apply();
        if (player->auras->TheLightningCapacitor->stacks == 3)
        {
            Spell::startCast();
            cooldownRemaining = cooldown;
            player->auras->TheLightningCapacitor->fade();
        }
    }
}

BladeOfWizardry::BladeOfWizardry(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Blade of Wizardry";
    cooldown = 50;
    onGcd = false;
    isItem = true;
    isProc = true;
    isAura = true;
    setup();
}

ShatteredSunPendantOfAcumen::ShatteredSunPendantOfAcumen(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Shattered Sun Pendant of Acumen";
    cooldown = 45;
    procChance = 15;
    onGcd = false;
    isItem = true;
    if (player->settings->isAldor)
    {
        this->isProc = true;
        this->isAura = true;
    }
    else
    {
        this->doesDamage = true;
        this->canCrit = true;
        this->dmg = 333; // confirm
    }
    setup();
}

RobeOfTheElderScribes::RobeOfTheElderScribes(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Robe of the Elder Scribes";
    cooldown = 50;
    procChance = 20;
    onGcd = false;
    isItem = true;
    isProc = true;
    isAura = true;
    setup();
}

QuagmirransEye::QuagmirransEye(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Quagmirran's Eye";
    cooldown = 45;
    procChance = 10;
    onGcd = false;
    isItem = true;
    isAura = true;
    setup();
}

ShiffarsNexusHorn::ShiffarsNexusHorn(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Shiffar's Nexus-Horn";
    cooldown = 45;
    procChance = 20;
    onGcd = false;
    isItem = true;
    isAura = true;
    setup();
}

SextantOfUnstableCurrents::SextantOfUnstableCurrents(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Sextant of Unstable Currents";
    cooldown = 45;
    procChance = 20;
    onGcd = false;
    isItem = true;
    isAura = true;
    setup();
}

BandOfTheEternalSage::BandOfTheEternalSage(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Band of the Eternal Sage";
    cooldown = 60;
    procChance = 10;
    onGcd = false;
    isItem = true;
    isAura = true;
    setup();
}

MysticalSkyfireDiamond::MysticalSkyfireDiamond(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Mystical Skyfire Diamond";
    cooldown = 35;
    procChance = 15;
    onGcd = false;
    isProc = true;
    isItem = true;
    isAura = true;
    setup();
}

InsightfulEarthstormDiamond::InsightfulEarthstormDiamond(Player* player) : Spell(player)
{
    name = "Insightful Earthstorm Diamond";
    cooldown = 15;
    procChance = 5;
    onGcd = false;
    isProc = true;
    isItem = true;
    manaGain = 300;
    setup();
}

void InsightfulEarthstormDiamond::cast()
{
    Spell::cast();
    const int currentPlayerMana = player->stats->mana;
    player->stats->mana = std::min(static_cast<double>(player->stats->maxMana), currentPlayerMana + manaGain);
    if (player->shouldWriteToCombatLog())
    {
        std::string msg = "Player gains " + std::to_string(round(player->stats->mana - currentPlayerMana)) + " mana from " + name + " (" + std::to_string(round(currentPlayerMana)) + " -> " + std::to_string(round(player->stats->mana)) + ")";
        player->combatLog(msg);
    }
}

AmplifyCurse::AmplifyCurse(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Amplify Curse";
    cooldown = 180;
    isAura = true;
    onGcd = false;
    setup();
}

PowerInfusion::PowerInfusion(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Power Infusion";
    cooldown = 180;
    isAura = true;
    onGcd = false;
    isNonWarlockAbility = true;
    setup();
}

Innervate::Innervate(Player* player, std::shared_ptr<Aura> aura) : Spell(player, aura)
{
    name = "Innervate";
    cooldown = 360;
    isAura = true;
    onGcd = false;
    isNonWarlockAbility = true;
    setup();
}