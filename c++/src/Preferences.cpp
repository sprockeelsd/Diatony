#include "../headers/Preferences.hpp"

/**
 * Computes the cost for diminished intervals, that is the number of diminished chords that don't respect the preference
 * Here, the preference is that they should be used in 3 voices instead of 4.
 * If the chord is not diminished, the value is forced to 0 since it doesn't matter
 * @param home the instance of the problem
 * @param size the number of chords
 * @param nVoices the number of voices
 * @param tonality the tonality of the piece
 * @param chordDegs the degrees of the chords
 * @param chordStas the state of the chord (inversion)
 * @param fullChordsVoicing the array containing all the chords in the form
 *          [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param nOfDifferentNotes the array containing the number of different notes in each diminished chord.
 * @param costVar the variable that will contain the number of diminished chords that don't respect the preference
 */
void compute_diminished_chords_cost(const Home& home, int size, int nVoices, Tonality *tonality, vector<int> chordDegs,
                                    vector<int> chordStas, IntVarArray fullChordsVoicing, IntVarArray nOfDifferentNotes,
                                    const IntVar& costVar) {
    for(int i = 0; i < size; ++i){// for each chord
        /// if the chord is diminished and in fundamental state
        if(tonality->get_chord_qualities()[chordDegs[i]] == DIMINISHED_CHORD_INTERVALS && chordStas[i] == FUNDAMENTAL_STATE){
            IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices));
            /// nOfDifferentNotes[i] = nOfDiffVals in current chord
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
        /// nOfDifferentNotes[i] = nOfDiffVals in current chord
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
 * @param chordDegs the array containing the degree of each chord
 * @param chordStas the array containing the state of each chord
 * @param fullChordsVoicing the array containing all the chords in the form [bass, alto, tenor, soprano]
 * @param nOccurrencesFund the array containing the number of times the fundamental is present in each chord
 * @param costVar the variable that will contain the cost
 */
void compute_fundamental_state_doubling_cost(const Home &home, int size, int nVoices, Tonality *tonality,
                                             vector<int> chordDegs, vector<int> chordStas,
                                             IntVarArray fullChordsVoicing, IntVarArray nOccurrencesFund,
                                             const IntVar &costVar) {
    for(int i = 0; i < size; ++i){// for each chord
        /// if the chord is in fundamental state
        if(chordStas[i] == FUNDAMENTAL_STATE){
            IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices)); // current chord
            /// nOccurencesFund[i] = nb of times the fundamental is present in the chord
            count(home, currentChord, tonality->get_scale_degree(chordDegs[i]), IRT_EQ,
                  nOccurrencesFund[i]);
        }
        else{ /// if not fundamental state, then we ignore it so it's 0
            rel(home, nOccurrencesFund[i], IRT_EQ, 0); // don't care
        }
    }
    /// costVar = nb of chords that only have their fundamental once (it is not doubled)
    count(home, nOccurrencesFund, 1, IRT_EQ, costVar);
}

/**
 * This function counts the number of times when a common note in the soprano voice when moving from a chord in first
 * inversion to another chord.
 * @param home the instance of the problem
 * @param commonNotesInSameVoice the number of common notes in each voice
 * @param nOfCommonNotesInSoprano the number of times when there is a common note in the soprano voice
 */
void compute_cost_for_common_note_in_soprano(const Home &home, IntVarArray commonNotesInSameVoice,
                                             IntVar nCommonNotesInSoprano) {
    rel(home, commonNotesInSameVoice[SOPRANO], IRT_EQ, nCommonNotesInSoprano);

}

/**
 * This function counts the number of incomplete chords
 * @param home the instance of the problem
 * @param size the number of chords in the chord progression
 * @param nVoices the number of voices in the chords
 * @param nNotesInChords the number of notes in each chord if it is complete
 * @param fullChordsVoicing all the notes of all the chords
 * @param nDiffNotesInChord An IntVarArray counting the number of different notes (regardless of their octave) in
 * each chord
 * @param nOfIncompleteChords an IntVar counting the number of incomplete chords in the chord progression
 */
void compute_cost_for_incomplete_chords(const Home &home, int size, int nVoices, IntArgs nNotesInChords,
                                        IntVarArray fullChordsVoicing, IntVarArray nDiffNotesInChord,
                                        IntVar nOfIncompleteChords) {
    for(int i = 0; i < size; i++) {
        IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices));
        /// note values regardless of their octave
        IntVarArgs currentChordNotes;
        for(int j = 0; j < nVoices; j++)
            currentChordNotes << expr(home, currentChord[j] % PERFECT_OCTAVE);

        /// nDiffNotesInChord[i] == the number of different notes regardless of their octave in the current chord
        nvalues(home, currentChordNotes, IRT_EQ,nDiffNotesInChord[i]);
    }
    /// count the number of incomplete chords (size - the number of chords that have the max amount of notes)
    count(home, nDiffNotesInChord, nNotesInChords, IRT_EQ, expr(home, size - nOfIncompleteChords));
}

/**
 * This function sets the cost for the number of times when there is a common note in the same voice between consecutive
 * This has to be MAXIMIZED!
 * @param home the instance of the problem
 * @param absoluteBassMelodicIntervals the array of absolute melodic intervals for the bass
 * @param absoluteTenorMelodicIntervals the array of absolute melodic intervals for the tenor
 * @param absoluteAltoMelodicIntervals the array of absolute melodic intervals for the alto
 * @param absoluteSopranoMelodicIntervals the array of absolute melodic intervals for the soprano
 * @param commonNotesInSameVoice an array containing the number of times when there is a common note in the same voice
 * for each voice
 * @param nOfCommonNotesInSameVoice the total number of times when there is a common note in the same voice
 */
void compute_cost_for_common_notes_not_in_same_voice(const Home &home, IntVarArray absoluteBassMelodicIntervals,
                                                     IntVarArray absoluteTenorMelodicIntervals,
                                                     IntVarArray absoluteAltoMelodicIntervals,
                                                     IntVarArray absoluteSopranoMelodicIntervals,
                                                     IntVarArray commonNotesInSameVoice,
                                                     IntVar nOfCommonNotesInSameVoice){

    vector<IntVarArray> absoluteMelodicIntervals = {absoluteBassMelodicIntervals, absoluteTenorMelodicIntervals,
                                                    absoluteAltoMelodicIntervals, absoluteSopranoMelodicIntervals};
    /// costsForEachVoice[voice] = nb of times where the interval is 0
    for(int voice = BASS; voice <= SOPRANO; voice++){
        count(home, absoluteMelodicIntervals[voice], 0, IRT_EQ, commonNotesInSameVoice[voice]);
    }
    /// the sum of costs for each voice = the number of times where there is a common note in the same voice
    linear(home,{-1,-1,-1,-1}, commonNotesInSameVoice, IRT_EQ, nOfCommonNotesInSameVoice);
}
