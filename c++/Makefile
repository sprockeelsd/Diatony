all: clean run_mac_os dylib

#define auxiliary files
PROBLEM_FILES = Utilities.cpp Tonality.cpp MajorTonality.cpp MinorTonality.cpp VoiceLeadingConstraints.cpp \
				HarmonicConstraints.cpp GeneralConstraints.cpp FourVoiceTexture.cpp Solver.cpp

#gecode libraries
LIBRARIES = -lgecodedriver -lgecodeflatzinc -lgecodefloat -lgecodeint -lgecodekernel -lgecodeminimodel \
			-lgecodesearch -lgecodeset -lgecodesupport

#compile the dynamic library for linux
so:
	g++ -fPIC $(PROBLEM_FILES) problem_wrapper.cpp -std=c++11 -shared $(LIBRARIES) -o ../sources/libgecode.so -O3 \
		-ffast-math

#compile the dynamic library for macOS
dylib: problem_wrapper.cpp
	g++ $(PROBLEM_FILES) problem_wrapper.cpp -std=c++11 -dynamiclib -fPIC -F/Library/Frameworks -framework gecode -o \
			../sources/libgecode.dylib  -ffast-math -O3
	install_name_tool -change gecode.framework/Versions/49/gecode /Library/Frameworks/gecode.framework/Versions/49/gecode\
 			../sources/libgecode.dylib

#run the main.cpp file on macOS
run_mac_os:
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o Main $(PROBLEM_FILES) Main.cpp
	./Main
	cat log.txt

test_tonality:
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o testTonality Utilities.cpp Tonality.cpp MajorTonality.cpp \
		MinorTonality.cpp testTonality.cpp
	./testTonality

#remove temporary files and dynamic library files
clean:
	rm -f  log.txt *.o *.so *.dylib ../sources/*.so ../sources/*.dylib Main testTonality

