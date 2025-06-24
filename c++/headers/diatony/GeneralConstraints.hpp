// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A file that contains all the general constraints for the diatonic harmonic problem
//
#ifndef GENERALCONSTRAINTS
#define GENERALCONSTRAINTS

#include "../aux/Utilities.hpp"
#include "FourVoiceTextureParameters.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 * This file contains all the general constraints. It currently contains:                                              *
 *     - restrain_voices_domains: sets the domains of the different voices to their range and gives them their order   *
 *     - link_melodic_arrays: links the melodic intervals arrays to the fullChordsVoicing array for each voice         *
 *     - link_harmonic_arrays: links the harmonic intervals arrays to the fullChordsVoicing array for each voice       *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the domains of the different voices to their range (defined in Utilities.hpp)
 * @param home the instance of the problem
 * @param nVoices the number of voices
 * @param n the number of chords
 * @param lowerBounds the lower bounds of the voices
 * @param upperBounds the upper bounds of the voices
 * @param fullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, ...]
 */
void restrain_voices_domains(const Home &home, int nVoices, int n, const vector<int> &lowerBounds,
    const vector<int> &upperBounds, IntVarArray &fullChordsVoicing);

/**
 * Link the melodic intervals arrays to the fullChordsVoicing array
 * @param home The instance of the problem
 * @param nVoices the number of voices
 * @param params the parameters of the progression
 * @param fullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, ...]
 * @param bassMelodicIntervals the melodic intervals of the bass
 * @param tenorMelodicIntervals the melodic intervals of the tenor
 * @param altoMelodicIntervals the melodic intervals of the alto
 * @param sopranoMelodicIntervals the melodic intervals of the soprano
 * @param allMelodicIntervals the array containing all the melodic intervals in the form
 */
void link_melodic_arrays(const Home &home, int nVoices, const FourVoiceTextureParameters* params,
                         IntVarArray &fullChordsVoicing, IntVarArray &bassMelodicIntervals,
                         IntVarArray &altoMelodicIntervals, IntVarArray &tenorMelodicIntervals,
                         IntVarArray &sopranoMelodicIntervals, IntVarArray &allMelodicIntervals);

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