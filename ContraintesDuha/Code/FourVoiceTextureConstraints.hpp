/**
 * @file FourVoiceTextureConstraints.h
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This class contains all the musical constraints. They are separated into 3 types : generic constraints, chord-related constraints and voice leading related constraints.
 * @version 1.1
 * @date 2023-02-01
 *
 */
#ifndef FOURVOICETEXTURECONSTRAINTS
#define FOURVOICETEXTURECONSTRAINTS

#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>

#include "Utilities.hpp"
#include "Tonality.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace Gecode;
using namespace std;

/**********************************************************************
 *                                                                    *
 *                         Generic constraints                        *
 *                                                                    *
 **********************************************************************/

/**
 * @brief Posts the constraint that the seventh of the scale can never occur twice in a chord
 *
 * @param home The space of the problem
 * @param chordNotes The variables representing a given chord
 * @param sevenths The set of notes that are the seventh of the key
 */
void dontDoubleTheSeventh(Home home, IntVarArgs chordNotes, IntSet sevenths);

/**
 * @brief Ensures that if there is a tritone in the chord it resolves properly.
 * That is, the seventh should resolve upwards in the next chord and the fourth should resolve downwards.
 *
 * @param home the space of the problem
 * @param chordNotes the variables for the notes of the current chord
 * @param nOfSeventh the number of seventh present in the chord (should be <=1)
 * @param chordPosition the position of the chord in the big array
 * @param chordQuality the quality of the given chord (M/m/7/...)
 */
void tritoneResolution(Home home, IntVarArray chords, Tonality& tonality, int chordPosition, vector<int> chordQuality, IntSet fourths, IntSet sevenths);

/**
 * @brief
 *
 * @todo Check with Karim for the consecutive fourths thing. Also ask if the fact that its an octave higher matters of not (a parallel fifth an octave higher e.g)
 *
 * @param home The space of the problem
 * @param interval the parallel interval we wish to forbid
 * @param currentPosition The current chord which corresponds to the index in the interval arrays
 * @param bassIntervals The variable array for the bass
 * @param tenorIntervals The variable array for the tenor
 * @param altoIntervals The variable array for the alto
 * @param sopranoIntervals The variable array for the soprano
 */
void forbidParallelIntervals(Home home, int interval, int currentPosition, IntVarArray bassIntervals, IntVarArray tenorIntervals, IntVarArray altoIntervals,
                             IntVarArray sopranoIntervals);

/**********************************************************************
 *                                                                    *
 *                      Chord-related constraints                     *
 *                                                                    *
 **********************************************************************/

/**
 * @brief Posts the constraint that the different voices of the chord have a value that is part of the chord
 *
 * @param home The space of the problem
 * @param chordNotes the variables representing the notes of the chord
 * @param chordRoot The root of the chord
 * @param chordQuality The quality of the chord (M/m/...)
 * @param chordBass The bass of the chord
 */
void setToChord(Home home, IntVarArgs chordNotes, int chordRoot, vector<int> chordQuality, int chordBass);

/**
 * @brief This function posts a variety of constraints on 3 note chords. These constraints include :
 * - The doubling of the bass (should be priority-based -> TODO)
 * - Diminished chords should be 3 voices only -> 2 voices have to be the same
 *
 * @param home The space of the problem
 * @param chordNotes the variables representing the notes of the chord
 * @param chordRoot The root of the chord
 * @param chordQuality The quality of the chord (M/m/...)
 * @param chordBass The bass of the chord
 * @param doublingCost The cost variable for the doubling
 */
void fundamentalStateThreeNoteChord(Home home, IntVarArgs chordNotes, int chordRoot, vector<int> chordQuality, int chordBass);

/**********************************************************************
 *                                                                    *
 *                  Voice leading related constraints                 *
 *                                                                    *
 **********************************************************************/

/**
 * @brief Post the constraints for moving from a chord in fundamental state to another. For now, it only posts a constraint if the interval is a second.
 * The constraint posted is the following : If the interval between the roots of the 2 chords is a second, the other voices have to move in contrary movement to the bass.
 * @param home The space of the problem
 * @param currentPosition The current chord which corresponds to the index in the interval arrays
 * @param bassIntervals The variable array for the bass
 * @param tenorIntervals The variable array for the tenor
 * @param altoIntervals The variable array for the alto
 * @param sopranoIntervals The variable array for the soprano
 * @param chordBass The array of bass given as input
 * @param chordRoots The array of roots given as input
 */
void fundamentalStateChordToFundamentalStateChord(Home home, int currentPosition, IntVarArray bassIntervals, IntVarArray tenorIntervals, IntVarArray altoIntervals,
                                                  IntVarArray sopranoIntervals, vector<int> chordBass, vector<int> chordRoots);

#endif