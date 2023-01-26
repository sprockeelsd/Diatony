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

using namespace Gecode;
using namespace std;

/**
 * @brief Construct a new FourVoiceTexture object
 * @param size The number of chords in the 4 voice texture to be generated
 */
FourVoiceTexture::FourVoiceTexture(int size)
{
    //---------------------Initialisation---------------------
    n = size;

    // TODO change the domains for all variables (intervals and chords)
    sopranoVoiceIntervals = IntVarArray(*this, n - 1, -3, 3);
    altoVoiceIntervals = IntVarArray(*this, n - 1, -3, 3);
    tenorVoiceIntervals = IntVarArray(*this, n - 1, -3, 3);
    bassVoiceIntervals = IntVarArray(*this, n - 1, -3, 3);

    chordsVoicings = IntVarArray(*this, 4 * n, 0, 4 * n);

    //----------------------Constraints------------------------

    //---------------------Branching---------------------------

    // Branch on each of the chords
    distinct(*this, chordsVoicings);
    // TODO réfléchir au branching

    branch(*this, chordsVoicings, INT_VAR_SIZE_MIN(), INT_VAL_MIN());

    branch(*this, sopranoVoiceIntervals, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, altoVoiceIntervals, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, tenorVoiceIntervals, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, bassVoiceIntervals, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
}

void FourVoiceTexture::print(void) const
{
    std::cout << "soprano : " << sopranoVoiceIntervals << std::endl
              << "alto : " << altoVoiceIntervals << std::endl
              << "tenor : " << tenorVoiceIntervals << std::endl
              << "bass : " << bassVoiceIntervals << std::endl;

    std::cout << chordsVoicings << std::endl;
}

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