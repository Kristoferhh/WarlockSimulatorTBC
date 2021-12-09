SOURCE_FILE_PATH = src/cpp/bindings.cc src/cpp/entity.cc src/cpp/spell/on_resist_proc.cc src/cpp/spell/on_dot_tick_proc.cc src/cpp/spell/on_damage_proc.cc src/cpp/spell/on_crit_proc.cc src/cpp/spell/spell_proc.cc src/cpp/spell/on_hit_proc.cc src/cpp/spell/life_tap.cc src/cpp/stat.cc src/cpp/rng.cc src/cpp/spell/mana_over_time.cc src/cpp/spell/mana_potion.cc src/cpp/common.cc src/cpp/player/player.cc src/cpp/simulation.cc src/cpp/spell/spell.cc src/cpp/aura.cc src/cpp/spell/damage_over_time.cc src/cpp/trinket.cc src/cpp/pet/pet.cc src/cpp/pet/pet_spell.cc
DEST_FILE_PATH = public/WarlockSim.js
FLAGS = -s EXPORT_NAME="WarlockSim" -O3 --bind --no-entry -s ASSERTIONS=2 -s NO_FILESYSTEM=1 -s MODULARIZE=1 -s ALLOW_MEMORY_GROWTH=1

all: $(SOURCE_FILE_PATH)
	em++ $(SOURCE_FILE_PATH) -o $(DEST_FILE_PATH) $(FLAGS)