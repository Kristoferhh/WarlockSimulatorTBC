#ifndef WARLOCK_SIMULATOR_TBC_PLAYER_SPELLS
#define WARLOCK_SIMULATOR_TBC_PLAYER_SPELLS

#include <vector>

#include "../spell/spell.h"

struct PlayerSpells {
  std::shared_ptr<Spell> life_tap;
  std::shared_ptr<Spell> seed_of_corruption;
  std::shared_ptr<Spell> shadow_bolt;
  std::shared_ptr<Spell> incinerate;
  std::shared_ptr<Spell> searing_pain;
  std::shared_ptr<Spell> corruption;
  std::shared_ptr<Spell> unstable_affliction;
  std::shared_ptr<Spell> siphon_life;
  std::shared_ptr<Spell> immolate;
  std::shared_ptr<Spell> curse_of_agony;
  std::shared_ptr<Spell> curse_of_the_elements;
  std::shared_ptr<Spell> curse_of_recklessness;
  std::shared_ptr<Spell> curse_of_doom;
  std::shared_ptr<Spell> conflagrate;
  std::shared_ptr<Spell> shadowburn;
  std::shared_ptr<Spell> death_coil;
  std::shared_ptr<Spell> shadowfury;
  std::shared_ptr<Spell> amplify_curse;
  std::shared_ptr<Spell> dark_pact;
  std::shared_ptr<Spell> destruction_potion;
  std::shared_ptr<Spell> super_mana_potion;
  std::shared_ptr<Spell> demonic_rune;
  std::shared_ptr<Spell> flame_cap;
  std::shared_ptr<Spell> blood_fury;
  std::shared_ptr<Spell> drums_of_battle;
  std::shared_ptr<Spell> drums_of_war;
  std::shared_ptr<Spell> drums_of_restoration;
  std::shared_ptr<Spell> blade_of_wizardry;
  std::shared_ptr<Spell> shattered_sun_pendant_of_acumen_aldor;
  std::shared_ptr<Spell> shattered_sun_pendant_of_acumen_scryers;
  std::shared_ptr<Spell> robe_of_the_elder_scribes;
  std::shared_ptr<Spell> mystical_skyfire_diamond;
  std::shared_ptr<Spell> insightful_earthstorm_diamond;
  std::shared_ptr<Spell> timbals_focusing_crystal;
  std::shared_ptr<Spell> mark_of_defiance;
  std::shared_ptr<Spell> the_lightning_capacitor;
  std::shared_ptr<Spell> quagmirrans_eye;
  std::shared_ptr<Spell> shiffars_nexus_horn;
  std::shared_ptr<Spell> sextant_of_unstable_currents;
  std::shared_ptr<Spell> band_of_the_eternal_sage;
  std::shared_ptr<Spell> chipped_power_core;
  std::shared_ptr<Spell> cracked_power_core;
  std::shared_ptr<Spell> mana_tide_totem;
  std::shared_ptr<Spell> judgement_of_wisdom;
  std::shared_ptr<Spell> flameshadow;
  std::shared_ptr<Spell> shadowflame;
  std::shared_ptr<Spell> spellstrike;
  std::shared_ptr<Spell> mana_etched_4_set;
  std::shared_ptr<Spell> ashtongue_talisman_of_shadows;
  std::shared_ptr<Spell> wrath_of_cenarius;
  std::shared_ptr<Spell> darkmoon_card_crusade;
  std::shared_ptr<Spell> eye_of_magtheridon;
  std::shared_ptr<Spell> improved_shadow_bolt;
  std::vector<std::shared_ptr<Spell>> power_infusion;
  std::vector<std::shared_ptr<Spell>> bloodlust;
  std::vector<std::shared_ptr<Spell>> innervate;
};

#endif