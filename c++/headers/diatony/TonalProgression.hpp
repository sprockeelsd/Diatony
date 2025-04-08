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
  * This class models a classic 4 voice harmonic problem of tonal music. It takes as arguments a number of chords, a
  * tonality, and a series of chords identified by their degree, quality and state. It models a 4 voice chord
  * progression following traditional rules and preferences of western tonal harmony for diatonic chord progressions.
  */

class TonalProgression {
protected:
    /** ----------------------------------Problem parameters-------------------------------------------------------- **/

    int                         nVoices = 4;                                // The number of voices
    int                         size;                                       // The number of chords
    Tonality*                   tonality;                                   // The tonality of the piece
    vector<int>                 chordDegrees;                               // The degrees of the chords
    vector<int>                 chordQualities;                             // The qualities of the chords
    vector<int>                 chordStates;                                // The states of the chords
    IntArgs                     nOfNotesInChord;                            // The max number of notes in each chord

    /** ----------------------------------Problem variables--------------------------------------------------------- **/

    ///global array for all the notes for all voices
    IntVarArray                 voicing;

    /// Melodic intervals
    IntVarArray                 bassMelodicIntervals;
    IntVarArray                 tenorMelodicIntervals;
    IntVarArray                 altoMelodicIntervals;
    IntVarArray                 sopranoMelodicIntervals;

    IntVarArray                 allMelodicIntervals;

    /// Harmonic intervals (always positive)
    IntVarArray                 bassTenorHarmonicIntervals;
    IntVarArray                 bassAltoHarmonicIntervals;
    IntVarArray                 bassSopranoHarmonicIntervals;
    IntVarArray                 tenorAltoHarmonicIntervals;
    IntVarArray                 tenorSopranoHarmonicIntervals;
    IntVarArray                 altoSopranoHarmonicIntervals;

    /// cost variables auxiliary arrays
    IntVarArray                 nDifferentValuesInDiminishedChord;
    IntVarArray                 nDifferentValuesAllChords;
    IntVarArray                 nOFDifferentNotesInChords;
    IntVarArray                 commonNotesInSameVoice;
    IntVarArray                 costsAllMelodicIntervals;

    /// Variables for each type of interval
    IntVar                      nOfUnissons;                                // number of intervals that are a unisson

    /// cost variables
    IntVar                      nOfFundStateDiminishedChordsWith4notes;
    IntVar                      nOfChordsWithLessThan4Values;
    IntVar                      nOfIncompleteChords;
    IntVar                      nOfCommonNotesInSameVoice;                  /// /!\ this cost needs to be maximized, so its value is negative
    IntVar                      costOfMelodicIntervals;

    IntVarArgs                  costVector;                                 // the costs in lexicographical order for minimization

    /** ---------------------------------------------Methods-------------------------------------------------------- **/

public:
    /**
     * Constructor
     * @param home
     * @param s the number of chords in the progression
     * @param *t a pointer to a Tonality object
     * @param chordDegs the degrees of the chord of the chord progression
     * @param chordQuals the qualities of the chord of the chord progression
     * @param chordStas the states of the chord of the chord progression (fundamental, 1st inversion,...)
     * @param fullVoicing
     * @return an instance of FourVoiceTexture initialized with the given parameters, constraints and branching strategies
     * posted as well as the cost vector to minimize in lexicographical order
     */
    TonalProgression(Home home, int s, Tonality *t,
        vector<int> chordDegs, vector<int> chordQuals, vector<int> chordStas,
        IntVarArray& fullVoicing);

    /**
     * Constructor to find all optimal solutions (with or without margin) based on the cost vector for one of the best ones.
     * @param home
     * @param s the number of chords in the progression
     * @param *t a pointer to a Tonality object
     * @param chordDegs the degrees of the chord of the chord progression
     * @param chordQuals the qualities of the chord of the chord progression
     * @param chordStas the states of the chord of the chord progression (fundamental, 1st inversion,...)
     * @param costs the cost vector for one of the best solutions, found by solving the optimization problem first
     * @param margin the offset percentage to add to the melodic cost vector to find close to optimal solutions
     */
    // TonalProgression(Home home, int s, Tonality *t, vector<int> chordDegs, vector<int> chordQuals, vector<int> chordStas,
    // IntVarArray& fullVoicing,
    // vector<int> costs, double margin = 0.0);

    /**
     * Copy constructor
     * @param home
     * @param s an instance of the FourVoiceTexture class
     * @return a copy of the given instance of the FourVoiceTexture class
     * /!\ It is important to copy every variable instance variable of the given instance to the new instance
     */
    TonalProgression(Home home, TonalProgression &s);

    /**
     * Returns the number of chords of the problem
     * @return an integer representing the number of chords of the problem
     */
    int get_size() const { return size; }

    /**
     * Returns the array of variables for the notes
     * @return
     */
    IntVarArray getFullVoicing(){ return voicing; }

    IntVarArray getBassMelodicIntervals(){ return bassMelodicIntervals; }

    IntVarArray getTenorMelodicIntervals(){ return tenorMelodicIntervals; }

    IntVarArray getAltoMelodicIntervals(){ return altoMelodicIntervals; }

    IntVarArray getSopranoMelodicIntervals(){ return sopranoMelodicIntervals; }

    IntVarArray getBassTenorHarmonicIntervals(){ return bassTenorHarmonicIntervals; }

    IntVarArray getBassAltoHarmonicIntervals(){ return bassAltoHarmonicIntervals; }

    IntVarArray getBassSopranoHarmonicIntervals(){ return bassSopranoHarmonicIntervals; }

    IntVarArray getTenorAltoHarmonicIntervals(){ return tenorAltoHarmonicIntervals; }

    IntVarArray getTenorSopranoHarmonicIntervals(){ return tenorSopranoHarmonicIntervals; }

    IntVarArray getAltoSopranoHarmonicIntervals(){ return altoSopranoHarmonicIntervals; }

    /**
     * Returns the values taken by the variables vars in a solution as a pointer to an integer array
     * @return an array of integers representing the values of the variables in a solution
     */
    int* return_solution() const;

    /**
     * Returns the cost variables in lexicographical order
     * @return an IntVarArgs containing cost variables in lexicographical order ONCE A SOLUTION IS FOUND
     */
    IntVarArgs get_cost_vector() const;

    /**
     * Cost function for lexicographical minimization. The order is as follows:
     * 1. Number of incomplete chords.
     * 2. Number of diminished chords in fundamental state with 4 notes.
     * 3. Number of chords with less than 4 note values.
     * 4. Number of fundamental state chords without doubled bass.
     * 5. Weighted sum of melodic intervals.
     * 6. Number of common notes in the same voice between consecutive chords.
     * @return the cost variables in order of importance
     */
    // virtual IntVarArgs cost() const;

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