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
#include <gecode/search.hh>
#include <gecode/minimodel.hh>

#include <iostream>
#include <vector>
#include <string>

using namespace Gecode;
using namespace std;

//Global Variables

class FourVoiceTexture : public Space
{
protected:
    // The number of chord for which to generate 4 voice texture
    int n;

    // Variable arrays for the melodic intervals of the different voices
    // Maybe change into Argument variables? Depending on whether we need to branch on them
    IntVarArray sopranoVoiceIntervals;
    IntVarArray altoVoiceIntervals;
    IntVarArray tenorVoiceIntervals;
    IntVarArray bassVoiceIntervals;

    // Array containing the chords
    // A 4*n array where each chord is represented with their voices in increasing order. Example : [bass0, tenor0, alto0, soprano0, bass1, tenor1,...]
    IntVarArray chordsVoicings;

public:
    /**
     * @brief Construct a new Four Voice Texture object
     *
     * @param size The number of chords in the 4 voice texture to be generated
     */
    FourVoiceTexture(int size);

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