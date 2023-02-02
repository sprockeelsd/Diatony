/**
 * @file FourVoiceTextureConstraints.h
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This class contains all the musical constraints. They are separated into 3 types : generic constraints, chord-related constraints and voice leading related constraints.
 * @version 1.1
 * @date 2023-02-01
 *
 */
#ifndef FOURVOICETEXTURECONSTRAINTS
#define FOURVOICETEXTURECONSTRAINTS

#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>

#include "Utilities.h"

#include <iostream>
#include <vector>
#include <string>

using namespace Gecode;
using namespace std;

/**
 * @brief Posts the constraint that the different voices of the chord have a value that is part of the chord
 *
 *ƒ @param home The space of the problem
 * @param chordNotes the variables representing the notes of the chord
 * @param chordRoot The root of the chord
 * @param chordQuality The quality of the chord (M/m/...)
 * @param chordBass The bass of the chord
 */
void setToChord(Home home, IntVarArgs chordNotes, int chordRoot, vector<int> chordQuality, int chordBass);

/**
 * @brief Posts the constraint that the seventh of the scale can never occur twice in a chord
 *
 * @param home The space of the problem
 * @param chordNotes The variables representing a given chord
 * @param sevenths The set of notes that are the seventh of the key
 */
void dontDoubleTheSeventh(Home home, IntVarArgs chordNotes, IntSet sevenths);

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
void tritoneResolution(Home home, IntVarArray chords, int chordPosition, vector<int> chordQuality);

/**
 * @brief This function posts a variety of constraints on 3 note chords. These constraints include :
 * - The doubling of the bass (should be priority-based -> TODO)
 * - Diminished chords should be 3 voices only -> 2 voices have to be the same
 *
 * @param home The space of the problem
 * @param chordNotes the variables representing the notes of the chord
 * @param chordRoot The root of the chord
 * @param chordQuality The quality of the chord (M/m/...)
 * @param chordBass The bass of the chord
 * @param doublingCost The cost variable for the doubling
 */
void fundamentalStateThreeNoteChord(Home home, IntVarArgs chordNotes, int chordRoot, vector<int> chordQuality, int chordBass, IntVar doublingCost);

#endif