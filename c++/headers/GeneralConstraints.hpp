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
 *     - link_melodic_arrays: links the melodic intervals arrays to the fullChordsVoicing array for each voice         *
 *     - link_squared_melodic_arrays: links the absolute melodic intervals arrays to the corresponding melodic arrays *
 *     - link_harmonic_arrays: links the harmonic intervals arrays to the fullChordsVoicing array for each voice       *
 *     - restrain_voices_domains: sets the domains of the different voices to their range and gives them their order   *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the domains of the different voices to their range
 * @param home the instance of the problem
 * @param nVoices the number of voices
 * @param n the number of chords
 * @param lowerBounds the lower bounds of the voices
 * @param upperBounds the upper bounds of the voices
 * @param fullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, ...]
 */
void restrain_voices_domains(const Home &home, int nVoices, int n, vector<int> lowerBounds, vector<int> upperBounds,
                        IntVarArray fullChordsVoicing);

/**
 * Link the melodic intervals arrays to the fullChordsVoicing array
 * @param home The instance of the problem
 * @param nVoices the number of voices
 * @param size the number of chords
 * @param fullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, ...]
 * @param bassMelodicIntervals the melodic intervals of the bass
 * @param tenorMelodicIntervals the melodic intervals of the tenor
 * @param altoMelodicIntervals the melodic intervals of the alto
 * @param sopranoMelodicIntervals the melodic intervals of the soprano
 */
void link_melodic_arrays(const Home &home, int nVoices, int size,
                         IntVarArray fullChordsVoicing, IntVarArray bassMelodicIntervals,
                         IntVarArray altoMelodicIntervals, IntVarArray tenorMelodicIntervals,
                         IntVarArray sopranoMelodicIntervals);

/**
 * Link the absolute melodic intervals arrays to the corresponding melodic interval arrays
 * @param home the instance of the problem
 * @param bassMelodicIntervals the melodic intervals of the bass
 * @param tenorMelodicIntervals the melodic intervals of the tenor
 * @param altoMelodicIntervals the melodic intervals of the alto
 * @param sopranoMelodicIntervals the melodic intervals of the soprano
 * @param squaredBassMelodicIntervals the squared melodic intervals of the bass
 * @param squaredTenorMelodicIntervals the squared melodic intervals of the tenor
 * @param squaredAltoMelodicIntervals the squared melodic intervals of the alto
 * @param squaredSopranoMelodicIntervals the squared melodic intervals of the soprano
 */
void link_squared_melodic_arrays(const Home &home,  IntVarArray bassMelodicIntervals,        IntVarArray tenorMelodicIntervals,
                                                    IntVarArray altoMelodicIntervals,        IntVarArray sopranoMelodicIntervals,
                                                    IntVarArray squaredBassMelodicIntervals, IntVarArray squaredTenorMelodicIntervals,
                                                    IntVarArray squaredAltoMelodicIntervals, IntVarArray squaredSopranoMelodicIntervals);

/**
 * Link the harmonic intervals arrays to the fullChordsVoicing array for each voice
 * @param home the instance of the problem
 * @param nVoices the number of voices in the chords
 * @param size the number of chords
 * @param fullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, ...]
 * @param bassTenorHarmonicIntervals the harmonic intervals between bass and tenor
 * @param bassAltoHarmonicIntervals the harmonic intervals between bass and alto
 * @param bassSopranoHarmonicIntervals the harmonic intervals between bass and soprano
 * @param tenorAltoHarmonicIntervals the harmonic intervals between tenor and alto
 * @param tenorSopranoHarmonicIntervals the harmonic intervals between tenor and soprano
 * @param altoSopranoHarmonicIntervals the harmonic intervals between alto and soprano
 */
void link_harmonic_arrays(const Home &home, int nVoices, int size, IntVarArray fullChordsVoicing,
                          IntVarArray bassTenorHarmonicIntervals,    IntVarArray bassAltoHarmonicIntervals,
                          IntVarArray bassSopranoHarmonicIntervals,  IntVarArray tenorAltoHarmonicIntervals,
                          IntVarArray tenorSopranoHarmonicIntervals, IntVarArray altoSopranoHarmonicIntervals);
#endif