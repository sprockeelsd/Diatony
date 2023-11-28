all: clean run_mac_os dylib

SRC_DIR = src



#define auxiliary files
PROBLEM_FILES = $(SRC_DIR)/Utilities.cpp $(SRC_DIR)/Tonality.cpp $(SRC_DIR)/MajorTonality.cpp \
				$(SRC_DIR)/MinorTonality.cpp $(SRC_DIR)/VoiceLeadingConstraints.cpp	$(SRC_DIR)/HarmonicConstraints.cpp \
				$(SRC_DIR)/GeneralConstraints.cpp $(SRC_DIR)/Preferences.cpp $(SRC_DIR)/FourVoiceTexture.cpp \
				$(SRC_DIR)/Solver.cpp

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

#run the main.cpp file on macOS
run_mac_os:
	clear
	rm -f $(LOG_FILE)
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o ../out/Main $(PROBLEM_FILES) src/Main.cpp
	./../out/Main

test_tonality:
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o testTonality Utilities.cpp Tonality.cpp MajorTonality.cpp \
		MinorTonality.cpp testTonality.cpp
	./testTonality

#Allows the executable generated to find Gecode. To execute only once
#With the Sonoma version of MacOS, gecode cannot be found. Creating a symbolic link solves the problem
find_gecode_mac_os:
	ln -s /Library/Framework/gecode.framework gecode.framework

#remove temporary files and dynamic library files
clean:
	rm -f  ../out/log.txt ../out/*.o *.so *.dylib ../out/*.so ../out/*.dylib ../out/Main testTonality

