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
set(MY_HEADERS headers/problem_wrapper.hpp headers/FourVoiceTexture.hpp src/FourVoiceTexture.cpp src/main.cpp src/Tonality.cpp headers/Tonality.hpp src/Utilities.cpp headers/Utilities.hpp src/MajorTonality.cpp headers/MajorTonality.hpp src/testTonality.cpp src/GeneralConstraints.cpp headers/GeneralConstraints.hpp src/MinorTonality.cpp headers/MinorTonality.hpp src/HarmonicConstraints.cpp headers/HarmonicConstraints.hpp src/VoiceLeadingConstraints.cpp headers/VoiceLeadingConstraints.hpp
        src/Solver.cpp
        headers/Solver.hpp
        src/Preferences.cpp
        headers/Preferences.hpp
        src/MidiFile.cpp
        src/Binasc.cpp
        src/MidiEvent.cpp
        src/MidiEventList.cpp
        src/MidiMessage.cpp
        src/Options.cpp
        src/MidiFileGeneration.cpp
        headers/MidiFileGeneration.hpp
)
add_library(damien ${MY_HEADERS} src/problem_wrapper.cpp src/main.cpp src/Tonality.cpp headers/Tonality.hpp src/Utilities.cpp headers/Utilities.hpp src/MajorTonality.cpp headers/MajorTonality.hpp src/testTonality.cpp src/GeneralConstraints.cpp headers/GeneralConstraints.hpp src/MinorTonality.cpp headers/MinorTonality.hpp src/HarmonicConstraints.cpp headers/HarmonicConstraints.hpp src/VoiceLeadingConstraints.cpp headers/VoiceLeadingConstraints.hpp
        src/Solver.cpp
        headers/Solver.hpp
        src/Preferences.cpp
        headers/Preferences.hpp
        src/MidiFile.cpp
        src/Binasc.cpp
        src/MidiEvent.cpp
        src/Options.cpp
)
target_link_libraries(damien ${GECODE_LIBRARY})