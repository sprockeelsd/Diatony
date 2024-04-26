#ifndef MYPROJECT_PREFERENCES_HPP
#define MYPROJECT_PREFERENCES_HPP

#include "Tonality.hpp"

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
                                    IntVarArray nOfDifferentNotes, const IntVar &costVar);

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
void compute_n_of_notes_in_chord_cost(const Home &home, int nVoices, int size, IntVarArray fullChordsVoicing,
                                      IntVarArray nOfDifferentNotes, const IntVar &costVar);

/**
 * This function counts the number of times when a common note in the soprano voice when moving from a chord in first
 * inversion to another chord.
 * @param home the instance of the problem
 * @param nChords the number of chords in the progression
 * @param nVoices the number of voices in the piece
 * @param chordStates the state of the chord (fundamental, first inversion, second inversion)
 * @param FullChordsVoicing the array containing all the notes of the chords in the progression
 * @param commonNotesInSoprano an array containing 1 if there is a common note in the soprano voice between this chord and
 * the next, and if the first chord is in first inversion
 * @param nOfCommonNotesInSoprano the number of times when there is a common note in the soprano voice
 */
void compute_cost_for_common_note_in_soprano(const Home &home, IntVarArray commonNotesInSameVoice,
                                             const IntVar& nCommonNotesInSoprano);

/**
 * This function counts the number of incomplete chords
 * @param home the instance of the problem
 * @param size the number of chords in the chord progression
 * @param nVoices the number of voices in the chords
 * @param fullChordsVoicing all the notes of all the chords
 * @param nDiffNotesInChord An IntVarArray counting the number of different notes (regardless of their octave) in
 * each chord
 * @param nOfIncompleteChords an IntVar counting the number of incomplete chords in the chord progression
 */
void compute_cost_for_incomplete_chords(const Home &home, int nVoices, int size, const IntArgs &nNotesInChords,
                                        IntVarArray fullChordsVoicing, IntVarArray nDiffNotesInChord,
                                        const IntVar &nOfIncompleteChords);

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
                                                     const IntVar &nOfCommonNotesInSameVoice);

/**
 * This function sets the cost for the melodic intervals in all voices.
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
                                   const IntVar &costOfMelodicIntervals, IntVarArray costAllMelodicIntervals);

#endif //MYPROJECT_PREFERENCES_HPP