// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A file that contains all the preferences for the diatonic harmonic problem
//
#include "../../headers/diatony/Preferences.hpp"

/**
 * Computes the cost for diminished chords with the preference that they should be used in 3 voices instead of 4,
 * and an array containing the number of different notes in each diminished chord (other chords are set to 0).
 * @param home the instance of the problem
 * @param nVoices the number of voices
 * @param size the number of chords
 * @param chordStates the state of the chord (inversion)
 * @param chordQualities the quality of the chord (diminished, minor, major, augmented, ...)
 * @param fullChordsVoicing the array containing all the chords in the form
 *          [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param nOfDifferentNotes the array containing the number of different notes in each diminished chord.
 */
void compute_diminished_chords_cost(const Home &home, const int nVoices, const int size, const vector<int> &chordStates,
    const vector<int> &chordQualities, IntVarArray &fullChordsVoicing, IntVarArray &nOfDifferentNotes) {
    for(int i = 0; i < size; ++i){
        if(chordQualities[i] == DIMINISHED_CHORD && chordStates[i] == FUNDAMENTAL_STATE){
            IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices));
            nvalues(home, currentChord, IRT_EQ, nOfDifferentNotes[i]);
        }
        else{ /// doesn't matter so is set to 0 to be ignored
            rel(home, nOfDifferentNotes[i], IRT_EQ, 0);
        }
    }
}

/**
 * Computes the cost for the number of chords that have less than 4 different
 * values
 * @param home the instance of the problem
 * @param nVoices the number of voices
 * @param size the size of the chord
 * @param fullChordsVoicing the array containing all the chords in the form [bass, alto, tenor, soprano]
 * @param nOfDifferentNotes the array containing the number of different notes in each chord
 * @param costVar the variable that will contain the cost
 */
void compute_n_of_notes_in_chord_cost(const Home &home, const int nVoices, const int size, IntVarArray &fullChordsVoicing,
    IntVarArray &nOfDifferentNotes, const IntVar &costVar) {
    for(int i = 0; i < size; i++) {
        IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices));
        nvalues(home, currentChord, IRT_EQ,nOfDifferentNotes[i]);
    }
    /// costVar = nb of vars in nOfDifferentNotes that are smaller than 4
    count(home, nOfDifferentNotes, IntSet({1,2,3}), IRT_EQ, costVar);
}

/**
 * This function counts the number of incomplete chords
 * @param home the instance of the problem
 * @param size the number of chords in the chord progression
 * @param nNotesInChords
 * @param nVoices the number of voices in the chords
 * @param fullChordsVoicing all the notes of all the chords
 * @param nDiffNotesInChord An IntVarArray counting the number of different notes (regardless of their octave) in
 * each chord
 * @param nOfIncompleteChords an IntVar counting the number of incomplete chords in the chord progression
 */
void compute_cost_for_incomplete_chords(const Home &home, const int nVoices, const int size, const IntArgs &nNotesInChords,
    IntVarArray fullChordsVoicing, IntVarArray nDiffNotesInChord, const IntVar &nOfIncompleteChords) {
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
 * @param nOfUnisons the number of intervals that are unison
 * @param commonNotesInSameVoice an array containing the number of times when there is a common note in the same voice
 * for each voice
 * @param nOfCommonNotesInSameVoice the total number of times when there is a common note in the same voice
 */
void compute_cost_for_common_notes_not_in_same_voice(const Home &home, const IntVarArray &bassMelodicIntervals,
    const IntVarArray &tenorMelodicIntervals, const IntVarArray &altoMelodicIntervals,
    const IntVarArray &sopranoMelodicIntervals, const IntVar &nOfUnisons,
    IntVarArray &commonNotesInSameVoice, const IntVar &nOfCommonNotesInSameVoice) {

    const vector<IntVarArray> melodicIntervals =
        {bassMelodicIntervals, tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals};
    /// costsForEachVoice[voice] = nb of times where the interval is 0
    for(int voice = BASS; voice <= SOPRANO; voice++){
        count(home, melodicIntervals[voice], UNISON, IRT_EQ, commonNotesInSameVoice[voice]);
    }
    /// the sum of costs for each voice = the number of times where there is a common note in the same voice
    rel(home, expr(home, nOfUnisons + nOfCommonNotesInSameVoice == 0));
}

/**
 * This function sets the cost for the melodic intervals in all voices.
 * @param home the instance of the problem
 * @param allMelodicIntervals the array of absolute melodic intervals for the bass
 * @param nOfUnisons the number of intervals that are unison
 * @param costOfMelodicIntervals the cost of the melodic intervals (weighted sum)
 * @param costAllMelodicIntervals an array containing the cost of melodic intervals for each voice
 */
void compute_cost_for_melodic_intervals(const Home &home, const IntVarArray &allMelodicIntervals,
    const IntVar &nOfUnisons, const IntVar &costOfMelodicIntervals, IntVarArray &costAllMelodicIntervals){
    ///                         -octave,     -major seventh,     -minor seventh,     -major sixth,   -minor sixth,
    const IntArgs weights({   OCTAVE_COST, SEVENTH_COST,       SEVENTH_COST,       SIXTH_COST,     SIXTH_COST,
    ///                     -perfect fifth,     -tritone,      -perfect fourth,     -major third,   -minor third,
                            FIFTH_COST,         TRITONE_COST,   FOURTH_COST,        THIRD_COST,     THIRD_COST,
    ///                     -major second,      -minor second,      unison,
                            SECOND_COST,        SECOND_COST,        UNISON_COST,
    ///                     minor second,   major second,   minor third,    major third,    perfect fourth,
                            SECOND_COST,    SECOND_COST,    THIRD_COST,     THIRD_COST,     FOURTH_COST,
    ///                     tritone,        perfect fifth,  minor sixth,    major sixth,    minor seventh,
                            TRITONE_COST,   FIFTH_COST,     SIXTH_COST,     SIXTH_COST,     SEVENTH_COST,
    ///                     major seventh,  octave
                            SEVENTH_COST,   OCTAVE_COST});
    for(int i = 0; i < allMelodicIntervals.size(); i++){
        // Shift the melodic intervals so that indexes are [0,24] instead of [-12,12]. This allows using the intervals
        // as indexes in the weights array
        element(home, weights, expr(home, allMelodicIntervals[i] + PERFECT_OCTAVE), costAllMelodicIntervals[i]);
    }
    /// the sum of the costs = total cost
    linear(home, costAllMelodicIntervals, IRT_EQ, costOfMelodicIntervals);

    /// count the number of occurences of each interval, useful for the preference of common notes in same voice
    count(home, allMelodicIntervals, UNISON, IRT_EQ, nOfUnisons);
}