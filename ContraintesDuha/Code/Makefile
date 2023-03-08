all: macOS

macOS:
	g++ -std=c++11 -F/Library/Frameworks -c Tonality.cpp Utilities.cpp FourVoiceTextureConstraints.cpp FourVoiceTexture.cpp Main.cpp

	g++ -std=c++11 -F/Library/Frameworks -framework gecode -o Main Tonality.cpp Utilities.cpp FourVoiceTextureConstraints.cpp FourVoiceTexture.cpp Main.cpp

	./Main

run:
	./Main

clean:
	rm -f *.o Main *.out