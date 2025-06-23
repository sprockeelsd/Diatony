// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A class that creates a constraint representation of a four voice harmonic problem of tonal music.
//
#ifndef TONAL_PROGRESSION_HPP
#define TONAL_PROGRESSION_HPP

#include "../aux/Utilities.hpp"
#include "TonalProgressionParameters.hpp"
#include "GeneralConstraints.hpp"
#include "HarmonicConstraints.hpp"
#include "VoiceLeadingConstraints.hpp"
#include "Preferences.hpp"

using namespace Gecode;
using namespace Gecode::Search;
using namespace std;

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                FourVoiceTexture class                                               *
 *                                                                                                                     *
 ***********************************************************************************************************************/
 /**
  * This class models a classic 4-voice harmonic problem of tonal music. It takes as arguments a number of chords, a
  * tonality, and a series of chords identified by their degree, quality and state. It models a 4-voice chord
  * progression following traditional rules and preferences of western tonal harmony for diatonic chord progressions.
  */

class TonalProgression {
protected:
    int                             nVoices = 4;
    TonalProgressionParameters*     params;
    IntArgs                         nOfNotesInChord;                            // The max number of notes in each chord

    /** ----------------------------------Problem variables--------------------------------------------------------- **/

    ///global array for all the notes for all voices
    IntVarArray                 voicing; // contains the notes for the four voices [bass0, tenor0, alto0, soprano0, bass1, tenor1, alto1, soprano1, ...]

    /// Melodic intervals
    IntVarArray                 bassMelodicIntervals;
    IntVarArray                 tenorMelodicIntervals;
    IntVarArray                 altoMelodicIntervals;
    IntVarArray                 sopranoMelodicIntervals;

    // contains the melodic intervals for all voices [bass0, tenor0, alto0, soprano0, bass1, tenor1, alto1, soprano1, ...]
    IntVarArray                 allMelodicIntervals;

    /// Harmonic intervals (always positive)
    IntVarArray                 bassTenorHarmonicIntervals;
    IntVarArray                 bassAltoHarmonicIntervals;
    IntVarArray                 bassSopranoHarmonicIntervals;
    IntVarArray                 tenorAltoHarmonicIntervals;
    IntVarArray                 tenorSopranoHarmonicIntervals;
    IntVarArray                 altoSopranoHarmonicIntervals;

    /// cost variables auxiliary arrays
    IntVarArray                 nDifferentValuesInDiminishedChord;  /// The number of different note values in diminished chords (distinct value, e.g., 60,72
    IntVarArray                 noFDifferentNotesInChords;          /// the number of different notes (e.g., root, third,...) in each chord

    /// Variables for each type of interval
    IntVar                      nOfUnisons;

    /// cost variables
    IntVar                      nOfIncompleteChords;

    /** ---------------------------------------------Methods-------------------------------------------------------- **/

public:
    /**
     * Constructor
     * @param home the space of the problem
     * @param params an object containing the parameters of the problem
     * @param fullVoicing the general array for the voicing of the whole piece
     * @param bassIntervals the melodic intervals for the bass voice
     * @param tenorIntervals the melodic intervals for the tenor voice
     * @param altoIntervals the melodic intervals for the alto voice
     * @param sopranoIntervals the melodic intervals for the soprano voice
     * @param allMIntervals the melodic intervals for all voices
     * @param bassTenorIntervals the harmonic intervals between the bass and tenor voices
     * @param bassAltoIntervals the harmonic intervals between the bass and alto voices
     * @param bassSopranoIntervals the harmonic intervals between the bass and soprano voices
     * @param tenorAltoIntervals the harmonic intervals between the tenor and alto voices
     * @param tenorSopranoIntervals the harmonic intervals between the tenor and soprano voices
     * @param altoSopranoIntervals the harmonic intervals between the alto and soprano voices
     * @param nDifferentValuesInDimChord the number of different values in diminished chords
     * @param nDNotesInChords the number of different notes in each chord
     * @param nIncompleteChords the number of incomplete chords in the progression
     * @return an object constraining the variables on which the problem is defined
     * /!\ dominant diminished seventh chords are considered as minor ninth dominant chords without their fundamental
     */
    TonalProgression(Home home, TonalProgressionParameters* params,
        IntVarArray& fullVoicing,                   IntVarArray& bassIntervals,         IntVarArray& tenorIntervals,
        IntVarArray& altoIntervals,                 IntVarArray& sopranoIntervals,      IntVarArray& allMIntervals,
        IntVarArray& bassTenorIntervals,            IntVarArray& bassAltoIntervals,     IntVarArray& bassSopranoIntervals,
        IntVarArray& tenorAltoIntervals,            IntVarArray& tenorSopranoIntervals, IntVarArray& altoSopranoIntervals,
        IntVarArray& nDifferentValuesInDimChord,    IntVarArray& nDNotesInChords,       IntVar& nIncompleteChords);

    // todo constructor for all solutions within a margin of the cost vector

    /**
     * Copy constructor
     * @param home the space of the problem
     * @param s an instance of the TonalProgression class
     * @return a copy of the given instance of the TonalProgression class
     * /!\ It is important to copy every variable of the given instance to the new instance
     */
    TonalProgression(Home home, TonalProgression &s);

    /**                 getters                      **/
    int get_size() const { return params->get_size(); }

    IntVarArray &getFullVoicing(){ return voicing; }

    IntVarArray &getBassMelodicIntervals(){ return bassMelodicIntervals; }

    IntVarArray &getTenorMelodicIntervals(){ return tenorMelodicIntervals; }

    IntVarArray &getAltoMelodicIntervals(){ return altoMelodicIntervals; }

    IntVarArray &getSopranoMelodicIntervals(){ return sopranoMelodicIntervals; }

    IntVarArray &getBassTenorHarmonicIntervals(){ return bassTenorHarmonicIntervals; }

    IntVarArray &getBassAltoHarmonicIntervals(){ return bassAltoHarmonicIntervals; }

    IntVarArray &getBassSopranoHarmonicIntervals(){ return bassSopranoHarmonicIntervals; }

    IntVarArray &getTenorAltoHarmonicIntervals(){ return tenorAltoHarmonicIntervals; }

    IntVarArray &getTenorSopranoHarmonicIntervals(){ return tenorSopranoHarmonicIntervals; }

    IntVarArray &getAltoSopranoHarmonicIntervals(){ return altoSopranoHarmonicIntervals; }

    IntVarArray &getNDifferentValuesInDiminishedChord(){ return nDifferentValuesInDiminishedChord; }

    /**
     * Returns the values taken by the fullVoicing array in a solution as a pointer to an integer array
     * @return an array of integers representing the values of the variables in a solution
     */
    int* return_solution() const;

    /**
     * returns the parameters in a string
     * @return a string containing the parameters of the problem
     */
    string parameters() const;

    /**
     * to_string method
     * @return A string representation of the current instance of the TonalProgression class.
     */
    string to_string() const;
};
#endif