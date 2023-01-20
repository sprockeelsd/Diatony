all: macOS

macOS:
	g++ -std=c++11 -F/Library/Frameworks -c Utilities.cpp Gauldin_csts.cpp Main.cpp

	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o Main Utilities.cpp Gauldin_csts.cpp Main.cpp

	./Main

test:
	g++ -std=c++11 -I/opt/homebrew/include -c Test.cpp

	g++ -std=c++11 -L/opt/homebrew/lib -lgecode -o Test Test.cpp

	./Test

clean:
	rm -f *.o Main Test
#/Users/sprockeelsd/gecode-release-6.2.0