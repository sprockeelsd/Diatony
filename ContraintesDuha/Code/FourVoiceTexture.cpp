/**
 * @file FourVoiceTexture.cpp
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This class creates a constraint problem to generate a 4 voice texture based on chord names, types and bass.
 * The variables are the following:
 * - An array for each voice containing the intervals between them. They are of size n-1 where n is the number of chords.
 * - An array for the chords. It is of size 4*n, and the first 4 variables represent one chord with the voices in ascending order (bass -> tenor -> alto -> soprano).
 *   The arrays are linked together
 * The currently supported constraints are the following :
 *      - The notes are in the right tonality
 *      - The notes are in the given chord
 *      - The seventh degree of a scale can never be doubled
 * @version 1.0
 * @date 2023-01-25
 *
 */

/**
 *
 * @todo Add an IntSet for each note of the scale in the attributes of the class so we don't have to compute it everytime we need it (maybe create a specific object for it?)
 * @todo Keep working on the tritone resolution constraint
 * @todo change the constraint linking the interval arrays to the chord array to an element-like constraint?
 * @todo Put the code for the cst that we can never double the seventh into its separate function
 * @todo Write a constraint that the last chord can't contain a tritone (maybe in the specs though)
 * @todo Check with Karim if the constraint for all notes must be present at least once in perfect chords is valid or not
 * @todo Move the tritone resolution constraint to a loop that iterates over intervals since the constraint has to access both the current and future chords
 * @todo Think about the branching strategy once we have enough constraints that it makes sense
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

    // The domain of all notes is the set of all the notes from the (key, mode) tonality
    chordsVoicings = IntVarArray(*this, 4 * n, getAllNotesFromTonality(key, mode));

    // TODO change the domains for the intervals -> ASK KARIM
    bassVoiceIntervals = IntVarArray(*this, n - 1, -12, 12);
    tenorVoiceIntervals = IntVarArray(*this, n - 1, -12, 12);
    altoVoiceIntervals = IntVarArray(*this, n - 1, -12, 12);
    sopranoVoiceIntervals = IntVarArray(*this, n - 1, -12, 12);

    //---------------------------------------------------------Linking the variables together--------------------------------------------------------------

    // Posts the constraints that the intervals are the difference between 2 consecutive notes for each voice
    for (int i = 0; i < n - 1; ++i)
    {
        rel(*this, bassVoiceIntervals[i] == chordsVoicings[(i + 1) * 4] - chordsVoicings[i * 4]);
        rel(*this, tenorVoiceIntervals[i] == chordsVoicings[((i + 1) * 4) + 1] - chordsVoicings[(i * 4) + 1]);
        rel(*this, altoVoiceIntervals[i] == chordsVoicings[((i + 1) * 4) + 2] - chordsVoicings[(i * 4) + 2]);
        rel(*this, sopranoVoiceIntervals[i] == chordsVoicings[((i + 1) * 4) + 3] - chordsVoicings[(i * 4) + 3]);
    }

    // Posts the constraint that bass[i] <= tenor[i] <= alto[i] <= soprano[i]
    for (int i = 0; i < n; ++i)
        rel(*this, chordsVoicings.slice(4 * i, 1, 4), IRT_LQ);

    //---------------------------------------------------------------------Constraints---------------------------------------------------------------------

    for (int i = 0; i < n; ++i) // For each chord
    {
        // Current chord
        IntVarArgs currentChord = chordsVoicings.slice(4 * i, 1, 4);

        // Set the domain of the notes of that chord to possible notes from the chord
        setToChord(currentChord, chordRoots[i], chordQualities[i], chordBass[i]);

        // Never double the seventh
        IntVar nOfSeventh(*this, 0, 4);                                            // Variable to count the number of sevenths
        count(*this, currentChord, getAllGivenNote(key + 11), IRT_EQ, nOfSeventh); // nOfSeventh == nb of seventh in the chord
        rel(*this, nOfSeventh, IRT_LQ, 1);                                         // nOfSeventh <= 1

        // For perfect chords, each note should be present at least once ASK KARIM
        if (chordQualities[i] == MAJOR_CHORD || chordQualities[i] == MINOR_CHORD || chordQualities[i] == AUGMENTED_CHORD || chordQualities[i] == DIMINISHED_CHORD) // If this is a perfect chord
        {
            count(*this, currentChord, getAllGivenNote(chordRoots[i]), IRT_GQ, 1);                                               // The fundamental is present at least once
            count(*this, currentChord, getAllGivenNote(chordRoots[i] + chordQualities[i][0]), IRT_GQ, 1);                        // The third is present at least once
            count(*this, currentChord, getAllGivenNote(chordRoots[i] + chordQualities[i][0] + chordQualities[i][1]), IRT_GQ, 1); // The fifth is present at least once
        }

        // For 3 note chords, double the fundamental in priority
        //fundamentalStateThreeNoteChord(currentChord, i);

        // If there is a tritone in the chord, the 7th of the scale should resolve upwards and the 4th of the scale should resolve downwards by a half step
        // tritoneResolution(currentChord, containsSeventh, i);
    }

    //----------------------------------------------------------------------Branching----------------------------------------------------------------------

    Rnd r1(0);
    Rnd r2(1);
    branch(*this, chordsVoicings, INT_VAR_RND(r1), INT_VAL_RND(r2));

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

/**
 * @brief Posts the constraint that the different voices of the chord have a value that is part of the chord
 *
 * @param chordNotes the variables representing the notes of the chord
 * @param chordRoot The root of the chord
 * @param chordQuality The quality of the chord (M/m/...)
 * @param chordBass The bass of the chord
 */
void FourVoiceTexture::setToChord(IntVarArgs chordNotes, int chordRoot, vector<int> chordQuality, int chordBass)
{
    IntSet chordNotesValues(getAllNotesFromChord(chordRoot, chordQuality)); // Get all notes of the chord
    dom(*this, chordNotes, chordNotesValues);
    dom(*this, chordNotes[0], getAllGivenNote(chordBass)); // Special treatment for the bass since it is already known
}

/**
 * @brief Ensures that if there is a tritone in the chord it resolves properly.
 * That is, the seventh should resolve upwards in the next chord and the fourth should resolve downwards.
 *
 * @param chordNotes the variables for the notes of the current chord
 * @param nOfSeventh the number of seventh present in the chord (should be <=1)
 * @param chordPosition the position of the chord in the big array
 */
void FourVoiceTexture::tritoneResolution(IntVarArgs chordNotes, IntVar nOfSeventh, int chordPosition)
{
    // Autre idée : if chordQuality[i] == dominant seventh or diminished
    // Then post for each element of the array : if the variable belongs to seventh -> the next is +1 and same for fourth

    BoolVar containsSeventh(*this, 0, 1);  // Variable to tell if the chord contains a seventh
    Reify cS(containsSeventh, RM_PMI);     // half reification
    rel(*this, nOfSeventh, IRT_EQ, 1, cS); // nOfSeventh == 1 => containsSeventh == 1 && containsSeventh == 0 => nOfSeventh != 1

    IntVar nOfFourth(*this, 0, 4);                                         // Variables to count the number of fourths
    count(*this, chordNotes, getAllGivenNote(key + 5), IRT_EQ, nOfFourth); // Count the fourths

    BoolVar containsFourth(*this, 0, 1);  // Variable to tell if the chord contains a fourth
    Reify cF(containsFourth, RM_PMI);     // half reification
    rel(*this, nOfFourth, IRT_GQ, 1, cF); // nOfFourth >= 1 => containsFourth == 1 && containsFourt == 0 => nOfFourth == 0

    BoolVar containsTritone(*this, 0, 1);                                  // Variable to tell if the chord contains a tritone
    rel(*this, containsSeventh, BOT_AND, containsFourth, containsTritone); // containsTritone = containsSeventh AND containsFourth

    // if containsTritone then we must know what voices have themso we can resolve the voices appropriately
    // TODO continuer
}

void FourVoiceTexture::fundamentalStateThreeNoteChord(IntVarArgs chordNotes, int chordPosition)
{
    if (chordRoots[chordPosition] % 12 + 12 == chordBass[chordPosition] % 12 + 12) // If the chord is in fundamental state
    {
    }
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
    for (int i = 0; i < chordsVoicings.size(); ++i)
    {
        if (i % 4 == 0 && i != 0)
            std::cout << std::endl;
        printNoteForOM(chordsVoicings[i]);
    }
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