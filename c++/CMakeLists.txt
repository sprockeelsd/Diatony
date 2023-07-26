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
set(MY_HEADERS headers/problem_wrapper.hpp headers/four_voice_texture.hpp four_voice_texture.cpp main.cpp Tonality.cpp headers/Tonality.hpp Utilities.cpp headers/Utilities.hpp MajorTonality.cpp headers/MajorTonality.hpp testTonality.cpp GeneralConstraints.cpp headers/GeneralConstraints.hpp MinorTonality.cpp headers/MinorTonality.hpp HarmonicConstraints.cpp headers/HarmonicConstraints.hpp VoiceLeadingConstraints.cpp headers/VoiceLeadingConstraints.hpp)
add_library(damien ${MY_HEADERS} problem_wrapper.cpp main.cpp Tonality.cpp headers/Tonality.hpp Utilities.cpp headers/Utilities.hpp MajorTonality.cpp headers/MajorTonality.hpp testTonality.cpp GeneralConstraints.cpp headers/GeneralConstraints.hpp MinorTonality.cpp headers/MinorTonality.hpp HarmonicConstraints.cpp headers/HarmonicConstraints.hpp VoiceLeadingConstraints.cpp headers/VoiceLeadingConstraints.hpp)
target_link_libraries(damien ${GECODE_LIBRARY})