#ifndef MYPROJECT_PREFERENCES_HPP
#define MYPROJECT_PREFERENCES_HPP

#include "Tonality.hpp"

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
                                    const IntVar& costVar);

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
                                      IntVarArray nOfDifferentNotes, const IntVar& costVar);

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
                                             const IntVar &costVar);

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
void compute_cost_for_common_note_in_soprano(const Home &home, int nChords, int nVoices, vector<int> chordStates,
                                             IntVarArray FullChordsVoicing, IntVarArray commonNotesInSoprano,
                                             const IntVar &nOfCommonNotesInSoprano);

/**
 * This function sets the cost for the number of times when there is a common note in the same voice between consecutive
 * This has to be MAXIMIZED!
 * @param home the instance of the problem
 * @param absoluteBassMelodicIntervals the array of absolute melodic intervals for the bass
 * @param absoluteTenorMelodicIntervals the array of absolute melodic intervals for the tenor
 * @param absoluteAltoMelodicIntervals the array of absolute melodic intervals for the alto
 * @param absoluteSopranoMelodicIntervals the array of absolute melodic intervals for the soprano
 * @param commonNotesInSameVoice an array containing the number of times when there is a common note in the same voice for each voice
 * @param nOfCommonNotesInSameVoice the total number of times when there is a common note in the same voice
 */
void compute_cost_for_common_notes_not_in_same_voice(const Home &home, IntVarArray absoluteBassMelodicIntervals,
                                                     IntVarArray absoluteTenorMelodicIntervals,
                                                     IntVarArray absoluteAltoMelodicIntervals,
                                                     IntVarArray absoluteSopranoMelodicIntervals,
                                                     IntVarArray commonNotesInSameVoice,
                                                     IntVar nOfCommonNotesInSameVoice);
#endif //MYPROJECT_PREFERENCES_HPP
