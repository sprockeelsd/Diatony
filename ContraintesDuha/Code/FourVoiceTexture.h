/**
 * @file FourVoiceTexture.h
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This class creates a constraint problem to generate a 4 voice texture based on chord names and inversion.
 * The variables are the following:
 * - An array for each voice containing the intervals between them. They are of size n-1 where n is the number of chords.
 * - An array for the chords. It is of size 4*n, and the first 4 variables represent one chord with the voices in ascending order (bass -> tenor -> alto -> soprano).
 * @version 1.0
 * @date 2023-01-25
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

class FourVoiceTexture : public Space
{
protected:
    int n;                              // The number of chord for which to generate 4 voice texture
    int key;                            // The key of the tonality of the chord progression
    vector<int> mode;                   // The mode of the tonality of the chord progression
    vector<int> chordRoots;             // The roots of the chords
    vector<vector<int>> chordQualities; // The qualities of the chords
    vector<int> chordBass;              // The bass of the chords

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
     *                          Constraint functions                      *
     *                                                                    *
     **********************************************************************/

    /**
     * @brief Posts the constraint that the different voices of the chord have a value that is part of the chord
     *
     * @param chordNotes the variables representing the notes of the chord 
     * @param chordRoot The root of the chord
     * @param chordQuality The quality of the chord (M/m/...)
     * @param chordBass The bass of the chord
     */
    void setToChord(IntVarArgs chordNotes, int chordRoot, vector<int> chordQuality, int chordBass);

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
};

#endif