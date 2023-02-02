/**
 * @file Utilities.h
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This file contains useful constants and functions
 * @version 1.1
 * @date 2023-02-01
 *
 */
#ifndef UTILITIES
#define UTILITIES

#include <gecode/int.hh>
#include <gecode/minimodel.hh>

#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace Gecode;

//--------------------Useful constants---------------------

const vector<std::string> noteNames = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"};

// Types of chords represented with the intervals going up to an octave so it can be nicely looped
const vector<int> MAJOR_CHORD = {4, 3, 5};
const vector<int> MINOR_CHORD = {3, 4, 5};
const vector<int> DIMINISHED_CHORD = {3, 3, 6};
const vector<int> AUGMENTED_CHORD = {4, 4, 4};
const vector<int> DOMINANT_SEVENTH_CHORD = {4, 3, 3, 2};
const vector<int> MAJOR_SEVENTH_CHORD = {4, 3, 4, 1};
const vector<int> MINOR_SEVENTH_CHORD = {3, 4, 3, 2};

// Notes
const int Bsharp = 12;
const int C = 12;
const int Csharp = 13;
const int Dflat = 13;
const int D = 14;
const int Dsharp = 15;
const int Eflat = 15;
const int E = 16;
const int Fflat = 16;
const int Esharp = 17;
const int F = 17;
const int Fsharp = 18;
const int Gflat = 18;
const int G = 19;
const int Gsharp = 20;
const int Aflat = 20;
const int A = 21;
const int Asharp = 22;
const int Bflat = 22;
const int B = 23;
const int Cflat = 23;

// Intervals
const int minorSecond = 1;
const int majorSecond = 2;
const int minorThird = 3;
const int majorThird = 4;
const int perfectFourth = 5;
const int tritone = 6;
const int perfectFifth = 7;
const int minorSixth = 8;
const int majorSixth = 9;
const int minorSeventh = 10;
const int majorSeventh = 11;
const int perfectOctave = 12;

// Scales
const vector<int> MAJOR_SCALE = {2, 2, 1, 2, 2, 2, 1};
const vector<int> MINOR_SCALE = {2, 1, 2, 2, 1, 2, 1, 1}; // To allow the major seventh necessary for the dominant chord

// Cost values for priorities on operations
const int NO_COST = 0;
const int SMALL_COST = 1;
const int FORBIDDEN = 100;

/**
 * @brief For a given tonality (root + mode), get all the possible notes
 *
 * @param root the root of the tonality (in [12,23])
 * @param scale the set of tones and semitones that define the scale
 * @return vector<int> all the possible notes from that tonality
 */
IntSet getAllNotesFromTonality(int root, vector<int> scale);

/**
 * @brief For a given chord (root + mode), get all the possible notes
 *
 * @param root the root of the chord
 * @param quality the set of tones and semitones that define the chord
 * @return vector<int> all the possible notes from that chord
 */
IntSet getAllNotesFromChord(int root, vector<int> quality);

/**
 * @brief Get all values for a given note
 * 
 * @param note a note
 * @return const vector<int> a vector containing all the given notes
 */
IntSet getAllGivenNote(int note);

/**
 * @brief Print a dynamic array of integers
 *
 * @param v a dynamic array of integers
 */
void printIntVector(vector<int> vector);

/**
 * @brief Prints A note with its name (e.g. 60 = C)
 * 
 * @param var an integer variable
 */
void printNoteInLetter(IntVar var);

/**
 * @brief Prints a node in MIDIcent value
 * 
 * @param var an integer variable
 */
void printNoteForOM(IntVar var);

#endif