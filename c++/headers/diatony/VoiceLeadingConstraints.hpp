// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A file that contains all the melodic constraints for the diatonic harmonic problem
//
#ifndef VOICELEADINGCONSTRAINTS_HPP
#define VOICELEADINGCONSTRAINTS_HPP

#include "../aux/Tonality.hpp"
#include "../aux/Utilities.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 * This file contains all the voice leading constraints, that is constraints on how voices move through successive     *
 * chords.                                                                                                             *
 * It currently contains the following constraints:                                                                    *
 *      - forbid_parallel_interval: forbids a given parallel interval between two voices                               *
 *      - Contrary motion to bass: enforces that all voices move in contrary movement to the bass                      *
 *      - Tritone resolution: forces the tritone to resolve properly                                                   *
 *      - interrupted_cadence: sets the constraint for a fifth degree followed by a sixth degree in fundamental state  *
 *      - Italian augmented sixth: ensures proper resolution                                                           *
 *      - Species seventh: the seventh must be prepared                                                                *
 *      - Fifth degree appogiatura: ensures proper voice movements                                                     *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Forbids a list of parallel intervals between two voices. This calls the forbid_parallel_interval function
 * @param home the instance of the problem
 * @param size the size of the chord progression
 * @param nOfVoices the number of voices
 * @param intervals the list of intervals to forbid
 * @param FullChordsVoicing the array containing all the notes of the chords in the progression
 * @param bassTenorHarmonicIntervals the array containing the harmonic intervals between bass and tenor
 * @param bassAltoHarmonicIntervals the array containing the harmonic intervals between bass and alto
 * @param bassSopranoHarmonicIntervals the array containing the harmonic intervals between bass and soprano
 * @param tenorAltoHarmonicIntervals the array containing the harmonic intervals between tenor and alto
 * @param tenorSopranoHarmonicIntervals the array containing the harmonic intervals between tenor and soprano
 * @param altoSopranoHarmonicIntervals the array containing the harmonic intervals between alto and soprano
 * @param start_position optional parameter to define the start position
 * @param end_position optional parameter to define the end position
 */
void forbid_parallel_intervals(const Home &home, int size, int nOfVoices, const vector<int> &intervals,
    const IntVarArray &FullChordsVoicing,           const IntVarArray &bassTenorHarmonicIntervals,
    const IntVarArray &bassAltoHarmonicIntervals,   const IntVarArray &bassSopranoHarmonicIntervals,
    const IntVarArray &tenorAltoHarmonicIntervals,  const IntVarArray &tenorSopranoHarmonicIntervals,
    const IntVarArray &altoSopranoHarmonicIntervals, int start_position = 0, int end_position = -1);

/**
 * Forbids a given parallel interval between two voices
 * @param home the instance of the problem
 * @param nVoices the number of voices
 * @param forbiddenParallelInterval the interval that must not be parallel
 * @param currentPosition the current position in the chord progression
 * @param voice1ID the ID of the first voice
 * @param voice2ID the ID of the second voice
 * @param voicesHarmonicIntervals the array containing the harmonic intervals between adjacent voices
 * @param FullChordsVoicing the array containing all the notes of the chords in the progression
 */
void forbid_parallel_interval(Home home, int nVoices, int forbiddenParallelInterval, int currentPosition,
    int voice1ID, int voice2ID, IntVarArray voicesHarmonicIntervals, IntVarArray FullChordsVoicing);

/**
 * Adds the constraint that Soprano, Alto and Tenor must move in contrary motion to the bass
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param bassMelodicInterval the array containing the melodic intervals of the bass
 * @param tenorMelodicInterval the array containing the melodic intervals of the tenor
 * @param altoMelodicInterval the array containing the melodic intervals of the alto
 * @param sopranoMelodicInterval the array containing the melodic intervals of the soprano
 */
void contrary_motion_to_bass(const Home& home, int currentPosition, const IntVarArray& bassMelodicInterval,
    const IntVarArray& tenorMelodicInterval, const IntVarArray& altoMelodicInterval,
    const IntVarArray& sopranoMelodicInterval);

/**
 * Forces the tritone to resolve properly
 * @param home the instance of the problem
 * @param nVoices the number of voices in the piece
 * @param currentPosition the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param chordDegs the degrees of the chords
 * @param chordQuals the qualities of the chords
 * @param chordStas the state of the chords
 * @param bassMelodicIntervals the melodic interval of the bass between the current position and the next
 * @param tenorMelodicIntervals the melodic interval of the tenor between the current position and the next
 * @param altoMelodicIntervals the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicIntervals the melodic interval of the soprano between the current position and the next
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 */
void tritone_resolution(const Home &home, int nVoices, int currentPosition, Tonality *tonality, const vector<int> &chordDegs,
    const vector<int> &chordQuals, const vector<int> &chordStas, const IntVarArray &bassMelodicIntervals,
    const IntVarArray &tenorMelodicIntervals, const IntVarArray &altoMelodicIntervals,
    const IntVarArray &sopranoMelodicIntervals, IntVarArray &fullChordsVoicing);

/**
 * Sets the constraint for a fifth degree followed by a sixth degree in fundamental state.
 * The seventh of the scale must rise to the tonic, and the other voices are going down (except for the bass)
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param fullChordsVoicing the array containing the notes of the chords in the progression
 * @param tenorMelodicInterval the melodic intervals of the tenor
 * @param altoMelodicInterval the melodic intervals of the alto
 * @param sopranoMelodicInterval the melodic intervals of the soprano
 */
void interrupted_cadence(const Home &home, int currentPosition, Tonality *tonality, IntVarArray fullChordsVoicing,
    const IntVarArray &tenorMelodicInterval, const IntVarArray &altoMelodicInterval,
    const IntVarArray &sopranoMelodicInterval);

/**
 * Posts the voicing constraints for augmented sixth chords (italian)
 * @param home the instance of the problem
 * @param nOfVoices the number of voices in the piece
 * @param currentPosition the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 * @param bassMelodicIntervals the melodic interval of the bass between the current position and the next
 * @param tenorMelodicIntervals the melodic interval of the tenor between the current position and the next
 * @param altoMelodicIntervals the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicIntervals the melodic interval of the soprano between the current position and the next
 */
void italian_augmented_sixth(const Home &home, int nOfVoices, int currentPosition, Tonality *tonality, IntVarArray &fullChordsVoicing,
    const IntVarArray &bassMelodicIntervals, const IntVarArray &tenorMelodicIntervals,
    const IntVarArray &altoMelodicIntervals, const IntVarArray &sopranoMelodicIntervals);

/**
 * Ensures that the seventh of the chord is prepared in the previous chord
 * @param home the instance of the problem
 * @param nOfVoices the number of voices in the piece
 * @param currentPosition the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param chordDegrees the degrees of the chords in the progression
 * @param chordQualities the qualities of the chords in the progression
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 */
void species_seventh(const Home &home, int nOfVoices, int currentPosition, Tonality* tonality,
    const vector<int> &chordDegrees, const vector<int> &chordQualities, IntVarArray &fullChordsVoicing);

/**
 * Sets the constraint for a first degree in second inversion followed by a fifth degree (appogiatura)
 * @param home the instance of the problem
 * @param nVoices the number of voices in the piece
 * @param currentPosition the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 * @param bassMelodicInterval the melodic interval of the bass between the current position and the next
 * @param tenorMelodicInterval the melodic interval of the tenor between the current position and the next
 * @param altoMelodicInterval the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicInterval the melodic interval of the soprano between the current position and the next
 */
void fifth_degree_appogiatura(const Home& home, int nVoices, int currentPosition, Tonality *tonality, IntVarArray &fullChordsVoicing,
    IntVarArray &bassMelodicInterval, const IntVarArray& tenorMelodicInterval,
    const IntVarArray& altoMelodicInterval, const IntVarArray& sopranoMelodicInterval);

#endif