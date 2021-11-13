SOURCE_FILE_PATH = src/cpp/bindings.cc src/cpp/character_stats.cc src/cpp/common.cc src/cpp/player.cc src/cpp/simulation.cc src/cpp/spell.cc src/cpp/aura.cc src/cpp/damage_over_time.cc src/cpp/trinket.cc src/cpp/aura_stats.cc src/cpp/combat_log_breakdown.cc src/cpp/pet.cc src/cpp/pet_spell.cc src/cpp/pet_aura.cc src/cpp/pet_stats.cc
DEST_FILE_PATH = public/WarlockSim.js
FLAGS = -s EXPORT_NAME="WarlockSim" -O3 --bind --no-entry -s ASSERTIONS=2 -s NO_FILESYSTEM=1 -s MODULARIZE=1 -s ALLOW_MEMORY_GROWTH=1

all: $(SOURCE_FILE_PATH)
	em++ $(SOURCE_FILE_PATH) -o $(DEST_FILE_PATH) $(FLAGS)