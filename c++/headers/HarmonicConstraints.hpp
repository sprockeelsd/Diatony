#ifndef HARMONICCONSTRAINTS_HPP
#define HARMONICCONSTRAINTS_HPP

#include "Tonality.hpp"
#include "Utilities.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace Gecode;
using namespace std;

/***********************************************************************************************************************
 *                                                                                                                     *
 * This file contains all the harmonic constraints, that is constraints within a given chord.                          *
 * It currently contains the following constraints:                                                                    *
 *      - setToChord: sets the domain of the current chord based on the tonality and the scale degree                  *
 *      - setBass: sets the bass of the chord according to the state of the chord                                      *
 *      - chordNoteOccurrenceFundamentalState: sets the number of times each note in the chord are present for fundam- *
 *          ental state chords                                                                                         *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Set the notes of the FullChordsVoicing array to the notes of the given chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void setToChord(const Home& home, Tonality* tonality, int degree, const IntVarArgs& currentChord);

/**
 * Set the bass of the chord to be the given note
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param state the state of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void setBass(const Home& home, Tonality *tonality, int degree, int state, IntVarArgs currentChord);

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                          Fundamental state chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * @todo change this for complete and incomplete chords later (third must be <=1 depending on the chord before and after if they are 5->1 and complete/incomplete)
 * @todo maybe make it a preference later
 * Sets the number of times each note of the notes of the chord are present in the chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param previous_chord_degree the degree of the following chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chordNoteOccurrenceFundamentalState(const Home& home, Tonality *tonality, int degree, int previous_chord_degree,
                                         const IntVarArgs& currentChord);

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                            First inversion chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the number of time each note of the chord are present in the chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chordNoteOccurrenceFirstInversion(const Home& home, Tonality *tonality, int degree, const IntVarArgs& currentChord);

#endif