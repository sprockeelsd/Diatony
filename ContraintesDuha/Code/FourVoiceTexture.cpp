/**
 * @file FourVoiceTexture.cpp
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This class creates a constraint problem to generate a 4 voice texture based on chord names and inversion.
 * The variables are the following:
 * - An array for each voice containing the intervals between them. They are of size n-1 where n is the number of chords.
 * - An array for the chords. It is of size 4*n, and the first 4 variables represent one chord with the voices in ascending order (bass -> tenor -> alto -> soprano).
 * @version 1.0
 * @date 2023-01-25
 *
 */

#include "FourVoiceTexture.h"

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
FourVoiceTexture::FourVoiceTexture(int size, int key, vector<int> mode, vector<int> chordRoots, vector<vector<int>> chordQualities, vector<int> chordBass)
{
    //-------------------------------------------------------------------Initialisation--------------------------------------------------------------------
    n = size;
    key = key;
    mode = mode;
    chordRoots = chordRoots;
    chordQualities = chordQualities;
    chordBass = chordBass;

    IntSet tonality(getAllNotesFromTonality(key, mode)); // Set of all the notes from a tonality

    chordsVoicings = IntVarArray(*this, 4 * n, tonality);

    // std::cout << "chordsVoicings : " << chordsVoicings << std::endl;

    // TODO change the domains for the intervals
    sopranoVoiceIntervals = IntVarArray(*this, n - 1, -24, 24);
    altoVoiceIntervals = IntVarArray(*this, n - 1, -24, 24);
    tenorVoiceIntervals = IntVarArray(*this, n - 1, -24, 24);
    bassVoiceIntervals = IntVarArray(*this, n - 1, -24, 24);

    // std::cout << "sopranoVoceIntervals : " << sopranoVoiceIntervals << std::endl;

    //----------------------------------------------------------Linking the Arrays together---------------------------------------------------------------
    // Posts the constraints that the intervals are the difference between 2 consecutive notes for each voice

    // /!\ This causes inconsistencies with the domain of soprano etc
    for (int i = 0; i < n - 1; ++i)
    {
        rel(*this, sopranoVoiceIntervals[i] == chordsVoicings[(i + 1) * 4] - chordsVoicings[i * 4]);
        rel(*this, bassVoiceIntervals[i] == chordsVoicings[((i + 1) * 4) + 3] - chordsVoicings[(i * 4) + 3]);
        rel(*this, altoVoiceIntervals[i] == chordsVoicings[((i + 1) * 4) + 1] - chordsVoicings[(i * 4) + 1]);
        rel(*this, tenorVoiceIntervals[i] == chordsVoicings[((i + 1) * 4) + 2] - chordsVoicings[(i * 4) + 2]);
    }

    //---------------------------------------------------------------------Constraints---------------------------------------------------------------------

    // Set the notes of each chord to belong to the given chord

    //----------------------------------------------------------------------Branching----------------------------------------------------------------------

    // TODO réfléchir au branching

    branch(*this, chordsVoicings, INT_VAR_SIZE_MIN(), INT_VAL_MIN());

    /*     branch(*this, sopranoVoiceIntervals, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
        branch(*this, altoVoiceIntervals, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
        branch(*this, tenorVoiceIntervals, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
        branch(*this, bassVoiceIntervals, INT_VAR_SIZE_MIN(), INT_VAL_MIN()); */
}

/**********************************************************************
 *                                                                    *
 *                          Constraint functions                      *
 *                                                                    *
 **********************************************************************/

/**********************************************************************
 *                                                                    *
 *                          Support functions                         *
 *                                                                    *
 **********************************************************************/

/**
 * @brief Print all the variables, used for development
 *
 */
void FourVoiceTexture::printDevelop(void) const
{
    std::cout << "soprano : " << sopranoVoiceIntervals << std::endl
              << "alto : " << altoVoiceIntervals << std::endl
              << "tenor : " << tenorVoiceIntervals << std::endl
              << "bass : " << bassVoiceIntervals << std::endl;

    std::cout << chordsVoicings << std::endl;
}

/**
 * @brief Print the solution
 *
 */
void FourVoiceTexture::print(void) const
{
    std::cout << "TODO" << std::endl;
}

/**
 * @brief Prints the solution in an OM-ready way (parenthesis with the note values in MIDIcent). Example output : ((6000 6200 6400) (6500 6700 6900))
 *
 */
void FourVoiceTexture::printForOM(void) const
{
    std::cout << "TODO" << std::endl;
}

/**
 * @brief Search support, updates the variables
 *
 * @param s
 */
FourVoiceTexture::FourVoiceTexture(FourVoiceTexture &s) : Space(s)
{
    sopranoVoiceIntervals.update(*this, s.sopranoVoiceIntervals);
    altoVoiceIntervals.update(*this, s.altoVoiceIntervals);
    tenorVoiceIntervals.update(*this, s.tenorVoiceIntervals);
    bassVoiceIntervals.update(*this, s.bassVoiceIntervals);

    chordsVoicings.update(*this, s.chordsVoicings);
}

/**
 * @brief This method is called when a Branch and Bound solver is used everytime a solution is found by the solver.
 *
 * @param _b The solution found by the solver
 */
void FourVoiceTexture::constrain(const Space &_b)
{
    std::cout << "TODO" << std::endl;
}