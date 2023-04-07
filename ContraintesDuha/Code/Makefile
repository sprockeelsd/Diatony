all: macOS

macOS:
	g++ -std=c++11 -F/Library/Frameworks -c Utilities.cpp Tonality.cpp MajorTonality.cpp MinorTonality.cpp FourVoiceTextureConstraints.cpp FourVoiceTexture.cpp Main.cpp

	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o Main Utilities.cpp Tonality.cpp MajorTonality.cpp MinorTonality.cpp FourVoiceTextureConstraints.cpp FourVoiceTexture.cpp Main.cpp

	./Main

test:
	g++ -std=c++11 -F/Library/Frameworks -c Utilities.cpp Tonality.cpp MajorTonality.cpp FourVoiceTextureConstraints.cpp FourVoiceTexture.cpp Tests.cpp

	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o Tests Utilities.cpp Tonality.cpp MajorTonality.cpp FourVoiceTextureConstraints.cpp FourVoiceTexture.cpp Tests.cpp

	./Tests

run:
	./Main

clean:
	rm -f *.o Main Tests *.out
