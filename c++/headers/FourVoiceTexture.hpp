#ifndef space_wrapper_hpp
#define space_wrapper_hpp

#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <ctime>
#include <exception>

#include "gecode/kernel.hh"
#include "gecode/int.hh"
#include "gecode/search.hh"
#include "gecode/minimodel.hh"
#include "gecode/set.hh"

#include "Utilities.hpp"
#include "Tonality.hpp"
#include "MajorTonality.hpp"
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
  * This class models a classic 4 voice harmonic problem of tonal music. It takes as arguments a tonality, and a series
  * of chords identified by their degree and state. It then generates a 4 voice chord progression following traditional
  * rules of western tonal harmony.
  */
class FourVoiceTexture: public IntLexMinimizeSpace {
protected:
    /// Data
    int nOfVoices = 4;        // The number of voices
    int size;                   // The size of the variable array of interest
    Tonality *tonality;         // The tonality of the piece
    vector<int> chordDegrees;   // The degrees of the chord of the chord progression
    vector<int> chordStates;    // The states of the chord of the chord progression (fundamental, 1st inversion,...)

    /// variable arrays for melodic intervals for each voice (not absolute value)
    IntVarArray bassMelodicIntervals;
    IntVarArray tenorMelodicIntervals;
    IntVarArray altoMelodicIntervals;
    IntVarArray sopranoMelodicIntervals;
    IntVarArray allMelodicIntervals;

    /// absolute melodic intervals
    IntVarArray absoluteBassMelodicIntervals;
    IntVarArray absoluteTenorMelodicIntervals;
    IntVarArray absoluteAltoMelodicIntervals;
    IntVarArray absoluteSopranoMelodicIntervals;

    /// variable arrays for harmonic intervals between adjacent voices (not absolute value but are always positive)
    IntVarArray bassTenorHarmonicIntervals;
    IntVarArray bassAltoHarmonicIntervals;
    IntVarArray bassSopranoHarmonicIntervals;
    IntVarArray tenorAltoHarmonicIntervals;
    IntVarArray tenorSopranoHarmonicIntervals;
    IntVarArray altoSopranoHarmonicIntervals;

    ///global array for all the notes for all voices
    IntVarArray FullChordsVoicing;                      // [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]

    /// cost variables auxiliary arrays
    IntVarArray nDifferentValuesInDiminishedChord;      // number of different note values in each chord
    IntVarArray nDifferentValuesAllChords;
    IntVarArray nOccurrencesBassInFundamentalState;     // number of chords that don't double the bass in fundamental state
    IntVarArray commonNotesInSoprano;                   // chords with common notes in outside voices
    IntVarArray commonNotesInSameVoice; // chords with common notes in the same voice between consecutive chords

    /// cost variables
    IntVar sumOfMelodicIntervals;                       // for minimizing voice movement between voices
    IntVar nOfDiminishedChordsWith4notes;               // number of diminished chords that don't respect the preferences
    IntVar nOfChordsWithLessThan4notes;                 // number of chords with less than 4 notes
    IntVar nOfFundamentalStateChordsWithoutDoubledBass; // number of fundamental state chords that don't follow the preferences
    IntVar nOfCommonNotesInSoprano;                     // number of common notes in outside voices
    IntVar nOfCommonNotesInSameVoice;                   // number of common notes in the same voice between consecutive chords
    /// /!\ this cost needs to be maximized, so its value is negative

public:
    /**
     * Constructor
     * @param s the number of chords in the progression
     * @param *t a pointer to a Tonality object
     * @param chordDegs the degrees of the chord of the chord progression
     * @param chordStas the states of the chord of the chord progression (fundamental, 1st inversion,...)
     * @return a FourVoiceTexture object
     */
    FourVoiceTexture(int s, Tonality *t, vector<int> chordDegs, vector<int> chordStas);

    /**
     * Copy constructor
     * @param s an instance of the FourVoiceTexture class
     * @return a copy of the given instance of the FourVoiceTexture class
     */
    FourVoiceTexture(FourVoiceTexture &s);

    /**
     * Returns the size of the problem
     * @return an integer representing the size of the vars array
     */
    int get_size() const;

    /**
     * Returns the values taken by the variables vars in a solution
     * @return an array of integers representing the values of the variables in a solution
     */
    int* return_solution() const;

    /**
     * Copy method
     * @return a copy of the current instance of the FourVoiceTexture class. Calls the copy constructor
     */
    virtual Space *copy(void);

    /**
     * Constrain method for bab search
     * @param _b a space to constrain the current instance of the FourVoiceTexture class with upon finding a solution
     */
    // virtual void constrain(const Space& _b);

    virtual IntVarArgs cost(void) const;

    /**
     * Prints the solution in the console
     */
    void print_solution();

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