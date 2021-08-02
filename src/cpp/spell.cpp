#include "spell.h"
#include "player.h"

struct ShadowBolt : public Spell
{
  ShadowBolt(Player &p) : Spell(p)
  {
    castTime = calculateCastTime();
    manaCost = 420 * (1 - 0.01 * player.talents["cataclysm"]);
    coefficient = (3 / 3.5) + (0.04 * player.talents["shadowAndFlame"]);
    minDmg = 544;
    maxDmg = 607;
    name = "Shadow Bolt";
    doesDamage = true;
    canCrit = true;
    school = School::SHADOW;
    type = Type::DESTRUCTION;
    setup();

    // T6 4pc bonus
    if (player.sets["670"] >= 4)
    {
      modifier *= 1.06;
    }
  }

  void startCast(double predictedDamage = 0) override
  {
    bool hasShadowTrance = player.auras.count("shadowTrance") > 0;

    /*if (hasShadowTrance && player.auras["shadowTrance"].active)
    {
      castTime = 0;
    }*/

    Spell::startCast();
    
    /*if (hasShadowTrance && player.auras["shadowTrance"].active)
    {
      castTime = calculateCastTime();
      player.auras["shadowTrance"].fade();
    }*/
  }

  double calculateCastTime()
  {
    return 3 - (0.1 * player.talents["bane"]);
  }
};

struct LifeTap : public Spell
{
  LifeTap(Player &p) : Spell(p)
  {
    name = "Life Tap";
    manaReturn = 582;
    coefficient = 0.8;
    modifier = 1 * (1 + 0.1 * player.talents["improvedLifeTap"]);
    breakdownTable = "manaGain";
    setup();
  }

  bool ready() override
  {
    return true;
    //return Spell::ready() && manaGain() + player.stats.mana < player.stats.maxMana;
  }

  double manaGain()
  {
    return 1000;
    //return (manaReturn + ((player.getSpellPower() + player.stats.shadowPower) * coefficient)) * modifier;
  }

  void cast() override
  {
    const double manaGain = this->manaGain();
    player.totalManaRegenerated += manaGain;
    
    /*if (player.stats.mana + manaGain > player.stats.maxMana)
    {
      player.combatLog("Life Tap used at too high mana (mana wasted)");
    }
    player.stats.mana = std::min(player.stats.maxMana, player.stats.mana + manaGain);*/
  }
};