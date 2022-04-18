SOURCE_FILE_PATH = cpp/WarlockSimulatorTBC/src/bindings.cc cpp/WarlockSimulatorTBC/src/spell.cc cpp/WarlockSimulatorTBC/src/entity.cc cpp/WarlockSimulatorTBC/src/on_resist_proc.cc cpp/WarlockSimulatorTBC/src/on_dot_tick_proc.cc cpp/WarlockSimulatorTBC/src/on_damage_proc.cc cpp/WarlockSimulatorTBC/src/on_crit_proc.cc cpp/WarlockSimulatorTBC/src/spell_proc.cc cpp/WarlockSimulatorTBC/src/on_hit_proc.cc cpp/WarlockSimulatorTBC/src/life_tap.cc cpp/WarlockSimulatorTBC/src/stat.cc cpp/WarlockSimulatorTBC/src/rng.cc cpp/WarlockSimulatorTBC/src/mana_over_time.cc cpp/WarlockSimulatorTBC/src/mana_potion.cc cpp/WarlockSimulatorTBC/src/common.cc cpp/WarlockSimulatorTBC/src/player.cc cpp/WarlockSimulatorTBC/src/simulation.cc cpp/WarlockSimulatorTBC/src/aura.cc cpp/WarlockSimulatorTBC/src/damage_over_time.cc cpp/WarlockSimulatorTBC/src/trinket.cc cpp/WarlockSimulatorTBC/src/pet.cc
DEST_FILE_PATH = public/WarlockSim.js
FLAGS = -s EXPORT_NAME="WarlockSim" --bind --no-entry -O2 -s ASSERTIONS=2 -s NO_FILESYSTEM=1 -s MODULARIZE=1 -s ALLOW_MEMORY_GROWTH=1 -std=c++20

all: $(SOURCE_FILE_PATH)
	em++ $(SOURCE_FILE_PATH) -o $(DEST_FILE_PATH) $(FLAGS)