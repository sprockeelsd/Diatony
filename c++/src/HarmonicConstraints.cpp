#include "../headers/HarmonicConstraints.hpp"

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
 * Set the notes of the fullChordsVoicing array to the notes of the given chord
 * @todo changer
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param quality the quality of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void set_to_chord(const Home& home, Tonality* tonality, int degree, int quality, const IntVarArgs& currentChord){
    IntSet set(get_all_notes_from_chord(tonality->get_degree_note(degree), chordQualitiesIntervals[quality]));
    dom(home, currentChord, set);
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
 * Sets the number of times each note of the notes of the chord are present in the chord
 * @param home the instance of the problem
 * @param degree the degree of the chord
 * @param quality the quality of the chord
 * @param nVoices the number of voices
 * @param tonality the tonality of the piece
 * @param nDifferentValuesInDiminishedChord the number of different values in the diminished chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chord_note_occurrence_fundamental_state(Home home, int nVoices, int degree, int quality, Tonality *tonality,
                                             const IntVarArgs &currentChord,
                                             const IntVar &nDifferentValuesInDiminishedChord,
                                             const IntVar& nOfNotesInChord) {
    /// if the chord is a diminished seventh degree, the third must be doubled
    if(degree == SEVENTH_DEGREE && quality == DIMINISHED_CHORD){
        /// If there are 4 different notes, then the third must be doubled. Otherwise any note can be doubled as
        /// there are only 3 values
        IntVar nOfThirds(home,0,nVoices);
        count(home, currentChord, tonality->get_scale_degree((degree + THIRD_DEGREE) % 7), IRT_EQ,nOfThirds);
        rel(home, expr(home, nDifferentValuesInDiminishedChord == nVoices), BOT_EQV,
            expr(home, nOfThirds == 2), true);
        /// each note is present at least once, doubling is determined by the costs
        count(home, currentChord, tonality->get_scale_degree(degree), IRT_GQ,1);
        count(home, currentChord, tonality->get_scale_degree((degree + THIRD_DEGREE) % 7), IRT_GQ,1);
        count(home, currentChord, tonality->get_scale_degree((degree + FIFTH_DEGREE) % 7), IRT_GQ,1);
    }
    if(degree == FIFTH_DEGREE){
        /// If there is a perfect cadence, then one of the chords must be incomplete.
        count(home, currentChord, tonality->get_scale_degree(degree), IRT_GQ,1);
        count(home, currentChord, tonality->get_scale_degree((degree + THIRD_DEGREE) % 7), IRT_EQ,1);
        count(home, currentChord, tonality->get_scale_degree((degree + FIFTH_DEGREE) % 7), IRT_LQ, 1);

        if(quality >= DOMINANT_SEVENTH_CHORD){
            count(home, currentChord, tonality->get_scale_degree((degree + SEVENTH_DEGREE) % 7), IRT_EQ, 1);
            /// if the chord is incomplete, double the bass
            BoolVar isIncomplete(home, 0,1);
            rel(home, expr(home, nOfNotesInChord < 4), BOT_EQV, isIncomplete, true);
            IntVar nOfBassNotes(home,0,4);
            count(home, currentChord, tonality->get_scale_degree(degree), IRT_EQ,nOfBassNotes);
            rel(home, isIncomplete, BOT_EQV, expr(home, nOfBassNotes == 2), true);
        }
    }
    else if(degree == FIRST_DEGREE){
        count(home, currentChord, tonality->get_scale_degree(degree), IRT_GQ,1);
        count(home, currentChord, tonality->get_scale_degree((degree + THIRD_DEGREE) % 7), IRT_EQ,1);
        count(home, currentChord, tonality->get_scale_degree((degree + FIFTH_DEGREE) % 7), IRT_LQ, 1);
        /// if the chord is incomplete, then the bass must be tripled and the third should be there once.
        BoolVar isIncomplete(expr(home, nOfNotesInChord < 3));
        rel(home, expr(home, nOfNotesInChord < 3), BOT_EQV, isIncomplete, true);
        IntVar nOfBassNotes(home,0,4);
        count(home, currentChord, tonality->get_scale_degree(degree), IRT_EQ,nOfBassNotes);
        rel(home, isIncomplete, BOT_EQV, expr(home, nOfBassNotes == 3), true);
    }
    else{
        /// each note is present at least once, doubling is determined by the costs
        count(home, currentChord, tonality->get_scale_degree(degree), IRT_GQ,1);
        count(home, currentChord, tonality->get_scale_degree((degree + THIRD_DEGREE) % 7), IRT_GQ,1);
        count(home, currentChord, tonality->get_scale_degree((degree + FIFTH_DEGREE) % 7), IRT_GQ, 1);
    }
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                            First inversion chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the number of time each note of the chord are present in the chord
 * @param home the instance of the problem
 * @param size the size of the chord progression
 * @param nVoices the number of voices
 * @param currentPos the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param degrees the degree of the chord
 * @param qualities the quality of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 * @param bassMelodicIntervals the array containing the melodic intervals of the bass
 * @param sopranoMelodicIntervals the array containing the melodic intervals of the soprano
 */
void chord_note_occurrence_first_inversion(Home home, int size, int nVoices, int currentPos, Tonality *tonality,
                                           vector<int> degrees, vector<int> qualities,
                                           const IntVarArgs &currentChord, IntVarArray bassMelodicIntervals,
                                           IntVarArray sopranoMelodicIntervals){
    /// if the third is a tonal note, then double it
    set<int> tonalNotes = tonality->get_tonal_notes();
    if(tonalNotes.find(tonality->get_degree_note((degrees[currentPos] + THIRD_DEGREE) % 7)) !=
       tonalNotes.end()) { /// double the third and other notes should be present at least once
        count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + THIRD_DEGREE) % 7), IRT_EQ, 2);
    }
    else if(degrees[currentPos] == SEVENTH_DEGREE && qualities[currentPos] == DIMINISHED_CHORD) {
        /// double the third and other notes should be present at least once
        count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + THIRD_DEGREE) % 7), IRT_EQ, 2);
    }
    else{ /// default case: double the fundamental or the fifth of the chord unless the top and bottom voices move down and up respectively
        if(currentPos < size-1 && currentPos > 0){ /// this special case cannot happen on the first and last chord
            ///BoolVar to see if the bass rises for the first motion
            BoolVar bassRises1 = expr(home, expr(home,bassMelodicIntervals[currentPos-1] > 0) &&
                expr(home, bassMelodicIntervals[currentPos-1] <= 2));
            /// BoolVar to see if the bass rises for the second motion
            BoolVar bassRises2 = expr(home, expr(home, bassMelodicIntervals[currentPos] > 0) &&
                expr(home, bassMelodicIntervals[currentPos] <= 2));

            /// BoolVar to see if the bass rises overall
            BoolVar bassRisesOverall = expr(home, bassRises1 && bassRises2);

            /// BoolVar to see if the soprano rises for the first motion
            BoolVar sopranoFalls1 = expr(home, expr(home, sopranoMelodicIntervals[currentPos-1] < 0) &&
                expr(home, sopranoMelodicIntervals[currentPos-1] >= -2));
            /// BoolVar to see if the soprano rises for the second motion
            BoolVar sopranoFalls2 = expr(home, expr(home, sopranoMelodicIntervals[currentPos] < 0) &&
                expr(home, sopranoMelodicIntervals[currentPos] >= -2));

            /// BoolVar to see if the soprano rises overall
            BoolVar sopranoFallsOverall = expr(home, sopranoFalls1 && sopranoFalls2);

            /// BoolVar to see if the voices move step wise by contrary motion over the three chords
            BoolVar contraryMotion = expr(home, bassRisesOverall && sopranoFallsOverall);

            IntVar nOfBassNotes(home,0,nVoices);
            count(home, currentChord, tonality->get_scale_degree(degrees[currentPos] + THIRD_DEGREE), IRT_EQ,nOfBassNotes);
            rel(home, contraryMotion, BOT_EQV, expr(home, nOfBassNotes == 2), true);
            rel(home, expr(home, !contraryMotion), BOT_EQV, expr(home, nOfBassNotes == 1), true);
        }
        else{ /// the bass can't be doubled
            count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + THIRD_DEGREE) % 7), IRT_EQ, 1);
        }
    }
    /// each note always has to be present at least once
    count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + FIRST_DEGREE) % 7), IRT_GQ, 1);
    count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + THIRD_DEGREE) % 7), IRT_GQ, 1);
    count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + FIFTH_DEGREE) % 7), IRT_GQ, 1);
    if(qualities[currentPos] >= DOMINANT_SEVENTH_CHORD)
        count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + SEVENTH_DEGREE) % 7), IRT_GQ, 1);
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                           Second inversion chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the number of time each note of the chord are present in the chord
 * @param home the instance of the problem
 * @param size the size of the chord progression
 * @param nVoices the number of voices
 * @param currentPos the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param degrees the degree of the chord
 * @param qualities the qualities of the chords
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chord_note_occurrence_second_inversion(const Home& home, int size, int nVoices, int currentPos, Tonality *tonality,
                                            vector<int> degrees, vector<int> qualities, const IntVarArgs &currentChord){
    if(degrees[currentPos] == SEVENTH_DEGREE && qualities[currentPos] == DIMINISHED_CHORD) {
        count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + FIRST_DEGREE) % 7), IRT_EQ, 1);
        count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + THIRD_DEGREE) % 7), IRT_EQ, 2);
        count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + FIFTH_DEGREE) % 7), IRT_EQ, 1);
    }
    else{ /// default case: if the fourth or the sixth of the chord are in the chord before, they can be doubled if it is in the chord before at the same height
        /// the bass can always be doubled -> @todo
        count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + FIFTH_DEGREE) % 7), IRT_GQ, 1);
        /// @todo other voices
        count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + FIRST_DEGREE) % 7), IRT_EQ, 1);
        count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + THIRD_DEGREE) % 7), IRT_EQ, 1);
        if(qualities[currentPos] >= DOMINANT_SEVENTH_CHORD)
            count(home, currentChord, tonality->get_scale_degree((degrees[currentPos] + SEVENTH_DEGREE) % 7), IRT_EQ, 1);
    }
}