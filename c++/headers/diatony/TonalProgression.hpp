// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A class that creates a constraint representation of a four voice harmonic problem of tonal music.
//
#ifndef TONAL_PROGRESSION_HPP
#define TONAL_PROGRESSION_HPP

#include "../aux/Utilities.hpp"
#include "../aux/Tonality.hpp"
#include "../aux/MajorTonality.hpp"
#include "../aux/MinorTonality.hpp"
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
  * tonality, and a series of chords identified by their degree, quality and state. It models a 4 voice chord
  * progression following traditional rules and preferences of western tonal harmony for diatonic chord progressions.
  */

class TonalProgression {
protected:
    int                             nVoices = 4;                                // The number of voices
    TonalProgressionParameters*     params;                                     // The parameters of the problem
    IntArgs                         nOfNotesInChord;                            // The max number of notes in each chord

    /** ----------------------------------Problem variables--------------------------------------------------------- **/

    ///global array for all the notes for all voices
    IntVarArray                 voicing; // contains the notes for the four voices [bass0, tenor0, alto0, soprano0, bass1, tenor1, alto1, soprano1, ...]

    /// Melodic intervals
    IntVarArray                 bassMelodicIntervals;
    IntVarArray                 tenorMelodicIntervals;
    IntVarArray                 altoMelodicIntervals;
    IntVarArray                 sopranoMelodicIntervals;

    IntVarArray                 allMelodicIntervals;        // contains the melodic intervals for all voices [bass0, tenor0, alto0, soprano0, bass1, tenor1, alto1, soprano1, ...]

    /// Harmonic intervals (always positive)
    IntVarArray                 bassTenorHarmonicIntervals;
    IntVarArray                 bassAltoHarmonicIntervals;
    IntVarArray                 bassSopranoHarmonicIntervals;
    IntVarArray                 tenorAltoHarmonicIntervals;
    IntVarArray                 tenorSopranoHarmonicIntervals;
    IntVarArray                 altoSopranoHarmonicIntervals;

    /// cost variables auxiliary arrays
    IntVarArray                 nDifferentValuesInDiminishedChord;
    IntVarArray                 nOFDifferentNotesInChords;
    IntVarArray                 commonNotesInSameVoice;

    /// Variables for each type of interval
    IntVar                      nOfUnisons;                                // number of intervals that are a unison

    /// cost variables
    IntVar                      nOfIncompleteChords;
    IntVar                      nOfCommonNotesInSameVoice;                  // /!\ this cost needs to be maximized, so its value is negative

    /** ---------------------------------------------Methods-------------------------------------------------------- **/

public:
    /**
     * Constructor
     * @param home the space of the problem
     * @param params an object containing the parameters of the problem
     * @param fullVoicing the general array for the voicing of the whole piece
     * @param bassIntervals
     * @param tenorIntervals
     * @param altoIntervals
     * @param sopranoIntervals
     * @param allMIntervals
     * @param nDifferentValuesInDimChord
     * @return an object constraining the variables on which the problem is defined
     * /!\ dominant diminished seventh chords are considered as minor ninth dominant chords without their fundamental
     */
    TonalProgression(Home home, TonalProgressionParameters* params,
        IntVarArray& fullVoicing, IntVarArray& bassIntervals, IntVarArray& tenorIntervals,
        IntVarArray& altoIntervals, IntVarArray& sopranoIntervals,
        IntVarArray& allMIntervals, IntVarArray& nDifferentValuesInDimChord);

    // todo constructor for all solutions within a margin of the cost vector
    // TonalProgression(Home home, int s, Tonality *t, vector<int> chordDegs, vector<int> chordQuals, vector<int> chordStas,
    // IntVarArray& fullVoicing,
    // vector<int> costs, double margin = 0.0);

    /**
     * Copy constructor
     * @param home the space of the problem
     * @param s an instance of the TonalProgression class
     * @return a copy of the given instance of the TonalProgression class
     * /!\ It is important to copy every variable instance variable of the given instance to the new instance
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
    string parameters();

    /**
     * toString method
     * @return a string representation of the current instance of the FourVoiceTexture class.
     * Right now, it returns a string "FourVoiceTexture object. size = <size>"
     * If a variable is not assigned when this function is called, it writes <not assigned> instead of the value
     */
    string to_string();
};
#endif