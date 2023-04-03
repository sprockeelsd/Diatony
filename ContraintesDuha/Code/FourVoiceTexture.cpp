/**
 * @file FourVoiceTexture.cpp
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
 *      - If two chords in fundamental position are after each other and the interval between their root notes is a second, the other voices need to move in contrary motion
 *        to the bass
 * @version 1.1
 * @date 2023-02-01
 *
 */

#include "FourVoiceTexture.hpp"

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
FourVoiceTexture::FourVoiceTexture(int size, Tonality &tonality, vector<int> chordRoots, vector<vector<int>> chordQualities, vector<int> chordBass)
{
    //-------------------------------------------------------------------Initialisation--------------------------------------------------------------------
    n = size;
    tonality = tonality;
    chordRoots = chordRoots;
    chordQualities = chordQualities;
    chordBass = chordBass;

    // The domain of all notes is the set of all the notes from the tonality
    chordsVoicings = IntVarArray(*this, 4 * n, tonality.getTonalityNotes());

    // Voices cannot move melodically by more than octave skips
    bassVoiceIntervals = IntVarArray(*this, n - 1, -perfectOctave, perfectOctave);
    tenorVoiceIntervals = IntVarArray(*this, n - 1, -perfectOctave, perfectOctave);
    altoVoiceIntervals = IntVarArray(*this, n - 1, -perfectOctave, perfectOctave);
    sopranoVoiceIntervals = IntVarArray(*this, n - 1, -perfectOctave, perfectOctave);

    // The interval between two adjacent voices of a given chord is at most an octave
    // except for the interval between the bass and tenor which can be up to an octave and a fifth
    bassTenorIntervals = IntVarArray(*this, n, 0, perfectOctave + perfectFifth);
    tenorAltoIntervals = IntVarArray(*this, n, 0, perfectOctave);
    altoSopranoIntervals = IntVarArray(*this, n, 0, perfectOctave);

    //---------------------------------------------------------Linking the variables together--------------------------------------------------------------

    // Posts the constraints that the melodic intervals are the difference between 2 consecutive notes for each voice
    for (int i = 0; i < n - 1; ++i)
    {
        rel(*this, bassVoiceIntervals[i] == chordsVoicings[(i + 1) * 4] - chordsVoicings[i * 4]);
        rel(*this, tenorVoiceIntervals[i] == chordsVoicings[((i + 1) * 4) + 1] - chordsVoicings[(i * 4) + 1]);
        rel(*this, altoVoiceIntervals[i] == chordsVoicings[((i + 1) * 4) + 2] - chordsVoicings[(i * 4) + 2]);
        rel(*this, sopranoVoiceIntervals[i] == chordsVoicings[((i + 1) * 4) + 3] - chordsVoicings[(i * 4) + 3]);
    }

    // Posts the constraints that the harmonic intervals are the difference between adjacent notes of each chord
    for (int i = 0; i < n; ++i)
    {
        rel(*this, bassTenorIntervals[i] == chordsVoicings[(4 * i) + 1] - chordsVoicings[4 * i]);
        rel(*this, tenorAltoIntervals[i] == chordsVoicings[(4 * i) + 2] - chordsVoicings[(4 * i) + 1]);
        rel(*this, altoSopranoIntervals[i] == chordsVoicings[(4 * i) + 3] - chordsVoicings[(4 * i) + 2]);
    }

    // Posts the constraints on the domain of the different voices
    for (int i = 0; i < n; ++i)
    {
        IntVarArgs currentChord(chordsVoicings.slice(4 * i, 1, 4));
        rel(*this, currentChord, IRT_LQ); // bass[i] <= tenor[i] <= alto[i] <= soprano[i]

        // E2 <= bass <= C3
        rel(*this, currentChord[0], IRT_GQ, 40);
        rel(*this, currentChord[0], IRT_LQ, 60);
        // C2 <= tenor <= A3
        rel(*this, currentChord[1], IRT_GQ, 48);
        rel(*this, currentChord[1], IRT_LQ, 69);
        // G2 <= alto <= D3
        rel(*this, currentChord[2], IRT_GQ, 55);
        rel(*this, currentChord[2], IRT_LQ, 75);
        // C3 <= soprano <= A4
        rel(*this, currentChord[3], IRT_GQ, 60);
        rel(*this, currentChord[3], IRT_LQ, 84);
    }

    //---------------------------------------------------------------------Constraints---------------------------------------------------------------------

    /**********************************************************************
     *                                                                    *
     *                         Generic constraints                        *
     *                                                                    *
     **********************************************************************/

    for (int i = 0; i < n; ++i)
    {
        // Never double the seventh
        dontDoubleTheSeventh(*this, chordsVoicings.slice(4 * i, 1, 4), tonality.getScaleDegree(tonality.getKey() + majorSeventh));
    }

    for (int j = 0; j < n - 1; ++j)
    {
        // consecutive fifths/octaves/unissons are forbidden
        forbidParallelIntervals(*this, unisson, j, bassVoiceIntervals, tenorVoiceIntervals,
                                altoVoiceIntervals, sopranoVoiceIntervals);
        forbidParallelIntervals(*this, perfectFifth, j, bassVoiceIntervals, tenorVoiceIntervals,
                                altoVoiceIntervals, sopranoVoiceIntervals);
        forbidParallelIntervals(*this, perfectOctave, j, bassVoiceIntervals, tenorVoiceIntervals,
                                altoVoiceIntervals, sopranoVoiceIntervals);
    }

    /**********************************************************************
     *                                                                    *
     *                      Chord-related constraints                     *
     *                                                                    *
     **********************************************************************/

    for (int i = 0; i < n; ++i) // For each chord
    {
        IntVarArgs currentChord = chordsVoicings.slice(4 * i, 1, 4); // Current chord

        // Set the domain of the notes of that chord to possible notes from the chord
        setToChord(*this, currentChord, chordRoots[i], chordQualities[i], chordBass[i]);

        // TEMPORARY UNTIL I TALK TO KARIM
        // For perfect chords, each note should be present at least once
        // If this is a perfect chord
        if (chordQualities[i] == MAJOR_CHORD || chordQualities[i] == MINOR_CHORD || chordQualities[i] == AUGMENTED_CHORD || chordQualities[i] == DIMINISHED_CHORD)
        {
            // The fundamental is present at least once
            count(*this, currentChord, getAllGivenNote(chordRoots[i]), IRT_GQ, 1);
            // The third is present at least once
            count(*this, currentChord, getAllGivenNote(chordRoots[i] + chordQualities[i][0]), IRT_GQ, 1);
            // The fifth is present at least once
            count(*this, currentChord, getAllGivenNote(chordRoots[i] + chordQualities[i][0] + chordQualities[i][1]), IRT_GQ, 1);
        }

        // For 3 note chords, double the fundamental in priority unless it is a diminished chord then each note should only be present once -> 3 distinct values
        fundamentalStateThreeNoteChord(*this, currentChord, chordRoots[i], chordQualities[i], chordBass[i]);
    }

    /**********************************************************************
     *                                                                    *
     *                  Voice leading related constraints                 *
     *                                                                    *
     **********************************************************************/

    for (int i = 0; i < n - 1; ++i) // For each interval between the chords
    {
        // Post the rules for moving from a chord in fundamental state to another
        fundamentalStateChordToFundamentalStateChord(*this, i, bassVoiceIntervals, tenorVoiceIntervals,
                                                     altoVoiceIntervals, sopranoVoiceIntervals, chordBass, chordRoots);
    }

    //----------------------------------------------------------------------Branching----------------------------------------------------------------------

    Rnd r1(0);
    Rnd r2(1);
    branch(*this, chordsVoicings, INT_VAR_RND(r1), INT_VAL_RND(r2));

    branch(*this, sopranoVoiceIntervals, INT_VAR_RND(r1), INT_VAL_MIN());
    branch(*this, altoVoiceIntervals, INT_VAR_RND(r1), INT_VAL_MIN());
    branch(*this, tenorVoiceIntervals, INT_VAR_RND(r1), INT_VAL_MIN());
    branch(*this, bassVoiceIntervals, INT_VAR_RND(r1), INT_VAL_MIN());
}

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
    std::cout << "bass : " << bassVoiceIntervals << std::endl
              << "tenor : " << tenorVoiceIntervals << std::endl
              << "alto : " << altoVoiceIntervals << std::endl
              << "soprano : " << sopranoVoiceIntervals << std::endl;

    std::cout << chordsVoicings << std::endl;
}

/**
 * @brief Print the solution
 *
 */
void FourVoiceTexture::print(void) const
{
    // std::for_each(chordsVoicings.begin(), chordsVoicings.end(), printIntVar);
    for (int i = 0; i < chordsVoicings.size(); ++i)
    {
        if (i % 4 == 0 && i != 0)
            std::cout << std::endl;
        printNoteInLetter(chordsVoicings[i]);
    }
}

/**
 * @brief Prints the solution in an OM-ready way (parenthesis with the note values in MIDIcent). Example output : ((6000 6200 6400) (6500 6700 6900))
 *
 */
void FourVoiceTexture::printForOM(void) const
{
    std::cout << "((";
    for (int i = 0; i < chordsVoicings.size(); ++i)
    {
        if (i % 4 == 0 && i != 0)
            std::cout << ")(";
        printNoteForOM(chordsVoicings[i]);
    }
    std::cout << "))" << std::endl;
}

/**
 * @brief Search support, updates the variables
 *
 * @param s
 */
FourVoiceTexture::FourVoiceTexture(FourVoiceTexture &s) : Space(s)
{
    bassVoiceIntervals.update(*this, s.bassVoiceIntervals);
    tenorVoiceIntervals.update(*this, s.tenorVoiceIntervals);
    altoVoiceIntervals.update(*this, s.altoVoiceIntervals);
    sopranoVoiceIntervals.update(*this, s.sopranoVoiceIntervals);

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

/* IntVar FourVoiceTexture::cost(void) const
{
    return totalIntervalCost;
} */