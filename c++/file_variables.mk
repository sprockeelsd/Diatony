# This file defines all the variables used for the compilation of the Diatony project. Having them outside of the
# Makefile allows to use the same variables when compiling the Harmoniser project as well.

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
				$(SRC_DIR)/$(DIATONY_DIR)/FourVoiceTextureParameters.cpp \
				$(SRC_DIR)/$(DIATONY_DIR)/SolveDiatony.cpp \
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