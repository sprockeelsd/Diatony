#include "headers/Constraints.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                  Generic constraints                                                *
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
void link_melodic_arrays(Home home, int n, IntVarArray FullChordsVoicing, IntVarArray bassMelodicIntervals,
                         IntVarArray tenorMelodicIntervals, IntVarArray altoMelodicIntervals,
                         IntVarArray sopranoMelodicIntervals){
    for (int i = 0; i < n - 1; ++i)
    {
        rel(home, bassMelodicIntervals[i] == FullChordsVoicing[(i + 1) * 4] - FullChordsVoicing[i * 4]);
        rel(home, tenorMelodicIntervals[i] == FullChordsVoicing[((i + 1) * 4) + 1] - FullChordsVoicing[(i * 4) + 1]);
        rel(home, altoMelodicIntervals[i] == FullChordsVoicing[((i + 1) * 4) + 2] - FullChordsVoicing[(i * 4) + 2]);
        rel(home, sopranoMelodicIntervals[i] == FullChordsVoicing[((i + 1) * 4) + 3] - FullChordsVoicing[(i * 4) + 3]);
    }
}

/**
 * Link the harmonic intervals arrays to the FullChordsVoicing array
 * @param home the instance of the problem
 * @param n the number of chords
 * @param FullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param bassTenorHarmonicIntervals the harmonic intervals between bass and tenor
 * @param tenorAltoHarmonicIntervals the harmonic intervals between tenor and alto
 * @param altoSopranoHarmonicIntervals the harmonic intervals between alto and soprano
 */
void link_harmonic_arrays(Home home, int n, IntVarArray FullChordsVoicing, IntVarArray bassTenorHarmonicIntervals,
                          IntVarArray tenorAltoHarmonicIntervals, IntVarArray altoSopranoHarmonicIntervals){
    for(int i = 0; i < n - 1; ++i){
        rel(home, bassTenorHarmonicIntervals[i] == FullChordsVoicing[(4 * i) + 1] - FullChordsVoicing[4 * i]);
        rel(home, tenorAltoHarmonicIntervals[i] == FullChordsVoicing[(4 * i) + 2] - FullChordsVoicing[(4 * i) + 1]);
        rel(home, altoSopranoHarmonicIntervals[i] == FullChordsVoicing[(4 * i) + 3] - FullChordsVoicing[(4 * i) + 2]);
    }
}

/**
 * Fixes the domains of the different voices to their range
 *      bass: [40, 60] E2 -> C3
 *      tenor: [48, 69] C2 -> A3
 *      alto: [55, 75] G2 -> D3
 *      soprano: [60, 84] C3 -> A4
 * @param home the instance of the problem
 * @param n the number of chords
 * @param FullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 */
void restrain_voices_domains(Home home, int n, IntVarArray FullChordsVoicing){
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

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                               Chord-related constraints                                             *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Set the notes of the FullChordsVoicing array to the notes of the given chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void setToChord(Home home, Tonality* tonality, int degree, IntVarArgs currentChord){
    dom(home, currentChord, tonality->get_scale_degree_chord(degree));
}

/**
 * Set the bass of the chord to be the given note
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param state the state of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void setBass(Home home, Tonality *tonality, int degree, int state, IntVarArgs currentChord){
    dom(home, currentChord[0], tonality->get_scale_degree((degree + 2*state) % 7));
}

/** ---------------------------------------------Fundamental state chords--------------------------------------------- */

/**
 * @todo change this for complete and incomplete chords later (third must be <=1 depending on the chord before and after if they are 5->1 and complete/incomplete)
 * Sets the number of times each note of the notes of the chord are present in the chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chordNoteOccurrenceFundamentalState(Home home, Tonality *tonality, int degree, IntVarArgs currentChord){
    count(home, currentChord, tonality->get_scale_degree(degree), IRT_EQ,2); // double the bass which is also the tonic
    count(home, currentChord, tonality->get_scale_degree((degree + 2) % 7), IRT_EQ,1); // the third should be present once
    count(home, currentChord, tonality->get_scale_degree((degree + 4) % 7), IRT_EQ, 1); // the fifth should be present once
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                            Voice leading related constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/
