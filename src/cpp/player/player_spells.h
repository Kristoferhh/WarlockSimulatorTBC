#ifndef WARLOCK_SIMULATOR_TBC_PLAYER_SPELLS
#define WARLOCK_SIMULATOR_TBC_PLAYER_SPELLS

#include <vector>

#include "player_spell/player_spell.h"

struct PlayerSpells {
  std::shared_ptr<PlayerSpell> life_tap;
  std::shared_ptr<PlayerSpell> seed_of_corruption;
  std::shared_ptr<PlayerSpell> shadow_bolt;
  std::shared_ptr<PlayerSpell> incinerate;
  std::shared_ptr<PlayerSpell> searing_pain;
  std::shared_ptr<PlayerSpell> corruption;
  std::shared_ptr<PlayerSpell> unstable_affliction;
  std::shared_ptr<PlayerSpell> siphon_life;
  std::shared_ptr<PlayerSpell> immolate;
  std::shared_ptr<PlayerSpell> curse_of_agony;
  std::shared_ptr<PlayerSpell> curse_of_the_elements;
  std::shared_ptr<PlayerSpell> curse_of_recklessness;
  std::shared_ptr<PlayerSpell> curse_of_doom;
  std::shared_ptr<PlayerSpell> conflagrate;
  std::shared_ptr<PlayerSpell> shadowburn;
  std::shared_ptr<PlayerSpell> death_coil;
  std::shared_ptr<PlayerSpell> shadowfury;
  std::shared_ptr<PlayerSpell> amplify_curse;
  std::shared_ptr<PlayerSpell> dark_pact;
  std::shared_ptr<PlayerSpell> destruction_potion;
  std::shared_ptr<PlayerSpell> super_mana_potion;
  std::shared_ptr<PlayerSpell> demonic_rune;
  std::shared_ptr<PlayerSpell> flame_cap;
  std::shared_ptr<PlayerSpell> blood_fury;
  std::shared_ptr<PlayerSpell> drums_of_battle;
  std::shared_ptr<PlayerSpell> drums_of_war;
  std::shared_ptr<PlayerSpell> drums_of_restoration;
  std::shared_ptr<PlayerSpell> blade_of_wizardry;
  std::shared_ptr<PlayerSpell> shattered_sun_pendant_of_acumen_aldor;
  std::shared_ptr<PlayerSpell> shattered_sun_pendant_of_acumen_scryers;
  std::shared_ptr<PlayerSpell> robe_of_the_elder_scribes;
  std::shared_ptr<PlayerSpell> mystical_skyfire_diamond;
  std::shared_ptr<PlayerSpell> insightful_earthstorm_diamond;
  std::shared_ptr<PlayerSpell> timbals_focusing_crystal;
  std::shared_ptr<PlayerSpell> mark_of_defiance;
  std::shared_ptr<PlayerSpell> the_lightning_capacitor;
  std::shared_ptr<PlayerSpell> quagmirrans_eye;
  std::shared_ptr<PlayerSpell> shiffars_nexus_horn;
  std::shared_ptr<PlayerSpell> sextant_of_unstable_currents;
  std::shared_ptr<PlayerSpell> band_of_the_eternal_sage;
  std::shared_ptr<PlayerSpell> chipped_power_core;
  std::shared_ptr<PlayerSpell> cracked_power_core;
  std::shared_ptr<PlayerSpell> mana_tide_totem;
  std::shared_ptr<PlayerSpell> judgement_of_wisdom;
  std::shared_ptr<PlayerSpell> flameshadow;
  std::shared_ptr<PlayerSpell> shadowflame;
  std::shared_ptr<PlayerSpell> spellstrike;
  std::shared_ptr<PlayerSpell> mana_etched_4_set;
  std::shared_ptr<PlayerSpell> ashtongue_talisman_of_shadows;
  std::shared_ptr<PlayerSpell> wrath_of_cenarius;
  std::shared_ptr<PlayerSpell> darkmoon_card_crusade;
  std::shared_ptr<PlayerSpell> eye_of_magtheridon;
  std::shared_ptr<PlayerSpell> improved_shadow_bolt;
  std::vector<std::shared_ptr<PlayerSpell>> power_infusion;
  std::vector<std::shared_ptr<PlayerSpell>> bloodlust;
  std::vector<std::shared_ptr<PlayerSpell>> innervate;
};

#endif