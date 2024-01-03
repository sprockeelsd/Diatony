#include "../headers/Preferences.hpp"

/**
 * Computes the cost for diminished intervals, that is the number of diminished chords that don't respect the preference
 * Here, the preference is that they should be used in 3 voices instead of 4.
 * If the chord is not diminished, the value is forced to 0 since it doesn't matter
 * @param home the instance of the problem
 * @param size the number of chords
 * @param nVoices the number of voices
 * @param tonality the tonality of the piece
 * @param chordDegrees the degrees of the chords
 * @param chordStates the state of the chord (inversion)
 * @param fullChordsVoicing the array containing all the chords in the form
 *          [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param nOfDifferentNotes the array containing the number of different notes in each diminished chord.
 * @param costVar the variable that will contain the number of diminished chords that don't respect the preference
 */
void compute_diminished_chords_cost(const Home& home, int size, int nVoices, Tonality *tonality, vector<int> chordDegrees,
                                    vector<int> chordStates, IntVarArray fullChordsVoicing, IntVarArray nOfDifferentNotes,
                                    const IntVar& costVar) {
    for(int i = 0; i < size; ++i){// for each chord
        /// if the chord is diminished and in fundamental state
        if(tonality->get_chord_qualities()[chordDegrees[i]] == DIMINISHED_CHORD_INTERVALS && chordStates[i] == FUNDAMENTAL_STATE){
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
 * @param chordDegrees the array containing the degree of each chord
 * @param chordStates the array containing the state of each chord
 * @param fullChordsVoicing the array containing all the chords in the form [bass, alto, tenor, soprano]
 * @param nOccurrencesFund the array containing the number of times the fundamental is present in each chord
 * @param costVar the variable that will contain the cost
 */
void compute_fundamental_state_doubling_cost(const Home &home, int size, int nVoices, Tonality *tonality,
                                             vector<int> chordDegrees, vector<int> chordStates,
                                             IntVarArray fullChordsVoicing, IntVarArray nOccurrencesFund,
                                             const IntVar &costVar) {
    for(int i = 0; i < size; ++i){// for each chord
        /// if the chord is in fundamental state
        if(chordStates[i] == FUNDAMENTAL_STATE){
            IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices)); // current chord
            /// nOccurencesFund[i] = nb of times the fundamental is present in the chord
            count(home, currentChord, tonality->get_scale_degree(chordDegrees[i]), IRT_EQ,
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
void compute_cost_for_incomplete_chords(const Home &home, int size, int nVoices, const IntArgs& nNotesInChords,
                                        IntVarArray fullChordsVoicing, IntVarArray nDiffNotesInChord,
                                        const IntVar& nOfIncompleteChords) {
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
void compute_cost_for_common_notes_not_in_same_voice(const Home &home, const IntVarArray& absoluteBassMelodicIntervals,
                                                     const IntVarArray& absoluteTenorMelodicIntervals,
                                                     const IntVarArray& absoluteAltoMelodicIntervals,
                                                     const IntVarArray& absoluteSopranoMelodicIntervals,
                                                     IntVarArray commonNotesInSameVoice,
                                                     IntVarArray negativeCommonNotesInSameVoice,
                                                     const IntVar& nOfCommonNotesInSameVoice) {

    vector<IntVarArray> absoluteMelodicIntervals = {absoluteBassMelodicIntervals, absoluteTenorMelodicIntervals,
                                                    absoluteAltoMelodicIntervals, absoluteSopranoMelodicIntervals};
    /// costsForEachVoice[voice] = nb of times where the interval is 0
    for(int voice = BASS; voice <= SOPRANO; voice++){
        count(home, absoluteMelodicIntervals[voice], 0, IRT_EQ, commonNotesInSameVoice[voice]);
        rel(home, expr(home, negativeCommonNotesInSameVoice[voice] == expr(home, - commonNotesInSameVoice[voice])));
    }
    /// the sum of costs for each voice = the number of times where there is a common note in the same voice
    linear(home,{-1,-1,-1,-1}, commonNotesInSameVoice, IRT_EQ, nOfCommonNotesInSameVoice);
}

/**
 * This function sets the cost for the melodic intervals in all voices. It is a weighted sum of the number of occurence
 * of each interval, and the weights are defined in the Utilities.hpp file.
 * @param home the instance of the problem
 * @param absoluteBassMelodicIntervals the array of absolute melodic intervals for the bass
 * @param absoluteTenorMelodicIntervals the array of absolute melodic intervals for the tenor
 * @param absoluteAltoMelodicIntervals the array of absolute melodic intervals for the alto
 * @param absoluteSopranoMelodicIntervals the array of absolute melodic intervals for the soprano
 * @param nOfSeconds the number of intervals that are a second
 * @param nOfThirds the number of intervals that are a third
 * @param nOfFourths the number of intervals that are a fourth
 * @param nOfFifths the number of intervals that are a fifth
 * @param nOfSixths the number of intervals that are a sixth
 * @param nOfSevenths the number of intervals that are a seventh
 * @param nOfOctaves the number of intervals that are an octave
 * @param costOfMelodicIntervals the cost of the melodic intervals (weighted sum)
 */
void compute_cost_for_melodic_intervals(const Home& home, const IntVarArray& absoluteBassMelodicIntervals,
                                        const IntVarArray& absoluteTenorMelodicIntervals,
                                        const IntVarArray& absoluteAltoMelodicIntervals,
                                        const IntVarArray& absoluteSopranoMelodicIntervals, const IntVar& nOfSeconds,
                                        const IntVar& nOfThirds, const IntVar& nOfFourths, const IntVar& nOfFifths,
                                        const IntVar& nOfSixths, const IntVar& nOfSevenths, const IntVar& nOfOctaves,
                                        const IntVar& costOfMelodicIntervals) {
    IntVarArgs allMelodicIntervals = IntVarArgs() << absoluteBassMelodicIntervals << absoluteTenorMelodicIntervals
                                         << absoluteAltoMelodicIntervals << absoluteSopranoMelodicIntervals;

    /// count the number of occurences of each interval
    count(home, allMelodicIntervals, IntSet({MINOR_SECOND, MAJOR_SECOND}),IRT_EQ, nOfSeconds);
    count(home, allMelodicIntervals, IntSet({MINOR_THIRD, MAJOR_THIRD}),IRT_EQ, nOfThirds);
    count(home, allMelodicIntervals, PERFECT_FOURTH,IRT_EQ, nOfFourths);
    count(home, allMelodicIntervals, IntSet({TRITONE, PERFECT_FIFTH}),IRT_EQ, nOfFifths); // tritones are counted as fifth to only be counted once
    count(home, allMelodicIntervals, IntSet({MINOR_SIXTH, MAJOR_SIXTH}),IRT_EQ, nOfSixths);
    count(home, allMelodicIntervals, IntSet({MINOR_SEVENTH, MAJOR_SEVENTH}),IRT_EQ, nOfSevenths);
    count(home, allMelodicIntervals, PERFECT_OCTAVE,IRT_EQ, nOfOctaves);

    /// weighted sum of the number of occurences of each interval
    linear(home, {SECOND_COST, THIRD_COST, FOURTH_COST, FIFTH_COST, SIXTH_COST, SEVENTH_COST, OCTAVE_COST},
           IntVarArgs() << nOfSeconds << nOfThirds << nOfFourths << nOfFifths << nOfSixths << nOfSevenths << nOfOctaves,
           IRT_EQ, costOfMelodicIntervals);
}