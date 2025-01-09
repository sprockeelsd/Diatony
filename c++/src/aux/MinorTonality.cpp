// Author: Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description: A class representing a minor tonality
//
#include "../../headers/aux/MinorTonality.hpp"
/**
 * Constructor
 * @param t the tonic of the tonality
 */
MinorTonality::MinorTonality(int t) : Tonality(t, MINOR_MODE, HARMONIC_MINOR_SCALE){ // calls super() constructor
    // set the chord qualities for major tonalities
    chord_qualities[FIRST_DEGREE]   = MINOR_CHORD;
    chord_qualities[SECOND_DEGREE]  = DIMINISHED_CHORD;
    chord_qualities[THIRD_DEGREE]   = MAJOR_CHORD;
    chord_qualities[FOURTH_DEGREE]  = MINOR_CHORD;
    chord_qualities[FIFTH_DEGREE]   = MAJOR_CHORD;
    chord_qualities[SIXTH_DEGREE]   = MAJOR_CHORD;
    chord_qualities[SEVENTH_DEGREE] = DIMINISHED_CHORD;
    chord_qualities[FIFTH_DEGREE_APPOGIATURA]   = MINOR_CHORD;
    /// chromatic chords
    chord_qualities[FIVE_OF_TWO]                = DOMINANT_SEVENTH_CHORD;
    chord_qualities[FIVE_OF_THREE]              = DOMINANT_SEVENTH_CHORD;
    chord_qualities[FIVE_OF_FOUR]               = DOMINANT_SEVENTH_CHORD;
    chord_qualities[FIVE_OF_FIVE]               = DOMINANT_SEVENTH_CHORD;
    chord_qualities[FIVE_OF_SIX]                = DOMINANT_SEVENTH_CHORD;
    chord_qualities[FIVE_OF_SEVEN]              = DOMINANT_SEVENTH_CHORD;
    chord_qualities[FLAT_TWO]                   = MAJOR_CHORD;
    chord_qualities[AUGMENTED_SIXTH]            = AUGMENTED_SIXTH_CHORD;

    for (int i = 0; i < chord_qualities.size(); i++){
//        IntSet set(get_all_notes_from_chord(degrees_notes[i], chord_qualities[i]));
//        scale_degrees_chords[i] = set;
    }
}

/**
 * Get the chord quality for each degree
 * @param degree a degree of the scale [0,6]
 * @return the default chord quality for the given degree
 */
int MinorTonality::get_chord_quality(int degree){
    return chord_qualities[degree];
}

/**
 * Get the chord notes for a given degree
 * @param degree a degree of the scale [0,6]
 * @return an IntSet containing the chord notes for the given degree
 */
IntSet MinorTonality::get_scale_degree_chord(int degree){
    return scale_degrees_chords[degree];
}