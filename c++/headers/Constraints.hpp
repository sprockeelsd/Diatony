#ifndef FOURVOICETEXTURECONSTRAINTS
#define FOURVOICETEXTURECONSTRAINTS

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
 *                                                  Initial constraints                                                *
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
                         IntVarArray sopranoMelodicIntervals);

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
                          IntVarArray tenorAltoHarmonicIntervals, IntVarArray altoSopranoHarmonicIntervals);

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
void restrain_voices_domains(Home home, int n, IntVarArray FullChordsVoicing);

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                  Generic constraints                                                *
 *                                                                                                                     *
 ***********************************************************************************************************************/


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
void setToChord(Home home, Tonality* tonality, int degree, IntVarArgs currentChord);

/**
 *
 * @param home
 * @param tonality
 * @param degree
 * @param state
 * @param currentChord
 */
void setBass(Home home, Tonality *tonality, int degree, int state, IntVarArgs currentChord);


/***********************************************************************************************************************
 *                                                                                                                     *
 *                                            Voice leading related constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/


#endif
