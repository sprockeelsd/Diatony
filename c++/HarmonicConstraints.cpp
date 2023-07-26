#include "headers/HarmonicConstraints.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 * This file contains all the harmonic constraints, that is constraints within a given chord.                          *
 * It currently contains the following constraints:                                                                    *
 *      - setToChord: sets the domain of the current chord based on the tonality and the scale degree                  *
 *      - setBass: sets the bass of the chord according to the state of the chord                                      *
 *      - chordNoteOccurrenceFundamentalState: sets the number of times each note in the chord are present for fundam- *
 *          ental state chords                                                                                         *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Set the notes of the currentChord array to the notes of the given chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void setToChord(const Home& home, Tonality* tonality, int degree, const IntVarArgs& currentChord){
    dom(home, currentChord, tonality->get_scale_degree_chord(degree));
}

/**
 * Set the bass of the chord to be the given note
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param state the state of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void setBass(const Home& home, Tonality *tonality, int degree, int state, IntVarArgs currentChord){
    dom(home, currentChord[0], tonality->get_scale_degree((degree + 2*state) % 7));
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                          Fundamental state chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * @todo change this for complete and incomplete chords later (third must be <=1 depending on the chord before and
 * @todo after if they are 5->1 and complete/incomplete)
 * @todo maybe make it a preference later
 * Sets the number of times each note of the notes of the chord are present in the chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param previous_chord_degree the degree of the following chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chordNoteOccurrenceFundamentalState(const Home& home, Tonality *tonality, int degree, int previous_chord_degree,
                                         const IntVarArgs& currentChord){
    /// exceptions
    if(degree == SIXTH_DEGREE && previous_chord_degree == FIFTH_DEGREE){ // 5->6 degree in fundamental state (broken cadence)
        count(home, currentChord, tonality->get_scale_degree(degree), IRT_EQ,1); // the fundamental should be present once
        count(home, currentChord, tonality->get_scale_degree((degree + 2) % 7), IRT_EQ,2); // double the third
        count(home, currentChord, tonality->get_scale_degree((degree + 4) % 7), IRT_EQ, 1); // the fifth should be present once
    }
        /// default = double the fundamental
    else{
        count(home, currentChord, tonality->get_scale_degree(degree), IRT_EQ,2); // double the bass which is also the tonic
        count(home, currentChord, tonality->get_scale_degree((degree + 2) % 7), IRT_EQ,1); // the third should be present once
        count(home, currentChord, tonality->get_scale_degree((degree + 4) % 7), IRT_EQ, 1); // the fifth should be present once
    }

    /// number of different note values in the chord
    if(tonality->get_chord_qualities()[degree] == DIMINISHED_CHORD){
        if( not (degree == SECOND_DEGREE && previous_chord_degree == SIXTH_DEGREE && tonality->get_mode() == MINOR_MODE)){ /// this is the only case where it is not possible (for now)
            nvalues(home, currentChord, IRT_EQ,3); // there should only be 3 different notes
        }
    }
    else{
        distinct(home, currentChord); // all notes should be different
    }
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                            First inversion chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/