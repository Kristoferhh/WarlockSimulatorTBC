#pragma once

#include <map>
#include <list>
#include <string>

struct PlayerSettings
{
    std::map<std::string, bool> auras;
    std::map<std::string, bool> talents;
    std::map<std::string, int> sets;
};

struct Player
{
    std::map<std::string, bool> auras;
    std::map<std::string, bool> talents;
    std::map<std::string, int> sets;
    std::list<std::string> combatLogEntries = {};
    double castTimeRemaining = 0;
    double totalManaRegenerated = 0;
    double gcdRemaining = 0;
    double gcdValue = 1.5;
    // The amount to increase spell cast times by.
    // This will not have any actual effect on the dps result because of how small the value is, but it will make things a lot more realistic
    // because cast times, dot ticks, and such will fall out of sync with each other (which is what happens when a real player is playing)
    // and it will make it easier to, for example, pre-cast Immolate to reapply it right after it expires.
    // If Immolate has 1.5 seconds remaining, the sim won't start casting immolate because it would refresh before it expires
    // so Immolate would need to have ~1.49999s left of its duration for it to start casting.
    // But with this delay, Immolate's cast time would be ~1.500001 which would allow it to reapply Immolate when it has 1.5 seconds left
    // This should solve that problem if I'm thinking through this correctly.
    double spellDelay = 0.0001;

    Player(PlayerSettings& settings);

    double getGcdValue(std::string varName);
    double getSpellPower();
    void combatLog(std::string entry);
};