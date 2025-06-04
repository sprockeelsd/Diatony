#default: generate MIDIfile for the best solution
all: clean compile midifile

#include the file variables
include $(dir $(lastword $(MAKEFILE_LIST)))file_variables.mk

#if some file does not find gecode properly, add it here in a dummy variable and update the makefile detection in CLion should fix it

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