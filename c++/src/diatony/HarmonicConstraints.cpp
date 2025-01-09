// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A file that contains all the harmonic constraints for the diatonic harmonic problem
//
// ReSharper disable All
#include "../../headers/diatony/HarmonicConstraints.hpp"

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

//todo refaire les contraintes de doublure pour tous les états en mettant le maximum des trucs en commun à la fin \
(septième etc)

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
    IntSet set(get_all_notes_from_chord(tonality->get_degree_note(degree),
                                        chordQualitiesIntervals.at(quality)));
    //std::cout << "set: " << set << std::endl;
    dom(home, currentChord, set);
}

/**
 * Set the bass of the chord to be the given note
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param quality the quality of the chord
 * @param state the state of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void set_bass(const Home &home, Tonality *tonality, int degree, int quality, int state, IntVarArgs currentChord) {
    auto diff = get_interval_from_root(quality, state);
    dom(home, currentChord[0], IntSet(get_all_given_note((tonality->get_degree_note(degree) + diff) % PERFECT_OCTAVE)));
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                          Fundamental state chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the number of times each note of the notes of the chord are present in the chord
 * @todo check this still works for the different types of Aug. 6th chords
 * @param home the instance of the problem
 * @param degree the degree of the chord
 * @param quality the quality of the chord
 * @param nVoices the number of voices
 * @param tonality the tonality of the piece
 * @param nDifferentValuesInDiminishedChord the number of different values in the diminished chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chord_note_occurrence_fundamental_state(Home home, int nVoices, int pos, vector<int> degrees, vector<int> qualities,
                                             Tonality *tonality, const IntVarArgs &currentChord,
                                             const IntVar &nDifferentValuesInDiminishedChord,
                                             const IntVar &nOfNotesInChord) {

    auto root = tonality->get_degree_note(degrees[pos]);
    auto third = (root + get_interval_from_root(qualities[pos],THIRD)) % PERFECT_OCTAVE;
    auto fifth = (root + get_interval_from_root(qualities[pos],FIFTH)) % PERFECT_OCTAVE;

    /// if the chord is a diminished seventh degree
    if(degrees[pos] == SEVENTH_DEGREE && qualities[pos] == DIMINISHED_CHORD){
        /// If there are 4 different notes, then the third must be doubled. Otherwise any note can be doubled as
        /// there are only 3 values
        IntVar nOfThirds(home,0,nVoices);
        count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ, nOfThirds);
        /// if there are 4 different notes, then the third must be doubled
        rel(home, expr(home, nDifferentValuesInDiminishedChord == nVoices), BOT_IMP,
            expr(home, nOfThirds == 2), true);
        /// each note is present at least once, doubling is determined by the costs
        count(home, currentChord, IntSet(get_all_given_note(root)), IRT_GQ,1);
        count(home, currentChord, IntSet(get_all_given_note(third)), IRT_GQ,1);
        count(home, currentChord, IntSet(get_all_given_note(fifth)), IRT_GQ,1);
    }
    else if (degrees[pos] == FLAT_TWO) {
        count(home, currentChord, IntSet(get_all_given_note(root)), IRT_EQ, 1);
        count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ, 2);
        count(home, currentChord, IntSet(get_all_given_note(fifth)), IRT_GQ, 1);
    }
    else if (degrees[pos] == AUGMENTED_SIXTH) {
        count(home, currentChord, IntSet(get_all_given_note(root)), IRT_EQ, 1);
        count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ, 2);
        count(home, currentChord, IntSet(get_all_given_note(fifth)), IRT_EQ, 1);
    }
    /// special rule for sixth degree because in the case of an interrupted cadence, the third of the chord is doubled instead of the fundamental
    else if(degrees[pos] == SIXTH_DEGREE && pos > 0 && degrees[pos-1] == FIFTH_DEGREE){
        /// double the third of the chord
        count(home, currentChord, IntSet(get_all_given_note(root)), IRT_EQ,1);
        count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ,2);
        count(home, currentChord, IntSet(get_all_given_note(fifth)), IRT_EQ, 1);
    }
    else if(degrees[pos] == FIFTH_DEGREE || (degrees[pos] >= FIVE_OF_TWO && degrees[pos] <= FIVE_OF_SEVEN)){
        /// If there is a perfect cadence, then one of the chords must be incomplete.
        count(home, currentChord, IntSet(get_all_given_note(root)), IRT_GQ,1);
        count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ,1);
        count(home, currentChord, IntSet(get_all_given_note(fifth)), IRT_LQ, 1);

        if(qualities[pos] >= DOMINANT_SEVENTH_CHORD){
            auto seventh = (root + get_interval_from_root(qualities[pos],SEVENTH)) % PERFECT_OCTAVE;
            /// the seventh must be present
            count(home, currentChord, IntSet(get_all_given_note(seventh)), IRT_EQ, 1);
            /// if the chord is incomplete, double the bass
            BoolVar isIncomplete(expr(home, nOfNotesInChord < 4));
            IntVar nOfBassNotes(home,0,4);
            count(home, currentChord, IntSet(get_all_given_note(root)), IRT_EQ,nOfBassNotes);
            rel(home, isIncomplete, BOT_EQV, expr(home, nOfBassNotes == 2), true);
        }
    }
    else if(degrees[pos] == FIRST_DEGREE){
        count(home, currentChord, IntSet(get_all_given_note(root)), IRT_GQ,1);
        count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ,1);
        count(home, currentChord, IntSet(get_all_given_note(fifth)), IRT_LQ, 1);
        /// if the chord is incomplete, then the bass must be tripled and the third should be there once.
        BoolVar isIncomplete(expr(home, nOfNotesInChord < 3));
        IntVar nOfBassNotes(home,0,4);
        count(home, currentChord, IntSet(get_all_given_note(root)), IRT_EQ,nOfBassNotes);
        rel(home, isIncomplete, BOT_EQV, expr(home, nOfBassNotes == 3), true);
    }
    else{
        std::cout << "Degree: " << degrees[pos]  << " Quality: " << qualities[pos] << std::endl;
        /// each note is present at least once, the bass is present at least once, the third exactly once and the fifth at most once
        count(home, currentChord, IntSet(get_all_given_note(root)), IRT_GQ,1);
        count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ,1);
        count(home, currentChord, IntSet(get_all_given_note(fifth)), IRT_LQ, 1);

        if(qualities[pos] >= DOMINANT_SEVENTH_CHORD && qualities[pos]){
            auto seventh = (root + get_interval_from_root(qualities[pos],SEVENTH)) % PERFECT_OCTAVE;
            /// the seventh must be present
            count(home, currentChord, IntSet(get_all_given_note(seventh)), IRT_EQ, 1);
        }
        else{
            /// the fifth must be present exactly once
            count(home, currentChord, IntSet(get_all_given_note(fifth)), IRT_EQ, 1);
        }
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
    auto root = tonality->get_degree_note(degrees[currentPos]);
    auto third = (root + get_interval_from_root(qualities[currentPos],THIRD)) % PERFECT_OCTAVE;
    auto fifth = (root + get_interval_from_root(qualities[currentPos],FIFTH)) % PERFECT_OCTAVE;

    /// if the third is a tonal note, then double it
    set<int> tonalNotes = tonality->get_tonal_notes();
    if(tonalNotes.find(third) != tonalNotes.end()) { /// double the third and other notes should be present at least once
        count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ, 2);
    }
    else if((degrees[currentPos] == SEVENTH_DEGREE && qualities[currentPos] == DIMINISHED_CHORD) ||
        degrees[currentPos] == FLAT_TWO) {
        /// double the third and other notes should be present at least once
        count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ, 2);
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

            /// BoolVar to see if the soprano falls for the first motion
            BoolVar sopranoFalls1 = expr(home, expr(home, sopranoMelodicIntervals[currentPos-1] < 0) &&
                expr(home, sopranoMelodicIntervals[currentPos-1] >= -2));
            /// BoolVar to see if the soprano falls for the second motion
            BoolVar sopranoFalls2 = expr(home, expr(home, sopranoMelodicIntervals[currentPos] < 0) &&
                expr(home, sopranoMelodicIntervals[currentPos] >= -2));

            /// BoolVar to see if the soprano falls overall
            BoolVar sopranoFallsOverall = expr(home, sopranoFalls1 && sopranoFalls2);

            /// BoolVar to see if the voices move step wise by contrary motion over the three chords
            BoolVar contraryMotion = expr(home, bassRisesOverall && sopranoFallsOverall);

            IntVar nOfBassNotes(home,0,nVoices);
            count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ,nOfBassNotes);
            rel(home, contraryMotion, BOT_EQV, expr(home, nOfBassNotes == 2), true);
            rel(home, expr(home, !contraryMotion), BOT_EQV, expr(home, nOfBassNotes == 1), true);
        }
        else{ /// the bass can't be doubled
            count(home, currentChord, IntSet(get_all_given_note(root)), IRT_EQ, 1);
        }
    }
    /// each note always has to be present at least once
    count(home, currentChord, IntSet(get_all_given_note(root)), IRT_GQ, 1);
    count(home, currentChord, IntSet(get_all_given_note(third)), IRT_GQ, 1);
    count(home, currentChord, IntSet(get_all_given_note(fifth)), IRT_GQ, 1);
    if(qualities[currentPos] >= DOMINANT_SEVENTH_CHORD){
        auto seventh = (root + get_interval_from_root(qualities[currentPos],SEVENTH)) % PERFECT_OCTAVE;
        count(home, currentChord, IntSet(get_all_given_note(seventh)), IRT_GQ, 1);
    }

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
    auto root = tonality->get_degree_note(degrees[currentPos]);
    auto third = (root + get_interval_from_root(qualities[currentPos],THIRD)) % PERFECT_OCTAVE;
    auto fifth = (root + get_interval_from_root(qualities[currentPos],FIFTH)) % PERFECT_OCTAVE;

    if(degrees[currentPos] == SEVENTH_DEGREE && qualities[currentPos] == DIMINISHED_CHORD) {
        count(home, currentChord, IntSet(get_all_given_note(root)), IRT_EQ, 1);
        count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ, 2);
        count(home, currentChord, IntSet(get_all_given_note(fifth)), IRT_EQ, 1);
    }
    else{ /// default case: if the fourth or the sixth of the chord are in the chord before, they can be doubled if it is in the chord before at the same height
        /// the bass can always be doubled -> @todo
        count(home, currentChord, IntSet(get_all_given_note(root)), IRT_EQ, 1);
        count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ, 1);
        count(home, currentChord, IntSet(get_all_given_note(fifth)), IRT_GQ, 1);
        if(qualities[currentPos] >= DOMINANT_SEVENTH_CHORD){
            auto seventh = (root + get_interval_from_root(qualities[currentPos],SEVENTH)) % PERFECT_OCTAVE;
            count(home, currentChord, IntSet(get_all_given_note(seventh)), IRT_EQ, 1);
        }
    }
}

/**
 *
 * @param home
 * @param size
 * @param nVoices
 * @param currentPos
 * @param tonality
 * @param degrees
 * @param qualities
 * @param currentChord
 */
void chord_note_occurrence_third_inversion(const Home& home, int size, int nVoices, int currentPos, Tonality *tonality,
                                           vector<int> degrees, vector<int> qualities, const IntVarArgs &currentChord) {
    auto root = tonality->get_degree_note(degrees[currentPos]);
    auto third = (root + get_interval_from_root(qualities[currentPos],THIRD)) % PERFECT_OCTAVE;
    auto fifth = (root + get_interval_from_root(qualities[currentPos],FIFTH)) % PERFECT_OCTAVE;

    count(home, currentChord, IntSet(get_all_given_note(root)), IRT_EQ, 1);
    count(home, currentChord, IntSet(get_all_given_note(third)), IRT_EQ, 1);
    count(home, currentChord, IntSet(get_all_given_note(fifth)), IRT_GQ, 1);
    if(qualities[currentPos] >= DOMINANT_SEVENTH_CHORD){
        auto seventh = (root + get_interval_from_root(qualities[currentPos],SEVENTH)) % PERFECT_OCTAVE;
        count(home, currentChord, IntSet(get_all_given_note(seventh)), IRT_EQ, 1);
    }
}