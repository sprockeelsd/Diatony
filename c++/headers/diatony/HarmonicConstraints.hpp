// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A file that contains all the harmonic constraints for the diatonic harmonic problem
//
#ifndef HARMONICCONSTRAINTS_HPP
#define HARMONICCONSTRAINTS_HPP

#include "../aux/Tonality.hpp"
#include "../aux/Utilities.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 * This file contains all the harmonic constraints, that is constraints within a given chord.                          *
 * It currently contains the following constraints:                                                                    *
 *      - set_to_chord: sets the domain of the current chord based on the tonality and the scale degree                *
 *      - set_bass: sets the bass of the chord according to the state of the chord                                     *
 *      - chord_note_occurrence_fundamental_state: sets the number of times each note in the chord are present for     *
 *          fundamental state chords                                                                                   *
 *      - chord_note_occurrence_first_inversion: sets the number of times each note in the chord are present for       *
 *          first inversion chords                                                                                     *
 *      - chord_note_occurrence_second_inversion: sets the number of times each note in the chord are present for      *
 *          second inversion chords                                                                                    *
 *      - chord_note_occurrence_third_inversion: sets the number of times each note in the chord are present for       *
 *          third inversion chords                                                                                     *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Set the notes of the fullChordsVoicing array to the notes of the given chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param quality the quality of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void set_to_chord(const Home& home, Tonality* tonality, int degree, int quality, const IntVarArgs& currentChord);

/**
 * Set the bass of the chord to be the given note
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param quality the quality of the chord
 * @param state the state of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void set_bass(const Home &home, Tonality *tonality, int degree, int quality, int state, IntVarArgs currentChord);

/**
 * Sets the number of times each note of the notes of the chord is present in the chord
 * @param home the instance of the problem
 * @param degree the degree of the chord
 * @param quality the quality of the chord
 * @param nVoices the number of voices
 * @param pos the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param nDifferentValuesInDiminishedChord the number of different values in the diminished chord
 * @param nOfNotesInChord the number of different notes in the chord based on its quality
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chord_note_occurrence_fundamental_state(Home home, int nVoices, int pos, vector<int> degree, vector<int> quality,
                                             Tonality *tonality, const IntVarArgs &currentChord,
                                             const IntVar &nDifferentValuesInDiminishedChord,
                                             const IntVar &nOfNotesInChord);

/**
 * Sets the number of times each note of the chord is present in the chord
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
                                           const IntVarArgs &currentChord, IntVarArray &bassMelodicIntervals,
                                           IntVarArray &sopranoMelodicIntervals);

/**
 * Sets the number of times each note of the chord is present in the chord
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
                                            vector<int> degrees, vector<int> qualities, const IntVarArgs &currentChord);

/**
 * Sets the number of times each note of the chord is present in the chord
 * @param home the instance of the problem
 * @param size the size of the chord progression
 * @param nVoices the number of voices
 * @param currentPos the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param degrees the degree of the chord
 * @param qualities the qualities of the chords
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chord_note_occurrence_third_inversion(const Home& home, int size, int nVoices, int currentPos, Tonality *tonality,
                                           vector<int> degrees, vector<int> qualities, const IntVarArgs &currentChord);
#endif