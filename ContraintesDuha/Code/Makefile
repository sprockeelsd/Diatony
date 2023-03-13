all: macOS

macOS:
	g++ -std=c++11 -F/Library/Frameworks -c Utilities.cpp Tonality.cpp MajorTonality.cpp FourVoiceTextureConstraints.cpp FourVoiceTexture.cpp Main.cpp

	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o Main Utilities.cpp Tonality.cpp MajorTonality.cpp FourVoiceTextureConstraints.cpp FourVoiceTexture.cpp Main.cpp

	./Main

test:
	g++ -std=c++11 -F/Library/Frameworks -c Utilities.cpp Tonality.cpp MajorTonality.cpp TonalityTest.cpp

	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o TonalityTest Utilities.cpp Tonality.cpp MajorTonality.cpp TonalityTest.cpp
	
	./TonalityTest

run:
	./Main

clean:
	rm -f *.o Main *.out
