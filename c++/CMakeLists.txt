cmake_minimum_required(VERSION 3.10)
project(MyProject)
set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build architectures" FORCE)
set(CMAKE_CXX_STANDARD 11)
# Set path to the Gecode library and headers
# Tell CMake where to find the Gecode library
find_library(GECODE_LIBRARY
        NAMES Gecode
        PATHS /Library/Frameworks
        NO_DEFAULT_PATH)
message(STATUS "Gecode library: ${GECODE_LIBRARY}")
# Add the include directory to the include path
include_directories(/Library/Frameworks/Gecode.framework/Headers)

# Add the include directories to the include path
include_directories(${GECODE_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers)
# Create the dynamic library and link to the Gecode library
set(MY_HEADERS
        headers/diatony/FourVoiceTexture.hpp
        src/diatony/FourVoiceTexture.cpp
        src/main.cpp src/Tonality.cpp
        headers/Tonality.hpp
        src/Utilities.cpp
        headers/Utilities.hpp
        src/MajorTonality.cpp
        headers/MajorTonality.hpp
        src/testTonality.cpp
        src/diatony/GeneralConstraints.cpp
        headers/diatony/GeneralConstraints.hpp
        src/MinorTonality.cpp
        headers/MinorTonality.hpp
        src/diatony/HarmonicConstraints.cpp
        headers/diatony/HarmonicConstraints.hpp
        src/diatony/VoiceLeadingConstraints.cpp
        headers/diatony/VoiceLeadingConstraints.hpp
        src/Solver.cpp
        headers/Solver.hpp
        src/diatony/Preferences.cpp
        headers/diatony/Preferences.hpp
        src/midifile/MidiFile.cpp
        src/midifile/Binasc.cpp
        src/midifile/MidiEvent.cpp
        src/midifile/MidiEventList.cpp
        src/midifile/MidiMessage.cpp
        src/midifile/Options.cpp
        src/MidiFileGeneration.cpp
        headers/MidiFileGeneration.hpp
        src/BranchingTests.cpp
        src/HeuristicsTestingSetup.cpp
        src/parallelRun.cpp
)
add_library(damien ${MY_HEADERS} src/main.cpp src/Tonality.cpp headers/Tonality.hpp src/Utilities.cpp headers/Utilities.hpp src/MajorTonality.cpp headers/MajorTonality.hpp src/testTonality.cpp src/diatony/GeneralConstraints.cpp headers/diatony/GeneralConstraints.hpp src/MinorTonality.cpp headers/MinorTonality.hpp src/diatony/HarmonicConstraints.cpp headers/diatony/HarmonicConstraints.hpp src/diatony/VoiceLeadingConstraints.cpp headers/diatony/VoiceLeadingConstraints.hpp
        src/Solver.cpp
        headers/Solver.hpp
        src/diatony/Preferences.cpp
        headers/diatony/Preferences.hpp
        src/midifile/MidiFile.cpp
        src/midifile/Binasc.cpp
        src/midifile/MidiEvent.cpp
        src/midifile/Options.cpp
        src/BranchingTests.cpp
        src/HeuristicsTestingSetup.cpp
)
target_link_libraries(damien ${GECODE_LIBRARY})