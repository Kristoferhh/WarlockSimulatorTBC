#include "petStats.h"

#include "player.h"

PetStats::PetStats(Player* player)
    : player(player), damageModifier(1), staminaModifier(1), intellectModifier(1), strengthModifier(1), agilityModifier(1), spiritModifier(1), attackPowerModifier(1)
{
    staminaModifier *= 1 + (0.05 * player->talents->felStamina);
    intellectModifier *= 1 + (0.05 * player->talents->felIntellect);
    damageModifier *= (1 + 0.04 * player->talents->unholyPower) * (1 + 0.05 * player->talents->soulLink);
}