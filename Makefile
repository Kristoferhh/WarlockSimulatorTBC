SOURCE_FILE_PATH = src/cpp/auras.cpp src/cpp/bindings.cpp src/cpp/common.cpp src/cpp/player.cpp src/cpp/playerSettings.cpp src/cpp/sets.cpp src/cpp/simulation.cpp src/cpp/simulationSettings.cpp src/cpp/spell.cpp src/cpp/talents.cpp src/cpp/characterStats.cpp src/cpp/aura.cpp src/cpp/damageOverTime.cpp src/cpp/trinket.cpp src/cpp/items.cpp src/cpp/auraStats.cpp src/cpp/combatLogBreakdown.cpp src/cpp/pet.cpp src/cpp/petSpell.cpp src/cpp/petAura.cpp src/cpp/petStats.cpp
DEST_FILE_PATH = public/WarlockSim.js
FLAGS = -s EXPORT_NAME="WarlockSim" --bind --no-entry -s ASSERTIONS=2 -s NO_FILESYSTEM=1 -s MODULARIZE=1 -s ALLOW_MEMORY_GROWTH=1

all: $(SOURCE_FILE_PATH)
	em++ $(SOURCE_FILE_PATH) -o $(DEST_FILE_PATH) $(FLAGS)