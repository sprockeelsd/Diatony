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
 * @brief Construct a new FourVoiceTexture object
 * @param size The number of chords in the 4 voice texture to be generated
 */
FourVoiceTexture::FourVoiceTexture(int size, int key, int mode)
{
    //---------------------Initialisation---------------------
    n = size;
    key = key;
    mode = mode;

    // TODO change the domains for all variables (intervals and chords)
    sopranoVoiceIntervals = IntVarArray(*this, n - 1, -24, 24);
    altoVoiceIntervals = IntVarArray(*this, n - 1, -24, 24);
    tenorVoiceIntervals = IntVarArray(*this, n - 1, -24, 24);
    bassVoiceIntervals = IntVarArray(*this, n - 1, -24, 24);

    chordsVoicings = IntVarArray(*this, 4 * n, 60, 72);

    //------------Linking the Arrays together------------------
    // Posts the constraints that the intervals are the difference between 2 consecutive notes for each voice
    for (int i = 0; i < n - 1; ++i)
    {
        sopranoVoiceIntervals[i] = expr(*this, chordsVoicings[(i + 1) * 4] - chordsVoicings[i * 4]);
        altoVoiceIntervals[i] = expr(*this, chordsVoicings[((i + 1) * 4) + 1] - chordsVoicings[(i * 4) + 1]);
        tenorVoiceIntervals[i] = expr(*this, chordsVoicings[((i + 1) * 4) + 2] - chordsVoicings[(i * 4) + 2]);
        bassVoiceIntervals[i] = expr(*this, chordsVoicings[((i + 1) * 4) + 3] - chordsVoicings[(i * 4) + 3]);
    }

    //----------------------Constraints------------------------

    setToTonality(chordsVoicings);

    //---------------------Branching---------------------------

    // TODO réfléchir au branching

    branch(*this, chordsVoicings, INT_VAR_SIZE_MIN(), INT_VAL_MIN());

    branch(*this, sopranoVoiceIntervals, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, altoVoiceIntervals, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, tenorVoiceIntervals, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, bassVoiceIntervals, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
}

/**
 * @brief Posts the constraint that the domain of vars is equal to domain
 *
 * @param space An instance of a 4 voice texture problem
 * @param vars The array of variables that we want to apply the constraint to
 * @param domain The set of notes representing the tonality
 */
void FourVoiceTexture::setToTonality(IntVarArray vars)
{
    IntSet domain(getAllNotesFromTonality(key, mode));
    dom(*this, vars, domain);
}

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

void FourVoiceTexture::constrain(const Space &_b)
{
    std::cout << "TODO" << std::endl;
}