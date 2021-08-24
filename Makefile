SOURCE_FILE_PATH = src/cpp/bindings.cpp src/cpp/simulation.cpp src/cpp/player.cpp src/cpp/common.cpp
DEST_FILE_PATH = src/js/WarlockSim.js
EXPORTED_FUNCTIONS="['_startSimulation']"
FLAGS = -O2 -s MODULARIZE=1 -s EXPORT_NAME="WarlockSim" -s NO_FILESYSTEM=1 -s EXPORTED_FUNCTIONS=$(EXPORTED_FUNCTIONS)

all: $(SOURCE_FILE_PATH)
	em++ $(SOURCE_FILE_PATH) -o $(DEST_FILE_PATH) $(FLAGS)