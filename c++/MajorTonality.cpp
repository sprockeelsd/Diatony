#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"
#include "headers/MajorTonality.hpp"

/**
 * Constructor
 * @param t the tonic of the tonality
 */
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
        scale_degrees_chords[i+1] = getAllNotesFromChord(degrees_notes[i+1], chord_qualities[i+1]); // @todo change when degrees_notes is a map
    }
}

/**
 * Get the chord quality for each degree
 * @return a map of [degree, chord_quality] for each degree of the scale (1 to 7)
 */
map<int, vector<int>> MajorTonality::get_chord_qualities(){
    return chord_qualities;
}

/**
 * Get the chord notes for each degree
 * @return a map of [degree, chord] for each degree of the scale (1 to 7)
 */
map<int, IntSet> MajorTonality::get_scale_degrees_chords(){
    return scale_degrees_chords;
}

/**
 * Get the chord notes for a given degree
 * @param degree a degree of the scale [1,7]
 * @return an IntSet containing the chord notes for the given degree
 */
IntSet MajorTonality::get_scale_degree_chord(int degree){
    return scale_degrees_chords[degree];
}