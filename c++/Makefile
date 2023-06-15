all: so dylib

#compile the dynamic library for linux
so:
	g++ -fPIC problem_wrapper.cpp gecode_problem.cpp -std=c++11 -shared -lgecodedriver -lgecodeflatzinc -lgecodefloat -lgecodeint -lgecodekernel -lgecodeminimodel -lgecodesearch -lgecodeset -lgecodesupport -o ../sources/libgecode.so -O3 -ffast-math

#compile the dynamic library for macOS
dylib: problem_wrapper.cpp
	g++  problem_wrapper.cpp gecode_problem.cpp -std=c++11 -dynamiclib -fPIC -F/Library/Frameworks -framework gecode -o ../sources/libgecode.dylib  -ffast-math -O3
	install_name_tool -change gecode.framework/Versions/49/gecode /Library/Frameworks/gecode.framework/Versions/49/gecode ../sources/libgecode.dylib

#run the main.cpp file on macOS
run_mac_os:
	g++ -std=c++11 -F/Library/Frameworks -c gecode_problem.cpp Main.cpp
	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o Main gecode_problem.cpp Main.cpp
	./Main

#remove temporary files and dynamic library files
clean:
	rm -f *.o *.so *.dylib ../sources/*.so ../sources/*.dylib Main

