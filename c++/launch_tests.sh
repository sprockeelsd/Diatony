#!/bin/bash
# Define the useful files
cpp_executable="../out/parallelRun"
inputFile="TestCases.txt"

# auxiliary files
PROBLEM_FILES="src/Utilities.cpp src/Tonality.cpp src/MajorTonality.cpp \
                src/MinorTonality.cpp src/VoiceLeadingConstraints.cpp src/HarmonicConstraints.cpp \
                src/GeneralConstraints.cpp src/Preferences.cpp src/FourVoiceTexture.cpp \
                src/Solver.cpp src/MidiFileGeneration.cpp src/parallelRun.cpp"

MIDI_FILES="src/Options.cpp src/MidiMessage.cpp src/MidiEvent.cpp src/MidiEventList.cpp \
            src/Binasc.cpp src/MidiFile.cpp"

# Compile C++ files
echo "Compiling C++ files..."
g++ -std=c++11 -F/Library/Frameworks -framework gecode -o "$cpp_executable" $PROBLEM_FILES $MIDI_FILES
echo "compilation complete"
echo "Initializing the output file"
currentDate=$(date +%Y-%m-%d_%H-%M-%S);
outFileOpt="../out/search-stats-${currentDate}.csv"  # filename of the results (with the date at the end of the file)
rm -f $outFileOpt
echo "Lauching experiments in parallel"
echo "Chord progression , Tonality, Variable selection strategy, Value selection strategy, Optimal solution found, Time to prove optimality, , \
      Best solution Statistics, Nodes traversed, Failed nodes explored, Restarts performed, Propagators executed, No \
      goods generated, Maximal depth of explored tree, number of incomplete chords, number of 4 notes diminished chords,\
      number of chords with less than 4 notes, number of fundamental chords without doubled bass, cost of melodic intervals, \
      number of common notes in the same voice,, \
      Total search statistics, Nodes traversed, Failed nodes explored, Restarts performed, Propagators executed, No \
      goods generated, Maximal depth of explored tree, , Intermediate solutions ,time, number of 4 note diminished chords,\
      number of chords with 3 notes, number of fundamental state chords without doubled bass, number of incomplete chords, \
      number of common notes in the soprano, number of common notes in the same voice, cost of melodic intervals, ," >> $outFileOpt
cat $inputFile | parallel --bar --colsep ' ' ./$cpp_executable {} >> $outFileOpt

#while IFS= read -r line; do
#    echo "Calling my_program with parameter: $line"
#    ./$cpp_executable "$line"
#    echo "Return code: $?"
#    if [ $? -ne 0 ]; then
#            echo "Error: Return code is not 0. Printing the line:"
#            echo "$line" >> "buggy_test_cases.txt"
#        fi
#done < $inputFile
