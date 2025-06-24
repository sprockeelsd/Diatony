// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A file that contains all the preferences for the diatonic harmonic problem
//
#ifndef MYPROJECT_PREFERENCES_HPP
#define MYPROJECT_PREFERENCES_HPP

#include "../aux/Tonality.hpp"

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
void compute_diminished_chords_cost(const Home &home, int nVoices, int size, const vector<int> &chordStates,
    const vector<int> &chordQualities, IntVarArray &fullChordsVoicing, IntVarArray &nOfDifferentNotes);

/**
 * Computes the cost for the number of notes in a chord, that is the number of chords that have less than 4 different
 * values
 * @param home the instance of the problem
 * @param nVoices the number of voices
 * @param size the size of the chord
 * @param fullChordsVoicing the array containing all the chords in the form [bass, alto, tenor, soprano]
 * @param nOfDifferentNotes the array containing the number of different notes in each chord
 * @param costVar the variable that will contain the cost
 */
void compute_n_of_notes_in_chord_cost(const Home &home, int nVoices, int size, IntVarArray &fullChordsVoicing,
    IntVarArray &nOfDifferentNotes, const IntVar &costVar);

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
void compute_cost_for_incomplete_chords(const Home &home, int nVoices, int size, const IntArgs &nNotesInChords,
    IntVarArray fullChordsVoicing, IntVarArray nDiffNotesInChord, const IntVar &nOfIncompleteChords);

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
    IntVarArray &commonNotesInSameVoice, const IntVar &nOfCommonNotesInSameVoice);

/**
 * This function sets the cost for the melodic intervals in all voices.
 * @param home the instance of the problem
 * @param allMelodicIntervals the array of absolute melodic intervals for the bass
 * @param nOfUnisons
 * @param costOfMelodicIntervals the cost of the melodic intervals (weighted sum)
 * @param costAllMelodicIntervals
 */
void compute_cost_for_melodic_intervals(const Home &home, const IntVarArray &allMelodicIntervals,
    const IntVar &nOfUnisons, const IntVar &costOfMelodicIntervals, IntVarArray &costAllMelodicIntervals);

#endif //MYPROJECT_PREFERENCES_HPP