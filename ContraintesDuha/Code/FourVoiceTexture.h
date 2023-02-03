/**
 * @file FourVoiceTexture.h
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This class creates a constraint problem to generate a 4 voice texture based on chord names, qualities and bass.
 * The variables are the following:
 * - An array for each voice containing the intervals between them. They are of size n-1 where n is the number of chords.
 * - An array for the chords. It is of size 4*n, and the first 4 variables represent one chord with the voices in ascending order (bass -> tenor -> alto -> soprano).
 *   The arrays are linked together
 * The currently supported constraints are the following :
 *      - The notes are in the right tonality
 *      - The notes are in the given chord
 *      - The seventh degree of a scale can never be doubled
 * @version 1.1
 * @date 2023-02-01
 *
 */

#ifndef FOUR_VOICE_TEXTURE
#define FOUR_VOICE_TEXTURE

#include <gecode/int.hh>
#include <gecode/minimodel.hh>

#include "Utilities.h"
#include "FourVoiceTextureConstraints.h"

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>

using namespace Gecode;
using namespace std;

class FourVoiceTexture : public IntMinimizeSpace
{
protected:
    int n;                              // The number of chord for which to generate 4 voice texture
    int key;                            // The key of the tonality of the chord progression
    vector<int> mode;                   // The mode of the tonality of the chord progression
    vector<int> chordRoots;             // The roots of the chords
    vector<vector<int>> chordQualities; // The qualities of the chords
    vector<int> chordBass;              // The bass of the chords

    IntSet fundamentals; // The set of notes that are fundamentals
    IntSet fourths;      // The set of notes that are fourths
    IntSet sevenths;     // The set of notes that are sevenths

    // Variable arrays for the melodic intervals of the different voices
    // Maybe change into Argument variables? Depending on whether we need to branch on them
    IntVarArray sopranoVoiceIntervals;
    IntVarArray altoVoiceIntervals;
    IntVarArray tenorVoiceIntervals;
    IntVarArray bassVoiceIntervals;

    // Array containing the chords
    // A 4*n array where each chord is represented with their voices in increasing order. Example :
    // [bass0, tenor0, alto0, soprano0, bass1, tenor1, alto1, soprano1, ...]
    IntVarArray chordsVoicings;

    // Cost variables
    IntVarArray intervalCosts; // Array to give priorities to the doubling of notes
    IntVar totalIntervalCost; // Sum of the doublingCosts

public:
    /**
     * @brief Construct a new Four Voice Texture object
     *
     * @param size the number of chords
     * @param key the key of the tonality
     * @param mode the mode of the tonality
     * @param chordRoots the roots of the chords
     * @param chordQualities the qualities of the chords
     * @param chordBass the bass of the chords
     */
    FourVoiceTexture(int size, int key, vector<int> mode, vector<int> chordRoots, vector<vector<int>> chordQualities, vector<int> chordBass);

    /**********************************************************************
     *                                                                    *
     *                          Support functions                         *
     *                                                                    *
     **********************************************************************/

    /**
     * @brief Search support, updates the variables
     *
     * @param s
     */
    FourVoiceTexture(FourVoiceTexture &s);

    /**
     * @brief Copies the instance of the problem
     *
     * @return Space* a copy of *this
     */
    virtual Space *copy(void)
    {
        return new FourVoiceTexture(*this);
    }

    /**
     * @brief Print the solution in letters
     *
     */
    void printNoteNames(void) const;

    /**
     * @brief Print all the variables, used for development
     *
     */
    void printDevelop(void) const;

    /**
     * @brief Print the solution
     *
     */
    void print(void) const;

    /**
     * @brief Prints the solution in an OM-ready way (parenthesis with the note values in MIDIcent). Example output : ((6000 6200 6400) (6500 6700 6900))
     *
     */
    void printForOM(void) const;

    /**
     * @brief This method is called when a Branch and Bound solver is used everytime a solution is found by the solver.
     *
     * @param _b The solution found by the solver
     */
    virtual void constrain(const Space &_b);

    virtual IntVar cost(void) const;
};

#endif