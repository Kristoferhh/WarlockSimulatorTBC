#include "player.h"
#include "spell.h"

Player::Player(PlayerSettings* playerSettings) : auras(playerSettings->auras), talents(playerSettings->talents), sets(playerSettings->sets), stats(playerSettings->stats)
{
    this->stats->maxMana = this->stats->mana;
    std::vector<std::string> combatLogEntries = {};
    double castTimeRemaining = 0;
    double totalManaRegenerated = 0;
    double gcdRemaining = 0;
    double gcdValue = 1.5;
    // Increases the cast time of spells by a very small amount to e.g. make it so that if Immolate has 1.5 seconds remaining, the sim can start precasting Immolate and it won't refresh before it expires.
    double spellDelay = 0.0001;
}

void Player::initialize()
{
    this->spells.insert(std::make_pair("lifeTap", new LifeTap(this)));
    this->spells.insert(std::make_pair("shadowBolt", new ShadowBolt(this)));
}

void Player::combatLog(std::string &entry)
{
    combatLogEntries.push_back(entry);
}

double Player::getGcdValue(std::string varName)
{
    return 1.5;
}

double Player::getSpellPower()
{
    return this->stats->spellPower;
}