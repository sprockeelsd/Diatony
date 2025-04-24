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
				$(SRC_DIR)/$(DIATONY_DIR)/TonalProgression.cpp \
				$(SRC_DIR)/$(DIATONY_DIR)/TonalProgressionParameters.cpp \
				$(SRC_DIR)/$(DIATONY_DIR)/ModulationParameters.cpp \
				$(SRC_DIR)/$(DIATONY_DIR)/FourVoiceTexture.cpp \
				$(SRC_DIR)/$(DIATONY_DIR)/FourVoiceTextureParameters.cpp \

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

dylib:
	g++ $(PROBLEM_FILES) $(MIDI_FILES) -std=c++11 -dynamiclib -fPIC -F/Library/Frameworks -framework gecode -o ../out/diatony.dylib
	install_name_tool -change gecode.framework/Versions/49/gecode /Library/Frameworks/gecode.framework/Versions/49/gecode ../out/diatony.dylib

#compile all files and generate executable
compile:
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o ../out/Main $(PROBLEM_FILES) $(MIDI_FILES) src/Main.cpp
	install_name_tool -change gecode.framework/Versions/49/gecode /Library/Frameworks/gecode.framework/Versions/49/gecode ../out/Main

#generate the midifile for the best solution
midifile: clean compile
	clear
	./../out/Main $(BEST_SOL) $(BUILD_MIDI)
#generate the midifile for all solutions
all_midifile: compile
	clear
	./../out/Main $(ALL_SOLS) $(BUILD_MIDI)
#print the best solution to the console
best_sol: compile
	clear
	./../out/Main $(BEST_SOL) $(NO_MIDI)
#print all solutions to the console
all_sols: compile
	clear
	./../out/Main $(ALL_SOLS) $(NO_MIDI)

#Creates a dynamic link to the Gecode framework (for Mac OS)
#With the Sonoma version of MacOS, the Gecode framework cannot be found. Creating a symbolic link solves the problem.
find_gecode_mac_os:
	ln -s /Library/Frameworks/gecode.framework gecode.framework

#remove temporary files and dynamic library files
clean:
	rm -f  ../out/log.txt ../out/statistics.txt ../out/*.o ../out/*.so ../out/*.dylib ../out/Main ../out/branch testTonality  \
	../out/MidiFiles/*.mid ../out/diatony.dylib