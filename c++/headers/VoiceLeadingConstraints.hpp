#ifndef VOICELEADINGCONSTRAINTS_HPP
#define VOICELEADINGCONSTRAINTS_HPP

#include "Tonality.hpp"
#include "Utilities.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <set>

using namespace Gecode;
using namespace std;

/***********************************************************************************************************************
 *                                                                                                                     *
 * This file contains all the voice leading constraints, that is constraints on how voices move through successive     *
 * chords.                                                                                                             *
 * It currently contains the following constraints:                                                                    *
 *      - forbid_parallel_intervals: forbids a given parallel interval between two voices                              *
 *      - fundamentalStateChordToFundamentalStateChord: sets the rules for the melodic movements between chords in     *
 *        fundamental state                                                                                            *
 *      - fifthDegreeFSToSixthDegreeFS: sets the constraint for a fifth degree followed by a sixth degree in funda-    *
 *        mental state                                                                                               *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Forbids a given parallel interval between two voices
 * @todo make it with argument variables + make it cleaner
 * @param home the instance of the problem
 * @param forbiddenParallelInterval the interval that must not be parallel
 * @param currentPosition the current position in the chord progression
 * @param voice1ID the ID of the first voice
 * @param voice2ID the ID of the second voice
 * @param voicesHarmonicIntervals the array containing the harmonic intervals between adjacent voices
 * @param FullChordsVoicing the array containing all the notes of the chords in the progression
 */
void forbid_parallel_intervals(Home home, int forbiddenParallelInterval, int currentPosition, int voice1ID, int voice2ID,
                               IntVarArray voicesHarmonicIntervals, IntVarArray FullChordsVoicing);

/**
 * Sets the general rules for the melodic movements between chords
 * Ensures that common notes between chords are kept in the same voice
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param chordDegrees the array containing the degrees of the chords in the progression
 * @param tonality the tonality of the piece
 * @param bassMelodicInterval The melodic interval of the bass between the current position and the next
 * @param tenorMelodicInterval the melodic interval of the tenor between the current position and the next
 * @param altoMelodicInterval the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicInterval the melodic interval of the soprano between the current position and the next
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 */
void generalVoiceLeadingRules(const Home& home, int currentPosition, vector<int> chordDegrees,
                                                  Tonality *tonality,
                                                  const IntVarArray& bassMelodicInterval, const IntVarArray& tenorMelodicInterval,
                                                  const IntVarArray& altoMelodicInterval, const IntVarArray& sopranoMelodicInterval,
                                                  IntVarArray fullChordsVoicing);

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                          Fundamental state chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the rules for the melodic movements between chords in fundamental state
 * For chords that are 1 degree apart, the other voices must move in contrary motion to the bass
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param chordDegrees the array containing the degrees of the chords in the progression
 * @param tonality the tonality of the piece
 * @param bassMelodicInterval The melodic interval of the bass between the current position and the next
 * @param tenorMelodicInterval the melodic interval of the tenor between the current position and the next
 * @param altoMelodicInterval the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicInterval the melodic interval of the soprano between the current position and the next
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 */
void fundamentalStateChordToFundamentalStateChord(const Home& home, int currentPosition, vector<int> chordDegrees,
                                                  Tonality *tonality,
                                                  const IntVarArray& bassMelodicInterval, const IntVarArray& tenorMelodicInterval,
                                                  const IntVarArray& altoMelodicInterval, const IntVarArray& sopranoMelodicInterval,
                                                  IntVarArray fullChordsVoicing);

void tritoneResolution(Home home, int currentPosition, Tonality *tonality, IntVarArray bassMelodicInterval,
                       IntVarArray tenorMelodicInterval, IntVarArray altoMelodicInterval, IntVarArray sopranoMelodicInterval,
                       IntVarArray fullChordsVoicing);

/**
 * Sets the constraint for a fifth degree followed by a sixth degree in fundamental state
 *      the seventh of the scale must rise to the tonic and the other voices are going down (except for the bass)
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param tenorMelodicInterval the melodic intervals of the tenor
 * @param altoMelodicInterval the melodic intervals of the alto
 * @param sopranoMelodicInterval the melodic intervals of the soprano
 * @param fullChordsVoicing the array containing the notes of the chords in the progression
 */
void fifthDegreeFSToSixthDegreeFS(const Home& home, int currentPosition, Tonality *tonality,
                                  const IntVarArray& tenorMelodicInterval, const IntVarArray& altoMelodicInterval,
                                  const IntVarArray& sopranoMelodicInterval, IntVarArray fullChordsVoicing);

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                            First inversion chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

void fromFirstInversionChord(const Home& home, int currentPosition, vector<int> chordDegrees,
                                                  Tonality *tonality,
                                                  const IntVarArray& bassMelodicInterval, const IntVarArray& tenorMelodicInterval,
                                                  const IntVarArray& altoMelodicInterval, const IntVarArray& sopranoMelodicInterval,
                                                  IntVarArray fullChordsVoicing);

#endif