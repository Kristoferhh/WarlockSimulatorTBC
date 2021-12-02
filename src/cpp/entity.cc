#include "entity.h"

Entity::Entity(Player* player, EntityType entity_type)
    : player(player),
      entity_type(entity_type),
      stats(CharacterStats()),
      cast_time_remaining(0),
      gcd_remaining(0),
      five_second_rule_timer_remaining(5),
      spirit_tick_timer_remaining(2) {}