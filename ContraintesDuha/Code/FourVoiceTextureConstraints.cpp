/**
 * @file FourVoiceTextureConstraints.cpp
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This class contains all the musical constraints. They are separated into 3 types : generic constraints, chord-related constraints and voice leading related constraints.
 * @version 1.1
 * @date 2023-02-01
 *
 */

#include "FourVoiceTextureConstraints.h"

/**********************************************************************
 *                                                                    *
 *                         Generic constraints                        *
 *                                                                    *
 **********************************************************************/

/**
 * @brief Posts the constraint that the seventh of the scale can never occur twice in a chord
 *
 * @param home The space of the problem
 * @param chordNotes The variables representing a given chord
 * @param sevenths The set of notes that are the seventh of the key
 */
void dontDoubleTheSeventh(Home home, IntVarArgs chordNotes, IntSet sevenths)
{
    IntVar nOfSeventh(home, 0, 4);                         // Variable to count the number of sevenths
    count(home, chordNotes, sevenths, IRT_EQ, nOfSeventh); // nOfSeventh == nb of seventh in the chord
    rel(home, nOfSeventh, IRT_LQ, 1);                      // nOfSeventh <= 1
}

/**
 * @brief Ensures that if there is a tritone in the chord it resolves properly.
 * That is, the seventh should resolve upwards in the next chord and the fourth should resolve downwards.
 *
 * @param home the space of the problem
 * @param chordNotes the variables for the notes of the current chord
 * @param nOfSeventh the number of seventh present in the chord (should be <=1)
 * @param chordPosition the position of the chord in the big array
 * @param chordQuality the quality of the given chord (M/m/7/...)
 */
void tritoneResolution(Home home, IntVarArgs chordNotes, IntVar nOfSeventh, int chordPosition, vector<int> chordQuality)
{
    if (chordQuality == DOMINANT_SEVENTH_CHORD || chordQuality == DIMINISHED_CHORD) // There is a tritone in the chord
    {
        // post for each element of the array : if the variable belongs to the set of seventh, the next is +1 and same for fourth
        for (int j = 0; j < 4; ++j)
        {
            BoolVar c(home, 0, 1);
            Reify check(c, RM_PMI); // x = y -> b = 1 reification
        }
    }
}

/**********************************************************************
 *                                                                    *
 *                      Chord-related constraints                     *
 *                                                                    *
 **********************************************************************/

/**
 * @brief Posts the constraint that the different voices of the chord have a value that is part of the chord
 *
 * @param home The space of the problem
 * @param chordNotes the variables representing the notes of the chord
 * @param chordRoot The root of the chord
 * @param chordQuality The quality of the chord (M/m/...)
 * @param chordBass The bass of the chord
 */
void setToChord(Home home, IntVarArgs chordNotes, int chordRoot, vector<int> chordQuality, int chordBass)
{
    IntSet chordNotesValues(getAllNotesFromChord(chordRoot, chordQuality)); // Get all notes of the chord
    dom(home, chordNotes, chordNotesValues);
    dom(home, chordNotes[0], getAllGivenNote(chordBass)); // Special treatment for the bass since it is already known
}

void fundamentalStateThreeNoteChord(IntVarArgs chordNotes, int chordPosition)
{
}

/**********************************************************************
 *                                                                    *
 *                  Voice leading related constraints                 *
 *                                                                    *
 **********************************************************************/
