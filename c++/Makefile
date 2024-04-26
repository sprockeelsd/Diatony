all: clean compile midifile

SRC_DIR = src
MIDI_DIR = midifile
AUX_DIR = aux

ALL_SOLS = 		all
BEST_SOL = 		best
BUILD_MIDI = 	true
NO_MIDI = 		false

#define auxiliary files
PROBLEM_FILES = $(SRC_DIR)/$(AUX_DIR)/Utilities.cpp $(SRC_DIR)/$(AUX_DIR)/Tonality.cpp $(SRC_DIR)/$(AUX_DIR)/MajorTonality.cpp \
				$(SRC_DIR)/$(AUX_DIR)/MinorTonality.cpp $(SRC_DIR)/VoiceLeadingConstraints.cpp	$(SRC_DIR)/HarmonicConstraints.cpp \
				$(SRC_DIR)/GeneralConstraints.cpp $(SRC_DIR)/Preferences.cpp $(SRC_DIR)/FourVoiceTexture.cpp \
				$(SRC_DIR)/Solver.cpp $(SRC_DIR)/$(AUX_DIR)/MidiFileGeneration.cpp

MIDI_FILES = $(SRC_DIR)/$(MIDI_DIR)/Options.cpp $(SRC_DIR)/$(MIDI_DIR)/MidiMessage.cpp $(SRC_DIR)/$(MIDI_DIR)/MidiEvent.cpp \
			$(SRC_DIR)/$(MIDI_DIR)/MidiEventList.cpp $(SRC_DIR)/$(MIDI_DIR)/Binasc.cpp $(SRC_DIR)/$(MIDI_DIR)/MidiFile.cpp

# Define the log file where all the results are printed
LOG_FILE = ../out/log.txt

#gecode libraries
LIBRARIES = -lgecodedriver -lgecodeflatzinc -lgecodefloat -lgecodeint -lgecodekernel -lgecodeminimodel \
			-lgecodesearch -lgecodeset -lgecodesupport

#compile the dynamic library for macOS
dylib: src/problem_wrapper.cpp
	g++ $(PROBLEM_FILES) src/problem_wrapper.cpp -std=c++11 -dynamiclib -fPIC -F/Library/Frameworks -framework gecode -o \
			../out/libgecode.dylib  -ffast-math -O3
	# replace the relative path to the Gecode library in the dylib by an absolute path
	install_name_tool -change gecode.framework/Versions/49/gecode /Library/Frameworks/gecode.framework/Versions/49/gecode\
 			../out/libgecode.dylib

#compile the dynamic library for linux
so:
	g++ -fPIC $(PROBLEM_FILES) src/problem_wrapper.cpp -std=c++11 -shared $(LIBRARIES) -o ../out/libgecode.so -O3 \
		-ffast-math

compile:
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o ../out/Main $(PROBLEM_FILES) $(MIDI_FILES) src/Main.cpp
#run the main.cpp file on macOS
midifile: clean compile
	clear
	./../out/Main $(BEST_SOL) $(BUILD_MIDI)

all_midifile: compile
	clear
	./../out/Main $(ALL_SOLS) $(BUILD_MIDI)

best_sol: compile
	clear
	./../out/Main $(BEST_SOL) $(NO_MIDI)

all_sols: compile
	clear
	./../out/Main $(ALL_SOLS) $(NO_MIDI)

test_branching: clean
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o ../out/branch $(PROBLEM_FILES) $(MIDI_FILES) src/BranchingTests.cpp
	clear
	./../out/branch

parallel_run: clean
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o ../out/parallelRun $(PROBLEM_FILES) $(MIDI_FILES) src/parallelRun.cpp
	clear
	#./../out/parallelRun 0 0 0 0 0


test_tonality:
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o testTonality $(SRC_DIR)/Utilities.cpp \
	$(SRC_DIR)/Tonality.cpp $(SRC_DIR)/MajorTonality.cpp $(SRC_DIR)/MinorTonality.cpp $(SRC_DIR)/testTonality.cpp
	./testTonality


heuristics_setup:
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o heuristics src/Utilities.cpp src/HeuristicsTestingSetup.cpp
	./heuristics
	rm -f heuristics

#Allows the executable generated to find Gecode. To execute only once
#With the Sonoma version of MacOS, gecode cannot be found. Creating a symbolic link solves the problem
find_gecode_mac_os:
	ln -s /Library/Frameworks/gecode.framework gecode.framework

#remove temporary files and dynamic library files
clean:
	rm -f  ../out/log.txt ../out/statistics.txt ../out/*.o ../out/*.so ../out/*.dylib ../out/Main ../out/branch testTonality  \
	../out/MidiFiles/*.mid

