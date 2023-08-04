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

using namespace Gecode;
using namespace Gecode::Search;
using namespace std;

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                FourVoiceTexture class                                               *
 *                                                                                                                     *
 ***********************************************************************************************************************/
class FourVoiceTexture: public IntLexMinimizeSpace {
protected:
    /// Data
    int n_of_voices = 4;        // The number of voices
    int size;                   // The size of the variable array of interest
    Tonality *tonality;         // The tonality of the piece
    vector<int> chordDegrees;   // The degrees of the chord of the chord progression
    vector<int> chordStates;    // The states of the chord of the chord progression (fundamental, 1st inversion,...)

    /// variable arrays for melodic intervals for each voice (not absolute value)
    IntVarArray bassMelodicIntervals;
    IntVarArray tenorMelodicIntervals;
    IntVarArray altoMelodicIntervals;
    IntVarArray sopranoMelodicIntervals;

    /// absolute melodic intervals
    IntVarArray absoluteBassMelodicIntervals;
    IntVarArray absoluteTenorMelodicIntervals;
    IntVarArray absoluteAltoMelodicIntervals;
    IntVarArray absoluteSopranoMelodicIntervals;

    /// variable arrays for harmonic intervals between adjacent voices (not absolute value)
    IntVarArray bassTenorHarmonicIntervals;
    IntVarArray bassAltoHarmonicIntervals;
    IntVarArray bassSopranoHarmonicIntervals;
    IntVarArray tenorAltoHarmonicIntervals;
    IntVarArray tenorSopranoHarmonicIntervals;
    IntVarArray altoSopranoHarmonicIntervals;

    ///global array for all the notes for all voices
    IntVarArray FullChordsVoicing; // [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]

    /// cost variables auxiliary arrays
    IntVarArray nDifferentValuesInDiminishedChord; // number of different note values in each chord
    IntVarArray nDifferentValuesAllChords;
    IntVarArray nOccurrencesBassInFundamentalState; // number of chords that don't double the bass in fundamental state

    /// cost variables
    IntVar sumOfMelodicIntervals;                       // for minimizing voice movement between voices
    IntVar nOfDiminishedChordsWith4notes;               // number of diminished chords that don't respect the preferences
    IntVar nOfChordsWithLessThan4notes;                 // number of chords with less than 4 notes
    IntVar nOfFundamentalStateChordsWithoutDoubledBass; // number of fundamental state chords that don't follow the preferences

public:
    /**
     * Constructor
     * @param s the size of the array of variables
     * @param *t a pointer to a Tonality object
     * @param chordDegs the degrees of the chord of the chord progression
     * @param chordStas the states of the chord of the chord progression (fundamental, 1st inversion,...)
     */
    FourVoiceTexture(int s, Tonality *t, vector<int> chordDegs, vector<int> chordStas);

    /**
     * Copy constructor
     * @param s an instance of the FourVoiceTexture class
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
    int* return_solution();

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
     * toString method
     * @return a string representation of the current instance of the FourVoiceTexture class.
     * Right now, it returns a string "FourVoiceTexture object. size = <size>"
     * If a variable is not assigned when this function is called, it writes <not assigned> instead of the value
     */
    string to_string();
};


/***********************************************************************************************************************
 *                                                Search engine methods                                                *
 ***********************************************************************************************************************/

/**
 * Creates a search engine for the given problem
 * @param pb an instance of the FourVoiceTexture class representing a given problem
 * @param type the type of search engine to create (see enumeration in headers/gecode_problem.hpp)
 * @return a search engine for the given problem
 */
Search::Base<FourVoiceTexture>* make_solver(FourVoiceTexture* pb, int type);

/**
 * Returns the next solution space for the problem
 * @param solver a solver for the problem
 * @return an instance of the FourVoiceTexture class representing the next solution to the problem
 */
FourVoiceTexture* get_next_solution_space(Search::Base<FourVoiceTexture>* solver);


/***********************************************************************************************************************
 *                                                 Auxiliary functions                                                 *
 ***********************************************************************************************************************/

/**
 * Write a text into a log file
 * @param message the text to write
 */
void write_to_log_file(const char* message);

#endif