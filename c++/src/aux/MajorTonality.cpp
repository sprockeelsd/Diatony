// Author: Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description: A class representing a major tonality. Inherits from the Tonality class
//
#include "../../headers/aux/MajorTonality.hpp"

/**
 * Constructor
 * @param t the tonic of the tonality
 */
MajorTonality::MajorTonality(int t) : Tonality(t, MAJOR_MODE, MAJOR_SCALE){ // calls super() constructor
    /// set the default chord qualities for each degree
    chord_qualities[FIRST_DEGREE]               = MAJOR_CHORD;
    chord_qualities[SECOND_DEGREE]              = MINOR_CHORD;
    chord_qualities[THIRD_DEGREE]               = MINOR_CHORD;
    chord_qualities[FOURTH_DEGREE]              = MAJOR_CHORD;
    chord_qualities[FIFTH_DEGREE]               = MAJOR_CHORD;
    chord_qualities[SIXTH_DEGREE]               = MINOR_CHORD;
    chord_qualities[SEVENTH_DEGREE]             = DIMINISHED_CHORD;
    chord_qualities[FIFTH_DEGREE_APPOGIATURA]   = MAJOR_CHORD;
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
        //IntSet set(get_all_notes_from_chord(degrees_notes[i], chord_qualities[i]));
        //scale_degrees_chords[i] = set;
    }
}

/**
 * Get the chord quality for each degree
 * @return a map of [degree, chord_quality] for each degree of the scale (0 to 6)
 */
int MajorTonality:: get_chord_quality(int degree){
    return chord_qualities[degree];
}

/**
 * Get the chord notes for a given degree
 * @param degree a degree of the scale [0,6]
 * @return an IntSet containing the chord notes for the given degree
 */
IntSet MajorTonality::get_scale_degree_chord(int degree){
    return scale_degrees_chords[degree];
}