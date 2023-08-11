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
 *      - forbid_parallel_interval: forbids a given parallel interval between two voices                               *
 *      - fundamental_state_chord_to_fundamental_state_chord: sets the rules for the melodic movements between chords  *
 *          in fundamental state                                                                                       *
 *      - fifth_degree_fs_to_sixth_degree_fs: sets the constraint for a fifth degree followed by a sixth degree in     *
 *          funda mental state                                                                                         *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Forbids a list of parallel intervals between two voices. This calls the forbid_parallel_interval function
 * @param home the instance of the problem
 * @param size the size of the chord progression
 * @param nOfVoices the number of voices
 * @param intervals the list of intervals to forbid
 * @param bassTenorHarmonicIntervals the array containing the harmonic intervals between bass and tenor
 * @param bassAltoHarmonicIntervals the array containing the harmonic intervals between bass and alto
 * @param bassSopranoHarmonicIntervals the array containing the harmonic intervals between bass and soprano
 * @param tenorAltoHarmonicIntervals the array containing the harmonic intervals between tenor and alto
 * @param tenorSopranoHarmonicIntervals the array containing the harmonic intervals between tenor and soprano
 * @param altoSopranoHarmonicIntervals the array containing the harmonic intervals between alto and soprano
 * @param FullChordsVoicing the array containing all the notes of the chords in the progression
 */
void forbid_parallel_intervals(const Home& home, int size, int nOfVoices, const vector<int>& intervals,
                               const IntVarArray& bassTenorHarmonicIntervals,
                               const IntVarArray& bassAltoHarmonicIntervals,
                               const IntVarArray& bassSopranoHarmonicIntervals,
                               const IntVarArray& tenorAltoHarmonicIntervals,
                               const IntVarArray& tenorSopranoHarmonicIntervals,
                               const IntVarArray& altoSopranoHarmonicIntervals,
                               const IntVarArray& FullChordsVoicing);

/**
 * Forbids a given parallel interval between two voices
 * @todo make it with argument variables + make it cleaner
 * @param home the instance of the problem
 * @param nVoices the number of voices
 * @param forbiddenParallelInterval the interval that must not be parallel
 * @param currentPosition the current position in the chord progression
 * @param voice1ID the ID of the first voice
 * @param voice2ID the ID of the second voice
 * @param voicesHarmonicIntervals the array containing the harmonic intervals between adjacent voices
 * @param FullChordsVoicing the array containing all the notes of the chords in the progression
 */
void forbid_parallel_interval(Home home, int nVoices, int forbiddenParallelInterval, int currentPosition, int voice1ID,
                              int voice2ID, IntVarArray voicesHarmonicIntervals, IntVarArray FullChordsVoicing);

/**
 * Sets the general rules for the melodic movements between chords
 * Ensures that common notes between chords are kept in the same voice
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param chordDegrees the array containing the degrees of the chords in the progression
 * @param tonality the tonality of the piece
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 */
void general_voice_leading_rules(const Home &home, int currentPosition, vector<int> chordDegrees, Tonality *tonality,
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
 * @param nVoices the number of voices in the progression
 * @param tonality the tonality of the piece
 * @param bassMelodicInterval The melodic interval of the bass between the current position and the next
 * @param tenorMelodicInterval the melodic interval of the tenor between the current position and the next
 * @param altoMelodicInterval the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicInterval the melodic interval of the soprano between the current position and the next
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 */
void fundamental_state_chord_to_fundamental_state_chord(const Home& home, int currentPosition, vector<int> chordDegrees,
                                                        int nVoices, Tonality *tonality,
                                                        const IntVarArray& bassMelodicInterval,
                                                        const IntVarArray& tenorMelodicInterval,
                                                        const IntVarArray& altoMelodicInterval,
                                                        const IntVarArray& sopranoMelodicInterval,
                                                        const IntVarArray& fullChordsVoicing);


/**
 * Forces the tritone to resolve properly
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param nvoices the number of voices in the piece
 * @param tonality the tonality of the piece
 * @param bassMelodicInterval the melodic interval of the bass between the current position and the next
 * @param tenorMelodicInterval the melodic interval of the tenor between the current position and the next
 * @param altoMelodicInterval the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicInterval the melodic interval of the soprano between the current position and the next
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 */
void tritone_resolution(const Home& home, int currentPosition, int nVoices, Tonality *tonality,
                        const IntVarArray& bassMelodicInterval, const IntVarArray& tenorMelodicInterval,
                        const IntVarArray& altoMelodicInterval, const IntVarArray& sopranoMelodicInterval,
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
void fifth_degree_fs_to_sixth_degree_fs(const Home& home, int currentPosition, Tonality *tonality,
                                        const IntVarArray& tenorMelodicInterval, const IntVarArray& altoMelodicInterval,
                                        const IntVarArray& sopranoMelodicInterval, IntVarArray fullChordsVoicing);

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                            First inversion chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

void from_first_inversion_chord(const Home &home, int currentPosition, int nVoices, vector<int> chordDegrees,
                                Tonality *tonality, const IntVarArray &bassMelodicInterval,
                                const IntVarArray &tenorMelodicInterval, const IntVarArray &altoMelodicInterval,
                                const IntVarArray &sopranoMelodicInterval, const IntVarArray &fullChordsVoicing);

#endif