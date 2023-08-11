#include "headers/MinorTonality.hpp"
/**
 * Constructor
 * @param t the tonic of the tonality
 */
MinorTonality::MinorTonality(int t) : Tonality(t, MINOR_MODE, HARMONIC_MINOR_SCALE){ // calls super() constructor
    // set the chord qualities for major tonalities
    chord_qualities[0] = MINOR_CHORD;
    chord_qualities[1] = DIMINISHED_CHORD;
    chord_qualities[2] = MAJOR_CHORD;
    chord_qualities[3] = MINOR_CHORD;
    chord_qualities[4] = MAJOR_CHORD;
    chord_qualities[5] = MAJOR_CHORD;
    chord_qualities[6] = DIMINISHED_CHORD;

    for (int i = 0; i < chord_qualities.size(); i++){
        IntSet set(get_all_notes_from_chord(degrees_notes[i], chord_qualities[i]));
        scale_degrees_chords[i] = set;
    }
}

/**
 * Get the chord quality for each degree
 * @return a map of [degree, chord_quality] for each degree of the scale (0 to 6)
 */
map<int, vector<int>> MinorTonality::get_chord_qualities(){
    return chord_qualities;
}

/**
 * Get the chord notes for each degree
 * @return a map of [degree, chord] for each degree of the scale (0 to 6)
 */
map<int, IntSet> MinorTonality::get_scale_degrees_chords(){
    return scale_degrees_chords;
}

/**
 * Get the chord notes for a given degree
 * @param degree a degree of the scale [0,6]
 * @return an IntSet containing the chord notes for the given degree
 */
IntSet MinorTonality::get_scale_degree_chord(int degree){
    return scale_degrees_chords[degree];
}