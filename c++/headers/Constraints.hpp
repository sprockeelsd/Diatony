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
 * Link the harmonic intervals arrays to the FullChordsVoicing array
 * @param home the instance of the problem
 * @param n the number of chords
 * @param FullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param bassTenorHarmonicIntervals the harmonic intervals between bass and tenor
 * @param tenorAltoHarmonicIntervals the harmonic intervals between tenor and alto
 * @param altoSopranoHarmonicIntervals the harmonic intervals between alto and soprano
 */
void link_harmonic_arrays(const Home& home, int n, IntVarArray FullChordsVoicing, IntVarArray bassTenorHarmonicIntervals,
                          IntVarArray tenorAltoHarmonicIntervals, IntVarArray altoSopranoHarmonicIntervals);

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

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                  Generic constraints                                                *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Forbids parallel intervals between two chords in the same voices
 * @param home the instance of the problem
 * @param forbiddenParallelInterval the interval to forbid
 * @param currentPosition the current chord position
 * @param lowerVoiceID the ID of the lower voice (0 -> bass, 1 -> tenor, 2 -> alto, 3 -> soprano)
 * @param voicesHarmonicIntervals The variable array containing the harmonic intervals between the two voices
 * @param FullChordsVoicing the variable array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 */
void forbid_parallel_intervals(Home home, int forbiddenParallelInterval, int currentPosition, int lowerVoiceID,
                               IntVarArray voicesHarmonicIntervals, IntVarArray FullChordsVoicing);

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
void setToChord(const Home& home, Tonality* tonality, int degree, const IntVarArgs& currentChord);

/**
 * Set the bass of the chord to be the given note
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param state the state of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void setBass(const Home& home, Tonality *tonality, int degree, int state, IntVarArgs currentChord);

/**
 * @todo change this for complete and incomplete chords later (third must be <=1 depending on the chord before and after if they are 5->1 and complete/incomplete)
 * Sets the number of times each note of the notes of the chord are present in the chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chordNoteOccurrenceFundamentalState(const Home& home, Tonality *tonality, int degree, const IntVarArgs& currentChord);


/***********************************************************************************************************************
 *                                                                                                                     *
 *                                            Voice leading related constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the rules for the melodic movements between chords in fundamental state
 * For chords that are 1 degree apart, the other voices must move in contrary motion to the bass
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param chordDegrees the array containing the degrees of the chords in the progression
 * @param bassMelodicInterval The melodic interval of the bass between the current position and the next
 * @param tenorMelodicInterval the melodic interval of the tenor between the current position and the next
 * @param altoMelodicInterval the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicInterval the melodic interval of the soprano between the current position and the next
 */
void fundamentalStateChordToFundamentalStateChord(const Home& home, int currentPosition, vector<int> chordDegrees,
                                                  Tonality &tonality,
                                                  const IntVarArray& bassMelodicInterval, const IntVarArray& tenorMelodicInterval,
                                                  const IntVarArray& altoMelodicInterval, const IntVarArray& sopranoMelodicInterval,
                                                  IntVarArray fullChordsVoicing);

/**
 * Sets the constraint for a fifth degree followed by a sixth degree in fundamental state
 * the seventh of the scale must rise to the tonic and the other voices are going down (except for the bass)
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param bassMelodicInterval the melodic intervals of the bass
 * @param tenorMelodicInterval the melodic intervals of the tenor
 * @param altoMelodicInterval the melodic intervals of the alto
 * @param sopranoMelodicInterval the melodic intervals of the soprano
 * @param fullChordsVoicing the array containing the notes of the chords in the progression
 */
void fifthDegreeFSToSixthDegreeFS(const Home& home, int currentPosition, Tonality& tonality,
                                  const IntVarArray& tenorMelodicInterval, const IntVarArray& altoMelodicInterval,
                                  const IntVarArray& sopranoMelodicInterval, IntVarArray fullChordsVoicing);


#endif