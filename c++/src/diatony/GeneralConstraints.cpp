// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A file that contains all the general constraints for the diatonic harmonic problem
//
#include "../../headers/diatony/GeneralConstraints.hpp"

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
void restrain_voices_domains(const Home &home, int nVoices, int n,
                             vector<int> lowerBounds, vector<int> upperBounds,
                             IntVarArray &fullChordsVoicing) {
    /// Restrain the domains of the different voices to their range
    for(int i = BASS; i <= SOPRANO; i++){
        IntVarArgs currentVoice(fullChordsVoicing.slice(i, nVoices, nVoices * n));
        rel(home, currentVoice, IRT_GQ, lowerBounds[i]);
        rel(home, currentVoice, IRT_LQ, upperBounds[i]);
    }
    /// Restrains the order of the voices
    for(int i = 0; i < n; i++){
        IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * i, 1, nVoices));
        rel(home, currentChord, IRT_LQ);
    }
}

/**
 * Link the melodic intervals arrays to the fullChordsVoicing array
 * @param home The instance of the problem
 * @param nVoices the number of voices
 * @param size the number of chords
 * @param fullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1,...]
 * @param bassMelodicIntervals the melodic intervals of the bass
 * @param tenorMelodicIntervals the melodic intervals of the tenor
 * @param altoMelodicIntervals the melodic intervals of the alto
 * @param sopranoMelodicIntervals the melodic intervals of the soprano
 */
void link_melodic_arrays(const Home &home, int nVoices, int size,
                         IntVarArray &fullChordsVoicing, IntVarArray &bassMelodicIntervals,
                         IntVarArray &altoMelodicIntervals, IntVarArray &tenorMelodicIntervals,
                         IntVarArray &sopranoMelodicIntervals) {

    for (int i = 0; i < size - 1; ++i)
    {
        rel(home, bassMelodicIntervals[i]    == fullChordsVoicing[(i + 1) * nVoices + BASS]      - fullChordsVoicing[i * nVoices + BASS]);
        rel(home, tenorMelodicIntervals[i]   == fullChordsVoicing[((i + 1) * nVoices) + TENOR]   - fullChordsVoicing[(i * nVoices) + TENOR]);
        rel(home, altoMelodicIntervals[i]    == fullChordsVoicing[((i + 1) * nVoices) + ALTO]    - fullChordsVoicing[(i * nVoices) + ALTO]);
        rel(home, sopranoMelodicIntervals[i] == fullChordsVoicing[((i + 1) * nVoices) + SOPRANO] - fullChordsVoicing[(i * nVoices) + SOPRANO]);
    }
}

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
void link_harmonic_arrays(const Home &home, int nVoices, int size,   IntVarArray fullChordsVoicing,
                          IntVarArray bassTenorHarmonicIntervals,    IntVarArray bassAltoHarmonicIntervals,
                          IntVarArray bassSopranoHarmonicIntervals,  IntVarArray tenorAltoHarmonicIntervals,
                          IntVarArray tenorSopranoHarmonicIntervals, IntVarArray altoSopranoHarmonicIntervals) {

    for(int i = 0; i < size; ++i){
        rel(home, bassTenorHarmonicIntervals[i] == fullChordsVoicing[(nVoices * i) + TENOR] - fullChordsVoicing[nVoices * i + BASS]);
        rel(home, bassAltoHarmonicIntervals[i] == fullChordsVoicing[(nVoices * i) + ALTO] - fullChordsVoicing[nVoices * i + BASS]);
        rel(home, bassSopranoHarmonicIntervals[i] == fullChordsVoicing[(nVoices * i) + SOPRANO] - fullChordsVoicing[nVoices * i + BASS]);

        rel(home, tenorAltoHarmonicIntervals[i] == fullChordsVoicing[(nVoices * i) + ALTO] - fullChordsVoicing[(nVoices * i) + TENOR]);
        rel(home, tenorSopranoHarmonicIntervals[i] == fullChordsVoicing[(nVoices * i) + SOPRANO] - fullChordsVoicing[(nVoices * i) + TENOR]);

        rel(home, altoSopranoHarmonicIntervals[i] == fullChordsVoicing[(nVoices * i) + SOPRANO] - fullChordsVoicing[(nVoices * i) + ALTO]);
    }
}