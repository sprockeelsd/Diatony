all: run_mac_os dylib

#compile the dynamic library for linux
so:
	g++ -fPIC Utilities.cpp Tonality.cpp MajorTonality.cpp MinorTonality.cpp problem_wrapper.cpp four_voice_texture.cpp -std=c++11 -shared -lgecodedriver -lgecodeflatzinc -lgecodefloat -lgecodeint -lgecodekernel -lgecodeminimodel -lgecodesearch -lgecodeset -lgecodesupport -o ../sources/libgecode.so -O3 -ffast-math

#compile the dynamic library for macOS TODO maybe add other files if it is necessary to make it work
dylib: problem_wrapper.cpp
	g++  Utilities.cpp Tonality.cpp MajorTonality.cpp MinorTonality.cpp VoiceLeadingConstraints.cpp HarmonicConstraints.cpp GeneralConstraints.cpp problem_wrapper.cpp four_voice_texture.cpp -std=c++11 -dynamiclib -fPIC -F/Library/Frameworks -framework gecode -o ../sources/libgecode.dylib  -ffast-math -O3
	install_name_tool -change gecode.framework/Versions/49/gecode /Library/Frameworks/gecode.framework/Versions/49/gecode ../sources/libgecode.dylib

#run the main.cpp file on macOS
run_mac_os: Main
	./Main

Main:
	g++ -std=c++11 -F/Library/Frameworks -c Utilities.cpp Tonality.cpp MajorTonality.cpp MinorTonality.cpp VoiceLeadingConstraints.cpp HarmonicConstraints.cpp GeneralConstraints.cpp four_voice_texture.cpp Main.cpp
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o Main Utilities.cpp Tonality.cpp MajorTonality.cpp MinorTonality.cpp VoiceLeadingConstraints.cpp HarmonicConstraints.cpp GeneralConstraints.cpp four_voice_texture.cpp Main.cpp

test_tonality:
	g++ -std=c++11 -F/Library/Frameworks -c Utilities.cpp Tonality.cpp MajorTonality.cpp MinorTonality.cpp testTonality.cpp
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o testTonality Utilities.cpp Tonality.cpp MajorTonality.cpp MinorTonality.cpp testTonality.cpp
	./testTonality

#remove temporary files and dynamic library files
clean:
	rm -f *.o *.so *.dylib ../sources/*.so ../sources/*.dylib Main

