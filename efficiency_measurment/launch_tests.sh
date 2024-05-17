#!/bin/bash
# Define the useful files
cpp_executable="out/parallelRun"
inputFile="TestCases.txt"

# auxiliary files
PROBLEM_FILES="../c++/src/aux/Utilities.cpp ../c++/src/aux/Tonality.cpp ../c++/src/aux/MajorTonality.cpp \
                ../c++/src/aux/MinorTonality.cpp ../c++/src/diatony/VoiceLeadingConstraints.cpp ../c++/src/diatony/HarmonicConstraints.cpp \
                ../c++/src/diatony/GeneralConstraints.cpp ../c++/src/diatony/Preferences.cpp ../c++/src/diatony/FourVoiceTexture.cpp \
                ../c++/src/aux/MidiFileGeneration.cpp parallelRun.cpp"

MIDI_FILES="../c++/src/midifile/Options.cpp ../c++/src/midifile/MidiMessage.cpp ../c++/src/midifile/MidiEvent.cpp ../c++/src/midifile/MidiEventList.cpp \
            ../c++/src/midifile/Binasc.cpp ../c++/src/midifile/MidiFile.cpp"

# Compile C++ files
echo "Compiling C++ files..."
g++ -std=c++11 -F/Library/Frameworks -framework gecode -o "$cpp_executable" $PROBLEM_FILES $MIDI_FILES
echo "compilation complete"
echo "Initializing the output file"
currentDate=$(date +%Y-%m-%d_%H-%M-%S);
outFileOpt="out/search-stats-${currentDate}.csv"  # filename of the results (with the date at the end of the file)
rm -f $outFileOpt
echo "Lauching experiments in parallel"
echo "Chord progression , Tonality, Optimal solution found, Time to prove optimality, , \
      Best solution Statistics, Nodes traversed, Failed nodes explored, Restarts performed, Propagators executed, No goods generated, Maximal depth of explored tree,, \
      number of incomplete chords, number of 4 notes diminished chords, number of chords with 3 notes, cost of melodic intervals, number of common notes in the same voice,,\
      Total search statistics, Nodes traversed, Failed nodes explored, Restarts performed, Propagators executed, No goods generated, Maximal depth of explored tree, \
      Intermediate solutions, time, \
      number of incomplete chords, number of 4 notes diminished chords, number of chords with 3 notes, cost of melodic intervals, number of common notes in the same voice,,\
      " >> $outFileOpt
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
python3 graphs.py $outFileOpt
