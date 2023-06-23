#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"
#include "headers/MajorTonality.hpp"


MajorTonality::MajorTonality(int t) : Tonality(t, MAJOR_SCALE){ // calls super() constructor
    // set the chord qualities for major tonalities
    chord_qualities[1] = MAJOR_CHORD;
    chord_qualities[2] = MINOR_CHORD;
    chord_qualities[3] = MINOR_CHORD;
    chord_qualities[4] = MAJOR_CHORD;
    chord_qualities[5] = MAJOR_CHORD;
    chord_qualities[6] = MINOR_CHORD;
    chord_qualities[7] = DIMINISHED_CHORD;

    for (int i = 0; i < chord_qualities.size(); i++){
        scale_degrees_chords[i+1] = getAllNotesFromChord(degrees_notes[i], chord_qualities[i+1]); // @todo change when degrees_notes is a map
    }
}