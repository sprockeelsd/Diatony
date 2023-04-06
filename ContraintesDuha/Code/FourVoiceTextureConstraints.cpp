/**
 * @file FourVoiceTextureConstraints.cpp
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This class contains all the musical constraints. They are separated into 3 types : generic constraints, chord-related constraints and voice leading related constraints.
 * @version 1.1
 * @date 2023-02-01
 *
 */

#include "FourVoiceTextureConstraints.hpp"

/**********************************************************************
 *                                                                    *
 *                         Generic constraints                        *
 *                                                                    *
 **********************************************************************/

/**
 * @brief Posts the constraint that the seventh of the scale can never occur twice in a chord.
 *
 * @param home The space of the problem
 * @param chordNotes The variables representing a given chord
 * @param sevenths The set of notes that are the seventh of the key
 */
void dontDoubleTheSeventh(Home home, IntVarArgs chordNotes, IntSet sevenths)
{
    IntVar nOfSeventh(home, 0, 1);                         // Variable to count the number of sevenths
    count(home, chordNotes, sevenths, IRT_EQ, nOfSeventh); // nOfSeventh == nb of seventh in the chord
    // rel(home, nOfSeventh, IRT_LQ, 1);                      // nOfSeventh <= 1 not necessary because of the domain
}

/**
 * @brief Posts the constraint that there can not be parallel intervals of value interval. Practically, it works by
 * checking for each chord if there is an interval of value interval between 2 adjacent voices. if there is, then this
 * interval cannot occur in the next chord between 2 adjacent voices unless both voices take the same value.
 *
 * @todo Develop this to include arguments + maybe explain it better. Also finish the function. Also figure out a way to make it work for octaves without adding +12 because it breaks the rest
 */
void forbidParallelIntervals(Home home, int forbiddenInterval, int currentPosition, IntVarArray bassMelodicIntervals,
                             IntVarArray tenorMelodicIntervals, IntVarArray altoMelodicIntervals,
                             IntVarArray sopranoMelodicIntervals, IntVarArray bassTenorIntervals,
                             IntVarArray tenorAltoIntervals, IntVarArray altoSopranoIntervals, IntVarArray chordVoicings){
    /**Bass and tenor**/
    BoolVar bassTenorIntervalForbidden(home, 0,1);
    rel(home, bassTenorIntervalForbidden, IRT_EQ, expr(home, bassTenorIntervals[currentPosition] % 12 + 12 == forbiddenInterval));

    // If the interval between the bass and the tenor is the one we don't want to occur in the next chord,
    // it cannot occur between adjacent voices in the next chord unless it is in the same voices with the same values
    rel(home, bassTenorIntervalForbidden, BOT_IMP, expr(home, tenorAltoIntervals[currentPosition + 1] % 12 + 12
    != forbiddenInterval), true);// cannot occur between tenor and alto
    rel(home, bassTenorIntervalForbidden, BOT_IMP, expr(home, altoSopranoIntervals[currentPosition + 1]% 12 + 12
    != forbiddenInterval), true);// cannot occur between alto and soprano

    BoolVar bassAndTenorAreDifferentInBothChords(home, 0,1);
    rel(home, bassAndTenorAreDifferentInBothChords != expr(home, bassTenorIntervals[currentPosition] == bassTenorIntervals[currentPosition+1] &&
    chordVoicings[currentPosition*4] == chordVoicings[(currentPosition+1)*4]));// The bass and tenor do not play the same notes for both chords
    rel(home, bassAndTenorAreDifferentInBothChords, BOT_IMP, expr(home, bassTenorIntervals[currentPosition+1] %12 + 12 != forbiddenInterval), true);

    /**Tenor and alto**/
    BoolVar tenorAltoIntervalForbidden(home, 0,1);
    rel(home, tenorAltoIntervalForbidden, IRT_EQ, expr(home, tenorAltoIntervals[currentPosition] % 12 + 12 == forbiddenInterval));

    rel(home, tenorAltoIntervalForbidden, BOT_IMP, expr(home, bassTenorIntervals[currentPosition+1] % 12 + 12
    != forbiddenInterval), true); // cannot occur between bass and tenor
    rel(home, tenorAltoIntervalForbidden, BOT_IMP, expr(home, altoSopranoIntervals[currentPosition+1] % 12 + 12
    != forbiddenInterval), true); // cannot occur between alto and soprano

    BoolVar tenorAndAltoAreDifferentInBothChords(home, 0,1);
    rel(home, tenorAndAltoAreDifferentInBothChords != expr(home, tenorAltoIntervals[currentPosition] == tenorAltoIntervals[currentPosition+1] &&
    chordVoicings[currentPosition*4+1] == chordVoicings[(currentPosition+1)*4+1]));
    rel(home, tenorAndAltoAreDifferentInBothChords, BOT_IMP, expr(home, tenorAltoIntervals[currentPosition+1] %12 + 12 != forbiddenInterval), true);
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

/**
 * @brief This function posts a variety of constraints on 3 note chords. These constraints include :
 * - The doubling of the bass (should be priority-based -> TODO)
 * - Diminished chords should be 3 voices only -> 2 voices have to be the same
 *
 * @todo Change the constraint for chords in fundamental form to a priority-based constraint
 *
 * @param home The space of the problem
 * @param chordNotes the variables representing the notes of the chord
 * @param chordRoot The root of the chord
 * @param chordQuality The quality of the chord (M/m/...)
 * @param chordBass The bass of the chord
 * @param doublingCost The cost variable for the doubling
 */
void fundamentalStateThreeNoteChord(Home home, IntVarArgs chordNotes, int chordRoot, vector<int> chordQuality, int chordBass)
{
    if (chordQuality == MAJOR_CHORD || chordQuality == MINOR_CHORD)
    {
        if (chordRoot % 12 + 12 == chordBass % 12 + 12) // It is in fundamental position
        {
            // doubler la basse
            count(home, chordNotes, getAllGivenNote(chordRoot), IRT_EQ, 2);
        }
    }
    else if (chordQuality == DIMINISHED_CHORD)
    {
        nvalues(home, chordNotes, IRT_EQ, 3); // The chordNotes array can only have 3 different values
    }
}

/**********************************************************************
 *                                                                    *
 *                  Voice leading related constraints                 *
 *                                                                    *
 **********************************************************************/

/**
 * @brief Post the constraints for moving from a chord in fundamental state to another. For now, it only posts a constraint if the interval is a second.
 * The constraint posted is the following : If the interval between the roots of the 2 chords is a second, the other voices have to move in contrary movement to the bass.
 * @param home The space of the problem
 * @param currentPosition The current chord which corresponds to the index in the interval arrays
 * @param bassIntervals The variable array for the bass
 * @param tenorIntervals The variable array for the tenor
 * @param altoIntervals The variable array for the alto
 * @param sopranoIntervals The variable array for the soprano
 * @param chordBass The array of bass given as input
 * @param chordRoots The array of roots given as input
 */
void fundamentalStateChordToFundamentalStateChord(Home home, int currentPosition, IntVarArray bassIntervals, IntVarArray tenorIntervals, IntVarArray altoIntervals,
                                                  IntVarArray sopranoIntervals, vector<int> chordBass, vector<int> chordRoots)
{
    // If both chords are in fundamental position
    if (chordBass[currentPosition] % 12 + 12 == chordRoots[currentPosition] % 12 + 12 && chordBass[currentPosition + 1] % 12 + 12 == chordRoots[currentPosition + 1] % 12 + 12)
    {
        int diff = (chordRoots[currentPosition + 1] % perfectOctave + 12) - (chordRoots[currentPosition] % 12 + 12); // The interval between the 2 roots

        if (diff == majorSecond || diff == -majorSecond || diff == minorSecond || diff == -minorSecond || diff == majorSeventh || diff == -majorSeventh ||
            diff == minorSeventh || diff == -minorSeventh) // If the interval between the roots of the chords is a second
        {
            // Other voices need to move in contrary motion to the bass

            // bassIntervals[i] <0 => tenorIntervals[i] > 0
            rel(home, expr(home, bassIntervals[currentPosition] < 0), BOT_IMP, expr(home, tenorIntervals[currentPosition] > 0), 1);
            // bassIntervals[i] >0 => tenorIntervals[i] < 0
            rel(home, expr(home, bassIntervals[currentPosition] > 0), BOT_IMP, expr(home, tenorIntervals[currentPosition] < 0), 1);

            // bassIntervals[i] <0 => altoIntervals[i] > 0
            rel(home, expr(home, bassIntervals[currentPosition] < 0), BOT_IMP, expr(home, altoIntervals[currentPosition] > 0), 1);
            // bassIntervals[i] >0 => altoIntervals[i] < 0
            rel(home, expr(home, bassIntervals[currentPosition] > 0), BOT_IMP, expr(home, altoIntervals[currentPosition] < 0), 1);

            // bassIntervals[i] <0 => sopranoIntervals[i] > 0
            rel(home, expr(home, bassIntervals[currentPosition] < 0), BOT_IMP, expr(home, sopranoIntervals[currentPosition] > 0), 1);
            // bassIntervals[i] >0 => sopranoIntervals[i] < 0
            rel(home, expr(home, bassIntervals[currentPosition] > 0), BOT_IMP, expr(home, sopranoIntervals[currentPosition] < 0), 1);
        }
    }
}
