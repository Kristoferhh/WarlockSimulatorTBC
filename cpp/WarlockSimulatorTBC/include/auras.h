#pragma once

#include "aura.h"
#include "damage_over_time.h"

struct Auras {
  std::shared_ptr<DamageOverTime> corruption;
  std::shared_ptr<DamageOverTime> unstable_affliction;
  std::shared_ptr<DamageOverTime> siphon_life;
  std::shared_ptr<DamageOverTime> immolate;
  std::shared_ptr<DamageOverTime> curse_of_agony;
  std::shared_ptr<DamageOverTime> curse_of_doom;
  std::shared_ptr<Aura> improved_shadow_bolt;
  std::shared_ptr<Aura> curse_of_the_elements;
  std::shared_ptr<Aura> curse_of_recklessness;
  std::shared_ptr<Aura> shadow_trance;
  std::shared_ptr<Aura> amplify_curse;
  std::shared_ptr<Aura> power_infusion;
  std::shared_ptr<Aura> innervate;
  std::shared_ptr<Aura> blood_fury;
  std::shared_ptr<Aura> destruction_potion;
  std::shared_ptr<Aura> flame_cap;
  std::shared_ptr<Aura> bloodlust;
  std::shared_ptr<Aura> drums_of_battle;
  std::shared_ptr<Aura> drums_of_war;
  std::shared_ptr<Aura> drums_of_restoration;
  std::shared_ptr<Aura> band_of_the_eternal_sage;
  std::shared_ptr<Aura> wrath_of_cenarius;
  std::shared_ptr<Aura> blade_of_wizardry;
  std::shared_ptr<Aura> shattered_sun_pendant_of_acumen_aldor;
  std::shared_ptr<Aura> robe_of_the_elder_scribes;
  std::shared_ptr<Aura> mystical_skyfire_diamond;
  std::shared_ptr<Aura> eye_of_magtheridon;
  std::shared_ptr<Aura> sextant_of_unstable_currents;
  std::shared_ptr<Aura> quagmirrans_eye;
  std::shared_ptr<Aura> shiffars_nexus_horn;
  std::shared_ptr<Aura> ashtongue_talisman_of_shadows;
  std::shared_ptr<Aura> darkmoon_card_crusade;
  std::shared_ptr<Aura> the_lightning_capacitor;
  std::shared_ptr<Aura> flameshadow;  // Shadowpower
  std::shared_ptr<Aura> shadowflame;  // Firepower
  std::shared_ptr<Aura> spellstrike;
  std::shared_ptr<Aura> mana_etched_4_set;
  std::shared_ptr<Aura> chipped_power_core;
  std::shared_ptr<Aura> cracked_power_core;
  std::shared_ptr<Aura> mana_tide_totem;
  std::shared_ptr<Aura> airmans_ribbon_of_gallantry;
  std::shared_ptr<Aura> demonic_frenzy;
  std::unique_ptr<Aura> black_book;
  std::unique_ptr<Aura> battle_squawk;
};