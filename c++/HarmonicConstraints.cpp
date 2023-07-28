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
    /// each note is present at least once
    count(home, currentChord, tonality->get_scale_degree(degree), IRT_GQ,1); // double the bass which is also the tonic
    count(home, currentChord, tonality->get_scale_degree((degree + 2) % 7), IRT_GQ,1); // the third should be present once
    count(home, currentChord, tonality->get_scale_degree((degree + 4) % 7), IRT_GQ, 1); // the fifth should be present once
}

/**
 * Computes the cost for the number of notes in a chord, that is the number of chords that have less than 4 different values
 * @param home the instance of the problem
 * @param size the size of the chord
 * @param tonality the tonality of the piece
 * @param fullChordsVoicing the array containing all the chords in the form [bass, alto, tenor, soprano]
 * @param nOfDifferentNotes the array containing the number of different notes in each chord
 * @param costVar the variable that will contain the cost
 */
void computeNOfNotesInChordCost(Home home, int size, Tonality *tonality, IntVarArray fullChordsVoicing,
                                IntVarArray nOfDifferentNotes, IntVar costVar){
    for(int i = 0; i < size; i++) {
        IntVarArgs currentChord(fullChordsVoicing.slice(4 * i, 1, 4));
        nvalues(home, currentChord, IRT_EQ,nOfDifferentNotes[i]); // nOfDifferentNotes[i] = nOfDiffVals in current chord
    }
    count(home, nOfDifferentNotes, IntSet({1,2,3}), IRT_EQ, costVar); // costVar = nb of vars in nOfDifferentNotes that are smaller than 4
}

/**
 * Computes the cost for the number of times the fundamental is not doubled in fundamental state chords.
 * @param home the instance of the problem
 * @param size the size of the chord
 * @param tonality the tonality of the piece
 * @param chordStas the array containing the state of each chord
 * @param chordDegs the array containing the degree of each chord
 * @param fullChordsVoicing the array containing all the chords in the form [bass, alto, tenor, soprano]
 * @param nOccurrencesFund the array containing the number of times the fundamental is present in each chord
 * @param costVar the variable that will contain the cost
 */
void compute_fundamental_state_doubling_cost(const Home& home, int size, Tonality *tonality, vector<int> chordStas,
                                             vector<int> chordDegs, IntVarArray fullChordsVoicing,
                                             IntVarArray nOccurrencesFund, const IntVar& costVar){
    for(int i = 0; i < size; ++i){// for each chord
        /// if the chord is in fundamental state
        if(chordStas[i] == FUNDAMENTAL_STATE){
            IntVarArgs currentChord(fullChordsVoicing.slice(4 * i, 1, 4)); // current chord
            /// nOccurencesFund[i] = nb of times the fundamental is present in the chord
            count(home, currentChord, tonality->get_scale_degree(chordDegs[i]), IRT_EQ,nOccurrencesFund[i]);
        }
        else{ /// if not fundamental state, then we ignore it so its 0
            rel(home, nOccurrencesFund[i], IRT_EQ, 0); // don't care
        }
    }
    /// costVar = nb of vars in nOfDifferentNotes that are different from 1
    count(home, nOccurrencesFund, 1, IRT_EQ, costVar);
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                            First inversion chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the number of time each note of the chord are present in the chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chordNoteOccurrenceFirstInversion(const Home& home, Tonality *tonality, int degree, const IntVarArgs& currentChord){
    // @todo simplify this
    /// exceptions
    // if the degree is a tonal note (1 4 5) then it can be doubled
    if(tonality->get_tonal_notes().find(degree) != tonality->get_tonal_notes().end()){
        count(home, currentChord, tonality->get_scale_degree(degree + FIRST_DEGREE), IRT_GQ, 1); // the fundamental should be present at least once
        count(home, currentChord, tonality->get_scale_degree((degree + THIRD_DEGREE) % 7), IRT_GQ,1); // the third should be present exactly once
        count(home, currentChord, tonality->get_scale_degree((degree + FIFTH_DEGREE) % 7), IRT_GQ, 1); // the fifth should be present at least once
    }
    else{ /// default case: double the fundamental or the fifth of the chord
        count(home, currentChord, tonality->get_scale_degree(degree + FIRST_DEGREE), IRT_GQ, 1); // the fundamental should be present at least once
        count(home, currentChord, tonality->get_scale_degree((degree + THIRD_DEGREE) % 7), IRT_EQ,1); // the third should be present exactly once
        count(home, currentChord, tonality->get_scale_degree((degree + FIFTH_DEGREE) % 7), IRT_GQ, 1); // the fifth should be present at least once
    }
    /// @todo temporary to make all notes different, but there might be exceptions to this
    distinct(home, currentChord);
}