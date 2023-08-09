#include "headers/HarmonicConstraints.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 * This file contains all the harmonic constraints, that is constraints within a given chord.                          *
 * It currently contains the following constraints:                                                                    *
 *      - set_to_chord: sets the domain of the current chord based on the tonality and the scale degree                *
 *      - set_bass: sets the bass of the chord according to the state of the chord                                     *
 *      - chord_note_occurrence_fundamental_state: sets the number of times each note in the chord are present for     *
 *          fundamental state chords                                                                                   *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Set the notes of the currentChord array to the notes of the given chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void set_to_chord(const Home& home, Tonality *tonality, int degree, const IntVarArgs& currentChord){
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
void set_bass(const Home& home, Tonality *tonality, int degree, int state, IntVarArgs currentChord){
    /// (degree + 2) * state gives the bas note of the chord since the state is 0 for fundamental, 1 for first inversion
    /// and 2 for second inversion
    dom(home, currentChord[0], tonality->get_scale_degree((degree + 2 * state) % 7));
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
 * @param degree the degree of the chord
 * @param nVoices the number of voices
 * @param tonality the tonality of the piece
 * @param nDifferentValuesInDiminishedChord the number of different values in the diminished chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chord_note_occurrence_fundamental_state(Home home, int degree, int nVoices, Tonality *tonality,
                                             const IntVar &nDifferentValuesInDiminishedChord,
                                             const IntVarArgs &currentChord) {
    /// if the chord is a diminished seventh degree, the third must be doubled
    if(degree == SEVENTH_DEGREE && tonality->get_chord_qualities()[degree] == DIMINISHED_CHORD){
        IntVar nOfThirds(home,0,nVoices); // @todo make argument variable
        count(home, currentChord, tonality->get_scale_degree(degree + FIRST_DEGREE), IRT_EQ,
              nOfThirds);
        rel(home, expr(home, nDifferentValuesInDiminishedChord == nVoices), BOT_IMP,
            expr(home, nOfThirds == 2), true);
    }
    /// each note is present at least once, doubling is determined by the costs
    count(home, currentChord, tonality->get_scale_degree(degree), IRT_GQ,1);
    count(home, currentChord, tonality->get_scale_degree((degree + 2) % 7), IRT_GQ,1);
    count(home, currentChord, tonality->get_scale_degree((degree + 4) % 7), IRT_GQ, 1);
}

/**
 * Computes the cost for the number of notes in a chord, that is the number of chords that have less than 4 different
 * values
 * @param home the instance of the problem
 * @param size the size of the chord
 * @param nVoices the number of voices
 * @param tonality the tonality of the piece
 * @param fullChordsVoicing the array containing all the chords in the form [bass, alto, tenor, soprano]
 * @param nOfDifferentNotes the array containing the number of different notes in each chord
 * @param costVar the variable that will contain the cost
 */
void compute_n_of_notes_in_chord_cost(const Home& home, int size, int nVoices, IntVarArray fullChordsVoicing,
                                      IntVarArray nOfDifferentNotes, const IntVar& costVar) {
    for(int i = 0; i < size; i++) {
        IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices));

        // nOfDifferentNotes[i] = nOfDiffVals in current chord
        nvalues(home, currentChord, IRT_EQ,nOfDifferentNotes[i]);
    }
    /// costVar = nb of vars in nOfDifferentNotes that are smaller than 4
    count(home, nOfDifferentNotes, IntSet({1,2,3}), IRT_EQ, costVar);
}

/**
 * Computes the cost for the number of times the fundamental is not doubled in fundamental state chords.
 * @param home the instance of the problem
 * @param size the size of the chord
 * @param nVoices the number of voices
 * @param tonality the tonality of the piece
 * @param chordStas the array containing the state of each chord
 * @param chordDegs the array containing the degree of each chord
 * @param fullChordsVoicing the array containing all the chords in the form [bass, alto, tenor, soprano]
 * @param nOccurrencesFund the array containing the number of times the fundamental is present in each chord
 * @param costVar the variable that will contain the cost
 */
void compute_fundamental_state_doubling_cost(const Home& home, int size, int nVoices, Tonality *tonality,
                                             vector<int> chordStas, vector<int> chordDegs,
                                             IntVarArray fullChordsVoicing, IntVarArray nOccurrencesFund,
                                             const IntVar& costVar){
    for(int i = 0; i < size; ++i){// for each chord
        /// if the chord is in fundamental state
        if(chordStas[i] == FUNDAMENTAL_STATE){
            IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices)); // current chord
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
void chord_note_occurrence_first_inversion(const Home& home, Tonality *tonality, int degree,
                                           const IntVarArgs& currentChord){
    /// exceptions
    /// if the third is a tonal note, then double it
    if(tonality->get_tonal_notes().find(tonality->get_degree_note(degree + THIRD_DEGREE % 7)) !=
            tonality->get_tonal_notes().end()){ /// double the third and other notes should be present at least once
        count(home, currentChord, tonality->get_scale_degree((degree + THIRD_DEGREE) % 7), IRT_EQ,2);
    }
    else{ /// default case: double the fundamental or the fifth of the chord
        count(home, currentChord, tonality->get_scale_degree((degree + THIRD_DEGREE) % 7), IRT_EQ,1);
    }
    /// happens either way
    count(home, currentChord, tonality->get_scale_degree(degree + FIRST_DEGREE), IRT_GQ, 1);
    count(home, currentChord, tonality->get_scale_degree((degree + FIFTH_DEGREE) % 7), IRT_GQ, 1);
}