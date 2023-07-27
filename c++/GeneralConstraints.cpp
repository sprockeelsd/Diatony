#include "headers/GeneralConstraints.hpp"

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
 * Link the melodic intervals arrays to the FullChordsVoicing array for each voice
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
                         IntVarArray sopranoMelodicIntervals){
    for (int i = 0; i < n - 1; ++i)
    {
        rel(home, bassMelodicIntervals[i] == FullChordsVoicing[(i + 1) * 4 + BASS] - FullChordsVoicing[i * 4 + BASS]);
        rel(home,tenorMelodicIntervals[i] == FullChordsVoicing[((i + 1) * 4) + TENOR] - FullChordsVoicing[(i * 4) + TENOR]);
        rel(home, altoMelodicIntervals[i] == FullChordsVoicing[((i + 1) * 4) + ALTO] - FullChordsVoicing[(i * 4) + ALTO]);
        rel(home, sopranoMelodicIntervals[i] == FullChordsVoicing[((i + 1) * 4) + SOPRANO] - FullChordsVoicing[(i * 4) + SOPRANO]);
    }
}

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
                                  IntVarArray absoluteAltoMelodicIntervals, IntVarArray absoluteSopranoMelodicIntervals){
    for(int i = 0; i < bassMelodicIntervals.size(); ++i){
        abs(home, bassMelodicIntervals[i], absoluteBassMelodicIntervals[i]);
        abs(home, tenorMelodicIntervals[i], absoluteTenorMelodicIntervals[i]);
        abs(home, altoMelodicIntervals[i], absoluteAltoMelodicIntervals[i]);
        abs(home, sopranoMelodicIntervals[i], absoluteSopranoMelodicIntervals[i]);
    }
}

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
                          IntVarArray altoSopranoHarmonicIntervals){
    for(int i = 0; i < n; ++i){
        rel(home, bassTenorHarmonicIntervals[i] == FullChordsVoicing[(4 * i) + TENOR] - FullChordsVoicing[4 * i + BASS]);
        rel(home, bassAltoHarmonicIntervals[i] == FullChordsVoicing[(4 * i) + ALTO] - FullChordsVoicing[4 * i + BASS]);
        rel(home, bassSopranoHarmonicIntervals[i] == FullChordsVoicing[(4 * i) + SOPRANO] - FullChordsVoicing[4 * i + BASS]);

        rel(home, tenorAltoHarmonicIntervals[i] == FullChordsVoicing[(4 * i) + ALTO] - FullChordsVoicing[(4 * i) + TENOR]);
        rel(home, tenorSopranoHarmonicIntervals[i] == FullChordsVoicing[(4 * i) + SOPRANO] - FullChordsVoicing[(4 * i) + TENOR]);

        rel(home, altoSopranoHarmonicIntervals[i] == FullChordsVoicing[(4 * i) + SOPRANO] - FullChordsVoicing[(4 * i) + ALTO]);
    }
}

void link_diminished_chords_cost(Home home, int size, Tonality &tonality, vector<int> chordDegs, IntVarArray fullChordsVoicing, IntVarArray nOfDifferentNotes, IntVar costVar){
    for(int i = 0; i < size; ++i){
        if(tonality.get_chord_qualities()[chordDegs[i]] == DIMINISHED_CHORD){
            IntVarArgs currentChord(fullChordsVoicing.slice(4 * i, 1, 4));
            // nOfDiffVals in current chord > 3 => cost += 1
            nvalues(home, currentChord, IRT_EQ, nOfDifferentNotes[i]); // nOfDifferentNotes[i] = nOfDiffVals in current chord
        }
        else{
            rel(home, nOfDifferentNotes[i], IRT_EQ, 0); // does not matter then
        }
        count(home, nOfDifferentNotes, 4, IRT_EQ, costVar); // costVar = number of diminished chords with 4 notes
    }
}

/**
 * Sets the domains of the different voices to their range
 *      bass: [40, 60] E2 -> C3
 *      tenor: [48, 69] C2 -> A3
 *      alto: [55, 75] G2 -> D3
 *      soprano: [60, 84] C3 -> A4
 * and states that bass <= tenor <= alto <= soprano
 * @param home the instance of the problem
 * @param n the number of chords
 * @param FullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 */
void restrain_voices_domains(const Home& home, int n, IntVarArray FullChordsVoicing){
    for (int i = 0; i < n; ++i)
    {
        IntVarArgs currentChord(FullChordsVoicing.slice(4 * i, 1, 4));
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