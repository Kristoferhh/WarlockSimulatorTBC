#include "spell.h"
#include "player.h"
#include "common.h"
#include <iomanip>

Spell::Spell(Player* player, Aura* aura, DamageOverTime* dot) : player(player), auraEffect(aura), dotEffect(dot)
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
}

void Spell::reset()
{
    cooldownRemaining = 0;
    casting = false;
}

void Spell::setup()
{
    varName = camelCase(name);
    if (minDmg > 0 && maxDmg > 0)
    {
      dmg = (minDmg + maxDmg) / 2;
    }
    if (minMana > 0 && maxMana > 0)
    {
      avgManaValue = (minMana + maxMana) / 2;
    }
    if (player->combatLogBreakdown.count(varName) == 0)
    {
        player->combatLogBreakdown.insert(std::make_pair(varName, new CombatLogBreakdown(name)));
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
        player->gcdRemaining = player->getGcdValue(varName);
    }

    std::string combatLogMsg = "";
    if (castTime > 0)
    {
        casting = true;
        player->castTimeRemaining = getCastTime();
        if (!isProc && player->shouldWriteToCombatLog())
        {
            combatLogMsg.append("Started casting " + name + " - Cast time: " + truncateTrailingZeros(std::to_string(player->castTimeRemaining - player->spellDelay), 4) + " (" + truncateTrailingZeros(std::to_string(player->getHastePercent() * 100), 4) + "% haste at a base cast speed of " + truncateTrailingZeros(std::to_string(castTime), 2) + ")");
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
        combatLogMsg.append(" - Estimated damage / Cast time: " + std::to_string(round(predictedDamage)));
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
    if (!isAura)
    {
        player->combatLogBreakdown.at(varName)->casts++;
    }   
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
        std::string msg = "Finished casting " + name + " - Mana: " + truncateTrailingZeros(std::to_string(currentMana)) + " -> " + truncateTrailingZeros(std::to_string(player->stats->mana)) + " - Mana Cost: " + truncateTrailingZeros(std::to_string(round(manaCost))) + " - Mana Cost Modifier: " + truncateTrailingZeros(std::to_string(round(player->stats->manaCostModifier * 100))) + "%";
        player->combatLog(msg);
    }

    bool isCrit = false;
    if (canCrit)
    {
        isCrit = player->isCrit(type, bonusCrit);
        if (isCrit)
        {
            // Increment the crit counter whether the spell hits or not so that the crit % on the damage breakdown is correct. Otherwise the crit % will be lower due to lost crits when the spell misses.
            player->combatLogBreakdown.at(varName)->crits++;
        }
    }

    if (((!isItem && !isNonWarlockAbility && varName != "amplifyCurse") || doesDamage) && !player->isHit(type))
    {
        if (player->shouldWriteToCombatLog())
        {
            std::string msg = name + " *resist*";
            player->combatLog(msg);
        }
        player->combatLogBreakdown.at(varName)->misses++;
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
    if (!isItem && !isProc && !isNonWarlockAbility && varName != "amplifyCurse")
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
        if (player->auras->ImprovedShadowBolt != NULL && player->auras->ImprovedShadowBolt->active && !player->settings->usingCustomIsbUptime)
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
    int totalDamage = dmg;
    int spellPower = player->getSpellPower(school);
    double critMultiplier = player->critMultiplier;
    double dmgModifier = getModifier();
    double partialResistMultiplier = player->getPartialResistMultiplier(school);
    
    totalDamage += spellPower * coefficient;
    totalDamage *= dmgModifier;
    totalDamage *= partialResistMultiplier;

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
    player->combatLogBreakdown.at(varName)->damage += totalDamage;
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
        if (varName == "shadowBolt" && player->auras->Corruption != NULL && player->auras->Corruption->active)
        {
            player->auras->Corruption->t5BonusModifier *= 1.1;
        }
        else if (varName == "incinerate" && player->auras->Immolate != NULL && player->auras->Immolate->active)
        {

            player->auras->Immolate->t5BonusModifier *= 1.1;
        }
    }
}

// Returns the non-RNG damage of the spell (basically just the base damage + spell power + damage modifiers, no crit/miss etc.)
// todo: investigate this noRng variable
double* Spell::getConstantDamage(bool noRng)
{
    double dmg = player->settings->randomizeValues && minDmg > 0 && maxDmg > 0 && !noRng ? random(minDmg, maxDmg) : dmg;
    double baseDamage = dmg;
    double spellPower = player->getSpellPower(school);
    double modifier = getModifier();
    double partialResistMultiplier = player->getPartialResistMultiplier(school);

    // If casting Incinerate and Immolate is up, add the bonus damage
    if (varName == "incinerate" && player->auras->Immolate != NULL && player->auras->Immolate->active)
    {
        dmg += player->settings->randomizeValues && noRng ? random(bonusDamageFromImmolateMin, bonusDamageFromImmolateMax) : bonusDamageFromImmolate;
    }
    // Add damage from Spell Power
    dmg += spellPower * coefficient;
    // Improved Shadow Bolt
    if (school == SpellSchool::SHADOW && player->auras->ImprovedShadowBolt != NULL && player->auras->ImprovedShadowBolt->active && !player->settings->usingCustomIsbUptime)
    {
        modifier *= player->auras->ImprovedShadowBolt->modifier;
    }

    dmg *= modifier * partialResistMultiplier;

    return new double[5] {baseDamage, dmg, modifier, partialResistMultiplier, spellPower};
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
    double* constantDamage = getConstantDamage();
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

    delete constantDamage;
    return (estimatedDamage * hitChance) / std::max(player->getGcdValue(varName), getCastTime());
}

void Spell::onCritProcs()
{
    // ISB
    if (varName == "shadowBolt" && !player->settings->usingCustomIsbUptime && player->talents->improvedShadowBolt > 0)
    {
        player->auras->ImprovedShadowBolt->apply();
    }
    // The Lightning Capacitor
    if (player->spells->TheLightningCapacitor != NULL)
    {
        player->spells->TheLightningCapacitor->startCast();
    }
    // Sextant of Unstable Currents
    if (player->spells->SextantOfUnstableCurrents != NULL && player->spells->SextantOfUnstableCurrents->ready() && random(1, 100) <= player->spells->SextantOfUnstableCurrents->procChance)
    {
        player->spells->SextantOfUnstableCurrents->startCast();
    }
    // Shiffar's Nexus-Horn
    if (player->spells->ShiffarsNexusHorn != NULL && player->spells->ShiffarsNexusHorn->ready() && random(1, 100) <= player->spells->ShiffarsNexusHorn->procChance)
    {
        player->spells->ShiffarsNexusHorn->startCast();
    }
}

void Spell::onDamageProcs()
{
    // Confirm that this procs on dealing damage
    // Shattered Sun Pendant of Acumen
    if (player->settings->exaltedWithShattrathFaction && player->spells->ShatteredSunPendantOfAcumen != NULL && player->spells->ShatteredSunPendantOfAcumen->cooldownRemaining <= 0 && random(1, 100) <= player->spells->ShatteredSunPendantOfAcumen->procChance)
    {
        player->spells->ShatteredSunPendantOfAcumen->startCast();
    }
}

void Spell::onHitProcs()
{
    // Judgement of Wisdom (50% proc rate)
    if (player->selectedAuras->judgementOfWisdom && random(1, 100) <= 50)
    {
        int manaVal = 74;
        int currentMana = player->stats->mana;
        int manaGained = std::min(player->stats->maxMana - currentMana, manaVal);
        player->stats->mana += manaGained;
        player->totalManaRegenerated += manaGained;
        player->combatLogBreakdown.at("judgementOfWisdom")->casts++;
        player->combatLogBreakdown.at("judgementOfWisdom")->manaGain += manaGained;
        if (player->shouldWriteToCombatLog())
        {
            std::string msg = "Player gains " + std::to_string(manaGained) + " mana from Judgement of Wisdom (" + std::to_string(currentMana) + " -> " + std::to_string(player->stats->mana) + ")";
            player->combatLog(msg);
        }
    }
    // T4 2pc
    if (player->sets->t4 >= 2 && (school == SpellSchool::SHADOW || school == SpellSchool::FIRE) && random(1, 100) <= player->auras->Flameshadow->procChance)
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
    if (player->sets->spellstrike == 2 && random(1, 100) <= player->auras->Spellstrike->procChance)
    {
        player->auras->Spellstrike->apply();
    }
    // Quagmirran's Eye
    if (player->spells->QuagmirransEye != NULL && player->spells->QuagmirransEye->ready() && random(1, 100) <= player->spells->QuagmirransEye->procChance)
    {
        player->spells->QuagmirransEye->startCast();
    }
    // Mana-Etched Regalia 4pc
    if (player->sets->manaEtched >= 4 && random(1, 100) <= player->auras->ManaEtched4Set->procChance)
    {
        player->auras->ManaEtched4Set->apply();
    }
    // Mark of Defiance
    if (player->spells->MarkOfDefiance != NULL && random(1, 100) <= player->spells->MarkOfDefiance->procChance)
    {
        player->spells->MarkOfDefiance->startCast();
    }
    // Darkmoon Card: Crusade
    if (player->auras->DarkmoonCardCrusade != NULL)
    {
        player->auras->DarkmoonCardCrusade->apply();
    }
    // Band of the Eternal Sage
    if (player->spells->BandOfTheEternalSage != NULL && player->spells->BandOfTheEternalSage->ready() && random(1, 100) <= player->spells->BandOfTheEternalSage->procChance)
    {
        player->spells->BandOfTheEternalSage->startCast();
    }
    // Blade of Wizardry
    if (player->spells->BladeOfWizardry != NULL && player->spells->BladeOfWizardry->ready() && random(1, 100) <= player->auras->BladeOfWizardry->procChance)
    {
        player->spells->BladeOfWizardry->startCast();
    }
    // Mystical Skyfire Diamond
    if (player->spells->MysticalSkyfireDiamond != NULL && player->spells->MysticalSkyfireDiamond->ready() && random(1, 100) <= player->spells->MysticalSkyfireDiamond->procChance)
    {
        player->spells->MysticalSkyfireDiamond->startCast();
    }
    // Robe of the Elder Scribes
    if (player->spells->RobeOfTheElderScribes != NULL && player->spells->RobeOfTheElderScribes->ready() && random(1, 100) <= player->spells->RobeOfTheElderScribes->procChance)
    {
        player->spells->RobeOfTheElderScribes->startCast();
    }
    // Insightful Earthstorm Diamond
    if (player->spells->InsightfulEarthstormDiamond != NULL && player->spells->InsightfulEarthstormDiamond->ready() && random(1, 100) <= player->spells->InsightfulEarthstormDiamond->procChance)
    {
        player->spells->InsightfulEarthstormDiamond->startCast();
    }
    // Wrath of Cenarius
    if (player->auras->WrathOfCenarius != NULL && random(1, 100) <= player->auras->WrathOfCenarius->procChance)
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
    //bool hasShadowTrance = player->getAura("shadowTrance") != NULL;

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
    player->combatLogBreakdown.at(varName)->casts++;
    player->combatLogBreakdown.at(varName)->manaGain += manaGain;
    
    if (player->shouldWriteToCombatLog() && player->stats->mana + manaGain > player->stats->maxMana)
    {
        std::string combatLogEntry = "Life Tap used at too high mana (mana wasted)";
        player->combatLog(combatLogEntry);
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
    coefficient = 0.22;
    setup();
};

void SeedOfCorruption::damage()
{
    int baseDamage = player->settings->randomizeValues && minDmg && maxDmg ? random(minDmg, maxDmg) : dmg;
    int enemyAmount = player->settings->enemyAmount - 1; // Minus one because the enemy that Seed is being cast on doesn't get hit
    int resistAmount = 0;
    int critAmount = 0;
    int spellPower = player->getSpellPower(school);
    double modifier = player->stats->shadowModifier; //todo debuffs increase dmg past the aoe cap

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
    double individualSeedCrit = individualSeedDamage * getCritMultiplier(player->critMultiplier);
    double bonusDamageFromCrit = individualSeedCrit - individualSeedDamage;
    totalSeedDamage += bonusDamageFromCrit * critAmount;
    // Partial resists (probably need to calculate a partial resist for each seed hit, not sure how it interacts for the aoe cap)
    double partialResistMultiplier = player->getPartialResistMultiplier(school);
    totalSeedDamage *= partialResistMultiplier;

    player->iterationDamage += totalSeedDamage;

    if (player->shouldWriteToCombatLog())
    {
        std::string msg = name + " " + std::to_string(round(totalSeedDamage)) + " (" + std::to_string(enemyAmount) + " Enemies (" + std::to_string(resistAmount) + " Resists & " + std::to_string(critAmount) + " Crits) - " + std::to_string(baseDamage) + " Base Damage - " + std::to_string(coefficient) + " Coefficient - " + std::to_string(spellPower) + " Spell Power - " + std::to_string(round(modifier * 1000) / 1000) + "% Modifier - " + std::to_string(partialResistMultiplier) + " % Partial Resist Multiplier)";
        player->combatLog(msg);
    }
    player->combatLogBreakdown.at(varName)->damage += totalSeedDamage;
    player->combatLogBreakdown.at(varName)->crits += critAmount;
    player->combatLogBreakdown.at(varName)->misses += resistAmount;
    // the cast() function already adds 1 to the amount of casts so we only need to add enemiesHit - 1 to the cast amount
    player->combatLogBreakdown.at(varName)->casts += (enemiesHit - 1);
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

Corruption::Corruption(Player* player, Aura* aura, DamageOverTime* dot) : Spell(player, aura, dot)
{
    name = "Corruption";
    manaCost = 370;
    castTime = round((2 - (0.4 * player->talents->improvedCorruption)) * 100) / 100;
    isDot = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
}

UnstableAffliction::UnstableAffliction(Player* player, Aura* aura, DamageOverTime* dot) : Spell(player, aura, dot)
{
    name = "Unstable Affliction";
    manaCost = 400;
    castTime = 1.5;
    isDot = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
}

SiphonLife::SiphonLife(Player* player, Aura* aura, DamageOverTime* dot) : Spell(player, aura, dot)
{
    name = "Siphon Life";
    manaCost = 410;
    isDot = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
}

Immolate::Immolate(Player* player, Aura* aura, DamageOverTime* dot) : Spell(player, aura, dot)
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

CurseOfAgony::CurseOfAgony(Player* player, Aura* aura, DamageOverTime* dot) : Spell(player, aura, dot)
{
    name = "Curse of Agony";
    manaCost = 265;
    isDot = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
}

CurseOfTheElements::CurseOfTheElements(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Curse of the Elements";
    manaCost = 260;
    type = SpellType::AFFLICTION;
    isAura = true;
    setup();
}

CurseOfRecklessness::CurseOfRecklessness(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Curse of Recklessness";
    manaCost = 160;
    type = SpellType::AFFLICTION;
    isAura = true;
    setup();
}

CurseOfDoom::CurseOfDoom(Player* player, Aura* aura, DamageOverTime* dot) : Spell(player, aura, dot)
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

DestructionPotion::DestructionPotion(Player* player, Aura* aura) : Spell(player, aura)
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
    player->totalManaRegenerated += manaGain;
    player->combatLogBreakdown.at(varName)->casts++;
    player->combatLogBreakdown.at(varName)->manaGain += manaGain;
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
    player->totalManaRegenerated += manaGain;
    player->combatLogBreakdown.at(varName)->casts++;
    player->combatLogBreakdown.at(varName)->manaGain += manaGain;
    player->stats->mana = std::min(player->stats->maxMana, currentPlayerMana + manaGain);
    if (player->shouldWriteToCombatLog())
    {
        std::string msg = "Player gains " + truncateTrailingZeros(std::to_string(round(player->stats->mana - currentPlayerMana))) + " mana from " + name + " (" + truncateTrailingZeros(std::to_string(round(currentPlayerMana))) + " -> " + truncateTrailingZeros(std::to_string(round(player->stats->mana))) + ")";
        player->combatLog(msg);
    }
}

FlameCap::FlameCap(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Flame Cap";
    cooldown = 180;
    isItem = true;
    isAura = true;
    onGcd = false;
    setup();
}

BloodFury::BloodFury(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Blood Fury";
    cooldown = 120;
    isAura = true;
    onGcd = false;
    setup();
}

Bloodlust::Bloodlust(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Bloodlust";
    cooldown = 600;
    isItem = true;
    isAura = true;
    onGcd = false;
    isNonWarlockAbility = true;
    setup();
}

DrumsOfBattle::DrumsOfBattle(Player* player, Aura* aura) : Spell(player, aura)
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

DrumsOfWar::DrumsOfWar(Player* player, Aura* aura) : Spell(player, aura)
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

DrumsOfRestoration::DrumsOfRestoration(Player* player, Aura* aura) : Spell(player, aura)
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
        player->totalManaRegenerated += avgManaValue;
        player->stats->mana = std::min(static_cast<double>(player->stats->maxMana), currentPlayerMana + avgManaValue);
        if (player->shouldWriteToCombatLog())
        {
            std::string msg = "Player gains " + truncateTrailingZeros(std::to_string(round(player->stats->mana - currentPlayerMana))) + " mana from " + name + " (" + truncateTrailingZeros(std::to_string(round(currentPlayerMana))) + " -> " + truncateTrailingZeros(std::to_string(round(player->stats->mana))) + ")";
            player->combatLog(msg);
        }
        cooldownRemaining = cooldown;
    }
}

TheLightningCapacitor::TheLightningCapacitor(Player* player, Aura* aura) : Spell(player, aura)
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
            cooldownRemaining = cooldown;
            player->auras->TheLightningCapacitor->fade();
            Spell::startCast();
        }
    }
}

BladeOfWizardry::BladeOfWizardry(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Blade of Wizardry";
    cooldown = 50;
    onGcd = false;
    isItem = true;
    isProc = true;
    isAura = true;
    setup();
}

ShatteredSunPendantOfAcumen::ShatteredSunPendantOfAcumen(Player* player, Aura* aura) : Spell(player, aura)
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

RobeOfTheElderScribes::RobeOfTheElderScribes(Player* player, Aura* aura) : Spell(player, aura)
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

QuagmirransEye::QuagmirransEye(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Quagmirran's Eye";
    cooldown = 45;
    procChance = 10;
    onGcd = false;
    isItem = true;
    isAura = true;
    setup();
}

ShiffarsNexusHorn::ShiffarsNexusHorn(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Shiffar's Nexus-Horn";
    cooldown = 45;
    procChance = 20;
    onGcd = false;
    isItem = true;
    isAura = true;
    setup();
}

SextantOfUnstableCurrents::SextantOfUnstableCurrents(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Sextant of Unstable Currents";
    cooldown = 45;
    procChance = 20;
    onGcd = false;
    isItem = true;
    isAura = true;
    setup();
}

BandOfTheEternalSage::BandOfTheEternalSage(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Band of the Eternal Sage";
    cooldown = 60;
    procChance = 10;
    onGcd = false;
    isItem = true;
    isAura = true;
    setup();
}

MysticalSkyfireDiamond::MysticalSkyfireDiamond(Player* player, Aura* aura) : Spell(player, aura)
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
    player->totalManaRegenerated += manaGain;
    player->stats->mana = std::min(static_cast<double>(player->stats->maxMana), currentPlayerMana + manaGain);
    if (player->shouldWriteToCombatLog())
    {
        std::string msg = "Player gains " + std::to_string(round(player->stats->mana - currentPlayerMana)) + " mana from " + name + " (" + std::to_string(round(currentPlayerMana)) + " -> " + std::to_string(round(player->stats->mana)) + ")";
        player->combatLog(msg);
    }
}

AmplifyCurse::AmplifyCurse(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Amplify Curse";
    cooldown = 180;
    isAura = true;
    onGcd = false;
    setup();
}

PowerInfusion::PowerInfusion(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Power Infusion";
    cooldown = 180;
    isAura = true;
    onGcd = false;
    isNonWarlockAbility = true;
    setup();
}

Innervate::Innervate(Player* player, Aura* aura) : Spell(player, aura)
{
    name = "Innervate";
    cooldown = 360;
    isAura = true;
    onGcd = false;
    isNonWarlockAbility = true;
    setup();
}