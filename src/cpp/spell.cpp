#include "spell.h"
#include "player.h"
#include "common.h"

Spell::Spell(Player* player) : player(player)
{
    modifier = 1;
    coefficient = 0;
    school = SpellSchool::NO_SCHOOL;
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
}

double Spell::getCastTime()
{
    return round((castTime / (1.0 + ((player->stats->hasteRating / player->hasteRatingPerPercent + player->stats->hastePercent) / 100))) * 10000) / 10000 + player->spellDelay;
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

    if (player->shouldWriteToCombatLog())
    {
        player->combatLog(combatLogMsg);
    }
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

    if (isAura)
    {
        player->getAura(varName)->apply();
    }
    if (isDot)
    {
        player->getDot(varName)->apply();
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
        if (player->getAura("improvedShadowBolt") != nullptr && player->getAura("improvedShadowBolt")->active && !player->settings->usingCustomIsbUptime)
        {
            dmgModifier *= player->getAura("improvedShadowBolt")->modifier;
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
    else if (school == SpellSchool::SHADOW && !isDot && player->getAura("improvedShadowBolt") != nullptr && player->getAura("improvedShadowBolt")->active && !player->settings->usingCustomIsbUptime)
    {
        player->getAura("improvedShadowBolt")->decrementStacks();
    }

    onDamageProcs();
    player->iterationDamage += totalDamage;

    // Combat Log
    if (player->shouldWriteToCombatLog())
    {
        std::string msg = name + " ";
        if (isCrit)
        {
            msg += "*";
        }
        msg += std::to_string(round(totalDamage));
        if (isCrit)
        {
            msg += "*";
        }
        msg += " (" + std::to_string(dmg) + " Base Damage - " + std::to_string(round(coefficient * 1000) / 1000) + " Coefficient - " + std::to_string(round(spellPower)) + " Spell Power - ";
        if (isCrit)
        {
            msg += std::to_string(round(critMultiplier * 100) / 100) + "% Crit Multiplier - ";
        }
        msg += std::to_string(round(dmgModifier * 10000) / 100) + "% Damage Modifier - " + std::to_string(round(partialResistMultiplier * 1000) / 10) + "% Partial Resist Multiplier)";
        player->combatLog(msg);
    }

    //T5 4pc
    if (player->sets->t5 >= 4)
    {
        /*if (varName == "shadowBolt" && player->getAura("corruption") != nullptrplayer->auras->corruption->active)
        {
            player->auras->corruption->t5BonusModifier *= 1.1;
        }
        else if (varName == "incinerate" && player->getAura("immolate") != nullptrplayer->auras->immolate->active)
        {

            player->auras->immolate->t5BonusModifier *= 1.1;
        }*/
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
    if (varName == "incinerate" && player->getAura("immolate") != nullptr && player->getAura("immolate")->active)
    {
        dmg += player->settings->randomizeValues && noRng ? random(bonusDamageFromImmolateMin, bonusDamageFromImmolateMax) : bonusDamageFromImmolate;
    }
    // Add damage from Spell Power
    dmg += spellPower * coefficient;
    // Improved Shadow Bolt
    if (school == SpellSchool::SHADOW && player->getAura("improvedShadowBolt") != nullptr && player->getAura("improvedShadowBolt")->active && !player->settings->usingCustomIsbUptime)
    {
        modifier *= player->getAura("improvedShadowBolt")->modifier;
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
        estimatedDamage += player->getDot(varName)->predictDamage();
    }

    // If the player is not using a custom ISB uptime, they have the ISB talent selected, but the ISB aura is not active, then give some % modifier as an "average" for the damage.
    // Without this, the sim will choose Incinerate over Shadow Bolt because it basically just doesn't know that ISB exists
    if (school == SpellSchool::SHADOW && !player->settings->usingCustomIsbUptime && player->getAura("improvedShadowBolt") != nullptr && !player->getAura("improvedShadowBolt")->active)
    {
        // If this isn't the player's first iteration then check what their ISB uptime is and add that %
        if (player->iteration > 1)
        {
            estimatedDamage *= (1 + 0.2 * player->getAura("improvedShadowBolt")->uptimeSoFar);
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
        player->getAura("improvedShadowBolt")->apply();
    }
    // The Lightning Capacitor
    if (player->getSpell("theLightningCapacitor") != nullptr)
    {
        player->getSpell("theLightningCapacitor")->startCast();
    }
    // Sextant of Unstable Currents
    if (player->getSpell("sextantOfUnstableCurrents") != nullptr && player->getSpell("sextantOfUnstableCurrents")->ready() && random(1, 100) <= player->getSpell("sextantOfUnstableCurrents")->procChance)
    {
        player->getSpell("sextantOfUnstableCurrents")->startCast();
    }
    // Shiffar's Nexus-Horn
    if (player->getSpell("shiffarsNexusHorn") != nullptr && player->getSpell("shiffarsNexusHorn")->ready() && random(1, 100) <= player->getSpell("shiffarsNexusHorn")->procChance)
    {
        player->getSpell("shiffarsNexusHorn")->startCast();
    }
}

void Spell::onDamageProcs()
{
    // Confirm that this procs on dealing damage
    // Shattered Sun Pendant of Acumen
    if (player->settings->exaltedWithShattrathFaction && player->getSpell("shatteredSunPendantOfAcumen") != nullptr && player->getSpell("shatteredSunPendantOfAcumen")->cooldownRemaining <= 0 && random(1, 100) <= player->getSpell("shatteredSunPendantOfAcumen")->procChance)
    {
        player->getSpell("shatteredSunPendantOfAcumen")->startCast();
    }
}

void Spell::onHitProcs()
{
    // T4 2pc
    if (player->sets->t4 >= 2 && (school == SpellSchool::SHADOW || school == SpellSchool::FIRE) && random(1, 100) <= player->getAura("flameshadow")->procChance)
    {
        if (school == SpellSchool::SHADOW)
        {
            player->getAura("flameshadow")->apply();
        }
        else if (school == SpellSchool::FIRE)
        {
            player->getAura("shadowflame")->apply();
        }
    }
    // Spellstrike
    if (player->sets->spellstrike == 2 && random(1, 100) <= player->getAura("spellstrike")->procChance)
    {
        player->getAura("spellstrike")->apply();
    }
    // Quagmirran's Eye
    if (player->getSpell("quagmirransEye") != nullptr && player->getSpell("quagmirransEye")->ready() && random(1, 100) <= player->getSpell("quagmirransEye")->procChance)
    {
        player->getSpell("quagmirransEye")->startCast();
    }
    // Mana-Etched Regalia 4pc
    if (player->sets->manaEtched >= 4 && random(1, 100) <= player->getAura("manaEtched4Set")->procChance)
    {
        player->getAura("manaEtched4Set")->apply();
    }
    // Mark of Defiance
    if (player->getSpell("markOfDefiance") != nullptr && random(1, 100) <= player->getSpell("markOfDefiance")->procChance)
    {
        player->getSpell("markOfDefiance")->startCast();
    }
    // Darkmoon Card: Crusade
    if (player->getSpell("darkmoonCardCrusade") != nullptr)
    {
        player->getAura("darkmoonCardCrusade")->apply();
    }
    // Band of the Eternal Sage
    if (player->getSpell("bandOfTheEternalSage") != nullptr && player->getSpell("bandOfTheEternalSage")->ready() && random(1, 100) <= player->getSpell("bandOfTheEternalSage")->procChance)
    {
        player->getSpell("bandOfTheEternalSage")->startCast();
    }
    // Blade of Wizardry
    if (player->getSpell("bladeOfWizardry") != nullptr && player->getSpell("bladeOfWizardry")->ready() && random(1, 100) <= player->getAura("bladeOfWizardry")->procChance)
    {
        player->getSpell("bladeOfWizardry")->startCast();
    }
    // Mystical Skyfire Diamond
    if (player->getSpell("mysticalSkyfireDiamond") != nullptr && player->getSpell("mysticalSkyfireDiamond")->ready() && random(1, 100) <= player->getSpell("mysticalSkyfireDiamond")->procChance)
    {
        player->getSpell("mysticalSkyfireDiamond")->startCast();
    }
    // Robe of the Elder Scribes
    if (player->getSpell("robeOfTheElderScribes") != nullptr && player->getSpell("robeOfTheElderScribes")->ready() && random(1, 100) <= player->getSpell("robeOfTheElderScribes")->procChance)
    {
        player->getSpell("robeOfTheElderScribes")->startCast();
    }
    // Insightful Earthstorm Diamond
    if (player->getSpell("insightfulEarthstormDiamond") != nullptr && player->getSpell("insightfulEarthstormDiamond")->ready() && random(1, 100) <= player->getSpell("insightfulEarthstormDiamond")->procChance)
    {
        player->getSpell("insightfulEarthstormDiamond")->startCast();
    }
    // Wrath of Cenarius
    if (player->getAura("wrathOfCenarius") != nullptr && player->getSpell("wrathOfCenarius")->ready() && random(1, 100) <= player->getSpell("wrathOfCenarius")->procChance)
    {
        player->getAura("wrathOfCenarius")->apply();
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
    //bool hasShadowTrance = player->getAura("shadowTrance") != nullptr;

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

double SeedOfCorruption::damage()
{
    return 0;
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

Corruption::Corruption(Player* player) : Spell(player)
{
    name = "Corruption";
    manaCost = 370;
    castTime = round((2 - (0.4 * player->talents->improvedCorruption)) * 100) / 100;
    isDot = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
}

UnstableAffliction::UnstableAffliction(Player* player) : Spell(player)
{
    name = "Unstable Affliction";
    manaCost = 400;
    castTime = 1.5;
    isDot = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
}

SiphonLife::SiphonLife(Player* player) : Spell(player)
{
    name = "Siphon Life";
    manaCost = 410;
    isDot = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
}

Immolate::Immolate(Player* player) : Spell(player)
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

CurseOfAgony::CurseOfAgony(Player* player) : Spell(player)
{
    name = "Curse of Agony";
    manaCost = 265;
    isDot = true;
    school = SpellSchool::SHADOW;
    type = SpellType::AFFLICTION;
    setup();
}

CurseOfTheElements::CurseOfTheElements(Player* player) : Spell(player)
{
    name = "Curse of the Elements";
    manaCost = 260;
    type = SpellType::AFFLICTION;
    isAura = true;
    setup();
}

CurseOfRecklessness::CurseOfRecklessness(Player* player) : Spell(player)
{
    name = "Curse of Recklessness";
    manaCost = 160;
    type = SpellType::AFFLICTION;
    isAura = true;
    setup();
}

CurseOfDoom::CurseOfDoom(Player* player) : Spell(player)
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
    /*if (player->getAura("immolate") != nullptr && player->auras->immolate->active)
    {
        Spell::startCast();
        player->auras->immolate->active = false;
    }*/
}

DestructionPotion::DestructionPotion(Player* player) : Spell(player)
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
    player->stats->mana = std::min(player->stats->maxMana, currentPlayerMana + manaGain);
    if (player->shouldWriteToCombatLog())
    {
        std::string msg = "Player gains " + std::to_string(round(player->stats->mana - currentPlayerMana)) + " mana from " + name + " (" + std::to_string(round(currentPlayerMana)) + " -> " + std::to_string(round(player->stats->mana)) + ")";
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
    player->stats->mana = std::min(player->stats->maxMana, currentPlayerMana + manaGain);
    if (player->shouldWriteToCombatLog())
    {
        std::string msg = "Player gains " + std::to_string(round(player->stats->mana - currentPlayerMana)) + " mana from " + name + " (" + std::to_string(round(currentPlayerMana)) + " -> " + std::to_string(round(player->stats->mana)) + ")";
    }
}

FlameCap::FlameCap(Player* player) : Spell(player)
{
    name = "Flame Cap";
    cooldown = 180;
    isItem = true;
    isAura = true;
    onGcd = false;
    setup();
}

BloodFury::BloodFury(Player* player) : Spell(player)
{
    name = "Blood Fury";
    cooldown = 120;
    isAura = true;
    onGcd = false;
    setup();
}

Bloodlust::Bloodlust(Player* player) : Spell(player)
{
    name = "Bloodlust";
    cooldown = 600;
    isItem = true;
    isAura = true;
    onGcd = false;
    setup();
}

DrumsOfBattle::DrumsOfBattle(Player* player) : Spell(player)
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

DrumsOfWar::DrumsOfWar(Player* player) : Spell(player)
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

DrumsOfRestoration::DrumsOfRestoration(Player* player) : Spell(player)
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
            std::string msg = "Player gains " + std::to_string(round(player->stats->mana - currentPlayerMana)) + " mana from " + name + " (" + std::to_string(round(currentPlayerMana)) + " -> " + std::to_string(round(player->stats->mana)) + ")";
            player->combatLog(msg);
        }
        cooldownRemaining = cooldown;
    }
}

TheLightningCapacitor::TheLightningCapacitor(Player* player) : Spell(player)
{
    name = "The Lightning Capacitor";
    cooldown = 2.5;
    onGcd = false;
    minDmg = 694;
    maxDmg = 806;
    doesDamage = true;
    canCrit = true;
    setup();
}

void TheLightningCapacitor::startCast(double predictedDamage)
{
    if (cooldownRemaining <= 0)
    {
        player->getAura("theLightningCapacitor")->apply();
        if (player->getAura("theLightningCapacitor")->stacks == 3)
        {
            cooldownRemaining = cooldown;
            player->getAura("theLightningCapacitor")->fade();
            Spell::startCast();
        }
    }
}

BladeOfWizardry::BladeOfWizardry(Player* player) : Spell(player)
{
    name = "Blade of Wizardry";
    cooldown = 50;
    onGcd = false;
    isItem = true;
    isProc = true;
    isAura = true;
    setup();
}

ShatteredSunPendantOfAcumen::ShatteredSunPendantOfAcumen(Player* player) : Spell(player)
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

RobeOfTheElderScribes::RobeOfTheElderScribes(Player* player) : Spell(player)
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

QuagmirransEye::QuagmirransEye(Player* player) : Spell(player)
{
    name = "Quagmirran's Eye";
    cooldown = 45;
    procChance = 10;
    onGcd = false;
    isItem = true;
    isAura = true;
    setup();
}

ShiffarsNexusHorn::ShiffarsNexusHorn(Player* player) : Spell(player)
{
    name = "Shiffar's Nexus-Horn";
    cooldown = 45;
    procChance = 20;
    onGcd = false;
    isItem = true;
    isAura = true;
    setup();
}

SextantOfUnstableCurrents::SextantOfUnstableCurrents(Player* player) : Spell(player)
{
    name = "Sextant of Unstable Currents";
    cooldown = 45;
    procChance = 20;
    onGcd = false;
    isItem = true;
    isAura = true;
    setup();
}

BandOfTheEternalSage::BandOfTheEternalSage(Player* player) : Spell(player)
{
    name = "Band of the Eternal Sage";
    cooldown = 60;
    procChance = 10;
    onGcd = false;
    isItem = true;
    isAura = true;
    setup();
}

MysticalSkyfireDiamond::MysticalSkyfireDiamond(Player* player) : Spell(player)
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

AmplifyCurse::AmplifyCurse(Player* player) : Spell(player)
{
    name = "Amplify Curse";
    cooldown = 180;
    isAura = true;
    onGcd = false;
    setup();
}

PowerInfusion::PowerInfusion(Player* player) : Spell(player)
{
    name = "Power Infusion";
    cooldown = 180;
    isAura = true;
    isNonWarlockAbility = true;
    setup();
}

Innervate::Innervate(Player* player) : Spell(player)
{
    name = "Innervate";
    cooldown = 360;
    isAura = true;
    isNonWarlockAbility = true;
    setup();
}