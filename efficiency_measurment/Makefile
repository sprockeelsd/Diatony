#default: generate MIDIfile for the best solution
all: clean compile midifile

#File directories
SRC_DIR = src
MIDI_DIR = midifile
AUX_DIR = aux
DIATONY_DIR = diatony

# Parameters for compilation
ALL_SOLS = 		all
BEST_SOL = 		best
BUILD_MIDI = 	true
NO_MIDI = 		false

#define auxiliary files
PROBLEM_FILES = $(SRC_DIR)/$(AUX_DIR)/Utilities.cpp \
				$(SRC_DIR)/$(AUX_DIR)/Tonality.cpp \
				$(SRC_DIR)/$(AUX_DIR)/MajorTonality.cpp \
				$(SRC_DIR)/$(AUX_DIR)/MinorTonality.cpp \
				$(SRC_DIR)/$(AUX_DIR)/MidiFileGeneration.cpp \
				$(SRC_DIR)/$(DIATONY_DIR)/VoiceLeadingConstraints.cpp	\
				$(SRC_DIR)/$(DIATONY_DIR)/HarmonicConstraints.cpp \
				$(SRC_DIR)/$(DIATONY_DIR)/GeneralConstraints.cpp \
				$(SRC_DIR)/$(DIATONY_DIR)/Preferences.cpp \
				$(SRC_DIR)/$(DIATONY_DIR)/FourVoiceTexture.cpp \

#MIDI handling files
MIDI_FILES = $(SRC_DIR)/$(MIDI_DIR)/Options.cpp \
			$(SRC_DIR)/$(MIDI_DIR)/MidiMessage.cpp \
			$(SRC_DIR)/$(MIDI_DIR)/MidiEvent.cpp \
			$(SRC_DIR)/$(MIDI_DIR)/MidiEventList.cpp \
			$(SRC_DIR)/$(MIDI_DIR)/Binasc.cpp \
			$(SRC_DIR)/$(MIDI_DIR)/MidiFile.cpp

# Define the log file where all the results are printed
LOG_FILE = ../out/log.txt

#gecode libraries (useful for linux compiling)
LIBRARIES = -lgecodedriver -lgecodeflatzinc -lgecodefloat -lgecodeint -lgecodekernel -lgecodeminimodel \
			-lgecodesearch -lgecodeset -lgecodesupport

#Creates a dynamic link to the Gecode framework (for Mac OS)
#With the Sonoma version of MacOS, the Gecode framework cannot be found. Creating a symbolic link solves the problem.
find_gecode_mac_os:
	ln -s /Library/Frameworks/gecode.framework gecode.framework

#remove temporary files and dynamic library files
clean:
	rm -f  ../out/log.txt ../out/statistics.txt ../out/*.o ../out/*.so ../out/*.dylib ../out/Main ../out/branch testTonality  \
	../out/MidiFiles/*.mid



#compile all files
compile:
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o ../out/Main $(PROBLEM_FILES) $(MIDI_FILES) $(PROJ_DIR)/src/Main.cpp

test_branching: clean
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o ../out/branch $(PROBLEM_FILES) $(MIDI_FILES) src/BranchingTests.cpp
	clear
	./../out/branch

parallel_run: clean
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o ../out/parallelRun $(PROBLEM_FILES) $(MIDI_FILES) src/parallelRun.cpp
	clear
	#./../out/parallelRun 0 0 0 0 0

heuristics_setup:
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o heuristics src/Utilities.cpp src/HeuristicsTestingSetup.cpp
	./heuristics
	rm -f heuristics

