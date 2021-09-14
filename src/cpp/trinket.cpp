#include "trinket.h"
#include "common.h"
#include "player.h"

Trinket::Trinket(Player* player) : player(player)
{
    cooldown = 0;
    cooldownRemaining = 0;
    duration = 0;
    durationRemaining = 0;
    active = false;
    sharesCooldown = true;
}

bool Trinket::ready()
{
    return cooldownRemaining <= 0;
}

void Trinket::reset()
{
    cooldownRemaining = 0;
}

void Trinket::setup()
{
    varName = camelCase(name);
    if (player->combatLogBreakdown.count(varName) == 0)
    {
        player->combatLogBreakdown.insert(std::make_pair(varName, new CombatLogBreakdown(name)));
    }
}

void Trinket::use()
{
    bool recalculatePetStats = false;
    if (player->shouldWriteToCombatLog())
    {
        std::string msg = name + " used";
        player->combatLog(msg);
    }
    player->combatLogBreakdown.at(varName)->appliedAt = player->fightTime;
    player->combatLogBreakdown.at(varName)->count++;

    if (duration > 0)
    {
        if (stats->spellPower > 0)
        {
            if (player->shouldWriteToCombatLog())
            {
                int currentSpellPower = player->getSpellPower();
                std::string msg = "Spell Power + " + std::to_string(stats->spellPower) + " (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower + stats->spellPower) + ")";
                player->combatLog(msg);
            }
            player->stats->spellPower += stats->spellPower;
            recalculatePetStats = true;
        }
    }

    /*if (recalculatePetStats && player->pet != null)
    {
        player->pet->calculateStatsFromPlayer();
    }*/
    
    active = true;
    durationRemaining = duration;
    cooldownRemaining = cooldown;
}

void Trinket::fade(bool endOfIteration)
{
    bool recalculatePetStats = false;
    if (!endOfIteration && player->shouldWriteToCombatLog())
    {
        std::string msg = name + " faded";
        player->combatLog(msg);
    }

    if (stats->spellPower > 0)
    {
        if (!endOfIteration && player->shouldWriteToCombatLog())
        {
            int currentSpellPower = player->getSpellPower();
            std::string msg = "Spell Power - " + std::to_string(stats->spellPower) + " (" + std::to_string(currentSpellPower) + " -> " + std::to_string(currentSpellPower - stats->spellPower) + ")";
            player->combatLog(msg);
        }
        player->stats->spellPower -= stats->spellPower;
        recalculatePetStats = true;
    }

    /*if (recalculatePetStats && player->pet != null)
    {
        player->pet->calculateStatsFromPlayer();
    }*/

    active = false;
    double auraUptime = player->fightTime - player->combatLogBreakdown.at(varName)->appliedAt;
    player->combatLogBreakdown.at(varName)->uptime += auraUptime;
}

void Trinket::tick(double t)
{
    if (player->shouldWriteToCombatLog() && cooldownRemaining > 0 && cooldownRemaining - t <= 0)
    {
        std::string msg = name + " off cooldown";
        player->combatLog(msg);
    }
    cooldownRemaining -= t;
    durationRemaining -= t;
    if (active && durationRemaining <= 0)
    {
        fade();
    }
}

RestrainedEssenceOfSapphiron::RestrainedEssenceOfSapphiron(Player* player) : Trinket(player)
{
    name = "Restrained Essence of Sapphiron";
    cooldown = 120;
    duration = 20;
    Trinket::stats = std::make_unique<AuraStats>(130, 0, 0, 0, 0, 0);
    setup();
}

ShiftingNaaruSliver::ShiftingNaaruSliver(Player* player) : Trinket(player)
{
    name = "Shifting Naaru Sliver";
    cooldown = 90;
    duration = 15;
    Trinket::stats = std::make_unique<AuraStats>(320, 0, 0, 0, 0, 0);
    setup();
}

SkullOfGuldan::SkullOfGuldan(Player* player) : Trinket(player)
{
    name = "Skull of Gul'dan";
    cooldown = 120;
    duration = 20;
    Trinket::stats = std::make_unique<AuraStats>(0, 0, 0, 175, 0, 0);
    setup();
}

HexShrunkenHead::HexShrunkenHead(Player* player) : Trinket(player)
{
    name = "Hex Shrunken Head";
    cooldown = 120;
    duration = 20;
    Trinket::stats = std::make_unique<AuraStats>(211, 0, 0, 0, 0, 0);
    setup();
}

IconOfTheSilverCrescent::IconOfTheSilverCrescent(Player* player) : Trinket(player)
{
    name = "Icon of the Silver Crescent";
    cooldown = 120;
    duration = 20;
    Trinket::stats = std::make_unique<AuraStats>(155, 0, 0, 0, 0, 0);
    setup();
}

ScryersBloodgem::ScryersBloodgem(Player* player) : Trinket(player)
{
    name = "Scryer's Bloodgem";
    cooldown = 90;
    duration = 15;
    Trinket::stats = std::make_unique<AuraStats>(150, 0, 0, 0, 0, 0);
    setup();
}

AncientCrystalTalisman::AncientCrystalTalisman(Player* player) : Trinket(player)
{
    name = "Ancient Crystal Talisman";
    cooldown = 120;
    duration = 20;
    Trinket::stats = std::make_unique<AuraStats>(104, 0, 0, 0, 0, 0);
    setup();
}

ArcanistsStone::ArcanistsStone(Player* player) : Trinket(player)
{
    name = "Arcanist's Stone";
    cooldown = 120;
    duration = 20;
    Trinket::stats = std::make_unique<AuraStats>(167, 0, 0, 0, 0, 0);
    setup();
}

TerokkarTabletOfVim::TerokkarTabletOfVim(Player* player) : Trinket(player)
{
    name = "Terokkar Table of Vim";
    cooldown = 90;
    duration = 15;
    Trinket::stats = std::make_unique<AuraStats>(84, 0, 0, 0, 0, 0);
    setup();
}

XirisGift::XirisGift(Player* player) : Trinket(player)
{
    name = "Xi'ri's Gift";
    cooldown = 90;
    duration = 15;
    Trinket::stats = std::make_unique<AuraStats>(150, 0, 0, 0, 0, 0);
    setup();
}

VengeanceOfTheIllidari::VengeanceOfTheIllidari(Player* player) : Trinket(player)
{
    name = "Vengeance of the Illidari";
    cooldown = 90;
    duration = 15;
    Trinket::stats = std::make_unique<AuraStats>(120, 0, 0, 0, 0, 0);
    setup();
}

FigurineLivingRubySerpent::FigurineLivingRubySerpent(Player* player) : Trinket(player)
{
    name = "Figurine: Living Ruby Serpent";
    cooldown = 300;
    duration = 20;
    Trinket::stats = std::make_unique<AuraStats>(150, 0, 0, 0, 0, 0);
    setup();
}

EssenceOfTheMartyr::EssenceOfTheMartyr(Player* player) : Trinket(player)
{
    name = "Essence of the Martyr";
    cooldown = 120;
    duration = 20;
    sharesCooldown = false;
    Trinket::stats = std::make_unique<AuraStats>(99, 0, 0, 0, 0, 0);
    setup();
}

StarkillersBauble::StarkillersBauble(Player* player) : Trinket(player)
{
    name = "Starkiller's Bauble";
    cooldown = 90;
    duration = 15;
    Trinket::stats = std::make_unique<AuraStats>(125, 0, 0, 0, 0, 0);
    setup();
}