#include "../headers/GeneralConstraints.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 * This file contains all the general constraints. It currently contains:                                              *
 *     - link_melodic_arrays: links the melodic intervals arrays to the FullChordsVoicing array for each voice         *
 *     - link_squared_melodic_arrays: links the absolute melodic intervals arrays to the corresponding melodic arrays *
 *     - link_harmonic_arrays: links the harmonic intervals arrays to the FullChordsVoicing array for each voice       *
 *     - restrain_voices_domains: sets the domains of the different voices to their range and gives them their order   *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the domains of the different voices to their range
 *      bass: [40, 60] E2 -> C3
 *      tenor: [48, 69] C2 -> A3
 *      alto: [55, 75] G2 -> D3
 *      soprano: [60, 84] C3 -> A4
 * @param home the instance of the problem
 * @param n the number of chords
 * @param nVoices the number of voices
 * @param FullChordsVoicing the array containing all the chords in the form
 *          [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 */
void restrain_voices_domains(const Home &home, int n, int nVoices, IntVarArray FullChordsVoicing) {
    for (int i = 0; i < n; ++i)
    {
        IntVarArgs currentChord(FullChordsVoicing.slice(nVoices * i, 1, nVoices));
        rel(home, currentChord, IRT_LQ); // bass[i] <= tenor[i] <= alto[i] <= soprano[i]

        // E2 <= bass <= C3
        rel(home, currentChord[0], IRT_GQ, 40);
        rel(home, currentChord[0], IRT_LQ, 60);
        // C2 <= tenor <= A3
        rel(home, currentChord[1], IRT_GQ, 48);
        rel(home, currentChord[1], IRT_LQ, 69);
        // G2 <= alto <= D3
        rel(home, currentChord[2], IRT_GQ, 55);
        rel(home, currentChord[2], IRT_LQ, 75);
        // C3 <= soprano <= A4
        rel(home, currentChord[3], IRT_GQ, 60);
        rel(home, currentChord[3], IRT_LQ, 84);
    }
}

/**
 * Link the melodic intervals arrays to the FullChordsVoicing array
 * @param home The instance of the problem
 * @param size the number of chords
 * @param nVoices the number of voices
 * @param FullChordsVoicing the array containing all the chords in the form
 *          [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param bassMelodicIntervals the melodic intervals of the bass
 * @param tenorMelodicIntervals the melodic intervals of the tenor
 * @param altoMelodicIntervals the melodic intervals of the alto
 * @param sopranoMelodicIntervals the melodic intervals of the soprano
 */
void link_melodic_arrays(const Home &home, int size, int nVoices, IntVarArray FullChordsVoicing,
                         IntVarArray bassMelodicIntervals, IntVarArray altoMelodicIntervals,
                         IntVarArray tenorMelodicIntervals, IntVarArray sopranoMelodicIntervals) {
    for (int i = 0; i < size - 1; ++i)
    {
        rel(home, bassMelodicIntervals[i] == FullChordsVoicing[(i + 1) * nVoices + BASS] -
            FullChordsVoicing[i * nVoices + BASS]);
        rel(home,tenorMelodicIntervals[i] == FullChordsVoicing[((i + 1) * nVoices) + TENOR] -
            FullChordsVoicing[(i * nVoices) + TENOR]);
        rel(home, altoMelodicIntervals[i] == FullChordsVoicing[((i + 1) * nVoices) + ALTO] -
            FullChordsVoicing[(i * nVoices) + ALTO]);
        rel(home, sopranoMelodicIntervals[i] == FullChordsVoicing[((i + 1) * nVoices) + SOPRANO] -
            FullChordsVoicing[(i * nVoices) + SOPRANO]);
    }
}

/**
 * Link the absolute melodic intervals arrays to the corresponding melodic interval arrays
 * @param home the instance of the problem
 * @param bassMelodicIntervals the melodic intervals of the bass
 * @param tenorMelodicIntervals the melodic intervals of the tenor
 * @param altoMelodicIntervals the melodic intervals of the alto
 * @param sopranoMelodicIntervals the melodic intervals of the soprano
 * @param squaredBassMelodicIntervals the absolute melodic intervals of the bass
 * @param squaredTenorMelodicIntervals the absolute melodic intervals of the tenor
 * @param squaredAltoMelodicIntervals the absolute melodic intervals of the alto
 * @param squaredSopranoMelodicIntervals the absolute melodic intervals of the soprano
 */
void link_squared_melodic_arrays(const Home &home, IntVarArray bassMelodicIntervals, IntVarArray tenorMelodicIntervals,
                                 IntVarArray altoMelodicIntervals, IntVarArray sopranoMelodicIntervals,
                                 IntVarArray squaredBassMelodicIntervals, IntVarArray squaredTenorMelodicIntervals,
                                 IntVarArray squaredAltoMelodicIntervals,
                                 IntVarArray squaredSopranoMelodicIntervals) {
    for(int i = 0; i < bassMelodicIntervals.size(); ++i){
        sqr(home, bassMelodicIntervals[i], squaredBassMelodicIntervals[i]);
        sqr(home, tenorMelodicIntervals[i], squaredTenorMelodicIntervals[i]);
        sqr(home, altoMelodicIntervals[i], squaredAltoMelodicIntervals[i]);
        sqr(home, sopranoMelodicIntervals[i], squaredSopranoMelodicIntervals[i]);
    }
}

/**
 * Link the harmonic intervals arrays to the FullChordsVoicing array for each voice
 * @param home the instance of the problem
 * @param size the number of chords
 * @param nVoices the number of voices in the chords
 * @param FullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param bassTenorHarmonicIntervals the harmonic intervals between bass and tenor
 * @param bassAltoHarmonicIntervals the harmonic intervals between bass and alto
 * @param bassSopranoHarmonicIntervals the harmonic intervals between bass and soprano
 * @param tenorAltoHarmonicIntervals the harmonic intervals between tenor and alto
 * @param tenorSopranoHarmonicIntervals the harmonic intervals between tenor and soprano
 * @param altoSopranoHarmonicIntervals the harmonic intervals between alto and soprano
 */
void link_harmonic_arrays(const Home &home, int size, int nVoices, IntVarArray FullChordsVoicing,
                          IntVarArray bassTenorHarmonicIntervals, IntVarArray bassAltoHarmonicIntervals,
                          IntVarArray bassSopranoHarmonicIntervals, IntVarArray tenorAltoHarmonicIntervals,
                          IntVarArray tenorSopranoHarmonicIntervals, IntVarArray altoSopranoHarmonicIntervals) {
    for(int i = 0; i < size; ++i){
        rel(home, bassTenorHarmonicIntervals[i] == FullChordsVoicing[(nVoices * i) + TENOR] -
            FullChordsVoicing[nVoices * i + BASS]);
        rel(home, bassAltoHarmonicIntervals[i] == FullChordsVoicing[(nVoices * i) + ALTO] -
            FullChordsVoicing[nVoices * i + BASS]);
        rel(home, bassSopranoHarmonicIntervals[i] == FullChordsVoicing[(nVoices * i) + SOPRANO] -
            FullChordsVoicing[nVoices * i + BASS]);

        rel(home, tenorAltoHarmonicIntervals[i] == FullChordsVoicing[(nVoices * i) + ALTO] -
            FullChordsVoicing[(nVoices * i) + TENOR]);
        rel(home, tenorSopranoHarmonicIntervals[i] == FullChordsVoicing[(nVoices * i) + SOPRANO] -
            FullChordsVoicing[(nVoices * i) + TENOR]);

        rel(home, altoSopranoHarmonicIntervals[i] == FullChordsVoicing[(nVoices * i) + SOPRANO] -
            FullChordsVoicing[(nVoices * i) + ALTO]);
    }
}