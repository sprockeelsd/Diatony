#include "../headers/Preferences.hpp"

/**
 * Computes the cost for diminished intervals, that is the number of diminished chords that don't respect the preference,
 * and an array containing the number of different notes in each diminished chord (other chords are set to 0).
 * Here, the preference is that they should be used in 3 voices instead of 4.
 * @param home the instance of the problem
 * @param nVoices the number of voices
 * @param size the number of chords
 * @param chordStates the state of the chord (inversion)
 * @param chordQualities the quality of the chord (diminished, minor, major, augmented,...)
 * @param fullChordsVoicing the array containing all the chords in the form
 *          [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param nOfDifferentNotes the array containing the number of different notes in each diminished chord.
 * @param costVar the variable that will contain the number of diminished chords that don't respect the preference
 */
void compute_diminished_chords_cost(const Home &home, int nVoices, int size, vector<int> chordStates,
                                    vector<int> chordQualities, IntVarArray fullChordsVoicing,
                                    IntVarArray nOfDifferentNotes, const IntVar &costVar) {
    for(int i = 0; i < size; ++i){
        if(chordQualities[i] == DIMINISHED_CHORD && chordStates[i] == FUNDAMENTAL_STATE){
            IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices));
            nvalues(home, currentChord, IRT_EQ, nOfDifferentNotes[i]);
        }
        else{ /// doesn't matter so is set to 0 to be ignored
            rel(home, nOfDifferentNotes[i], IRT_EQ, 0);
        }
    }
    /// costVar = number of diminished chords with 4 notes
    count(home, nOfDifferentNotes, 4, IRT_EQ, costVar);
}

/**
 * Computes the cost for the number of notes in a chord, that is the number of chords that have less than 4 different
 * values
 * @param home the instance of the problem
 * @param nVoices the number of voices
 * @param size the size of the chord
 * @param tonality the tonality of the piece
 * @param fullChordsVoicing the array containing all the chords in the form [bass, alto, tenor, soprano]
 * @param nOfDifferentNotes the array containing the number of different notes in each chord
 * @param costVar the variable that will contain the cost
 */
void compute_n_of_notes_in_chord_cost(const Home &home, int nVoices, int size, IntVarArray fullChordsVoicing,
                                      IntVarArray nOfDifferentNotes, const IntVar &costVar) {
    for(int i = 0; i < size; i++) {
        IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices));
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
 * @param chordDegrees the array containing the degree of each chord
 * @param chordStates the array containing the state of each chord
 * @param fullChordsVoicing the array containing all the chords in the form [bass, alto, tenor, soprano]
 * @param nOccurrencesFund the array containing the number of times the fundamental is present in each chord
 * @param costVar the variable that will contain the cost
 */
void compute_fundamental_state_doubling_cost(const Home &home, int nVoices, int size, Tonality *tonality,
                                             vector<int> chordDegrees, vector<int> chordStates,
                                             IntVarArray fullChordsVoicing, IntVarArray nOccurrencesFund,
                                             const IntVar &costVar) {
    for(int i = 0; i < size; ++i){
        if(chordStates[i] == FUNDAMENTAL_STATE){
            IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices));
            /// nOccurencesFund[i] = nb of times the fundamental is present in the chord
            count(home, currentChord, tonality->get_scale_degree(chordDegrees[i]), IRT_EQ,
                  nOccurrencesFund[i]);
        }
        else{ /// if not fundamental state, then we ignore it so it's 0
            rel(home, nOccurrencesFund[i], IRT_EQ, 0);
        }
    }
    /// costVar = nb of chords that only have their fundamental once (it is not doubled)
    count(home, nOccurrencesFund, 1, IRT_EQ, costVar);
}

/**
 * This function counts the number of incomplete chords
 * @param home the instance of the problem
 * @param nVoices the number of voices in the chords
 * @param size the number of chords in the chord progression
 * @param nNotesInChords the number of notes in each chord if it is complete
 * @param fullChordsVoicing all the notes of all the chords
 * @param nDiffNotesInChord An IntVarArray counting the number of different notes (regardless of their octave) in
 * each chord
 * @param nOfIncompleteChords an IntVar counting the number of incomplete chords in the chord progression
 */
void compute_cost_for_incomplete_chords(const Home &home, int nVoices, int size, const IntArgs &nNotesInChords,
                                        IntVarArray fullChordsVoicing, IntVarArray nDiffNotesInChord,
                                        const IntVar &nOfIncompleteChords) {
    for(int i = 0; i < size; i++) {
        IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices));
        /// note values regardless of their octave
        IntVarArgs currentChordNotes;
        for(int j = 0; j < nVoices; j++)
            currentChordNotes << expr(home, currentChord[j] % PERFECT_OCTAVE);
        nvalues(home, currentChordNotes, IRT_EQ,nDiffNotesInChord[i]);
    }
    /// count the number of incomplete chords (size - the number of chords that have the max amount of notes)
    count(home, nDiffNotesInChord, nNotesInChords, IRT_EQ, expr(home, size - nOfIncompleteChords));
}

/**
 * This function sets the cost for the number of times when there is a common note in the same voice between consecutive
 * This has to be MAXIMIZED!
 * @param home the instance of the problem
 * @param bassMelodicIntervals the array of absolute melodic intervals for the bass
 * @param tenorMelodicIntervals the array of absolute melodic intervals for the tenor
 * @param altoMelodicIntervals the array of absolute melodic intervals for the alto
 * @param sopranoMelodicIntervals the array of absolute melodic intervals for the soprano
 * @param nOfUnissons the number of intervals that are a unisson
 * @param commonNotesInSameVoice an array containing the number of times when there is a common note in the same voice
 * for each voice
 * @param nOfCommonNotesInSameVoice the total number of times when there is a common note in the same voice
 */
void compute_cost_for_common_notes_not_in_same_voice(const Home &home, const IntVarArray &bassMelodicIntervals,
                                                     const IntVarArray &tenorMelodicIntervals,
                                                     const IntVarArray &altoMelodicIntervals,
                                                     const IntVarArray &sopranoMelodicIntervals,
                                                     const IntVar &nOfUnissons, IntVarArray commonNotesInSameVoice,
                                                     const IntVar &nOfCommonNotesInSameVoice) {

    vector<IntVarArray> melodicIntervals = {bassMelodicIntervals, tenorMelodicIntervals,
                                            altoMelodicIntervals, sopranoMelodicIntervals};
    /// costsForEachVoice[voice] = nb of times where the interval is 0
    for(int voice = BASS; voice <= SOPRANO; voice++){
        count(home, melodicIntervals[voice], UNISSON, IRT_EQ, commonNotesInSameVoice[voice]);
    }
    /// the sum of costs for each voice = the number of times where there is a common note in the same voice
    rel(home, expr(home, nOfUnissons + nOfCommonNotesInSameVoice == 0));
}

/**
 * This function sets the cost for the melodic intervals in all voices. It is a weighted sum of the number of occurence
 * of each interval, and the weights are defined in the Utilities.hpp file.
 * @param home the instance of the problem
 * @param allMelodicIntervals the array of absolute melodic intervals for the bass
 * @param nOfSeconds the number of intervals that are a second
 * @param nOfThirds the number of intervals that are a third
 * @param nOfFourths the number of intervals that are a fourth
 * @param nOfFifths the number of intervals that are a fifth
 * @param nOfSixths the number of intervals that are a sixth
 * @param nOfSevenths the number of intervals that are a seventh
 * @param nOfOctaves the number of intervals that are an octave
 * @param costOfMelodicIntervals the cost of the melodic intervals (weighted sum)
 */
void
compute_cost_for_melodic_intervals(const Home &home, const IntVarArray &allMelodicIntervals, const IntVar &nOfUnissons,
                                   const IntVar &nOfSeconds, const IntVar &nOfThirds, const IntVar &nOfFourths,
                                   const IntVar &nOfFifths, const IntVar &nOfSixths, const IntVar &nOfSevenths,
                                   const IntVar &nOfOctaves, const IntVar &costOfMelodicIntervals,
                                   IntVarArray costAllMelodicIntervals) {

    ///                    -octave,     - major seventh,    - minor seventh,    - major sixth,  - minor sixth,
    IntArgs weights({   OCTAVE_COST, SEVENTH_COST,       SEVENTH_COST,       SIXTH_COST,     SIXTH_COST,
    ///                     -perfect fifth,     - tritone,      -perfect fourth,    - major third,  - minor third,
                            FIFTH_COST,         TRITONE_COST,   FOURTH_COST,        THIRD_COST,     THIRD_COST,
    ///                     - major second,     - minor second,     unison,
                            SECOND_COST,        SECOND_COST,        UNISON_COST,
    ///                     minor second,   major second,   minor third,    major third,    perfect fourth,
                            SECOND_COST,    SECOND_COST,    THIRD_COST,     THIRD_COST,     FOURTH_COST,
    ///                     tritone,        perfect fifth,  minor sixth,    major sixth,    minor seventh,
                            TRITONE_COST,   FIFTH_COST,     SIXTH_COST,     SIXTH_COST,     SEVENTH_COST,
    ///                     major seventh,  octave
                            SEVENTH_COST,   OCTAVE_COST});

    for(int i = 0; i < allMelodicIntervals.size(); i++){
        element(home, weights, expr(home, allMelodicIntervals[i] + PERFECT_OCTAVE), costAllMelodicIntervals[i]); // so indexes are [0,24] instead of [-12,12]
    }
    /// the sum of the costs = total cost
    linear(home, costAllMelodicIntervals, IRT_EQ, costOfMelodicIntervals);

    /// count the number of occurences of each interval
    count(home, allMelodicIntervals, UNISSON,
          IRT_EQ, nOfUnissons);
//    count(home, allMelodicIntervals, IntSet({-MAJOR_SECOND, -MINOR_SECOND, MINOR_SECOND, MAJOR_SECOND}),
//          IRT_EQ, nOfSeconds);
//    count(home, allMelodicIntervals, IntSet({-MAJOR_THIRD, -MINOR_THIRD, MINOR_THIRD, MAJOR_THIRD}),
//          IRT_EQ, nOfThirds);
//    count(home, allMelodicIntervals, IntSet({-PERFECT_FOURTH, PERFECT_FOURTH}),
//          IRT_EQ, nOfFourths);
//    count(home, allMelodicIntervals, IntSet({-PERFECT_FIFTH, -TRITONE, TRITONE, PERFECT_FIFTH}),
//          IRT_EQ, nOfFifths); /// tritones are counted as fifth to only be counted once
//    count(home, allMelodicIntervals, IntSet({-MAJOR_SIXTH, -MINOR_SIXTH, MINOR_SIXTH, MAJOR_SIXTH}),
//          IRT_EQ, nOfSixths);
//    count(home, allMelodicIntervals, IntSet({-MAJOR_SEVENTH, -MINOR_SEVENTH, MINOR_SEVENTH, MAJOR_SEVENTH}),
//          IRT_EQ, nOfSevenths);
//    count(home, allMelodicIntervals, IntSet({-PERFECT_OCTAVE, PERFECT_OCTAVE}),
//          IRT_EQ, nOfOctaves);
//
//    /// weighted sum of the number of occurences of each interval
//    linear(home,
//           {UNISON_COST, SECOND_COST, THIRD_COST, FOURTH_COST, FIFTH_COST, SIXTH_COST, SEVENTH_COST, OCTAVE_COST},
//           IntVarArgs() << nOfUnissons << nOfSeconds << nOfThirds << nOfFourths << nOfFifths << nOfSixths << nOfSevenths << nOfOctaves,
//           IRT_EQ,
//           costOfMelodicIntervals);
}