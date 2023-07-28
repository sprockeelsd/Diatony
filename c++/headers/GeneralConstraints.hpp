#ifndef GENERALCONSTRAINTS
#define GENERALCONSTRAINTS

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
 * This file contains all the general constraints. It currently contains:                                              *
 *     - link_melodic_arrays: links the melodic intervals arrays to the FullChordsVoicing array for each voice         *
 *     - link_absolute_melodic_arrays: links the absolute melodic intervals arrays to the corresponding melodic arrays *
 *     - link_harmonic_arrays: links the harmonic intervals arrays to the FullChordsVoicing array for each voice       *
 *     - restrain_voices_domains: sets the domains of the different voices to their range and gives them their order   *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Link the melodic intervals arrays to the FullChordsVoicing array
 * @param home The instance of the problem
 * @param n the number of chords
 * @param FullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param bassMelodicIntervals the melodic intervals of the bass
 * @param tenorMelodicIntervals the melodic intervals of the tenor
 * @param altoMelodicIntervals the melodic intervals of the alto
 * @param sopranoMelodicIntervals the melodic intervals of the soprano
 */
void link_melodic_arrays(const Home& home, int n, IntVarArray FullChordsVoicing, IntVarArray bassMelodicIntervals,
                         IntVarArray tenorMelodicIntervals, IntVarArray altoMelodicIntervals,
                         IntVarArray sopranoMelodicIntervals);

/**
 * Link the absolute melodic intervals arrays to the corresponding melodic interval arrays
 * @param home the instance of the problem
 * @param bassMelodicIntervals the melodic intervals of the bass
 * @param tenorMelodicIntervals the melodic intervals of the tenor
 * @param altoMelodicIntervals the melodic intervals of the alto
 * @param sopranoMelodicIntervals the melodic intervals of the soprano
 * @param absoluteBassMelodicIntervals the absolute melodic intervals of the bass
 * @param absoluteTenorMelodicIntervals the absolute melodic intervals of the tenor
 * @param absoluteAltoMelodicIntervals the absolute melodic intervals of the alto
 * @param absoluteSopranoMelodicIntervals the absolute melodic intervals of the soprano
 */
void link_absolute_melodic_arrays(const Home& home, IntVarArray bassMelodicIntervals, IntVarArray tenorMelodicIntervals,
                                  IntVarArray altoMelodicIntervals, IntVarArray sopranoMelodicIntervals,
                                  IntVarArray absoluteBassMelodicIntervals, IntVarArray absoluteTenorMelodicIntervals,
                                  IntVarArray absoluteAltoMelodicIntervals, IntVarArray absoluteSopranoMelodicIntervals);

/**
 * Link the harmonic intervals arrays to the FullChordsVoicing array for each voice
 * @param home the instance of the problem
 * @param n the number of chords
 * @param FullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param bassTenorHarmonicIntervals the harmonic intervals between bass and tenor
 * @param bassAltoHarmonicIntervals the harmonic intervals between bass and alto
 * @param bassSopranoHarmonicIntervals the harmonic intervals between bass and soprano
 * @param tenorAltoHarmonicIntervals the harmonic intervals between tenor and alto
 * @param tenorSopranoHarmonicIntervals the harmonic intervals between tenor and soprano
 * @param altoSopranoHarmonicIntervals the harmonic intervals between alto and soprano
 */
void link_harmonic_arrays(const Home& home, int n, IntVarArray FullChordsVoicing, IntVarArray bassTenorHarmonicIntervals,
                          IntVarArray bassAltoHarmonicIntervals, IntVarArray bassSopranoHarmonicIntervals,
                          IntVarArray tenorAltoHarmonicIntervals, IntVarArray tenorSopranoHarmonicIntervals,
                          IntVarArray altoSopranoHarmonicIntervals);

/**
 * Computes the cost for diminished intervals, that is the number of diminished chords that don't respect the preference
 * Here, the preference is that they should be used in 3 voices instead of 4
 * @param home the instance of the problem
 * @param size the number of chords
 * @param tonality the tonality of the piece
 * @param chordDegs the degrees of the chords
 * @param fullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param nOfDifferentNotes the array containing the number of different notes in each diminished chord. In other chords, its value is 0
 * @param costVar the variable that will contain the cost, that is the number of diminished chords that don't respect the preference
 */
void compute_diminished_chords_cost(Home home, int size, Tonality &tonality, vector<int> chordDegs, IntVarArray fullChordsVoicing, IntVarArray nOfDifferentNotes, IntVar costVar);

/**
 * Sets the domains of the different voices to their range
 *      bass: [40, 60] E2 -> C3
 *      tenor: [48, 69] C2 -> A3
 *      alto: [55, 75] G2 -> D3
 *      soprano: [60, 84] C3 -> A4
 * @param home the instance of the problem
 * @param n the number of chords
 * @param FullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 */
void restrain_voices_domains(const Home& home, int n, IntVarArray FullChordsVoicing);

#endif