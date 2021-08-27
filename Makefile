SOURCE_FILE_PATH = src/cpp/auras.cpp src/cpp/bindings.cpp src/cpp/common.cpp src/cpp/player.cpp src/cpp/playerSettings.cpp src/cpp/sets.cpp src/cpp/simulation.cpp src/cpp/simulationSettings.cpp src/cpp/spell.cpp src/cpp/talents.cpp src/cpp/simulationResults.cpp src/cpp/characterStats.cpp
DEST_FILE_PATH = src/js/WarlockSim.js
EXPORTED_FUNCTIONS="['_startSimulation','_allocPlayerSettings','_allocPlayer','_allocSimSettings','_allocSim','_allocAuras','_allocTalents','_allocSets','_freeAuras','_freeTalents','_freeSets','_freePlayerSettings','_freePlayer','_freeSimSettings','_freeSim','_allocStats','_freeStats']"
FLAGS = -O3 -s MODULARIZE=1 -s EXPORT_NAME="WarlockSim" -g -s SAFE_HEAP=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_FILESYSTEM=1 -s FETCH=1 -s ASSERTIONS=2 -s LLD_REPORT_UNDEFINED=1 -s EXPORTED_FUNCTIONS=$(EXPORTED_FUNCTIONS)

all: $(SOURCE_FILE_PATH)
	em++ $(SOURCE_FILE_PATH) -o $(DEST_FILE_PATH) $(FLAGS)